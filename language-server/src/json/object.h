/**
 * @file object.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines the object type among the elements that make up JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_OBJECT_H_
#define WINRSLS_JSON_OBJECT_H_

#include <initializer_list>
#include <ostream>
#include <string>
#include <unordered_map>

#include "element.h"
#include "element_like.h"
#include "string.h"

namespace winrsls::json {
  class Object {
    std::unordered_map<String, Element> value;

    friend bool operator==(const Object& lhs, const Object& rhs);

  public:
    Object(const std::unordered_map<String, Element>& value = {});
    Object(std::unordered_map<String, Element>&& value) noexcept;
    Object(std::initializer_list<std::pair<String, Element>> value);

    Element& at(const String& key);
    const Element& at(const String& key) const;

    bool contains(const String& key) const;

    template <element_like... T>
    bool contains_as(const String& key) const {
      return contains(key) &&
             (false || ... || value.at(key).is<std::remove_cvref_t<T>>());
    }

    Element& operator[](const String& key);

    std::string toJsonString() const;
  };

  static_assert(element_like<Object>,
                "Object does not satisfy the concept element_like.");

  bool operator==(const Object& lhs, const Object& rhs);

  std::ostream& operator<<(std::ostream& out, const Object& value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_OBJECT_H_