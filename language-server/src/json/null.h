/**
 * @file null.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines the null type among the elements that make up JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_NULL_H_
#define WINRSLS_JSON_NULL_H_

#include <string>

#include "element_like.h"

namespace winrsls::json {
  class Null {
    friend constexpr bool operator==(const Null& lhs, const Null& rhs) noexcept;

  public:
    constexpr std::string toJsonString() const { return "null"; }
  };

  static_assert(element_like<Null>,
                "Null does not satisfy the concept element_like.");

  constexpr bool operator==([[maybe_unused]] const Null& lhs,
                            [[maybe_unused]] const Null& rhs) noexcept {
    return true;
  }
} // namespace winrsls::json

#endif // WINRSLS_JSON_NULL_H_