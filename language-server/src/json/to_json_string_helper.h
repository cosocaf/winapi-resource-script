/**
 * @file to_json_string_helper.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines helper macro functions to assist in the implementation of
 * toJsonString for object-like JSON elements.
 * It will not work with older MSVC preprocessors. Please add the
 * /Zc:preprocessor option.
 * @version 0.1
 * @date 2022-01-27
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_TO_JSON_STRING_HELPER_H_
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_

#include <optional>
#include <string>

#include "element_like.h"

#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_REVERSE_SEQ()                    \
  32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, \
    13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_COUNT_VA_ARGS_IMPL(             \
  a01, a02, a03, a04, a05, a06, a07, a08, a09, a10, a11, a12, a13, a14, a15, \
  a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, \
  a31, a32, N, ...)                                                          \
  N
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_COUNT_VA_ARGS_AUX(...) \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_COUNT_VA_ARGS_IMPL(__VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_COUNT_VA_ARGS(...) \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_COUNT_VA_ARGS_AUX(       \
    __VA_ARGS__ __VA_OPT__(, )                                  \
      WINRSLS_JSON_TO_JSON_STRING_HELPER_H_REVERSE_SEQ())

#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_CONCAT_AUX(a, b) a##b
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_CONCAT(a, b) \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_CONCAT_AUX(a, b)

#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_1(callback, \
                                                                    a1)       \
  callback(a1);
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_2(callback, \
                                                                    a1, ...)  \
  callback(a1);                                                               \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_1(callback,       \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_3(callback, \
                                                                    a1, ...)  \
  callback(a1);                                                               \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_2(callback,       \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_4(callback, \
                                                                    a1, ...)  \
  callback(a1);                                                               \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_3(callback,       \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_5(callback, \
                                                                    a1, ...)  \
  callback(a1);                                                               \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_4(callback,       \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_6(callback, \
                                                                    a1, ...)  \
  callback(a1);                                                               \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_5(callback,       \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_7(callback, \
                                                                    a1, ...)  \
  callback(a1);                                                               \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_6(callback,       \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_8(callback, \
                                                                    a1, ...)  \
  callback(a1);                                                               \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_7(callback,       \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_9(callback, \
                                                                    a1, ...)  \
  callback(a1);                                                               \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_8(callback,       \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_10(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_9(callback,        \
                                                              __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_11(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_10(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_12(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_11(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_13(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_12(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_14(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_13(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_15(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_14(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_16(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_15(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_17(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_16(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_18(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_17(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_19(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_18(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_20(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_19(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_21(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_20(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_22(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_21(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_23(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_22(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_24(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_23(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_25(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_24(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_26(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_25(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_27(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_26(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_28(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_27(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_29(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_28(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_30(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_29(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_31(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_30(callback,       \
                                                               __VA_ARGS__)
#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_32(callback, \
                                                                     a1, ...)  \
  callback(a1);                                                                \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_32(callback,       \
                                                               __VA_ARGS__)

#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS(callback, ...) \
  WINRSLS_JSON_TO_JSON_STRING_HELPER_H_CONCAT(                               \
    WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS_AUX_,              \
    WINRSLS_JSON_TO_JSON_STRING_HELPER_H_COUNT_VA_ARGS(__VA_ARGS__))         \
  (callback, __VA_ARGS__)

#define WINRSLS_JSON_TO_JSON_STRING_HELPER_H_TO_JSON_STRING_AUX(name) \
  ::winrsls::json::internal::toJsonString(result, "\"" #name "\":", name)

#define CREATE_TO_JSON_STRING_HELPER(...)                                    \
  [this](std::string& result) {                                              \
    result += '{';                                                           \
    WINRSLS_JSON_TO_JSON_STRING_HELPER_H_FOR_EACH_VA_ARGS(                   \
      WINRSLS_JSON_TO_JSON_STRING_HELPER_H_TO_JSON_STRING_AUX, __VA_ARGS__); \
    if constexpr (WINRSLS_JSON_TO_JSON_STRING_HELPER_H_COUNT_VA_ARGS(        \
                    __VA_ARGS__) > 0) {                                      \
      result.pop_back();                                                     \
    }                                                                        \
    result += '}';                                                           \
  }

namespace winrsls::json::internal {
  template <element_like T, size_t N>
  void toJsonString(std::string& output, const char (&name)[N],
                    const T& value) {
    output.append(std::begin(name), std::end(name));
    output += value.toJsonString();
    output += ',';
  }
  template <element_like T, size_t N>
  void toJsonString(std::string& output, const char (&name)[N],
                    const std::optional<T>& value) {
    if (value) {
      toJsonString(output, name, value.value());
    }
  }

} // namespace winrsls::json::internal

#endif // WINRSLS_JSON_TO_JSON_STRING_HELPER_H_