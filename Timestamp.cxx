/*****************************************************************************
 * Timestamp.cxx
 *****************************************************************************
 * Implementation for class Timestamp.
 *****************************************************************************/

// System includes.
#if defined(PLAT_WIN32)
  #if defined(HAS_WINDOWS_H)
    #include <windows.h>
  #endif
#elif defined(PLAT_POSIX)
  #if defined(HAS_SYS_TIME_H)
    #include <sys/time.h>
  #endif
  #if defined(HAS_TIME_H)
    #include <time.h>
  #endif
#endif

// Class include.
#include "Timestamp.hxx"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Class Timestamp::I_Month.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Get the number of days in this month.
  /*
  Timestamp::I_Day Timestamp::I_Month::daysInMonth (I_Year const& year) const {
    
    // Work area.
    Timestamp::I_Day days;
    
    // Easy.
    try {
      switch (Value<int>()) {
        
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
          days = 31;
        break;
        
        case 2:
          days = ((year % 4 != 0) && ((year % 100 == 0) || (year % 400 != 0))) ? 29 : 28;
        break;
        
        case 4:
        case 6:
        case 9:
        case 11:
          days = 30;
        break;
        
        default:
          days = 0;
        break;
        
      }
    } catch (ArbIntErrors::ScalarOverflow) {
      days = 0;
    }
    
    // We done.
    return days;
    
  }
  */
  
  /***************************************************************************
   * Class Timestamp.
   ***************************************************************************/
  
  /***************************************************************************/
  // Static data members.
  
  // True when class has been initialized.
  bool Timestamp::s_initialized = false;
  
  // Default list of leap seconds.
  ReferencePointer<Timestamp::LeapSecondList> Timestamp::s_defaultleapseconds;
  
  /***************************************************************************/
  // Function members.
  
  // Default constructor.
  Timestamp::Timestamp () {
    
    // Call common init.
    _init();
    
  }
  
  // Copy constructor.
  Timestamp::Timestamp (Timestamp const& ts) {
    
    // Call common init.
    _init();
    
    // Set the timestamp.
    copy(ts);
    
  }
  
  // Set this timestamp.
  Timestamp& Timestamp::set (Interval const& time) {
    
    // Work area.
    Timestamp newtime(*this);
    
    // Verify that the new time is a valid time.
    if (!time.isSet_Second()      ||
        !time.isSet_Minute()      ||
        !time.isSet_Hour()        ||
        !time.isSet_Day()         ||
        !time.isSet_Month()       ||
        !time.isSet_Year()        ||
        !time.Year().isInteger()        || (time.Year() == 0)       ||
        !time.Month().isInteger()       || (time.Month()       < 1) || (time.Month()       > 12)                                                      ||
        //!time.Day().isInteger()         || (time.Day()         < 1) || (time.Day()         > time.Month().daysInMonth(time.Year()))                   ||
        !time.Hour().isInteger()        || (time.Hour()        < 0) || (time.Hour()        > 23)                                                      ||
        !time.Minute().isInteger()      || (time.Minute()      < 0) || (time.Minute()      > 59)                                                      ||
        !time.Second().isInteger()      || (time.Second()      < 0) || (time.Second()      > 59 + _leapSecond(time.Year(), time.Month(), time.Day())) ||
        !time.Millisecond().isInteger() || (time.Millisecond() < 0) || (time.Millisecond() > 999)
    ) {
      throw TimestampErrors::InvalidTime();
    }
    
    // Set the julian date.
    /*
    TimeVal a   = ((TimeVal(14) - time.Month()) / TimeVal(12)).floor();
    TimeVal y   = time.Year() + TimeVal(time.Year().isPositive() ? 0 : 1) + TimeVal(4800) - a;
    TimeVal m   = time.Month() + TimeVal(12) * a - TimeVal(3);
    TimeVal jdn = ((time.Year() >  _lastjulian.Year) || (
                   (time.Year() == _lastjulian.Year) && ((time.Month() >  _lastjulian.Month) || (
                                                         (time.Month() == _lastjulian.Month) && (time.Day() > _lastjulian.Day)
                                                        ))
                  )) ?
                    time.Day() + ((TimeVal(153) * m + TimeVal(2)) / TimeVal(5)).floor() + TimeVal(365) * y + (y / TimeVal(4)).floor() - (y / TimeVal(100)).floor() + (y / TimeVal(400)).floor() - TimeVal(32045)
                  :
                    time.Day() + ((TimeVal(153) * m + TimeVal(2)) / TimeVal(5)).floor() + TimeVal(365) * y + (y / TimeVal(4)).floor() - TimeVal(32083);
    newtime._jd = jdn + ((time.Hour() - TimeVal(12)) + (time.Minute() + (time.Second() + (time.Millisecond() / TimeVal(1000))) / TimeVal(60)) / TimeVal(60)) / TimeVal(24);
    */
    
    // Set the julian date. Different formulas for different calendar systems.
    if ((time.Year() >  _lastjulian.Year) || (
        (time.Year() == _lastjulian.Year) && ((time.Month() >  _lastjulian.Month) || (
                                              (time.Month() == _lastjulian.Month) && (time.Day() > _lastjulian.Day
                                             ))
        ))
       ) {
      
      // Gregorian.
      newtime._jd = 367 * time.Year() - (7 * (time.Year() + ((time.Month() + 9) / 12).toInt()) / 4).toInt()
                  - (3 * (((time.Year() + (time.Month() - 9) / 7) / 100).toInt() + 1) / 4).toInt()
                  + (275 * time.Month() / 9).toInt() + time.Day() + 1721028.5
                  + (time.Hour() + (time.Minute() + (time.Second() + (time.Millisecond() / 1000)) / 60) / 60) / 24;
      
    } else {
      
      // Julian.
      TimeVal y   = time.Year() + (time.Year().isPositive() ? 0 : 1);
      newtime._jd = 367 * y - (7 * (y + 5001 + ((time.Month() - 9) / 7).toInt()) / 4).toInt()
                  + (275 * time.Month() / 9).toInt() + time.Day() + 1729776.5
                  + (time.Hour() + (time.Minute() + (time.Second() + (time.Millisecond() / 1000)) / 60) / 60) / 24;
      
    }
    
    // We done, return.
    _jd = newtime._jd;
    return *this;
    
  }
  
  // Get the individual values of this timestamp.
  Timestamp::Interval Timestamp::get () const {
    
    // Work area.
    Interval retval;
    
    /*
    retval.Year((_jd / TimeVal(365.2425)).toInt() - TimeVal(4712));
    retval.Month(_jd - ((retval.Year() + TimeVal(4712)) * TimeVal(365.2425)));
    */
    
    // Return.
    return retval;
    
  };
  
  // Get the current system time.
  Timestamp& Timestamp::getSystemTime () {
    
    // Verify that we are on a supported platform.
  #if !defined(PLAT_WIN32) && \
      !defined(PLAT_POSIX)
    throw TimestampErrors::UnknownPlatform();
  #endif
    
    // Verify that we have the necessary system support to get the system
    // time.
  #if   defined(PLAT_WIN32)
    #if !defined(HAS__SYSTEMTIME)
    throw TimestampErrors::MissingSysSupport();
    #endif
  #elif defined(PLAT_POSIX)
    #if (!defined(HAS_GMTIME_R) && !defined(HAS_GMTIME)) || \
        !defined(HAS_TIME_T) || !defined(HAS_TM)
    throw TimestampErrors::MissingSysSupport();
    #endif
    #if !defined(HAS_GETTIMEOFDAY) || !defined(HAS_TIMEVAL) || !defined(HAS_TIMEZONE)
      #define USE_TIME
      #if !defined(HAS_TIME)
    throw TimestampErrors::MissingSysSupport();
      #endif
    #endif
  #endif
    
    // Work area.
    Timestamp       newtime(*this);
    Interval        interval;
  #if   defined(PLAT_WIN32)
    _SYSTEMTIME     systime = { 0, 0, 0, 0, 0, 0, 0, 0 };
  #elif defined(PLAT_POSIX)
    #if !defined(USE_TIME)
    struct timezone tz      = { 0, 0 };
    struct timeval  tv      = { 0, 0 };
    #endif
    time_t          t       = 0;
    struct tm       systime = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    struct tm*      stp     = &systime;
  #endif
    
    // Set the interval.
  #if   defined(PLAT_WIN32)
    GetSystemTime(&systime);
    interval.Millisecond(I_Millisecond(systime.wMilliseconds))
            .Second(I_Second(systime.wSecond))
            .Minute(I_Minute(systime.wMinute))
            .Hour(I_Hour(systime.wHour))
            .Day(I_Day(systime.wDay))
            .Month(I_Month(systime.wMonth))
            .Year(I_Year(systime.wYear));
  #elif defined(PLAT_POSIX)
    TimeVal ms;
    #if defined(USE_TIME)
    if (!time(&t)) {
      throw TimestampErrors::SysCallError();
    }
    #else
    if (gettimeofday(&tv, &tz)) {
      throw TimestampErrors::SysCallError();
    }
    t  = tv.tv_sec;
    ms = tv.tv_usec / 1000;
    #endif
    #if defined(HAS_GMTIME_R)
    if ((!gmtime_r(&t, &systime)) && (!(stp = gmtime(&t)))) {
      throw TimestampErrors::SysCallError();
    }
    #else
    if ((!stp = gmtime(&t))) {
      throw TimestampErrors::SysCallError();
    }
    #endif
    interval.Millisecond(I_Millisecond(ms))
            .Second(I_Second(stp->tm_sec))
            .Minute(I_Minute(stp->tm_min))
            .Hour(I_Hour(stp->tm_hour))
            .Day(I_Day(stp->tm_mday))
            .Month(I_Month(1) + stp->tm_mon)
            .Year(I_Year(1900) + stp->tm_year);
  #endif
    
    // Set the new time.
    newtime.set(interval);
    
    // We done, return.
    _jd = newtime._jd;
    return *this;
    
  }
  
  // Reset to just-constructed state.
  Timestamp& Timestamp::clear () {
    
    // Make a new jd.
    TimeVal tmp_jd;
    tmp_jd.PropCopy(false).Base(10).PointPos(8).Fixed(true);
    
    // Make a new lastjulian.
    YMD new_lastjulian(1582, 10, 4);
    _lastjulian = new_lastjulian;
    
    // Set the leap second list to the default.
    _leapseconds = s_defaultleapseconds;
    
    // Clear the jd.
    _jd = tmp_jd;
    
    // We done.
    return *this;
    
  }
  
  // Copy another timestamp.
  Timestamp& Timestamp::copy (Timestamp const& ts) {
    
    // Set the last julian date.
    _lastjulian = ts._lastjulian;
    
    // Set the leap second list.
    _leapseconds = ts._leapseconds;
    
    // Set the jd.
    _jd = ts._jd;
    
    // We done.
    return *this;
    
  }
  
  // Convert this timestamp to a string.
  string Timestamp::toString () const {
    
    // This is the string we will be returning.
    string retval;
    
    // We done, return the string.
    return retval;
    
  }
  
  // Common initialization tasks.
  void Timestamp::_init () {
    
    // Perform class initialization.
    if (!s_initialized) {
      s_classInit();
    }
    
    // Construct this object fully.
    clear();
    
  }
  
  // Return the leap seconds of a given day.
  Timestamp::I_Second Timestamp::_leapSecond (I_Year const& year, I_Month const& month, I_Day const& day) {
    
    // List should be very short, just iterate through it and determine if this is a leap second.
    for (LeapSecondList::iterator i = _leapseconds->begin(); i != _leapseconds->end(); ++i) {
      if ((i->Year == year) && (i->Month == month) && (i->Day == day)) {
        return i->Leap;
      }
    }
    
    // No leap second found.
    return I_Second(0);
    
  }
  
  // Class initialization.
  void Timestamp::s_classInit () {
    
    // Set the default leap seconds.
    s_defaultleapseconds = new LeapSecondList;
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1972,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1972, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1973, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1974, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1975, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1976, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1977, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1978, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1979, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1981,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1982,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1983,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1985,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1987, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1989, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1990, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1992,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1993,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1994,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1995, 12, 31), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1997,  6, 30), 1));
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(1998, 12, 31), 1));
    
    // Class has been successfully initialized.
    s_initialized = true;
    
  }
  
};
