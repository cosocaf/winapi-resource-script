#include "server.h"

#include <cassert>
#include <thread>

namespace winrsls {
  Server::Server(std::istream& in, std::ostream& out) : in(in), out(out) {}

#define REQUEST_HELPER(processRequestFunc)        \
  [](Server* thiz, lsp::Request&& request) {      \
    thiz->processRequestFunc(std::move(request)); \
  }

  const std::unordered_map<lsp::MessageKind, Server::ProcRequestFunc>
    Server::requestTable{
      {lsp::MessageKind::Initialize, REQUEST_HELPER(initialize)},
    };

#undef REQUEST_HELPER

#define NOTIFICATION_HELPER(processNotifictionFunc)        \
  [](Server* thiz, lsp::Notification&& notification) {     \
    thiz->processNotifictionFunc(std::move(notification)); \
  }

  const std::unordered_map<lsp::MessageKind, Server::ProcNotificationFunc>
    Server::notificationTable{
      {lsp::MessageKind::DidOpen, NOTIFICATION_HELPER(didOpen)},
      {lsp::MessageKind::DidChange, NOTIFICATION_HELPER(didChange)},
    };

#undef NOTIFICATION_HELPER

  int Server::start() {
    assert(out);
    if (!in) {
      logMessage("Input is not valid.");
      return -1;
    }

    interpreter.start();

    std::thread inputThread([this]() {
      while (true) {
        const char c = static_cast<char>(in.get());
        if (c == EOF) break;
        interpreter.push(c);
        std::this_thread::yield();
      }
    });

    std::thread outputThread([this]() {
      while (true) {
        if (auto&& request = interpreter.popRequest()) {
          processRequest(std::move(request.value()));
        }
        if (auto&& response = interpreter.popResponse()) {
          processResponse(std::move(response.value()));
        }
        if (auto&& notification = interpreter.popNotification()) {
          processNotification(std::move(notification.value()));
        }
        if (auto&& error = interpreter.popError()) {
          processError(error.value());
        }
        std::this_thread::yield();
      }
    });

    inputThread.join();
    outputThread.join();
    interpreter.stop();

    return 0;
  }
  void Server::processRequest(lsp::Request&& request) {
    logMessage("Request: " + request.method.getValue());
    const auto msg = lsp::toMessageKind(request.method);
    if (requestTable.contains(msg)) {
      requestTable.at(msg)(this, std::move(request));
    } else {
      logWarning("Unsupported request method: " + request.method.getValue());
    }
  }
  void Server::processResponse(lsp::Response&& response) {
    logMessage("Response: " + response.toJsonString());
  }
  void Server::processNotification(lsp::Notification&& notification) {
    logMessage("Notification: " + notification.toJsonString());
    const auto msg = lsp::toMessageKind(notification.method);
    if (notificationTable.contains(msg)) {
      notificationTable.at(msg)(this, std::move(notification));
    } else {
      // do nothing
    }
  }
  void Server::processError(const std::string& message) {
    logError("Error: " + message);
  }

  void Server::sendMessage(const lsp::Message& message) {
    const auto json = message.toJsonString();
    const auto length = json.size();
    writeString("Content-Length: ");
    writeString(std::to_string(length));
    writeString("\r\n\r\n");
    writeString(json);
  }
  void Server::logError(const json::String& message) { logMessage(message, 1); }
  void Server::logWarning(const json::String& message) {
    logMessage(message, 2);
  }
  void Server::logInfomation(const json::String& message) {
    logMessage(message, 3);
  }
  void Server::logMessage(const json::String& message,
                          const json::Number& type) {
    assert(type.asInt() >= 1 && type.asInt() <= 4);
    sendMessage(lsp::Notification(
      "window/logMessage", json::Object{{"type", type}, {"message", message}}));
  }

  void Server::writeString(std::string_view str) {
    out.write(str.data(), str.size());
    out.flush();
  }

  void Server::initialize(lsp::Request&& request) {
    json::Object capabilities;
    capabilities["textDocumentSync"] = 1;
    sendMessage(
      lsp::Response(request.id, json::Object{{"capabilities", capabilities}}));
  }

  void Server::didOpen(lsp::Notification&& notification) {
    auto& msg = notification.params.value().as<json::Object>();
    auto& doc = msg.at("textDocument").as<json::Object>();
    auto& uri = doc.at("uri").as<json::String>();
    auto& text = doc.at("text").as<json::String>();

    if (syntaxAnalyzers.contains(uri.getValue())) {
      logError("The already opened document \"" + uri.getValue() +
               "\" is now open.");
      syntaxAnalyzers.erase(uri.getValue());
    }

    // syntaxAnalyzers.emplace(uri.getValue(), syntax::SyntaxAnalyzer());
  }
  void Server::didChange(lsp::Notification&& notification) {
    const auto& msg = notification.params.value().as<json::Object>();
    const auto& cnt = msg.at("contentChanges").as<json::Array>();

    logMessage("Change: " + cnt.toJsonString());
    if (cnt.length() != 0) {
      const auto& doc = msg.at("textDocument").as<json::Object>();
      const auto& uri = doc.at("uri").as<json::String>();
      const auto& text = cnt.back();
    }
  }
} // namespace winrsls