#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

size_t read_file(const char* filename, uint8_t** data) {
  std::ifstream file(filename, std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  char* buf = new char[size];

  if (file.read(buf, size)) {
    *data = (uint8_t*) buf;
    return size;
  }

  delete[] buf;
  return 0;
}

int main(int argc, char** argv) {
  if (argc < 2) return 1;

  uint8_t* data;
  size_t size = read_file(argv[1], &data);

  LLVMFuzzerTestOneInput(data, size);
}
