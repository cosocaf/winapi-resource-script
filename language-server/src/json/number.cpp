/**
 * @file number.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of number.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "number.h"

#include <cassert>
#include <charconv>
#include <cmath>
#include <numeric>

namespace winrsls::json {
  Number::Number(int value) noexcept
    : kind(Kind::Integer), value{.integer = value} {}
  Number::Number(int64_t value) noexcept
    : kind(Kind::Integer), value{.integer = value} {}
  Number::Number(double_t value) noexcept
    : kind(Kind::Decimal), value{.decimal = value} {}

  int64_t Number::asInt() const noexcept { return value.integer; }
  double_t Number::asDec() const noexcept { return value.decimal; }
  int64_t Number::toInt() const noexcept {
    return kind == Kind::Integer ? value.integer
                                 : static_cast<int64_t>(value.decimal);
  }
  double_t Number::toDec() const noexcept {
    return kind == Kind::Decimal ? value.decimal
                                 : static_cast<double_t>(value.integer);
  }

  std::string Number::toJsonString() const {
    char result[128];
    char* end;
    switch (kind) {
      case Kind::Integer: {
        auto [ptr, ec] =
          std::to_chars(std::begin(result), std::end(result), value.integer);
        end = ptr;
        break;
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

  bool operator==(const Number& lhs, const Number& rhs) noexcept {
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