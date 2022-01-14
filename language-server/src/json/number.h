#ifndef WINRSLS_JSON_NUMBER_H_
#define WINRSLS_JSON_NUMBER_H_

#include <ostream>
#include <string>

namespace winrsls::json {
  class Number {
    enum struct Kind {
      Integer,
      Decimal,
    };
    union Value {
      int64_t integer;
      double_t decimal;
    };

    Kind kind;
    Value value;

    friend bool operator==(const Number& lhs, const Number& rhs);

  public:
    Number(int value);
    Number(int64_t value);
    Number(double_t value);

    std::string toJsonString() const;
  };

  bool operator==(const Number& lhs, const Number& rhs);
  std::ostream& operator<<(std::ostream& out, const Number& value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_NUMBER_H_