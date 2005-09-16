/*****************************************************************************
 * Timestamp.cxx
 *****************************************************************************
 * Implementation for class Timestamp.
 *****************************************************************************/

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
  
  // Short weekday name array.
  char const* const Timestamp::SHORT_WEEKDAY_NAME[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  
  // Long weekday name array.
  char const* const Timestamp::LONG_WEEKDAY_NAME[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
  };
  
  // Short month name array.
  char const* const Timestamp::SHORT_MONTH_NAME[] = {
    "Err",
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  
  // Long month name array.
  char const* const Timestamp::LONG_MONTH_NAME[] = {
    "Error",
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"
  };
  
  // Days of year by month.
  int const Timestamp::_DAYS_OF_YEAR_NY[] = { 366, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
  int const Timestamp::_DAYS_OF_YEAR_LY[] = { 367, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };
  
  // True when class has been initialized.
  bool Timestamp::s_initialized = false;
  
  // Default list of leap seconds.
  Timestamp::LSLptr Timestamp::s_defaultleapseconds;
  
  // Default format.
  Timestamp::Formatptr Timestamp::s_defaultformat;
  
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
  
  // Conversion constructor.
  Timestamp::Timestamp (Interval const& time) {
    
    // Call common init.
    _init();
    
    // Set the timestamp.
    set(time);
    
  }
  
  // Conversion constructor.
  Timestamp::Timestamp (TimeVal const& jd) {
    
    // Call common init.
    _init();
    
    // Set the timestamp.
    set(jd);
    
  }
  
  // Set this timestamp.
  Timestamp& Timestamp::set (Interval const& time) {
    
    // Work area.
    Timestamp newtime(*this);
    Interval  settime(time);
    
    // Set default values.
    if (!settime.isSet_Year()       ) { settime.Year       (TimeVal( 1)); }
    if (!settime.isSet_Month()      ) { settime.Month      (TimeVal( 1)); }
    if (!settime.isSet_Day()        ) { settime.Day        (TimeVal( 1)); }
    if (!settime.isSet_Hour()       ) { settime.Hour       (TimeVal(12)); }
    if (!settime.isSet_Minute()     ) { settime.Minute     (TimeVal( 0)); }
    if (!settime.isSet_Second()     ) { settime.Second     (TimeVal( 0)); }
    if (!settime.isSet_Millisecond()) { settime.Millisecond(TimeVal( 0)); }
    
    // Determine the calendar type.
    CalendarType caltype = _getCalendarType(settime.Year(), settime.Month(), settime.Day());
    
    // Verify that the new time is a valid time.
    if (!settime.Year().isInteger()        || (settime.Year() == 0)       ||
        !settime.Month().isInteger()       || (settime.Month()       < 1) || (settime.Month()       > 12)                                                                                                                        ||
        !settime.Day().isInteger()         || (settime.Day()         < 1) || (settime.Day()         > _daysInMonth(settime.Year(), settime.Month()))                                                                    ||
        !settime.Hour().isInteger()        || (settime.Hour()        < 0) || (settime.Hour()        > 23)                                                                                                                        ||
        !settime.Minute().isInteger()      || (settime.Minute()      < 0) || (settime.Minute()      > 59)                                                                                                                        ||
        !settime.Second().isInteger()      || (settime.Second()      < 0) || (settime.Second()      > 59 + ((settime.Hour() == 23 && settime.Minute() == 59) ? _leapSecond(settime.Year(), settime.Month(), settime.Day()) : 0)) ||
        !settime.Millisecond().isInteger() || (settime.Millisecond() < 0) || (settime.Millisecond() > 999)
    ) {
      throw TimestampErrors::InvalidTime();
    }
    
    // Adjust BCE years.
    TimeVal y = settime.Year() + ((settime.Year() < 0) ? 1 : 0);
    
    // Set the julian date.
    TimeVal n;
    TimeVal r;
    if (settime.Month() < 3) {
      n = y - 1;
      r = 13;
    } else {
      n = y;
      r = 1;
    }
    newtime._jd.set(1720995 + floor(n * 365.25) + (30.6001 * (settime.Month() + r)).truncate() + settime.Day());
    if (caltype == CT_GREGORIAN) {
      TimeVal m = (n / 100).truncate();
      TimeVal q = 2 + 2 * (n / 400).truncate();
      newtime._jd += q - m - (m / 4).truncate();
    }
    
    // Add the time.
    newtime._jd += ((settime.Hour() - 12) + (settime.Minute() + (settime.Second() + settime.Millisecond() / 1000) / 60) / 60) / 24;
    
    // We done, return.
    _jd = newtime._jd;
    return *this;
    
  }
  
  // Get the individual values of this timestamp.
  Timestamp::Interval Timestamp::get () const {
    
    // Return value.
    Interval retval;
    
    // Work area.
    TimeVal alpha;
    TimeVal a;
    TimeVal b;
    TimeVal c;
    TimeVal d;
    TimeVal e;
    TimeVal ty;
    TimeVal tm;
    TimeVal td;
    TimeVal th;
    TimeVal tn;
    TimeVal ts;
    TimeVal ti;
    
    // Create a "corrected" julian date, minus the time. Use set() to insulate
    // from picking up _jd's fixed-pointedness. Also create a time only.
    TimeVal cjd;
    TimeVal time;
    cjd.set((_jd + 0.5).floor());
    time.set(_jd + 0.5 - cjd);
    
    // Calculate time first so that any corrections needed can be pushed up
    // to the jd and accounted for properly. In practice, this is never, ever
    // going to happen, but _just_in_case_...
    time *= 24;   th = time.toInt(); time -= th; if (time < 0) { --th; ++time; }
    time *= 60;   tn = time.toInt(); time -= tn; if (time < 0) { --tn; ++time; }
    time *= 60;   ts = time.toInt(); time -= ts; if (time < 0) { --ts; ++time; }
    time *= 1000; ti = time.toInt();
    if (ti > 999) {
      ti = 0;
      ++ts;
      if (ts > 59) {
        ts = 0;
        ++tn;
        if (tn > 59) {
          tn = 0;
          ++th;
          if (th > 24) {
            th = 0;
            ++cjd;
          }
        }
      }
    }
    
    // Correct for Julian / Gregorian calendars.
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
    
    td = b - d - floor(30.6001 * e);
    tm = e - 1;
    if (tm > 12) {
      tm -= 12;
    }
    ty = c - 4715;
    if (tm > 2) {
      --ty;
    }
    if (ty <= 0) {
      --ty;
    }
    
    // Return.
    retval.Millisecond(ti)
          .Second     (ts)
          .Minute     (tn)
          .Hour       (th)
          .Day        (td)
          .Month      (tm)
          .Year       (ty);
    return retval;
    
  }
  
  // Get the current system time.
  Timestamp& Timestamp::getSystemTime () {
    
    // Work area.
    Timestamp  newtime(*this);
    Interval   interval;
    time_t     t       = 0;
    struct tm  systime = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    struct tm* stp     = &systime;
    
    // Set the interval.
    if (!time(&t)) {
      throw TimestampErrors::SysCallError();
    }
    if (!(stp = gmtime(&t))) {
      throw TimestampErrors::SysCallError();
    }
    interval.Second(TimeVal(stp->tm_sec))
            .Minute(TimeVal(stp->tm_min))
            .Hour(TimeVal(stp->tm_hour))
            .Day(TimeVal(stp->tm_mday))
            .Month(TimeVal(1) + stp->tm_mon)
            .Year(TimeVal(1900) + stp->tm_year);
    
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
    tmp_jd.Base(10).PointPos(8).Fixed(true);
    
    // Make a new lastjulian.
    YMD     new_lastjulianymd(1582, 10, 4);
    TimeVal new_lastjulianjd(2299160);
    _lastjulianymd = new_lastjulianymd;
    _lastjulianjd  = new_lastjulianjd;
    
    // Set the GMT offset.
    _offset = 0;
    
    // Set the leap second list to the default.
    _leapseconds = s_defaultleapseconds;
    
    // Set the format to the default.
    _format = s_defaultformat;
    
    // Set the calendar type to default.
    _caltype = CT_DEFAULT;
    
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
    
    // Set the format.
    _format = ts._format;
    
    // Set the calendar type.
    _caltype = ts._caltype;
    
    // Set the jd.
    _jd = ts._jd;
    
    // Set the GMT offset.
    _offset = ts._offset;
    
    // We done.
    return *this;
    
  }
  
  // Convert this timestamp to a string.
  string Timestamp::toString (string const& format) const {
    
    // This is the string we will be returning.
    string retval;
    
    // Different types of field padding.
    enum PadType { PAD_DEPAD, PAD_NOPAD, PAD_SPPAD, PAD_ZRPAD };
    
    // Select the format.
    ConstReferencePointer<Format> fmt;
    if (format.empty()) {
      fmt = _format;
    } else {
      fmt = new Format(format);
    }
    
    // Check for control characters.
    for (Format::const_iterator i = fmt->begin(); i != fmt->end(); ++i) {
      
      // Escape character.
      if (*i == '%') {
        
        // No dangling escape character.
        if (++i == fmt->end()) {
          throw TimestampErrors::BadFormat().Problem("Dangling escape character").Position(i - 1 - fmt->begin()).Format(fmt);
        }
        
        // Modifiers.
        PadType mod = PAD_DEPAD;
        switch (*i) {
          case '-': mod = PAD_NOPAD; ++i; break;
          case '_': mod = PAD_SPPAD; ++i; break;
        }
        
        // No dangling modifier.
        if (i == fmt->end()) {
          throw TimestampErrors::BadFormat().Problem("Dangling modifier").Position(i - 1 - fmt->begin()).Format(fmt);
        }
        
        // Next character is a format option.
        string::size_type fieldlen = 0;
        PadType           fieldpad = PAD_ZRPAD;
        string            numfield;
        switch (*i) {
          
          // Literal %.
          case '%': {
            retval += '%';
          } break;
          
          // Abbreviated weekday name.
          case 'a': {
            retval += SHORT_WEEKDAY_NAME[dow()];
          } break;
          
          // Full weekday name, variable length.
          case 'A': {
            retval += LONG_WEEKDAY_NAME[dow()];
          } break;
          
          // Abbreviated month name.
          case 'b': {
            retval += SHORT_MONTH_NAME[get().Month()];
          } break;
          
          // Full month name, variable length.
          case 'B': {
            retval += LONG_MONTH_NAME[get().Month()];
          } break;
          
          // Default date and time format.
          case 'c': {
            retval += toString("%a %b %e %T %Y");
          } break;
          
          // Century (00..99).
          case 'C': {
            fieldlen = 2;
            numfield = ((get().Year() / 100).floor().abs() % 100).toString();
          } break;
          
          // Day of month (01..31).
          case 'd': {
            fieldlen = 2;
            numfield = get().Day().toString();
          } break;
          
          // Date (mm/dd/yy).
          case 'D': {
            retval += toString("%m/%d/%y");
          } break;
          
          // Day of month, blank padded.
          case 'e': {
            fieldlen = 2;
            fieldpad = PAD_SPPAD;
            numfield = get().Month().toString();
          } break;
          
          // Same as %Y-%m-%d, ISO 8601:2000.
          case 'F': {
            retval += toString("%Y-%m-%d");
          } break;
          
          // The 2-digit year corresponding to the %V week number.
          case 'g': {
            fieldlen = 2;
            numfield = (getISOYear() % 100).abs().toString();
          } break;
          
          // The ISO year.
          case 'G': {
            retval += get().Year().toString();
          } break;
          
          // Equivalent to %b.
          case 'h': {
            retval += toString("%b");
          } break;
          
          // 24-hour clock hour (00..23).
          case 'H': {
            fieldlen = 2;
            numfield = get().Hour().toString();
          } break;
          
          // 12-hour clock hour (01..12).
          case 'I': {
            fieldlen = 2;
            TimeVal hour = get().Hour();
            numfield = ((hour > 12) ? hour - 12 : ((hour == 0) ? hour + 12 : hour)).toString();
          } break;
          
          // Day of year (001..366).
          case 'j': {
            fieldlen = 3;
            numfield = doy().toString();
          } break;
          
          // Month (01..12).
          case 'm': {
            fieldlen = 2;
            numfield = get().Month().toString();
          } break;
          
          // Minute (00..59).
          case 'M': {
            fieldlen = 2;
            numfield = get().Minute().toString();
          } break;
          
          // Newline.
          case 'n': {
            retval += "\n";
          } break;
          
          // Capital AM/PM indicator.
          case 'p': {
            retval += (get().Hour() < 12) ? "AM" : "PM";
          } break;
          
          // Lowercase am/pm indicator.
          case 'P': {
            retval += (get().Hour() < 12) ? "am" : "pm";
          } break;
          
          // Time in am/pm notation. In POSIX locale this is equivalent to
          // "%I:%M:%S %p".
          case 'r': {
            retval += toString("%I:%M:%S %p");
          } break;
          
          // Time in 24-hour notation (%H:%M).
          case 'R': {
            retval += toString("%H:%M");
          } break;
          
          // Second (00..60).
          case 'S': {
            fieldlen = 2;
            numfield = get().Second().toString();
          } break;
          
          // Horizontal tab.
          case 't': {
            retval += "\t";
          } break;
          
          // Time (%H:%M:%S).
          case 'T': {
            retval += toString("%H:%M:%S");
          } break;
          
          // ISO weekday (1=Monday..7=Sunday).
          case 'u': {
            retval += dowISO().toString();
          } break;
          
          // Week number. First Sunday of January is the first day of week 1.
          // Days before this are week 0 (00..53).
          case 'U': {
            fieldlen = 2;
            numfield = woy(DOW_SUNDAY).toString();
          } break;
          
          // Week of year according to ISO-8601 rules, week 1 of a given year
          // is the week containing the 4th of January, Monday is the first
          // day of the week. (01..53).
          case 'V': {
            fieldlen = 2;
            numfield = woyISO().toString();
          } break;
          
          // Weekday number (0=Sunday..6=Saturday).
          case 'w': {
            retval += dow().toString();
          } break;
          
          // Week number. First Monday of January is the first day of week 1.
          // Days before this are week 0 (00..53).
          case 'W': {
            fieldlen = 2;
            numfield = woy(DOW_MONDAY).toString();
          } break;
          
          // Date represtation (%m/%d/%y).
          case 'x': {
            retval += toString("%m/%d/%y");
          } break;
          
          // Time representation (%H:%M:%S).
          case 'X': {
            retval += toString("%H:%M:%S");
          } break;
          
          // Last two digits of the year.
          case 'y': {
            fieldlen = 2;
            numfield = (get().Year() % 100).toString();
          } break;
          
          // Year.
          case 'Y': {
            retval += get().Year().toString();
          } break;
          
          // Offset from UTC in the ISO 8601:2000 standard format
          // (+hhmm or -hhmm), or by nothing if no timezone is determinable.
          // FIXME: implement timezone support.
          case 'z': {
            retval += "";
          } break;
          
          // Timezone name or abbreviation, or nothing if no timezone.
          // FIXME: implement timezone support.
          case 'Z': {
            retval += "";
          } break;
          
          // Unknown option.
          default: {
            throw TimestampErrors::BadFormat().Problem("Invalid character").Position(i - fmt->begin()).Format(fmt);
          } break;
          
        }
        
        // Add numeric field.
        if (!numfield.empty()) {
          
          // Pad if necessary.
          if (numfield.length() < fieldlen) {
            switch ((mod == PAD_DEPAD) ? fieldpad : mod) {
              case PAD_DEPAD:
              case PAD_ZRPAD: numfield.insert(0, fieldlen - numfield.length(), '0'); break;
              case PAD_NOPAD: break;
              case PAD_SPPAD: numfield.insert(0, fieldlen - numfield.length(), ' '); break;
            }
          }
          
          // Add the field.
          retval += numfield;
          
        }
        
      // Regular character.
      } else {
        retval += *i;
      }
      
    }
    
    // We done, return the string.
    return retval;
    
  }
  
  // Add.
  Timestamp& Timestamp::op_add (TimeVal const& tv, ValueType const vt) {
    
    // Work area.
    Timestamp newtime(*this);
    
    // Add differently for the different value types.
    switch (vt) {
      
      // Years.
      case VT_YEAR: {
        Interval oldtime(newtime.get());
        TimeVal  wholeyears = tv.truncate();
        newtime.set(oldtime.Year(oldtime.Year() + wholeyears));
        newtime._jd += (tv - wholeyears) * ((_getCalendarType(newtime._jd) == CT_GREGORIAN) ? 365.2425 : 365.25);
      } break;
      
      // Months.
      case VT_MONTH: {
        Interval oldtime(newtime.get());
        TimeVal wholemonths = tv.truncate();
        TimeVal tmpmonths   = wholemonths + oldtime.Month();
        TimeVal years       = floor(tmpmonths / 12);
        oldtime.Year(oldtime.Year() + years);
        tmpmonths -= years * 12;
        newtime.set(oldtime.Month(tmpmonths));
        newtime._jd += (tv - wholemonths) * ((_getCalendarType(newtime._jd) == CT_GREGORIAN) ? 365.2425 : 365.25) / 12;
      } break;
      
      // Days.
      case VT_DAY: {
        newtime._jd += tv;
      } break;
      
      // Hours.
      case VT_HOUR: {
        newtime._jd += tv / 24;
      }
      
      // Minutes.
      case VT_MINUTE: {
        newtime._jd += tv / 1440;
      }
      
      // Seconds.
      case VT_SECOND: {
        newtime._jd += tv / 86400;
      }
      
      // Milliseconds.
      case VT_MILLISECOND: {
        newtime._jd += tv / 86400000;
      }
      
    }
    
    // Move the new time into place and return.
    _jd = newtime._jd;
    return *this;
    
  }
  
  // Get the day of the week.
  Timestamp::TimeVal Timestamp::dow () const {
    
    // Day of week is simply the JD modulo 7 rot 1.
    TimeVal daynum;
    daynum.set((_jd + 0.5).floor() % 7 + 1);
    return (daynum > static_cast<unsigned int>(DOW_SATURDAY)) ? daynum - 7 : daynum;
    
  }
  
  // Get the ISO day of the week.
  Timestamp::TimeVal Timestamp::dowISO () const {
    
    // ISO day of the week is 1 = Monday through 7 = Sunday.
    TimeVal retval = dow();
    if (retval == 0) {
      retval = 7;
    }
    return retval;
    
  }
  
  // Get the day of the year.
  Timestamp::TimeVal Timestamp::doy () const {
    
    // Cache individual values.
    Interval now(get());
    
    // This is it.
    return (_isLeapYear(now.Year()) ? _DAYS_OF_YEAR_LY[now.Month()] : _DAYS_OF_YEAR_NY[now.Month()]) + now.Day();
    
  }
  
  // Get the week of the year.
  Timestamp::TimeVal Timestamp::woy (DayOfWeek const base) const {
    
    // Get the first day of this year.
    Timestamp dayone(Interval().Year(get().Year()).Month(TimeVal(static_cast<unsigned int>(MON_JANUARY))).Day(TimeVal(1)));
    
    // Find the next base day.
    if (dayone.dow() > static_cast<unsigned int>(base)) {
      dayone += 7 - dayone.dow();
    }
    dayone += static_cast<unsigned int>(base) - dayone.dow();
    
    // Get the difference between today and the first day of the first week of
    // the year.
    TimeVal woyday = this->jdday() - (dayone.jdday() - 1);
    
    // Divide by 7, that's the week.
    return ceil(woyday / 7);
    
  }
  
  // Get the ISO week.
  Timestamp::TimeVal Timestamp::woyISO () const {
    
    // Get the ISO year so we know what year to calculate from.
    TimeVal year = getISOYear();
    
    // Get the first ISO day. ISO week one is the first week of January that
    // contains a Thursday, or more easily, the first that contains January
    // 4th. Get the Monday of the week containing January 4th.
    Timestamp isodayone;
    isodayone.set(Interval().Year(year).Month(TimeVal(static_cast<unsigned int>(MON_JANUARY))).Day(TimeVal(4)));
    isodayone -= isodayone.dowISO() - static_cast<unsigned int>(ISO_DOW_MONDAY);
    
    // Get the difference between this date and the 1st ISO day.
    TimeVal isoday = this->jdday() - (isodayone.jdday() - 1);
    
    // Divide by 7, that's the ISO week.
    return ceil(isoday / 7);
    
  }
  
  // Get the ISO year.
  Timestamp::TimeVal Timestamp::getISOYear () const {
    
    // Get the current date as individual values.
    Interval date(get());
    
    // If it is before January 4th and this week did not contain a Thursday,
    // then according to ISO it is last year.
    if ((date.Month() == 1 && date.Day() < 4) && dowISO() - date.Day() >= static_cast<unsigned int>(ISO_DOW_THURSDAY)) {
      return date.Year() - 1;
    }
    
    // If it is after December 28th and this week will not contain a Thursday,
    // then according to ISO it is next year.
    if ((date.Month() == 12 && date.Day() > 28) && date.Day() + static_cast<unsigned int>(ISO_DOW_THURSDAY) - dowISO() > _daysInMonth(date.Year(), TimeVal(static_cast<unsigned int>(MON_DECEMBER)))) {
      return date.Year() + 1;
    }
    
    // Otherwise, it's this year.
    return date.Year();
    
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
  bool Timestamp::_isLeapYear (TimeVal const& year) const {
    
    // Work area.
    bool retval = false;
    
    // Determine the calendar type. The calendar that matters is the one that
    // was in effect during the (potential) leap day. Since we do not know if
    // 2/29 occured this year, setting the date to 3/0 will amount to the same
    // thing. Watch out for bugs here if any changes are made to
    // _getCalendarType internally.
    CalendarType caltype = _getCalendarType(year, TimeVal(3), TimeVal(0));
    
    // The year 0 did not exist.
    if (year == 0) {
      throw TimestampErrors::NoYearZero();
    }
    
    // Correct for no 0 year.
    TimeVal y = year + ((year < 0) ? 1 : 0);
    
    // Choose the calendar type.
    switch (caltype) {
      
      case CT_DEFAULT:
      case CT_GREGORIAN:
        retval = y % 4 == 0 && (y % 100 != 0 || y % 400 == 0);
      break;
      
      case CT_JULIAN:
        retval = y % 4 == 0;
      break;
      
    }
    
    // We done, return result.
    return retval;
    
  }
  
  // Get the number of days in this month.
  Timestamp::TimeVal Timestamp::_daysInMonth (TimeVal const& year, TimeVal const& month) const {
    
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
          days = _isLeapYear(year) ? 29 : 28;
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
    s_defaultleapseconds->push_back(LeapSecondDay(YMD(2005, 12, 31), 1));
    
    // Set the default format.
    s_defaultformat = new Format("%c");
    
    // Class has been successfully initialized.
    s_initialized = true;
    
  }
  
}
