#ifndef WINRSLS_LSP_MESSAGE_KIND_H_
#define WINRSLS_LSP_MESSAGE_KIND_H_

#include <string>

#include "json/string.h"

namespace winrsls::lsp {
  enum struct MessageKind {
    // General Messages
    Initialize,
    Initialized,
    Shutdown,
    Exit,
    LogTrace,
    SetTrace,

    // Window Messages
    ShowMessage,
    ShowMessageRequest,
    ShowDocument,
    LogMessage,
    ProgressCreate,
    ProgressCancel,

    // Text Document Messages
    DidOpen,
    DidChange,
    WillSave,
    DidSave,
    DidClose,
  };

  std::string fromMessageKind(MessageKind kind);
  MessageKind toMessageKind(const std::string& message);
  MessageKind toMessageKind(const json::String& message);
} // namespace winrsls::lsp

#endif // WINRSLS_LSP_MESSAGE_KIND_H_