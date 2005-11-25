/*****************************************************************************
 * Timestamp.h++
 *****************************************************************************
 * Interface for class Timestamp.
 *****************************************************************************/

// Include guard.
#if !defined(TIMESTAMP_982yk298huh98u)
  #define TIMESTAMP_982yk298huh98u

// STL includes.
  #include <iostream>
  #include <string>
  #include <vector>
  #include <map>

// System includes.
  #include <cxx-general/Exception.h++>
  #include <cxx-general/Arb/Arb.h++>
  #include <cxx-general/ReferencePointer.h++>

// Namespace wrapping.
namespace DAC {
  
  /***************************************************************************
   * Timestamp
   ***************************************************************************
   * This is a timestamp class.
   ***************************************************************************/
  class Timestamp {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Forward declarations.
      
      class LeapSecondDay;
      
      /***********************************************************************/
      // Enums.
      
      // Calendar system.
      enum CalendarType { CT_DEFAULT, CT_JULIAN, CT_GREGORIAN };
      
      // Days of the week.
      enum DayOfWeek {
        DOW_SUNDAY    = 0,
        DOW_MONDAY    = 1,
        DOW_TUESDAY   = 2,
        DOW_WEDNESDAY = 3,
        DOW_THURSDAY  = 4,
        DOW_FRIDAY    = 5,
        DOW_SATURDAY  = 6
      };
      
      // ISO days of the week.
      enum ISODayOfWeek {
        ISO_DOW_MONDAY    = 1,
        ISO_DOW_TUESDAY   = 2,
        ISO_DOW_WEDNESDAY = 3,
        ISO_DOW_THURSDAY  = 4,
        ISO_DOW_FRIDAY    = 5,
        ISO_DOW_SATURDAY  = 6,
        ISO_DOW_SUNDAY    = 7
      };
      
      // Months.
      enum MonthName {
        MON_JANUARY   =  1,
        MON_FEBRUARY  =  2,
        MON_MARCH     =  3,
        MON_APRIL     =  4,
        MON_MAY       =  5,
        MON_JUNE      =  6,
        MON_JULY      =  7,
        MON_AUGUST    =  8,
        MON_SEPTEMBER =  9,
        MON_OCTOBER   = 10,
        MON_NOVEMBER  = 11,
        MON_DECEMBER  = 12
      };
      
      /***********************************************************************/
      // Typedefs.
      
      // Type used for all time values.
      typedef Arb TimeVal;
      
      // List of leap seconds.
      typedef std::vector<LeapSecondDay> LeapSecondList;
      
      // Format string.
      typedef std::string Format;
      
      // Pointer to a list of leap seconds.
      typedef ReferencePointer<LeapSecondList> LSLptr;
      
      // Pointer to a format.
      typedef ReferencePointer<Format> Formatptr;
      
      /***********************************************************************/
      // Classes.
      
      // Year, month, and day grouped.
      class YMD {
        public:
          YMD () {};
          YMD (YMD const& ymd) : Year(ymd.Year), Month(ymd.Month), Day(ymd.Day) {};
          YMD (TimeVal const& year, TimeVal const& month, TimeVal const& day) : Year(year), Month(month), Day(day) {};
          template <class T, class U, class V> YMD (T const year, U const month, V const day) : Year(year), Month(month), Day(day) {}
          TimeVal  Year;
          TimeVal Month;
          TimeVal   Day;
      };
      
      // Leap second day.
      class LeapSecondDay : public YMD {
        public:
          LeapSecondDay () {};
          LeapSecondDay (YMD const& ymd, TimeVal const& leap) : YMD(ymd), Leap(leap) {};
          template <class T> LeapSecondDay (YMD const& ymd, T const leap) : YMD(ymd), Leap(leap) {}
          TimeVal Leap;
      };
      
      // Interval container, for passing multiple intervals at once.
      class Interval {
        
        /*
         * Public members.
         */
        public:
          
          /*******************************************************************/
          // Class members.
          
          // Default constructor.
          Interval ();
          
          // Set properties.
          Interval& Millisecond (TimeVal const& millisecond);
          Interval& Second      (TimeVal const& second     );
          Interval& Minute      (TimeVal const& minute     );
          Interval& Hour        (TimeVal const& hour       );
          Interval& Day         (TimeVal const& day        );
          Interval& Month       (TimeVal const& month      );
          Interval& Year        (TimeVal const& year       );
          
          // Get properties.
          TimeVal Millisecond () const;
          TimeVal Second      () const;
          TimeVal Minute      () const;
          TimeVal Hour        () const;
          TimeVal Day         () const;
          TimeVal Month       () const;
          TimeVal Year        () const;
          
          // Check if properties are set.
          bool isSet_Millisecond () const;
          bool isSet_Second      () const;
          bool isSet_Minute      () const;
          bool isSet_Hour        () const;
          bool isSet_Day         () const;
          bool isSet_Month       () const;
          bool isSet_Year        () const;
          
        /*
         * Private members.
         */
        private:
          
          /*******************************************************************/
          // Data members.
          
          // Properties.
          TimeVal _millisecond;
          TimeVal _second;
          TimeVal _minute;
          TimeVal _hour;
          TimeVal _day;
          TimeVal _month;
          TimeVal _year;
          bool    _set_millisecond;
          bool    _set_second;
          bool    _set_minute;
          bool    _set_hour;
          bool    _set_day;
          bool    _set_month;
          bool    _set_year;
        
      };
      
      /***********************************************************************/
      // Errors
      class Errors {
        
        public:
          
          // All Timestamp errors are based off of this.
          class Base : public Exception {
            public:
              virtual char const* what () const throw() { return "Undefined error in Timestamp."; };
              virtual ~Base () throw() {};
          };
          
          // Bad format.
          class BadFormat : public Base {
            public:
              virtual char const* what () const throw() {
                try {
                  return (_problem + " at position " + DAC::toString(SafeInt<std::string::size_type>(_position) + 1) + " in format \"" + *_format + "\".").c_str();
                } catch (...) {
                  return "Bad format. Error creating message string.";
                }
              };
              virtual ~BadFormat () throw() {};
              BadFormat& Problem  (char const*                        const problem ) throw() { _problem  = problem ; return *this; };
              BadFormat& Position (std::string::size_type             const position) throw() { _position = position; return *this; };
              BadFormat& Format   (ConstReferencePointer<std::string> const format  ) throw() { _format   = format  ; return *this; };
              char const*                        Problem  () const throw() { return _problem.c_str(); }
              std::string::size_type             Position () const throw() { return _position       ; }
              ConstReferencePointer<std::string> Format   () const throw() { return _format         ; }
            private:
              std::string                        _problem ;
              std::string::size_type             _position;
              ConstReferencePointer<std::string> _format  ;
          };
          
          // Bad value.
          class BadValue : public Base {
            public:
              virtual char const* what () const throw() { return "Bad value passed."; };
          };
          
          // Invalid MJD sent.
          class BadMJD : public BadValue {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Invalid MJD of " + DAC::toString(_mjd) + " sent.").c_str();
                } catch (...) {
                  return "Invalid MJD sent. Error creating message string.";
                }
              };
              BadMJD& MJD (unsigned int const mjd) throw() { _mjd = mjd; return *this; };
              unsigned int MJD () const throw() { return _mjd; };
            protected:
              unsigned int _mjd;
          };
          class MJDMax : public BadMJD {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("MJD of " + DAC::toString(_mjd) + " sent, MJD maximum is 99999.").c_str();
                } catch (...) {
                  return "MJD over maximum of 99999 sent. Error creating message string.";
                }
              };
          };
          
          // Invalid offset.
          class BadOffset : public BadValue {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Invalid offset of " + DAC::toString(_offset) + " sent.").c_str();
                } catch (...) {
                  return "Invalid offset sent. Error creating message string.";
                }
              };
              BadOffset& Offset (int const offset) throw() { _offset = offset; return *this; }
              int Offset () const throw() { return _offset; }
            protected:
              int _offset;
          };
          class OffsetMin : public BadOffset {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Offset of " + DAC::toString(_offset) + " sent, offset minimum is -720.").c_str();
                } catch (...) {
                  return "Offset under minimum of -720 sent. Error creating message string.";
                }
              };
          };
          class OffsetMax : public BadOffset {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Offset of " + DAC::toString(_offset) + " sent, offset maximum is 720.").c_str();
                } catch (...) {
                  return "Offset over maximum of 720 sent. Error creating message string.";
                }
              };
          };
          
          // Invalid time.
          class InvalidTime : public BadValue {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Invalid time specified: " + _month.toString() + "/" + _day.toString() + "/" + _year.toString() + " " + _hour.toString() + ":" + _minute.toString() + ":" + _second.toString() + "." + _millisecond.toString()).c_str();
                } catch (...) {
                  return "Invalid time specified. Error creating message string.";
                }
              };
              InvalidTime& Year        (TimeVal const& year       ) throw() { _year        = year       ; return *this; };
              InvalidTime& Month       (TimeVal const& month      ) throw() { _month       = month      ; return *this; };
              InvalidTime& Day         (TimeVal const& day        ) throw() { _day         = day        ; return *this; };
              InvalidTime& Hour        (TimeVal const& hour       ) throw() { _hour        = hour       ; return *this; };
              InvalidTime& Minute      (TimeVal const& minute     ) throw() { _minute      = minute     ; return *this; };
              InvalidTime& Second      (TimeVal const& second     ) throw() { _second      = second     ; return *this; };
              InvalidTime& Millisecond (TimeVal const& millisecond) throw() { _millisecond = millisecond; return *this; };
              TimeVal Year        () const throw() { return _year       ; };
              TimeVal Month       () const throw() { return _month      ; };
              TimeVal Day         () const throw() { return _day        ; };
              TimeVal Hour        () const throw() { return _hour       ; };
              TimeVal Minute      () const throw() { return _minute     ; };
              TimeVal Second      () const throw() { return _second     ; };
              TimeVal Millisecond () const throw() { return _millisecond; };
            private:
              TimeVal _year       ;
              TimeVal _month      ;
              TimeVal _day        ;
              TimeVal _hour       ;
              TimeVal _minute     ;
              TimeVal _second     ;
              TimeVal _millisecond;
          };
          
          // The year zero does not exist.
          class NoYearZero : public InvalidTime {
            public:
              virtual char const* what () const throw() {
                return "There is no year 0.";
              }
          };
          
          // Error making a system call.
          class SysCallError : public Base {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Error making the requested system call to " + _syscall + ".").c_str();
                } catch (...) {
                  return "Error making the requested system call. Error creating message string.";
                }
              };
              virtual ~SysCallError () throw() {};
              SysCallError& SysCall (char const* const syscall) throw() { _syscall = syscall; return *this; };
              char const* SysCall () const throw() { return _syscall.c_str(); }
            private:
              std::string _syscall;
          };
  #if defined(TIMESTAMP_SYSTIME_GETSYSTEMTIME)
          class SysCall_GetSystemTime : public SysCallError {
            public:
              SysCall_GetSystemTime () {
                SysCall("GetSystemTime");
              };
          };
  #endif
  #if defined(TIMESTAMP_SYSTIME_GETTIMEOFDAY_GMTIME_R) || \
      defined(TIMESTAMP_SYSTIME_GETTIMEOFDAY_GMTIME  )
          class SysCall_gettimeofday : public SysCallError {
            public:
              SysCall_gettimeofday () {
                SysCall("gettimeofday");
              };
          };
  #endif
  #if defined(TIMESTAMP_SYSTIME_TIME_GMTIME_R) || \
      defined(TIMESTAMP_SYSTIME_TIME_GMTIME  )
          class SysCall_time : public SysCallError {
            public:
              SysCall_time () {
                SysCall("time");
              };
          };
  #endif
  #if defined(TIMESTAMP_SYSTIME_GETTIMEOFDAY_GMTIME_R) || \
      defined(TIMESTAMP_SYSTIME_TIME_GMTIME_R        )
          class SysCall_gmtime_r : public SysCallError {
            public:
              SysCall_gmtime_r () {
                SysCall("gmtime_r");
              };
          };
  #endif
  #if defined(TIMESTAMP_SYSTIME_GETTIMEOFDAY_GMTIME) || \
      defined(TIMESTAMP_SYSTIME_TIME_GMTIME)
          class SysCall_gmtime : public SysCallError {
            public:
              SysCall_gmtime () {
                SysCall("gmtime");
              };
          };
  #endif
          
          // Missing system support.
          class MissingSysSupport : public Base {
            public:
              virtual char const* what () const throw() {
                try {
                  return ("Missing support for the requested operation: " + _op).c_str();
                } catch (...) {
                  return "Missing support for the requested operation. Error creating message string.";
                }
              };
              virtual ~MissingSysSupport () throw() {};
              MissingSysSupport& Operation (char const* const op) throw() { _op = op; return *this; };
              char const* Operation () const throw() { return _op.c_str(); }
            private:
              std::string _op;
          };
        
        // Private constructor, no instantiation.
        private:
          Errors ();
          Errors (Errors const&);
          Errors& operator = (Errors const&);
        
      };
      
      /***********************************************************************/
      // Constants.
      
      static const bool THREADSAFE;
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      Timestamp ();
      
      // Copy constructor.
      Timestamp (Timestamp const& ts);
      
      // Conversion constructor.
      explicit Timestamp (Interval const& time);
      explicit Timestamp (TimeVal  const& jd  );
      
      // Increment / decrement operators.
      Timestamp& operator ++ ();
      Timestamp  operator ++ (int);
      Timestamp& operator -- ();
      Timestamp  operator -- (int);
      
      // Casting operators.
      operator bool               () const;
      operator signed   char      () const;
      operator unsigned char      () const;
      operator signed   short int () const;
      operator unsigned short int () const;
      operator signed   int       () const;
      operator unsigned int       () const;
      operator signed   long int  () const;
      operator unsigned long int  () const;
      
      // Assignment operator.
      Timestamp& operator = (Timestamp const& ts  );
      Timestamp& operator = (Interval  const& time);
      Timestamp& operator = (TimeVal   const& jd  );
      
      // Arithmetic assignment operators.
      Timestamp& operator += (TimeVal const& days);
      Timestamp& operator -= (TimeVal const& days);
      
      // Properties.
      Timestamp&   LastJulianDate (YMD         const& lastjulian)      ;
      YMD          LastJulianDate (                             ) const;
      Timestamp&   Julian         (TimeVal     const& jd        )      ;
      TimeVal      Julian         (                             ) const;
      Timestamp&   MJD            (unsigned int const mjd       )      ;
      unsigned int MJD            (                             ) const;
      Timestamp&   POSIXDate      (TimeVal     const& posixdate )      ;
      TimeVal      POSIXDate      (                             ) const;
      Timestamp&   Offset         (int const          offset    )      ;
      int          Offset         (                             ) const;
      Timestamp&   TZName         (char const* const  name      )      ;
      char const*  TZName         (                             ) const;
      TimeVal      Year           (                             ) const;
      TimeVal      Month          (                             ) const;
      TimeVal      Day            (                             ) const;
      TimeVal      Hour           (                             ) const;
      TimeVal      Minute         (                             ) const;
      TimeVal      Second         (                             ) const;
      TimeVal      Millisecond    (                             ) const;
      
      // Reset to just-constructed defaults.
      Timestamp& clear ();
      
      // Copy another timestamp.
      Timestamp& copy (Timestamp const& ts);
      
      // Set this timestamp.
      Timestamp& set (Interval const& time);
      Timestamp& set (TimeVal  const& jd  );
      
      // Set in GMT.
      Timestamp& setGMT (Interval const& time);
      Timestamp& setGMT (TimeVal  const& jd  );
      
      // Get the individual values of this timestamp.
      Interval get () const;
      
      // Get the values of this timestamp in GMT.
      Interval getGMT () const;
      
      // Get the current system time.
      Timestamp& getSystemTime ();
      
      // Follows the formatting rules at the following URL:
      // http://www.opengroup.org/onlinepubs/009695399/functions/strftime.html
      std::string toString (std::string const& format = "") const;
      
      // Arithmetic operator backends.
      Timestamp& op_add (TimeVal   const& tv)      ;
      Timestamp& op_sub (TimeVal   const& tv)      ;
      TimeVal    op_sub (Timestamp const& ts) const;
      
      // Comparison operator backends.
      bool op_gt (Timestamp const& ts) const;
      bool op_ge (Timestamp const& ts) const;
      bool op_lt (Timestamp const& ts) const;
      bool op_le (Timestamp const& ts) const;
      bool op_eq (Timestamp const& ts) const;
      bool op_ne (Timestamp const& ts) const;
      
      // Get the day only JD.
      TimeVal jdday () const;
      
      // Get the day of the week.
      TimeVal dow () const;
      
      // Get the ISO day of the week.
      TimeVal dowISO () const;
      
      // Get the day of the year.
      TimeVal doy () const;
      
      // Get the week of the year.
      TimeVal woy (DayOfWeek const base) const;
      
      // Get the ISO week.
      TimeVal woyISO () const;
      
      // Get the ISO year.
      TimeVal getISOYear () const;
      
      // Get whether or not this is a leap year.
      bool isLeapYear () const;
      
      // Get the leap seconds for this day.
      TimeVal leapSecond () const;
      
      // Get the number of days in this year.
      TimeVal daysInYear () const;
      
      // Get the number of days in this month.
      TimeVal daysInMonth () const;
      
      // Get the number of seconds in this minute.
      TimeVal secondsInMinute () const;
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Constants.
      
      // 3-letter weekday abbreviations, Sun-Sat, 0 indexed.
      static char const* const _SHORT_WEEKDAY_NAME[];
      
      // Full weekday names, Sunday-Saturday, 0 indexed.
      static char const* const _LONG_WEEKDAY_NAME[];
      
      // 3-letter month abbreviation, Jan-Dec, 1 indexed.
      static char const* const _SHORT_MONTH_NAME[];
      
      // Full month names, January-December, 1 indexed.
      static char const* const _LONG_MONTH_NAME[];
      
      /***********************************************************************/
      // Data members.
      
      // This is the time, in Julian Date format, GMT.
      TimeVal _jd;
      
      // Offset, in fraction of a day, from GMT.
      TimeVal _offset;
      
      // Timezone name. Only exists for cosmetic purposes at the moment.
      std::string _tzname;
      
      // Cache individual date components.
      mutable TimeVal _cache_year;
      mutable TimeVal _cache_month;
      mutable TimeVal _cache_day;
      mutable TimeVal _cache_hour;
      mutable TimeVal _cache_minute;
      mutable TimeVal _cache_second;
      mutable TimeVal _cache_millisecond;
      mutable bool    _cache_valid;
      
      // List of leap seconds, must always be sorted.
      LSLptr _leapseconds;
      
      // Last julian date.
      YMD     _lastjulianymd;
      TimeVal _lastjulianjd ;
      
      // Format string for conversion to string.
      Formatptr _format;
      
      // Calendar type of this timestamp.
      CalendarType _caltype;
      
      /***********************************************************************/
      // Static data members.
      
      // True when this class has been initialzed.
      static bool s_initialized;
      
      // Default list of leap seconds, must always be sorted.
      static LSLptr s_defaultleapseconds;
      
      // Default format string.
      static Formatptr s_defaultformat;
      
      /***********************************************************************/
      // Constants.
      
      // Number of days per month in a leap or non-leap year.
      static int const _DAYS_OF_YEAR_NY[];
      static int const _DAYS_OF_YEAR_LY[];
      
      /***********************************************************************/
      // Function members.
      
      // Common initialization routines.
      void _init ();
      
      // Internal set routine.
      Timestamp& _set (Interval const& time);
      
      // Internal get routine.
      Interval _get () const;
      
      // Load the cache.
      void _loadCache () const;
      
      // Return whether a given year is a leap year.
      bool _isLeapYear (TimeVal const& year) const;
      
      // Return true if the given date is Gregorian.
      bool _isGregorian (TimeVal const& year, TimeVal const& month, TimeVal const& day) const;
      bool _isGregorian (TimeVal const& jd                                            ) const;
      
      // Get the calendar type.
      CalendarType _getCalendarType (TimeVal const& year, TimeVal const& month, TimeVal const& day) const;
      CalendarType _getCalendarType (TimeVal const& jd                                            ) const;
      
      // Return the number of days in a given month.
      TimeVal _daysInMonth (TimeVal const& year, TimeVal const& month) const;
      
      // Return the leap seconds of a given day.
      TimeVal _leapSecond (TimeVal const& year, TimeVal const& month, TimeVal const& day) const;
      
      /***********************************************************************/
      // Static function members.
      
      // Class initialization.
      static void s_classInit ();
      
  };
  
  /***************************************************************************
   * Operators.
   ***************************************************************************/
  
  // Stream I/O operators.
  std::ostream& operator << (std::ostream& l, Timestamp const& r);
  
  // Arithmetic operators.
  Timestamp          operator + (Timestamp          const& l, Timestamp::TimeVal const& r);
  Timestamp          operator + (Timestamp::TimeVal const& l, Timestamp          const& r);
  Timestamp          operator - (Timestamp          const& l, Timestamp::TimeVal const& r);
  Timestamp::TimeVal operator - (Timestamp          const& l, Timestamp          const& r);
  
  // Comparison operators.
  bool operator >  (Timestamp const& l, Timestamp const& r);
  bool operator >= (Timestamp const& l, Timestamp const& r);
  bool operator <  (Timestamp const& l, Timestamp const& r);
  bool operator <= (Timestamp const& l, Timestamp const& r);
  bool operator == (Timestamp const& l, Timestamp const& r);
  bool operator != (Timestamp const& l, Timestamp const& r);
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * Class Timestamp.
   ***************************************************************************/
  
  // Increment / decrement operators.
  inline Timestamp& Timestamp::operator ++ ()    { return op_add(TimeVal(1));                                  }
  inline Timestamp  Timestamp::operator ++ (int) { Timestamp retval(*this); op_add(TimeVal(1)); return retval; }
  inline Timestamp& Timestamp::operator -- ()    { return op_sub(TimeVal(1));                                  }
  inline Timestamp  Timestamp::operator -- (int) { Timestamp retval(*this); op_sub(TimeVal(1)); return retval; }
  
  // Casting operators.
  inline Timestamp::operator bool               () const { return Julian(); }
  inline Timestamp::operator signed   char      () const { return Julian(); }
  inline Timestamp::operator unsigned char      () const { return Julian(); }
  inline Timestamp::operator signed   short int () const { return Julian(); }
  inline Timestamp::operator unsigned short int () const { return Julian(); }
  inline Timestamp::operator signed   int       () const { return Julian(); }
  inline Timestamp::operator unsigned int       () const { return Julian(); }
  inline Timestamp::operator signed   long int  () const { return Julian(); }
  inline Timestamp::operator unsigned long int  () const { return Julian(); }
  
  // Assignment operator.
  inline Timestamp& Timestamp::operator = (Timestamp const& ts)   { return copy(ts);  }
  inline Timestamp& Timestamp::operator = (Interval  const& time) { return set(time); }
  inline Timestamp& Timestamp::operator = (TimeVal   const& jd)   { return set(jd);   }
  
  // Arithmetic assignment operators.
  inline Timestamp& Timestamp::operator += (TimeVal const& days) { return op_add(days); }
  inline Timestamp& Timestamp::operator -= (TimeVal const& days) { return op_sub(days); }
  
  // Properties.
  // TODO: See if there's a practical way to set this as a JD.
  inline Timestamp& Timestamp::LastJulianDate (YMD const& lastjulian) {
    Timestamp tmp;
    tmp.set(
      Interval().Year(lastjulian.Year)
                .Month(lastjulian.Month)
                .Day(lastjulian.Day)
                .Hour(TimeVal(12))
    );
    _cache_valid   = false;
    _lastjulianymd = lastjulian;
    _lastjulianjd  = tmp.Julian().floor();
    return *this;
  }
  inline Timestamp::YMD Timestamp::LastJulianDate () const { return _lastjulianymd; }
  
  inline Timestamp&         Timestamp::Julian (TimeVal const& jd)       { _cache_valid = false; _jd.set(jd); return *this; }
  inline Timestamp::TimeVal Timestamp::Julian (                 ) const { return _jd;                                      }
  
  inline unsigned int Timestamp::MJD () const { return (_jd - 2400000 + 0.5 + _offset).floor() % 100000; }
  
  inline Timestamp&         Timestamp::POSIXDate (TimeVal const& posixdate)       { _cache_valid = false; _jd.set(posixdate / 86400 + 2440587.5); return *this; }
  inline Timestamp::TimeVal Timestamp::POSIXDate (                        ) const { return (_jd - 2440587.5) * 86400;                                           }
  
  inline Timestamp::TimeVal Timestamp::Year        () const { if (!_cache_valid) { _loadCache(); } return _cache_year       ; }
  inline Timestamp::TimeVal Timestamp::Month       () const { if (!_cache_valid) { _loadCache(); } return _cache_month      ; }
  inline Timestamp::TimeVal Timestamp::Day         () const { if (!_cache_valid) { _loadCache(); } return _cache_day        ; }
  inline Timestamp::TimeVal Timestamp::Hour        () const { if (!_cache_valid) { _loadCache(); } return _cache_hour       ; }
  inline Timestamp::TimeVal Timestamp::Minute      () const { if (!_cache_valid) { _loadCache(); } return _cache_minute     ; }
  inline Timestamp::TimeVal Timestamp::Second      () const { if (!_cache_valid) { _loadCache(); } return _cache_second     ; }
  inline Timestamp::TimeVal Timestamp::Millisecond () const { if (!_cache_valid) { _loadCache(); } return _cache_millisecond; }
  
  inline int Timestamp::Offset () const { return _offset * 1440; }
  
  inline Timestamp&  Timestamp::TZName (char const* const name)       { _tzname = name; return *this; }
  inline char const* Timestamp::TZName ()                       const { return _tzname.c_str();       }
    
  // Set the timestamp with an interval.
  inline Timestamp& Timestamp::set (Interval const& time) { Timestamp newtime(*this); newtime._set(time); _cache_valid = false; _jd = newtime._jd - _offset; return *this; }
  
  // Set the timestamp with a Julian Date.
  inline Timestamp& Timestamp::set (TimeVal const& jd) { return Julian(jd); }
  
  // Set the timestamp with an interval, no GMT offset.
  inline Timestamp& Timestamp::setGMT (Interval const& time) { Timestamp newtime(*this); newtime._set(time); _cache_valid = false; _jd = newtime._jd; return *this; }
  
  // Get the interval of the timestamp.
  inline Timestamp::Interval Timestamp::get () const { return (*this + _offset)._get(); }
  
  // Get the interval of the timestamp, no GMT offset.
  inline Timestamp::Interval Timestamp::getGMT () const { return _get(); }
  
  // Arithmetic.
  inline Timestamp&         Timestamp::op_add (TimeVal   const& tv)       { _cache_valid = false; _jd += tv; return *this;  }
  inline Timestamp&         Timestamp::op_sub (TimeVal   const& tv)       { return op_add(-tv);                             }
  inline Timestamp::TimeVal Timestamp::op_sub (Timestamp const& ts) const { return (_jd + _offset) - (ts._jd + ts._offset); }
  
  // Comparison operator backends.
  inline bool Timestamp::op_gt (Timestamp const& ts) const { return (_jd + _offset) >  (ts._jd + ts._offset); }
  inline bool Timestamp::op_ge (Timestamp const& ts) const { return (_jd + _offset) >= (ts._jd + ts._offset); }
  inline bool Timestamp::op_lt (Timestamp const& ts) const { return (_jd + _offset) <  (ts._jd + ts._offset); }
  inline bool Timestamp::op_le (Timestamp const& ts) const { return (_jd + _offset) <= (ts._jd + ts._offset); }
  inline bool Timestamp::op_eq (Timestamp const& ts) const { return (_jd + _offset) == (ts._jd + ts._offset); }
  inline bool Timestamp::op_ne (Timestamp const& ts) const { return (_jd + _offset) != (ts._jd + ts._offset); }
  
  // Get the day only JD.
  inline Timestamp::TimeVal Timestamp::jdday () const { TimeVal retval; retval.set((_jd + 0.5).floor()); return retval; }
  
  // Get whether or not this is a leap year.
  inline bool Timestamp::isLeapYear () const { return _isLeapYear(Year()); }
  
  // Get the leap second for this day, 1, -1, or 0.
  inline Timestamp::TimeVal Timestamp::leapSecond () const { return (Hour() == 23 && Minute() == 59) ? _leapSecond(Year(), Month(), Day()) : TimeVal(0); }
  
  // Get the number of days in this year.
  inline Timestamp::TimeVal Timestamp::daysInYear () const { return TimeVal(isLeapYear() ? 366 : 365); }
  
  // Get the number of days in this month.
  inline Timestamp::TimeVal Timestamp::daysInMonth () const { return _daysInMonth(Year(), Month()); }
  
  // Get the number of seconds in this minute.
  inline Timestamp::TimeVal Timestamp::secondsInMinute () const { return 60 + leapSecond(); }
  
  // Return true if the given date is Gregorian.
  inline bool Timestamp::_isGregorian (TimeVal const& year, TimeVal const& month, TimeVal const& day) const {
    return year >  _lastjulianymd.Year || (
           year == _lastjulianymd.Year && (month >  _lastjulianymd.Month || (
                                           month == _lastjulianymd.Month && day > _lastjulianymd.Day
                                          ))
           );
  }
  inline bool Timestamp::_isGregorian (TimeVal const& jd) const { return jd > _lastjulianjd; }
  
  // Get the calendar type. _isLeapYear depends on this function counting 3/0
  // as 2/28 or 2/29.
  inline Timestamp::CalendarType Timestamp::_getCalendarType (TimeVal const& year, TimeVal const& month, TimeVal const& day) const { return (_caltype == CT_DEFAULT) ? (_isGregorian(year, month, day) ? CT_GREGORIAN : CT_JULIAN) : _caltype; }
  inline Timestamp::CalendarType Timestamp::_getCalendarType (TimeVal const& jd                                            ) const { return (_caltype == CT_DEFAULT) ? (_isGregorian(jd)               ? CT_GREGORIAN : CT_JULIAN) : _caltype; }
  
  /***************************************************************************
   * Class Timestamp::Interval.
   ***************************************************************************/
  
  // Default constructor.
  inline Timestamp::Interval::Interval () :
    _set_millisecond(false),
    _set_second     (false),
    _set_minute     (false),
    _set_hour       (false),
    _set_day        (false),
    _set_month      (false),
    _set_year       (false)
  {}
  
  // Set properties.
  inline Timestamp::Interval& Timestamp::Interval::Millisecond (TimeVal const& millisecond) { _millisecond = millisecond; _set_millisecond = true; return *this; }
  inline Timestamp::Interval& Timestamp::Interval::Second      (TimeVal const& second     ) { _second      = second;      _set_second      = true; return *this; }
  inline Timestamp::Interval& Timestamp::Interval::Minute      (TimeVal const& minute     ) { _minute      = minute;      _set_minute      = true; return *this; }
  inline Timestamp::Interval& Timestamp::Interval::Hour        (TimeVal const& hour       ) { _hour        = hour;        _set_hour        = true; return *this; }
  inline Timestamp::Interval& Timestamp::Interval::Day         (TimeVal const& day        ) { _day         = day;         _set_day         = true; return *this; }
  inline Timestamp::Interval& Timestamp::Interval::Month       (TimeVal const& month      ) { _month       = month;       _set_month       = true; return *this; }
  inline Timestamp::Interval& Timestamp::Interval::Year        (TimeVal const& year       ) { _year        = year;        _set_year        = true; return *this; }
  
  // Get properties.
  inline Timestamp::TimeVal Timestamp::Interval::Millisecond () const { return _millisecond; }
  inline Timestamp::TimeVal Timestamp::Interval::Second      () const { return _second;      }
  inline Timestamp::TimeVal Timestamp::Interval::Minute      () const { return _minute;      }
  inline Timestamp::TimeVal Timestamp::Interval::Hour        () const { return _hour;        }
  inline Timestamp::TimeVal Timestamp::Interval::Day         () const { return _day;         }
  inline Timestamp::TimeVal Timestamp::Interval::Month       () const { return _month;       }
  inline Timestamp::TimeVal Timestamp::Interval::Year        () const { return _year;        }
  
  // Check if properties are set.
  inline bool Timestamp::Interval::isSet_Millisecond () const { return _set_millisecond; }
  inline bool Timestamp::Interval::isSet_Second      () const { return _set_second;      }
  inline bool Timestamp::Interval::isSet_Minute      () const { return _set_minute;      }
  inline bool Timestamp::Interval::isSet_Hour        () const { return _set_hour;        }
  inline bool Timestamp::Interval::isSet_Day         () const { return _set_day;         }
  inline bool Timestamp::Interval::isSet_Month       () const { return _set_month;       }
  inline bool Timestamp::Interval::isSet_Year        () const { return _set_year;        }
  
  // Stream I/O operators.
  inline std::ostream& operator << (std::ostream& l, Timestamp const& r) { l << r.toString(); return l; }
  
  // Arithmetic operators.
  inline Timestamp          operator + (Timestamp          const& l, Timestamp::TimeVal const& r) { return Timestamp(l).op_add(r); }
  inline Timestamp          operator + (Timestamp::TimeVal const& l, Timestamp          const& r) { return Timestamp(r).op_add(l); }
  inline Timestamp          operator - (Timestamp          const& l, Timestamp::TimeVal const& r) { return Timestamp(l).op_sub(r); }
  inline Timestamp::TimeVal operator - (Timestamp          const& l, Timestamp          const& r) { return l.op_sub(r);                 }
  
  // Comparison operators.
  inline bool operator >  (Timestamp const& l, Timestamp const& r) { return l.op_gt(r); }
  inline bool operator >= (Timestamp const& l, Timestamp const& r) { return l.op_ge(r); }
  inline bool operator <  (Timestamp const& l, Timestamp const& r) { return l.op_lt(r); }
  inline bool operator <= (Timestamp const& l, Timestamp const& r) { return l.op_le(r); }
  inline bool operator == (Timestamp const& l, Timestamp const& r) { return l.op_eq(r); }
  inline bool operator != (Timestamp const& l, Timestamp const& r) { return l.op_ne(r); }
  
}

#endif