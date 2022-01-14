#ifndef WINRSLS_JSON_STRING_H_
#define WINRSLS_JSON_STRING_H_

#include <ostream>
#include <string>
#include <string_view>

namespace winrsls::json {
  class String {
    std::string value;
    friend struct std::hash<String>;
    friend bool operator==(const String& lhs, const String& rhs);

  public:
    String(const std::string& value);
    String(std::string&& value);
    String(std::string_view value);
    String(const char* value);

    std::string toJsonString() const;
  };

  bool operator==(const String& lhs, const String& rhs);
  std::ostream& operator<<(std::ostream& out, const String& value);
} // namespace winrsls::json

namespace std {
  template <>
  struct hash<::winrsls::json::String> {
    size_t operator()(const ::winrsls::json::String& value) const {
      return std::hash<std::string>()(value.value);
    }
  };
} // namespace std

#endif // WINRSLS_JSON_STRING_H_