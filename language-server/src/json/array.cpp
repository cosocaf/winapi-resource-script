#include "array.h"

#include "element.h"

namespace winrsls::json {
  Array::Array(const std::vector<Element>& value) : value(value) {}
  Array::Array(std::vector<Element>&& value) : value(std::move(value)) {}
  Array::Array(std::initializer_list<Element> value) : value(value) {}
  std::string Array::toJsonString() const {
    std::string result;
    result += '[';
    for (const auto& elem : value) {
      result += elem.toJsonString();
      result += ',';
    }
    // erase last comma. [1, 2, 3,/* <- like this */]
    if (!value.empty()) {
      result.pop_back();
    }
    result += ']';
    return result;
  }

  bool operator==(const Array& lhs, const Array& rhs) {
    return lhs.value == rhs.value;
  }

  std::ostream& operator<<(std::ostream& out, const Array& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json