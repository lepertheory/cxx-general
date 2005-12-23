// STL includes.
#include <iostream>

// Testing include.
#include "GetOptCXX.hxx"

// Namespace declarations.
using namespace std;
using namespace DAC;

// This is it.
int main (int argc, char** argv) {
  
  GetOptCXX test(argc, argv);
  test.add_option(GetOptCXX::Option('h', "help"  , GetOptCXX::ARG_NONE,     false));
  test.add_option(GetOptCXX::Option('o', "output", GetOptCXX::ARG_REQUIRED, false));
  test.add_option(GetOptCXX::Option('b', "bling" , GetOptCXX::ARG_OPTIONAL, false));
  
  cout << "set: " << test.isSet('o') << endl;
  
  return 0;
  
}

