/**
 * @file element_like.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines the concept for considering a type as an element of JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_ELEMENT_LIKE_H_
#define WINRSLS_JSON_ELEMENT_LIKE_H_

#include <concepts>
#include <string>

namespace winrsls::json {
  class Element;
  template <typename T>
  concept element_like = requires(T element) {
    { element.toJsonString() } -> std::same_as<std::string>;
  };
} // namespace winrsls::json

#endif // WINRSLS_JSON_ELEMENT_LIKE_H_