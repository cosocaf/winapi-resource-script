#include "object.h"

#include "element.h"

namespace winrsls::json {
  Object::Object(const std::unordered_map<String, Element>& value)
    : value(value) {}
  Object::Object(std::unordered_map<String, Element>&& value)
    : value(std::move(value)) {}
  Object::Object(std::initializer_list<std::pair<String, Element>> value)
    : value(value.begin(), value.end()) {}

  const Element& Object::at(const String& key) const { return value.at(key); }
  bool Object::contains(const String& key) const { return value.contains(key); }

  Element& Object::operator[](const String& key) { return value[key]; }

  bool operator==(const Object& lhs, const Object& rhs) {
    return lhs.value == rhs.value;
  }

  std::string Object::toJsonString() const {
    std::string result;
    result += '{';
    for (const auto& [key, val] : value) {
      result += key.toJsonString();
      result += ':';
      result += val.toJsonString();
      result += ',';
    }
    // erase last comma. {"a": 1, "b": 2, /* <- like this */}
    if (!value.empty()) {
      result.pop_back();
    }
    result += '}';
    return result;
  }
} // namespace winrsls::json