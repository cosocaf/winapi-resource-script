/**
 * @file number.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines the number type among the elements that make up JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_NUMBER_H_
#define WINRSLS_JSON_NUMBER_H_

#include <compare>
#include <string>

#include "element_like.h"

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

    friend constexpr std::partial_ordering operator<=>(const Number& lhs,
                                                       const Number& rhs);

  public:
    constexpr Number() noexcept : value{.integer = 0}, kind(Kind::Integer) {}
    constexpr Number(int value) noexcept
      : kind(Kind::Integer), value{.integer = value} {}
    constexpr Number(int64_t value) noexcept
      : kind(Kind::Integer), value{.integer = value} {}
    constexpr Number(double_t value) noexcept
      : kind(Kind::Decimal), value{.decimal = value} {}

    /**
     * @brief Convert to integer type.
     *
     * This method should be used in situations where the type of the internal
     * value can be guaranteed to be an integer. Otherwise, the toInt method can
     * be used to cast safely.
     *
     * @return int64_t value
     */
    constexpr int64_t asInt() const noexcept { return value.integer; }
    /**
     * @brief Convert to decimal type.
     *
     * This method should be used in situations where the type of the internal
     * value can be guaranteed to be a real number. Otherwise, the toDec method
     * can be used to cast safely.
     *
     * @return double_t value
     */
    constexpr double_t asDec() const noexcept { return value.decimal; }
    /**
     * @brief Convert to integer type.
     *
     * If you can guarantee that the type of the internal value is an integer,
     * it is recommended to use the asInt method.
     *
     * @return int64_t value
     */
    constexpr int64_t toInt() const noexcept {
      if (kind == Kind::Integer) return value.integer;
      return static_cast<int64_t>(value.decimal);
    }
    /**
     * @brief Convert to decimal type.
     *
     * If you can guarantee that the type of the internal value is a real
     * number, it is recommended to use the asDec method.
     *
     * @return double_t value
     */
    constexpr double_t toDec() const noexcept {
      if (kind == Kind::Decimal) return value.decimal;
      return static_cast<double_t>(value.integer);
    }

    std::string toJsonString() const;
  };

  static_assert(element_like<Number>,
                "Number does not satisfy the concept element_like.");

  constexpr std::partial_ordering operator<=>(const Number& lhs,
                                              const Number& rhs) {
    if (lhs.kind == rhs.kind) {
      switch (lhs.kind) {
        case Number::Kind::Integer:
          return lhs.value.integer <=> rhs.value.integer;
        case Number::Kind::Decimal:
          return lhs.value.decimal <=> rhs.value.decimal;
      }
    } else {
      switch (lhs.kind) {
        case Number::Kind::Integer:
          return static_cast<double_t>(lhs.value.integer) <=> rhs.value.decimal;
        case Number::Kind::Decimal:
          return lhs.value.decimal <=> static_cast<double_t>(rhs.value.integer);
      }
    }

    return std::partial_ordering::unordered;
  }
} // namespace winrsls::json

#endif // WINRSLS_JSON_NUMBER_H_