/**
 * @file array.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines the array type among the elements that make up JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_ARRAY_H_
#define WINRSLS_JSON_ARRAY_H_

#include <initializer_list>
#include <string>
#include <vector>

#include "element.h"
#include "number.h"

namespace winrsls::json {
  template <element_like Elem>
  class TypedArray {
    std::vector<Elem> value;

    template <element_like T, element_like U>
    friend bool operator==(const TypedArray<T>& lhs, const TypedArray<U>& rhs);

  public:
    constexpr TypedArray() : value(std::vector<Elem>()) {}
    constexpr TypedArray(const std::vector<Elem>& value) : value(value) {}
    constexpr TypedArray(std::vector<Elem>&& value) noexcept
      : value(std::move(value)) {}
    constexpr TypedArray(std::initializer_list<Elem> value) : value(value) {}

    constexpr size_t length() const noexcept { return value.size(); }
    constexpr size_t size() const noexcept { return value.size(); }

    constexpr Elem& back() { return value.back(); }
    constexpr const Elem& back() const { return value.back(); }

    constexpr Elem& operator[](size_t idx) { return value[idx]; }
    constexpr Elem& operator[](const Number& idx) { return value[idx.asInt()]; }

    constexpr const Elem& operator[](size_t idx) const { return value[idx]; }
    constexpr const Elem& operator[](const Number& idx) const {
      return value[idx.asInt()];
    }

    std::string toJsonString() const {
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
  };

  template <element_like T, element_like U>
  bool operator==(const TypedArray<T>& lhs, const TypedArray<U>& rhs) {
    return lhs.value == rhs.value;
  }

  using Array = TypedArray<Element>;
  static_assert(element_like<Array>,
                "Array does not satisfy the concept element_like.");
} // namespace winrsls::json

#endif // WINRSLS_JSON_ARRAY_H_