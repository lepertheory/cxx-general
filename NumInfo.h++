/*****************************************************************************
 * NumInfo.h++
 *****************************************************************************
 * Classes that retrieve information about numbers.
 *****************************************************************************/

// Include guard.
#if !defined(NUMINFO_8989hu89h)
  #define NUMINFO_8989hu89h

// Standard includes.
  #include <limits>

// Namespace wrapper.
namespace DAC {

  /***************************************************************************/
  // Types.
  
  // Different possible relationships.
  namespace RelTypes {
    enum Type {
      SE_SE, SE_UE, SS_SL, SS_UL, SL_SS, SL_US,
      UE_UE, UE_SE, US_SL, US_UL, UL_SS, UL_US,
      ERROR_INTEGER_ONLY
    };
  }

  /***************************************************************************
   * IntRel
   ***************************************************************************
   * Returns relationship between two integers. Compiles down to a constant,
   * do not instantiate.
   ***************************************************************************/
  template <class T, class U> class IntRel {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Constants.
      static RelTypes::Type const type;
      
    /*
     * Private members.
     */
    private:
      
      // Instantiation not allowed.
      IntRel ();
      IntRel (IntRel const&);
      operator = (IntRel const&);
      
  };
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************
   * IntRel
   ***************************************************************************/
  
  /***************************************************************************/
  // Constants.
  
  // The relationship itself.
  template <class T, class U> RelTypes::Type const IntRel<T, U>::value =
    (std::numeric_limits<T>::is_integer && std::numeric_limits<U>::is_integer) ? (
      (std::numeric_limits<T>::digits > std::numeric_limits<U>::digits) ? (
        std::numeric_limits<T>::is_signed ? (
          std::numeric_limits<U>::is_signed ? (
            SL_SS
          ) : (
            SL_US
          )
        ) : (
          std::numeric_limits<U>::is_signed ? (
            UL_SS
          ) : (
            UL_US
          )
        )
      ) : (
        (std::numeric_limits<T>::digits < std::numeric_limits<U>::digits) ? (
          std::numeric_limits<T>::is_signed ? (
            std::numeric_limits<U>::is_signed ? (
              SS_SL
            ) : (
              SS_UL
            )
          ) : (
            std::numeric_limits<U>::is_signed ? (
              US_SL
            ) : (
              US_UL
            )
          )
        ) : (
          std::numeric_limits<T>::is_signed ? (
            std::numeric_limits<U>::is_signed ? (
              SE_SE
            ) : (
              SE_UE
            )
          ) : (
            std::numeric_limits<U>::is_signed ? (
              UE_SE
            ) : (
              UE_UE
            )
          )
        )
      )
    ) : (
      ERROR_INTEGER_ONLY
    )
  ;
  
}

// End include guard.
#endif
