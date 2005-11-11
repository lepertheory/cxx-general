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
#include <cxx-general/ReferencePointer.hxx>
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
      POSIXFile& Filename (std::string const& filename);
      std::string Filename () const;
      
      // Reset to just-constructed defaults.
      void clear ();
      
      // Copy a given File object. Does not copy the file itself.
      void copy (POSIXFile const& source);
      
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
          _FD& operator = (_FDType const fd);
          
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
      
      /***********************************************************************
       * _Data
       ***********************************************************************/
      class _Data {
        
        /*
         * Public members.
         */
        public:
          
          /*******************************************************************/
          // Data members.
          
          // The name of the file.
          std::string filename;
          
          // File descriptor. 0 if file is closed.
          _FD fd;
          
          // Results of stat().
          struct stat stat;
          
          // Whether the stat cache is valid.
          bool cache_valid;
          
          /*******************************************************************/
          // Function members.

          // Default constructor.
          _Data ();
          
          // Copy constructor.
          _Data (_Data const& source);
          
          // Assignment operator.
          _Data& operator = (_Data const& source);
          
          // Reset to just-constructed defaults.
          void clear ();
          
          // Copy a given _Data.
          void copy (_Data const& source);
          
      };
      
      // Pointer to _Data.
      typedef ReferencePointer<_Data> _DataPT;
      
      /***********************************************************************/
      // Data members.
      
      // All file data. Allows multiple references to the same file, and COW.
      _DataPT _data;
      
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
  
  // Properties.
  inline POSIXFile& POSIXFile::Filename (std::string const& filename) {
    if (!_data->filename.empty() && filename != _data->filename) {
      clear();
    }
    _data->filename = filename;
    return *this;
  }
  inline std::string POSIXFile::Filename () const { return _data->filename; }
  
  // File info.
  inline dev_t     POSIXFile::device    (bool cache) const { _check_cache(cache); return _data->stat.st_dev    ; }
  inline ino_t     POSIXFile::inode     (bool cache) const { _check_cache(cache); return _data->stat.st_ino    ; }
  inline mode_t    POSIXFile::mode      (bool cache) const { _check_cache(cache); return _data->stat.st_mode   ; }
  inline nlink_t   POSIXFile::links     (bool cache) const { _check_cache(cache); return _data->stat.st_nlink  ; }
  inline uid_t     POSIXFile::uid       (bool cache) const { _check_cache(cache); return _data->stat.st_uid    ; }
  inline gid_t     POSIXFile::gid       (bool cache) const { _check_cache(cache); return _data->stat.st_gid    ; }
  inline dev_t     POSIXFile::devid     (bool cache) const { _check_cache(cache); return _data->stat.st_rdev   ; }
  inline off_t     POSIXFile::size      (bool cache) const { _check_cache(cache); return _data->stat.st_size   ; }
  inline blksize_t POSIXFile::blockSize (bool cache) const { _check_cache(cache); return _data->stat.st_blksize; }
  inline blkcnt_t  POSIXFile::blocks    (bool cache) const { _check_cache(cache); return _data->stat.st_blocks ; }
  inline time_t    POSIXFile::atime     (bool cache) const { _check_cache(cache); return _data->stat.st_atime  ; }
  inline time_t    POSIXFile::mtime     (bool cache) const { _check_cache(cache); return _data->stat.st_mtime  ; }
  inline time_t    POSIXFile::ctime     (bool cache) const { _check_cache(cache); return _data->stat.st_ctime  ; }
  
  // Update cache if necessary.
  inline void POSIXFile::_check_cache (bool cache) const { if (!cache || !_data->cache_valid) { _update_cache(); } }
  
  // _FD constructor.
  inline POSIXFile::_FD::_FD (_FDType const fd) { set(fd); }
  
  // _FD destructor.
  inline POSIXFile::_FD::~_FD () { set(0); }
  
  // _FD assignment operator.
  inline POSIXFile::_FD& POSIXFile::_FD::operator = (_FDType const fd) { set(fd); return *this; }
  
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
  
  // _Data default constructor.
  inline POSIXFile::_Data::_Data () { clear(); }
  
  // _Data copy constructor.
  inline POSIXFile::_Data::_Data (_Data const& source) { copy(source); }
  
  // _Data assignment operator.
  inline POSIXFile::_Data& POSIXFile::_Data::operator = (_Data const& right) { copy(right); return *this; }
  
}

#endif

