#ifndef WINRSLS_JSON_ELEMENT_H_
#define WINRSLS_JSON_ELEMENT_H_

#include <cassert>
#include <optional>
#include <ostream>
#include <string>
#include <type_traits>
#include <variant>

#include "array.h"
#include "boolean.h"
#include "element_type.h"
#include "null.h"
#include "number.h"
#include "object.h"
#include "string.h"
#include "union.h"
#include "variant_cast.h"

namespace winrsls::json {
  class Element {
    std::variant<String, Number, Boolean, Null, Array, Object> value;
    friend class Object;

    friend bool operator==(const Element& lhs, const Element& rhs);

  public:
    Element(const decltype(value)& value = Null());
    Element(decltype(value)&& value);

    template <element_type T>
    Element(const T& value) : value(value) {}
    template <element_type T>
    Element(T&& value) : value(std::move(value)) {}

    template <element_type... Elems>
    Element(const Union<Elems...>& value)
      : value(variant_cast<String, Number, Boolean, Null, Array, Object>(
          value.value)) {}
    template <element_type... Elems>
    Element(Union<Elems...>&& value)
      : value(variant_cast<String, Number, Boolean, Null, Array, Object>(
          std::move(value.value))) {}

    template <element_type T>
    inline bool is() const {
      return std::holds_alternative<T>(value);
    }
    template <element_type... T>
    inline bool either() const {
      return (false || ... || std::holds_alternative<T>(value));
    }
    template <element_type T>
    inline const T& as() const {
      assert(is<T>());
      return std::get<T>(value);
    }
    template <element_type T>
    inline T& as() {
      assert(is<T>());
      return std::get<T>(value);
    }
    template <element_type... T>
    inline Union<T...> as_union() const {
      assert(either<T...>());
      std::optional<Union<T...>> result;
      (void)std::initializer_list{(void(_assign<T>(result)), 0)...};
      return result.value();
    }

    std::string toJsonString() const;

  private:
    template <typename T, typename... U>
    void _assign(std::optional<Union<U...>>& result) const {
      if (std::holds_alternative<T>(value)) {
        result.emplace(std::get<T>(value));
      }
    }
  };

  bool operator==(const Element& lhs, const Element& rhs);

  std::ostream& operator<<(std::ostream& out, const Element& value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_ELEMENT_H_