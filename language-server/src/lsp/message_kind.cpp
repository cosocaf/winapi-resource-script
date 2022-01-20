#include "message_kind.h"

#include <unordered_map>

namespace winrsls::lsp {
  namespace {
    std::unordered_map<MessageKind, std::string> msgToStrTable{
      {MessageKind::Initialize, "initialize"},
      {MessageKind::Initialized, "initialized"},
      {MessageKind::Shutdown, "shutdown"},
      {MessageKind::Exit, "exit"},
      {MessageKind::LogTrace, "logTrace"},
      {MessageKind::SetTrace, "setTrace"},

      {MessageKind::ShowMessage, "window/showMessage"},
      {MessageKind::ShowMessageRequest, "window/showMessageRequest"},
      {MessageKind::ShowDocument, "window/showDocument"},
      {MessageKind::LogMessage, "window/logMessage"},
      {MessageKind::ProgressCreate, "window/workDownProgress/create"},
      {MessageKind::ProgressCancel, "window/workDownProgress/cancel"},

      {MessageKind::DidOpen, "textDocument/didOpen"},
      {MessageKind::DidChange, "textDocument/didChange"},
      {MessageKind::WillSave, "textDocument/willSave"},
      {MessageKind::DidSave, "textDocument/didSave"},
      {MessageKind::DidClose, "textDocument/didClose"}};

    std::unordered_map<std::string, MessageKind> strToMsgTable{
      {"initialize", MessageKind::Initialize},
      {"initialized", MessageKind::Initialized},
      {"shutdown", MessageKind::Shutdown},
      {"exit", MessageKind::Exit},
      {"logTrace", MessageKind::LogTrace},
      {"setTrace", MessageKind::SetTrace},

      {"window/showMessage", MessageKind::ShowMessage},
      {"window/showMessageRequest", MessageKind::ShowMessageRequest},
      {"window/showDocument", MessageKind::ShowDocument},
      {"window/logMessage", MessageKind::LogMessage},
      {"window/workDownProgress/create", MessageKind::ProgressCreate},
      {"window/workDownProgress/cancel", MessageKind::ProgressCancel},

      {"textDocument/didOpen", MessageKind::DidOpen},
      {"textDocument/didChange", MessageKind::DidChange},
      {"textDocument/willSave", MessageKind::WillSave},
      {"textDocument/didSave", MessageKind::DidSave},
      {"textDocument/didClose", MessageKind::DidClose}};
  } // namespace
  std::string fromMessageKind(MessageKind kind) {
    return msgToStrTable.at(kind);
  }
  MessageKind toMessageKind(const std::string& message) {
    return strToMsgTable.at(message);
  }
  MessageKind toMessageKind(const json::String& message) {
    return toMessageKind(message.getValue());
  }
} // namespace winrsls::lsp