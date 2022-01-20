/**
 * @file union.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines a type to represent a composite type of JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_UNION_H_
#define WINRSLS_JSON_UNION_H_

#include <cassert>
#include <string>
#include <variant>

#include "../variant_cast.h"
#include "element_like.h"
#include "null.h"

namespace winrsls::json {
  template <typename T, typename... U>
  inline constexpr bool is_union_element_type_v =
    std::disjunction_v<std::is_same<std::remove_cvref_t<T>, U>...>;
  template <typename T, typename... U>
  concept union_element_type = is_union_element_type_v<T, U...>;

  template <element_like... Elems>
  class Union {
  public:
    std::variant<Elems...> value;

  public:
    template <element_like... T>
    Union(const Union<T...>& other)
      : value(variant_cast<Elems...>(other.value)) {}
    template <element_like... T>
    Union(Union<T...>&& other) noexcept
      : value(variant_cast<Elems...>(std::move(other.value))) {}

    template <union_element_type<Elems...> T>
    Union(const T& value) : value(value) {}
    template <union_element_type<Elems...> T>
    Union(T&& value) noexcept : value(std::move(value)) {}

    Union(const std::variant<Elems...>& value) : value(value) {}
    Union(std::variant<Elems...>&& value) noexcept : value(std::move(value)) {}

    template <element_like T>
    bool is() const noexcept {
      return std::holds_alternative<T>(value);
    }
    template <element_like T>
    const T& as() const {
      assert(is<T>());
      return std::get<T>(value);
    }
    template <element_like T>
    T& as() {
      assert(is<T>());
      return std::get<T>(value);
    }

    std::string toJsonString() const {
      return std::visit([](auto&& v) { return v.toJsonString(); }, value);
    }
  };

  template <element_like... T>
  using Nullable = Union<T..., Null>;
} // namespace winrsls::json

#endif // WINRSLS_JSON_UNION_H_