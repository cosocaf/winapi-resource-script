#ifndef WINRSLS_JSON_UNION_H_
#define WINRSLS_JSON_UNION_H_

#include <cassert>
#include <string>
#include <variant>

#include "element_type.h"

namespace winrsls::json {
  template <typename T, typename... U>
  inline constexpr bool is_union_element_type_v =
    is_any_of_v<std::remove_cv_t<T>, U...>;
  template <typename T, typename... U>
  concept union_element_type = is_union_element_type_v<T, U...>;

  template <element_type... Elems>
  class Union {
    std::variant<Elems...> value;
    friend class Element;

  public:
    Union(const Union& other) : value(other.value) {}
    Union(Union&& other) : value(std::move(other.value)) {}
    template <union_element_type<Elems...> T>
    Union(const T& value) : value(value) {}
    template <union_element_type<Elems...> T>
    Union(T&& value) : value(std::move(value)) {}
    Union(const std::variant<Elems...>& value) : value(value) {}
    Union(std::variant<Elems...>&& value) : value(std::move(value)) {}

    Union& operator=(const Union& other) {
      value = other.value;
      return *this;
    }
    Union& operator=(Union&& other) {
      value = std::move(std::move(other.value));
      return *this;
    }

    template <typename... To>
    operator Union<To...>() const {
      return std::visit([](auto&& arg) -> Union<To...> { return arg; }, value);
    }

    template <element_type T>
    bool is() const {
      return std::holds_alternative<T>(value);
    }
    template <element_type T>
    const T& as() const {
      assert(is<T>());
      return std::get<T>(value);
    }
    template <element_type T>
    T& as() {
      assert(is<T>());
      return std::get<T>(value);
    }

    std::string toJsonString() const {
      return std::visit([](auto&& v) { return v.toJsonString(); }, value);
    }
  };
} // namespace winrsls::json

#endif // WINRSLS_JSON_UNION_H_