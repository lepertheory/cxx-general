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
  
  INIFile::SectionListPT sections(test.get_sections());
  for (INIFile::SectionListPT::value_type::iterator i = sections->begin(); i != sections->end(); ++i) {
    cout << "Section: \"" << *i << "\"\n";
    INIFile::KeyListPT keys(test.get_keys(*i));
    for (INIFile::KeyListPT::value_type::iterator j = keys->begin(); j != keys->end(); ++j) {
      cout << "Key: \"" << *j << "\"\n";
    }
  }
  
  return 0;
  
}

