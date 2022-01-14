#ifndef WINRSLS_JSON_ARRAY_H_
#define WINRSLS_JSON_ARRAY_H_

#include <initializer_list>
#include <ostream>
#include <string>
#include <vector>

#include "element_type.h"

namespace winrsls::json {
  class Element;
  class Array {
    std::vector<Element> value;

    friend bool operator==(const Array& lhs, const Array& rhs);

  public:
    Array(const std::vector<Element>& value = {});
    Array(std::vector<Element>&& value);
    Array(std::initializer_list<Element> value);

    template <element_type T>
    static Array from(std::initializer_list<T> value) {
      std::vector<Element> result;
      for (const auto& elem : value) {
        result.emplace_back(elem);
      }
      return Array(std::move(result));
    }

    std::string toJsonString() const;
  };

  bool operator==(const Array& lhs, const Array& rhs);

  std::ostream& operator<<(std::ostream& out, const Array& value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_ARRAY_H_