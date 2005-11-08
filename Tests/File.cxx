#include <iostream>

#include "POSIXFile/POSIXFile.hxx"

using namespace std;
using namespace DAC;

int main (int argc, char** argv) {
  
  if (argc != 2) {
    cout << "Wrong number of arguments.\n";
    return 1;
  }
  
  cout << "File: " << argv[1] << endl;
  
  return 0;
  
}

