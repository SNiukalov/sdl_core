/*
 Copyright (c) 2019, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdl_rpc_plugin/commands/mobile/create_window_request.h"

#include <algorithm>

#include "application_manager/application_impl.h"
#include "application_manager/application_state.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
using app_mngr::ApplicationSharedPtr;

namespace commands {

CreateWindowRequest::CreateWindowRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

CreateWindowRequest::~CreateWindowRequest() {}

bool CreateWindowRequest::WindowIdExists(
    ApplicationSharedPtr app, const app_mngr::WindowID window_id) const {
  const WindowIds window_ids = app->GetWindowIds();
  return helpers::in_range(window_ids, window_id);
}

bool CreateWindowRequest::CheckWindowName(
    app_mngr::ApplicationSharedPtr app,
    const app_mngr::WindowID window_id,
    const std::string& window_name) const {
  if (mobile_apis::PredefinedWindows::PRIMARY_WIDGET == window_id) {
    LOG4CXX_DEBUG(logger_,
                  "Window name check is ignored for the primary widgets");
    return true;
  }

  const bool names_are_equal = window_name == app->name().c_str();
  if (names_are_equal &&
      mobile_apis::PredefinedWindows::DEFAULT_WINDOW != window_id) {
    LOG4CXX_ERROR(logger_,
                  "Regular widget can't have the same name as application: "
                      << window_name);
    return false;
  }

  const WindowNames window_names = app->GetWindowNames();
  return !helpers::in_range(window_names, window_name);
}

void CreateWindowRequest::ApplyWindowInitialState(
    ApplicationSharedPtr app) const {
  const mobile_apis::WindowType::eType window_type =
      static_cast<mobile_apis::WindowType::eType>(
          (*message_)[strings::msg_params][strings::window_type].asInt());

  // State should be initialized with INVALID_ENUM value to let state controller
  // trigger OnHmiStatus notifiation sending
  HmiStatePtr initial_state = application_manager_.CreateRegularState(
      app,
      window_type,
      mobile_apis::HMILevel::INVALID_ENUM,
      mobile_apis::AudioStreamingState::INVALID_ENUM,
      mobile_apis::VideoStreamingState::INVALID_ENUM,
      mobile_api::SystemContext::INVALID_ENUM);

  const WindowID window_id =
      (*message_)[strings::msg_params][strings::window_id].asUInt();
  const std::string window_name =
      (*message_)[strings::msg_params][strings::window_name].asString();

  app->SetInitialState(window_id, window_name, initial_state);

  // Default HMI level for all windows except the main one is always NONE
  application_manager_.state_controller().OnAppWindowAdded(
      app, window_id, window_type, mobile_apis::HMILevel::HMI_NONE);
}

app_mngr::WindowID CreateWindowRequest::window_id() const {
  return mobile_apis::PredefinedWindows::DEFAULT_WINDOW;
}

void CreateWindowRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  const auto application = application_manager_.application(connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const auto window_id =
      (*message_)[strings::msg_params][strings::window_id].asUInt();
  if (WindowIdExists(application, window_id)) {
    LOG4CXX_ERROR(logger_,
                  "Window with id #" << window_id << " does already exist");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  const auto window_type =
      (*message_)[strings::msg_params][strings::window_type].asInt();

  if (mobile_apis::WindowType::eType::MAIN == window_type) {
    LOG4CXX_ERROR(logger_, "MAIN application window already exists");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(
          strings::duplicate_updates_from_window_id)) {
    const auto duplicate_updates_from_window_id =
        (*message_)[strings::msg_params]
                   [strings::duplicate_updates_from_window_id]
                       .asInt();
    if (!WindowIdExists(application, duplicate_updates_from_window_id)) {
      LOG4CXX_ERROR(logger_,
                    "Window with id #" << duplicate_updates_from_window_id
                                       << " does not exist");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
  }

  const std::string window_name =
      (*message_)[strings::msg_params][strings::window_name].asString();
  if (!CheckWindowName(application, window_id, window_name)) {
    LOG4CXX_ERROR(logger_,
                  "Window name \"" << window_name
                                   << "\" is disallowed for window #"
                                   << window_id);
    SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
    return;
  }

  smart_objects::SmartObject msg_params = (*message_)[strings::msg_params];
  msg_params[strings::app_id] = application->hmi_app_id();

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(hmi_apis::FunctionID::UI_CreateWindow, &msg_params, true);
}

void CreateWindowRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (hmi_apis::FunctionID::UI_CreateWindow != event.id()) {
    LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
    return;
  }

  auto application = application_manager_.application(connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  LOG4CXX_INFO(logger_, "Received CreateWindow event");
  EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);

  const smart_objects::SmartObject& response_message = event.smart_object();
  const auto result_code = CommandRequestImpl::GetMobileResultCode(
      static_cast<hmi_apis::Common_Result::eType>(
          response_message[strings::params][hmi_response::code].asInt()));

  const bool is_success = IsMobileResultSuccess(result_code);
  std::string response_info;
  GetInfo(response_message, response_info);

  if (!is_success) {
    LOG4CXX_ERROR(logger_, "CreateWindow request has failed on HMI side");
    SendResponse(is_success,
                 result_code,
                 response_info.empty() ? nullptr : response_info.c_str());
    return;
  }

  ApplyWindowInitialState(application);

  SendResponse(is_success,
               result_code,
               response_info.empty() ? nullptr : response_info.c_str());
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
