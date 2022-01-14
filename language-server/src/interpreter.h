#ifndef WINRSLS_INTERPRETER_H_
#define WINRSLS_INTERPRETER_H_

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>

#include "json/object.h"
#include "lsp/message.h"

namespace winrsls {
  enum struct LSPPart { Header, Content };

  template <typename T>
  class InterpreterQueue {
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<T> que;

  public:
    void push(const T& val) {
      std::lock_guard<std::mutex> lock(mtx);
      const auto empty = que.empty();
      que.push(val);
      if (empty) {
        cv.notify_all();
      }
    }
    void push(T&& val) {
      std::lock_guard<std::mutex> lock(mtx);
      const auto empty = que.empty();
      que.push(std::move(val));
      if (empty) {
        cv.notify_all();
      }
    }
    std::optional<T> pop() {
      std::lock_guard<std::mutex> lock(mtx);
      if (que.empty()) {
        return std::nullopt;
      }
      const auto result = std::move(que.front());
      que.pop();
      return result;
    }
    T pop_wait() {
      while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        if (que.empty()) {
          cv.wait(lock);
        }
        if (que.empty()) {
          continue;
        }

        const auto result = std::move(que.front());
        que.pop();
        return result;
      }
    }
  };

  class Interpreter {
    InterpreterQueue<char> inputQueue;
    InterpreterQueue<lsp::Request> requestQueue;
    InterpreterQueue<lsp::Response> responseQueue;
    InterpreterQueue<lsp::Notification> notificationQueue;
    InterpreterQueue<std::string> errorQueue;
    std::string buffer;
    LSPPart part;
    std::unordered_map<std::string, std::string> header;
    std::thread interpreterThread;

  public:
    Interpreter();
    void push(char data);
    std::optional<lsp::Request> popRequest();
    std::optional<lsp::Response> popResponse();
    std::optional<lsp::Notification> popNotification();
    std::optional<std::string> popError();
    void start();
    void stop();

  private:
    void interpret();
    void interpretHeader();
    /**
     * @brief Load a header part.
     *
     * This function locks the inputMutex.
     *
     * @param headers output
     * @result Header fields
     */
    std::vector<std::string> loadHeaderPart();
    /**
     * @brief Validate a header part.
     *
     * @return std::optional<std::string> If it is an invalid header, it will
     * return the reason, otherwise it will return nullopt.
     */
    std::optional<std::string> validateHeader();
    void interpretContent();
    /**
     * @brief Load a content part.
     *
     * This function locks the inputMutex.
     * The read value will be buffered in the buffer.
     *
     * @param contentLength Length of bytes to load
     */
    void loadContentPart(size_t contentLength);
    void switchPart();
    void cleanupHeader();
    void cleanupContent();
    void pushRequest(const json::Object& request);
    void pushResponse(const json::Object& response);
    void pushNotification(const json::Object& notification);
    void pushError(const std::string& message);
  };
} // namespace winrsls

#endif // WINRSLS_INTERPRETER_H_