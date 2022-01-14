#include "interpreter.h"

#include <charconv>

#include "json/decoder.h"
#include "str_utils.h"

namespace winrsls {
  Interpreter::Interpreter() : part(LSPPart::Header) {}
  void Interpreter::start() {
    stop();
    interpreterThread = std::thread([this]() {
      while (true) {
        interpret();
        std::this_thread::yield();
      }
    });
  }
  void Interpreter::stop() {
    if (interpreterThread.joinable()) {
      interpreterThread.join();
    }
  }
  void Interpreter::interpret() {
    switch (part) {
      case LSPPart::Header: interpretHeader(); break;
      case LSPPart::Content: interpretContent(); break;
    }
  }
  void Interpreter::interpretHeader() {
    const auto& headers = loadHeaderPart();
    for (const auto& head : headers) {
      const auto kv = split(head, ": ");
      if (kv.size() != 2) {
        pushError("Header fields are not separated by ': '.");
        continue;
      }
      const auto key = kv[0];
      const auto val = kv[1];
      header[std::string(key)] = val;
    }
    if (const auto&& error = validateHeader()) {
      pushError(std::move(error.value()));
    }
    switchPart();
  }
  std::vector<std::string> Interpreter::loadHeaderPart() {
    std::vector<std::string> headers;
    while (true) {
      buffer += inputQueue.pop_wait();
      if (buffer == "\r\n") {
        buffer.clear();
        break;
      } else if (buffer.ends_with("\r\n")) {
        buffer.pop_back();
        buffer.pop_back();
        headers.push_back(std::move(buffer));
        buffer = std::string();
      }
    }
    return headers;
  }
  std::optional<std::string> Interpreter::validateHeader() {
    if (header.empty() || !header.contains("Content-Length")) {
      return "Content-Length is missing.";
    }
    for (const auto c : header.at("Content-Length")) {
      if (c < '0' || c > '9') {
        return "Content-Length is not a number.";
      }
    }
    if (header.contains("Content-Type")) {
      const auto& type = header.at("Content-Type");
      const auto directive = split(type, ";");
      const auto mime = trim(directive[0]);
      if (mime.find("json") == std::string_view::npos) {
        return "Content-Type must be in JSON format.";
      }
      for (const auto& elem : directive) {
        const auto dir = trim(elem);
        if (dir.starts_with("charset=")) {
          const auto charset = dir.substr(8);
          if (charset != "utf-8" && charset != "utf8") {
            return "This server supports UTF-8 only.";
          }
        }
      }
    }

    return std::nullopt;
  }

  void Interpreter::interpretContent() {
    const auto& cntLenStr = header.at("Content-Length");
    size_t cntLen;
    std::from_chars(cntLenStr.c_str(), cntLenStr.c_str() + cntLenStr.size(),
                    cntLen);

    loadContentPart(cntLen);

    json::Decoder decoder(buffer);
    const auto res = decoder.decode();
    if (res) {
      const auto msg = res.get();
      if (msg.contains("id") && msg.contains("method")) {
        if (!msg.contains_as<json::Number, json::String>("id") ||
            !msg.contains_as<json::String>("method")) {
          pushError("Invalid Request Response");
        } else {
          pushRequest(msg);
        }
      } else if (msg.contains("id")) {
        if (!msg.contains_as<json::Number, json::String>("id")) {
          pushError("Invalid Request Response");
        } else {
          pushResponse(msg);
        }
      } else if (msg.contains("method")) {
        if (!msg.contains_as<json::String>("method")) {
          pushError("Invalid Request Response");
        } else {
          pushNotification(msg);
        }
      } else {
        pushError("Invalid Request Response");
      }
    } else {
      pushError(res.err());
    }
    switchPart();
  }
  void Interpreter::loadContentPart(size_t contentLength) {
    while (buffer.size() < contentLength) {
      buffer += inputQueue.pop_wait();
    }
  }

  void Interpreter::switchPart() {
    buffer.clear();
    if (part == LSPPart::Header) {
      part = LSPPart::Content;
      cleanupContent();
    } else {
      part = LSPPart::Header;
      cleanupHeader();
    }
  }
  void Interpreter::cleanupHeader() { header.clear(); }
  void Interpreter::cleanupContent() {}
  void Interpreter::push(char data) { inputQueue.push(data); }

  std::optional<lsp::Request> Interpreter::popRequest() {
    return requestQueue.pop();
  }
  std::optional<lsp::Response> Interpreter::popResponse() {
    return responseQueue.pop();
  }
  std::optional<lsp::Notification> Interpreter::popNotification() {
    return notificationQueue.pop();
  }
  std::optional<std::string> Interpreter::popError() {
    return errorQueue.pop();
  }

  void Interpreter::pushRequest(const json::Object& request) {
    using params_t = json::Union<json::Array, json::Object>;
    std::optional<params_t> params;
    if (request.contains("params")) {
      params = request.at("params").as_union<json::Array, json::Object>();
    }
    requestQueue.push(
      lsp::Request(request.at("id").as_union<json::Number, json::String>(),
                   request.at("method").as<json::String>(), std::move(params)));
  }
  void Interpreter::pushResponse(const json::Object& response) {
    if (response.contains("result")) {
      responseQueue.push(lsp::Response(
        response.at("id").as_union<json::Number, json::String, json::Null>(),
        response.at("result")
          .as_union<json::String, json::Number, json::Boolean, json::Object,
                    json::Null>()));
    } else if (response.contains("error")) {
      const auto& error = response.at("error").as<json::Object>();

      std::optional<json::Element> data;
      if (error.contains("data")) {
        data = error.at("data");
      }

      lsp::ResponseError resError(error.at("code").as<json::Number>(),
                                  error.at("message").as<json::String>(), data);

      responseQueue.push(lsp::Response(
        response.at("id").as_union<json::Number, json::String, json::Null>(),
        resError));
    }
  }
  void Interpreter::pushNotification(const json::Object& notification) {
    using params_t = json::Union<json::Array, json::Object>;
    std::optional<params_t> params;
    if (notification.contains("params")) {
      params = notification.at("params").as_union<json::Array, json::Object>();
    }
    notificationQueue.push(lsp::Notification(
      notification.at("method").as<json::String>(), std::move(params)));
  }
  void Interpreter::pushError(const std::string& message) {
    errorQueue.push(message);
  }
} // namespace winrsls