#include <iostream>

#include "INIFile/INIFile.h++"

using namespace std;
using namespace DAC;

int main (int argc, char** argv) {
  
  if (argc != 2) {
    cout << "Wrong number of arguments.\n";
    return 1;
  }
  
  INIFile test(argv[1]);
  
  test.read();
  
  return 0;
  
}

