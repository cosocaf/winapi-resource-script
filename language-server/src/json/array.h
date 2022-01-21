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
#include <ostream>
#include <string>
#include <vector>

#include "element.h"
#include "number.h"

namespace winrsls::json {
  class Array {
    std::vector<Element> value;

    friend bool operator==(const Array& lhs, const Array& rhs);

  public:
    constexpr Array() : value(std::vector<Element>()) {}
    constexpr Array(const std::vector<Element>& value) : value(value) {}
    constexpr Array(std::vector<Element>&& value) noexcept
      : value(std::move(value)) {}
    constexpr Array(std::initializer_list<Element> value) : value(value) {}

    template <element_like T>
    constexpr static Array from(std::initializer_list<T> value) {
      std::vector<Element> result;
      result.reserve(value.size());
      for (const auto& elem : value) {
        result.emplace_back(elem);
      }
      return Array(std::move(result));
    }

    constexpr size_t length() const noexcept { return value.size(); }
    constexpr size_t size() const noexcept { return value.size(); }

    constexpr Element& back() { return value.back(); }
    constexpr const Element& back() const { return value.back(); }

    constexpr Element& operator[](size_t idx) { return value[idx]; }
    constexpr Element& operator[](const Number& idx) {
      return value[idx.asInt()];
    }

    constexpr const Element& operator[](size_t idx) const { return value[idx]; }
    constexpr const Element& operator[](const Number& idx) const {
      return value[idx.asInt()];
    }

    std::string toJsonString() const;
  };

  static_assert(element_like<Array>,
                "Array does not satisfy the concept element_like.");

  bool operator==(const Array& lhs, const Array& rhs);
} // namespace winrsls::json

#endif // WINRSLS_JSON_ARRAY_H_