#include "element.h"

#include <cassert>

namespace winrsls::json {
  Element::Element(const decltype(value)& value) : value(value) {}
  Element::Element(decltype(value)&& value) : value(std::move(value)) {}

  std::string Element::toJsonString() const {
    std::string result;
    std::visit([&result](auto&& value) { result = value.toJsonString(); },
               value);
    return result;
  }

  bool operator==(const Element& lhs, const Element& rhs) {
    return lhs.value == rhs.value;
  }

  std::ostream& operator<<(std::ostream& out, const Element& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json