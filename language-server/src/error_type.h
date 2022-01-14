#ifndef WINRSLS_ERROR_TYPE_H_
#define WINRSLS_ERROR_TYPE_H_

namespace winrsls {
  enum struct ErrorType {
    InvalidHeader = 1,
    InvalidContent = 2,
  };
}

#endif // WINRSLS_ERROR_TYPE_H_