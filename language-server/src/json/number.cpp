#include "number.h"

#include <cassert>
#include <charconv>
#include <cmath>
#include <numeric>

namespace winrsls::json {
  Number::Number(int value) : kind(Kind::Integer), value{.integer = value} {}
  Number::Number(int64_t value)
    : kind(Kind::Integer), value{.integer = value} {}
  Number::Number(double_t value)
    : kind(Kind::Decimal), value{.decimal = value} {}

  std::string Number::toJsonString() const {
    char result[128];
    char* end;
    switch (kind) {
      case Kind::Integer: {
        auto [ptr, ec] =
          std::to_chars(std::begin(result), std::end(result), value.integer);
        end = ptr;
      }
      case Kind::Decimal: {
        auto [ptr, ec] =
          std::to_chars(std::begin(result), std::end(result), value.decimal);
        end = ptr;
        break;
      }
      default: std::abort();
    }
    return std::string(result, end);
  }

  template <std::floating_point T>
  bool fequals(T a, T b) {
    return fabs(a - b) <= std::numeric_limits<T>::epsilon() *
                            fmax(static_cast<T>(1), fmax(fabs(a), fabs(b)));
  }

  bool operator==(const Number& lhs, const Number& rhs) {
    if (lhs.kind == rhs.kind) {
      switch (lhs.kind) {
        case Number::Kind::Integer:
          return lhs.value.integer == rhs.value.integer;
        case Number::Kind::Decimal:
          return fequals(lhs.value.decimal, rhs.value.decimal);
        default: std::abort();
      }
    } else {
      switch (lhs.kind) {
        case Number::Kind::Integer:
          assert(rhs.kind == Number::Kind::Decimal);
          return fequals(static_cast<double_t>(lhs.value.integer),
                         rhs.value.decimal);
        case Number::Kind::Decimal:
          assert(rhs.kind == Number::Kind::Integer);
          return fequals(lhs.value.decimal,
                         static_cast<double_t>(rhs.value.integer));
        default: std::abort();
      }
    }
  }

  std::ostream& operator<<(std::ostream& out, const Number& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json