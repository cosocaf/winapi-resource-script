/**
 * @file element.h
 * @author cosocaf (cosocaf@gmail.com)
 * @brief Defines a type for handling JSON elements in an abstract way.
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022 cosocaf
 * This software is released under the MIT license.
 * See https://opensource.org/licenses/MIT
 */

#ifndef WINRSLS_JSON_ELEMENT_H_
#define WINRSLS_JSON_ELEMENT_H_

#include <cassert>
#include <initializer_list>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "element_like.h"

namespace winrsls::json {
  class String;
  template <element_like... T>
  class Union;

  class Element;

  template <typename T>
  concept element_like_without_element =
    element_like<T> && !std::same_as<Element, std::remove_cvref_t<T>>;

  class Element {
    struct Storage {
      constexpr static size_t maxSmallSize = sizeof(void*) * 4;
      union {
        void* ptr;
        unsigned char small[maxSmallSize];
        void* large;
      };
      void (*deleter)(void**);
      void (*copy)(void**, void* const*);
      bool (*equals)(void* const*, void* const*);
      std::string (*toJsonString)(void* const*);

      Storage(const Storage& other);
      Storage(Storage&& other) noexcept;

      template <element_like_without_element T>
      Storage(T&& value)
        : deleter(_deleter<std::remove_cvref_t<T>>)
        , copy(_copy<std::remove_cvref_t<T>>)
        , equals(_equals<std::remove_cvref_t<T>>)
        , toJsonString(_toJsonString<std::remove_cvref_t<T>>) {
        using U = std::remove_cvref_t<T>;
        if constexpr (sizeof(U) <= maxSmallSize) {
          new (reinterpret_cast<U*>(&small)) U(std::forward<T>(value));
        } else {
          large = new U(std::forward<T>(value));
        }
      }
      ~Storage();

      Storage& operator=(const Storage& other) &;
      Storage& operator=(Storage&& other) & noexcept;

      template <element_like_without_element T>
      static bool is(const Storage& value) noexcept {
        return value.deleter == _deleter<T>;
      }

      template <element_like_without_element T>
      static T& as(Storage& value) noexcept {
        assert(is<T>(value));
        if constexpr (sizeof(T) <= maxSmallSize) {
          return *reinterpret_cast<T*>(&value.small[0]);
        } else {
          return *reinterpret_cast<T*>(value.large);
        }
      }

      template <element_like_without_element T>
      static const T& as(const Storage& value) noexcept {
        assert(is<T>(value));
        if constexpr (sizeof(T) <= maxSmallSize) {
          return *reinterpret_cast<const T*>(&value.small[0]);
        } else {
          return *reinterpret_cast<const T*>(value.large);
        }
      }

      static bool same(const Storage& value1, const Storage& value2) noexcept {
        if (value1.deleter == nullptr || value2.deleter == nullptr) {
          return false;
        }
        if (value1.deleter != value2.deleter) {
          return false;
        }

        return value1.equals(&value1.ptr, &value2.ptr);
      }

    private:
      template <element_like_without_element T>
      static void _deleter(void** ptr) noexcept {
        if constexpr (sizeof(T) <= maxSmallSize) {
          reinterpret_cast<T*>(ptr)->~T();
        } else {
          delete reinterpret_cast<T*>(*ptr);
        }
        memset(ptr, 0, maxSmallSize);
      }
      template <element_like_without_element T>
      static void _copy(void** dst, void* const* src) {
        if constexpr (sizeof(T) <= maxSmallSize) {
          new (reinterpret_cast<T*>(dst)) T(*reinterpret_cast<const T*>(src));
        } else {
          *reinterpret_cast<T**>(dst) =
            new T(**reinterpret_cast<T* const*>(src));
        }
      }
      template <element_like_without_element T>
      static std::string _toJsonString(void* const* ptr) {
        if constexpr (sizeof(T) <= maxSmallSize) {
          return reinterpret_cast<const T*>(ptr)->toJsonString();
        } else {
          return reinterpret_cast<const T*>(*ptr)->toJsonString();
        }
      }

      template <element_like_without_element T>
      static bool _equals(void* const* ptr1, void* const* ptr2) noexcept {
        if constexpr (sizeof(T) <= maxSmallSize) {
          return *reinterpret_cast<const T*>(ptr1) ==
                 *reinterpret_cast<const T*>(ptr2);
        } else {
          return *reinterpret_cast<T*>(*ptr1) == *reinterpret_cast<T*>(*ptr2);
        }
      }
    };
    Storage value;

    friend bool operator==(const Element& lhs, const Element& rhs);

  public:
    Element(const Element& other);
    Element(Element&& other) noexcept;

    template <element_like_without_element T>
    Element(T&& value) : value(std::forward<T>(value)) {}

    // Create as String

    Element(const std::string& value);
    Element(std::string&& value);
    Element(std::string_view value);
    Element(const char* value);

    // Create as Number

    Element(int64_t value);
    Element(int value);
    Element(double_t value);

    // Create as Boolean

    Element(bool value);

    // Create as Null (Default constructor)

    Element();

    // Create as Array

    Element(const std::vector<Element>& value);
    Element(std::vector<Element>&& value);
    Element(std::initializer_list<Element> value);

    // Create as Object

    Element(const std::unordered_map<String, Element>& value);
    Element(std::unordered_map<String, Element>&& value);
    Element(std::initializer_list<std::pair<String, Element>> value);

    Element& operator=(const Element& other) &;
    Element& operator=(Element&& other) & noexcept;

    template <element_like_without_element T>
    Element& operator=(T&& other) & {
      value = Storage(std::forward<T>(other));
      return *this;
    }

    template <element_like_without_element T>
    bool is() const noexcept {
      return Storage::is<std::remove_cvref_t<T>>(value);
    }
    template <element_like_without_element... T>
    bool either() const noexcept {
      return (false || ... || Storage::is<std::remove_cvref_t<T>>(value));
    }
    template <element_like_without_element T>
    const std::remove_cvref_t<T>& as() const noexcept {
      return Storage::as<std::remove_cvref_t<T>>(value);
    }
    template <element_like_without_element T>
    std::remove_cvref_t<T>& as() noexcept {
      return Storage::as<std::remove_cvref_t<T>>(value);
    }
    template <element_like_without_element... T>
    inline Union<std::remove_cvref_t<T>...> as_union() const& {
      assert(either<std::remove_cvref_t<T>...>());
      std::optional<Union<std::remove_cvref_t<T>...>> result;
      (void)std::initializer_list{
        (void(_assign<std::remove_cvref_t<T>>(result)), 0)...};
      return result.value();
    }
    template <element_like_without_element... T>
    inline Union<std::remove_cvref_t<T>...> as_union() && {
      assert(either<std::remove_cvref_t<T>...>());
      std::optional<Union<std::remove_cvref_t<T>...>> result;
      (void)std::initializer_list{
        (void(_moveAssign<std::remove_cvref_t<T>>(result)), 0)...};
      return result.value();
    }

    std::string toJsonString() const;

  private:
    template <typename T, typename... U>
    void _assign(std::optional<Union<U...>>& result) const {
      if (Storage::is<T>(value)) {
        result.emplace(Storage::as<T>(value));
      }
    }
    template <typename T, typename... U>
    void _moveAssign(std::optional<Union<U...>>& result) const {
      if (Storage::is<T>(value)) {
        result.emplace(std::move(Storage::as<T>(value)));
      }
    }
  };

  bool operator==(const Element& lhs, const Element& rhs);
} // namespace winrsls::json

#endif // WINRSLS_JSON_ELEMENT_H_