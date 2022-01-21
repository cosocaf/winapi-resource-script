/**
 * @file error_info.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-01-21
 *
 * @copyright Copyright (c) 2022 cosocaf
 *
 */

#ifndef WINRSLS_ERROR_INFO_H_
#define WINRSLS_ERROR_INFO_H_

#include <string>
#include <string_view>

namespace winrsls {
  class ErrorInfo {
    std::string msg;

  public:
    ErrorInfo(const std::string& msg);
    ErrorInfo(std::string&& msg);
    ErrorInfo(std::string_view msg);
    ErrorInfo(const char* msg);

    const std::string& getMessage() const;
    std::string& getMessage();
  };
} // namespace winrsls

#ifdef NDEBUG

#define ERROR_INFO(msg) ::winrsls::ErrorInfo(msg)

#else

#include <vector>

namespace winrsls::debug {
  struct StackTrace {
    std::vector<void*> traces;
    std::vector<std::string> symbols;
  };
  StackTrace getStackTrace();
  std::string generateErrorMessage(const std::string& message,
                                   const char* const file,
                                   const char* const func, const int line);
} // namespace winrsls::debug

#if defined(_MSC_VER)
#define WINRSLS_ERROR_INFO_FUNCTION_NAME __FUNCSIG__
#elif defined(__GNUC__)
#define WINRSLS_ERROR_INFO_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define WINRSLS_ERROR_INFO_FUNCTION_NAME __func__
#endif // defined(_MSC_VER)

#define ERROR_INFO(msg)                                        \
  ::winrsls::ErrorInfo(::winrsls::debug::generateErrorMessage( \
    msg, __FILE__, WINRSLS_ERROR_INFO_FUNCTION_NAME, __LINE__))

#endif // NDEBUG

#endif // WINRSLS_ERROR_INFO_H_