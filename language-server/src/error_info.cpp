#include "error_info.h"

namespace winrsls {
  ErrorInfo::ErrorInfo(const std::string& msg) : msg(msg) {}
  ErrorInfo::ErrorInfo(std::string&& msg) : msg(std::move(msg)) {}
  ErrorInfo::ErrorInfo(std::string_view msg) : msg(msg) {}
  ErrorInfo::ErrorInfo(const char* msg) : msg(msg) {}

  const std::string& ErrorInfo::getMessage() const { return msg; }
  std::string& ErrorInfo::getMessage() { return msg; }
} // namespace winrsls

#ifndef NDEBUG

#include <iomanip>
#include <sstream>

namespace winrsls::debug {
  std::string generateErrorMessage(const std::string& message,
                                   const char* const file,
                                   const char* const func, const int line) {
    std::string result = "[ERROR] ";
    result += message;
    result += " @ ";
    result += func;
    result += '[';
    result += file;
    result += ':';
    result += std::to_string(line);
    result += "]\n";

    result += "[STACKTRACE]\n";

    const auto stackTrace = getStackTrace();
    const auto numDigit = std::to_string(stackTrace.traces.size() - 1).size();

    // Skip getStackTrace, this function.
    for (size_t i = 2, l = stackTrace.traces.size(); i < l; ++i) {
      std::stringstream stream;
      const auto digit = std::to_string(i);
      stream << '[' << std::setw(numDigit) << digit << ']';
      stream << ' ' << stackTrace.traces[i] << ' ' << stackTrace.symbols[i];
      result += stream.str();
      result += '\n';
    }

    return result;
  }
} // namespace winrsls::debug

#if defined(_WIN32) || defined(_WIN64)

// clang-format off
#include <Windows.h>
#include <ImageHlp.h>
// clang-format on

#pragma comment(lib, "imagehlp.lib")

namespace winrsls::debug {
  StackTrace getStackTrace() {
    constexpr DWORD maxSize = 256;
    void* traces[maxSize];

    HANDLE hProcess = GetCurrentProcess();
    SymInitialize(hProcess, nullptr, TRUE);

    WORD traceSize = CaptureStackBackTrace(0, maxSize, traces, nullptr);

    constexpr size_t maxNameSize = 255;
    constexpr size_t symInfoSize =
      sizeof(SYMBOL_INFO) + (maxNameSize + 1) * sizeof(char);

    PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(malloc(symInfoSize));

    StackTrace stackTrace;

    if (symbol != nullptr) {
      symbol->MaxNameLen = maxNameSize;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

      stackTrace.traces.reserve(traceSize);
      stackTrace.symbols.reserve(traceSize);
      for (WORD i = 0; i < traceSize; ++i) {
        SymFromAddr(hProcess, reinterpret_cast<DWORD64>(traces[i]), nullptr,
                    symbol);
        stackTrace.traces.push_back(traces[i]);
        stackTrace.symbols.emplace_back(symbol->Name, symbol->NameLen);
      }

      free(symbol);
    }

    return stackTrace;
  }
} // namespace winrsls::debug

#else

namespace winrsls::debug {
  StackTrace getStackTrace() {
    StackTrace stackTrace;
    return stackTrace;
  }
} // namespace winrsls::debug

#endif // defined(_WIN32) || defined(_WIN64)

#endif // NDEBUG