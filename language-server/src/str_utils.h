#ifndef WINRSLS_U8STR_UTILS_H_
#define WINRSLS_U8STR_UTILS_H_

#include <string_view>
#include <vector>

namespace winrsls {
  /**
   * @brief Split strings with separators.
   *
   * The lifetime of the return value is equal to the lifetime of str.
   *
   * @param str String to split
   * @param separator Separator
   * @return std::vector<std::basic_string_view<CharT>> Vector of split strings
   */
  std::vector<std::string_view> split(std::string_view str,
                                      std::string_view separator);

  std::string_view trim(std::string_view str,
                        std::string_view trimChars = " \t\v\r\n");
}; // namespace winrsls

#endif // WINRSLS_U8STR_UTILS_H_