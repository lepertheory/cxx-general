/*****************************************************************************
 * ArbInt.c++
 *****************************************************************************
 * Implementation for class ArbInt.
 *****************************************************************************/

// Standard includes.
#include <string>

// System includes.
#include <UArbInt.h++>
#include <SafeInt.h++>

// Class include.
#include "ArbInt.h++"

// Bring in namespaces.
using namespace std;

// Namespace wrapper.
namespace DAC {

  /***************************************************************************
   * ArbInt
   ***************************************************************************/

  /***************************************************************************/
  // Function members.

  /*
   * Set from a string.
   */
  ArbInt& ArbInt::set (string const& number, bool const autobase) {
    
    // Empty string is zero.
    if (number.empty()) {
      _digits   =    0;
      _positive = true;
      return *this;
    }
    
    // Set with sign.
    if (number.length() > 1 && (number[0] == '+' || number[0] == '-')) {
      try {
        _digits.set(number.substr(1), autobase);
        _positive = number[0] == '+';
      } catch (UArbInt::Errors::BadFormat& e) {
        throw Errors::BadFormat().Problem(e.Problem()).Position(SafeInt<string::size_type>(e.Position()) + 1);
      }
      
    // Set without sign.
    } else {
      try {
        _digits.set(number, autobase);
        _positive = true;
      } catch (UArbInt::Errors::BadFormat& e) {
        throw Errors::BadFormat().Problem(e.Problem()).Position(e.Position());
      }
    }
    
    // Done.
    return *this;
    
  }
  
  /*
   * Multiply.
   */
  ArbInt& ArbInt::op_mul (ArbInt const& number) {
    
    // Mulitply values and set sign.
    _digits   *= number._digits;
    _positive  = _positive == number._positive;
    
    // Done.
    return *this;
    
  }
  ArbInt& ArbInt::op_mul (UArbInt const& number) {
    
    // Multiply values, sign does not change.
    _digits *= number;
    
    // Done.
    return *this;
    
  }
  
  /*
   * Divide.
   */
  ArbInt& ArbInt::op_div (ArbInt const& number, ArbInt* const remainder) {
    
    // Divide values and set signs.
    if (remainder) {
      try {
        _digits.op_div(number._digits, remainder->_digits);
      } catch (UArbInt::Errors::DivByZero&) {
        throw Errors::DivByZero;
      }
      remainder->_positive = _positive;
    } else {
      try {
        _digits /= number._digits;
      } catch (UArbInt::Errors::DivByZero&) {
        throw Errors::DivByZero;
      }
    }
    _positive = _positive == number._positive;
    
    // Done.
    return *this;
    
  }
  ArbInt& ArbInt::op_div (UArbInt const& number, ArbInt* const remainder) {
    
    // Divide values. Set the sign on the remainder.
    if (remainder) {
      try {
        _digits.op_div(number, remainder->_digits);
      } catch (UArbInt::Errors::DivByZero&) {
        throw Errors::DivByZero;
      }
      remainder->_positive = _positive;
    } else {
      try {
        _digits /= number;
      } catch (UArbInt::Errors::DivByZero&) {
        throw Errors::DivByZero;
      }
    }
    
    // Done.
    return *this;
    
  }
  
  /*
   * Modulo divide.
   */
  ArbInt& ArbInt::op_mod (ArbInt const& number) {
    
    // Modulo divide. Sign remains the same.
    try {
      _digits %= number._digits;
    } catch (UArbInt::Errors::DivByZero&) {
      throw Errors::DivByZero;
    }
    
    // Done.
    return *this;
    
  }
  ArbInt& ArbInt::op_mod (UArbInt const& number) {
    
    // Modulo divide.
    try {
      _digits %= number;
    } catch (UArbInt::Errors::DivByZero&) {
      throw Errors::DivByZero;
    }
    
    // Done.
    return *this;
    
  }
  
  /*
   * Add.
   */
  ArbInt& ArbInt::op_add (ArbInt const& number) {
    
    // Add or subtract based on signs.
    if (number._positive == _positive) {
      _digits += number._digits;
    } else {
      if (_digits >= number._digits) {
        _digits -= number._digits;
      } else {
        _digits = number._digits - _digits;
        _positive = !_positive;
      }
    }
    
    // Done.
    return *this;
    
  }
  ArbInt& ArbInt::op_add (UArbInt const& number) {
    
    // Add or subtract based on signs.
    if (_positive) {
      _digits += number;
    } else {
      if (_digits >= number) {
        _digits -= number;
      } else {
        _digits = number - _digits;
        _positive = !_positive;
      }
    }
    
    // Done.
    return *this;
    
  }
  
  /*
   * Subtract.
   */
  ArbInt& ArbInt::op_sub (ArbInt const& number) {
    
    // Subtract or add based on signs.
    if (number._positive == _positive) {
      if (_digits >= number._digits) {
        _digits -= number._digits;
      } else {
        _digits = number._digits - _digits;
        _positive = !_positive;
      }
    } else {
      _digits += number._digits;
    }
    
    // Done.
    return *this;
    
  }
  ArbInt& ArbInt::op_sub (UArbInt const& number) {
    
    // Subtract or add based on signs.
    if (_positive) {
      if (_digits >= number._digits) {
        _digits -= number;
      } else {
        _digits = number - _digits;
        _positive = !_positive;
      }
    } else {
      _digits += number;
    }
    
    // Done.
    return *this;
    
  }
  
  /*
   * Master comparison operator.
   */
  int ArbInt::op_compare (ArbInt const& number) {
    
    // Check signs first.
    if (_positive) {
      if (!number._positive) {
        return 1;
      }
    } else {
      if (number._positive) {
        return -1;
      }
    }
    
    // Signs are the same, compare numbers.
    switch (_digits.op_compare(number._digits)) {
      case -1: return _positive ? return -1 : return  1;
      case  0; return 0;
      case  1; return _positive ? return  1 : return -1;
    }
    
  }
  int ArbInt::op_compare (UArbInt const& number) {
    
    // Check signs first.
    if (!_positive) {
      return -1;
    }
    
    // Signs are the same, compare numbers. No need to check for sign
    // inversion.
    return _digits.op_compare(number);
    
  }
  
}

