/*****************************************************************************
 * Arb.c++
 *****************************************************************************
 * Implementation for class Arb.
 * FIXME: This COW stuff seemed like a good idea at the time, but it's
 *        just not thread safe, and making it safe would make this so slow
 *        its pointless. Fun while it lasted, get rid of it.
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
  Arb::Arb (Arb const& number, bool const copynow) {
    
    // Call common init.
    _init();
    
    // Set the number.
    if (copynow) {
      deepcopy(number);
    } else {
      copy(number);
    }
    
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
    if (base != _data->base) {
      Arb new_num(*this, true);
      new_num._data->base = base;
      if (new_num._data->fix && new_num._data->fixtype == FIX_RADIX) {
        new_num._data->fixq = _DigsT(new_num._data->base).pow(new_num._data->pointpos);
        new_num._reduce();
      }
      _data = new_num._data;
    }
    return *this;
  }
  Arb& Arb::MaxRadix (std::string::size_type const maxradix) {
    if (maxradix != _data->maxradix) {
      Arb new_num(*this, true);
      new_num._data->maxradix = maxradix;
      _data = new_num._data;
    }
    return *this;
  }
  Arb& Arb::PointPos (PointPosT const pointpos) {
    if (pointpos != _data->pointpos) {
      Arb new_num(*this, true);
      new_num._data->pointpos = pointpos;
      if (new_num._data->fix && new_num._data->fixtype == FIX_RADIX) {
        new_num._data->fixq = _DigsT(new_num._data->base).pow(new_num._data->pointpos);
        new_num._reduce();
      }
      _data = new_num._data;
    }
    return *this;
  }
  Arb& Arb::Fixed (bool const fixed) {
    if (fixed != _data->fix) {
      Arb new_num(*this, true);
      new_num._data->fix = fixed;
      if (new_num._data->fix && new_num._data->fixtype == FIX_RADIX) {
        new_num._data->fixq = _DigsT(new_num._data->base).pow(new_num._data->pointpos);
      }
      new_num._reduce();
      _data = new_num._data;
    }
    return *this;
  }
  Arb& Arb::Fix (FixType const fix) {
    if (fix != _data->fixtype) {
      Arb new_num(*this, true);
      new_num._data->fixtype = fix;
      if (new_num._data->fix) {
        new_num._reduce();
      }
      _data = new_num._data;
    }
    return *this;
  }
  Arb& Arb::FixQ (Arb const& fixq) {
    if (fixq != _data->fixq) {
      if (!fixq.isInteger()) {
        throw Arb::Errors::NonInteger();
      }
      Arb new_num(*this, true);
      new_num._data->fixq    = fixq._data->p;
      new_num._data->fixtype = FIX_DENOM;
      if (new_num._data->fix) {
        new_num._reduce();
      }
      _data = new_num._data;
    }
    return *this;
  }
  Arb& Arb::Format (OutputFormat const format) {
    if (format != FMT_DEFAULT && format != _data->format) {
      Arb new_num(*this, true);
      new_num._data->format = format;
      _data = new_num._data;
    }
    return *this;
  }
  Arb& Arb::Round (RoundMethod const round) {
    if (round != ROUND_DEFAULT && round != _data->round) {
      Arb new_num(*this, true);
      new_num._data->round = round;
      _data = new_num._data;
    }
    return *this;
  }
  
  /*
   * Reset to just-constructed state.
   */
  Arb& Arb::clear () {
    
    // Clear this number's data. Instead of calling the clear() method of
    // _Data, we are simply creating a new structure and letting the
    // ReferencePointer take care of cleaning up any no-longer referenced
    // data. This is to allow COW.
    _data = new _Data;
    
    // We done.
    return *this;
    
  }
  
  /*
   * Copy another number.
   */
  Arb& Arb::copy (Arb const& number) throw() {
    
    // Copy. Easy.
    _data = number._data;
    
    // We done.
    return *this;
    
  }
  
  /*
   * Perform a deep copy of another number, do not wait for COW.
   */
  Arb& Arb::deepcopy (Arb const& number) {
    
    // Set the new data. This is the deep copy.
    _DataPT new_data(new _Data(*(number._data)));
    
    // Now do non-throwing operations.
    _data = new_data;
    
    // We done.
    return *this;
    
  }
  
  /*
   * Convert this number to a string.
   */
  string& Arb::to_string (string& buffer, OutputFormat const format) const {
    
    // This is the string we will be returning.
    string retval;
    
    // Determine the output format.
    OutputFormat fmt = (format == FMT_DEFAULT) ? _data->format : format;
    
    // Choose the output format.
    switch (fmt) {
      
      // Output both notations.
      case FMT_BOTH: {
        retval += to_string(FMT_RADIX   ) + " " = to_string(FMT_FRACTION)      ;
      } break;
      
      // Output in radix notation.
      case FMT_DEFAULT:
      case FMT_RADIX  : {
        
        // Work area.
        _DigsT numeric;
        numeric.Base(_data->base);
        
        // Get the whole number part. Use set() to avoid overwriting the base
        // of numeric.
        numeric.set(_data->p / _data->q);
        
        // This is the number of digits from the end of the string to put
        // the radix point.
        std::string::size_type radixpos = 0;
        
        // Create the radix part.
        _DigsT remainder = _data->p % _data->q;
        if ((_data->fix && _data->fixq != 1) || (!_data->fix && remainder)) {
          
          // Get the radix digits, one by one. Output digits until the entire
          // fraction is output or the maximum requested significant radix digits
          // are output.
          std::string::size_type sigdigs  = 0;
          bool                   sigstart = (numeric != 0);
          _DigsT                 digit;
          while ((sigdigs < _data->maxradix) && (remainder != 0)) {
            
            // Push one base digit to a whole number.
            remainder *= _data->base;
            digit      = remainder / _data->q;
            
            // Push that digit onto the end of the number we are constructing
            // and increment the number of radix positions.
            numeric.push_back(digit);
            ++radixpos;
            
            // Remove that digit from the remainder.
            remainder %= _data->q;
            
            // Start incrementing significant digits when we actually hit one,
            // do not count 0s.
            if (sigstart || (digit != 0)) {
              ++sigdigs;
              sigstart = true;
            }
            
          }
          
          // Round.
          if (remainder != 0) {
            switch (_data->round) {
              case ROUND_UP: {
                if (_data->positive) {
                  ++numeric;
                }
              } break;
              case ROUND_DOWN: {
                if (!_data->positive) {
                  ++numeric;
                }
              } break;
              case ROUND_TOWARD_ZERO: {
              } break;
              case ROUND_FROM_ZERO: {
                ++numeric;
              } break;
              case ROUND_NORMAL: {
                if (remainder * 2 >= _data->q) {
                  ++numeric;
                }
              } break;
              case ROUND_DEFAULT:
              case ROUND_EVEN: {
                // Double remainder to make it easy to check for half over,
                // but when checking for odd, make sure to op& with 2 since we
                // just left-shifted by 1.
                remainder *= 2;
                if (remainder > _data->q || (remainder == _data->q && remainder & 2)) {
                  ++numeric;
                }
              } break;
            }
          }
          
          // If this is a fixed-radix number, fix the radix.
          if (_data->fix) {
          
            // Pad with zeros.
            if (radixpos < _data->pointpos) {
              numeric  *= _DigsT(_data->base).pow(_data->pointpos - radixpos);
              radixpos  = _data->pointpos;
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
        if (!_data->fix && radixpos > 0) {
          
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
        retval = _data->p.Base(_data->base).to_string() + "/" + _data->q.Base(_data->base).to_string();
      } break;
      
    }
        
    // Output the sign if negative, unless outputting both formats.
    if ((fmt != FMT_BOTH) && !(_data->positive) && (_data->p != 0)) {
      retval.insert(0, 1, '-');
    }
    
    // We done.
    buffer.swap(retval);
    return buffer;
    
  }
  
  /*
   * Set the number from a string.
   */
  Arb& Arb::set (string const& number, bool const autobase) {
    
    // Load the number into this for exception safety.
    Arb new_num;
    
    // Hold the number in case an error needs to be throw.
    ConstReferencePointer<string> tmp_number(new std::string(number));
    
    // Parser will load data into here.
    string            num                     ;
    string            rad                     ;
    string            exp                     ;
    bool              p_num      = true       ;
    bool              p_exp      = true       ;
    string::size_type nexp       =           0;
    string::size_type numstart   =           0;
    string::size_type radstart   =           0;
    string::size_type expstart   =           0;
    string::size_type numlen     =           0;
    string::size_type radlen     =           0;
    string::size_type explen     =           0;
    bool              numstarted = false      ;
    bool              radstarted = false      ;
    bool              expstarted = false      ;
    BaseT             num_base   = _data->base;
    
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
      new_num._data->p.Base(num_base).set(num, false);
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
      
      // Carry fixed-point status from last number.
      new_num._data->pointpos = _data->pointpos;
      new_num._data->fix      = _data->fix;
      new_num._data->fixtype  = _data->fixtype;
      new_num._data->fixq     = _data->fixq;
      
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
        new_num._data->q = _DigsT(num_base).pow(expn);
        if (p_exp) {
          new_num._data->p *= new_num._data->q;
          new_num._data->q  = 1;
          expn              = 0;
        } 
      }
      
      // If a fixed-point number is requested, increase or decrease the
      // radix digits as needed.
      if (new_num._data->fix) {
        
        // A number that is fixed by the number of radix digits needs to
        // figure q, others will be told what q is.
        if (new_num._data->fixtype == FIX_RADIX) {
          
          // If the exponent is positive, it is a simple case of increasing the
          // number of radix digits, this is a number represented by 1s by now.
          if (p_exp) {
            new_num._data->q  = _DigsT(_data->base).pow(_DigsT(new_num._data->pointpos));
            new_num._data->p *= new_num._data->q;
          
          // If the exponent is negative, we must add or subtract the difference
          // in digits.
          } else {
            if (_DigsT(new_num._data->pointpos) >= expn) {
              _DigsT mod        = _DigsT(_data->base).pow(_DigsT(new_num._data->pointpos) - expn);
              new_num._data->p *= mod;
              new_num._data->q *= mod;
            } else {
              _DigsT mod        = _DigsT(_data->base).pow(expn - _DigsT(new_num._data->pointpos));
              new_num._data->p /= mod;
              new_num._data->q /= mod;
            }
          }
          
          // This is the q that we always need to fix at.
          new_num._data->fixq = new_num._data->q;
          
        }
        
      }
      
    }
    
    // Set the sign.
    new_num._data->positive = p_num;
    
    // Carry over the old base.
    new_num._data->base = _data->base;
    
    // Reduce the number.
    new_num._reduce();
    
    // The number has been loaded, swap it in. COW is preserved.
    _data = new_num._data;
    return *this;
    
  }
  
  /*
   * Set the number from another Arb.
   */
  Arb& Arb::set (Arb const& number) {
    
    // Work area.
    Arb new_num(*this, true);
    
    // Set the new number.
    new_num._data->p        = number._data->p;
    new_num._data->q        = number._data->q;
    new_num._data->positive = number._data->positive;
    
    // Reduce the fraction.
    new_num._reduce();
    
    // Move the result into place and return.
    _data = new_num._data;
    return *this;
    
  }
  
  /*
   * Set from an ArbInt.
   */
  Arb& Arb::set (ArbInt const& number) {
    
    // Work area.
    Arb new_num(*this, true);
    
    // Set the number.
    new_num._data->p        = number;
    new_num._data->q        = 1;
    new_num._data->positive = true;
    
    // Reduce the fraction.
    new_num._reduce();
    
    // Move the result into place and return.
    _data = new_num._data;
    return *this;
    
  }
  
  /*
   * Multiplication operator backend.
   */
  Arb& Arb::op_mul (Arb const& number) {
    
    // Work area.
    Arb retval(*this, true);
    
    // Multiply the numbers.
    retval._data->p *= number._data->p;
    retval._data->q *= number._data->q;
    
    // Set the sign.
    retval._data->positive = (retval._data->positive == number._data->positive);
    
    // Reduce the fraction.
    retval._reduce();
    
    // Move the result into place and return.
    _data = retval._data;
    return *this;
    
  }
  Arb& Arb::op_mul (ArbInt const& number) {
    
    // Work area.
    Arb retval(*this, true);
    
    // Multiply.
    retval._data->p *= number;
    
    // Reduce the fraction.
    retval._reduce();
    
    // Move the result into place and return.
    _data = retval._data;
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
    
    // Work area
    Arb retval(*this, true);
    
    // Divide the numbers, multiply by the inverse.
    retval._data->p *= number._data->q;
    retval._data->q *= number._data->p;
    
    // Set the sign.
    retval._data->positive = (retval._data->positive == number._data->positive);
    
    // Reduce the fraction.
    retval._reduce();
    
    // Move the result into place and return.
    _data = retval._data;
    return *this;
    
  }
  Arb& Arb::op_div (ArbInt const& number) {
    
    // Throw an error on divide by zero.
    if (number == 0) {
      throw Arb::Errors::DivByZero();
    }
    
    // Work area.
    Arb retval(*this, true);
    
    // Divide the numbers, multiply by the inverse.
    retval._data->q *= number;
    
    // Reduce the fraction.
    retval._reduce();
    
    // Move the result into place and return.
    _data = retval._data;
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
    
    // Work area.
    Arb retval(*this, true);
    
    // Simple, just modulo divide p's.
    retval._data->p %= number._data->p;
    
    // We done, return.
    _data = retval._data;
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
    
    // Work area.
    Arb retval(*this, true);
    
    // Modulo divide p.
    retval._data->p %= number;
    
    // We done, return.
    _data = retval._data;
    return *this;
    
  }
  
  /*
   * Addition operator backend.
   */
  Arb& Arb::op_add (Arb const& number) {
    
    // Work area.
    Arb tmp_l(*this,  true);
    Arb tmp_r(number, true);
    
    // Numbers must have the same denominator to add.
    tmp_l._normalize(tmp_r);
    
    // If signs are same add, otherwise subtract.
    if (tmp_l < 0 == tmp_r < 0) {
      tmp_l._data->p += tmp_r._data->p;
    } else {
      if (tmp_r._data->p > tmp_l._data->p) {
        tmp_l._data->p        = tmp_r._data->p - tmp_l._data->p;
        tmp_l._data->positive = !tmp_l._data->positive;
      } else {
        tmp_l._data->p -= tmp_r._data->p;
      }
    }
    
    // Reduce the fraction.
    tmp_l._reduce();
    
    // We done. Move the result into place and return.
    _data = tmp_l._data;
    return *this;
    
  }
  Arb& Arb::op_add (ArbInt const& number) {
    
    // Work area.
    Arb    retval(*this, true);
    ArbInt tmp(number, true);
    
    // Normalize.
    tmp *= retval._data->q;
    
    // If signs are same add, otherwise subtract.
    if (retval < 0) {
      if (tmp > retval._data->p) {
        retval._data->p        = tmp - retval._data->p;
        retval._data->positive = !retval._data->positive;
      } else {
        retval._data->p -= tmp;
      }
    } else {
      retval._data->p += tmp;
    }
    
    // Reduce the fraction.
    retval._reduce();
    
    // We done. Move the result into place and return.
    _data = retval._data;
    return *this;
    
  }
  
  /*
   * Subtraction operator backend.
   */
  Arb& Arb::op_sub (Arb const& number) {
    
    // Work area.
    Arb tmp_l(*this,  true);
    Arb tmp_r(number, true);
    
    // Numbers must have the same denominator to subtract.
    tmp_l._normalize(tmp_r);
    
    // If signs are same subtract, otherwise add.
    if (tmp_l < 0 == tmp_r < 0) {
      if (tmp_r._data->p > tmp_l._data->p) {
        tmp_l._data->p        = tmp_r._data->p - tmp_l._data->p;
        tmp_l._data->positive = !tmp_l._data->positive;
      } else {
        tmp_l._data->p -= tmp_r._data->p;
      }
    } else {
      tmp_l._data->p += tmp_r._data->p;
    }
    
    // Reduce the fraction.
    tmp_l._reduce();
    
    // We done. Move the result into place and return.
    _data = tmp_l._data;
    return *this;
    
  }
  Arb& Arb::op_sub (ArbInt const& number) {
    
    // Work area.
    Arb    retval(*this, true);
    ArbInt tmp(number, true);
    
    // Normalize.
    tmp *= retval._data->q;
    
    // If signs are same subtract, otherwise add.
    if (retval < 0) {
      retval._data->p += tmp;
    } else {
      if (tmp > retval._data->p) {
        retval._data->p        = tmp - retval._data->p;
        retval._data->positive = !retval._data->positive;
      } else {
        retval._data->p -= tmp;
      }
    }
    
    // Reduce the fraction.
    retval._reduce();
    
    // We done. Move the result into place and return.
    _data = retval._data;
    return *this;
    
  }
  
  /*
   * Greater-than operator backend.
   */
  bool Arb::op_gt (Arb const& number) const {
    
    // If one or both numbers are zero, compare is easy.
    if (_data->p.isZero()) {
      if (number._data->p.isZero()) {
        return false;
      } else {
        return !number._data->positive;
      }
    } else if (number._data->p.isZero()) {
      return _data->positive;
    }
    
    // Neither number is zero. If signs are different, they are sufficient
    // for this test.
    if (_data->positive && !number._data->positive) {
      return true;
    } else if (!_data->positive && number._data->positive) {
      return false;
    }
    
    // Signs are the same, we will have to compare numbers. Copy numbers since
    // we will be modifying them.
    Arb tmp_l(*this,  true);
    Arb tmp_r(number, true);
    
    // Numbers must have the same denomiator to compare.
    tmp_l._normalize(tmp_r);
    
    // If numbers are negative, comparison is reversed.
    if (_data->positive) {
      return (tmp_l._data->p > tmp_r._data->p);
    } else {
      return (tmp_l._data->p < tmp_r._data->p);
    }
    
  }
  bool Arb::op_gt (ArbInt const& number) const {
    
    // If one or both numbers are zero, compare is easy.
    if (_data->p.isZero()) {
      return false;
    } else if (number == 0) {
      return _data->positive;
    }
    
    // Neither number is zero. If signs are different, they are sufficient
    // for this test.
    if (!_data->positive) {
      return false;
    }
    
    // Signs are the same, we will have to compare numbers. Copy numbers since
    // we will be modifying them.
    ArbInt tmp(number, true);
    
    // Normalize.
    tmp *= _data->q;
    
    // Compare.
    return _data->p > tmp;
    
  }
  
  /*
   * Less-than operator backend.
   */
  bool Arb::op_lt (Arb const& number) const {
    
    // If one or both numbers are zero, compare is easy.
    if (_data->p.isZero()) {
      if (number._data->p.isZero()) {
        return false;
      } else {
        return number._data->positive;
      }
    } else if (number._data->p.isZero()) {
      return !_data->positive;
    }
    
    // Neither number is zero. If signs are different, they are sufficient for
    // this test.
    if (_data->positive && !number._data->positive) {
      return false;
    } else if (!_data->positive && number._data->positive) {
      return true;
    }
    
    // Signs are the same, we will have to compare numbers.
    Arb tmp_l(*this,  true);
    Arb tmp_r(number, true);
    
    // Numbers must have the same denominator to compare.
    tmp_l._normalize(tmp_r);
    
    // If numbers are negative, comparison is reversed.
    if (_data->positive) {
      return (tmp_l._data->p < tmp_r._data->p);
    } else {
      return (tmp_l._data->p > tmp_r._data->p);
    }
    
  }
  bool Arb::op_lt (ArbInt const& number) const {
    
    // If one or both numbers are zero, compare is easy.
    if (_data->p.isZero()) {
      return number > 0;
    } else if (number == 0) {
      return !_data->positive;
    }
    
    // Neither number is zero. If signs are different, they are sufficient
    // for this test.
    if (!_data->positive) {
      return true;
    }
    
    // Signs are the same, we will have to compare numbers. Copy numbers since
    // we will be modifying them.
    ArbInt tmp(number, true);
    
    // Normalize.
    tmp *= _data->q;
    
    // Compare.
    return _data->p < tmp;
    
  }
  
  /*
   * Equal-to operator backend.
   */
  bool Arb::op_eq (Arb const& number) const {
    
    // Check for 0.
    if (_data->p.isZero()) {
      return number._data->p.isZero();
    } else if (number._data->p.isZero()) {
      return false;
    }
    
    // Neither number is zero. Check signs.
    if (_data->positive != number._data->positive) {
      return false;
    }
    
    // Check the numbers themselves. No need to normalize, if they're not,
    // they're not equal.
    return ((_data->q == number._data->q) && (_data->p == number._data->p));
    
  }
  bool Arb::op_eq (ArbInt const& number) const {
    
    // Check for 0.
    if (_data->p.isZero()) {
      return number == 0;
    } else if (number == 0) {
      return false;
    }
    
    // Neither number is zero. Check signs.
    if (!_data->positive) {
      return false;
    }
    
    // Check the numbers themselves. No need to normalize, if they're not,
    // they're not equal.
    return (isInteger() && (_data->p == number));
    
  }
  
  /*
   * Get the ceiling of this fractional number.
   */
  Arb Arb::ceil () const {
    
    // Work area.
    Arb retval(*this, true);
    
    // Only work if we have to.
    if (!isInteger()) {
      
      // Easy, p/q.
      _DigsT remainder  = retval._data->p % retval._data->q;
      retval._data->p  /= retval._data->q;
      retval._data->q   = 1;
      
      // If the number was positive and there was a remainder, we need to add
      // 1.
      if (isPositive() && remainder) {
        ++retval._data->p;
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
    Arb retval(*this, true);
    
    // Only work if we have to.
    if (!isInteger()) {
      
      // Easy, p/q.
      _DigsT remainder  = retval._data->p % retval._data->q;
      retval._data->p  /= retval._data->q;
      retval._data->q   = 1;
      
      // If the number was negative and there was a remainder, we need to
      // subtract 1. Since the number is negative, adding is subtracting.
      if (!isPositive() && (remainder != 0)) {
        ++retval._data->p;
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
    Arb retval(*this, true);
    
    // Only work if we have to.
    if (!isInteger()) {
      
      // Easy, p/q.
      retval._data->p /= retval._data->q;
      retval._data->q  = 1;
      
    }
    
    // We done.
    return retval;
    
  }
  
  /*
   * Raise this number to a power.
   */
  Arb Arb::pow (Arb const& exp) const {
    
    // Work area.
    Arb retval(*this, true);
    
    // No work if this is zero or if we are raising to the 1.
    if (!isZero() || (exp == 1)) {
      
      // Only raise if we have to.
      if (exp != 0) {
        
        // Raise this number to an integer power.
        if (exp.isInteger()) {
          
          // No need to raise 1 or to 1.
          if ((retval.abs() != 1) && (exp.abs() != 1)) {
            
            // Raise p & q.
            retval._data->p = retval._data->p.pow(exp._data->p);
            retval._data->q = retval._data->q.pow(exp._data->p);
            
            // Reduce
            retval._reduce();
            
          }
          
          // The number is positive if it began positive or the exponent is even.
          retval._data->positive = retval._data->positive || exp.isEven();
        
        // Raising to a fraction.
        } else {
          
          // x^(y/z): find the raise to the yth power and find the zth root.
          Arb x;
          Arb y;
          Arb z;
          
          // Raise to the yth power.
          y._data->p = exp._data->p;
          x = pow(y);
          
          // Get the zth root.
          z._data->p = exp._data->q;
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
      Arb accuracy(Arb(_data->base).pow(-Arb(_data->maxradix)));
      
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
        if (retval._data->q > accuracy._data->q) {
          retval._forcereduce(accuracy._data->q);
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
    if (_data->fix) {
      _forcereduce(_data->fixq);
      
    // Floating-point numbers are simply reduced.
    } else {
      reduce(_data->p, _data->q);
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
      _data->p << bits._data->p;
    } else {
      _data->q << bits._data->p;
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
      _data->p << bits;
    } else {
      _data->q << bits;
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
    if (_data->q != q) {
      
      //  p       x
      // --- == ------
      //  q      fixq
      _DigsT remainder = _data->p * q % _data->q;
      _data->p         = _data->p * q / _data->q;
      
      // Round.
      remainder *= 2;
      if (remainder != 0) {
        switch (_data->round) {
          case ROUND_UP: {
            if (_data->positive) {
              ++_data->p;
            }
          } break;
          case ROUND_DOWN: {
            if (!_data->positive) {
              ++_data->p;
            }
          } break;
          case ROUND_TOWARD_ZERO: {
          } break;
          case ROUND_FROM_ZERO: {
            ++_data->p;
          } break;
          case ROUND_NORMAL: {
            if (remainder >= _data->q) {
              ++_data->p;
            }
          } break;
          case ROUND_DEFAULT:
          case ROUND_EVEN: {
            // If odd, round, otherwise drop remainder. This will implement
            // round-to-even.
            if (remainder > _data->q || (remainder == _data->q && _data->p & 1)) {
              ++_data->p;
            }
          } break;
        }
      }
      
      // Finally, set the new q.
      _data->q = q;
      
    }
    
    // We done.
    return *this;
    
  }
  
  /*
   * Normalize two numbers.
   */
  Arb& Arb::_normalize (Arb& number) {
    
    // Only work if necessary.
    if (_data->q != number._data->q) {
      
      // Raise each number's q to their LCM, bring p along.
      _DigsT qgcd(gcd(_data->q, number._data->q));
      _DigsT qlcm((_data->q / qgcd) * number._data->q);
      _data->p        *= number._data->q / qgcd;
      number._data->p *= _data->q        / qgcd;
      _data->q         = qlcm;
      number._data->q  = qlcm;
      
    }
    
    // We done, return.
    return *this;
    
  }
  
  /*
   * Set from a float.
   */
  template <> void Arb::_Set<float, Arb::_NUM_FLPT>::op (Arb& l, float const r) {
    
    // Work area.
    Arb new_num;
    
    // Carry over the old fixed-point properties.
    new_num._data->fix      = l._data->fix;
    new_num._data->pointpos = l._data->pointpos;
    new_num._data->base     = l._data->base;
    new_num._data->fixq     = l._data->fixq;
    
    // Easy common cases.
    if (r == 0.0) {
      l._data = new_num._data;
      return;
    }
    if (r == 1.0) {
      new_num._data->p = 1;
      l._data = new_num._data;
      return;
    }
    if (r == -1.0) {
      new_num._data->p        = 1;
      new_num._data->positive = false;
      l._data = new_num._data;
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
    new_num._data->q = 1 << _FloatInfo<float>::mantissabits - 1;
    
    // Denormalized numbers.
    if (converter.bits.exponent == 0) {
      
      // Load the mantissa as a fraction. Denormalized numbers do not have a
      // hidden bit.
      new_num._data->p = converter.bits.mantissa;
      
      // Correct the exponent.
      converter.bits.exponent = 1;
      
    // Normalized numbers.
    } else {
      
      // Load the mantissa as a fraction. Add in the hidden bit.
      new_num._data->p = new_num._data->q + converter.bits.mantissa;
      
    }
    
    // Multiply by the exponent to get the actual number. Exponent has a bias
    // of 127.
    new_num *= Arb(1) << SafeInt<int>(converter.bits.exponent) - _FloatInfo<float>::bias;
    
    // Set the sign.
    new_num._data->positive = !converter.bits.sign;
    
    // Move in the result and return.
    l._data = new_num._data;
    
  }
  
  /*
   * Set from a double.
   */
  template <> void Arb::_Set<double, Arb::_NUM_FLPT>::op (Arb& l, double const r) {
    
    // Work area.
    Arb new_num;
    
    // Carry over the old fixed-point properties.
    new_num._data->fix      = l._data->fix;
    new_num._data->pointpos = l._data->pointpos;
    new_num._data->base     = l._data->base;
    new_num._data->fixq     = l._data->fixq;
    
    // Easy common cases.
    if (r == 0.0) {
      l._data = new_num._data;
      return;
    }
    if (r == 1.0) {
      new_num._data->p = 1;
      l._data = new_num._data;
      return;
    }
    if (r == -1.0) {
      new_num._data->p        = 1;
      new_num._data->positive = false;
      l._data = new_num._data;
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
    new_num._data->q = ArbInt(1) << _FloatInfo<double>::mantissabits - 1;
    
    // Denormalized numbers.
    if (converter.bits.exponent == 0) {
      
      // Load the mantissa as a fraction. Denormalized numbers do not have a
      // hidden bit.
      new_num._data->p = converter.bits.mantissal + converter.bits.mantissah * (ArbInt(1) << 32);
      
      // Correct the exponent.
      converter.bits.exponent = 1;
      
    // Normalized numbers.
    } else {
      
      // Load the mantissa as a fraction. Add in the hidden bit.
      new_num._data->p = new_num._data->q + converter.bits.mantissal + converter.bits.mantissah * (ArbInt(1) << 32);
      
    }
    
    // Multiply by the exponent to get the actual number. Exponent has a bias
    // of 1023.
    new_num *= Arb(1) << SafeInt<int>(converter.bits.exponent) - _FloatInfo<double>::bias;
    
    // Set the sign.
    new_num._data->positive = !converter.bits.sign;
    
    // Move in the result and return.
    l._data = new_num._data;
    
  }
  
  /*
   * Set from a long double.
   */
  template <> void Arb::_Set<long double, Arb::_NUM_FLPT>::op (Arb& l, long double const r) {
    
    // Work area.
    Arb new_num;
    
    // Carry over the old fixed-point properties.
    new_num._data->fix      = l._data->fix;
    new_num._data->pointpos = l._data->pointpos;
    new_num._data->base     = l._data->base;
    new_num._data->fixq     = l._data->fixq;
    
    // Easy common cases.
    if (r == 0.0) {
      l._data = new_num._data;
      return;
    }
    if (r == 1.0) {
      new_num._data->p = 1;
      l._data = new_num._data;
      return;
    }
    if (r == -1.0) {
      new_num._data->p        = 1;
      new_num._data->positive = false;
      l._data = new_num._data;
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
    new_num._data->q = ArbInt(1) << _FloatInfo<long double>::mantissabits - 1;
    
    // In long double, normalized and denormalized are handled basically the
    // same. Correct the exponent, but that's all we need to do because the
    // MSB is explicit.
    if (converter.bits.exponent == 0) {
      ++converter.bits.exponent;
    }
    
    // Load the mantissa as a fraction.
    new_num._data->p = converter.bits.mantissal + converter.bits.mantissah * (ArbInt(1) << 32);
    
    // Multiply by the exponent to get the actual number. Exponent has a bias
    // of 16383.
    new_num *= Arb(1) << (SafeInt<int>(converter.bits.exponent) - _FloatInfo<long double>::bias);
    
    // Set the sign.
    new_num._data->positive = !converter.bits.sign;
    
    // Move in the result and return.
    l._data = new_num._data;
    
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
    converter.bits.sign     = !r._data->positive;
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
    converter.bits.sign      = !r._data->positive;
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
    converter.bits.sign      = !r._data->positive;
    converter.bits.exponent  = exponent;
    converter.bits.mantissah = tmpnum >> std::numeric_limits<unsigned int>::digits;
    converter.bits.mantissal = tmpnum  & std::numeric_limits<unsigned int>::max();
    l = converter.number;
    
  }
  
  /***************************************************************************
   * Class Arb::_Data.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  /*
   * Default constructor.
   */
  Arb::_Data::_Data () {
    
    // Call common init.
    _init();

  }
  
  /*
   * Copy constructor.
   */
  Arb::_Data::_Data (_Data const& data) {
    
    // Call common init.
    _init();
    
    // Copy the given data.
    copy(data);
    
  }
  
  /*
   * Reset to just constructed state.
   */
  Arb::_Data& Arb::_Data::clear () {
    
    // These operations may throw, do them first so any error will be thrown
    // before any changes are made.
    _DigsT new_p;
    _DigsT new_q;
    _DigsT new_fixq;
    new_q    = 1;
    new_fixq = 1;
    
    // These operations will never throw.
    p    = new_p;
    q    = new_q;
    fixq = new_fixq;
    
    positive = true;
    pointpos = 0;
    fix      = false;
    fixtype  = FIX_RADIX;
    base     = 10;
    
    maxradix = 10;
    format   = FMT_RADIX;
    round    = ROUND_EVEN;
    
    // We done.
    return *this;
    
  }
  
  /*
   * Copy a given _Data object.
   */
  Arb::_Data& Arb::_Data::copy (_Data const& data) {
    
    // Copy the given data.
    p    = data.p;
    q    = data.q;
    fixq = data.fixq;
    
    positive = data.positive;
    pointpos = data.pointpos;
    fix      = data.fix;
    fixtype  = data.fixtype;
    base     = data.base;
    
    // We done.
    return *this;
    
  }
  
  /*
   * Common initialization tasks.
   */
  void Arb::_Data::_init () {
    
    // Construct this object fully. By definition of the clear() function, the
    // default constructor must do nothing more than call clear().
    clear();

  }
  
}
