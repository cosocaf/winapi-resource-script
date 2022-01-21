/**
 * @file boolean.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines the boolean type among the elements that make up JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_BOOLEAN_H_
#define WINRSLS_JSON_BOOLEAN_H_

#include <string>

#include "element_like.h"

namespace winrsls::json {
  class Boolean {
    bool value;

    friend constexpr bool operator==(const Boolean& lhs, const Boolean& rhs) noexcept;

  public:
    constexpr Boolean() noexcept : value(false){};
    constexpr Boolean(bool value) noexcept : value(value) {}

    constexpr std::string toJsonString() const {
      return value ? "true" : "false";
    }
  };

  static_assert(element_like<Boolean>,
                "Boolean does not satisfy the concept element_like.");

  constexpr bool operator==(const Boolean& lhs, const Boolean& rhs) noexcept {
    return lhs.value == rhs.value;
  }
} // namespace winrsls::json

#endif // WINRSLS_JSON_BOOLEAN_H_