#include "null.h"

namespace winrsls::json {
  std::string Null::toJsonString() const { return "null"; }

  bool operator==(const Null& lhs, const Null& rhs) { return true; }

  std::ostream& operator<<(std::ostream& out, const Null& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json