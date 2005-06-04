/*****************************************************************************
 * rppower.hxx
 *****************************************************************************
 * Russian Peasant Algorithm method of finding a power. Why isn't this in the
 * Standard Library? I don't want floating point.
 *****************************************************************************/

// Include guard.
#if !defined(RPPOWER_gu2j9g8uuhu)
  #define RPPOWER_gu2j9g8uuhu

// Internal includes.
  #include "SafeInt.hxx"

// Contain in namespace.
namespace DAC {
  
  // Raise a number to a power.
  template <class NumT, class ExpT> NumT          rppower (NumT          const  x, ExpT          const  y);
  template <class NumT, class ExpT> SafeInt<NumT> rppower (SafeInt<NumT> const& x, SafeInt<ExpT> const& y);
  template <class NumT, class ExpT> SafeInt<NumT> rppower (SafeInt<NumT> const& x, ExpT          const  y);
  template <class NumT, class ExpT> SafeInt<NumT> rppower (NumT          const  x, SafeInt<ExpT> const& y);
  
  /*************************************************************************/
  // Raise a number to a power.
  template <class NumT, class ExpT> inline NumT rppower (NumT x, ExpT y) {
    
    // Powers start at 1.
    NumT result = static_cast<NumT>(1);
    
    // Here it is. I didn't write this. It's very sneaky. x's last
    // multiplication gets thrown away. If small exponents are primarily
    // being used, it may be more efficient to put in a test on the odd
    // number to see if y is 1 and return at that point. I like less lines
    // though. Whee. I did write the bit that makes this handle negative
    // powers though. Whee.
    if (y >= 0) {
      while (y) {
        if (y & 1) {
          result *= x;
        }
        x  *= x;
        y >>= 1;
      }
    } else {
      while (y) {
        if (y & 1) {
          result /= x; // I did this. I am so smrt. S-M-R-T.
        }
        x  *= x;
        y >>= 1;
      }
    }
    
    // Done.
    return result;
    
  }
  /*************************************************************************/
  
  // Other specializations of rppower.
  template <class NumT, class ExpT> SafeInt<NumT> rppower (SafeInt<NumT> const& x, SafeInt<ExpT> const& y) {
    SafeInt<ExpT> tmp_y(y);
    SafeInt<NumT> tmp_x(x);
    SafeInt<NumT> result(1);
    if (tmp_y >= 0) {
      while (tmp_y.Value()) {
        if ((tmp_y & 1).Value()) {
          result *= tmp_x;
        }
        if (tmp_y > 1) {
          tmp_x *= tmp_x;
        }
        tmp_y >>= 1;
      }
    } else {
      while (tmp_y.Value()) {
        if ((tmp_y & 1).Value()) {
          result /= tmp_x;
        }
        if (tmp_y > 1) {
          tmp_x *= tmp_x;
        }
        tmp_y >>= 1;
      }
    }
    return result;
  }
  template <class NumT, class ExpT> SafeInt<NumT> rppower (SafeInt<NumT> const& x, ExpT const y) {
    SafeInt<ExpT> tmp_y(y);
    SafeInt<NumT> tmp_x(x);
    SafeInt<NumT> result(1);
    if (tmp_y >= 0) {
      while (tmp_y.Value()) {
        if ((tmp_y & 1).Value()) {
          result *= tmp_x;
        }
        if (tmp_y > 1) {
          tmp_x *= tmp_x;
        }
        tmp_y >>= 1;
      }
    } else {
      while (tmp_y.Value()) {
        if ((tmp_y & 1).Value()) {
          result /= tmp_x;
        }
        if (tmp_y > 1) {
          tmp_x *= tmp_x;
        }
        tmp_y >>= 1;
      }
    }
    return result;
  }
  template <class NumT, class ExpT> SafeInt<NumT> rppower (NumT const x, SafeInt<ExpT> const& y) {
    SafeInt<ExpT> tmp_y(y);
    SafeInt<NumT> tmp_x(x);
    SafeInt<NumT> result(1);
    if (tmp_y >= 0) {
      while (tmp_y.Value()) {
        if ((tmp_y & 1).Value()) {
          result *= tmp_x;
        }
        if (tmp_y > 1) {
          tmp_x *= tmp_x;
        }
        tmp_y >>= 1;
      }
    } else {
      while (tmp_y.Value()) {
        if ((tmp_y & 1).Value()) {
          result /= tmp_x;
        }
        if (tmp_y > 1) {
          tmp_x *= tmp_x;
        }
        tmp_y >>= 1;
      }
    }
    return result;
  }
  
}

// End include guard.
#endif
