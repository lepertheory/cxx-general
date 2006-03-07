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
  #include <vector>

// System includes.
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <Exception.h++>
  #include <toString.h++>
  #include <AutoArray.h++>
  #include <getErrorText.h++>

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
      enum OpenMode {
        OM_READ     ,
        OM_WRITE    ,
        OM_READWRITE
      };
      
      // Seek modes.
      enum SeekMode {
        SM_SET = SEEK_SET,
        SM_CUR = SEEK_CUR,
        SM_END = SEEK_END
      };
      
      // File types.
      enum FileType {
        FT_SOCKET    = S_IFSOCK,
        FT_SYMLINK   = S_IFLNK ,
        FT_FILE      = S_IFREG ,
        FT_BLOCKDEV  = S_IFBLK ,
        FT_DIRECTORY = S_IFDIR ,
        FT_CHARDEV   = S_IFCHR ,
        FT_FIFO      = S_IFIFO
      };
      
      // File descriptor type.
      typedef int FDType;
      
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
          
          // No file was given to operate on.
          class NoFile : public Base {
            public:
              virtual ~NoFile () throw() {};
              virtual char const* what () const throw() { return "No filename was provided."; };
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
              EoF& Operation (std::string const& op) { _op = op; return *this; };
              std::string Operation () const { return _op; };
            private:
              std::string _op;
          };
          
          // Offset overrun.
          class Overrun : public Base {
            public:
              virtual ~Overrun () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Offset " + toString(_offset) + " overruns end of file with size of " + DAC::toString(_size) + ".").c_str();
                } catch (...) {
                  return "Offset overruns end of file. Error creating message string.";
                }
              };
              Overrun& Offset   (off_t const offset) { _offset = offset; return *this; };
              Overrun& Size     (off_t const size  ) { _size   = size  ; return *this; };
              off_t Offset () const { return _offset; };
              off_t Size   () const { return _size  ; };
            private:
              off_t _offset;
              off_t _size  ;
          };
          
          // Error making a system call.
          class SysCallError : public Base {
            public:
              virtual ~SysCallError () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("System error number " + toString(_errno) + ", \"" + getErrorText(_errno) + "\" occured when attempting \"" + _syscall + "\".").c_str();
                } catch (...) {
                  try {
                    return ("System error number " + toString(_errno) + " occured when attempting \"" + _syscall + "\". Error fetching system message string.").c_str();
                  } catch (...) {
                    return "Error making system call. Error creating message string.";
                  }
                }
              };
              SysCallError& Errno   (int         const  errnum ) { _errno   = errnum ; return *this; };
              SysCallError& SysCall (std::string const& syscall) { _syscall = syscall; return *this; };
              int         Errno   () const { return _errno  ; };
              std::string SysCall () const { return _syscall; };
            private:
              int         _errno  ;
              std::string _syscall;
          };
          class AccessDenied  : public SysCallError {};
          class BadDescriptor : public SysCallError {};
          class CannotSeek    : public SysCallError {};
          class Deadlock      : public SysCallError {};
          class FileBusy      : public SysCallError {};
          class FileExists    : public SysCallError {};
          class FileOverflow  : public SysCallError {};
          class IOError       : public SysCallError {};
          class Interrupted   : public SysCallError {};
          class Invalid       : public SysCallError {};
          class IsDirectory   : public SysCallError {};
          class LockTableFull : public SysCallError {};
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
          
          // Unknown file type returned.
          class UnknownType : public Base {
            public:
              virtual ~UnknownType () throw() {};
              virtual char const* what () const throw() {
                try {
                  return ("Unknown file type " + toString(_type) + " returned.").c_str();
                } catch (...) {
                  return "Unknown file type returned. Error creating message string.";
                }
              };
              UnknownType& Type (mode_t const filetype) { _type = filetype; return *this; };
              mode_t Type () const { return _type; };
            private:
              std::string _filename;
              mode_t      _type    ;
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
      
      // Constructor with a file descriptor.
      POSIXFile (FDType const fd);
      
      // Copy constructor.
      POSIXFile (POSIXFile const& source);
      
      // Assignment operator.
      POSIXFile& operator = (POSIXFile const& right);
      POSIXFile& operator = (FDType    const  fd   );
      
      // Properties.
      POSIXFile& FD        (FDType        const  fd       );
      POSIXFile& RecordSep (std::string   const& recordsep);
      POSIXFile& OpenMode  (enum OpenMode const  openmode );
      POSIXFile& Append    (bool          const  append   );
      POSIXFile& Create    (bool          const  create   );
      POSIXFile& Exclusive (bool          const  exclusive);
      POSIXFile& DoATime   (bool          const  doatime  );
      POSIXFile& CanCTTY   (bool          const  canctty  );
      POSIXFile& FollowSym (bool          const  followsym);
      POSIXFile& Synch     (bool          const  synch    );
      POSIXFile& Truncate  (bool          const  truncate );
      POSIXFile& SetUID    (bool          const  setuid   );
      POSIXFile& SetGID    (bool          const  setgid   );
      POSIXFile& Sticky    (bool          const  sticky   );
      POSIXFile& U_Read    (bool          const  u_read   );
      POSIXFile& U_Write   (bool          const  u_write  );
      POSIXFile& U_Execute (bool          const  u_execute);
      POSIXFile& G_Read    (bool          const  g_read   );
      POSIXFile& G_Write   (bool          const  g_write  );
      POSIXFile& G_Execute (bool          const  g_execute);
      POSIXFile& O_Read    (bool          const  o_read   );
      POSIXFile& O_Write   (bool          const  o_write  );
      POSIXFile& O_Execute (bool          const  o_execute);
      POSIXFile& A_Read    (bool          const  a_read   );
      POSIXFile& A_Write   (bool          const  a_write  );
      POSIXFile& A_Execute (bool          const  a_execute);
      POSIXFile& Mode      (mode_t        const  new_mode );
      POSIXFile& UID       (uid_t         const  new_uid  );
      POSIXFile& GID       (gid_t         const  new_gid  );
      FDType        FD        () const;
      std::string   RecordSep () const;
      enum OpenMode OpenMode  () const;
      bool          Append    () const;
      bool          Create    () const;
      bool          Exclusive () const;
      bool          DoATime   () const;
      bool          CanCTTY   () const;
      bool          FollowSym () const;
      bool          Synch     () const;
      bool          Truncate  () const;
      bool          SetUID    () const;
      bool          SetGID    () const;
      bool          Sticky    () const;
      bool          U_Read    () const;
      bool          U_Write   () const;
      bool          U_Execute () const;
      bool          G_Read    () const;
      bool          G_Write   () const;
      bool          G_Execute () const;
      bool          O_Read    () const;
      bool          O_Write   () const;
      bool          O_Execute () const;
      bool          A_Read    () const;
      bool          A_Write   () const;
      bool          A_Execute () const;
      mode_t        Mode      () const;
      uid_t         UID       () const;
      gid_t         GID       () const;
      FileType      Type      () const;
      
      // Reset to just-constructed defaults.
      void clear ();
      
      // Copy a given File object. Does not copy the file itself.
      void copy (POSIXFile const& source);
      
      // Open the file.
      void open (                           );
      void open (std::string const& filename);
      
      // Close the file.
      void close ();
      
      // Check if the file has been opened.
      bool is_open () const;
      
      // Lock the file.
      bool lock (bool const shared = false, bool const wait = true, off_t const start = 0, off_t const len = 0);
      
      // Unlock the file.
      void unlock (off_t const start = 0, off_t const len = 0);
      
      // Test for a file lock.
      bool is_locked (bool const shared = true, off_t const start = 0, off_t const len = 0);
      
      // Change the permissions of the file.
      void chmod (mode_t const new_mode);
      
      // Change the owner of the file.
      void chown (uid_t const owner = UID_NOCHANGE, gid_t const group = GID_NOCHANGE);
      
      // Truncate or expand to an exact length.
      void truncate (off_t const length);
      
      // Seek to a particular location.
      void seek (off_t const offset, SeekMode const whence = SM_SET);
      
      // Return the current read/write offset.
      off_t tell () const;
      
      // Read a particular number of bytes.
      std::string read (size_t const bytes);
      
      // Read a line.
      std::string read_line (bool const trim = false);
      
      // Read the entire file as a vector of lines.
      std::vector<std::string>& read_all_lines (std::vector<std::string>& buffer);
      
      // Read the entire file as a string.
      std::string read_file ();
      
      // Write.
      ssize_t write (std::string const& data                    );
      ssize_t write (void const* const  data, size_t const bytes);
      
      // Detect EOF.
      bool eof      () const;
      bool eof_line () const;
      
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
      dev_t     device    () const;
      ino_t     inode     () const;
      mode_t    mode      () const;
      nlink_t   links     () const;
      uid_t     uid       () const;
      gid_t     gid       () const;
      dev_t     devid     () const;
      off_t     size      () const;
      blksize_t blockSize () const;
      blkcnt_t  blocks    () const;
      time_t    atime     () const;
      time_t    mtime     () const;
      time_t    ctime     () const;
      
      /***********************************************************************/
      // Static function members.
      
      // Properties.
      static void SetUID    (std::string const& filename, bool   const setuid   );
      static void SetGID    (std::string const& filename, bool   const setgid   );
      static void Sticky    (std::string const& filename, bool   const sticky   );
      static void U_Read    (std::string const& filename, bool   const u_read   );
      static void U_Write   (std::string const& filename, bool   const u_write  );
      static void U_Execute (std::string const& filename, bool   const u_execute);
      static void G_Read    (std::string const& filename, bool   const g_read   );
      static void G_Write   (std::string const& filename, bool   const g_write  );
      static void G_Execute (std::string const& filename, bool   const g_execute);
      static void O_Read    (std::string const& filename, bool   const o_read   );
      static void O_Write   (std::string const& filename, bool   const o_write  );
      static void O_Execute (std::string const& filename, bool   const o_execute);
      static void A_Read    (std::string const& filename, bool   const a_read   );
      static void A_Write   (std::string const& filename, bool   const a_write  );
      static void A_Execute (std::string const& filename, bool   const a_execute);
      static void Mode      (std::string const& filename, mode_t const new_mode );
      static void UID       (std::string const& filename, uid_t  const new_uid  );
      static void GID       (std::string const& filename, gid_t  const new_gid  );
      static bool     SetUID    (std::string const& filename);
      static bool     SetGID    (std::string const& filename);
      static bool     Sticky    (std::string const& filename);
      static bool     U_Read    (std::string const& filename);
      static bool     U_Write   (std::string const& filename);
      static bool     U_Execute (std::string const& filename);
      static bool     G_Read    (std::string const& filename);
      static bool     G_Write   (std::string const& filename);
      static bool     G_Execute (std::string const& filename);
      static bool     O_Read    (std::string const& filename);
      static bool     O_Write   (std::string const& filename);
      static bool     O_Execute (std::string const& filename);
      static bool     A_Read    (std::string const& filename);
      static bool     A_Write   (std::string const& filename);
      static bool     A_Execute (std::string const& filename);
      static mode_t   Mode      (std::string const& filename);
      static uid_t    UID       (std::string const& filename);
      static gid_t    GID       (std::string const& filename);
      static FileType Type      (std::string const& filename);
      
      // Create a hard link.
      static void link (std::string const& src, std::string const& dest);
      
      // Create a symbolic link.
      static void symlink (std::string const& src, std::string const& dest);
      
      // Rename/move the file.
      static void rename (std::string const& src, std::string const& dest);
      static void mv     (std::string const& src, std::string const& dest);
      
      // Delete the file.
      static void unlink (std::string const& filename);
      static void rm     (std::string const& filename);
      static void del    (std::string const& filename);
      
      // Change the permissions of the file.
      static void chmod (std::string const& filename, mode_t const new_mode);
      
      // Change the owner of the file.
      static void chown (std::string const& filename, uid_t const owner = UID_NOCHANGE, gid_t const group = GID_NOCHANGE);
      
      // Truncate or expand to an exact length.
      static void truncate (std::string const& filename, off_t const length);
      
      // Get file / directory names.
      static std::string basename (std::string const& filename);
      static std::string dirname  (std::string const& filename);
      
      // Convert relative name to an absolute filename.
      static std::string expand_path (std::string const& filename);
      
      // Read the target of a symbolic link.
      static std::string readlink (std::string const filename);
      
      // File info.
      static bool is_blockDev        (std::string const& filename);
      static bool is_charDev         (std::string const& filename);
      static bool is_dir             (std::string const& filename);
      static bool is_directory       (std::string const& filename);
      static bool is_executable      (std::string const& filename);
      static bool is_executable_real (std::string const& filename);
      static bool is_exist           (std::string const& filename);
      static bool is_file            (std::string const& filename);
      static bool is_groupOwned      (std::string const& filename);
      static bool is_userOwned       (std::string const& filename);
      static bool is_pipe            (std::string const& filename);
      static bool is_readable        (std::string const& filename);
      static bool is_readable_real   (std::string const& filename);
      static bool is_setGID          (std::string const& filename);
      static bool is_setUID          (std::string const& filename);
      static bool is_socket          (std::string const& filename);
      static bool is_sticky          (std::string const& filename);
      static bool is_symlink         (std::string const& filename);
      static bool is_writable        (std::string const& filename);
      static bool is_writable_real   (std::string const& filename);
      static bool is_zero            (std::string const& filename);
      
      // stat() info.
      static dev_t     device    (std::string const& filename);
      static ino_t     inode     (std::string const& filename);
      static mode_t    mode      (std::string const& filename);
      static nlink_t   links     (std::string const& filename);
      static uid_t     uid       (std::string const& filename);
      static gid_t     gid       (std::string const& filename);
      static dev_t     devid     (std::string const& filename);
      static off_t     size      (std::string const& filename);
      static blksize_t blockSize (std::string const& filename);
      static blkcnt_t  blocks    (std::string const& filename);
      static time_t    atime     (std::string const& filename);
      static time_t    mtime     (std::string const& filename);
      static time_t    ctime     (std::string const& filename);
      
      // Get the current working directory.
      static std::string getCWD ();
    
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data types.
      
      // Open mode flag type.
      typedef int _OMFlagType;
      
      /***********************************************************************/
      // Constants.
      
      // Hard minimum / maximum of path lengths.
      static size_t const _MIN_PATH;
      static size_t const _MAX_PATH;
      
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
          _FD (FDType const fd = -1);
          
          // Destructor.
          ~_FD ();
          
          // Assignment operator.
          _FD& operator = (FDType const right);
          
          // Casting operator, use _FD just like a normal fd.
          operator FDType () const;
          
          // Set a new fd.
          int set (FDType const fd);
          
        /*
         * Private members.
         */
        private:
          
          /*******************************************************************/
          // Data members.
          
          // Guess.
          FDType _fd;
          
          /*******************************************************************/
          // Function members.
          
          // Copying is not allowed.
          _FD (_FD const&);
          _FD& operator = (_FD const&);
        
      };
      
      /***********************************************************************/
      // Constants.
      
      // Mask to get only permissions out of a mode.
      static mode_t const _PERM_MASK;
      
      /***********************************************************************/
      // Data members.
      
      // The name of the file.
      std::string _filename;
      
      // Record info.
      std::string _recordsep;
      off_t       _recordnum;
      
      // File descriptor. 0 if file is closed.
      _FD _fd;
      
      // Current file read/write position.
      off_t _curpos;
      
      // Results of stat().
      mutable struct stat _stat;
      
      // Whether the stat cache is valid.
      mutable bool _cache_valid;
      
      // Open flags.
      _OMFlagType   _flags     ;
      enum OpenMode _openmode  ;
      mode_t        _createmode;
      
      // End of file.
      bool _eof     ;
      bool _eof_line;
      
      /***********************************************************************/
      // Function members.
      
      // Update the cache if necessary.
      void _check_cache () const;
      
      // Update the cache now.
      void _update_cache () const;
      
      // Wrapper around ::read().
      ssize_t _read (void* const buf, off_t const bufsize);
      
      // Seek to a particular offset.
      void _seek            (off_t const offset, SeekMode const whence = SM_SET);
      void _seek_update_pos (off_t const offset, SeekMode const whence = SM_SET);
      
      /***********************************************************************/
      // Static members.
      
      // stat() a file.
      static struct stat* s_stat     (std::string const& filename, struct stat* const buf);
      static struct stat* s_try_stat (std::string const& filename, struct stat* const buf);
      
      // Handle and throw a system call error.
      static void s_throwSysCallError (int const errnum, std::string const& syscall, std::string const& filename);
      
      // Compress redundant directory separators.
      static void s_compress_dirSep (std::string& path);
      
      // Convert from stat() file type to POSIXFile::FileType.
      static FileType s_convert_FileType (mode_t const type);
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.

  /*
   * Default constructor.
   */
  inline POSIXFile::POSIXFile () { clear(); }
  
  /*
   * Constructor names the file.
   */
  inline POSIXFile::POSIXFile (std::string const& filename) { clear(); _filename = filename; }
  
  /*
   * Constructor with a file descriptor.
   */
  inline POSIXFile::POSIXFile (FDType const fd) { clear(); _fd = fd; }
  
  /*
   * Copy constructor.
   */
  inline POSIXFile::POSIXFile (POSIXFile const& source) { copy(source); }
  
  /*
   * Assignment operator.
   */
  inline POSIXFile& POSIXFile::operator = (POSIXFile const& right) { copy(right)         ; return *this; }
  inline POSIXFile& POSIXFile::operator = (FDType    const  right) { clear(); _fd = right; return *this; }
  
  /*
   * Set / get the record separator.
   */
  inline POSIXFile& POSIXFile::RecordSep (std::string const& recordsep) {
    if (recordsep != _recordsep) {
      _recordnum = 0;
      _recordsep = recordsep;
    }
    return *this;
  }
  inline std::string POSIXFile::RecordSep () const { return _recordsep; }
  
  /*
   * Set / get the open flags.
   */
  inline POSIXFile& POSIXFile::OpenMode (enum OpenMode const openmode) { _openmode = openmode; return *this; }
  inline POSIXFile& POSIXFile::Append    (bool const append   ) { if (append   ) { _flags |=  O_APPEND  ; } else { _flags &= ~O_APPEND  ; } return *this; }
  inline POSIXFile& POSIXFile::Create    (bool const create   ) { if (create   ) { _flags |=  O_CREAT   ; } else { _flags &= ~O_CREAT   ; } return *this; }
  inline POSIXFile& POSIXFile::Exclusive (bool const exclusive) { if (exclusive) { _flags |=  O_EXCL    ; } else { _flags &= ~O_EXCL    ; } return *this; }
  inline POSIXFile& POSIXFile::DoATime   (bool const doatime  ) { if (doatime  ) { _flags &= ~O_NOATIME ; } else { _flags |=  O_NOATIME ; } return *this; }
  inline POSIXFile& POSIXFile::CanCTTY   (bool const canctty  ) { if (canctty  ) { _flags &= ~O_NOCTTY  ; } else { _flags |=  O_NOCTTY  ; } return *this; }
  inline POSIXFile& POSIXFile::FollowSym (bool const followsym) {
    if (followsym != FollowSym()) {
      _cache_valid = false;
      if (followsym) {
        _flags &= ~O_NOFOLLOW;
      } else {
        _flags |=  O_NOFOLLOW;
      }
    }
    return *this;
  }
  inline POSIXFile& POSIXFile::Synch     (bool const synch    ) { if (synch    ) { _flags |=  O_SYNC    ; } else { _flags &= ~O_SYNC    ; } return *this; }
  inline POSIXFile& POSIXFile::Truncate  (bool const trunc    ) { if (trunc    ) { _flags |=  O_TRUNC   ; } else { _flags &= ~O_TRUNC   ; } return *this; }
  inline enum POSIXFile::OpenMode POSIXFile::OpenMode () const { return _openmode; }
  inline bool POSIXFile::Append    () const { return   _flags & O_APPEND   ; }
  inline bool POSIXFile::Create    () const { return   _flags & O_CREAT    ; }
  inline bool POSIXFile::Exclusive () const { return   _flags & O_EXCL     ; }
  inline bool POSIXFile::DoATime   () const { return !(_flags & O_NOATIME ); }
  inline bool POSIXFile::CanCTTY   () const { return !(_flags & O_NOCTTY  ); }
  inline bool POSIXFile::FollowSym () const { return !(_flags & O_NOFOLLOW); }
  inline bool POSIXFile::Synch     () const { return   _flags & O_SYNC     ; }
  inline bool POSIXFile::Truncate  () const { return   _flags & O_TRUNC    ; }
  
  /*
   * Set file modes.
   */
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
  
  /*
   * Get file modes.
   */
  inline bool POSIXFile::SetUID    () const { return Mode() & S_ISUID; }
  inline bool POSIXFile::SetGID    () const { return Mode() & S_ISGID; }
  inline bool POSIXFile::Sticky    () const { return Mode() & S_ISVTX; }
  inline bool POSIXFile::U_Read    () const { return Mode() & S_IRUSR; }
  inline bool POSIXFile::U_Write   () const { return Mode() & S_IWUSR; }
  inline bool POSIXFile::U_Execute () const { return Mode() & S_IXUSR; }
  inline bool POSIXFile::G_Read    () const { return Mode() & S_IRGRP; }
  inline bool POSIXFile::G_Write   () const { return Mode() & S_IWGRP; }
  inline bool POSIXFile::G_Execute () const { return Mode() & S_IXGRP; }
  inline bool POSIXFile::O_Read    () const { return Mode() & S_IROTH; }
  inline bool POSIXFile::O_Write   () const { return Mode() & S_IWOTH; }
  inline bool POSIXFile::O_Execute () const { return Mode() & S_IXOTH; }
  inline bool POSIXFile::A_Read    () const { mode_t tmp = Mode(); return tmp & S_IRUSR && tmp & S_IRGRP && tmp & S_IROTH; }
  inline bool POSIXFile::A_Write   () const { mode_t tmp = Mode(); return tmp & S_IWUSR && tmp & S_IWGRP && tmp & S_IWOTH; }
  inline bool POSIXFile::A_Execute () const { mode_t tmp = Mode(); return tmp & S_IXUSR && tmp & S_IXGRP && tmp & S_IXOTH; }
  inline mode_t POSIXFile::Mode () const {
    return mode() & _PERM_MASK;
  }
  
  /*
   * Set/get file owner & group.
   */
  inline POSIXFile& POSIXFile::UID (uid_t const new_uid) { chown(new_uid     , GID_NOCHANGE); return *this; }
  inline POSIXFile& POSIXFile::GID (gid_t const new_gid) { chown(UID_NOCHANGE, new_gid     ); return *this; }
  inline uid_t POSIXFile::UID () const { return uid(); }
  inline gid_t POSIXFile::GID () const { return gid(); }
  
  /*
   * Get the file type.
   */
  inline POSIXFile::FileType POSIXFile::Type () const { return s_convert_FileType(mode() & S_IFMT); }
  
  /*
   * Open a given file.
   */
  inline void POSIXFile::open (std::string const& filename) { _filename = filename; open(); }
  
  /*
   * Check if the file has been opened.
   */
  inline bool POSIXFile::is_open () const { return _fd == -1; }
  
  /*
   * Seek to a particular location.
   */
  inline void POSIXFile::seek (off_t const offset, SeekMode const whence) {
    if (!is_open()) {
      open();
    }
    _seek(offset, whence);
  }
  
  /*
   * Return the current read/write position.
   */
  inline off_t POSIXFile::tell () const { return _curpos; }
  
  /*
   * Write.
   */
  inline ssize_t POSIXFile::write (std::string const& data) { return write(data.data(), data.length()); }
  
  /*
   * End of file.
   */
  inline bool POSIXFile::eof      () const { return _eof             ; }
  inline bool POSIXFile::eof_line () const { return _eof && _eof_line; }
  
  /*
   * File info.
   */
  inline bool POSIXFile::is_blockDev   () const { return is_exist() && ((mode() & S_IFMT) & S_IFBLK ) == S_IFBLK ; }
  inline bool POSIXFile::is_charDev    () const { return is_exist() && ((mode() & S_IFMT) & S_IFCHR ) == S_IFCHR ; }
  inline bool POSIXFile::is_dir        () const { return is_exist() && ((mode() & S_IFMT) & S_IFDIR ) == S_IFDIR ; }
  inline bool POSIXFile::is_directory  () const { return is_dir();                                                 }
  inline bool POSIXFile::is_file       () const { return is_exist() && ((mode() & S_IFMT) & S_IFREG ) == S_IFREG ; }
  inline bool POSIXFile::is_pipe       () const { return is_exist() && ((mode() & S_IFMT) & S_IFIFO ) == S_IFIFO ; }
  inline bool POSIXFile::is_socket     () const { return is_exist() && ((mode() & S_IFMT) & S_IFSOCK) == S_IFSOCK; }
  inline bool POSIXFile::is_symlink    () const { return is_exist() && ((mode() & S_IFMT) & S_IFLNK ) == S_IFLNK ; }
  inline bool POSIXFile::is_groupOwned () const { return is_exist() && gid() == getegid(); }
  inline bool POSIXFile::is_userOwned  () const { return is_exist() && uid() == geteuid(); }
  inline bool POSIXFile::is_setGID     () const { return is_exist() && SetGID(); }
  inline bool POSIXFile::is_setUID     () const { return is_exist() && SetUID(); }
  inline bool POSIXFile::is_sticky     () const { return is_exist() && Sticky(); }
  
  inline bool POSIXFile::is_executable () const {
    return (geteuid() == 0) ? (
      is_exist() && (U_Execute() || G_Execute() || O_Execute())
    ) : (
      (is_userOwned () && U_Execute()) ||
      (is_groupOwned() && G_Execute()) ||
      (is_exist     () && O_Execute())
    );
  }
  inline bool POSIXFile::is_executable_real () const {
    return is_exist() && ((getuid() == 0) ? (
      U_Execute() || G_Execute() || O_Execute()
    ) : (
      (uid() == getuid() && U_Execute()) ||
      (gid() == getgid() && G_Execute()) ||
                            O_Execute()
    ));
  }
  inline bool POSIXFile::is_readable () const {
    return (geteuid() == 0 && is_exist()) ||
           (is_userOwned () && U_Read() ) ||
           (is_groupOwned() && G_Read() ) ||
           (is_exist     () && O_Read() );
  }
  inline bool POSIXFile::is_readable_real () const {
    return is_exist() && (
      (getuid() == 0                ) ||
      (uid() == getuid() && U_Read()) ||
      (gid() == getgid() && G_Read()) ||
                            O_Read()
    );
  }
  inline bool POSIXFile::is_writable () const {
    return (geteuid() == 0 && is_exist()) ||
           (is_userOwned () && U_Write()) ||
           (is_groupOwned() && G_Write()) ||
           (is_exist     () && O_Write());
  }
  inline bool POSIXFile::is_writable_real () const {
    return is_exist() && (
      (getuid == 0                   ) ||
      (uid() == getuid() && U_Write()) ||
      (gid() == getgid() && G_Write()) ||
                            O_Write()
    );
  }
  
  inline bool POSIXFile::is_exist () const {
    return !stat(_filename.c_str(), &_stat) && (_cache_valid = true);
  }
  inline bool POSIXFile::is_zero () const { return is_exist() && size() == 0; }
  
  /*
   * stat() info.
   */
  inline dev_t     POSIXFile::device    () const { _check_cache(); return _stat.st_dev    ; }
  inline ino_t     POSIXFile::inode     () const { _check_cache(); return _stat.st_ino    ; }
  inline mode_t    POSIXFile::mode      () const { _check_cache(); return _stat.st_mode   ; }
  inline nlink_t   POSIXFile::links     () const { _check_cache(); return _stat.st_nlink  ; }
  inline uid_t     POSIXFile::uid       () const { _check_cache(); return _stat.st_uid    ; }
  inline gid_t     POSIXFile::gid       () const { _check_cache(); return _stat.st_gid    ; }
  inline dev_t     POSIXFile::devid     () const { _check_cache(); return _stat.st_rdev   ; }
  inline off_t     POSIXFile::size      () const { _check_cache(); return _stat.st_size   ; }
  inline blksize_t POSIXFile::blockSize () const { _check_cache(); return _stat.st_blksize; }
  inline blkcnt_t  POSIXFile::blocks    () const { _check_cache(); return _stat.st_blocks ; }
  inline time_t    POSIXFile::atime     () const { _check_cache(); return _stat.st_atime  ; }
  inline time_t    POSIXFile::mtime     () const { _check_cache(); return _stat.st_mtime  ; }
  inline time_t    POSIXFile::ctime     () const { _check_cache(); return _stat.st_ctime  ; }
  
  /*
   * Update cache if necessary.
   */
  inline void POSIXFile::_check_cache () const { if (!_cache_valid) { _update_cache(); } }
  
  /***************************************************************************/
  // Static function members.
  
  /*
   * Set properties.
   */
  inline void POSIXFile::SetUID    (std::string const& filename, bool   const setuid   ) { if (setuid   ) { Mode(filename, Mode(filename) | S_ISUID); } else { Mode(filename, Mode(filename) & ~S_ISUID); } }
  inline void POSIXFile::SetGID    (std::string const& filename, bool   const setgid   ) { if (setgid   ) { Mode(filename, Mode(filename) | S_ISGID); } else { Mode(filename, Mode(filename) & ~S_ISGID); } }
  inline void POSIXFile::Sticky    (std::string const& filename, bool   const sticky   ) { if (sticky   ) { Mode(filename, Mode(filename) | S_ISVTX); } else { Mode(filename, Mode(filename) & ~S_ISVTX); } }
  inline void POSIXFile::U_Read    (std::string const& filename, bool   const u_read   ) { if (u_read   ) { Mode(filename, Mode(filename) | S_IRUSR); } else { Mode(filename, Mode(filename) & ~S_IRUSR); } }
  inline void POSIXFile::U_Write   (std::string const& filename, bool   const u_write  ) { if (u_write  ) { Mode(filename, Mode(filename) | S_IWUSR); } else { Mode(filename, Mode(filename) & ~S_IWUSR); } }
  inline void POSIXFile::U_Execute (std::string const& filename, bool   const u_execute) { if (u_execute) { Mode(filename, Mode(filename) | S_IXUSR); } else { Mode(filename, Mode(filename) & ~S_IXUSR); } }
  inline void POSIXFile::G_Read    (std::string const& filename, bool   const g_read   ) { if (g_read   ) { Mode(filename, Mode(filename) | S_IRGRP); } else { Mode(filename, Mode(filename) & ~S_IRGRP); } }
  inline void POSIXFile::G_Write   (std::string const& filename, bool   const g_write  ) { if (g_write  ) { Mode(filename, Mode(filename) | S_IWGRP); } else { Mode(filename, Mode(filename) & ~S_IWGRP); } }
  inline void POSIXFile::G_Execute (std::string const& filename, bool   const g_execute) { if (g_execute) { Mode(filename, Mode(filename) | S_IXGRP); } else { Mode(filename, Mode(filename) & ~S_IXGRP); } }
  inline void POSIXFile::O_Read    (std::string const& filename, bool   const o_read   ) { if (o_read   ) { Mode(filename, Mode(filename) | S_IROTH); } else { Mode(filename, Mode(filename) & ~S_IROTH); } }
  inline void POSIXFile::O_Write   (std::string const& filename, bool   const o_write  ) { if (o_write  ) { Mode(filename, Mode(filename) | S_IWOTH); } else { Mode(filename, Mode(filename) & ~S_IWOTH); } }
  inline void POSIXFile::O_Execute (std::string const& filename, bool   const o_execute) { if (o_execute) { Mode(filename, Mode(filename) | S_IXOTH); } else { Mode(filename, Mode(filename) & ~S_IXOTH); } }
  inline void POSIXFile::A_Read    (std::string const& filename, bool   const a_read   ) {
    if (a_read) {
      Mode(filename, Mode(filename) | S_IRUSR | S_IRGRP | S_IROTH);
    } else {
      Mode(filename, Mode(filename) & ~(S_IRUSR | S_IRGRP | S_IROTH));
    }
  }
  inline void POSIXFile::A_Write   (std::string const& filename, bool   const a_write  ) {
    if (a_write) {
      Mode(filename, Mode(filename) | S_IWUSR | S_IWGRP | S_IWOTH);
    } else {
      Mode(filename, Mode(filename) & ~(S_IWUSR | S_IWGRP | S_IWOTH));
    }
  }
  inline void POSIXFile::A_Execute (std::string const& filename, bool   const a_execute) {
    if (a_execute) {
      Mode(filename, Mode(filename) | S_IXUSR | S_IXGRP | S_IXOTH);
    } else {
      Mode(filename, Mode(filename) & ~(S_IXUSR | S_IXGRP | S_IXOTH));
    }
  }
  inline void POSIXFile::Mode      (std::string const& filename, mode_t const new_mode ) { chmod(filename, new_mode);                   }
  inline void POSIXFile::UID       (std::string const& filename, uid_t  const new_uid  ) { chown(filename, new_uid     , GID_NOCHANGE); }
  inline void POSIXFile::GID       (std::string const& filename, gid_t  const new_gid  ) { chown(filename, UID_NOCHANGE, new_gid     ); }
  
  /*
   * Get properties.
   */
  inline bool   POSIXFile::SetUID    (std::string const& filename) { return Mode(filename) & S_ISUID; }
  inline bool   POSIXFile::SetGID    (std::string const& filename) { return Mode(filename) & S_ISGID; }
  inline bool   POSIXFile::Sticky    (std::string const& filename) { return Mode(filename) & S_ISVTX; }
  inline bool   POSIXFile::U_Read    (std::string const& filename) { return Mode(filename) & S_IRUSR; }
  inline bool   POSIXFile::U_Write   (std::string const& filename) { return Mode(filename) & S_IWUSR; }
  inline bool   POSIXFile::U_Execute (std::string const& filename) { return Mode(filename) & S_IXUSR; }
  inline bool   POSIXFile::G_Read    (std::string const& filename) { return Mode(filename) & S_IRGRP; }
  inline bool   POSIXFile::G_Write   (std::string const& filename) { return Mode(filename) & S_IWGRP; }
  inline bool   POSIXFile::G_Execute (std::string const& filename) { return Mode(filename) & S_IXGRP; }
  inline bool   POSIXFile::O_Read    (std::string const& filename) { return Mode(filename) & S_IROTH; }
  inline bool   POSIXFile::O_Write   (std::string const& filename) { return Mode(filename) & S_IWOTH; }
  inline bool   POSIXFile::O_Execute (std::string const& filename) { return Mode(filename) & S_IXOTH; }
  inline bool   POSIXFile::A_Read    (std::string const& filename) { mode_t tmp = Mode(filename); return tmp & S_IRUSR && tmp & S_IRGRP && tmp & S_IROTH; }
  inline bool   POSIXFile::A_Write   (std::string const& filename) { mode_t tmp = Mode(filename); return tmp & S_IWUSR && tmp & S_IWGRP && tmp & S_IWOTH; }
  inline bool   POSIXFile::A_Execute (std::string const& filename) { mode_t tmp = Mode(filename); return tmp & S_IXUSR && tmp & S_IXGRP && tmp & S_IXOTH; }
  inline mode_t POSIXFile::Mode      (std::string const& filename) { return mode(filename) & _PERM_MASK; }
  inline uid_t  POSIXFile::UID       (std::string const& filename) { return uid(filename); }
  inline gid_t  POSIXFile::GID       (std::string const& filename) { return gid(filename); }
  inline POSIXFile::FileType POSIXFile::Type (std::string const& filename) { return s_convert_FileType(mode(filename) & S_IFMT); }
  
  /*
   * Rename/move the file.
   */
  inline void POSIXFile::mv (std::string const& src, std::string const& dest) { rename(src, dest); }
  
  /*
   * Delete the file.
   */
  inline void POSIXFile::rm  (std::string const& filename) { unlink(filename); }
  inline void POSIXFile::del (std::string const& filename) { unlink(filename); }
  
  /*
   * File info.
   */
  inline bool POSIXFile::is_blockDev        (std::string const& filename) { return ((s_try_mode(filename) & S_IFMT) & S_IFBLK ) == S_IFBLK ; }
  inline bool POSIXFile::is_charDev         (std::string const& filename) { return ((s_try_mode(filename) & S_IFMT) & S_IFCHR ) == S_IFCHR ; }
  inline bool POSIXFile::is_dir             (std::string const& filename) { return ((s_try_mode(filename) & S_IFMT) & S_IFDIR ) == S_IFDIR ; }
  inline bool POSIXFile::is_directory       (std::string const& filename) { return is_dir();                                           }
  inline bool POSIXFile::is_executable      (std::string const& filename) { return ((s_try_mode(filename) & S_IFMT) & S_IFREG ) == S_IFREG ; }
  inline bool POSIXFile::is_executable_real (std::string const& filename) { return ((s_try_mode(filename) & S_IFMT) & S_IFIFO ) == S_IFIFO ; }
  inline bool POSIXFile::is_exist           (std::string const& filename) { return ((s_try_mode(filename) & S_IFMT) & S_IFSOCK) == S_IFSOCK; }
  inline bool POSIXFile::is_file            (std::string const& filename) { return ((s_try_mode(filename) & S_IFMT) & S_IFLNK ) == S_IFLNK ; }
  inline bool POSIXFile::is_groupOwned      (std::string const& filename) {}
  inline bool POSIXFile::is_userOwned       (std::string const& filename) {}
  inline bool POSIXFile::is_pipe            (std::string const& filename) {}
  inline bool POSIXFile::is_readable        (std::string const& filename) {}
  inline bool POSIXFile::is_readable_real   (std::string const& filename) {}
  inline bool POSIXFile::is_setGID          (std::string const& filename) {}
  inline bool POSIXFile::is_setUID          (std::string const& filename) {}
  inline bool POSIXFile::is_socket          (std::string const& filename) {}
  inline bool POSIXFile::is_sticky          (std::string const& filename) {}
  inline bool POSIXFile::is_symlink         (std::string const& filename) {}
  inline bool POSIXFile::is_writable        (std::string const& filename) {}
  inline bool POSIXFile::is_writable_real   (std::string const& filename) {}
  inline bool POSIXFile::is_zero            (std::string const& filename) {}
  
  /*
   * stat() info.
   */
  inline mode_t POSIXFile::mode (std::string const& filename) {
    struct stat retval;
    return s_stat(filename, &retval).st_mode;
  }
  
  /*
   * Convert from stat() file type to POSIXFile::FileType.
   */
  inline POSIXFile::FileType POSIXFile::s_convert_FileType (mode_t const type) {
    switch (type) {
      case S_IFSOCK: return FT_SOCKET   ;
      case S_IFLNK : return FT_SYMLINK  ;
      case S_IFREG : return FT_FILE     ;
      case S_IFBLK : return FT_BLOCKDEV ;
      case S_IFDIR : return FT_DIRECTORY;
      case S_IFCHR : return FT_CHARDEV  ;
      case S_IFIFO : return FT_FIFO     ;
      default      : throw Errors::UnknownType().Type(type);
    };
  }
  
  /***************************************************************************
   * _FD
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * _FD constructor.
   */
  inline POSIXFile::_FD::_FD (FDType const fd) : _fd(fd) {}
  
  /*
   * _FD destructor.
   */
  inline POSIXFile::_FD::~_FD () { set(-1); }
  
  /*
   * _FD assignment operator.
   */
  inline POSIXFile::_FD& POSIXFile::_FD::operator = (FDType const right) { set(right); return *this; }
  
  /*
   * _FD cast operator.
   */
  inline POSIXFile::_FD::operator FDType () const { return _fd; }
  
  /*
   * Set a new fd.
   */
  inline int POSIXFile::_FD::set (FDType const fd) {
    int retval = 0;
    if (fd != _fd) {
      if (_fd != -1) {
        retval = ::close(_fd);
      }
      _fd = fd;
    }
    return retval;
  }
  
}

#endif

