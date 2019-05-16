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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_CONVERTER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_CONVERTER_H_

#include <functional>
#include <string>
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

/*
 * @brief Сontains a method for translating strings into equivalent enum values.
 * Accepts smart objects with a structure defined in API, changes strings to
 * enum values ​​in place.
 */
class HMICapabilitiesConverter {
 public:
  /*
   * @brief Used to specify a mandatory parameter or not. If the API indicates
   * mandatory = true, you need to use MANDATORY else use  OPTIONAL
   */
  enum class Required { MANDATORY, OPTIONAL };

  typedef std::function<bool(smart_objects::SmartObject&)> Converter;

  /*
   * @brief Convert strings in a DisplayCapability structure into equivalent
   * enum values if the field is enum in API.
   * @param out_display_capability smart object that represents
   * DisplayCapability structure. string to enum value converted in place.
   * @return true if converted successfully else false
   */
  static bool ConvertDisplayCapability(
      smart_objects::SmartObject& out_display_capability);

  /*
   * @brief the pattern for convert SmartArray
   * @param obj smart object that contain array for convert
   * @param key array key in obj
   * @param converter UnaryPredicate that accepts smart object for converting
   * @param required requirement for key, mandatory or optional
   * @return true if converted successfully else false
   */
  static bool ArrayConvertPattern(smart_objects::SmartObject& obj,
                                  const std::string& key,
                                  const Converter& converter,
                                  const Required required = Required::OPTIONAL);

 private:
  /*
   * @brief Convert strings in a WindowTypeCapabilities structure into
   * equivalent enum values if the field is enum in API.
   * @param out_window_type smart object that represents TypeCapabilities
   * structure. string to enum value converted in place.
   * @return true if converted successfully else false
   */
  static bool ConvertWindowTypeCapabilities(
      smart_objects::SmartObject& out_window_type);

  /*
   * @brief Convert strings in a WindowCapability structure into equivalent
   * enum values if the field is enum in API.
   * @param out_window_capability smart object that represents WindowCapability
   * structure. string to enum value converted in place.
   * @return true if converted successfully else false
   */
  static bool ConvertWindowCapability(
      smart_objects::SmartObject& out_window_capability);

  /*
   * @brief Convert strings in a TextField structure into equivalent
   * enum values if the field is enum in API.
   * @param out_text_field smart object that represents TextField
   * structure. string to enum value converted in place.
   * @return true if converted successfully else false
   */
  static bool ConvertTextField(smart_objects::SmartObject& out_text_field);

  /*
   * @brief Convert strings in a ImageField structure into equivalent
   * enum values if the field is enum in API.
   * @param out_image_field smart object that represents ImageField
   * structure. string to enum value converted in place.
   * @return true if converted successfully else false
   */
  static bool ConvertImageField(smart_objects::SmartObject& out_image_field);

  /*
   * @brief Convert strings in a ButtonCapabilities structure into equivalent
   * enum values if the field is enum in API.
   * @param out_button_capabilities smart object that represents
   * ButtonCapabilities structure. string to enum value converted in place.
   * @return true if converted successfully else false
   */
  static bool ConvertButtonCapabilities(
      smart_objects::SmartObject& out_button_capabilities);

  /*
   * @brief Convert string to the equivalent enum value
   * @param out_str_value smart_objects that must be SmartType_String. string to
   * enum value converted in place
   * @return true if converted successfully else false
   */
  template <typename EnumType>
  static bool ConvertSmartTypeStringToEnumValue(
      smart_objects::SmartObject& out_str_value);
};

template <typename EnumType>
bool HMICapabilitiesConverter::ConvertSmartTypeStringToEnumValue(
    smart_objects::SmartObject& out_str_value) {
  using namespace smart_objects;
  if (SmartType_String != out_str_value.getType()) {
    return false;
  }

  EnumType enum_value;
  if (EnumConversionHelper<EnumType>::StringToEnum(out_str_value.asString(),
                                                   &enum_value)) {
    out_str_value = enum_value;
    return true;
  }
  out_str_value = EnumType::INVALID_ENUM;
  return true;
}

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_CONVERTER_H_
