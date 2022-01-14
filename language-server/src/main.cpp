#include <fcntl.h>
#include <io.h>

#include <cstdio>
#include <iostream>

#include "server.h"

int main() {
  using namespace winrsls;

  std::ios_base::sync_with_stdio(false);
  _setmode(_fileno(stdin), _O_BINARY);
  _setmode(_fileno(stdout), _O_BINARY);
  std::ios_base::sync_with_stdio(true);

  Server server(std::cin, std::cout);
  const auto result = server.start();
  return result;
}
