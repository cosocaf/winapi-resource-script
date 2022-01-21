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

#include <string>
#include <string_view>

#include "element_like.h"

namespace winrsls::json {
  class String {
    std::string value;
    friend struct std::hash<String>;
    friend constexpr bool operator==(const String& lhs, const String& rhs) noexcept;

  public:
    constexpr String() : value(std::string()) {}
    constexpr String(const std::string& value) : value(value) {}
    constexpr String(std::string&& value) noexcept : value(std::move(value)) {}
    constexpr String(std::string_view value) : value(value) {}
    constexpr String(const char* value) : value(value) {}

    constexpr const std::string& getValue() const noexcept { return value; }

    constexpr std::string toJsonString() const {
      std::string result("\"");
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
  };

  static_assert(element_like<String>,
                "String does not satisfy the concept element_like.");

  constexpr bool operator==(const String& lhs, const String& rhs) noexcept {
    return lhs.value == rhs.value;
  }
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