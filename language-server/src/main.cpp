#include <fcntl.h>
#include <io.h>

#include <cstdio>
#include <fstream>
#include <iostream>

#include "server.h"

int main(int argc, char **argv) {
  using namespace winrsls;

  std::ios_base::sync_with_stdio(false);
  _setmode(_fileno(stdin), _O_BINARY);
  _setmode(_fileno(stdout), _O_BINARY);
  std::ios_base::sync_with_stdio(true);

  if (argc == 3 && argv[1] == std::string("--input")) {
    std::ifstream input(argv[2], std::ios_base::binary);
    if (!input) {
      std::cout << "Can't open input file: " << argv[2] << std::endl;
      return -1;
    }
    Server server(input, std::cout);
    const auto result = server.start();
    return result;
  } else {
    Server server(std::cin, std::cout);
    const auto result = server.start();
    return result;
  }
}
