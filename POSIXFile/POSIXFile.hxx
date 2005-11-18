/*****************************************************************************
 * POSIXFile.hxx
 *****************************************************************************
 * Interface for class POSIXFile.
 *****************************************************************************/

// Include guard.
#if !defined(POSIXFILE_234b734278udud8hb48)
  #define POSIXFILE_234b734278udud8hb48

// STL includes.
#include <string>
#include <fstream>

// System includes.
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cxx-general/Exception.hxx>
#include <cxx-general/toString.hxx>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * POSIXFile
   ***************************************************************************
   * POSIXFile class.
   ***************************************************************************/
  class POSIXFile {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Data types.
      
      // Open mode.
      enum OpenMode { OM_READ, OM_WRITE, OM_READWRITE };
      
      // Seek modes.
      enum SeekMode { SM_SET = SEEK_SET, SM_CUR = SEEK_CUR, SM_END = SEEK_END };
      
      /***********************************************************************/
      // Constants.
      
      // Directory separator.
      static char const DIR_SEP;
      
      /***********************************************************************/
      // Errors.
      class Errors {
        public:
          
          // All POSIXFile errors are based off of this.
          class Base : public Exception {
            public:
              virtual ~Base () throw() {};
              virtual char const* what () const throw() { return "Undefined error in POSIXFile."; };
          };
          
          // Unexpected error.
          class Unexpected : public Base {
            public:
              virtual ~Unexpected () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Unexpected system error. Error number is " + DAC::toString(_errno) + ".").c_str();
                } catch (...) {
                  return "Unexpected system error. Error creating message string.";
                }
              };
              Unexpected& Errno (int const errnum) { _errno = errnum; return *this; };
              int Errno () const { return _errno; }
            private:
              int _errno;
          };
          
          // Attempt to open an already open file.
          class AlreadyOpen : public Base {
            public:
              virtual ~AlreadyOpen () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("File \"" + _filename + "\" is already open.").c_str();
                } catch (...) {
                  return "File is already open. Error creating message string.";
                }
              };
              AlreadyOpen& Filename (std::string const& filename) { _filename = filename; return *this; }
              std::string Filename () const { return _filename; }
            private:
              std::string _filename;
          };
          
          // Attempt to close a file that has not been opened.
          class NotOpen : public Base {
            public:
              virtual ~NotOpen () throw() {};
              virtual char const* what () const throw() {
                return "Attempted to close a file that is not open.";
              };
          };  
          
          // Access error.
          class AccessDenied : public Base {
            public:
              virtual ~AccessDenied () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Access denied attempting to " + _op + " \"" + _filename + "\".").c_str();
                } catch (...) {
                  return "Access denied. Error creating message string.";
                }
              };
              AccessDenied& Operation (std::string const& op      ) { _op       = op      ; return *this; };
              AccessDenied& Filename  (std::string const& filename) { _filename = filename; return *this; };
              std::string Operation () const { return _op      ; };
              std::string Filename  () const { return _filename; };
            private:
              std::string _op      ;
              std::string _filename;
          };
          
          // File is unsuitable for requested operation.
          class Invalid : public Base {
            public:
              virtual ~Invalid () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("\"" + _filename + "\" is unsuitable for requested " + _op + " operation.").c_str();
                } catch (...) {
                  return "File is unsuitable for requested operation. Error creating message string.";
                }
              };
              Invalid& Operation (std::string const& op      ) { _op       = op      ; return *this; };
              Invalid& Filename  (std::string const& filename) { _filename = filename; return *this; };
              std::string Operation () const { return _op      ; };
              std::string Filename  () const { return _filename; };
            private:
              std::string _op      ;
              std::string _filename;
          };
          
          // File already exists.
          class FileExists : public Base {
            public:
              virtual ~FileExists () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("File already exists error attempting to " + _op + " \"" + _filename + "\".").c_str();
                } catch (...) {
                  return "File already exists. Error creating message string.";
                }
              };
              FileExists& Operation (std::string const& op      ) { _op       = op      ; return *this; };
              FileExists& Filename  (std::string const& filename) { _filename = filename; return *this; };
              std::string Operation () const { return _op      ; };
              std::string Filename  () const { return _filename; };
            private:
              std::string _op      ;
              std::string _filename;
          };
          
          // Write access requested for a directory.
          class IsDirectory : public Base {
            public:
              virtual ~IsDirectory () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Attempted to open directory \"" + _filename + "\" for write.").c_str();
                } catch (...) {
                  return "Attempted to open directory for write. Error creating message string.";
                }
              };
              IsDirectory& Filename (std::string const& filename) { _filename = filename; return *this; };
              std::string Filename () const { return _filename; };
            private:
              std::string _filename;
          };
          
          // Maximum number of files are open.
          class MaxFiles : public Base {
            public:
              virtual ~MaxFiles () throw() {};
              virtual char const* what () const throw() {
                return "The maximum number of available files are already open.";
              };
          };
          
          // Maximum files for this process.
          class ProcMaxFiles : public MaxFiles {
            public:
              virtual ~ProcMaxFiles () throw() {};
              virtual char const* what () const throw() {
                return "This process is already at the maximum number of open files.";
              };
          };
          
          // Maximum files for the system.
          class SysMaxFiles : public MaxFiles {
            public:
              virtual ~SysMaxFiles () throw() {};
              virtual char const* what () const throw() {
                return "The maximum number of files for this system are already open.";
              };
          };
          
          // No space available for the new file.
          class NoSpace : public Base {
            public:
              virtual ~NoSpace () throw() {};
              virtual char const* what () const throw() {
                return "No space available to create the file.";
              };
          };
          
          // File is too large to open without large file support.
          class FileOverflow : public Base {
            public:
              virtual ~FileOverflow () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("File \"" + _filename + "\" is too large to open without large file support.").c_str();
                } catch (...) {
                  return "File is too large to open without large file support. Error creating message string.";
                }
              };
              FileOverflow& Filename (std::string const& filename) { _filename = filename; return *this; };
              std::string Filename () const { return _filename; };
            private:
              std::string _filename;
          };
          
          // Write access was requested on a file that is currently executing.
          class ExecuteWrite : public Base {
            public:
              virtual ~ExecuteWrite () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("File \"" + _filename + "\" is currently executing and cannot be opened for write.").c_str();
                } catch (...) {
                  return "File is currently executing and cannot be opened for write. Error creating message string.";
                }
              };
              ExecuteWrite& Filename (std::string const& filename) { _filename = filename; return *this; };
              std::string Filename () const { return _filename; };
            private:
              std::string _filename;
          };
          
          // Bad file descriptor.
          class BadDescriptor : public Base {
            public:
              virtual ~BadDescriptor () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Bad file descriptor when attempting " + _op + ".").c_str();
                } catch (...) {
                  return "Bad file descriptor. Error creating message string.";
                }
              };
              BadDescriptor& Operation  (std::string const& op) { _op = op; return *this; };
              BadDescriptor& Descriptor (int         const  fd) { _fd = fd; return *this; };
              std::string Operation  () const { return _op; };
              int         Descriptor () const { return _fd; };
            private:
              std::string _op;
              int         _fd;
          };
          
          // Operation was interrupted by a signal.
          class Interrupted : public Base {
            public:
              virtual ~Interrupted () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Attempted " + _op + " operation was interrupted by a signal.").c_str();
                } catch (...) {
                  return "Attempted operation was interrupted by a signal. Error creating message string.";
                }
              };
              Interrupted& Operation (std::string const& op) { _op = op; return *this; }
              std::string Operation () const { return _op; }
            private:
              std::string _op;
          };
          
          // Operation attempted at end of file.
          class EoF : public Base {
            public:
              virtual ~EoF () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Requested " + _op + " operation cannot be performed at end of file.").c_str();
                } catch (...) {
                  return "Requested operation cannot be performed at end of file. Error creating message string.";
                }
              };
              EoF& Operation (std::string const& op) { _op = op; return *this; }
              std::string Operation () const { return _op; }
            private:
              std::string _op;
          };
          
          // I/O error.
          class IOError : public Base {
            public:
              virtual ~IOError () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("I/O error when attempting to " + _op + " \"" + _filename + "\".").c_str();
                } catch (...) {
                  return "I/O error. Error creating message string.";
                }
              };
              IOError& Operation (std::string const& op      ) { _op       = op      ; return *this; };
              IOError& Filename  (std::string const& filename) { _filename = filename; return *this; };
              std::string Operation () const { return _op      ; };
              std::string Filename  () const { return _filename; };
            private:
              std::string _op      ;
              std::string _filename;
          };
          
          // No permission to specified file.
          class NoPermission : public Base {
            public:
              virtual ~NoPermission () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("No permission to " + _op + " \"" + _filename + "\".").c_str();
                } catch (...) {
                  return "No permission. Error creating message string.";
                }
              };
              NoPermission& Operation (std::string const& op      ) { _op       = op      ; return *this; };
              NoPermission& Filename  (std::string const& filename) { _filename = filename; return *this; };
              std::string Operation () const { return _op      ; };
              std::string Filename  () const { return _filename; };
            private:
              std::string _op      ;
              std::string _filename;
          };
          
          // Operation on a read-only filesystem.
          class ReadOnlyFS : public Base {
            public:
              virtual ~ReadOnlyFS () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Cannot " + _op + " \"" + _filename + "\" on a read-only filesystem.").c_str();
                } catch (...) {
                  return "Cannot perform operation on a read-only filesystem. Error creating message string.";
                }
              };
              ReadOnlyFS& Operation (std::string const& op      ) { _op       = op      ; return *this; };
              ReadOnlyFS& Filename  (std::string const& filename) { _filename = filename; return *this; };
              std::string Operation () const { return _op      ; };
              std::string Filename  () const { return _filename; };
            private:
              std::string _op      ;
              std::string _filename;
          };
          
          // Symlink loop / too many symlinks.
          class SymlinkLoop : public Base {
            public:
              virtual ~SymlinkLoop () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Symlink loop or too many symlinks traversing path attempting to " + _op + " \"" + _filename + "\".").c_str();
                } catch (...) {
                  return "Symlink loop or too many symlinks. Error creating message string.";
                }
              };
              SymlinkLoop& Operation (std::string const& op      ) { _op       = op      ; return *this; };
              SymlinkLoop& Filename  (std::string const& filename) { _filename = filename; return *this; };
              std::string Operation () const { return _op      ; };
              std::string Filename  () const { return _filename; };
            private:
              std::string _op      ;
              std::string _filename;
          };
          
          // Cannot seek on this type of file.
          class CannotSeek : public Base {
            public:
              virtual ~CannotSeek () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("File \"" + _filename + "\" is a pipe, socket, or FIFO and cannot seek.").c_str();
                } catch (...) {
                  return "File is a pipe, socket, or FIFO and cannot seek. Error creating message string.";
                }
              }
              CannotSeek& Filename (std::string const& filename) { _filename = filename; return *this; };
              std::string Filename () const { return _filename; };
            private:
              std::string _filename;
          };
          
          // Filename is too long.
          class NameTooLong : public Base {
            public:
              virtual ~NameTooLong () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Filename \"" + _filename + "\" is too long.").c_str();
                } catch (...) {
                  return "Filename is too long. Error creating message string.";
                }
              };
              NameTooLong& Filename (std::string const& filename) { _filename = filename; return *this; };
              std::string Filename () const { return _filename; };
            private:
              std::string _filename;
          };
          
          // A component of the path does not exist.
          class PathNonExist : public Base {
            public:
              virtual ~PathNonExist () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("A component of the path \"" + _filename + "\" does not exist.").c_str();
                } catch (...) {
                  return "A component of the specified path does not exist. Error creating message string.";
                }
              };
              PathNonExist& Filename (std::string const& filename) { _filename = filename; return *this; };
              std::string Filename () const { return _filename; };
            private:
              std::string _filename;
          };
          
          // Out of memory.
          class OutOfMemory : public Base {};
          
          // Non-blocking I/O was selected and there is no data available.
          class TryAgain : public Base {};
          
          // Component of the path is not a directory.
          class NotDirectory : public Base {
            public:
              virtual ~NotDirectory () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("A component of the path to \"" + _filename + "\" is not a directory.").c_str();
                } catch (...) {
                  return "A component of the path is not a directory. Error creating message string.";
                }
              };
              NotDirectory& Filename (std::string const& filename) { _filename = filename; return *this; };
              std::string Filename () const { return _filename; };
            private:
              std::string _filename;
          };
          
          // Passed buffer is too small.
          class BufTooSmall : public Base {
            public:
              virtual ~BufTooSmall () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Passed buffer of " + DAC::toString(_bufsize) + " bytes, file is " + DAC::toString(_filesize) + " bytes.").c_str();
                } catch (...) {
                  return "Passed buffer is smaller than file size.";
                }
              };
              BufTooSmall& BufSize  (off_t const bufsize ) { _bufsize  = bufsize ; return *this; };
              BufTooSmall& FileSize (off_t const filesize) { _filesize = filesize; return *this; };
              off_t BufSize  () const { return _bufsize ; };
              off_t FileSize () const { return _filesize; };
            private:
              off_t _bufsize ;
              off_t _filesize;
          };
        
        // This class cannot be initialized.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator= (Errors const&);
        
      };
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      POSIXFile ();
      
      // Constructor names the file.
      POSIXFile (std::string const& filename);
      
      // Copy constructor.
      POSIXFile (POSIXFile const& source);
      
      // Assignment operator.
      POSIXFile& operator = (POSIXFile const& right);
      
      // Properties.
      POSIXFile& Filename  (std::string const& filename );
      POSIXFile& Append    (bool        const  append   );
      POSIXFile& Create    (bool        const  create   );
      POSIXFile& Exclusive (bool        const  exclusive);
      POSIXFile& DoATime   (bool        const  doatime  );
      POSIXFile& CanCTTY   (bool        const  canctty  );
      POSIXFile& FollowSym (bool        const  followsym);
      POSIXFile& Synch     (bool        const  synch    );
      POSIXFile& Truncate  (bool        const  truncate );
      POSIXFile& SetUID    (bool        const  setuid   );
      POSIXFile& SetGID    (bool        const  setgid   );
      POSIXFile& Sticky    (bool        const  sticky   );
      POSIXFile& U_Read    (bool        const  u_read   );
      POSIXFile& U_Write   (bool        const  u_write  );
      POSIXFile& U_Execute (bool        const  u_execute);
      POSIXFile& G_Read    (bool        const  g_read   );
      POSIXFile& G_Write   (bool        const  g_write  );
      POSIXFile& G_Execute (bool        const  g_execute);
      POSIXFile& O_Read    (bool        const  o_read   );
      POSIXFile& O_Write   (bool        const  o_write  );
      POSIXFile& O_Execute (bool        const  o_execute);
      POSIXFile& A_Read    (bool        const  a_read   );
      POSIXFile& A_Write   (bool        const  a_write  );
      POSIXFile& A_Execute (bool        const  a_execute);
      POSIXFile& Mode      (mode_t      const  new_mode , bool const cache = true);
      std::string Filename  () const;
      bool        Append    () const;
      bool        Create    () const;
      bool        Exclusive () const;
      bool        DoATime   () const;
      bool        CanCTTY   () const;
      bool        FollowSym () const;
      bool        Synch     () const;
      bool        Truncate  () const;
      bool        SetUID    () const;
      bool        SetGID    () const;
      bool        Sticky    () const;
      bool        U_Read    () const;
      bool        U_Write   () const;
      bool        U_Execute () const;
      bool        G_Read    () const;
      bool        G_Write   () const;
      bool        G_Execute () const;
      bool        O_Read    () const;
      bool        O_Write   () const;
      bool        O_Execute () const;
      bool        A_Read    () const;
      bool        A_Write   () const;
      bool        A_Execute () const;
      mode_t      Mode      (bool const cache = true) const;
      
      // Reset to just-constructed defaults.
      void clear ();
      
      // Copy a given File object. Does not copy the file itself.
      void copy (POSIXFile const& source);
      
      // Open the file.
      void open ();
      
      // Close the file.
      void close ();
      
      // Get the filename itself.
      std::string basename () const;
      
      // Seek to a particular location.
      void seek (off_t const offset, SeekMode const whence);
      
      // Read the entire file as a string.
      std::string get_file ();
      
      // File info.
      dev_t     device    (bool cache = true) const;
      ino_t     inode     (bool cache = true) const;
      mode_t    mode      (bool cache = true) const;
      nlink_t   links     (bool cache = true) const;
      uid_t     uid       (bool cache = true) const;
      gid_t     gid       (bool cache = true) const;
      dev_t     devid     (bool cache = true) const;
      off_t     size      (bool cache = true) const;
      blksize_t blockSize (bool cache = true) const;
      blkcnt_t  blocks    (bool cache = true) const;
      time_t    atime     (bool cache = true) const;
      time_t    mtime     (bool cache = true) const;
      time_t    ctime     (bool cache = true) const;
    
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data types.
      
      // File descriptor type.
      typedef int _FDType;
      
      // Open mode flag type.
      typedef int _OMFlagType;
      
      /***********************************************************************
       * _FD
       ***********************************************************************
       * Provides RAAI for file descriptors.
       ***********************************************************************/
      class _FD {
        
        /*
         * Public members.
         */
        public:
          
          /*******************************************************************/
          // Function members.
          
          // Default constructor.
          _FD (_FDType const fd = 0);
          
          // Destructor.
          ~_FD ();
          
          // Assignment operator.
          _FD& operator = (_FDType const right);
          
          // Casting operator, use _FD just like a normal fd.
          operator _FDType () const;
          
          // Set a new fd.
          int set (_FDType const fd);
          
        /*
         * Private members.
         */
        private:
          
          /*******************************************************************/
          // Data members.
          
          // Guess.
          _FDType _fd;
          
          /*******************************************************************/
          // Function members.
          
          // Copying is not allowed.
          _FD (_FD const&);
          _FD& operator = (_FD const&);
        
      };
      
      /***********************************************************************/
      // Constants.
      
      static mode_t const PERM_MASK;
      
      /***********************************************************************/
      // Data members.
      
      // The name of the file.
      std::string _filename;
      
      // File descriptor. 0 if file is closed.
      _FD _fd;
      
      // Current file read/write position.
      off_t _curpos;
      
      // Results of stat().
      mutable struct stat _stat;
      
      // Whether the stat cache is valid.
      mutable bool _cache_valid;
      
      // Open flags.
      _OMFlagType _flags;
      
      /***********************************************************************/
      // Function members.
      
      // Update the cache if necessary.
      void _check_cache (bool cache = true) const;
      
      // Update the cache now.
      void _update_cache () const;
      
      // Wrapper around ::read().
      ssize_t _read (void* const buf, off_t const bufsize, bool const updatepos = true);
      
      // Seek to a particular offset.
      void _seek            (off_t const offset, SeekMode const whence = SM_SET);
      void _seek_update_pos (off_t const offset, SeekMode const whence = SM_SET);
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.

  // Default constructor.
  inline POSIXFile::POSIXFile () { clear(); }
  
  // Copy constructor.
  inline POSIXFile::POSIXFile (POSIXFile const& source) { copy(source); }
  
  // Assignment operator.
  inline POSIXFile& POSIXFile::operator = (POSIXFile const& right) { copy(right); return *this; }
  
  // Set / get the filename.
  inline POSIXFile& POSIXFile::Filename (std::string const& filename) {
    if (filename != _filename) {
      clear();
      _filename = filename;
    }
    return *this;
  }
  inline std::string POSIXFile::Filename () const { return _filename; }
  
  // Set / get the open flags.
  inline POSIXFile& POSIXFile::Append    (bool const append   ) { if (append   ) { _flags |=  O_APPEND  ; } else { _flags &= ~O_APPEND  ; } return *this; }
  inline POSIXFile& POSIXFile::Create    (bool const create   ) { if (create   ) { _flags |=  O_CREAT   ; } else { _flags &= ~O_CREAT   ; } return *this; }
  inline POSIXFile& POSIXFile::Exclusive (bool const exclusive) { if (exclusive) { _flags |=  O_EXCL    ; } else { _flags &= ~O_EXCL    ; } return *this; }
  inline POSIXFile& POSIXFile::DoATime   (bool const doatime  ) { if (doatime  ) { _flags &= ~O_NOATIME ; } else { _flags |=  O_NOATIME ; } return *this; }
  inline POSIXFile& POSIXFile::CanCTTY   (bool const canctty  ) { if (canctty  ) { _flags &= ~O_NOCTTY  ; } else { _flags |=  O_NOCTTY  ; } return *this; }
  inline POSIXFile& POSIXFile::FollowSym (bool const followsym) { if (followsym) { _flags &= ~O_NOFOLLOW; } else { _flags |=  O_NOFOLLOW; } return *this; }
  inline POSIXFile& POSIXFile::Synch     (bool const synch    ) { if (synch    ) { _flags |=  O_SYNC    ; } else { _flags &= ~O_SYNC    ; } return *this; }
  inline POSIXFile& POSIXFile::Truncate  (bool const truncate ) { if (truncate ) { _flags |=  O_TRUNC   ; } else { _flags &= ~O_TRUNC   ; } return *this; }
  inline bool POSIXFile::Append    () const { return   _flags & O_APPEND   ; }
  inline bool POSIXFile::Create    () const { return   _flags & O_CREAT    ; }
  inline bool POSIXFile::Exclusive () const { return   _flags & O_EXCL     ; }
  inline bool POSIXFile::DoATime   () const { return !(_flags & O_NOATIME ); }
  inline bool POSIXFile::CanCTTY   () const { return !(_flags & O_NOCTTY  ); }
  inline bool POSIXFile::FollowSym () const { return !(_flags & O_NOFOLLOW); }
  inline bool POSIXFile::Synch     () const { return   _flags & O_SYNC     ; }
  inline bool POSIXFile::Truncate  () const { return   _flags & O_TRUNC    ; }
  
  // Set file modes.
  inline POSIXFile& POSIXFile::SetUID    (bool   const setuid   ) { if (setuid   ) { Mode(Mode() | S_ISUID); } else { Mode(Mode() & ~S_ISUID); } return *this; }
  inline POSIXFile& POSIXFile::SetGID    (bool   const setgid   ) { if (setgid   ) { Mode(Mode() | S_ISGID); } else { Mode(Mode() & ~S_ISGID); } return *this; }
  inline POSIXFile& POSIXFile::Sticky    (bool   const sticky   ) { if (sticky   ) { Mode(Mode() | S_ISVTX); } else { Mode(Mode() & ~S_ISVTX); } return *this; }
  inline POSIXFile& POSIXFile::U_Read    (bool   const u_read   ) { if (u_read   ) { Mode(Mode() | S_IRUSR); } else { Mode(Mode() & ~S_IRUSR); } return *this; }
  inline POSIXFile& POSIXFile::U_Write   (bool   const u_write  ) { if (u_write  ) { Mode(Mode() | S_IWUSR); } else { Mode(Mode() & ~S_IWUSR); } return *this; }
  inline POSIXFile& POSIXFile::U_Execute (bool   const u_execute) { if (u_execute) { Mode(Mode() | S_IXUSR); } else { Mode(Mode() & ~S_IXUSR); } return *this; }
  inline POSIXFile& POSIXFile::G_Read    (bool   const g_read   ) { if (g_read   ) { Mode(Mode() | S_IRGRP); } else { Mode(Mode() & ~S_IRGRP); } return *this; }
  inline POSIXFile& POSIXFile::G_Write   (bool   const g_write  ) { if (g_write  ) { Mode(Mode() | S_IWGRP); } else { Mode(Mode() & ~S_IWGRP); } return *this; }
  inline POSIXFile& POSIXFile::G_Execute (bool   const g_execute) { if (g_execute) { Mode(Mode() | S_IXGRP); } else { Mode(Mode() & ~S_IXGRP); } return *this; }
  inline POSIXFile& POSIXFile::O_Read    (bool   const o_read   ) { if (o_read   ) { Mode(Mode() | S_IROTH); } else { Mode(Mode() & ~S_IROTH); } return *this; }
  inline POSIXFile& POSIXFile::O_Write   (bool   const o_write  ) { if (o_write  ) { Mode(Mode() | S_IWOTH); } else { Mode(Mode() & ~S_IWOTH); } return *this; }
  inline POSIXFile& POSIXFile::O_Execute (bool   const o_execute) { if (o_execute) { Mode(Mode() | S_IXOTH); } else { Mode(Mode() & ~S_IXOTH); } return *this; }
  inline POSIXFile& POSIXFile::A_Read (bool const a_read) {
    if (a_read) {
      Mode(Mode() | S_IRUSR | S_IRGRP | S_IROTH);
    } else {
      Mode(Mode() & ~(S_IRUSR | S_IRGRP | S_IROTH));
    }
    return *this;
  }
  inline POSIXFile& POSIXFile::A_Write (bool const a_write) {
    if (a_write) {
      Mode(Mode() | S_IWUSR | S_IWGRP | S_IWOTH);
    } else {
      Mode(Mode() & ~(S_IWUSR | S_IWGRP | S_IWOTH));
    }
    return *this;
  }
  inline POSIXFile& POSIXFile::A_Execute (bool const a_execute) {
    if (a_execute) {
      Mode(Mode() | S_IXUSR | S_IXGRP | S_IXOTH);
    } else {
      Mode(Mode() & ~(S_IXUSR | S_IXGRP | S_IXOTH));
    }
    return *this;
  }
  
  // Get file modes.
  inline bool POSIXFile::SetUID    () const { return Mode() & S_ISUID                    ; }
  inline bool POSIXFile::SetGID    () const { return Mode() & S_ISGID                    ; }
  inline bool POSIXFile::Sticky    () const { return Mode() & S_ISVTX                    ; }
  inline bool POSIXFile::U_Read    () const { return Mode() & S_IRUSR                    ; }
  inline bool POSIXFile::U_Write   () const { return Mode() & S_IWUSR                    ; }
  inline bool POSIXFile::U_Execute () const { return Mode() & S_IXUSR                    ; }
  inline bool POSIXFile::G_Read    () const { return Mode() & S_IRGRP                    ; }
  inline bool POSIXFile::G_Write   () const { return Mode() & S_IWGRP                    ; }
  inline bool POSIXFile::G_Execute () const { return Mode() & S_IXGRP                    ; }
  inline bool POSIXFile::O_Read    () const { return Mode() & S_IROTH                    ; }
  inline bool POSIXFile::O_Write   () const { return Mode() & S_IWOTH                    ; }
  inline bool POSIXFile::O_Execute () const { return Mode() & S_IXOTH                    ; }
  inline bool POSIXFile::A_Read    () const { return Mode() & S_IRUSR & S_IRGRP & S_IROTH; }
  inline bool POSIXFile::A_Write   () const { return Mode() & S_IWUSR & S_IWGRP & S_IWOTH; }
  inline bool POSIXFile::A_Execute () const { return Mode() & S_IXUSR & S_IXGRP & S_IXOTH; }
  inline mode_t POSIXFile::Mode (bool const cache) const {
    return mode(cache) & PERM_MASK;
  }
  
  // File info.
  inline dev_t     POSIXFile::device    (bool cache) const { _check_cache(cache); return _stat.st_dev    ; }
  inline ino_t     POSIXFile::inode     (bool cache) const { _check_cache(cache); return _stat.st_ino    ; }
  inline mode_t    POSIXFile::mode      (bool cache) const { _check_cache(cache); return _stat.st_mode   ; }
  inline nlink_t   POSIXFile::links     (bool cache) const { _check_cache(cache); return _stat.st_nlink  ; }
  inline uid_t     POSIXFile::uid       (bool cache) const { _check_cache(cache); return _stat.st_uid    ; }
  inline gid_t     POSIXFile::gid       (bool cache) const { _check_cache(cache); return _stat.st_gid    ; }
  inline dev_t     POSIXFile::devid     (bool cache) const { _check_cache(cache); return _stat.st_rdev   ; }
  inline off_t     POSIXFile::size      (bool cache) const { _check_cache(cache); return _stat.st_size   ; }
  inline blksize_t POSIXFile::blockSize (bool cache) const { _check_cache(cache); return _stat.st_blksize; }
  inline blkcnt_t  POSIXFile::blocks    (bool cache) const { _check_cache(cache); return _stat.st_blocks ; }
  inline time_t    POSIXFile::atime     (bool cache) const { _check_cache(cache); return _stat.st_atime  ; }
  inline time_t    POSIXFile::mtime     (bool cache) const { _check_cache(cache); return _stat.st_mtime  ; }
  inline time_t    POSIXFile::ctime     (bool cache) const { _check_cache(cache); return _stat.st_ctime  ; }
  
  // Update cache if necessary.
  inline void POSIXFile::_check_cache (bool cache) const { if (!cache || !_cache_valid) { _update_cache(); } }
  
  // Seek and update the current file position.
  inline void POSIXFile::_seek_update_pos (off_t const offset, SeekMode const whence) {
    _seek(offset, whence);
    switch (whence) {
      case SM_SET: _curpos  = offset         ; break;
      case SM_CUR: _curpos += offset         ; break;
      case SM_END: _curpos  = size() + offset; break;
    };
  }
  
  // _FD constructor.
  inline POSIXFile::_FD::_FD (_FDType const fd) { set(fd); }
  
  // _FD destructor.
  inline POSIXFile::_FD::~_FD () { set(0); }
  
  // _FD assignment operator.
  inline POSIXFile::_FD& POSIXFile::_FD::operator = (_FDType const right) { set(right); return *this; }
  
  // _FD cast operator.
  inline POSIXFile::_FD::operator _FDType () const { return _fd; }
  
  // Set a new fd.
  inline int POSIXFile::_FD::set (_FDType const fd) {
    int retval = 0;
    if (fd != _fd) {
      if (_fd) {
        retval = ::close(_fd);
      }
      _fd = fd;
    }
    return retval;
  }
  
}

#endif

