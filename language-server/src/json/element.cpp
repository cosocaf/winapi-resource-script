/**
 * @file element.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of element.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "element.h"

#include "json.h"

namespace winrsls::json {
  Element::Element(const Element& other) : value(other.value) {}
  Element::Element(Element&& other) noexcept : value(std::move(other.value)) {}

  // Create as String

  Element::Element(const std::string& value) : value(String(value)) {}
  Element::Element(std::string&& value) : value(String(std::move(value))) {}
  Element::Element(std::string_view value) : value(String(value)) {}
  Element::Element(const char* value) : value(String(value)) {}

  // Create as Number

  Element::Element(int64_t value) : value(Number(value)) {}
  Element::Element(int value) : value(Number(value)) {}
  Element::Element(double_t value) : value(Number(value)) {}

  // Create as Boolean

  Element::Element(bool value) : value(Boolean(value)) {}

  // Create as Null (Default constructor)

  Element::Element() : value(Null()) {}

  // Create as Array

  Element::Element(const std::vector<Element>& value) : value(Array(value)) {}
  Element::Element(std::vector<Element>&& value)
    : value(Array(std::move(value))) {}
  Element::Element(std::initializer_list<Element> value)
    : value(Array(value)) {}

  // Create as Object

  Element::Element(const std::unordered_map<String, Element>& value)
    : value(Object(value)) {}
  Element::Element(std::unordered_map<String, Element>&& value)
    : value(Object(std::move(value))) {}
  Element::Element(std::initializer_list<std::pair<String, Element>> value)
    : value(Object(value)) {}

  Element& Element::operator=(const Element& other) & {
    value = other.value;
    return *this;
  }
  Element& Element::operator=(Element&& other) & noexcept {
    value = std::move(other.value);
    return *this;
  }

  std::string Element::toJsonString() const {
    return value.toJsonString(&value.ptr);
  }

  Element::Storage::Storage(const Storage& other) {
    if (other.copy) {
      other.copy(&ptr, &other.ptr);
      deleter = other.deleter;
      copy = other.copy;
      equals = other.equals;
      toJsonString = other.toJsonString;
    } else {
      memset(&ptr, 0, maxSmallSize);
      deleter = nullptr;
      copy = nullptr;
      equals = nullptr;
      toJsonString = nullptr;
    }
  }
  Element::Storage::Storage(Storage&& other) noexcept {
    memcpy(&ptr, &other.ptr, maxSmallSize);
    deleter = other.deleter;
    copy = other.copy;
    equals = other.equals;
    toJsonString = other.toJsonString;

    memset(&other.ptr, 0, maxSmallSize);
    other.deleter = nullptr;
    other.copy = nullptr;
    other.equals = nullptr;
    other.toJsonString = nullptr;
  }
  Element::Storage::~Storage() {
    if (deleter) {
      deleter(&ptr);
    }
    deleter = nullptr;
    copy = nullptr;
    equals = nullptr;
    toJsonString = nullptr;
  }
  Element::Storage& Element::Storage::operator=(const Storage& other) & {
    this->~Storage();
    new (this) Storage(other);
    return *this;
  }
  Element::Storage& Element::Storage::operator=(Storage&& other) & noexcept {
    this->~Storage();
    new (this) Storage(std::move(other));
    return *this;
  }

  bool operator==(const Element& lhs, const Element& rhs) {
    return Element::Storage::same(lhs.value, rhs.value);
  }

  std::ostream& operator<<(std::ostream& out, const Element& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json