#ifndef WINRSLS_JSON_OBJECT_H_
#define WINRSLS_JSON_OBJECT_H_

#include <initializer_list>
#include <ostream>
#include <string>
#include <unordered_map>

#include "element_type.h"
#include "string.h"
#include "union.h"

namespace winrsls::json {
  class Element;
  class Object {
    std::unordered_map<String, Element> value;

    friend bool operator==(const Object& lhs, const Object& rhs);

  public:
    Object(const std::unordered_map<String, Element>& value = {});
    Object(std::unordered_map<String, Element>&& value);
    Object(std::initializer_list<std::pair<String, Element>> value);

    const Element& at(const String& key) const;

    bool contains(const String& key) const;

    template <element_type... T>
    inline bool contains_as(const String& key) const {
      return contains(key) && (false || ... || value.at(key).is<T>());
    }

    Element& operator[](const String& key);

    std::string toJsonString() const;
  };

  bool operator==(const Object& lhs, const Object& rhs);

  std::ostream& operator<<(std::ostream& out, const Object& value);
} // namespace winrsls::json

#endif // WINRSLS_JSON_OBJECT_H_