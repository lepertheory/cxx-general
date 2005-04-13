/*****************************************************************************
 * Arb.cxx
 *****************************************************************************
 * Implementation for class Arb.
 *****************************************************************************/

// STL includes.
#include <string>
#include <algorithm>
#include <iostream> // FIXME: For debugging only.

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
  
  // Accessors.
  Arb& Arb::Base     (BaseT     const base)     {
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
  Arb& Arb::Fixed    (bool      const fixed)    {
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
    
    // Output the sign if negative, unless outputting both formats.
    if ((fmt != FMT_BOTH) && !(_data->positive) && _data->p) {
      retval += "-";
    }
    
    // Choose the output format.
    switch (fmt) {
      
      // Output both notations.
      case FMT_BOTH: {
        
        retval += toString(FMT_RADIX) + " " + toString(FMT_FRACTION);
        
      } break;
      
      // Output in radix notation.
      case FMT_RADIX: {
        
        // Output the whole number part.
        _DigsT whole(_data->p / _data->q);
        retval += whole.Base(_data->base).toString();
        
        // Output the radix part if it exists.
        _DigsT remainder = _data->p % _data->q;
        if (((_data->fixq) && (_data->pointpos > 0)) || (!(_data->fixq) && remainder)) {
          
          // Add the radix point.
          retval += ".";
          
          // Get the radix digits, one by one. Output digits until the entire
          // fraction is output or the maximum requested significant radix digits
          // are output.
          std::string::size_type sigdigs  = 0;
          bool                   sigstart = whole;
          _DigsT                 digit;
          digit.Base(_data->base);
          while ((sigdigs < _maxradix) && remainder) {
            remainder *= _data->base;
            digit      = remainder / _data->q;
            retval    += digit.toString();
            remainder %= _data->q;
            if (sigstart || digit) {
              ++sigdigs;
              sigstart = true;
            }
          }
          
          // Strip insignificant zeros.
          if (retval.find_last_not_of('0') != string::npos) {
            retval.erase(retval.find_last_not_of('0') + 1);
          }
          
          // If this is a fixed-radix number, fix the radix.
          if (_data->fixq) {
            
            // Pad with zeros.
            std::string::size_type raddigs = (retval.size() - retval.find('.')) - 1;
            if (raddigs < _data->pointpos) {
              retval.append(_data->pointpos - raddigs, '0');
            }
            
          // If this is not a fixed-radix number, we may need to remove the radix
          // point.
          } else {
            
            if ((retval.size() - 1) == retval.find('.')) {
              retval.erase(retval.find('.'));
            }
            
          }
          
        }
        
      } break;
      
      // Output in fractional format.
      case FMT_FRACTION: {
        
        // Easy, output p/q.
        retval += _data->p.Base(_data->base).toString() + "/" + _data->q.Base(_data->base).toString();
        
      } break;
      
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
    Arb retval(*this, true);
    
    // Set the new p & q;
    retval._data->p = number._data->p;
    retval._data->q = number._data->q;
    
    // Reduce the fraction.
    retval._reduce();
    
    // Move the result into place and return.
    _data = retval._data;
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
    if (!number) {
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
  
  // Addition operator backend.
  Arb& Arb::op_add (Arb const& number) {
    
    // If adding an opposite sign, subtract the negative.
    if (_data->positive != number._data->positive) {
      return op_sub(-number);
    }
    
    // Work area.
    Arb tmp_l(*this,  true);
    Arb tmp_r(number, true);
    
    // Numbers must have the same denominator to add.
    tmp_l._normalize(tmp_r);
    
    // Add the numerators together.
    tmp_l._data->p += tmp_r._data->p;
    
    // Reduce the fraction.
    tmp_l._reduce();
    
    // We done. Move the result into place and return.
    _data = tmp_l._data;
    return *this;
    
  }
  
  // Subtraction operator backend.
  Arb& Arb::op_sub (Arb const& number) {
    
    // If subtracting an opposite sign, add the negative.
    if (_data->positive != number._data->positive) {
      return op_add(-number);
    }
    
    // Work area.
    Arb tmp_l(*this,  true);
    Arb tmp_r(number, true);
    
    // Numbers must have the same denominator to subtract.
    tmp_l._normalize(tmp_r);
    
    // Subtract the numbers.
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
  
  // Raise this number to a power.
  Arb Arb::pow (Arb const& exp) const {
    
    // Work area.
    Arb retval(*this, true);
    Arb one(1);
    
    // No work if this is zero or if we are raising to the 1.
    if (!isZero() || (exp == one)) {
      
      // Only raise if we have to.
      if (exp) {
        
        // Raise this number to an integer power.
        if (exp.isInteger()) {
          
          // No need to raise 1 or to 1.
          if ((retval.abs() != one) && (exp.abs() != one)) {
            
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
          retval = one / retval;
        }
        
      // Raising to the 0 power is always 1.
      } else {
        
        retval = one;
        
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
    Arb one(1);
    
    // Only find integer roots.
    if (n.isInteger()) {
      
      // Work area.
      Arb lastguess;
      Arb two(2);
      Arb accuracy(Arb(_data->base).pow(-Arb(_maxradix)));
      
      // Create an initial guess, will be within at least one ^2, so Newton's
      // algorithm should begin converging by doubling correct # of bits each
      // iteration.
      if (abs() > one) {
        retval = two;
        while (retval.pow(n) < *this) {
          retval *= two;
        }
      }
      
      Arb precision = retval;
      
      // Newton's algorithm. 
      Arb nmo(n - one);
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
      
      retval = pow(one / n);
      
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
      _forcereduce(_data->q);
      
    // Floating-point numbers are simply reduced.
    } else {
      reduce(_data->p, _data->q);
    }
    
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
      _data->p = _data->p * q / _data->q;
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
  
  // Find a whole-number root of this number.
  Arb& Arb::_root (_DigsT const& root) {
    
    // We can only get the root of a whole number, so if this is not a whole
    // number, return the result of this formula:
    //        _____     _ x___
    //  _  x /  y        \/ y
    //   \  /  ---  == --------
    //    \/    z       _ x___
    //                   \/ z
    // 
    // x = root, y = _data->p, z = _data->q.
    Arb tmp_p;
    Arb tmp_q;
    tmp_p._data->p = _data->p;
    tmp_q._data->p = _data->q;
    return set(tmp_p._root(root) / tmp_q._root(root));
    
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
    positive = true;
    
    p    = new_p;
    q    = new_q;
    fixq = new_fixq;
    
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
    
    pointpos = data.pointpos;
    fix      = data.fix;
    base     = data.base;
    
  }
  
  // Common initialization tasks.
  void Arb::_Data::_init () {
    
    // Construct this object fully. By definition of the clear() function, the
    // default constructor must do nothing more than call clear().
    clear();

  }
  
};
