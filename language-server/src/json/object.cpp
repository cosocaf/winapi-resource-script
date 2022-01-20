/**
 * @file object.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of object.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "object.h"

namespace winrsls::json {
  Object::Object(const std::unordered_map<String, Element>& value)
    : value(value) {}
  Object::Object(std::unordered_map<String, Element>&& value) noexcept
    : value(std::move(value)) {}
  Object::Object(std::initializer_list<std::pair<String, Element>> value)
    : value(value.begin(), value.end()) {}

  Element& Object::at(const String& key) { return value.at(key); }
  const Element& Object::at(const String& key) const { return value.at(key); }

  bool Object::contains(const String& key) const { return value.contains(key); }

  Element& Object::operator[](const String& key) { return value[key]; }

  std::string Object::toJsonString() const {
    std::string result;
    result += '{';
    for (const auto& [key, val] : value) {
      result += key.toJsonString();
      result += ':';
      result += val.toJsonString();
      result += ',';
    }
    // erase last comma. {"a": 1, "b": 2, /* <- like this */}
    if (!value.empty()) {
      result.pop_back();
    }
    result += '}';
    return result;
  }

  bool operator==(const Object& lhs, const Object& rhs) {
    return lhs.value == rhs.value;
  }

  std::ostream& operator<<(std::ostream& out, const Object& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json