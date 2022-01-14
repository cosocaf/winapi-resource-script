#include "server.h"

#include <cassert>
#include <thread>

#include "variant_cast.h"

namespace winrsls {
  Server::Server(std::istream& in, std::ostream& out) : in(in), out(out) {}

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
        if (const auto request = interpreter.popRequest()) {
          processRequest(request.value());
        }
        if (const auto response = interpreter.popResponse()) {
          processResponse(response.value());
        }
        if (const auto notification = interpreter.popNotification()) {
          processNotification(notification.value());
        }
        if (const auto error = interpreter.popError()) {
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

  void Server::processRequest(const lsp::Request& request) {
    if (request.method == "initialize") {
      sendMessage(lsp::Response(
        request.id, json::Object{{"capabilities", json::Object()}}));
    } else {
      logMessage("Unknown request: " + request.method.toJsonString());
    }
  }
  void Server::processResponse(const lsp::Response& response) {}
  void Server::processNotification(const lsp::Notification& notification) {
    if (notification.method == "initialized") {
      logMessage("initialized!");
    }
  }
  void Server::processError(const std::string& message) {
    logMessage("Error: " + message);
  }

  void Server::sendMessage(const lsp::Message& message) {
    const auto json = message.toJsonString();
    const auto length = json.size();
    writeString("Content-Length: ");
    writeString(std::to_string(length));
    writeString("\r\n\r\n");
    writeString(json);
  }
  void Server::logMessage(const json::String& message) {
    sendMessage(lsp::Notification(
      "window/logMessage",
      json::Object{{"type", json::Number(3)}, {"message", message}}));
  }

  void Server::writeString(std::string_view str) {
    out.write(str.data(), str.size());
    out.flush();
  }

} // namespace winrsls