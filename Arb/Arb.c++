/*****************************************************************************
 * Arb.c++
 *****************************************************************************
 * Implementation for class Arb.
 *****************************************************************************/

// STL includes.
  #include <string>
  #include <algorithm>

// System includes.
  #include <rppower.h++>
  #include <to_string.h++>
  #include <reduce.h++>
  #include <CaseConvert.h++>

// Class include.
  #include "Arb.h++"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Class Arb.
   ***************************************************************************/
  
  /***************************************************************************/
  // Constants.
  
  // Float info.
  template <> unsigned int const Arb::_FloatInfo<float      >::mantissabits =    24;
  template <> unsigned int const Arb::_FloatInfo<float      >::exponentbits =     8;
  template <> unsigned int const Arb::_FloatInfo<float      >::bias         =   127;
  template <> unsigned int const Arb::_FloatInfo<double     >::mantissabits =    53;
  template <> unsigned int const Arb::_FloatInfo<double     >::exponentbits =    11;
  template <> unsigned int const Arb::_FloatInfo<double     >::bias         =  1023;
  template <> unsigned int const Arb::_FloatInfo<long double>::mantissabits =    64;
  template <> unsigned int const Arb::_FloatInfo<long double>::exponentbits =    15;
  template <> unsigned int const Arb::_FloatInfo<long double>::bias         = 16383;
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Default constructor.
   */
  Arb::Arb () {
    
    // Call common init.
    _init();
    
  }
  
  /*
   * Copy constructor.
   */
  Arb::Arb (Arb const& number) {
    
    // Call common init.
    _init();
    
    // Set the number.
    copy(number);
    
  }
  
  /*
   * Conversion constructor.
   */
  Arb::Arb (std::string const& number) {
    
    // Call common init.
    _init();
    
    // Set the number.
    set(number);
    
  }
  Arb::Arb (ArbInt const& number) {
    
    // Call common init.
    _init();
    
    // Set the number.
    set(number);
    
  }
  
  /*
   * Accessors.
   */
  Arb& Arb::Base (BaseT const base) {
    if (base != _base) {
      _base = base;
      if (_fix && _fixtype == FIX_RADIX) {
        _fixq = _DigsT(_base).pow(_pointpos);
        _reduce();
      }
    }
    return *this;
  }
  Arb& Arb::PointPos (PointPosT const pointpos) {
    if (pointpos != _pointpos) {
      _pointpos = pointpos;
      if (_fix && _fixtype == FIX_RADIX) {
        _fixq = _DigsT(_base).pow(_pointpos);
        _reduce();
      }
    }
    return *this;
  }
  Arb& Arb::Fixed (bool const fixed) {
    if (fixed != _fix) {
      _fix = fixed;
      if (_fix && _fixtype == FIX_RADIX) {
        _fixq = _DigsT(_base).pow(_pointpos);
      }
      _reduce();
    }
    return *this;
  }
  Arb& Arb::Fix (FixType const fix) {
    if (fix != _fixtype) {
      _fixtype = fix;
      if (_fix) {
        _reduce();
      }
    }
    return *this;
  }
  Arb& Arb::FixQ (Arb const& fixq) {
    if (fixq != _fixq) {
      if (!fixq.isInteger()) {
        throw Arb::Errors::NonInteger();
      }
      _fixq    = fixq._p;
      _fixtype = FIX_DENOM;
      if (_fix) {
        _reduce();
      }
    }
    return *this;
  }
  
  /*
   * Reset to just-constructed state.
   */
  void Arb::clear () {
    
    // Operations that may throw, but do not change the number.
    _DigsT new_p;
    _DigsT new_q;
    _DigsT new_fixq;
    new_q    = 1;
    new_fixq = 1;
    
    // Operations that never throw.
    _p   .swap(new_p   );
    _q   .swap(new_q   );
    _fixq.swap(new_fixq);
    _positive = true      ;
    _pointpos =          0;
    _fix      = false     ;
    _fixtype  = FIX_RADIX ;
    _base     =         10;
    _maxradix =         10;
    _format   = FMT_RADIX ;
    _round    = ROUND_EVEN;
    
  }
  
  /*
   * Copy another number.
   */
  void Arb::copy (Arb const& number) {
    
    // Operations that may throw, but do not change the number.
    _DigsT new_p   (number._p   );
    _DigsT new_q   (number._q   );
    _DigsT new_fixq(number._fixq);
    
    // Operations that never throw.
    _p   .swap(new_p   );
    _q   .swap(new_q   );
    _fixq.swap(new_fixq);
    _positive = number._positive;
    _pointpos = number._pointpos;
    _fix      = number._fix     ;
    _fixtype  = number._fixtype ;
    _base     = number._base    ;
    _maxradix = number._maxradix;
    _format   = number._format  ;
    _round    = number._round   ;
    
  }
  
  /*
   * Convert this number to a string.
   */
  string& Arb::to_string (string& buffer, OutputFormat const format) const {
    
    // This is the string we will be returning.
    string retval;
    
    // Determine the output format.
    OutputFormat fmt = (format == FMT_DEFAULT) ? _format : format;
    
    // Choose the output format.
    switch (fmt) {
      
      // Output both notations.
      case FMT_BOTH: {
        retval += to_string(FMT_RADIX) + " " + to_string(FMT_FRACTION);
      } break;
      
      // Output in radix notation.
      case FMT_DEFAULT:
      case FMT_RADIX  : {
        
        // Work area.
        _DigsT numeric;
        numeric.Base(_base);
        
        // Get the whole number part.
        numeric = _p / _q;
        
        // This is the number of digits from the end of the string to put
        // the radix point.
        std::string::size_type radixpos = 0;
        
        // Create the radix part.
        _DigsT remainder(_p % _q);
        if ((_fix && _fixq != 1) || (!_fix && remainder)) {
          
          // Get the radix digits, one by one. Output digits until the entire
          // fraction is output or the maximum requested significant radix digits
          // are output.
          std::string::size_type sigdigs  = 0;
          bool                   sigstart = (numeric != 0);
          _DigsT                 digit;
          while ((sigdigs < _maxradix) && (remainder != 0)) {
            
            // Push one base digit to a whole number.
            remainder *= _base;
            digit      = remainder / _q;
            
            // Push that digit onto the end of the number we are constructing
            // and increment the number of radix positions.
            numeric.push_back(digit);
            ++radixpos;
            
            // Remove that digit from the remainder.
            remainder %= _q;
            
            // Start incrementing significant digits when we actually hit one,
            // do not count 0s.
            if (sigstart || (digit != 0)) {
              ++sigdigs;
              sigstart = true;
            }
            
          }
          
          // Round.
          if (remainder != 0) {
            switch (_round) {
              case ROUND_UP: {
                if (_positive) {
                  ++numeric;
                }
              } break;
              case ROUND_DOWN: {
                if (!_positive) {
                  ++numeric;
                }
              } break;
              case ROUND_TOWARD_ZERO: {
              } break;
              case ROUND_FROM_ZERO: {
                ++numeric;
              } break;
              case ROUND_NORMAL: {
                if (remainder * 2 >= _q) {
                  ++numeric;
                }
              } break;
              case ROUND_DEFAULT:
              case ROUND_EVEN: {
                // Double remainder to make it easy to check for half over,
                // but when checking for odd, make sure to op& with 2 since we
                // just left-shifted by 1.
                remainder *= 2;
                if (remainder > _q || (remainder == _q && remainder & 2)) {
                  ++numeric;
                }
              } break;
            }
          }
          
          // If this is a fixed-radix number, fix the radix.
          if (_fix) {
          
            // Pad with zeros.
            if (radixpos < _pointpos) {
              numeric  *= _DigsT(_base).pow(_pointpos - radixpos);
              radixpos  = _pointpos;
            }
            
          }
          
        }
        
        // Convert the number to a string. Make sure that numeric is in the
        // desired base before this line.
        {
          retval += numeric.to_string();
        }
        
        // Add placeholder zeros.
        if (radixpos >= retval.length()) {
          retval.insert(0, radixpos - retval.length() + 1, '0');
        }
        
        // Place the radix point.
        if (radixpos) {
          retval.insert(retval.length() - radixpos, 1, '.');
        }
        
        // If this is not a fixed-radix number, we may need to remove the radix
        // point.
        if (!_fix && radixpos > 0) {
          
          // Strip insignificant zeros.
          if (retval.find_last_not_of('0') != string::npos) {
            retval.erase(retval.find_last_not_of('0') + 1);
          }
          
          // Remove the radix point if it is dangling.
          if ((retval.size() - 1) == retval.find('.')) {
            retval.erase(retval.find('.'));
          }
          
        }
        
      } break;
      
      // Output in fractional format.
      case FMT_FRACTION: {
        retval = _p.to_string(_base) + "/" + _q.to_string(_base);
      } break;
      
    }
        
    // Output the sign if negative, unless outputting both formats.
    if ((fmt != FMT_BOTH) && !(_positive) && (_p != 0)) {
      retval.insert(0, 1, '-');
    }
    
    // We done.
    buffer.swap(retval);
    return buffer;
    
  }
  
  /*
   * Set the number from a string.
   */
  void Arb::set (string const& number, bool const autobase) {
    
    // Parser will load data into here.
    string            num               ;
    string            rad               ;
    string            exp               ;
    bool              p_num      = true ;
    bool              p_exp      = true ;
    string::size_type nexp       =     0;
    string::size_type numstart   =     0;
    string::size_type radstart   =     0;
    string::size_type expstart   =     0;
    string::size_type numlen     =     0;
    string::size_type radlen     =     0;
    string::size_type explen     =     0;
    bool              numstarted = false;
    bool              radstarted = false;
    bool              expstarted = false;
    BaseT             num_base   = _base;
    
    // Parse the number, scoped for temp variables.
    {
      
      // Data for initial pass.
      enum Mode { NUM, RAD, EXP } mode = NUM;
      string::size_type length    = number.length();
      bool              s_num     = false;
      bool              s_exp     = false;
      bool              diggiven  = false;
      string::size_type num_start =     0;
      
      // Check for sign before number base.
      if (number.length() > 1) {
        if (number[0] == '+') {
          s_num     = true;
          p_num     = true;
          num_start =    1;
        } else if (number[0] == '-') {
          s_num     = true ;
          p_num     = false;
          num_start =     1;
        }
      }
      
      // Determine the number base.
      if (autobase) {
        string baseprefix(number.substr(num_start, 2));
        uppercase(baseprefix);
        if (number.length() > 2 + num_start && baseprefix == "0X") {
          num_base  = 16;
          num_start =  2 + num_start;
        } else if (number.length() > 2 + num_start && baseprefix == "0B") {
          num_base  = 2;
          num_start = 2 + num_start;
        } else if (number.length() > 1 + num_start && number[num_start] == '0' && (number.length() < 3 + num_start || number[num_start + 1] != '.')) {
          num_base  = 8;
          num_start = 1 + num_start;
        }
      }
      
      for (string::size_type i = num_start; i != length; ++i) {
        
        switch (number[i]) {
          
          // Decimal point.
          case '.':
            switch (mode) {
              case NUM: mode = RAD;                                                                                break;
              case RAD: throw Arb::Errors::BadFormat().Problem("Radix point given for a second time").Position(i); break;
              case EXP: throw Arb::Errors::BadFormat().Problem("Radix point given in exponent"      ).Position(i); break;
            }
          break;
          
          // Exponent symbol.
          case 'e':
          case 'E':
            switch (mode) {
              case NUM: mode = EXP;                                                                                    break;
              case RAD: mode = EXP;                                                                                    break;
              case EXP: throw Arb::Errors::BadFormat().Problem("Exponent symbol given for a second time").Position(i); break;
            }
            diggiven = false;
          break;
          
          // Sign symbol.
          case '+':
          case '-':
            if (diggiven) {
              throw Arb::Errors::BadFormat().Problem("Sign given after digits").Position(i);
            }
            switch (mode) {
              case NUM:
                if (s_num) {
                  throw Arb::Errors::BadFormat().Problem("Sign of number given for a second time").Position(i);
                }
                p_num = (number[i] == '+');
                s_num = true;
              break;
              case RAD:
                throw Arb::Errors::BadFormat().Problem("Sign given after radix point").Position(i);
              break;
              case EXP:
                if (s_exp) {
                  throw Arb::Errors::BadFormat().Problem("Sign of exponent given for a second time").Position(i);
                }
                p_exp = (number[i] == '+');
                s_exp = true;
              break;
            }
          break;
          
          // All other characters, most likely digits.
          default:
            
            // Add the digit to its proper place.
            switch (mode) {
              case NUM: if (!numstarted) { numstarted = true; numstart = i; } ++numlen; num += number[i]; break;
              case RAD: if (!radstarted) { radstarted = true; radstart = i; } ++radlen; rad += number[i]; break;
              case EXP: if (!expstarted) { expstarted = true; expstart = i; } ++explen; exp += number[i]; break;
            }
            diggiven = true;
            
          break;
          
        }
        
      }
      
    }
    
    // Trim insignificant zeros from gathered digits.
    num.erase(0, num.find_first_not_of('0'));
    if (rad.find_last_not_of('0') != string::npos) {
      rad.erase(rad.find_last_not_of('0') + 1);
    }
    exp.erase(0, exp.find_first_not_of('0'));
    
    // Count the number of decimal positions.
    nexp = rad.size();
    
    // Concatenate numeric and radix digits.
    num += rad;
    
    // Load the number.
    try {
      _p.Base(num_base).set(num, false);
    } catch (ArbInt::Errors::BadFormat& e) {
      if (e.Position() < numstart + numlen) {
        throw Arb::Errors::BadFormat().Problem(e.Problem()).Position(e.Position() + numstart                      );
      } else {
        throw Arb::Errors::BadFormat().Problem(e.Problem()).Position(e.Position() + numstart + (radstart - numlen));
      }
    }
    
    // Adjust the number based on the exponent. Negative exponent increases
    // radix digits, positive exponent ups order of magnitude.
    {
      
      // Determine the exponent given.
      _DigsT expn;
      _DigsT expr(nexp);
      try {
        expn.Base(num_base).set(exp, false);
      } catch (ArbInt::Errors::BadFormat& e) {
        throw Arb::Errors::BadFormat().Problem(e.Problem()).Position(e.Position() + expstart);
      }
      if (p_exp) {
        if (expn >= expr) {
          expn -= expr;
        } else {
          p_exp = false;
          expn  = expr - expn;
        }
      } else {
        expn += expr;
      }
      
      // Convert to a fully-represented number (no positive exponent).
      if (!expn.isZero()) {
        
        // This calculation will be needed whether exponent is positive or
        // negative. If positive, multiply p by it and clear, thus bringing p
        // down to 1s. If negative, leave as it is, it is already accurate.
        _q = _DigsT(num_base).pow(expn);
        if (p_exp) {
          _p   *= _q;
          _q    = 1;
          expn  = 0;
        }
        
      // No exponent, this is a whole number already.
      } else {
        _q = 1;
      }
      
      // If a fixed-point number is requested, increase or decrease the
      // radix digits as needed.
      if (_fix) {
        
        // A number that is fixed by the number of radix digits needs to
        // figure q, others will be told what q is.
        if (_fixtype == FIX_RADIX) {
          
          // If the exponent is positive, it is a simple case of increasing the
          // number of radix digits, this is a number represented by 1s by now.
          if (p_exp) {
            _q  = _DigsT(_base).pow(_DigsT(_pointpos));
            _p *= _q;
          
          // If the exponent is negative, we must add or subtract the difference
          // in digits.
          } else {
            if (_DigsT(_pointpos) >= expn) {
              _DigsT mod(_DigsT(_base).pow(_DigsT(_pointpos) - expn));
              _p *= mod;
              _q *= mod;
            } else {
              _DigsT mod(_DigsT(_base).pow(expn - _DigsT(_pointpos)));
              _p /= mod;
              _q /= mod;
            }
          }
          
          // This is the q that we always need to fix at.
          _fixq = _q;
          
        }
        
      }
      
    }
    
    // Set the sign.
    _positive = p_num;
    
    // Reduce the number.
    _reduce();
    
  }
  
  /*
   * Set the number from another Arb.
   */
  void Arb::set (Arb const& number) {
    
    // Set the new number.
    _p        = number._p;
    _q        = number._q;
    _positive = number._positive;
    
    // Reduce the fraction if setting from a fixed number or if this is a
    // fixed number, should be no need otherwise.
    // if it was not fixed point.
    if (_fix || number._fix) {
      _reduce();
    }
    
  }
  
  /*
   * Set from an ArbInt.
   */
  void Arb::set (ArbInt const& number) {
    
    // Set the number.
    _p        = number;
    _q        = 1;
    _positive = true;
    
    // Reduce if this is a fixed number, otherwise no need.
    if (_fix) {
      _reduce();
    }
    
  }
  
  /*
   * Multiplication operator backend.
   */
  Arb& Arb::op_mul (Arb const& number) {
    
    // Multiply the numbers.
    _p *= number._p;
    _q *= number._q;
    
    // Set the sign.
    _positive = (_positive == number._positive);
    
    // Reduce the fraction.
    _reduce();
    
    // Done.
    return *this;
    
  }
  Arb& Arb::op_mul (ArbInt const& number) {
    
    // Multiply.
    _p *= number;
    
    // Reduce the fraction.
    _reduce();
    
    // Done.
    return *this;
    
  }
  
  /*
   * Division operator backend.
   */
  Arb& Arb::op_div (Arb const& number) {
    
    // Throw an error on divide by zero.
    if (number == 0) {
      throw Arb::Errors::DivByZero();
    }
    
    // Divide the numbers, multiply by the inverse.
    _p *= number._q;
    _q *= number._p;
    
    // Set the sign.
    _positive = (_positive == number._positive);
    
    // Reduce the fraction.
    _reduce();
    
    // Done.
    return *this;
    
  }
  Arb& Arb::op_div (ArbInt const& number) {
    
    // Throw an error on divide by zero.
    if (number == 0) {
      throw Arb::Errors::DivByZero();
    }
    
    // Divide the numbers, multiply by the inverse.
    _q *= number;
    
    // Reduce the fraction.
    _reduce();
    
    // Done.
    return *this;
    
  }
  
  /*
   * Modulo division operator backend.
   */
  Arb& Arb::op_mod (Arb const& number) {
    
    // Throw an error on divide by zero.
    if (number == 0) {
      throw Arb::Errors::DivByZero();
    }
    
    // Throw an error if both numbers are not integer.
    if (!isInteger() || !number.isInteger()) {
      throw Arb::Errors::NonInteger();
    }
    
    // Simple, just modulo divide p's.
    _p %= number._p;
    
    // We done, return.
    return *this;
    
  }
  Arb& Arb::op_mod (ArbInt const& number) {
    
    // Throw an error on divide by zero.
    if (number == 0) {
      throw Arb::Errors::DivByZero();
    }
    
    // Throw an error if this number is not integer.
    if (!isInteger()) {
      throw Arb::Errors::NonInteger();
    }
    
    // Modulo divide p.
    _p %= number;
    
    // We done, return.
    return *this;
    
  }
  
  /*
   * Addition operator backend.
   */
  Arb& Arb::op_add (Arb const& number) {
    
    // Work area.
    Arb tmp_r(number);
    
    // Numbers must have the same denominator to add.
    _normalize(tmp_r);
    
    // If signs are same add, otherwise subtract.
    if (*this < 0 == number < 0) {
      _p += tmp_r._p;
    } else {
      if (tmp_r._p > _p) {
        _p        = tmp_r._p - _p;
        _positive = !_positive;
      } else {
        _p -= tmp_r._p;
      }
    }
    
    // Reduce the fraction.
    _reduce();
    
    // We done.
    return *this;
    
  }
  Arb& Arb::op_add (ArbInt const& number) {
    
    // Work area.
    ArbInt tmp(number);
    
    // Normalize.
    tmp *= _q;
    
    // If signs are same add, otherwise subtract.
    if (*this < 0) {
      if (tmp > _p) {
        _p        = tmp - _p;
        _positive = !_positive;
      } else {
        _p -= tmp;
      }
    } else {
      _p += tmp;
    }
    
    // Reduce the fraction.
    _reduce();
    
    // We done.
    return *this;
    
  }
  
  /*
   * Subtraction operator backend.
   */
  Arb& Arb::op_sub (Arb const& number) {
    
    // Work area.
    Arb tmp_r(number);
    
    // Numbers must have the same denominator to subtract.
    _normalize(tmp_r);
    
    // If signs are same subtract, otherwise add.
    if (*this < 0 == tmp_r < 0) {
      if (tmp_r._p > _p) {
        _p        = tmp_r._p - _p;
        _positive = !_positive;
      } else {
        _p -= tmp_r._p;
      }
    } else {
      _p += tmp_r._p;
    }
    
    // Reduce the fraction.
    _reduce();
    
    // We done.
    return *this;
    
  }
  Arb& Arb::op_sub (ArbInt const& number) {
    
    // Work area.
    ArbInt tmp(number);
    
    // Normalize.
    tmp *= _q;
    
    // If signs are same subtract, otherwise add.
    if (*this < 0) {
      _p += tmp;
    } else {
      if (tmp > _p) {
        _p        = tmp - _p;
        _positive = !_positive;
      } else {
        _p -= tmp;
      }
    }
    
    // Reduce the fraction.
    _reduce();
    
    // We done.
    return *this;
    
  }
  
  /*
   * Greater-than operator backend.
   */
  bool Arb::op_gt (Arb const& number) const {
    
    // If one or both numbers are zero, compare is easy.
    if (_p.isZero()) {
      if (number._p.isZero()) {
        return false;
      } else {
        return !number._positive;
      }
    } else if (number._p.isZero()) {
      return _positive;
    }
    
    // Neither number is zero. If signs are different, they are sufficient
    // for this test.
    if (_positive && !number._positive) {
      return true;
    } else if (!_positive && number._positive) {
      return false;
    }
    
    // Signs are the same, we will have to compare numbers. Copy numbers since
    // we will be modifying them.
    Arb tmp_l(*this );
    Arb tmp_r(number);
    
    // Numbers must have the same denomiator to compare.
    tmp_l._normalize(tmp_r);
    
    // If numbers are negative, comparison is reversed.
    if (_positive) {
      return (tmp_l._p > tmp_r._p);
    } else {
      return (tmp_l._p < tmp_r._p);
    }
    
  }
  bool Arb::op_gt (ArbInt const& number) const {
    
    // If one or both numbers are zero, compare is easy.
    if (_p.isZero()) {
      return false;
    } else if (number == 0) {
      return _positive;
    }
    
    // Neither number is zero. If signs are different, they are sufficient
    // for this test.
    if (!_positive) {
      return false;
    }
    
    // Signs are the same, we will have to compare numbers. Copy numbers since
    // we will be modifying them.
    ArbInt tmp(number);
    
    // Normalize.
    tmp *= _q;
    
    // Compare.
    return _p > tmp;
    
  }
  
  /*
   * Less-than operator backend.
   */
  bool Arb::op_lt (Arb const& number) const {
    
    // If one or both numbers are zero, compare is easy.
    if (_p.isZero()) {
      if (number._p.isZero()) {
        return false;
      } else {
        return number._positive;
      }
    } else if (number._p.isZero()) {
      return !_positive;
    }
    
    // Neither number is zero. If signs are different, they are sufficient for
    // this test.
    if (_positive && !number._positive) {
      return false;
    } else if (!_positive && number._positive) {
      return true;
    }
    
    // Signs are the same, we will have to compare numbers.
    Arb tmp_l(*this );
    Arb tmp_r(number);
    
    // Numbers must have the same denominator to compare.
    tmp_l._normalize(tmp_r);
    
    // If numbers are negative, comparison is reversed.
    if (_positive) {
      return (tmp_l._p < tmp_r._p);
    } else {
      return (tmp_l._p > tmp_r._p);
    }
    
  }
  bool Arb::op_lt (ArbInt const& number) const {
    
    // If one or both numbers are zero, compare is easy.
    if (_p.isZero()) {
      return number > 0;
    } else if (number == 0) {
      return !_positive;
    }
    
    // Neither number is zero. If signs are different, they are sufficient
    // for this test.
    if (!_positive) {
      return true;
    }
    
    // Signs are the same, we will have to compare numbers. Copy numbers since
    // we will be modifying them.
    ArbInt tmp(number);
    
    // Normalize.
    tmp *= _q;
    
    // Compare.
    return _p < tmp;
    
  }
  
  /*
   * Equal-to operator backend.
   */
  bool Arb::op_eq (Arb const& number) const {
    
    // Check for 0.
    if (_p.isZero()) {
      return number._p.isZero();
    } else if (number._p.isZero()) {
      return false;
    }
    
    // Neither number is zero. Check signs.
    if (_positive != number._positive) {
      return false;
    }
    
    // Check the numbers themselves. No need to normalize, if they're not,
    // they're not equal. If one of the numbers is fixed, we need to compare
    // non-fixed versions, otherwise do it the easy way.
    if (_fix || number._fix) {
      Arb tmp_l = *this ;
      Arb tmp_r = number;
      return ((tmp_l._q == tmp_r._q) && (tmp_l._p ==tmp_r._p));
    } else {
      return ((_q == number._q) && (_p == number._p));
    }
    
  }
  bool Arb::op_eq (ArbInt const& number) const {
    
    // Check for 0.
    if (_p.isZero()) {
      return number == 0;
    } else if (number == 0) {
      return false;
    }
    
    // Neither number is zero. Check signs.
    if (!_positive) {
      return false;
    }
    
    // Check the numbers themselves. No need to normalize, if they're not,
    // they're not equal. If this number is fixed, we need to compare to a
    // non-fixed version, otherwise do it the easy way.
    if (_fix) {
      Arb tmp_l = *this;
      return (tmp_l.isInteger() && (tmp_l._p == number));
    } else {
      return (isInteger() && (_p == number));
    }
    
  }
  
  /*
   * Get the ceiling of this fractional number.
   */
  Arb Arb::ceil () const {
    
    // Work area.
    Arb retval(*this);
    
    // Only work if we have to.
    if (!isInteger()) {
      
      // Easy, p/q.
      _DigsT remainder(retval._p % retval._q);
      retval._p /= retval._q;
      retval._q  = 1;
      
      // If the number was positive and there was a remainder, we need to add
      // 1.
      if (isPositive() && remainder) {
        ++retval._p;
      }
      
    }
    
    // We done.
    return retval;
    
  }
  
  /*
   * Get the floor of this fractional number.
   */
  Arb Arb::floor () const {
    
    // Work area.
    Arb retval(*this);
    
    // Only work if we have to.
    if (!isInteger()) {
      
      // Easy, p/q.
      _DigsT remainder(retval._p % retval._q);
      retval._p /= retval._q;
      retval._q  = 1;
      
      // If the number was negative and there was a remainder, we need to
      // subtract 1. Since the number is negative, adding is subtracting.
      if (!isPositive() && (remainder != 0)) {
        ++retval._p;
      }
      
    }
    
    // We done.
    return retval;
    
  }
  
  /*
   * Truncate to just the integer portion.
   */
  Arb Arb::truncate () const {
    
    // Work area.
    Arb retval(*this);
    
    // Only work if we have to.
    if (!isInteger()) {
      
      // Easy, p/q.
      retval._p /= retval._q;
      retval._q  = 1;
      
    }
    
    // We done.
    return retval;
    
  }
  
  /*
   * Raise this number to a power.
   */
  Arb Arb::pow (Arb const& exp) const {
    
    // Work area.
    Arb retval(*this);
    
    // No work if this is zero or if we are raising to the 1.
    if (!isZero() || (exp == 1)) {
      
      // Only raise if we have to.
      if (exp != 0) {
        
        // Raise this number to an integer power.
        if (exp.isInteger()) {
          
          // No need to raise 1 or to 1.
          if ((retval.abs() != 1) && (exp.abs() != 1)) {
            
            // Raise p & q.
            retval._p = retval._p.pow(exp._p);
            retval._q = retval._q.pow(exp._p);
            
            // Reduce
            retval._reduce();
            
          }
          
          // The number is positive if it began positive or the exponent is even.
          retval._positive = retval._positive || exp.isEven();
        
        // Raising to a fraction.
        } else {
          
          // x^(y/z): find the raise to the yth power and find the zth root.
          Arb x;
          Arb y;
          Arb z;
          
          // Raise to the yth power.
          y._p = exp._p;
          x = pow(y);
          
          // Get the zth root.
          z._p = exp._q;
          retval = x.root(z);
          
        }
        
        // If exp is negative, 1/pow.
        if (!exp.isPositive()) {
          retval = 1 / retval;
        }
        
      // Raising to the 0 power is always 1.
      } else {
        
        retval = 1;
        
      }
      
    }
    
    // Return the result.
    return retval;
    
  }
  
  /*
   * Find the nth root of this number.
   */
  Arb Arb::root (Arb const& n) const {
    
    // No divide by zero.
    if (n.isZero()) {
      throw Arb::Errors::DivByZero();
    }
    
    // This is a rational number class, not complex.
    if (!isPositive() && n.isEven()) {
      throw Arb::Errors::Complex();
    }
    
    // Common work area.
    Arb retval(1);
    
    // Only find integer roots.
    if (n.isInteger()) {
      
      // Work area.
      Arb lastguess;
      Arb accuracy(Arb(_base).pow(-Arb(_maxradix)));
      
      // Create an initial guess, will be within at least one ^2, so Newton's
      // algorithm should begin converging by doubling correct # of bits each
      // iteration.
      if (abs() > 1) {
        retval = 2;
        while (retval.pow(n) < *this) {
          retval *= 2;
        }
      }
      
      Arb precision = retval;
      
      // Newton's algorithm. 
      Arb nmo(n - 1);
      do {
        
        // Save the last guess for comparison and make a new one.
        lastguess = retval;
        retval    = (*this / retval.pow(nmo) + nmo * retval) / n;
        
        // Restrict to the specified precision for performance. Since we know
        // that the number of valid bits doubles each time we iterate, this
        // should be possible to do far more intelligently.
        if (retval._q > accuracy._q) {
          retval._forcereduce(accuracy._q);
        }
        
      // Loop until we are within our specified accuracy.
      } while ((retval - lastguess).abs() >= accuracy);
      
      // Always reduce.
      retval._reduce();
      
    // If non-integer, let pow() take care of making it an integer.
    } else {
      
      retval = pow(1 / n);
      
    }
    
    // Return the result.
    return retval;
    
  }
  
  /*
   * Common initialization tasks.
   */
  void Arb::_init () {
    
    // Construct this object fully. By definition of the clear() function,
    // the default constructor must do nothing more than call clear().
    clear();
    
  }
  
  /*
   * Reduce the number to its most compact representation.
   */
  Arb& Arb::_reduce () {
    
    // Fixed-point numbers are forced to their dividend.
    if (_fix) {
      _forcereduce(_fixq);
      
    // Floating-point numbers are simply reduced.
    } else {
      reduce(_p, _q);
    }
    
    // We done.
    return *this;
    
  }
  
  /*
   * Bitwise shift this number.
   */
  Arb& Arb::_shift (Arb const& bits, _Dir const dir) {
    
    // Only work if necessary.
    if (!*this || !bits) {
      return *this;
    }
    
    // Number of bits to shift must be integer.
    if (!bits.isInteger()) {
      if (dir == _DIR_L) {
        throw Arb::Errors::NonInteger();
      } else {
        throw Arb::Errors::NonInteger();
      }
    }
    
    // Shift left by left-shifting p. Shift right by left-shifting q. Shift
    // in the opposite direction if bits is negative.
    if ((dir == _DIR_L && bits > 0) || (dir == _DIR_R && bits < 0)) {
      _p << bits._p;
    } else {
      _q << bits._p;
    }
    
    // Reduce the fraction.
    _reduce();
    
    // We done.
    return *this;
    
  }
  Arb& Arb::_shift (ArbInt const& bits, _Dir const dir) {
    
    // Only work if necessary.
    if (!*this || !bits) {
      return *this;
    }
    
    // Shift left by left-shifting p. Shift right by left-shifting q.
    if (dir == _DIR_L) {
      _p << bits;
    } else {
      _q << bits;
    }
    
    // Reduce the fraction.
    _reduce();
    
    // We done.
    return *this;
    
  }
  
  /*
   * Reduce the number to a specific q.
   */
  Arb& Arb::_forcereduce (_DigsT const& q) {
    
    // Only work if we have to.
    if (_q != q) {
      
      //  p       x
      // --- == ------
      //  q      fixq
      _DigsT remainder(_p * q % _q);
      _p = _p * q / _q;
      
      // Round.
      remainder *= 2;
      if (remainder != 0) {
        switch (_round) {
          case ROUND_UP: {
            if (_positive) {
              ++_p;
            }
          } break;
          case ROUND_DOWN: {
            if (!_positive) {
              ++_p;
            }
          } break;
          case ROUND_TOWARD_ZERO: {
          } break;
          case ROUND_FROM_ZERO: {
            ++_p;
          } break;
          case ROUND_NORMAL: {
            if (remainder >= _q) {
              ++_p;
            }
          } break;
          case ROUND_DEFAULT:
          case ROUND_EVEN: {
            // If odd, round, otherwise drop remainder. This will implement
            // round-to-even.
            if (remainder > _q || (remainder == _q && _p & 1)) {
              ++_p;
            }
          } break;
        }
      }
      
      // Finally, set the new q.
      _q = q;
      
    }
    
    // We done.
    return *this;
    
  }
  
  /*
   * Normalize two numbers.
   */
  Arb& Arb::_normalize (Arb& number) {
    
    // Only work if necessary.
    if (_q != number._q) {
      
      // Raise each number's q to their LCM, bring p along.
      _DigsT qgcd(gcd(_q, number._q));
      _DigsT qlcm((_q / qgcd) * number._q);
      _p        *= number._q / qgcd;
      number._p *= _q        / qgcd;
      _q         = qlcm;
      number._q  = qlcm;
      
    }
    
    // We done, return.
    return *this;
    
  }
  
  /*
   * Set from a float.
   */
  template <> void Arb::_Set<float, Arb::_NUM_FLPT>::op (Arb& l, float const r) {
    
    // Easy common cases.
    if (r == 0.0) {
      l._p = 0;
      l._q = 1;
      l._reduce();
      return;
    }
    if (r == 1.0) {
      l._p = 1;
      l._q = 1;
      l._reduce();
      return;
    }
    if (r == -1.0) {
      l._p        =     1;
      l._q        =     1;
      l._positive = false;
      l._reduce();
      return;
    }
    
    // Load the union.
    _FloatParts converter = { r };
    
    // Only actual numbers are supported.
    if (converter.bits.exponent == (static_cast<unsigned int>(1) << _FloatInfo<float>::exponentbits) - 1) {
      
      // Infinity.
      if (converter.bits.mantissa == 0) {
        if (converter.bits.sign == 0) {
          throw Arb::Errors::PositiveInfinity();
        } else {
          throw Arb::Errors::NegativeInfinity();
        }
      }
      
      // NaN.
      throw Arb::Errors::NaN();
      
    }
    
    // Fraction is always x over 2^one bit less than mantissa.
    l._q = 1 << _FloatInfo<float>::mantissabits - 1;
    
    // Denormalized numbers.
    if (converter.bits.exponent == 0) {
      
      // Load the mantissa as a fraction. Denormalized numbers do not have a
      // hidden bit.
      l._p = converter.bits.mantissa;
      
      // Correct the exponent.
      converter.bits.exponent = 1;
      
    // Normalized numbers.
    } else {
      
      // Load the mantissa as a fraction. Add in the hidden bit.
      l._p = l._q + converter.bits.mantissa;
      
    }
    
    // Multiply by the exponent to get the actual number. Exponent has a bias
    // of 127.
    l *= Arb(1) << SafeInt<int>(converter.bits.exponent) - _FloatInfo<float>::bias;
    
    // Set the sign.
    l._positive = !converter.bits.sign;
    
    // Reduce.
    l._reduce();
    
  }
  
  /*
   * Set from a double.
   */
  template <> void Arb::_Set<double, Arb::_NUM_FLPT>::op (Arb& l, double const r) {
    
    // Easy common cases.
    if (r == 0.0) {
      l._p = 0;
      l._q = 1;
      l._reduce();
      return;
    }
    if (r == 1.0) {
      l._p = 1;
      l._q = 1;
      l._reduce();
      return;
    }
    if (r == -1.0) {
      l._p        =     1;
      l._q        =     1;
      l._positive = false;
      l._reduce();
      return;
    }
    
    // Load the union.
    _DoubleParts converter = { r };
    
    // Only actual numbers are supported.
    if (converter.bits.exponent == (static_cast<unsigned int>(1) << _FloatInfo<double>::exponentbits) - 1) {
      
      // Infinity.
      if (converter.bits.mantissah == 0 && converter.bits.mantissal == 0) {
        if (converter.bits.sign == 0) {
          throw Arb::Errors::PositiveInfinity();
        } else {
          throw Arb::Errors::NegativeInfinity();
        }
      }
      
      // NaN.
      throw Arb::Errors::NaN();
      
    }
    
    // Fraction is always x over 2^one bit less than mantissa.
    l._q = ArbInt(1) << _FloatInfo<double>::mantissabits - 1;
    
    // Denormalized numbers.
    if (converter.bits.exponent == 0) {
      
      // Load the mantissa as a fraction. Denormalized numbers do not have a
      // hidden bit.
      l._p = converter.bits.mantissal + converter.bits.mantissah * (ArbInt(1) << 32);
      
      // Correct the exponent.
      converter.bits.exponent = 1;
      
    // Normalized numbers.
    } else {
      
      // Load the mantissa as a fraction. Add in the hidden bit.
      l._p = l._q + converter.bits.mantissal + converter.bits.mantissah * (ArbInt(1) << 32);
      
    }
    
    // Multiply by the exponent to get the actual number. Exponent has a bias
    // of 1023.
    l *= Arb(1) << SafeInt<int>(converter.bits.exponent) - _FloatInfo<double>::bias;
    
    // Set the sign.
    l._positive = !converter.bits.sign;
    
    // Reduce.
    l._reduce();
    
  }
  
  /*
   * Set from a long double.
   */
  template <> void Arb::_Set<long double, Arb::_NUM_FLPT>::op (Arb& l, long double const r) {
    
    // Easy common cases.
    if (r == 0.0) {
      l._p = 0;
      l._q = 1;
      l._reduce();
      return;
    }
    if (r == 1.0) {
      l._p = 1;
      l._q = 1;
      l._reduce();
      return;
    }
    if (r == -1.0) {
      l._p        =     1;
      l._q        =     1;
      l._positive = false;
      l._reduce();
      return;
    }
    
    // Load the union.
    _LongDoubleParts converter = { r };
    
    // Only actual numbers are supported.
    if (converter.bits.exponent == (static_cast<unsigned int>(1) << _FloatInfo<long double>::exponentbits) - 1) {
      
      // Infinity.
      if (converter.bits.mantissah == 0 && converter.bits.mantissal == 0) {
        if (converter.bits.sign == 0) {
          throw Arb::Errors::PositiveInfinity();
        } else {
          throw Arb::Errors::NegativeInfinity();
        }
      }
      
      // NaN.
      throw Arb::Errors::NaN();
      
    }
    
    // Fraction is always x/2^one bit less than mantissa.
    l._q = ArbInt(1) << _FloatInfo<long double>::mantissabits - 1;
    
    // In long double, normalized and denormalized are handled basically the
    // same. Correct the exponent, but that's all we need to do because the
    // MSB is explicit.
    if (converter.bits.exponent == 0) {
      ++converter.bits.exponent;
    }
    
    // Load the mantissa as a fraction.
    l._p = converter.bits.mantissal + converter.bits.mantissah * (ArbInt(1) << 32);
    
    // Multiply by the exponent to get the actual number. Exponent has a bias
    // of 16383.
    l *= Arb(1) << (SafeInt<int>(converter.bits.exponent) - _FloatInfo<long double>::bias);
    
    // Set the sign.
    l._positive = !converter.bits.sign;
    
    // Reduce.
    l._reduce();
    
  }
  
  /*
   * Get as a float.
   */
  template <> void Arb::_Get<float, Arb::_NUM_FLPT>::op (float& l, Arb const& r) {
    
    // Work area.
    ArbInt       tmpnum;
    unsigned int exponent;
    
    // Convert to floating-point format.
    tmpnum = r._toFloat<float>(exponent);
    
    // Truncate to 23 bits or less. _toFloat should never return more than 24
    // bits, and bit 24 is discarded.
    if (tmpnum.bitsInNumber() > _FloatInfo<float>::mantissabits - 1) {
      tmpnum &= (ArbInt(1) << (_FloatInfo<float>::mantissabits - 1)) - 1;
    }
    
    // Set the number.
    _FloatParts converter;
    converter.bits.sign     = !r._positive;
    converter.bits.exponent = exponent;
    converter.bits.mantissa = tmpnum;
    l = converter.number;
    
  }
  
  /*
   * Get as a double.
   */
  template <> void Arb::_Get<double, Arb::_NUM_FLPT>::op (double& l, Arb const& r) {
    
    // Work area.
    ArbInt       tmpnum;
    unsigned int exponent;
    
    // Convert to floating-point format.
    tmpnum = r._toFloat<double>(exponent);
    
    // Truncate to 53 bits or less. _toFloat should never return more than 54
    // bits, and bit 54 is discarded.
    if (tmpnum.bitsInNumber() > _FloatInfo<double>::mantissabits - 1) {
      tmpnum &= (ArbInt(1) << (_FloatInfo<double>::mantissabits - 1)) - 1;
    }
    
    // Set the number.
    _DoubleParts converter;
    converter.bits.sign      = !r._positive;
    converter.bits.exponent  = exponent;
    converter.bits.mantissah = tmpnum >> std::numeric_limits<unsigned int>::digits;
    converter.bits.mantissal = tmpnum  & std::numeric_limits<unsigned int>::max();
    l = converter.number;
    
  }
  
  /*
   * Get as a long double.
   */
  template <> void Arb::_Get<long double, Arb::_NUM_FLPT>::op (long double& l, Arb const& r) {
    
    // Work area.
    ArbInt       tmpnum;
    unsigned int exponent;
    
    // Convert to floating-point format.
    tmpnum = r._toFloat<long double>(exponent);
    
    // Set the number. We did not truncate because j, the explicit leading bit
    // is also in the long double.
    _LongDoubleParts converter;
    converter.bits.sign      = !r._positive;
    converter.bits.exponent  = exponent;
    converter.bits.mantissah = tmpnum >> std::numeric_limits<unsigned int>::digits;
    converter.bits.mantissal = tmpnum  & std::numeric_limits<unsigned int>::max();
    l = converter.number;
    
  }
  
}

