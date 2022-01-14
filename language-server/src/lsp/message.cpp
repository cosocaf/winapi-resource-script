#include "message.h"

#include <cassert>

namespace winrsls::lsp {
  Message::~Message() {}
  std::string Message::toJsonString() const {
    return toJsonElement().toJsonString();
  }

  Request::Request(
    const json::Union<json::Number, json::String>& id,
    const json::String& method,
    const std::optional<json::Union<json::Array, json::Object>>& params)
    : id(id), method(method), params(params) {}
  Request::Request(
    json::Union<json::Number, json::String>&& id, json::String&& method,
    std::optional<json::Union<json::Array, json::Object>>&& params)
    : id(std::move(id)), method(std::move(method)), params(std::move(params)) {}

  json::Element Request::toJsonElement() const {
    json::Object json;
    json["jsonrpc"] = jsonrpc;
    json["id"] = id;
    if (params) {
      json["params"] = params.value();
    }
    return json;
  }

  Notification::Notification(
    json::String method,
    std::optional<json::Union<json::Array, json::Object>> params)
    : method(method), params(params) {}
  json::Element Notification::toJsonElement() const {
    json::Object json;
    json["jsonrpc"] = jsonrpc;
    json["method"] = method;
    if (params) {
      json["params"] = params.value();
    }
    return json;
  }

  Response::Response(
    const json::Union<json::Number, json::String, json::Null>& id,
    const json::Union<json::String, json::Number, json::Boolean, json::Object,
                      json::Null>& result)
    : id(id), result(result) {}
  Response::Response(
    const json::Union<json::Number, json::String, json::Null>& id,
    const std::optional<ResponseError>& error)
    : id(id), error(error) {}
  json::Element Response::toJsonElement() const {
    json::Object json;
    json["jsonrpc"] = jsonrpc;
    json["id"] = id;
    if (result) {
      assert(!error);
      json["result"] = result.value();
    } else {
      assert(error);
      json["error"] = error.value().toJsonElement();
    }
    return json;
  }

  ResponseError::ResponseError(const json::Number& code,
                               const json::String& message,
                               const std::optional<json::Element>& data)
    : code(code), message(message), data(data) {}
  json::Element ResponseError::toJsonElement() const {
    json::Object json;
    json["code"] = code;
    json["message"] = message;
    if (data) {
      json["data"] = data.value();
    }
    return json;
  }
} // namespace winrsls::lsp