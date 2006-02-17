/*****************************************************************************
 * Syslog
 *****************************************************************************
 * TODO: Fill this with useful text. In fact, do it on everything.
 *****************************************************************************/

// STL includes.
#include <string>

// System includes.
#include <syslog.h>

// Class include.
#include "Syslog.h++"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Default constructor.
   */
  Syslog::Syslog () {
    
    // Standard init.
    clear();
    
  }
  
  /*
   * Destructor.
   */
  Syslog::~Syslog () {
    
    // Clean up after ourselves.
    if (_is_open) {
      close();
    }
    
  }
  
  /*
   * Reset to just-constructed defaults.
   */
  void Syslog::clear () {
    
    // Sensible default options.
    _options = LO_CONSOLE;
    
    // Default facility is USER.
    _facility = LF_USER;
    
    // Default level is debug.
    _level = LL_DEBUG;
    
    // Default mask is all messages.
    _mask = LL_EMERGENCY | LL_ALERT | LL_CRITICAL | LL_ERROR | LL_WARNING | LL_NOTICE | LL_INFO | LL_DEBUG;
    
    // Connection is not open.
    _is_open = false;
    
    // No default ident.
    _ident.clear();
    
  }
  
  /*
   * Properties.
   */
  Syslog& Syslog::MaskUpTo (Log_Level const level) {
    _mask = 0;
    switch (level) {
      case LL_DEBUG    : _mask |= LL_DEBUG    ;
      case LL_INFO     : _mask |= LL_INFO     ;
      case LL_NOTICE   : _mask |= LL_NOTICE   ;
      case LL_WARNING  : _mask |= LL_WARNING  ;
      case LL_ERROR    : _mask |= LL_ERROR    ;
      case LL_CRITICAL : _mask |= LL_CRITICAL ;
      case LL_ALERT    : _mask |= LL_ALERT    ;
      case LL_EMERGENCY: _mask |= LL_EMERGENCY;
    };
    return *this;
  }
  
  /*
   * Log something.
   */
  void Syslog::log (string const& message, Log_Level const level) const {
    
    // Open log if necessary.
    if (!_is_open) {
      open();
    }
    
    // Log it.
    syslog(_llToLevel(level), "%s", message.c_str());
    
  }
  
  /*
   * Open log.
   */
  void Syslog::open () const {
    
    // Simple system call. Appearantly this stuff can't fail.
    openlog(_ident.c_str(), _loToOption(_options), _lfToFacility(_facility));
    _setMask();
    _is_open = true;
    
  }
  
  /*
   * Close log.
   */
  void Syslog::close () const {
    
    // Simple call again.
    closelog();
    _is_open = false;
    
  }
  
  /*
   * Set the log mask.
   */
  void Syslog::_setMask () const {
    
    // Only set mask if log is open.
    if (_is_open) {
      setlogmask(_maskConvert(_mask));
    }
    
  }
  
  /*
   * Convert LogOption to options used by syslog.
   */
  int Syslog::_loToOption (int const lo) {
    int retval = 0;
    if (  lo & LO_CONSOLE   ) { retval |= LOG_CONS  ; }
    if (  lo & LO_DELAYOPEN ) { retval |= LOG_ODELAY; } else { retval |= LOG_NDELAY; }
    if (!(lo & LO_WAITCHILD)) { retval |= LOG_NOWAIT; }
    if (  lo & LO_STDERR    ) { retval |= LOG_PERROR; }
    if (  lo & LO_WITHPID   ) { retval |= LOG_PID   ; }
    return retval;
  }
  
  /*
   * Convert LogFacility to facility used by syslog.
   */
  int Syslog::_lfToFacility (Log_Facility const lf) {
    switch (lf) {
      case LF_AUTHPRIVATE: return LOG_AUTHPRIV;
      case LF_CRON       : return LOG_CRON    ;
      case LF_DAEMON     : return LOG_DAEMON  ;
      case LF_FTP        : return LOG_FTP     ;
      case LF_KERNEL     : return LOG_KERN    ;
      case LF_LOCAL0     : return LOG_LOCAL0  ;
      case LF_LOCAL1     : return LOG_LOCAL1  ;
      case LF_LOCAL2     : return LOG_LOCAL2  ;
      case LF_LOCAL3     : return LOG_LOCAL3  ;
      case LF_LOCAL4     : return LOG_LOCAL4  ;
      case LF_LOCAL5     : return LOG_LOCAL5  ;
      case LF_LOCAL6     : return LOG_LOCAL6  ;
      case LF_LOCAL7     : return LOG_LOCAL7  ;
      case LF_LPR        : return LOG_LPR     ;
      case LF_MAIL       : return LOG_MAIL    ;
      case LF_NEWS       : return LOG_NEWS    ;
      case LF_SYSLOG     : return LOG_SYSLOG  ;
      case LF_USER       : return LOG_USER    ;
      case LF_UUCP       : return LOG_UUCP    ;
    };
    return 0; // Shut up compiler.
  }
  
  /*
   * Convert Log_Level to level used by syslog.
   */
  int Syslog::_llToLevel (Log_Level const ll) {
    switch (ll) {
      case LL_EMERGENCY: return LOG_EMERG  ;
      case LL_ALERT    : return LOG_ALERT  ;
      case LL_CRITICAL : return LOG_CRIT   ;
      case LL_ERROR    : return LOG_ERR    ;
      case LL_WARNING  : return LOG_WARNING;
      case LL_NOTICE   : return LOG_NOTICE ;
      case LL_INFO     : return LOG_INFO   ;
      case LL_DEBUG    : return LOG_DEBUG  ;
    };
    return 0; // Shut up compiler.
  }
  
  /*
   * Convert _mask to one usable by syslog.
   */
  int Syslog::_maskConvert (int const mask) {
    int retval = 0;
    if (mask & LL_EMERGENCY) { retval |= LOG_MASK(_llToLevel(LL_EMERGENCY)); }
    if (mask & LL_ALERT    ) { retval |= LOG_MASK(_llToLevel(LL_ALERT    )); }
    if (mask & LL_CRITICAL ) { retval |= LOG_MASK(_llToLevel(LL_CRITICAL )); }
    if (mask & LL_ERROR    ) { retval |= LOG_MASK(_llToLevel(LL_ERROR    )); }
    if (mask & LL_WARNING  ) { retval |= LOG_MASK(_llToLevel(LL_WARNING  )); }
    if (mask & LL_NOTICE   ) { retval |= LOG_MASK(_llToLevel(LL_NOTICE   )); }
    if (mask & LL_INFO     ) { retval |= LOG_MASK(_llToLevel(LL_INFO     )); }
    if (mask & LL_DEBUG    ) { retval |= LOG_MASK(_llToLevel(LL_DEBUG    )); }
    return retval;
  }
  
}

