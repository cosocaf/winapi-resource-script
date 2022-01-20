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

#include <ostream>
#include <string>

#include "element_like.h"

namespace winrsls::json {
  class Boolean {
    bool value;

    friend bool operator==(const Boolean& lhs, const Boolean& rhs) noexcept;

  public:
    Boolean(bool value) noexcept;

    std::string toJsonString() const;
  };

  static_assert(element_like<Boolean>,
                "Boolean does not satisfy the concept element_like.");

  bool operator==(const Boolean& lhs, const Boolean& rhs) noexcept;
  std::ostream& operator<<(std::ostream& out, Boolean value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_BOOLEAN_H_