#ifndef WINRSLS_JSON_NULL_H_
#define WINRSLS_JSON_NULL_H_

#include <ostream>
#include <string>

namespace winrsls::json {
  class Null {
    friend bool operator==(const Null& lhs, const Null& rhs);

  public:
    std::string toJsonString() const;
  };

  bool operator==(const Null& lhs, const Null& rhs);

  std::ostream& operator<<(std::ostream& out, const Null& value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_NULL_H_