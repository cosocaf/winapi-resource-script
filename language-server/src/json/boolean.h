#ifndef WINRSLS_JSON_BOOLEAN_H_
#define WINRSLS_JSON_BOOLEAN_H_

#include <ostream>
#include <string>

namespace winrsls::json {
  class Boolean {
    bool value;

    friend bool operator==(const Boolean& lhs, const Boolean& rhs);

  public:
    Boolean(bool value);

    std::string toJsonString() const;
  };

  bool operator==(const Boolean& lhs, const Boolean& rhs);
  std::ostream& operator<<(std::ostream& out, Boolean value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_BOOLEAN_H_