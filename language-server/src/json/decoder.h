/**
 * @file decoder.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Define a class to decode JSON.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_DECODER_H_
#define WINRSLS_JSON_DECODER_H_

#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>

#include "error_info.h"
#include "json.h"
#include "result.h"

namespace winrsls::json {
  class Decoder {
    std::string_view data;
    size_t idx;
    static const std::unordered_set<char> whitespaces;

  public:
    Decoder(std::string_view data);
    Result<Object, ErrorInfo> decode();

  private:
    Result<String, ErrorInfo> parseString();
    Result<Number, ErrorInfo> parseNumber();
    Result<Boolean, ErrorInfo> parseBoolean();
    Result<Null, ErrorInfo> parseNull();
    Result<Array, ErrorInfo> parseArray();
    Result<Object, ErrorInfo> parseObject();
    Result<Element, ErrorInfo> parseElement();

    char cur();
    char next();
    std::optional<std::string_view> curWord();
  };
} // namespace winrsls::json

#endif // WINRSLS_JSON_DECODER_H_