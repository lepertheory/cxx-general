#include <iostream>

#include "POSIXFile/POSIXFile.h++"

using namespace std;
using namespace DAC;

int main (int argc, char** argv) {
  
  if (argc != 2) {
    cout << "Wrong number of arguments.\n";
    return 1;
  }
  
  POSIXFile test(argv[1]);
  
  cout << "File: " << argv[1] << "\n"
       << "  Device:    " << test.device   () << "\n"
       << "  inode:     " << test.inode    () << "\n"
       << "  mode:      " << test.mode     () << "\n"
       << "  links:     " << test.links    () << "\n"
       << "  uid:       " << test.uid      () << "\n"
       << "  gid:       " << test.gid      () << "\n"
       << "  devid:     " << test.devid    () << "\n"
       << "  size:      " << test.size     () << "\n"
       << "  blockSize: " << test.blockSize() << "\n"
       << "  blocks:    " << test.blocks   () << "\n"
       << "  atime:     " << test.atime    () << "\n"
       << "  mtime:     " << test.mtime    () << "\n"
       << "  ctime:     " << test.ctime    () << "\n"
       << "  \n"
       << "  basename: " << test.basename() << "\n";
  
  cout << "  contents:\n"
       << test.get_file();
  
  return 0;
  
}

