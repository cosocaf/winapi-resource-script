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
#include <string>
#include <unordered_map>

#include "element.h"
#include "string.h"

namespace winrsls::json {
  template <typename T, typename U>
  concept implement_is = requires(T value) {
    { value.template is<U>() } -> std::same_as<bool>;
  };

  template <element_like Elem>
  class TypedObject {
    std::unordered_map<String, Elem> value;

    template <element_like T, element_like U>
    friend bool operator==(const TypedObject<T>& lhs,
                           const TypedObject<U>& rhs);

  public:
    TypedObject(const std::unordered_map<String, Elem>& value = {})
      : value(value) {}
    TypedObject(std::unordered_map<String, Elem>&& value) noexcept
      : value(std::move(value)) {}
    TypedObject(std::initializer_list<std::pair<String, Elem>> value)
      : value(value.begin(), value.end()) {}

    Elem& at(const String& key) { return value.at(key); }
    const Elem& at(const String& key) const { return value.at(key); }

    bool contains(const String& key) const { return value.contains(key); }

    template <element_like... T>
    bool contains_as(const String& key) const {
      if constexpr ((false || ... || implement_is<Elem, T>)) {
        return contains(key) &&
               (false || ... || value.at(key).is<std::remove_cvref_t<T>>());
      } else {
        return contains(key) &&
               std::disjunction_v<
                 std::is_same<std::remove_cvref_t<Elem>, T>...>;
      }
    }

    Elem& operator[](const String& key) { return value[key]; }

    std::string toJsonString() const {
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
  };

  template <element_like T, element_like U>
  bool operator==(const TypedObject<T>& lhs, const TypedObject<U>& rhs) {
    return lhs.value == rhs.value;
  }

  using Object = TypedObject<Element>;
  static_assert(element_like<Object>,
                "Object does not satisfy the concept element_like.");
} // namespace winrsls::json

#endif // WINRSLS_JSON_OBJECT_H_