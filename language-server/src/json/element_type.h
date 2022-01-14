#ifndef WINRSLS_JSON_ELEMENT_TYPE_H_
#define WINRSLS_JSON_ELEMENT_TYPE_H_

namespace winrsls::json {
  class String;
  class Number;
  class Boolean;
  class Null;
  class Array;
  class Object;

  template <typename T, typename... Ts>
  inline constexpr bool is_any_of_v =
    std::disjunction_v<std::is_same<T, Ts>...>;

  template <typename T>
  inline constexpr bool is_element_type_v =
    is_any_of_v<std::remove_cv_t<T>, String, Number, Boolean, Null, Array,
                Object>;
  template <typename T>
  concept element_type = is_element_type_v<T>;
} // namespace winrsls::json

#endif // WINRSLS_JSON_ELEMENT_TYPE_H_