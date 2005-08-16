/*****************************************************************************
 * Arb.cxx
 *****************************************************************************
 * Implementation for class Arb.
 *****************************************************************************/

// STL includes.
  #include <string>
  #include <algorithm>

// Internal includes.
  #include "rppower.hxx"
  #include "toString.hxx"
  #include "reduce.hxx"

// Class include.
  #include "Arb.hxx"

// Namespaces used.
using namespace std;

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * Class Arb.
   ***************************************************************************/
  
  /***************************************************************************/
  // Constants.
  
  Arb::_FloatInfo const Arb::s_floatinfo      = { 24,  8,   127 };
  Arb::_FloatInfo const Arb::s_doubleinfo     = { 53, 11,  1023 };
  Arb::_FloatInfo const Arb::s_longdoubleinfo = { 64, 15, 16383 };
  
  /***************************************************************************/
  // Function members.
  
  // Default constructor.
  Arb::Arb () {
    
    // Call common init.
    _init();
    
  }
  
  // Copy constructor.
  Arb::Arb (Arb const& number, bool const copynow) {
    
    // Call common init.
    _init();
    
    // Set the number.
    copy(number, copynow);
    
  }
  
  // Conversion constructor.
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
  
  // Accessors.
  Arb& Arb::Base (BaseT const base) {
    if (base != _data->base) {
      _data = new _Data(*_data);
      _data->base = base;
      if (_data->fix) {
        _data->fixq = _DigsT(_data->base).pow(_data->pointpos);
        _reduce();
      }
    }
    return *this;
  }
  Arb& Arb::PointPos (PointPosT const pointpos) {
    if (pointpos != _data->pointpos) {
      _data = new _Data(*_data);
      _data->pointpos = pointpos;
      if (_data->fix) {
        _data->fixq = _DigsT(_data->base).pow(_data->pointpos);
        _reduce();
      }
    }
    return *this;
  }
  Arb& Arb::Fixed (bool const fixed) {
    if (fixed != _data->fix) {
      _data = new _Data(*_data);
      _data->fix = fixed;
      if (_data->fix) {
        _data->fixq = _DigsT(_data->base).pow(_data->pointpos);
      }
      _reduce();
    }
    return *this;
  }
  
  // Reset to just-constructed state.
  Arb& Arb::clear () {
    
    // Clear this number's data. Instead of calling the clear() method of
    // _Data, we are simply creating a new structure and letting the
    // ReferencePointer take care of cleaning up any no-longer referenced
    // data. This is to allow COW.
    _data = new _Data;
    
    // These instructions cannot throw, so they come last.
    _maxradix = 10;
    _format   = FMT_RADIX;
    _round    = ROUND_NORMAL;
    
    // We done.
    return *this;
    
  }
  
  // Copy another number.
  Arb& Arb::copy (Arb const& number, bool const copynow) {
    
    // Set the new data. Copy if instructed to do so, otherwise just
    // make a reference and wait for COW.
    _DataPT new_data;
    if (copynow) {
      new_data = new _Data(*(number._data));
    } else {
      new_data = number._data;
    }
    
    // Now do non-throwing operations.
    _maxradix = number._maxradix;
    _format   = number._format;
    _round    = number._round;
    _data     = new_data;
    
    // We done.
    return *this;
    
  }
  
  // Convert this number to a string.
  string Arb::toString (OutputFormat const format) const {
    
    // This is the string we will be returning.
    string retval;
    
    // Determine the output format.
    OutputFormat fmt = (format == FMT_DEFAULT) ? _format : format;
    
    // Choose the output format.
    switch (fmt) {
      
      // Output both notations.
      case FMT_BOTH: {
        
        retval += toString(FMT_RADIX) + " " + toString(FMT_FRACTION);
        
      } break;
      
      // Output in radix notation.
      case FMT_DEFAULT:
      case FMT_RADIX: {
        
        // Work area.
        _DigsT numeric;
        numeric.Base(_data->base);
        
        // Get the whole number part.
        numeric = _data->p / _data->q;
        
        // This is the number of digits from the end of the string to put
        // the radix point.
        std::string::size_type radixpos = 0;
        
        // Create the radix part.
        _DigsT remainder = _data->p % _data->q;
        if (((_data->fixq != 0) && (_data->pointpos > 0)) || ((_data->fixq == 0) && (remainder != 0))) {
          
          // Get the radix digits, one by one. Output digits until the entire
          // fraction is output or the maximum requested significant radix digits
          // are output.
          std::string::size_type sigdigs  = 0;
          bool                   sigstart = (numeric != 0);
          _DigsT                 digit;
          digit.Base(_data->base);
          while ((sigdigs < _maxradix) && (remainder != 0)) {
            remainder *= _data->base;
            digit      = remainder / _data->q;
            numeric.push_back(digit);
            ++radixpos;
            remainder %= _data->q;
            if (sigstart || (digit != 0)) {
              ++sigdigs;
              sigstart = true;
            }
          }
          
          // Round.
          if (remainder != 0) {
            switch (_round) {
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
              case ROUND_DEFAULT:
              case ROUND_NORMAL: {
                if (remainder * 2 >= _data->q) {
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
        
        // Convert the number to a string.
        retval += numeric.toString();
        
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
        
        // Easy, output p/q.
        retval += _data->p.Base(_data->base).toString() + "/" + _data->q.Base(_data->base).toString();
        
      } break;
      
    }
        
    // Output the sign if negative, unless outputting both formats.
    if ((fmt != FMT_BOTH) && !(_data->positive) && (_data->p != 0)) {
      retval.insert(0, 1, '-');
    }
    
    // We done.
    return retval;
    
  }
  
  // Set the number from a string.
  Arb& Arb::set (string const& number) {
    
    // Load the number into this for exception safety.
    Arb new_num;
    
    // Hold the number in case an error needs to be throw.
    ConstReferencePointer<string> tmp_number(new std::string(number));
    
    // Parser will load data into here.
    string            num;
    string            rad;
    string            exp;
    bool              p_num = true;
    bool              p_exp = true;
    string::size_type nexp  = 0;
    
    // Parse the number, scoped for temp variables.
    {
      
      // Data for initial pass.
      enum Mode { NUM, RAD, EXP } mode = NUM;
      string::size_type length   = number.length();
      bool              s_num    = false;
      bool              s_exp    = false;
      bool              diggiven = false;
      
      for (string::size_type i = 0; i != length; ++i) {
        
        switch (number[i]) {
          
          // Decimal point.
          case '.':
            switch (mode) {
              case NUM: mode = RAD;                                                                                                 break;
              case RAD: throw ArbErrors::BadFormat().Problem("Radix point given for a second time").Position(i).Number(tmp_number); break;
              case EXP: throw ArbErrors::BadFormat().Problem("Radix point given in exponent").Position(i).Number(tmp_number);       break;
            }
          break;
          
          // Exponent symbol.
          case 'e':
          case 'E':
            switch (mode) {
              case NUM: mode = EXP;                                                                                                     break;
              case RAD: mode = EXP;                                                                                                     break;
              case EXP: throw ArbErrors::BadFormat().Problem("Exponent symbol given for a second time").Position(i).Number(tmp_number); break;
            }
            diggiven = false;
          break;
          
          // Sign symbol.
          case '+':
          case '-':
            if (diggiven) {
              throw ArbErrors::BadFormat().Problem("Sign given after digits").Position(i).Number(tmp_number);
            }
            switch (mode) {
              case NUM:
                if (s_num) {
                  throw ArbErrors::BadFormat().Problem("Sign of number given for a second time").Position(i).Number(tmp_number);
                }
                p_num = (number[i] == '+');
                s_num = true;
              break;
              case RAD:
                throw ArbErrors::BadFormat().Problem("Sign given after radix point").Position(i).Number(tmp_number);
              break;
              case EXP:
                if (s_exp) {
                  throw ArbErrors::BadFormat().Problem("Sign of exponent given for a second time").Position(i).Number(tmp_number);
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
              case NUM: num += number[i]; break;
              case RAD: rad += number[i]; break;
              case EXP: exp += number[i]; break;
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
    new_num._data->p.Base(_data->base) = num;
    
    // Adjust the number based on the exponent. Negative exponent increases
    // radix digits, positive exponent ups order of magnitude.
    {
      
      // Carry fixed-point status from last number.
      new_num._data->pointpos = _data->pointpos;
      new_num._data->fix      = _data->fix;
      
      // Determine the exponent given.
      _DigsT expn(exp);
      _DigsT expr(nexp);
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
        new_num._data->q = _DigsT(_data->base).pow(expn);
        if (p_exp) {
          new_num._data->p *= new_num._data->q;
          new_num._data->q  = 1;
          expn              = 0;
        } 
      }
      
      // If a fixed-point number is requested, increase or decrease the
      // radix digits as needed.
      if (new_num._data->fix) {
        
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
  
  // Set the number from another Arb.
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
  
  // Set from an ArbInt.
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
  
  // Multiplication operator backend.
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
  
  // Division operator backend.
  Arb& Arb::op_div (Arb const& number) {
    
    // Throw an error on divide by zero.
    if (number == 0) {
      throw ArbErrors::DivByZero();
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
  
  // Modulo division operator backend.
  Arb& Arb::op_mod (Arb const& number) {
    
    // Throw an error on divide by zero.
    if (number == 0) {
      throw ArbErrors::DivByZero();
    }
    
    // Throw an error if both numbers are not integer.
    if (!isInteger() || !number.isInteger()) {
      throw ArbErrors::NonInteger();
    }
    
    // Work area.
    Arb retval(*this, true);
    
    // Simple, just modulo divide p's.
    retval._data->p %= number._data->p;
    
    // We done, return.
    _data = retval._data;
    return *this;
    
  }
  
  // Addition operator backend.
  Arb& Arb::op_add (Arb const& number) {
    
    // If signs are opposite, subtract the negative.
    if (_data->positive != number._data->positive) {
      return op_sub(-number);
    }
    
    // Work area.
    Arb tmp_l(*this,  true);
    Arb tmp_r(number, true);
    
    // Numbers must have the same denominator to add.
    tmp_l._normalize(tmp_r);
    
    // Add the numerators.
    tmp_l._data->p += tmp_r._data->p;
    
    // Reduce the fraction.
    tmp_l._reduce();
    
    // We done. Move the result into place and return.
    _data = tmp_l._data;
    return *this;
    
  }
  
  // Subtraction operator backend.
  Arb& Arb::op_sub (Arb const& number) {
    
    // If signs are opposite, add the negative.
    if (_data->positive != number._data->positive) {
      return op_add(-number);
    }
    
    // Work area.
    Arb tmp_l(*this,  true);
    Arb tmp_r(number, true);
    
    // Numbers must have the same denominator to subtract.
    tmp_l._normalize(tmp_r);
    
    // Add or subtract the numbers.
    if (tmp_r._data->p > tmp_l._data->p) {
      tmp_l._data->p        = tmp_r._data->p - tmp_l._data->p;
      tmp_l._data->positive = !tmp_l._data->positive;
    } else {
      tmp_l._data->p -= tmp_r._data->p;
    }
    
    // Reduce the fraction.
    tmp_l._reduce();
    
    // We done. Move the result into place and return.
    _data = tmp_l._data;
    return *this;
    
  }
  
  // Greater-than operator backend.
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
  
  // Less-than operator backend.
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
  
  // Equal-to operator backend.
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
  
  // Get the ceiling of this fractional number.
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
  
  // Get the floor of this fractional number.
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
  
  // Truncate to just the integer portion.
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
  
  // Raise this number to a power.
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
  
  // Find the nth root of this number.
  Arb Arb::root (Arb const& n) const {
    
    // No divide by zero.
    if (n.isZero()) {
      throw ArbErrors::DivByZero();
    }
    
    // This is a rational number class, not complex.
    if (!isPositive() && n.isEven()) {
      throw ArbErrors::Complex();
    }
    
    // Common work area.
    Arb retval(1);
    
    // Only find integer roots.
    if (n.isInteger()) {
      
      // Work area.
      Arb lastguess;
      Arb accuracy(Arb(_data->base).pow(-Arb(_maxradix)));
      
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
  
  // Common initialization tasks.
  void Arb::_init () {
    
    // Construct this object fully. By definition of the clear() function,
    // the default constructor must do nothing more than call clear().
    clear();
    
  }
  
  // Reduce the number to its most compact representation.
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
  
  // Bitwise shift this number.
  Arb& Arb::_shift (Arb const& bits, _Dir const dir) {
    
    // Only work if necessary.
    if (!*this || !bits) {
      return *this;
    }
    
    // Number of bits to shift must be integer.
    if (!bits.isInteger()) {
      if (dir == _DIR_L) {
        throw ArbErrors::NonIntegerBinary<Arb, Arb>().Left(*this).Operator("<<").Right(bits);
      } else {
        throw ArbErrors::NonIntegerBinary<Arb, Arb>().Left(*this).Operator(">>").Right(bits);
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
  
  // Reduce the number to a specific q.
  Arb& Arb::_forcereduce (_DigsT const& q) {
    
    // Only work if we have to.
    if (_data->q != q) {
      
      //  p       x
      // --- == ------
      //  q      fixq
      _DigsT remainder = _data->p * q % _data->q;
      _data->p         = _data->p * q / _data->q;
      
      // Round.
      if (remainder != 0) {
        switch (_round) {
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
          case ROUND_DEFAULT:
          case ROUND_NORMAL: {
            if (remainder * _DigsT(2) >= _data->q) {
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
  
  // Normalize two numbers.
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
  
  // Set from a float.
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
    if (converter.bits.exponent == (static_cast<unsigned int>(1) << s_floatinfo.exponentbits) - 1) {
      
      // Infinity.
      if (converter.bits.mantissa == 0) {
        if (converter.bits.sign == 0) {
          throw ArbErrors::PositiveInfinity<float>().Number(r);
        } else {
          throw ArbErrors::NegativeInfinity<float>().Number(r);
        }
      }
      
      // NaN.
      throw ArbErrors::NaN<float>().Number(r);
      
    }
    
    // Fraction is always x over 2^one bit less than mantissa.
    new_num._data->q = 1 << s_floatinfo.mantissabits - 1;
    
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
    new_num *= Arb(1) << SafeInt<int>(converter.bits.exponent) - s_floatinfo.bias;
    
    // Set the sign.
    new_num._data->positive = !converter.bits.sign;
    
    // Move in the result and return.
    l._data = new_num._data;
    
  }
  
  // Set from a double.
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
    if (converter.bits.exponent == (static_cast<unsigned int>(1) << s_doubleinfo.exponentbits) - 1) {
      
      // Infinity.
      if (converter.bits.mantissah == 0 && converter.bits.mantissal == 0) {
        if (converter.bits.sign == 0) {
          throw ArbErrors::PositiveInfinity<double>().Number(r);
        } else {
          throw ArbErrors::NegativeInfinity<double>().Number(r);
        }
      }
      
      // NaN.
      throw ArbErrors::NaN<double>().Number(r);
      
    }
    
    // Fraction is always x over 2^one bit less than mantissa.
    new_num._data->q = 1 << s_doubleinfo.mantissabits - 1;
    
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
    new_num *= Arb(1) << SafeInt<int>(converter.bits.exponent) - s_doubleinfo.bias;
    
    // Set the sign.
    new_num._data->positive = !converter.bits.sign;
    
    // Move in the result and return.
    l._data = new_num._data;
    
  }
  
  // Set from a long double.
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
    if (converter.bits.exponent == (static_cast<unsigned int>(1) << s_longdoubleinfo.exponentbits) - 1) {
      
      // Infinity.
      if (converter.bits.mantissah == 0 && converter.bits.mantissal == 0) {
        if (converter.bits.sign == 0) {
          throw ArbErrors::PositiveInfinity<long double>().Number(r);
        } else {
          throw ArbErrors::NegativeInfinity<long double>().Number(r);
        }
      }
      
      // NaN.
      throw ArbErrors::NaN<long double>().Number(r);
      
    }
    
    // Fraction is always x/2^one bit less than mantissa.
    new_num._data->q = ArbInt(1) << s_longdoubleinfo.mantissabits - 1;
    
    // Denormalized numbers.
    if (converter.bits.exponent == 0) {
      
      // Load the mantissa as a fraction. Denormalized use the hidden bit.
      new_num._data->p = converter.bits.mantissal + converter.bits.mantissah * (ArbInt(1) << 32) + converter.bits.j * (ArbInt(1) << 64);
      
      // Correct the exponent.
      converter.bits.exponent = 1;
      
    // Normalized numbers.
    } else {
      
      // Load the mantissa as a fraction. Add in the hidden bit.
      new_num._data->p = new_num._data->q + converter.bits.mantissal + converter.bits.mantissah * (ArbInt(1) << 32);
      
    }
    
    // Multiply by the exponent to get the actual number. Exponent has a bias
    // of 16383.
    new_num *= Arb(1) << (SafeInt<int>(converter.bits.exponent) - s_longdoubleinfo.bias);
    
    // Set the sign.
    new_num._data->positive = !converter.bits.sign;
    
    // Move in the result and return.
    l._data = new_num._data;
    
  }
  
  // Get as a float.
  template <> void Arb::_Get<float, Arb::_NUM_FLPT>::op (float& l, Arb const& r) {
    
    // Work area.
    Arb    tmpnum;
    ArbInt exponent;
    
    // Set tmpnum to r here to avoid carrying over rounding or fixed-point
    // properties, all we are interested in is the value.
    tmpnum.set(r);
    
    // Convert tmpnum to the range of 1 <= tmpnum < 2, save changes in
    // exponent so that tmpnum * 2^exponent == r.
    ArbInt bits_p = tmpnum._data->p.bitsInNumber();
    ArbInt bits_q = tmpnum._data->q.bitsInNumber();
    bool   denorm = false;
    cout << "p: " << tmpnum._data->p << "  bits_p: " << bits_p << endl;
    cout << "q: " << tmpnum._data->q << "  bits_q: " << bits_q << endl;
    if (bits_p < bits_q) {
      
      // Get the difference in bits.
      ArbInt bitdiff = bits_q - bits_p;
      
      // If bitdiff >= bias, try to store this number as a denormalized number.
      if (bitdiff >= s_floatinfo.bias) {
        
        // See if we can keep the exponent within range and still have bits of
        // result left. If not, do the IEEE 754 thing and quietly underflow.
        if (bitdiff >= s_floatinfo.bias + (s_floatinfo.mantissabits - 1)) {
          l = 0;
          return;
        }
        
        // We will have room, so reduce the bitdiff and flag this as
        // denormalized.
        bitdiff = s_floatinfo.bias - 1;
        denorm  = true;
        
      }
      
      // Shift p up until it has the same number of bits as q, then make sure
      // that 1 <= tmpnum < 2, unless this is a denormalized number. Set
      // exponent acconding to the number of bits shifted.
      tmpnum._data->p <<= bitdiff;
      exponent          = s_floatinfo.bias - bitdiff;
      if (!denorm && tmpnum._data->q > tmpnum._data->p) {
        if (exponent == 1) {
          denorm = true;
        } else {
          tmpnum._data->p <<= 1;
          exponent         -= 1;
        }
      }
      
    } else {
      
      // Get the difference in bits.
      ArbInt bitdiff = bits_p - bits_q;
      
      // If bitdiff > bias, we won't be able to store this number, it will
      // overflow the exponent.
      if (bitdiff > s_floatinfo.bias) {
        throw ArbErrors::ScalarOverflowSpecialized<float>().Number(r).Limit(numeric_limits<float>::max());
      }
      
      // Shift q up until it has the same number of bits as p, then make sure
      // that 1 <= tmpnum < 2. Set exponent according to the number of bits
      // shifted.
      tmpnum._data->q <<= bitdiff;
      exponent          = s_floatinfo.bias + bitdiff;
      if (tmpnum._data->q > tmpnum._data->p) {
        tmpnum._data->q >>= 1;
        exponent         -= 1;
      }
      
    }
    
    l = 0;
    
    cout << "r: " << r << endl;
    // 1e-38 makes this fail.
    cout << "tmpnum: " << tmpnum << "  exponent: " << exponent << endl;
    
  }
  
  // Get as a double.
  template <> void Arb::_Get<double, Arb::_NUM_FLPT>::op (double& l, Arb const& r) {
    // FIXME: Dummy.
    if (l > 0) {};
    if (r > 0) {};
  }
  
  // Get as a long double.
  template <> void Arb::_Get<long double, Arb::_NUM_FLPT>::op (long double& l, Arb const& r) {
    // FIXME: Dummy.
    if (l > 0) {};
    if (r > 0) {};
  }
  
  /***************************************************************************
   * Class Arb::_Data.
   ***************************************************************************/
  
  // Default constructor.
  Arb::_Data::_Data () {
    
    // Call common init.
    _init();

  }
  
  // Copy constructor.
  Arb::_Data::_Data (_Data const& data) {
    
    // Call common init.
    _init();
    
    // Copy the given data.
    copy(data);
    
  }
  
  // Assignment operator.
  Arb::_Data& Arb::_Data::operator = (_Data const& data) {
    
    // Copy the given data.
    return copy(data);
    
  }
  
  // Reset to just constructed state.
  Arb::_Data& Arb::_Data::clear () {
    
    // These operations may throw, do them first so any error will be thrown
    // before any changes are made.
    _DigsT new_p;
    _DigsT new_q;
    _DigsT new_fixq;
    new_q.set(1);
    
    // These operations will never throw.
    p    = new_p;
    q    = new_q;
    fixq = new_fixq;
    
    positive = true;
    pointpos = 0;
    fix      = false;
    base     = 10;
    
    // We done.
    return *this;
    
  }
  
  // Copy a given _Data object.
  Arb::_Data& Arb::_Data::copy (_Data const& data) {
    
    // Copy the given data.
    p    = data.p;
    q    = data.q;
    fixq = data.fixq;
    
    positive = data.positive;
    pointpos = data.pointpos;
    fix      = data.fix;
    base     = data.base;
    
    // We done.
    return *this;
    
  }
  
  // Common initialization tasks.
  void Arb::_Data::_init () {
    
    // Construct this object fully. By definition of the clear() function, the
    // default constructor must do nothing more than call clear().
    clear();

  }
  
  /***************************************************************************
   * Errors.
   ***************************************************************************/
  namespace ArbErrors {
    
    char const* Base::what () const throw() {
      return "Undefined error in Arb.";
    }
    Base::~Base () throw() {
      // Nothing.
    }
    
    char const* BadFormat::what () const throw() {
      try {
        return (std::string(_problem) + " at position " + DAC::toString(SafeInt<std::string::size_type>(_position) + 1) + " in number \"" + *_number + "\".").c_str();
      } catch (...) {
        return "Bad format. Error creating message string.";
      }
    }
    
    char const* DivByZero::what () const throw() {
      return "Divide by zero.";
    }
    
    char const* Complex::what () const throw() {
      return "Even roots of negative numbers can only be complex numbers.";
    }
    
    char const* ComplexRoot::what () const throw() {
      try {
        return (std::string("Attempting to take the even root ") + DAC::toString(_root) + " of negative number " + DAC::toString(_number) + " results in a complex number.").c_str();
      } catch (...) {
        return "Even roots of negative numbers can only be complex numbers. Error creating message string.";
      }
    }
    
    char const* NonInteger::what () const throw() {
      return "An integer operation was attempted on a non-integer number.";
    }
    
    char const* ScalarOverflow::what () const throw() {
      return "Arb overflows requested scalar type.";
    }
    
    char const* InvalidFloat::what () const throw() {
      return "Attempt to set Arb from an invalid floating-point number.";
    }
    
  }
  
}
