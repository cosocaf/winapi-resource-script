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

#include <ostream>
#include <string>

#include "element_like.h"

namespace winrsls::json {
  class Null {
    friend bool operator==(const Null& lhs, const Null& rhs) noexcept;

  public:
    std::string toJsonString() const;
  };

  static_assert(element_like<Null>,
                "Null does not satisfy the concept element_like.");

  bool operator==(const Null& lhs, const Null& rhs) noexcept;

  std::ostream& operator<<(std::ostream& out, const Null& value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_NULL_H_