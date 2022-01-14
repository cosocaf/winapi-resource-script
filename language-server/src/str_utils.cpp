#include "str_utils.h"

namespace winrsls {
  std::vector<std::string_view> split(std::string_view str,
                                      std::string_view separator) {
    std::vector<std::string_view> result;
    std::string_view::size_type offset = 0;
    while (true) {
      const auto index = str.find(separator, offset);
      result.push_back(str.substr(offset, index - offset));
      if (index == std::string_view::npos) {
        break;
      }
      offset = index + separator.size();
    }
    return result;
  }

  std::string_view trim(std::string_view str, std::string_view trimChars) {
    auto pos = str.find_first_not_of(trimChars);
    if (pos != std::string_view::npos) {
      str.remove_prefix(pos);
    }
    pos = str.find_last_not_of(trimChars);
    if (pos != std::string_view::npos) {
      str.remove_suffix(str.size() - pos - 1);
    }
    return str;
  }
} // namespace winrsls