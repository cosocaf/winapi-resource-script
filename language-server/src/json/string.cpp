/**
 * @file string.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of string.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "string.h"

namespace winrsls::json {
  String::String(const std::string& value) : value(value) {}
  String::String(std::string&& value) noexcept : value(std::move(value)) {}
  String::String(std::string_view value) : value(value) {}
  String::String(const char* value) : value(value) {}

  const std::string& String::getValue() const noexcept { return value; }

  std::string String::toJsonString() const {
    std::string result;
    result += '\"';
    for (const auto c : value) {
      switch (c) {
        case '\n': result += "\\n"; break;
        case '\\': result += "\\\\"; break;
        case '\t': result += "\\t"; break;
        case '\?': result += "\\?"; break;
        case '\v': result += "\\v"; break;
        case '\'': result += "\\'"; break;
        case '\"': result += "\\\""; break;
        case '\b': result += "\\b"; break;
        case '\r': result += "\\r"; break;
        case '\0': result += "\\0"; break;
        case '\f': result += "\\f"; break;
        case '\a': result += "\\a"; break;
        default: result += c;
      }
    }
    result += '\"';

    return result;
  }

  bool operator==(const String& lhs, const String& rhs) noexcept {
    return lhs.value == rhs.value;
  }
  std::ostream& operator<<(std::ostream& out, const String& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json