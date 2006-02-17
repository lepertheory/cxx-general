/*****************************************************************************
 * Syslog
 *****************************************************************************
 * Provides an interface to POSIX logging facilities.
 *****************************************************************************/

// Include guard.
#if !defined(SYSLOG_7hb7hu2879)
  #define SYSLOG_7hp7hu2879

// STL includes.
  #include <string>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * Syslog
   ***************************************************************************
   * This is the class, baby.
   ***************************************************************************/
  class Syslog {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Data types.
      
      // Logging options.
      enum Log_Option {
        LO_CONSOLE   = 0x01,
        LO_DELAYOPEN = 0x02,
        LO_WAITCHILD = 0x04,
        LO_STDERR    = 0x08,
        LO_WITHPID   = 0x10
      };
      
      // Logging facility, type of program logging the messages.
      enum Log_Facility {
        LF_AUTHPRIVATE,
        LF_CRON       ,
        LF_DAEMON     ,
        LF_FTP        ,
        LF_KERNEL     ,
        LF_LOCAL0     ,
        LF_LOCAL1     ,
        LF_LOCAL2     ,
        LF_LOCAL3     ,
        LF_LOCAL4     ,
        LF_LOCAL5     ,
        LF_LOCAL6     ,
        LF_LOCAL7     ,
        LF_LPR        ,
        LF_MAIL       ,
        LF_NEWS       ,
        LF_SYSLOG     ,
        LF_USER       ,
        LF_UUCP
      };
      
      // Log level, in order of decreasing importance.
      enum Log_Level {
        LL_EMERGENCY = 0x01,
        LL_ALERT     = 0x02,
        LL_CRITICAL  = 0x04,
        LL_ERROR     = 0x08,
        LL_WARNING   = 0x10,
        LL_NOTICE    = 0x20,
        LL_INFO      = 0x40,
        LL_DEBUG     = 0x80
      };
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      Syslog ();
      
      // Destructor.
      ~Syslog ();
      
      // Override () operator for logging.
      void operator () (std::string const& message, Log_Level const level) const;
      void operator () (std::string const& message                       ) const;
      
      // Reset to just-constructed defaults.
      void clear ();
      
      // Properties.
      Syslog& Console  (bool         const  console );
      Syslog& Wait     (bool         const  wait    );
      Syslog& StdErr   (bool         const  std_err );
      Syslog& WithPID  (bool         const  withPID );
      Syslog& Facility (Log_Facility const  facility);
      Syslog& Mask     (int          const  mask    );
      Syslog& MaskUpTo (Log_Level    const  maskupto);
      Syslog& Ident    (std::string  const& ident   );
      Syslog& DefLevel (Log_Level    const  deflevel);
      bool         Console  () const;
      bool         Wait     () const;
      bool         StdErr   () const;
      bool         WithPID  () const;
      Log_Facility Facility () const;
      int          Mask     () const;
      std::string  Ident    () const;
      Log_Level    DefLevel () const;
      
      // Log a message.
      void log (std::string const& message, Log_Level const level) const;
      void log (std::string const& message                       ) const;
      
      // Reopen log. Do nothing if the log is closed.
      void reopen () const;
      
      // Open log manually.
      void open () const;
      
      // Close log manually.
      void close () const;
    
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data members.
      
      // Options.
      int _options;
      
      // Facility.
      Log_Facility _facility;
      
      // Default level.
      Log_Level _level;
      
      // Mask.
      int _mask;
      
      // True if connection is open.
      mutable bool _is_open;
      
      // Ident string at beginning of each log message.
      std::string _ident;
      
      /***********************************************************************/
      // Function members.
      
      // Set log priority mask.
      void _setMask () const;
      
      /***********************************************************************/
      // Static function members.
      
      // Convert Log_Option to options used by syslog.
      static int _loToOption (int const lo);
      
      // Convert Log_Facility to facility used by syslog.
      static int _lfToFacility (Log_Facility const lf);
      
      // Convert Log_Level to level used by syslog.
      static int _llToLevel (Log_Level const ll);
      
      // Convert _mask to mask used by syslog.
      static int _maskConvert (int const mask);
    
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * Syslog.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Redirect to log().
   */
  inline void Syslog::operator () (std::string const& message, Log_Level const level) const { log(message, level); return; }
  inline void Syslog::operator () (std::string const& message                       ) const { log(message       ); return; }
  
  /*
   * Properties.
   */
  inline Syslog& Syslog::Console  (bool         const  console ) { if (console ) { _options |= LO_CONSOLE  ; } else { _options &= ~LO_CONSOLE  ; } reopen()  ; return *this; }
  inline Syslog& Syslog::Wait     (bool         const  wait    ) { if (wait    ) { _options |= LO_WAITCHILD; } else { _options &= ~LO_WAITCHILD; } reopen()  ; return *this; }
  inline Syslog& Syslog::StdErr   (bool         const  std_err ) { if (std_err ) { _options |= LO_STDERR   ; } else { _options &= ~LO_STDERR   ; } reopen()  ; return *this; }
  inline Syslog& Syslog::WithPID  (bool         const  withPID ) { if (withPID ) { _options |= LO_WITHPID  ; } else { _options &= ~LO_WITHPID  ; } reopen()  ; return *this; }
  inline Syslog& Syslog::Facility (Log_Facility const  facility) { _facility = facility;                                                           reopen()  ; return *this; }
  inline Syslog& Syslog::Mask     (int          const  mask    ) { _mask     = mask    ;                                                           _setMask(); return *this; }
  inline Syslog& Syslog::Ident    (std::string  const& ident   ) { _ident    = ident   ;                                                           reopen()  ; return *this; }
  inline Syslog& Syslog::DefLevel (Log_Level    const  deflevel) { _level    = deflevel;                                                                       return *this; }
  inline bool                 Syslog::Console  () const { return _options & LO_CONSOLE  ; }
  inline bool                 Syslog::Wait     () const { return _options & LO_WAITCHILD; }
  inline bool                 Syslog::StdErr   () const { return _options & LO_STDERR   ; }
  inline bool                 Syslog::WithPID  () const { return _options & LO_WITHPID  ; }
  inline Syslog::Log_Facility Syslog::Facility () const { return _facility              ; }
  inline int                  Syslog::Mask     () const { return _mask                  ; }
  inline std::string          Syslog::Ident    () const { return _ident                 ; }
  inline Syslog::Log_Level    Syslog::DefLevel () const { return _level                 ; }
  
  /*
   * Add default level if none was given.
   */
  inline void Syslog::log (std::string const& message) const { log(message, _level); return; }
  
  /*
   * Reopen log.
   */
  inline void Syslog::reopen () const { if (_is_open) { close(); open(); } }
  
}

#endif

