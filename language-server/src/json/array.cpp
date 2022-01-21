/**
 * @file array.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of array.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "array.h"

namespace winrsls::json {
  std::string Array::toJsonString() const {
    std::string result;
    result += '[';
    for (const auto& elem : value) {
      result += elem.toJsonString();
      result += ',';
    }
    // erase last comma. [1, 2, 3,/* <- like this */]
    if (!value.empty()) {
      result.pop_back();
    }
    result += ']';
    return result;
  }

  bool operator==(const Array& lhs, const Array& rhs) {
    return lhs.value == rhs.value;
  }
} // namespace winrsls::json