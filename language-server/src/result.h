#ifndef WINRSLS_RESULT_H_
#define WINRSLS_RESULT_H_

#include <cassert>
#include <utility>

namespace winrsls {
  template <typename OK, typename Error>
  class Result {
    enum struct Tag { None, OK, Error };
    Tag tag;
    union {
      OK ok;
      Error error;
    };

  public:
    Result(const Result& result) : tag(result.tag) {
      switch (tag) {
        case Tag::None: break;
        case Tag::OK: new (&ok) OK(result.ok); break;
        case Tag::Error: new (&error) Error(result.error); break;
        default: std::abort();
      }
    }
    Result(Result&& result) : tag(result.tag) {
      switch (tag) {
        case Tag::None: break;
        case Tag::OK: new (&ok) OK(std::move(result.ok)); break;
        case Tag::Error: new (&error) Error(std::move(result.error)); break;
        default: std::abort();
      }
      result.tag = Tag::None;
    }
    ~Result() {
      switch (tag) {
        case Tag::None: break;
        case Tag::OK: ok.~OK(); break;
        case Tag::Error: error.~Error(); break;
        default: std::abort();
      }
      tag = Tag::None;
    }
    Result& operator=(const Result& result) {
      if (&result != this) {
        switch (result.tag) {
          case Tag::None: this->~Result(); break;
          case Tag::OK: *this = result.ok(); break;
          case Tag::Error: *this = result.error(); break;
          default: std::abort();
        }
      }
      return *this;
    }
    Result& operator=(Result&& result) {
      assert(this != &result);
      switch (result.tag) {
        case Tag::None: this->~Result(); break;
        case Tag::OK: *this = std::move(result.ok); break;
        case Tag::Error: *this = std::move(result.error); break;
        default: std::abort();
      }
      result.tag = Tag::None;
      return *this;
    }
    Result(const OK& ok) : tag(Tag::OK), ok(ok) {}
    Result(OK&& ok) : tag(Tag::OK), ok(std::move(ok)) {}
    Result& operator=(const OK& ok) {
      if (tag != Tag::OK) {
        this->~Result();
        new (this) Result(ok);
      } else {
        this->ok = ok;
      }
      return *this;
    }
    Result& operator=(OK&& ok) {
      if (tag != Tag::OK) {
        this->~Result();
        new (this) Result(std::move(ok));
      } else {
        this->ok = std::move(ok);
      }
      return *this;
    }
    Result(const Error& error) : tag(Tag::Error), error(error) {}
    Result(Error&& error) : tag(Tag::Error), error(std::move(error)) {}
    Result& operator=(const Error& error) {
      if (tag != Tag::Error) {
        this->~Result();
        new (this) Result(error);
      } else {
        this->error = error;
      }
      return *this;
    }
    Result& operator=(Error&& error) {
      if (tag != Tag::Error) {
        this->~Result();
        new (this) Result(std::move(error));
      } else {
        this->error = std::move(error);
      }
      return *this;
    }
    operator bool() const noexcept {
      assert(tag != Tag::None);
      return tag == Tag::OK;
    }
    bool operator!() const noexcept {
      assert(tag != Tag::None);
      return tag == Tag::Error;
    }
    const OK& get() const {
      assert(tag == Tag::OK);
      return ok;
    }
    OK& get() {
      assert(tag == Tag::OK);
      return ok;
    }
    const Error& err() const {
      assert(tag == Tag::Error);
      return error;
    }
    Error& err() {
      assert(tag == Tag::Error);
      return error;
    }
  };

  struct _ {};

  template <typename OK>
  class ok_t {
    OK ok;

  public:
    explicit ok_t(OK&& ok) : ok(std::forward<OK>(ok)) {}
    template <typename Error>
    operator Result<OK, Error>() {
      return Result<OK, Error>(std::move(ok));
    }
  };
  template <typename OK>
  decltype(auto) ok(OK&& ok) {
    return ok_t(std::forward<OK>(ok));
  }
  template <typename OK = _>
  ok_t<_> ok() {
    return ok(_{});
  }

  template <typename Error>
  class error_t {
    Error error;

  public:
    explicit error_t(Error&& error) : error(std::forward<Error>(error)) {}
    template <typename OK>
    operator Result<OK, Error>() {
      return Result<OK, Error>(std::move(error));
    }
  };
  template <typename Error>
  decltype(auto) error(Error&& error) {
    return error_t(std::forward<Error>(error));
  }
} // namespace winrsls

#endif // WINRSLS_RESULT_H_