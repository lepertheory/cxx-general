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
      void open () const;
      
      // Get the filename itself.
      std::string basename () const;
      
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
      
      // Results of stat().
      mutable struct stat _stat;
      
      // Whether the stat cache is valid.
      mutable bool _cache_valid;
      
      // Open in append mode if opening for writing. Defaults to true.
      bool _append;
      
      // Create a non-existant file on open. Defaults to true.
      bool _create;
      
      // Open fails if the file already exists.
      bool _exclusive;
      
      // Update access time on read. Defaults to true.
      bool _doatime;
      
      // Allow this file to become the controlling process's terminal.
      // Defaults to true.
      bool _canctty;
      
      // Follow symlinks when opening. Defaults to true.
      bool _followsym;
      
      // Open for synchronous I/O.
      bool _synch;
      
      // Truncate the file when opening for write.
      bool _truncate;
      
      /***********************************************************************/
      // Function members.
      
      // Update the cache if necessary.
      void _check_cache (bool cache = true) const;
      
      // Update the cache now.
      void _update_cache () const;
    
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
  
  // Set / get the append mode.
  inline POSIXFile& POSIXFile::Append (bool const append)       { _append = append; return *this; }
  inline bool       POSIXFile::Append (                 ) const { return _append;                 }
  
  // Set / get whether to create a non-existant file on open.
  inline POSIXFile& POSIXFile::Create (bool const create)       { _create = create; return *this; }
  inline bool       POSIXFile::Create (                 ) const { return _create;                 }
  
  // Set / get whether we want exclusive access to this file.
  inline POSIXFile& POSIXFile::Exclusive (bool const exclusive)       { _exclusive = exclusive; return *this; }
  inline bool       POSIXFile::Exclusive (                    ) const { return _exclusive;                    }
  
  // Set / get whether we are updating access time on read.
  inline POSIXFile& POSIXFile::DoATime (bool const doatime)       { _doatime = doatime; return *this; }
  inline bool       POSIXFile::DoATime (                  ) const { return _doatime;                  }
  
  // Set / get whether this is allowed to become the process's controlling terminal.
  inline POSIXFile& POSIXFile::CanCTTY (bool const canctty)       { _canctty = canctty; return *this; }
  inline bool       POSIXFile::CanCTTY (                  ) const { return _canctty;                  }
  
  // Follow symlinks when opening the file.
  inline POSIXFile& POSIXFile::FollowSym (bool const followsym)       { _followsym = followsym; return *this; }
  inline bool       POSIXFile::FollowSym (                    ) const { return _followsym;                    }
  
  // Use synchronous I/O.
  inline POSIXFile& POSIXFile::Synch (bool const synch)       { _synch = synch; return *this; }
  inline bool       POSIXFile::Synch (                ) const { return _synch;                }
  
  // Truncate the file when opening for write.
  inline POSIXFile& POSIXFile::Truncate (bool const truncate)       { _truncate = truncate; return *this; }
  inline bool       POSIXFile::Truncate (                   ) const { return _truncate;                   }
  
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
        retval = close(_fd);
      }
      _fd = fd;
    }
    return retval;
  }
  
}

#endif

