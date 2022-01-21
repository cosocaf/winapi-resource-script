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
} // namespace winrsls::json