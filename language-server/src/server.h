#ifndef WINRSLS_SERVER_H_
#define WINRSLS_SERVER_H_

#include <istream>
#include <ostream>

#include "interpreter.h"
#include "lsp/message.h"

namespace winrsls {
  class Server {
    std::istream& in;
    std::ostream& out;

    Interpreter interpreter;

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
    void logMessage(const json::String& message);

  private:
    void writeString(std::string_view str);
    void processRequest(const lsp::Request& request);
    void processResponse(const lsp::Response& response);
    void processNotification(const lsp::Notification& notification);
    void processError(const std::string& message);
  };
}; // namespace winrsls

#endif // WINRSLS_SERVER_H_