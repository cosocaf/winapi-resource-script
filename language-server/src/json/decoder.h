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

#include "json.h"
#include "result.h"

namespace winrsls::json {
  class Decoder {
    std::string_view data;
    size_t idx;
    static const std::unordered_set<char> whitespaces;

  public:
    Decoder(std::string_view data);
    Result<Object, std::string> decode();

  private:
    Result<String, std::string> parseString();
    Result<Number, std::string> parseNumber();
    Result<Boolean, std::string> parseBoolean();
    Result<Null, std::string> parseNull();
    Result<Array, std::string> parseArray();
    Result<Object, std::string> parseObject();
    Result<Element, std::string> parseElement();

    char cur();
    char next();
    std::optional<std::string_view> curWord();
  };
} // namespace winrsls::json

#endif // WINRSLS_JSON_DECODER_H_