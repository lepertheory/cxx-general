#include <iostream>
#include <vector>

#include "POSIXFile.h++"

using namespace std;
using namespace DAC;

int main (int argc, char** argv) {
  
  if (argc != 2) {
    cout << "Wrong number of arguments.\n";
    return 1;
  }
  
  POSIXFile test(argv[1]);
  
  cout << "File: " << argv[1] << "\n"
       << "  Filename: " << test.Filename() << "\n"
       << "  getCWD:   " << test.getCWD  () << "\n"
       << "  \n"
       << "  Append:    " << test.Append   () << "\n"
       << "  Create:    " << test.Create   () << "\n"
       << "  Exclusive: " << test.Exclusive() << "\n"
       << "  DoATime:   " << test.DoATime  () << "\n"
       << "  CanCTTY:   " << test.CanCTTY  () << "\n"
       << "  FollowSym: " << test.FollowSym() << "\n"
       << "  Synch:     " << test.Synch    () << "\n"
       << "  Truncate:  " << test.Truncate () << "\n"
       << "  \n"
       << "  is_blockDev:        " << test.is_blockDev       () << "\n"
       << "  is_charDev:         " << test.is_charDev        () << "\n"
       << "  is_dir:             " << test.is_dir            () << "\n"
       << "  is_directory:       " << test.is_directory      () << "\n"
       << "  is_executable:      " << test.is_executable     () << "\n"
       << "  is_executable_real: " << test.is_executable_real() << "\n"
       << "  is_exist:           " << test.is_exist          () << "\n"
       << "  is_file:            " << test.is_file           () << "\n"
       << "  is_groupOwned:      " << test.is_groupOwned     () << "\n"
       << "  is_userOwned:       " << test.is_userOwned      () << "\n"
       << "  is_pipe:            " << test.is_pipe           () << "\n"
       << "  is_readable:        " << test.is_readable       () << "\n"
       << "  is_readable_real:   " << test.is_readable_real  () << "\n"
       << "  is_setGID:          " << test.is_setGID         () << "\n"
       << "  is_setUID:          " << test.is_setUID         () << "\n"
       << "  is_socket:          " << test.is_socket         () << "\n"
       << "  is_sticky:          " << test.is_sticky         () << "\n"
       << "  is_symlink:         " << test.is_symlink        () << "\n"
       << "  is_writable:        " << test.is_writable       () << "\n"
       << "  is_writable_real:   " << test.is_writable_real  () << "\n"
       << "  is_zero:            " << test.is_zero           () << "\n"
       << "  \n"
       << "  basename:    " << test.basename   () << "\n"
       << "  dirname:     " << test.dirname    () << "\n"
       << "  expand_path: " << test.expand_path() << "\n"
       << "  \n";
  cout << "  SetUID:    " << test.SetUID   () << "\n"
       << "  SetGID:    " << test.SetGID   () << "\n"
       << "  Sticky:    " << test.Sticky   () << "\n"
       << "  U_Read:    " << test.U_Read   () << "\n"
       << "  U_Write:   " << test.U_Write  () << "\n"
       << "  U_Execute: " << test.U_Execute() << "\n"
       << "  G_Read:    " << test.G_Read   () << "\n"
       << "  G_Write:   " << test.G_Write  () << "\n"
       << "  G_Execute: " << test.G_Execute() << "\n"
       << "  O_Read:    " << test.O_Read   () << "\n"
       << "  O_Write:   " << test.O_Write  () << "\n"
       << "  O_Execute: " << test.O_Execute() << "\n"
       << "  A_Read:    " << test.A_Read   () << "\n"
       << "  A_Write:   " << test.A_Write  () << "\n"
       << "  A_Execute: " << test.A_Execute() << "\n"
       << "  \n"
       << "  Mode: " << test.Mode() << "\n"
       << "  UID:  " << test.UID () << "\n"
       << "  GID:  " << test.GID () << "\n"
       << "  \n"
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
       << "  ctime:     " << test.ctime    () << "\n";
  
  cout << "  contents:\n"
       << test.read_file();
  
  cout << "  line by line contents:\n";
  test.open();
  while (!test.eof_line()) {
    cout << "\"" << test.read_line() << "\"\n";
  }
  test.close();
  
  cout << "  line by line contents as vector:\n";
  vector<string> temp;
  test.read_all_lines(temp);
  for (vector<string>::iterator i = temp.begin(); i != temp.end(); ++i) {
    cout << "\"" << *i << "\"\n";
  }
  
  return 0;
  
}

