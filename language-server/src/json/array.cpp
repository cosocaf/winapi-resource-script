/**
 * @file array.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of array.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "array.h"

namespace winrsls::json {
  Array::Array(const std::vector<Element>& value) : value(value) {}
  Array::Array(std::vector<Element>&& value) noexcept
    : value(std::move(value)) {}
  Array::Array(std::initializer_list<Element> value) : value(value) {}

  size_t Array::length() const noexcept { return value.size(); }
  size_t Array::size() const noexcept { return value.size(); }

  Element& Array::back() { return value.back(); }
  const Element& Array::back() const { return value.back(); }

  Element& Array::operator[](size_t idx) { return value[idx]; }
  Element& Array::operator[](const Number& idx) { return value[idx.asInt()]; }

  const Element& Array::operator[](size_t idx) const { return value[idx]; }
  const Element& Array::operator[](const Number& idx) const {
    return value[idx.asInt()];
  }

  std::string Array::toJsonString() const {
    std::string result;
    result += '[';
    for (const auto& elem : value) {
      result += elem.toJsonString();
      result += ',';
    }
    // erase last comma. [1, 2, 3,/* <- like this */]
    if (!value.empty()) {
      result.pop_back();
    }
    result += ']';
    return result;
  }

  bool operator==(const Array& lhs, const Array& rhs) {
    return lhs.value == rhs.value;
  }

  std::ostream& operator<<(std::ostream& out, const Array& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json