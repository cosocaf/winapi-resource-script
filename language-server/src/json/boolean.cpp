#include "boolean.h"

namespace winrsls::json {
  Boolean::Boolean(bool value) : value(value) {}

  std::string Boolean::toJsonString() const { return value ? "true" : "false"; }

  bool operator==(const Boolean& lhs, const Boolean& rhs) {
    return lhs.value == rhs.value;
  }

  std::ostream& operator<<(std::ostream& out, Boolean value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json