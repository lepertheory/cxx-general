/*****************************************************************************
 * Overflow.hxx
 *****************************************************************************
 * A collection of functions for dealing with overflow.
 *****************************************************************************/

// Include guard.
#if !defined(OVERFLOW_0o0e9ia07i0a)
  #define OVERFLOW_0o0e9ia07i0a
  
  // Internal includes.
  #include "ReferencePointer.hxx"
  #include "Exception.hxx"
  #include "toString.hxx"
  
  // Namespace wrapping.
  namespace DAC {
    
    // Errors.
    namespace OverflowErrors {
      
      class Base             : public Exception {};
      class UnsignedNegative : public Base      {};
      class NaN              : public Base      {};
      
      void throwBase             (std::string const& text = "") throw(Base);
      void throwUnsignedNegative (std::string const& text = "") throw(UnsignedNegative);
      void throwNaN              (std::string const& text = "") throw(NaN);
      
    };
    
    // Casting.
    template <class T, class FT> T of_static_cast (FT const from);
    
    // Arithmetic.
    template <class T, class LT, class RT> T of_add (LT const left, RT const right);
    template <class T, class LT, class RT> T of_sub (LT const left, RT const right);
    template <class T, class LT, class RT> T of_mul (LT const left, RT const right);
    template <class T, class LT, class RT> T of_div (LT const left, RT const right);
    
    /*************************************************************************/
    // Throw errors.
    namespace OverflowErrors {
      inline void throwBase             (std::string const& text) throw(Base)             { Base             error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwUnsignedNegative (std::string const& text) throw(UnsignedNegative) { UnsignedNegative error; try { error.Text(text); } catch (...) {} throw error; }
      inline void throwNaN              (std::string const& text) throw(NaN)              { NaN              error; try { error.Text(text); } catch (...) {} throw error; }
    };
    /*************************************************************************/
    
    /*************************************************************************/
    // Cast to a new type.
    template <class T, class FT> T of_static_cast (FT from) {
      
      // Check for negative number problems.
      if (from < 0) {
        
        // Cast to unsigned type.
        if (!std::numeric_limits<T>::is_signed) {
          OverflowErrors::throwUnsignedNegative("Attempt to cast " + toString(from) + " to unsigned type.");
        }
        
        // Underflow.
        if (from < std::numeric_limits<T>::min()) {
          OverflowErrors::throwBase(toString(from) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
        }
        
      }
      
      // Check for overflow.
      if (from > std::numeric_limits<T>::max()) {
        OverflowErrors::throwBase(toString(from) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
      }
      
      // Do the cast.
      return static_cast<T>(from);
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Add.
    template <class T, class LT, class RT> T of_add (LT const left, RT const right) {
      
      // Work area. No need to initialize, we'll be setting each one.
      T tmp_left;
      T tmp_right;
      
      // Cast to the proper type.
      tmp_left  = of_static_cast<T, LT>(left);
      tmp_right = of_static_cast<T, RT>(right);
      
      // Check for underflow.
      if (std::numeric_limits<T>::is_signed && ((tmp_left < 0) && (tmp_right < 0)) && (tmp_right < (std::numeric_limits<T>::min() - tmp_left))) {
        OverflowErrors::throwBase(toString(left) + " + " + toString(right) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
      }
      
      // Check for overflow.
      if ((!std::numeric_limits<T>::is_signed && (tmp_right > (std::numeric_limits<T>::max() - tmp_left))) || (std::numeric_limits<T>::is_signed && ((tmp_left > 0) && (tmp_right > 0)) && (tmp_right > (std::numeric_limits<T>::max() - tmp_left)))) {
        OverflowErrors::throwBase(toString(left) + " + " + toString(right) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
      }
      
      // Do the add and return the result.
      return (tmp_left + tmp_right);
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Subtract.
    template <class T, class LT, class RT> T of_sub (LT const left, RT const right) {
      
      // Work area. No need to initialize, we'll be setting each one.
      T tmp_left;
      T tmp_right;
      
      // Cast to the proper type.
      tmp_left  = of_static_cast<T, LT>(left);
      tmp_right = of_static_cast<T, RT>(right);
      
      // Check for underflow.
      if ((std::numeric_limits<T>::is_signed && (((tmp_left < 0) && (tmp_right > 0)) && (-tmp_right > (std::numeric_limits<T>::min() - tmp_left)))) || (!std::numeric_limits<T>::is_signed && (tmp_right > (std::numeric_limits<T>::min() + tmp_left)))) {
        OverflowErrors::throwBase(toString(left) + " - " + toString(right) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
      }
      
      // Check for overflow.
      if (std::numeric_limits<T>::is_signed && (((tmp_left > 0) && (tmp_right < 0)) && (-tmp_right > (std::numeric_limits<T>::max() - tmp_left)))) {
        OverflowErrors::throwBase(toString(left) + " - " + toString(right) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
      }
      
      // Do the subtract and return the result.
      return (tmp_left - tmp_right);
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Multiply.
    template <class T, class LT, class RT> T of_mul (LT const left, RT const right) {
      
      // Work area. No need to initialize, we'll be setting each one.
      T tmp_left;
      T tmp_right;
      
      // Cast to the proper type.
      tmp_left  = of_static_cast<T, LT>(left);
      tmp_right = of_static_cast<T, RT>(right);
      
      // Check for underflow.
      if ((((tmp_left > 0) && (tmp_right < 0)) && (tmp_right < (std::numeric_limits<T>::min() / tmp_left))) || (((tmp_left < 0) && (tmp_right > 0)) && (tmp_right > (std::numeric_limits<T>::min() / tmp_left)))) {
        OverflowErrors::throwBase(toString(left) + " * " + toString(right) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
      }
      
      // Check for overflow.
      if ((((tmp_left > 0) && (tmp_right > 0)) && (tmp_right > (std::numeric_limits<T>::max() / tmp_left))) || (((tmp_left < 0) && (tmp_right < 0)) && (-tmp_right > (std::numeric_limits<T>::max() / -tmp_left)))) {
        OverflowErrors::throwBase(toString(left) + " * " + toString(right) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
      }
      
      // Do the multiply and return the result.
      return (tmp_left * tmp_right);
      
    }
    /*************************************************************************/
    
    /*************************************************************************/
    // Divide.
    template <class T, class LT, class RT> T of_div (LT const left, RT const right) {
      
      // Work area. No need to initialize, we'll be setting each one.
      T tmp_left;
      T tmp_right;
      
      // Cast to the proper type.
      tmp_left  = of_static_cast<T, LT>(left);
      tmp_right = of_static_cast<T, RT>(right);
      
      // Check for divide by zero.
      if (tmp_right == 0) {
        OverflowErrors::throwNaN("Divide by zero.");
      }
      
      // Cannow overflow unless it's floating-point math.
      if (!std::numeric_limits<T>::is_integer) {
        
        // Check for underflow
        if ((((tmp_left > 0) && (tmp_right < 0)) && (tmp_right > -1) && (-tmp_left < (std::numeric_limits<T>::min() * -tmp_right))) || (((tmp_left < 0) && (tmp_right > 0)) && (tmp_right < 1) && (tmp_left < (std::numeric_limits<T>::min() * tmp_right)))) {
          OverflowErrors::throwBase(toString(left) + " / " + toString(right) + " is less than type minimum of " + toString(std::numeric_limits<T>::min()) + ".");
        }
        
        // Check for overflow.
        if ((((tmp_left > 0) && (tmp_right > 0)) && (tmp_right < 1) && (tmp_left > (std::numeric_limits<T>::max() * tmp_right))) || (((tmp_left < 0) && (tmp_right < 0)) && (tmp_right > -1) && (-tmp_left > (std::numeric_limits<T>::max() * -tmp_right)))) {
          OverflowErrors::throwBase(toString(left) + " / " + toString(right) + " is greater than type maximum of " + toString(std::numeric_limits<T>::max()) + ".");
        }
        
      }
      
      // Do the divide and return the result.
      return (tmp_left / tmp_right);
      
    }
    /*************************************************************************/
    
  };
  
// End include guard.
#endif
