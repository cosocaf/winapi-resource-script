/**
 * @file decoder.cpp
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Implementation of decoder.h
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#include "decoder.h"

#include <cassert>
#include <charconv>

namespace winrsls::json {
  Decoder::Decoder(std::string_view data) : data(data), idx(0) {}

  const std::unordered_set<char> Decoder::whitespaces = {
    '\t',
    '\r',
    '\n',
    ' ',
  };

  Result<Object, std::string> Decoder::decode() {
    using namespace std::string_literals;

    auto&& result = parseElement();
    if (!result) return error(std::move(result.err()));
    if (!result.get().is<Object>()) {
      return error("Invalid Content: Failed to decode JSON."s);
    }

    return ok(std::move(result.get().as<Object>()));
  }
  Result<Element, std::string> Decoder::parseElement() {
    using namespace std::string_literals;
    const auto c = next();
    switch (c) {
      case '{': {
        auto&& res = parseObject();
        if (!res) return error(std::move(res.err()));
        return ok(Element(std::move(res.get())));
      }
      case '[': {
        auto&& res = parseArray();
        if (!res) return error(std::move(res.err()));
        return ok(Element(std::move(res.get())));
      }
      case '"': {
        auto&& res = parseString();
        if (!res) return error(std::move(res.err()));
        return ok(Element(std::move(res.get())));
      }
      case 't':
      case 'f': {
        auto&& res = parseBoolean();
        if (!res) return error(std::move(res.err()));
        return ok(Element(std::move(res.get())));
      }
      case 'n': {
        auto&& res = parseNull();
        if (!res) return error(std::move(res.err()));
        return ok(Element(std::move(res.get())));
      }
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '-': {
        auto&& res = parseNumber();
        if (!res) return error(std::move(res.err()));
        return ok(Element(std::move(res.get())));
      }
      case '\0': {
        return error(
          "Invalid Content: Failed to decode JSON. Unexpected end."s);
      }
      default: {
        return error(
          "Invalid Content: Failed to decode JSON. Unexpected token: "s + c);
      }
    }
  }
  Result<Object, std::string> Decoder::parseObject() {
    using namespace std::string_literals;
    assert(cur() == '{');

    auto c = next();

    // empty object.
    if (c == '}') {
      return ok(Object());
    }

    std::unordered_map<String, Element> kv;
    while (true) {
      if (c == '\0') {
        return error(
          "Invalid Content: Failed to decode JSON. Unexpected end."s);
      }
      if (c != '"') {
        return error(
          "Invalid Content: Failed to decode JSON. Unexpected token: "s + c);
      }
      auto&& key = parseString();
      if (!key) return error(std::move(key.err()));

      c = next();
      if (c != ':') {
        return error(
          "Invalid Content: Failed to decode JSON. Unexpected token: "s + c);
      }
      auto&& val = parseElement();
      if (!val) return error(std::move(val.err()));

      kv[key.get()] = std::move(val.get());

      c = next();
      if (c == ',') {
        c = next();
      } else if (c == '}') {
        break;
      } else {
        return error(
          "Invalid Content: Failed to decode JSON. Unexpected token: "s + c);
      }
    }

    return ok(Object(std::move(kv)));
  }
  Result<Array, std::string> Decoder::parseArray() {
    using namespace std::string_literals;
    assert(cur() == '[');

    auto c = next();

    // empty array.
    if (c == ']') {
      return ok(Array());
    }

    std::vector<Element> elems;
    while (true) {
      if (c == '\0') {
        return error(
          "Invalid Content: Failed to decode JSON. Unexpected end."s);
      }
      --idx;
      auto&& elem = parseElement();
      if (!elem) return error(std::move(elem.err()));
      elems.push_back(std::move(elem.get()));

      c = next();
      if (c == ',') {
        c = next();
      } else if (c == ']') {
        break;
      } else {
        return error(
          "Invalid Content: Failed to decode JSON. Unexpected token: "s + c);
      }
    }

    return ok(Array(std::move(elems)));
  }
  Result<String, std::string> Decoder::parseString() {
    using namespace std::string_literals;
    assert(cur() == '"');

    std::string str;
    while (idx < data.size()) {
      auto c = data[idx++];
      if (c == '\\') {
        c = data[idx++];
        switch (c) {
          case '"': str += '"'; break;
          case '\\': str += '\\'; break;
          case '/': str += '/'; break;
          case 'b': str += '\b'; break;
          case 'f': str += '\f'; break;
          case 'n': str += '\n'; break;
          case 'r': str += '\r'; break;
          case 't': str += '\t'; break;
          case 'u': {
            size_t code = 0;
            for (int i = 0; i < 4; ++i) {
              code <<= 4;
              c = next();
              if (c >= '0' && c <= '9') {
                code |= size_t(c - '0');
              } else if (c >= 'A' && c <= 'F') {
                code |= size_t(10 + c - 'A');
              } else if (c >= 'a' && c <= 'f') {
                code |= size_t(10 + c - 'a');
              } else {
                return error(
                  "Invalid Content: Failed to decode JSON. Unexpected token: "s +
                  c);
              }
            }
            // ASCII characters
            if (code <= 0x7F) {
              str += static_cast<char>(code);
            }
            // 11bit characters
            else if (code <= 0x07FF) {
              const auto upper = code >> 6;
              const auto lower = code & 0x3F;
              str += static_cast<char>(0xC0 | upper);
              str += static_cast<char>(0x80 | lower);
            }
            // 16 characters
            else {
              const auto upper = code >> 12;
              const auto middle = (code >> 6) & 0x3F;
              const auto lower = code & 0x3F;
              str += static_cast<char>(0xE0 | upper);
              str += static_cast<char>(0x80 | middle);
              str += static_cast<char>(0x80 | lower);
            }
          }
          case '\0':
            return error(
              "Invalid Content: Failed to decode JSON. Unexpected end."s);
          default:
            return error(
              "Invalid Content: Failed to decode JSON. Unexpected token: "s +
              c);
        }
      } else if (c == '"') {
        return ok(String(std::move(str)));
      } else {
        str += c;
      }
    }

    return error("Invalid Content: Failed to decode JSON. Unexpected end."s);
  }
  Result<Number, std::string> Decoder::parseNumber() {
    using namespace std::string_literals;
    assert((cur() >= '0' && cur() <= '9') || cur() == '-');

    const auto& res = curWord();
    if (!res) {
      return error("Invalid Content: Failed to decode JSON. Unexpected end."s);
    }

    const auto& word = res.value();

    const auto getErrorMessage = [](const std::errc& ec) {
      switch (ec) {
        case std::errc::invalid_argument:
          return error(
            "Invalid Content: Failed to decode JSON. Invalid Argument."s);
        case std::errc::result_out_of_range:
          return error(
            "Invalid Content: Failed to decode JSON. Result Out Of Range."s);
        default:
          return error(
            "Internal Error: Failed to decode JSON. Unknown POSIX Error Code: "s +
            std::to_string(static_cast<int>(ec)));
      }
    };

    int64_t integer;
    if (const auto [ptr, ec] =
          std::from_chars(word.data(), word.data() + word.size(), integer);
        static_cast<int>(ec) == 0) {
      if (ptr == word.data() + word.size()) {
        return ok(Number(integer));
      }
    } else {
      return getErrorMessage(ec);
    }

    double_t decimal;
    if (const auto [ptr, ec] =
          std::from_chars(word.data(), word.data() + word.size(), decimal);
        static_cast<int>(ec) == 0) {
      if (ptr == word.data() + word.size()) {
        return ok(Number(decimal));
      }
    } else {
      return getErrorMessage(ec);
    }

    return error(
      "Invalid Content: Failed to decode JSON. Invalid number format."s);
  }
  Result<Boolean, std::string> Decoder::parseBoolean() {
    using namespace std::string_literals;

    const auto res = curWord();
    if (!res) {
      return error("Invalid Content: Failed to decode JSON. Unexpected end."s);
    }

    if (res.value() == "true") {
      return ok(Boolean(true));
    } else if (res.value() == "false") {
      return ok(Boolean(false));
    }
    return error("Invalid Content: Failed to decode JSON. Unexpected token: "s +
                 std::string(res.value()));
  }
  Result<Null, std::string> Decoder::parseNull() {
    using namespace std::string_literals;

    const auto res = curWord();
    if (!res) {
      return error("Invalid Content: Failed to decode JSON. Unexpected end."s);
    }

    if (res.value() == "null") {
      return ok(Null());
    }
    return error("Invalid Content: Failed to decode JSON. Unexpected token: "s +
                 std::string(res.value()));
  }
  char Decoder::cur() {
    if (idx == 0) return '\0';
    return data[idx - 1];
  }
  char Decoder::next() {
    while (idx < data.size()) {
      if (whitespaces.contains(data[idx])) {
        ++idx;
      } else {
        return data[idx++];
      }
    }
    return '\0';
  }
  std::optional<std::string_view> Decoder::curWord() {
    const auto c = cur();
    if (c == '\0') {
      return std::nullopt;
    }
    --idx;
    size_t cnt = 1;
    while (idx + cnt < data.size()) {
      if (whitespaces.contains(data[idx + cnt]) || data[idx + cnt] == ',' ||
          data[idx + cnt] == '}' || data[idx + cnt] == ']') {
        break;
      }
      ++cnt;
    }
    const auto word = data.substr(idx, cnt);
    idx += cnt;
    return word;
  }
} // namespace winrsls::json