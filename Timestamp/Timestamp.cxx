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
  Timestamp& Timestamp::set (Interval const& time, CalendarType caltype) {
    
    // Work area.
    Timestamp newtime(*this);
    
    // Determine the calendar type.
    caltype = (caltype == CT_DEFAULT) ? _getCalendarType(time.Year(), time.Month(), time.Day()) : caltype;
    
    // Verify that the new time is a valid time.
    if (!time.isSet_Second()            ||
        !time.isSet_Minute()            ||
        !time.isSet_Hour()              ||
        !time.isSet_Day()               ||
        !time.isSet_Month()             ||
        !time.isSet_Year()              ||
        !time.Year().isInteger()        || (time.Year() == 0)       ||
        !time.Month().isInteger()       || (time.Month()       < 1) || (time.Month()       > 12)                                                                                                         ||
        !time.Day().isInteger()         || (time.Day()         < 1) || (time.Day()         > _daysInMonth(time.Year(), time.Month(), caltype))                                                                ||
        !time.Hour().isInteger()        || (time.Hour()        < 0) || (time.Hour()        > 23)                                                                                                         ||
        !time.Minute().isInteger()      || (time.Minute()      < 0) || (time.Minute()      > 59)                                                                                                         ||
        !time.Second().isInteger()      || (time.Second()      < 0) || (time.Second()      > 59 + ((time.Hour() == 23 && time.Minute() == 59) ? _leapSecond(time.Year(), time.Month(), time.Day()) : 0)) ||
        !time.Millisecond().isInteger() || (time.Millisecond() < 0) || (time.Millisecond() > 999)
    ) {
      throw TimestampErrors::InvalidTime();
    }
    
    // Adjust BCE years.
    TimeVal y(time.Year() + ((time.Year() < 0) ? 1 : 0));
    
    // Set the julian date.
    TimeVal n;
    TimeVal r;
    if (time.Month() < 3) {
      n = y - 1;
      r = 13;
    } else {
      n = y;
      r = 1;
    }
    newtime._jd = 1720995 + floor(n * 365.25) + (30.6001 * (time.Month() + r)).truncate() + time.Day();
    if (caltype == CT_GREGORIAN) {
      TimeVal m((n / 100).truncate());
      TimeVal q(2 + 2 * (n / 400).truncate());
      newtime._jd += q - m - (m / 4).truncate();
    }
    
    // Add the time.
    newtime._jd += ((time.Hour() - 12) + (time.Minute() + (time.Second() + time.Millisecond() / 1000) / 60) / 60) / 24;
    
    // We done, return.
    _jd = newtime._jd;
    return *this;
    
  }
  
  // Get the individual values of this timestamp.
  Timestamp::Interval Timestamp::get () const {
    
    // Work area.
    Interval retval;
    
    TimeVal alpha;
    TimeVal a;
    TimeVal b;
    TimeVal c;
    TimeVal d;
    TimeVal e;
    
    // Create a "corrected" julian date, minus the time. Set propcopy to false
    // so that we don't grab _jd's fixed-decimal status.
    TimeVal cjd;
    cjd.PropCopy(false);
    cjd = (_jd + 0.5).floor();
    
    // I don't know. It works, that's all that matters.
    if (cjd > _lastjulianjd) {
      alpha = floor((cjd - 1867216 - 0.25) / 36524.25);
      a     = cjd + 1 + alpha - floor(alpha * 0.25);
    } else {
      a     = cjd;
    }
    
    b = a + 1524;
    c = 6680 + floor((b - 2439870 - 122.1) / 365.25);
    d = 365 * c + floor(0.25 * c);
    e = floor((b - d) / 30.6001);
    
    retval.Day(b - d - floor(30.6001 * e));
    retval.Month(e - 1);
    if (retval.Month() > 12) {
      retval.Month(retval.Month() - 12);
    }
    retval.Year(c - 4715);
    if (retval.Month() > 2) {
      retval.Year(retval.Year() - 1);
    }
    if (retval.Year() <= 0) {
      retval.Year(retval.Year() - 1);
    }
    
    // Get the time.
    TimeVal time(cjd - _jd + 0.5);
    cout << "time: " << time << endl;
    time *= 24;
    retval.Hour(time.floor());
    time -= retval.Hour();
    time *= 60;
    retval.Minute(time.floor());
    time -= retval.Minute();
    time *= 60;
    retval.Second(time.floor());
    time -= retval.Second();
    time *= 1000;
    retval.Millisecond(time.floor());
    
    // Return.
    return retval;
    
  }
  
  // Get the current system time.
  Timestamp& Timestamp::getSystemTime () {
    
    // Verify that we are on a supported platform.
  #if !defined(PLAT_WIN32) && \
      !defined(PLAT_POSIX)
    throw TimestampErrors::UnknownPlatform();
  #endif
    
    // Verify that we have the necessary system support to get the system
    // time.
  #if defined(PLAT_WIN32)
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
  #if defined(PLAT_WIN32)
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
  #if defined(PLAT_WIN32)
    GetSystemTime(&systime);
    interval.Millisecond(TimeVal(systime.wMilliseconds))
            .Second(TimeVal(systime.wSecond))
            .Minute(TimeVal(systime.wMinute))
            .Hour(TimeVal(systime.wHour))
            .Day(TimeVal(systime.wDay))
            .Month(TimeVal(systime.wMonth))
            .Year(TimeVal(systime.wYear));
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
    interval.Millisecond(TimeVal(ms))
            .Second(TimeVal(stp->tm_sec))
            .Minute(TimeVal(stp->tm_min))
            .Hour(TimeVal(stp->tm_hour))
            .Day(TimeVal(stp->tm_mday))
            .Month(TimeVal(1) + stp->tm_mon)
            .Year(TimeVal(1900) + stp->tm_year);
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
    YMD     new_lastjulianymd(1582, 10, 4);
    TimeVal new_lastjulianjd(2299160);
    _lastjulianymd = new_lastjulianymd;
    _lastjulianjd  = new_lastjulianjd;
    
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
    _lastjulianymd = ts._lastjulianymd;
    _lastjulianjd  = ts._lastjulianjd;
    
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
  
  // Get whether a given year is a leap year.
  bool Timestamp::_isLeapYear (TimeVal const& year, CalendarType caltype) const {
    
    // Determine the calendar type. The calendar that matters is the one that
    // was in effect during the (potential) leap day. Since we do not know if
    // 2/29 occured this year, setting the date to 3/0 will amount to the same
    // thing. Watch out for bugs here if any changes are made to
    // _getCalendarType internally.
    caltype = (caltype == CT_DEFAULT) ? _getCalendarType(year, TimeVal(3), TimeVal(0)) : caltype;
    
    // The year 0 did not exist.
    if (year == 0) {
      throw TimestampErrors::NoYearZero();
    }
    
    // Correct for no 0 year.
    TimeVal y(year + ((year < 0) ? 1 : 0));
    
    // Choose the calendar type.
    switch (caltype) {
      
      case CT_DEFAULT:
      case CT_GREGORIAN:
        return y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
      break;
      
      case CT_JULIAN:
        return y % 4 == 0;
      break;
      
    }
    
    // Dummy instruction to avoid warning.
    return false;
    
  }
  
  // Get the number of days in this month.
  Timestamp::TimeVal Timestamp::_daysInMonth (TimeVal const& year, TimeVal const& month, CalendarType const caltype) const {
    
    // Work area.
    Timestamp::TimeVal days;
    
    // Easy.
    try {
      switch (month.Value<int>()) {
        
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
          days = _isLeapYear(year, caltype) ? 29 : 28;
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
  
  // Return the leap seconds of a given day.
  Timestamp::TimeVal Timestamp::_leapSecond (TimeVal const& year, TimeVal const& month, TimeVal const& day) const {
    
    // List should be very short, just iterate through it and determine if this is a leap second.
    for (LeapSecondList::iterator i = _leapseconds->begin(); i != _leapseconds->end(); ++i) {
      if ((i->Year == year) && (i->Month == month) && (i->Day == day)) {
        return i->Leap;
      }
    }
    
    // No leap second found.
    return TimeVal(0);
    
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
  
}
