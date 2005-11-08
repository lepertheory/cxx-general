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
  class POSIXFile {
    
    /*
     * Public members.
     */
    public:
      
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
  inline POSIXFile::POSIXFile () { clear(); }
  
  // Copy constructor.
  inline POSIXFile::POSIXFile (POSIXFile const& source) { copy(source); }
  
  // Assignment operator.
  inline POSIXFile& POSIXFile::operator = (POSIXFile const& right) { copy(right); return *this; }
  
  // Properties.
  inline POSIXFile& POSIXFile::Filename (std::string const& filename) {
    if (!_filename.empty() && filename != _filename) {
      clear();
    }
    _filename = filename;
    return *this;
  }
  inline std::string POSIXFile::Filename () const { return _filename; }
  
  // File info.
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
  
  // Update cache if necessary.
  inline void POSIXFile::_check_cache () const { if (!_cachevalid) { _update_cache(); } }
  
}

#endif

