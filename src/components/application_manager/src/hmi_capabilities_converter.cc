/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/hmi_capabilities_converter.h"

#include <algorithm>

#include "application_manager/smart_object_keys.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "HMICapabilitiesConverter")

bool HMICapabilitiesConverter::ConvertDisplayCapability(
    smart_objects::SmartObject& out_display_capability) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!out_display_capability.keyExists(strings::display_type)) {
    LOG4CXX_ERROR(logger_, "displayType does not exist");
    return false;
  }

  if (!ConvertSmartTypeStringToEnumValue<hmi_apis::Common_DisplayType::eType>(
          out_display_capability[strings::display_type])) {
    LOG4CXX_ERROR(logger_, "DisplayType conversion is failed");
    return false;
  }

  if (!ArrayConvertPattern(out_display_capability,
                           strings::window_type_supported,
                           &ConvertWindowTypeCapabilities)) {
    LOG4CXX_ERROR(logger_, "WindowTypeCapabilities conversion is failed");
    return false;
  }

  if (!ArrayConvertPattern(out_display_capability,
                           strings::window_capabilities,
                           &ConvertWindowCapability)) {
    LOG4CXX_ERROR(logger_, "WindowCapability conversion is failed");
    return false;
  }
  return true;
}

bool HMICapabilitiesConverter::ConvertWindowTypeCapabilities(
    smart_objects::SmartObject& out_window_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!out_window_type.keyExists(strings::type)) {
    LOG4CXX_ERROR(logger_, "Mandatory field does not exist");
    return false;
  }

  return ConvertSmartTypeStringToEnumValue<hmi_apis::Common_WindowType::eType>(
      out_window_type[strings::type]);
}

bool HMICapabilitiesConverter::ConvertWindowCapability(
    smart_objects::SmartObject& out_window_capability) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ArrayConvertPattern(
          out_window_capability, strings::text_fields, &ConvertTextField)) {
    LOG4CXX_ERROR(logger_, "TextField conversion is failed");
    return false;
  }

  if (!ArrayConvertPattern(
          out_window_capability, strings::image_fields, &ConvertImageField)) {
    LOG4CXX_ERROR(logger_, "ImageField conversion is failed");
    return false;
  }

  if (!ArrayConvertPattern(out_window_capability,
                           strings::image_type_supported,
                           ConvertSmartTypeStringToEnumValue<
                               hmi_apis::Common_ImageType::eType>)) {
    LOG4CXX_ERROR(logger_, "ImageType conversion is failed");
    return false;
  }

  if (!ArrayConvertPattern(out_window_capability,
                           strings::button_capabilities,
                           &ConvertButtonCapabilities)) {
    LOG4CXX_ERROR(logger_, "ButtonCapabilities conversion is failed");
    return false;
  }

  return true;
}

bool HMICapabilitiesConverter::ConvertTextField(
    smart_objects::SmartObject& out_text_field) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!out_text_field.keyExists(strings::name) ||
      !ConvertSmartTypeStringToEnumValue<hmi_apis::Common_TextFieldName::eType>(
          out_text_field[strings::name])) {
    LOG4CXX_ERROR(logger_, "TextFieldName conversion is failed");
    return false;
  }
  if (!out_text_field.keyExists(strings::character_set) ||
      !ConvertSmartTypeStringToEnumValue<hmi_apis::Common_CharacterSet::eType>(
          out_text_field[strings::character_set])) {
    LOG4CXX_ERROR(logger_, "CharacterSet conversion is failed");
    return false;
  }
  return true;
}

bool HMICapabilitiesConverter::ConvertImageField(
    smart_objects::SmartObject& out_image_field) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!out_image_field.keyExists(strings::name) ||
      !ConvertSmartTypeStringToEnumValue<
          hmi_apis::Common_ImageFieldName::eType>(
          out_image_field[strings::name])) {
    LOG4CXX_ERROR(logger_, "ImageFieldName conversion is failed");
    return false;
  }

  if (!ArrayConvertPattern(out_image_field,
                           strings::image_type_supported,
                           ConvertSmartTypeStringToEnumValue<
                               hmi_apis::Common_FileType::eType>)) {
    LOG4CXX_ERROR(logger_, "FileType conversion is failed");
    return false;
  }
  return true;
}

bool HMICapabilitiesConverter::ConvertButtonCapabilities(
    smart_objects::SmartObject& out_button_capabilities) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!out_button_capabilities.keyExists(strings::name)) {
    LOG4CXX_ERROR(logger_, "ButtonName conversion is failed");
    return false;
  }
  return ConvertSmartTypeStringToEnumValue<hmi_apis::Common_ButtonName::eType>(
      out_button_capabilities[strings::name]);
}

bool HMICapabilitiesConverter::ArrayConvertPattern(
    smart_objects::SmartObject& obj,
    const std::string& key,
    const Converter& converter,
    const Required required) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!obj.keyExists(key)) {
    return Required::OPTIONAL == required;
  }
  auto array = obj[key].asArray();
  if (!array) {
    return false;
  }
  /*
   * std::find_if_not searches first element equals false. If result equal end()
   * that converting for all array element success
   */
  return array->end() ==
         std::find_if_not(array->begin(), array->end(), converter);
}

}  // namespace application_manager
