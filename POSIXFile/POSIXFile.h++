/*****************************************************************************
 * POSIXFile.h++
 *****************************************************************************
 * Interface for class POSIXFile.
 *****************************************************************************/

// Include guard.
#if !defined(POSIXFILE_234b734278udud8hb48)
  #define POSIXFILE_234b734278udud8hb48

// STL includes.
  #include <string>
  #include <fstream>
  #include <cstring>

// System includes.
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <cxx-general/Exception.h++>
  #include <cxx-general/toString.h++>
  #include <cxx-general/AutoArray.h++>
  #include <cxx-general/getErrorText.h++>

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
      
      // Cache mode.
      enum CacheMode { CACHE_NO = 0, CACHE_DO = 1 };
      
      /***********************************************************************/
      // Constants.
      
      // Directory separator.
      static char const DIR_SEP;
      
      // Do not change UID/GID.
      static uid_t const UID_NOCHANGE;
      static gid_t const GID_NOCHANGE;
      
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
          
          // Invalid operation on a non-opened file.
          class NotOpen : public Base {
            public:
              virtual ~NotOpen () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Requested " + _op + " operation cannot be performed on non-open file \"" + _filename + "\".").c_str();
                } catch (...) {
                  return "Requested operation cannot be performed on a non-opened file. Error creating message string.";
                }
              };
              NotOpen& Filename  (std::string const& filename) { _filename = filename; return *this; }
              NotOpen& Operation (std::string const& op      ) { _op       = op      ; return *this; }
              std::string Filename  () const { return _filename; }
              std::string Operation () const { return _op      ; }
            private:
              std::string _filename;
              std::string _op      ;
          };  
          
          // Error making a system call.
          class SysCallError : public Base {
            public:
              virtual ~SysCallError () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("System error number " + DAC::toString(_errno) + ", \"" + getErrorText(_errno) + "\" occured when attempting \"" + _syscall + "\" on file \"" + _filename + "\".").c_str();
                } catch (...) {
                  try {
                    return ("System error number " + DAC::toString(_errno) + " occured when attempting \"" + _syscall + "\" on file \"" + _filename + "\". Error fetching system message string.").c_str();
                  } catch (...) {
                    return "Error making system call. Error creating message string.";
                  }
                }
              };
              SysCallError& Errno    (int         const  errnum  ) { _errno    = errnum  ; return *this; };
              SysCallError& SysCall  (std::string const& syscall ) { _syscall  = syscall ; return *this; };
              SysCallError& Filename (std::string const& filename) { _filename = filename; return *this; };
              int         Errno   () const { return _errno   ; };
              std::string SysCall () const { return _syscall ; };
              std::string File    () const { return _filename; };
            private:
              int         _errno   ;
              std::string _syscall ;
              std::string _filename;
          };
          class AccessDenied  : public SysCallError {};
          class BadDescriptor : public SysCallError {};
          class CannotSeek    : public SysCallError {};
          class FileBusy      : public SysCallError {};
          class FileExists    : public SysCallError {};
          class FileOverflow  : public SysCallError {};
          class IOError       : public SysCallError {};
          class Interrupted   : public SysCallError {};
          class Invalid       : public SysCallError {};
          class IsDirectory   : public SysCallError {};
          class NameTooLong   : public SysCallError {};
          class NotPermitted  : public SysCallError {};
          class NoSpace       : public SysCallError {};
          class NotDirectory  : public SysCallError {};
          class OutOfMemory   : public SysCallError {};
          class PathNonExist  : public SysCallError {};
          class ProcMaxFiles  : public SysCallError {};
          class ReadOnlyFS    : public SysCallError {};
          class SymlinkLoop   : public SysCallError {};
          class SysMaxFiles   : public SysCallError {};
          class TryAgain      : public SysCallError {};
          class Unexpected    : public SysCallError {};
        
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
      POSIXFile& Mode      (mode_t      const  new_mode );
      POSIXFile& UID       (uid_t       const  new_uid  );
      POSIXFile& GID       (gid_t       const  new_gid  );
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
      mode_t      Mode      (CacheMode const cache = CACHE_DO) const;
      uid_t       UID       () const;
      gid_t       GID       () const;
      
      // Reset to just-constructed defaults.
      void clear ();
      
      // Copy a given File object. Does not copy the file itself.
      void copy (POSIXFile const& source);
      
      // Open the file.
      void open ();
      
      // Close the file.
      void close ();
      
      // Delete the file.
      void unlink ();
      void rm     ();
      void del    ();
      
      // Change the permissions of the file.
      void chmod (mode_t const new_mode);
      
      // Change the owner of the file.
      void chown (uid_t const owner = UID_NOCHANGE, gid_t const group = GID_NOCHANGE);
      
      // Get the filename itself.
      std::string basename () const;
      
      // Seek to a particular location.
      void seek (off_t const offset, SeekMode const whence);
      
      // Read the entire file as a string.
      std::string get_file ();
      
      // File info.
      bool is_blockDev        () const;
      bool is_charDev         () const;
      bool is_dir             () const;
      bool is_directory       () const;
      bool is_executable      () const;
      bool is_executable_real () const;
      bool is_exist           () const;
      bool is_file            () const;
      bool is_groupOwned      () const;
      bool is_userOwned       () const;
      bool is_pipe            () const;
      bool is_readable        () const;
      bool is_readable_real   () const;
      bool is_setGID          () const;
      bool is_setUID          () const;
      bool is_socket          () const;
      bool is_sticky          () const;
      bool is_symlink         () const;
      bool is_writable        () const;
      bool is_writable_real   () const;
      bool is_zero            () const;
      
      // stat() info.
      dev_t     device    (CacheMode const cache = CACHE_DO) const;
      ino_t     inode     (CacheMode const cache = CACHE_DO) const;
      mode_t    mode      (CacheMode const cache = CACHE_DO) const;
      nlink_t   links     (CacheMode const cache = CACHE_DO) const;
      uid_t     uid       (CacheMode const cache = CACHE_DO) const;
      gid_t     gid       (CacheMode const cache = CACHE_DO) const;
      dev_t     devid     (CacheMode const cache = CACHE_DO) const;
      off_t     size      (CacheMode const cache = CACHE_DO) const;
      blksize_t blockSize (CacheMode const cache = CACHE_DO) const;
      blkcnt_t  blocks    (CacheMode const cache = CACHE_DO) const;
      time_t    atime     (CacheMode const cache = CACHE_DO) const;
      time_t    mtime     (CacheMode const cache = CACHE_DO) const;
      time_t    ctime     (CacheMode const cache = CACHE_DO) const;
    
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
      
      static mode_t const _PERM_MASK;
      
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
      void _check_cache (CacheMode const cache = CACHE_DO) const;
      
      // Update the cache now.
      void _update_cache () const;
      
      // Wrapper around ::read().
      ssize_t _read (void* const buf, off_t const bufsize);
      
      // Seek to a particular offset.
      void _seek            (off_t const offset, SeekMode const whence = SM_SET);
      void _seek_update_pos (off_t const offset, SeekMode const whence = SM_SET);
      
      /***********************************************************************/
      // Static members.

      // Handle and throw a system call error.
      static void s_throwSysCallError (int const errnum, std::string const& syscall, std::string const& filename);
    
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
  inline POSIXFile& POSIXFile::Mode (mode_t const new_mode) {
    chmod(new_mode);
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
  inline mode_t POSIXFile::Mode (CacheMode const cache) const {
    return mode(cache) & _PERM_MASK;
  }
  
  // Set/get file owner & group.
  inline POSIXFile& POSIXFile::UID (uid_t const new_uid) { chown(new_uid     , GID_NOCHANGE); return *this; }
  inline POSIXFile& POSIXFile::GID (gid_t const new_gid) { chown(UID_NOCHANGE, new_gid     ); return *this; }
  inline uid_t POSIXFile::UID () const { return uid(); }
  inline gid_t POSIXFile::GID () const { return gid(); }
  
  // File info.
  inline dev_t     POSIXFile::device    (CacheMode const cache) const { _check_cache(cache); return _stat.st_dev    ; }
  inline ino_t     POSIXFile::inode     (CacheMode const cache) const { _check_cache(cache); return _stat.st_ino    ; }
  inline mode_t    POSIXFile::mode      (CacheMode const cache) const { _check_cache(cache); return _stat.st_mode   ; }
  inline nlink_t   POSIXFile::links     (CacheMode const cache) const { _check_cache(cache); return _stat.st_nlink  ; }
  inline uid_t     POSIXFile::uid       (CacheMode const cache) const { _check_cache(cache); return _stat.st_uid    ; }
  inline gid_t     POSIXFile::gid       (CacheMode const cache) const { _check_cache(cache); return _stat.st_gid    ; }
  inline dev_t     POSIXFile::devid     (CacheMode const cache) const { _check_cache(cache); return _stat.st_rdev   ; }
  inline off_t     POSIXFile::size      (CacheMode const cache) const { _check_cache(cache); return _stat.st_size   ; }
  inline blksize_t POSIXFile::blockSize (CacheMode const cache) const { _check_cache(cache); return _stat.st_blksize; }
  inline blkcnt_t  POSIXFile::blocks    (CacheMode const cache) const { _check_cache(cache); return _stat.st_blocks ; }
  inline time_t    POSIXFile::atime     (CacheMode const cache) const { _check_cache(cache); return _stat.st_atime  ; }
  inline time_t    POSIXFile::mtime     (CacheMode const cache) const { _check_cache(cache); return _stat.st_mtime  ; }
  inline time_t    POSIXFile::ctime     (CacheMode const cache) const { _check_cache(cache); return _stat.st_ctime  ; }
  
  // Delete the file.
  inline void POSIXFile::rm  () { unlink(); }
  inline void POSIXFile::del () { unlink(); }
  
  // Update cache if necessary.
  inline void POSIXFile::_check_cache (CacheMode const cache) const { if (!cache || !_cache_valid) { _update_cache(); } }
  
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

