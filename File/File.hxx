/*****************************************************************************
 * File.hxx
 *****************************************************************************
 * Interface for class File.
 *****************************************************************************/

// Include guard.
#if !defined(FILE_234b734278udud8hb48)
  #define FILE_234b734278udud8hb48

// STL includes.
#include <string>

// System includes.
#include <sys/types.h>
#include <sys/stat.h>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * File
   ***************************************************************************
   * File class.
   ***************************************************************************/
  class File {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      File ();
      
      // Constructor names the file.
      File (std::string const& filename);
      
      // Copy constructor.
      File (File const& source);
      
      // Assignment operator.
      File& operator = (File const& right);
      
      // Properties.
      File& Filename (std::string const& filename);
      std::string Filename () const;
      
      // Reset to just-constructed defaults.
      void clear ();
      
      // Copy a given File object. Does not copy the file itself.
      void copy (File const& source);
      
      // File info.
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
    
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data members.
      
      // The name of the file, duh.
      std::string _filename;
      
      // Results of stat()
      struct stat _stat;
      
      // True if cache is valid.
      bool _cachevalid;
      
      /***********************************************************************/
      // Function members.

      // Update the cache if necessary.
      void _check_cache () const;
      
      // Update the cache now.
      void _update_cache () const;
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.

  // Default constructor.
  inline File::File () { clear(); }
  
  // Copy constructor.
  inline File::File (File const& source) { copy(source); }
  
  // Assignment operator.
  inline File& File::operator = (File const& right) { copy(right); return *this; }
  
  // Properties.
  inline File& File::Filename (std::string const& filename) {
    if (!_filename.empty() && filename != _filename) {
      clear();
    }
    _filename = filename;
    return *this;
  }
  inline std::string File::Filename () const { return _filename; }
  
  // File info.
  inline dev_t     File::device    () const { _check_cache(); return _stat.st_dev    ; }
  inline ino_t     File::inode     () const { _check_cache(); return _stat.st_ino    ; }
  inline mode_t    File::mode      () const { _check_cache(); return _stat.st_mode   ; }
  inline nlink_t   File::links     () const { _check_cache(); return _stat.st_nlink  ; }
  inline uid_t     File::uid       () const { _check_cache(); return _stat.st_uid    ; }
  inline gid_t     File::gid       () const { _check_cache(); return _stat.st_gid    ; }
  inline dev_t     File::devid     () const { _check_cache(); return _stat.st_rdev   ; }
  inline off_t     File::size      () const { _check_cache(); return _stat.st_size   ; }
  inline blksize_t File::blockSize () const { _check_cache(); return _stat.st_blksize; }
  inline blkcnt_t  File::blocks    () const { _check_cache(); return _stat.st_blocks ; }
  inline time_t    File::atime     () const { _check_cache(); return _stat.st_atime  ; }
  inline time_t    File::mtime     () const { _check_cache(); return _stat.st_mtime  ; }
  inline time_t    File::ctime     () const { _check_cache(); return _stat.st_ctime  ; }
  
  // Update cache if necessary.
  inline void File::_check_cache () const { if (!_cachevalid) { _update_cache(); } }
  
}

#endif

