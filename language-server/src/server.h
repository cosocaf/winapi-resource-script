#ifndef WINRSLS_SERVER_H_
#define WINRSLS_SERVER_H_

#include <functional>
#include <istream>
#include <ostream>
#include <unordered_map>

#include "interpreter.h"
#include "lsp/message.h"
#include "lsp/message_kind.h"
#include "syntax/syntax_analyzer.h"

namespace winrsls {
  class Server {
    std::istream& in;
    std::ostream& out;

    Interpreter interpreter;
    std::unordered_map<std::string, syntax::SyntaxAnalyzer> syntaxAnalyzers;

    using ProcRequestFunc = std::function<void(Server*, lsp::Request&&)>;
    using ProcNotificationFunc =
      std::function<void(Server*, lsp::Notification&&)>;

    static const std::unordered_map<lsp::MessageKind, ProcRequestFunc>
      requestTable;
    static const std::unordered_map<lsp::MessageKind, ProcNotificationFunc>
      notificationTable;

  public:
    /**
     * @brief Construct a new Server object.
     *
     * out must be normal throughout.
     *
     * @param in input stream
     * @param out output stream
     */
    Server(std::istream& in, std::ostream& out);
    /**
     * @brief Start this server.
     *
     * @return int result
     */
    int start();
    /**
     * @brief Send a message.
     *
     * @param message
     */
    void sendMessage(const lsp::Message& message);
    void logError(const json::String& message);
    void logWarning(const json::String& message);
    void logInfomation(const json::String& message);
    void logMessage(const json::String& message, const json::Number& type = 4);

  private:
    void writeString(std::string_view str);
    void processRequest(lsp::Request&& request);
    void processResponse(lsp::Response&& response);
    void processNotification(lsp::Notification&& notification);
    void processError(const std::string& message);

    // requests

    void initialize(lsp::Request&& request);

    // responses

    // notifications

    void didOpen(lsp::Notification&& notification);
    void didChange(lsp::Notification&& notification);
  };
}; // namespace winrsls

#endif // WINRSLS_SERVER_H_