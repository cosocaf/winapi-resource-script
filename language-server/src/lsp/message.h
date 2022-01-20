#ifndef WINRSLS_LSP_MESSAGE_H_
#define WINRSLS_LSP_MESSAGE_H_

#include <optional>

#include "json/json.h"

namespace winrsls::lsp {
  struct Message {
    // Always "2.0"
    json::String jsonrpc = "2.0";
    std::string toJsonString() const;
    virtual json::Element toJsonElement() const = 0;
    virtual ~Message();
  };

  struct Request : public Message {
    json::Union<json::Number, json::String> id;
    json::String method;
    std::optional<json::Union<json::Array, json::Object>> params;
    Request(
      const json::Union<json::Number, json::String>& id,
      const json::String& method,
      const std::optional<json::Union<json::Array, json::Object>>& params);
    Request(json::Union<json::Number, json::String>&& id, json::String&& method,
            std::optional<json::Union<json::Array, json::Object>>&& params);
    virtual json::Element toJsonElement() const override;
  };

  struct Notification : public Message {
    /**
     * If it is a cancellation notification,
     * it should be "$/cancel",
     * if it is a progress notification,
     * it should be "$/progress".
     */
    json::String method;
    std::optional<json::Union<json::Array, json::Object>> params;

    Notification(json::String method,
                 std::optional<json::Union<json::Array, json::Object>> params =
                   std::nullopt);
    virtual json::Element toJsonElement() const override;
  };

  enum struct ErrorCode : int {
    // Defined by JSON RPC
    ParseError = -32700,
    InvalidRequest = -32600,
    MethodNotFound = -32601,
    InvalidParams = -32602,
    InternalError = -32603,
    ServerErrorStart = -32099,
    ServerErrorEnd = -32000,
    ServerNotInitialized = -32002,
    UnknownErrorCode = -32001,

    // Defined by LSP
    RequestCancelled = -32800,
    ContentModified = -32801,
  };
  struct ResponseError {
    json::Number code;
    json::String message;
    std::optional<json::Element> data;
    ResponseError(const json::Number& code, const json::String& message,
                  const std::optional<json::Element>& data = std::nullopt);
    json::Element toJsonElement() const;
  };
  struct Response : public Message {
    json::Union<json::Number, json::String, json::Null> id;
    std::optional<json::Union<json::String, json::Number, json::Boolean,
                              json::Object, json::Null>>
      result;
    std::optional<ResponseError> error;
    Response(
      const json::Union<json::Number, json::String, json::Null>& id,
      const json::Union<json::String, json::Number, json::Boolean, json::Object,
                        json::Null>& result = json::Null());
    Response(const json::Union<json::Number, json::String, json::Null>& id,
             const std::optional<ResponseError>& error);
    virtual json::Element toJsonElement() const override;
  };
} // namespace winrsls::lsp

#endif // WINRSLS_LSP_MESSAGE_H_