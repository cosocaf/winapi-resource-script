/**
 * @file null.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of null.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "null.h"

namespace winrsls::json {
  std::string Null::toJsonString() const { return "null"; }

  bool operator==([[maybe_unused]] const Null& lhs,
                  [[maybe_unused]] const Null& rhs) noexcept {
    return true;
  }

  std::ostream& operator<<(std::ostream& out, const Null& value) {
    return out << value.toJsonString();
  }
} // namespace winrsls::json