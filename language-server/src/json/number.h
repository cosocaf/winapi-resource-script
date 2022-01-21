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

#include <ostream>
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

    friend bool operator==(const Number& lhs, const Number& rhs) noexcept;

  public:
    Number(int value = 0) noexcept;
    Number(int64_t value) noexcept;
    Number(double_t value) noexcept;

    /**
     * @brief Convert to integer type.
     *
     * This method should be used in situations where the type of the internal
     * value can be guaranteed to be an integer. Otherwise, the toInt method can
     * be used to cast safely.
     *
     * @return int64_t value
     */
    int64_t asInt() const noexcept;
    /**
     * @brief Convert to decimal type.
     *
     * This method should be used in situations where the type of the internal
     * value can be guaranteed to be a real number. Otherwise, the toDec method
     * can be used to cast safely.
     *
     * @return double_t value
     */
    double_t asDec() const noexcept;
    /**
     * @brief Convert to integer type.
     *
     * If you can guarantee that the type of the internal value is an integer,
     * it is recommended to use the asInt method.
     *
     * @return int64_t value
     */
    int64_t toInt() const noexcept;
    /**
     * @brief Convert to decimal type.
     *
     * If you can guarantee that the type of the internal value is a real
     * number, it is recommended to use the asDec method.
     *
     * @return double_t value
     */
    double_t toDec() const noexcept;

    std::string toJsonString() const;
  };

  static_assert(element_like<Number>,
                "Number does not satisfy the concept element_like.");

  bool operator==(const Number& lhs, const Number& rhs) noexcept;
  std::ostream& operator<<(std::ostream& out, const Number& value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_NUMBER_H_