/**
 * @file boolean.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of boolean.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "boolean.h"

namespace winrsls::json {
  Boolean::Boolean(bool value) noexcept : value(value) {}

  std::string Boolean::toJsonString() const { return value ? "true" : "false"; }

  bool operator==(const Boolean& lhs, const Boolean& rhs) noexcept {
    return lhs.value == rhs.value;
  }

  std::ostream& operator<<(std::ostream& out, Boolean value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json