/**
 * @file string.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines the string type among the elements that make up JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_STRING_H_
#define WINRSLS_JSON_STRING_H_

#include <ostream>
#include <string>
#include <string_view>

#include "element_like.h"

namespace winrsls::json {
  class String {
    std::string value;
    friend struct std::hash<String>;
    friend bool operator==(const String& lhs, const String& rhs) noexcept;

  public:
    String(const std::string& value = std::string());
    String(std::string&& value) noexcept;
    String(std::string_view value);
    String(const char* value);

    const std::string& getValue() const noexcept;

    std::string toJsonString() const;
  };

  static_assert(element_like<String>,
                "String does not satisfy the concept element_like.");

  bool operator==(const String& lhs, const String& rhs) noexcept;
  std::ostream& operator<<(std::ostream& out, const String& value);
} // namespace winrsls::json

namespace std {
  template <>
  struct hash<::winrsls::json::String> {
    size_t operator()(const ::winrsls::json::String& value) const {
      return std::hash<std::string>()(value.value);
    }
  };
} // namespace std

#endif // WINRSLS_JSON_STRING_H_