/*****************************************************************************
 * SafeInt.hxx
 *****************************************************************************
 * Safe integer class.
 *****************************************************************************/

// Include guard.
#if !defined(SAFEINT_3k54kbuihub7hbh0)
  #define SAFEINT_3k54kbuihub7hbh0

// STL includes.
  #include <string>
  #include <cmath>

// Internal includes.
  #include "Exception.hxx"
  
// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * SafeInt utilities.
   ***************************************************************************/
  namespace SafeIntUtil {
  
    /*************************************************************************/
    // Enums.
    
    // Relationship types.
    enum RelType {
      SE_SE, SE_UE, SS_SL, SS_UL, SL_SS, SL_US,
      UE_UE, UE_SE, US_SL, US_UL, UL_SS, UL_US
    };
    
    // Determine the relationship between two types.
    template <class T, class U> class Relationship {
      public:
        const static RelType value;
    };
    
    // Safely cast.
    template <class T, class U, RelType> class SafeCast;
    template <class T, class U> class SafeCast<T, U, SE_SE> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, SE_UE> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, SS_SL> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, SS_UL> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, SL_SS> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, SL_US> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, UE_UE> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, UE_SE> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, US_SL> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, US_UL> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, UL_SS> { public: static U op (T const value); };
    template <class T, class U> class SafeCast<T, U, UL_US> { public: static U op (T const value); };
    
    // Raw cast.
    template <class T, class U, RelType> class RawCast;
    template <class T, class U> class RawCast<T, U, SE_SE> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, SE_UE> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, SS_SL> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, SS_UL> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, SL_SS> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, SL_US> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, UE_UE> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, UE_SE> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, US_SL> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, US_UL> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, UL_SS> { public: static U op (T const value); };
    template <class T, class U> class RawCast<T, U, UL_US> { public: static U op (T const value); };
    
    // Safely multiply.
    template <class T, class U, RelType> class SafeMul;
    template <class T, class U> class SafeMul<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMul<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely divide.
    template <class T, class U, RelType> class SafeDiv;
    template <class T, class U> class SafeDiv<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeDiv<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely modulo divide.
    template <class T, class U, RelType> class SafeMod;
    template <class T, class U> class SafeMod<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeMod<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely add.
    template <class T, class U, RelType> class SafeAdd;
    template <class T, class U> class SafeAdd<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeAdd<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely subtract.
    template <class T, class U, RelType> class SafeSub;
    template <class T, class U> class SafeSub<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeSub<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely shift left.
    template <class T, class U, RelType> class SafeShL;
    template <class T, class U> class SafeShL<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShL<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely shift right.
    template <class T, class U, RelType> class SafeShR;
    template <class T, class U> class SafeShR<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeShR<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely test for greater than.
    template <class T, class U, RelType> class SafeGT;
    template <class T, class U> class SafeGT<T, U, SE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, SE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, SS_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, SS_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, SL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, SL_US> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, UE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, UE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, US_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, US_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, UL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGT<T, U, UL_US> { public: static bool op (T const l, U const r); };
    
    // Safely test for greater than or equal to.
    template <class T, class U, RelType> class SafeGE;
    template <class T, class U> class SafeGE<T, U, SE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, SE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, SS_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, SS_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, SL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, SL_US> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, UE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, UE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, US_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, US_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, UL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeGE<T, U, UL_US> { public: static bool op (T const l, U const r); };
    
    // Safely test for less than.
    template <class T, class U, RelType> class SafeLT;
    template <class T, class U> class SafeLT<T, U, SE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, SE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, SS_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, SS_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, SL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, SL_US> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, UE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, UE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, US_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, US_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, UL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLT<T, U, UL_US> { public: static bool op (T const l, U const r); };
    
    // Safely test for less than or equal to.
    template <class T, class U, RelType> class SafeLE;
    template <class T, class U> class SafeLE<T, U, SE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, SE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, SS_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, SS_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, SL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, SL_US> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, UE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, UE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, US_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, US_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, UL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeLE<T, U, UL_US> { public: static bool op (T const l, U const r); };
    
    // Safely test for equal to.
    template <class T, class U, RelType> class SafeEQ;
    template <class T, class U> class SafeEQ<T, U, SE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, SE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, SS_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, SS_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, SL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, SL_US> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, UE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, UE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, US_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, US_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, UL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeEQ<T, U, UL_US> { public: static bool op (T const l, U const r); };
    
    // Safely test for not equal.
    template <class T, class U, RelType> class SafeNE;
    template <class T, class U> class SafeNE<T, U, SE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, SE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, SS_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, SS_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, SL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, SL_US> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, UE_UE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, UE_SE> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, US_SL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, US_UL> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, UL_SS> { public: static bool op (T const l, U const r); };
    template <class T, class U> class SafeNE<T, U, UL_US> { public: static bool op (T const l, U const r); };
    
    // Safely bitwise AND.
    template <class T, class U, RelType> class SafeBitAnd;
    template <class T, class U> class SafeBitAnd<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitAnd<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely bitwise inclusive OR.
    template <class T, class U, RelType> class SafeBitIOr;
    template <class T, class U> class SafeBitIOr<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitIOr<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely bitwise exclusive OR.
    template <class T, class U, RelType> class SafeBitXOr;
    template <class T, class U> class SafeBitXOr<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeBitXOr<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely bitwise compliment.
    template <class T> class SafeBitCpm { public: static T op (T const value); };
    
    // Safely logical AND.
    template <class T, class U, RelType> class SafeLogAnd;
    template <class T, class U> class SafeLogAnd<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogAnd<T, U, UL_US> { public: static T op (T const l, U const r); };
    
    // Safely logical inclusive OR.
    template <class T, class U, RelType> class SafeLogIOr;
    template <class T, class U> class SafeLogIOr<T, U, SE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, SE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, SS_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, SS_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, SL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, SL_US> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, UE_UE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, UE_SE> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, US_SL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, US_UL> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, UL_SS> { public: static T op (T const l, U const r); };
    template <class T, class U> class SafeLogIOr<T, U, UL_US> { public: static T op (T const l, U const r); };
    
  }
  
  /***************************************************************************
   * SafeInt
   ***************************************************************************
   * This is an overflow safe replacement for any integer type.
   ***************************************************************************/
  template <class T> class SafeInt {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Function members.
      
      // Default constructor.
      explicit SafeInt (T const value = 0);
      
      // Copy constructor.
      SafeInt (SafeInt<T> const& value);
      
      // Conversion constructor.
      template <class U> explicit SafeInt (U          const  value);
      template <class U> explicit SafeInt (SafeInt<U> const& value);
      
      // Increment / decrement operators.
      SafeInt& operator ++ ();
      SafeInt  operator ++ (int);
      SafeInt& operator -- ();
      SafeInt  operator -- (int);
      
      // Sign operators.
      SafeInt operator + () const;
      SafeInt operator - () const;
      
      // Not operator.
      bool operator ! () const;
      
      // Bitwise compliment.
      SafeInt operator ~ () const;
      
      // Casting operators.
      operator bool                   () const;
      operator signed   char          () const;
      operator unsigned char          () const;
      operator signed   short int     () const;
      operator unsigned short int     () const;
      operator signed   int           () const;
      operator unsigned int           () const;
      operator signed   long int      () const;
      operator unsigned long int      () const;
      operator signed   long long int () const;
      operator unsigned long long int () const;
      
      // Assignment operator.
                         SafeInt& operator = (SafeInt<T> const& value);
                         SafeInt& operator = (T          const  value);
      template <class U> SafeInt& operator = (SafeInt<U> const& value);
      template <class U> SafeInt& operator = (U          const  value);
      
      // Arithmetic assignment operators.
      template <class U> SafeInt& operator *= (SafeInt<U> const& value);
      template <class U> SafeInt& operator *= (U          const  value);
      template <class U> SafeInt& operator /= (SafeInt<U> const& value);
      template <class U> SafeInt& operator /= (U          const  value);
      template <class U> SafeInt& operator %= (SafeInt<U> const& value);
      template <class U> SafeInt& operator %= (U          const  value);
      template <class U> SafeInt& operator += (SafeInt<U> const& value);
      template <class U> SafeInt& operator += (U          const  value);
      template <class U> SafeInt& operator -= (SafeInt<U> const& value);
      template <class U> SafeInt& operator -= (U          const  value);
      
      // Bit shift assignment operators.
      template <class U> SafeInt& operator <<= (SafeInt<U> const& value);
      template <class U> SafeInt& operator <<= (U          const  value);
      template <class U> SafeInt& operator >>= (SafeInt<U> const& value);
      template <class U> SafeInt& operator >>= (U          const  value);
      
      // Bitwise assignment operators.
      template <class U> SafeInt& operator &= (SafeInt<U> const& value);
      template <class U> SafeInt& operator &= (U          const  value);
      template <class U> SafeInt& operator |= (SafeInt<U> const& value);
      template <class U> SafeInt& operator |= (U          const  value);
      template <class U> SafeInt& operator ^= (SafeInt<U> const& value);
      template <class U> SafeInt& operator ^= (U          const  value);
      
      // Arithmetic operator backends.
      template <class U> SafeInt& op_mul (SafeInt<U> const& value);
      template <class U> SafeInt& op_mul (U          const  value);
      template <class U> SafeInt& op_div (SafeInt<U> const& value);
      template <class U> SafeInt& op_div (U          const  value);
      template <class U> SafeInt& op_mod (SafeInt<U> const& value);
      template <class U> SafeInt& op_mod (U          const  value);
      template <class U> SafeInt& op_add (SafeInt<U> const& value);
      template <class U> SafeInt& op_add (U          const  value);
      template <class U> SafeInt& op_sub (SafeInt<U> const& value);
      template <class U> SafeInt& op_sub (U          const  value);
      
      // Bit shift operator backends.
      template <class U> SafeInt& op_shl (SafeInt<U> const& value);
      template <class U> SafeInt& op_shl (U          const  value);
      template <class U> SafeInt& op_shr (SafeInt<U> const& value);
      template <class U> SafeInt& op_shr (U          const  value);
      
      // Comparison operator backends.
      template <class U> bool op_gt (SafeInt<U> const& value) const;
      template <class U> bool op_gt (U          const  value) const;
      template <class U> bool op_ge (SafeInt<U> const& value) const;
      template <class U> bool op_ge (U          const  value) const;
      template <class U> bool op_lt (SafeInt<U> const& value) const;
      template <class U> bool op_lt (U          const  value) const;
      template <class U> bool op_le (SafeInt<U> const& value) const;
      template <class U> bool op_le (U          const  value) const;
      template <class U> bool op_eq (SafeInt<U> const& value) const;
      template <class U> bool op_eq (U          const  value) const;
      template <class U> bool op_ne (SafeInt<U> const& value) const;
      template <class U> bool op_ne (U          const  value) const;
      
      // Bitwise operator backends.
      template <class U> SafeInt& op_bit_and (SafeInt<U> const& value);
      template <class U> SafeInt& op_bit_and (U          const  value);
      template <class U> SafeInt& op_bit_ior (SafeInt<U> const& value);
      template <class U> SafeInt& op_bit_ior (U          const  value);
      template <class U> SafeInt& op_bit_xor (SafeInt<U> const& value);
      template <class U> SafeInt& op_bit_xor (U          const  value);
                         SafeInt& op_bit_cpm ();
      
      // Logical operator backends.
      template <class U> bool op_log_and (SafeInt<U> const& value) const;
      template <class U> bool op_log_and (U          const  value) const;
      template <class U> bool op_log_ior (SafeInt<U> const& value) const;
      template <class U> bool op_log_ior (U          const  value) const;
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data members.
      
      // This is the number all this trouble is for.
      T _value;
      
  };
  
  /***************************************************************************
   * SafeInt Exceptions.
   ***************************************************************************/
  namespace SafeIntErrors {
    
    // All SafeInt errors are based off of this.
    class Base : public Exception {};
    
    // Overflow.
    class Overflow : public Base {
      public:
        virtual char const* what () const throw();
        virtual ~Overflow () throw();
        std::string message;
    };
    
    // Overflow resulting from a cast.
    class CastOverflow : public Overflow {
      public:
        template <class T, class U> CastOverflow (T const num, U const limit) throw();
    };
    
    // Overflow in a binary operation.
    class BinOpOverflow : public Overflow {
      public:
        template <class T, class U> BinOpOverflow (T const l, char const* const op, U const r, T const limit) throw();
    };
    
    // Divide by zero.
    class DivByZero : public Base {
      public:
        template <class T, class U> DivByZero (T const l, char const* const op, U const r) throw();
        virtual char const* what () const throw();
        virtual ~DivByZero () throw();
        std::string message;
    };
    
    // Result of operation is undefined.
    class Undefined : public Base {
      public:
        virtual char const* what () const throw();
        virtual ~Undefined () throw();
        std::string message;
    };
    
    // Undefined binary operation.
    class BinOpUndefined : public Undefined {
      public:
        template <class T, class U> BinOpUndefined (T const l, char const* const op, U const r) throw ();
    };
    
  }
  
}

/*****************************************************************************
 * Global operators.
 *****************************************************************************/

// Stream I/O operators.
template <class T> std::ostream& operator << (std::ostream& l, DAC::SafeInt<T> const& r);
template <class T> std::istream& operator >> (std::istream& l, DAC::SafeInt<T>&       r);

// Arithmetic operators.
template <class T, class U> DAC::SafeInt<T> operator * (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator * (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator * (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator / (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator / (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator / (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator % (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator % (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator % (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator + (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator + (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator + (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator - (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator - (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator - (T               const  l, DAC::SafeInt<U> const& r);

// Bit shift operators.
template <class T, class U> DAC::SafeInt<T> operator << (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator << (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator << (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator >> (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator >> (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator >> (T               const  l, DAC::SafeInt<U> const& r);

// Comparison operators.
template <class T, class U> bool operator >  (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator >  (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator >  (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator >= (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator >= (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator >= (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator <  (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator <  (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator <  (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator <= (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator <= (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator <= (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator == (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator == (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator == (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator != (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> bool operator != (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> bool operator != (T               const  l, DAC::SafeInt<U> const& r);

// Bitwise operators.
template <class T, class U> DAC::SafeInt<T> operator & (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator & (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator & (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator | (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator | (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator | (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator ^ (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator ^ (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator ^ (T               const  l, DAC::SafeInt<U> const& r);

// Logical operators.
template <class T, class U> DAC::SafeInt<T> operator && (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator && (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator && (T               const  l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator || (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r);
template <class T, class U> DAC::SafeInt<T> operator || (DAC::SafeInt<T> const& l, U               const  r);
template <class T, class U> DAC::SafeInt<T> operator || (T               const  l, DAC::SafeInt<U> const& r);

/*****************************************************************************
 * Inline and template definitions.
 *****************************************************************************/

namespace DAC {
  
  /***************************************************************************
   * Class SafeInt.
   ***************************************************************************/
  
  /***************************************************************************/
  // Function members.
  
  // Default constructor.
  template <class T> inline SafeInt<T>::SafeInt (T const value) { _value = value; }
  
  // Copy constructor.
  template <class T> inline SafeInt<T>::SafeInt (SafeInt<T> const& value) { _value = value._value; }
  
  // Conversion constructor.
  template <class T> template <class U> inline SafeInt<T>::SafeInt (U          const  value) { _value = SafeIntUtil::SafeCast<U, T, SafeIntUtil::Relationship<U, T>::value>::op(value); }
  template <class T> template <class U> inline SafeInt<T>::SafeInt (SafeInt<U> const& value) { _value = static_cast<T>(value);                                                          }
  
  // Increment / decrement operators.
  template <class T> inline SafeInt<T>& SafeInt<T>::operator ++ ()    { return op_add(1);                                   }
  template <class T> inline SafeInt<T>  SafeInt<T>::operator ++ (int) { SafeInt<T> retval(*this); op_add(1); return retval; }
  template <class T> inline SafeInt<T>& SafeInt<T>::operator -- ()    { return op_sub(1);                                   }
  template <class T> inline SafeInt<T>  SafeInt<T>::operator -- (int) { SafeInt<T> retval(*this); op_sub(1); return retval; }
  
  // Sign operators.
  template <class T> inline SafeInt<T> SafeInt<T>::operator + () const { return *this;      }
  template <class T> inline SafeInt<T> SafeInt<T>::operator - () const { return *this * -1; }
  
  // Not operator.
  template <class T> inline bool SafeInt<T>::operator ! () const { return !_value; }
  
  // Bitwise compliment.
  template <class T> inline SafeInt<T> SafeInt<T>::operator ~ () const { return SafeInt<T>(*this).op_bit_cpm(); }
  
  // Casting operators.
  template <class T> inline SafeInt<T>::operator bool                   () const { return _value != static_cast<T>(0);                                                                                               }
  template <class T> inline SafeInt<T>::operator signed   char          () const { return SafeIntUtil::SafeCast<T, signed   char,          SafeIntUtil::Relationship<T, signed   char         >::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator unsigned char          () const { return SafeIntUtil::SafeCast<T, unsigned char,          SafeIntUtil::Relationship<T, unsigned char         >::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator signed   short int     () const { return SafeIntUtil::SafeCast<T, signed   short int,     SafeIntUtil::Relationship<T, signed   short int    >::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator unsigned short int     () const { return SafeIntUtil::SafeCast<T, unsigned short int,     SafeIntUtil::Relationship<T, unsigned short int    >::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator signed   int           () const { return SafeIntUtil::SafeCast<T, signed   int,           SafeIntUtil::Relationship<T, signed   int          >::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator unsigned int           () const { return SafeIntUtil::SafeCast<T, unsigned int,           SafeIntUtil::Relationship<T, unsigned int          >::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator signed   long int      () const { return SafeIntUtil::SafeCast<T, signed   long int,      SafeIntUtil::Relationship<T, signed   long int     >::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator unsigned long int      () const { return SafeIntUtil::SafeCast<T, unsigned long int,      SafeIntUtil::Relationship<T, unsigned long int     >::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator signed   long long int () const { return SafeIntUtil::SafeCast<T, signed   long long int, SafeIntUtil::Relationship<T, signed   long long int>::value>::op(_value); }
  template <class T> inline SafeInt<T>::operator unsigned long long int () const { return SafeIntUtil::SafeCast<T, unsigned long long int, SafeIntUtil::Relationship<T, unsigned long long int>::value>::op(_value); }
  
  // Assignment operator.
  template <class T>                    inline SafeInt<T>& SafeInt<T>::operator = (SafeInt<T> const& value) { _value = value._value;                                                                   return *this; }
  template <class T>                    inline SafeInt<T>& SafeInt<T>::operator = (T          const  value) { _value = value;                                                                          return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator = (SafeInt<U> const& value) { _value = static_cast<T>(value);                                                          return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator = (U          const  value) { _value = SafeIntUtil::SafeCast<U, T, SafeIntUtil::Relationship<U, T>::value>::op(value); return *this; }
  
  // Arithmetic assignment operators.
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator *= (SafeInt<U> const& value) { return op_mul(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator *= (U          const  value) { return op_mul(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator /= (SafeInt<U> const& value) { return op_div(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator /= (U          const  value) { return op_div(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator %= (SafeInt<U> const& value) { return op_mod(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator %= (U          const  value) { return op_mod(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator += (SafeInt<U> const& value) { return op_add(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator += (U          const  value) { return op_add(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator -= (SafeInt<U> const& value) { return op_sub(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator -= (U          const  value) { return op_sub(value); }
  
  // Bit shift assignment operators.
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator <<= (SafeInt<U> const& value) { return op_shl(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator <<= (U          const  value) { return op_shl(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator >>= (SafeInt<U> const& value) { return op_shr(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator >>= (U          const  value) { return op_shr(value); }
  
  // Bitwise assignment operators.
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator &= (SafeInt<U> const& value) { return op_bit_and(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator &= (U          const  value) { return op_bit_and(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator |= (SafeInt<U> const& value) { return op_bit_ior(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator |= (U          const  value) { return op_bit_ior(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator ^= (SafeInt<U> const& value) { return op_bit_xor(value); }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::operator ^= (U          const  value) { return op_bit_xor(value); }
  
  // Arithmetic operator backends.
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_mul (SafeInt<U> const& value) { _value = SafeIntUtil::SafeMul<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_mul (U          const  value) { _value = SafeIntUtil::SafeMul<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_div (SafeInt<U> const& value) { _value = SafeIntUtil::SafeDiv<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_div (U          const  value) { _value = SafeIntUtil::SafeDiv<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_mod (SafeInt<U> const& value) { _value = SafeIntUtil::SafeMod<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_mod (U          const  value) { _value = SafeIntUtil::SafeMod<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_add (SafeInt<U> const& value) { _value = SafeIntUtil::SafeAdd<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_add (U          const  value) { _value = SafeIntUtil::SafeAdd<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_sub (SafeInt<U> const& value) { _value = SafeIntUtil::SafeSub<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_sub (U          const  value) { _value = SafeIntUtil::SafeSub<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  
  // Bit shift operator backends.
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_shl (SafeInt<U> const& value) { _value = SafeIntUtil::SafeShL<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_shl (U          const  value) { _value = SafeIntUtil::SafeShL<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_shr (SafeInt<U> const& value) { _value = SafeIntUtil::SafeShR<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_shr (U          const  value) { _value = SafeIntUtil::SafeShR<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  
  // Comparison operator backends.
  template <class T> template <class U> inline bool SafeInt<T>::op_gt (SafeInt<U> const& value) const { return SafeIntUtil::SafeGT<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); }
  template <class T> template <class U> inline bool SafeInt<T>::op_gt (U          const  value) const { return SafeIntUtil::SafeGT<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 }
  template <class T> template <class U> inline bool SafeInt<T>::op_ge (SafeInt<U> const& value) const { return SafeIntUtil::SafeGE<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); }
  template <class T> template <class U> inline bool SafeInt<T>::op_ge (U          const  value) const { return SafeIntUtil::SafeGE<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 }
  template <class T> template <class U> inline bool SafeInt<T>::op_lt (SafeInt<U> const& value) const { return SafeIntUtil::SafeLT<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); }
  template <class T> template <class U> inline bool SafeInt<T>::op_lt (U          const  value) const { return SafeIntUtil::SafeLT<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 }
  template <class T> template <class U> inline bool SafeInt<T>::op_le (SafeInt<U> const& value) const { return SafeIntUtil::SafeLE<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); }
  template <class T> template <class U> inline bool SafeInt<T>::op_le (U          const  value) const { return SafeIntUtil::SafeLE<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 }
  template <class T> template <class U> inline bool SafeInt<T>::op_eq (SafeInt<U> const& value) const { return SafeIntUtil::SafeEQ<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); }
  template <class T> template <class U> inline bool SafeInt<T>::op_eq (U          const  value) const { return SafeIntUtil::SafeEQ<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 }
  template <class T> template <class U> inline bool SafeInt<T>::op_ne (SafeInt<U> const& value) const { return SafeIntUtil::SafeNE<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); }
  template <class T> template <class U> inline bool SafeInt<T>::op_ne (U          const  value) const { return SafeIntUtil::SafeNE<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 }
  
  // Bitwise operator backends.
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_bit_and (SafeInt<U> const& value) { _value = SafeIntUtil::SafeBitAnd<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_bit_and (U          const  value) { _value = SafeIntUtil::SafeBitAnd<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_bit_ior (SafeInt<U> const& value) { _value = SafeIntUtil::SafeBitIOr<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_bit_ior (U          const  value) { _value = SafeIntUtil::SafeBitIOr<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_bit_xor (SafeInt<U> const& value) { _value = SafeIntUtil::SafeBitXOr<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); return *this; }
  template <class T> template <class U> inline SafeInt<T>& SafeInt<T>::op_bit_xor (U          const  value) { _value = SafeIntUtil::SafeBitXOr<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 return *this; }
  template <class T>                    inline SafeInt<T>& SafeInt<T>::op_bit_cpm ()                        { _value = SafeIntUtil::SafeBitCpm<T>                                           ::op(_value);                        return *this; }
  
  // Logical operator backends.
  template <class T> template <class U> inline bool SafeInt<T>::op_log_and (SafeInt<U> const& value) const { return SafeIntUtil::SafeLogAnd<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); }
  template <class T> template <class U> inline bool SafeInt<T>::op_log_and (U          const  value) const { return SafeIntUtil::SafeLogAnd<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 }
  template <class T> template <class U> inline bool SafeInt<T>::op_log_ior (SafeInt<U> const& value) const { return SafeIntUtil::SafeLogIOr<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, static_cast<U>(value)); }
  template <class T> template <class U> inline bool SafeInt<T>::op_log_ior (U          const  value) const { return SafeIntUtil::SafeLogIOr<T, U, SafeIntUtil::Relationship<T, U>::value>::op(_value, value);                 }
  
  /***************************************************************************
   * SafeIntUtil.
   ***************************************************************************/
  namespace SafeIntUtil {
    
    /*************************************************************************
     * Class SafeCaster.
     *************************************************************************/
    
    // Cast.
    template <class T, class U> inline U SafeCast<T, U, SE_SE>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, SE_UE>::op (T const value) {
      if (value < static_cast<T>(0)) {
        throw SafeIntErrors::CastOverflow(value, static_cast<U>(0));
      }
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, SS_SL>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, SS_UL>::op (T const value) {
      if (value < static_cast<T>(0)) {
        throw SafeIntErrors::CastOverflow(value, static_cast<U>(0));
      }
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, SL_SS>::op (T const value) {
      if (value < static_cast<T>(std::numeric_limits<U>::min())) {
        throw SafeIntErrors::CastOverflow(value, std::numeric_limits<U>::min());
      }
      if (value > static_cast<T>(std::numeric_limits<U>::max())) {
        throw SafeIntErrors::CastOverflow(value, std::numeric_limits<U>::max());
      }
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, SL_US>::op (T const value) {
      if (value < static_cast<T>(0)) {
        throw SafeIntErrors::CastOverflow(value, static_cast<U>(0));
      }
      if (value > static_cast<T>(std::numeric_limits<U>::max())) {
        throw SafeIntErrors::CastOverflow(value, std::numeric_limits<U>::max());
      }
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, UE_UE>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, UE_SE>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, US_SL>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, US_UL>::op (T const value) {
      if (value > static_cast<T>(std::numeric_limits<U>::max())) {
        throw SafeIntErrors::CastOverflow(value, std::numeric_limits<U>::max());
      }
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, UL_SS>::op (T const value) {
      if (value > static_cast<T>(std::numeric_limits<U>::max())) {
        throw SafeIntErrors::CastOverflow(value, std::numeric_limits<U>::max());
      }
      return static_cast<U>(value);
    }
    template <class T, class U> inline U SafeCast<T, U, UL_US>::op (T const value) {
      if (value > static_cast<T>(std::numeric_limits<U>::max())) {
        throw SafeIntErrors::CastOverflow(value, std::numeric_limits<U>::max());
      }
      return static_cast<U>(value);
    }
    
    // Raw cast.
    template <class T, class U> U RawCast<T, U, SE_SE>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> U RawCast<T, U, SE_UE>::op (T const value) {
      return static_cast<U>(value & std::numeric_limits<T>::max());
    }
    template <class T, class U> U RawCast<T, U, SS_SL>::op (T const value) {
      if (value < static_cast<T>(0)) {
        return static_cast<U>(value & std::numeric_limits<T>::max()) | static_cast<U>(1) << std::numeric_limits<T>::digits;
      } else {
        return static_cast<U>(value);
      }
    }
    template <class T, class U> U RawCast<T, U, SS_UL>::op (T const value) {
      if (value < static_cast<T>(0)) {
        return static_cast<U>(value & std::numeric_limits<T>::max()) | static_cast<U>(1) << std::numeric_limits<T>::digits;
      } else {
        return static_cast<U>(value);
      }
    }
    template <class T, class U> U RawCast<T, U, SL_SS>::op (T const value) {
      return static_cast<U>(value & static_cast<T>(std::numeric_limits<U>::max())) | static_cast<U>(static_cast<T>(1) << std::numeric_limits<U>::digits & value);
    }
    template <class T, class U> U RawCast<T, U, SL_US>::op (T const value) {
      return static_cast<U>(value & static_cast<T>(std::numeric_limits<U>::max()));
    }
    template <class T, class U> U RawCast<T, U, UE_UE>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> U RawCast<T, U, UE_SE>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> U RawCast<T, U, US_SL>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> U RawCast<T, U, US_UL>::op (T const value) {
      return static_cast<U>(value);
    }
    template <class T, class U> U RawCast<T, U, UL_SS>::op (T const value) {
      return static_cast<U>(value & static_cast<T>(std::numeric_limits<U>::max())) | static_cast<U>(static_cast<T>(1) << std::numeric_limits<U>::digits & value);
    }
    template <class T, class U> U RawCast<T, U, UL_US>::op (T const value) {
      return static_cast<U>(value & static_cast<T>(std::numeric_limits<U>::max()));
    }
    
    // Multiply.
    template <class T, class U> T SafeMul<T, U, SE_SE>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r == static_cast<U>(-1)) {
        if (l == std::numeric_limits<T>::min()) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        return (l * static_cast<T>(-1));
      }
      if (l == static_cast<T>(1)) {
        return static_cast<T>(r);
      }
      if (l == static_cast<T>(-1)) {
        if (r == static_cast<U>(std::numeric_limits<T>::min())) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(r * static_cast<U>(-1));
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          if (r > static_cast<U>(std::numeric_limits<T>::max() / l)) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
          }
        } else {
          if (r < static_cast<U>(std::numeric_limits<T>::min() / l)) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
          }
        }
      } else {
        if (r > static_cast<U>(0)) {
          if (r > static_cast<U>(std::numeric_limits<T>::min() / l)) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
          }
        } else {
          if (r < static_cast<U>(std::numeric_limits<T>::max() / l)) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
          }
        }
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, SE_UE>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (l == static_cast<T>(1)) {
        return static_cast<T>(r);
      }
      if (l == static_cast<T>(-1)) {
        return static_cast<T>(r * static_cast<U>(-1));
      }
      if (l > static_cast<T>(0)) {
        if (static_cast<T>(r) > std::numeric_limits<T>::max() / l) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
      } else {
        if (static_cast<T>(r) > std::numeric_limits<T>::min() / l) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
        }
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, SS_SL>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r == static_cast<U>(-1)) {
        if (l == std::numeric_limits<T>::min()) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        return (l * static_cast<T>(-1));
      }
      if (l == static_cast<T>(1)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        if (r < static_cast<U>(std::numeric_limits<T>::min())) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
        }
        return static_cast<T>(r);
      }
      if (l == static_cast<T>(-1)) {
        if (r <= static_cast<U>(std::numeric_limits<T>::min())) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        if (r > (static_cast<U>(std::numeric_limits<T>::min()) * static_cast<U>(-1))) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
        }
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          if (r > static_cast<U>(std::numeric_limits<T>::max() / l)) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
          }
        } else {
          if (r < static_cast<U>(std::numeric_limits<T>::min() / l)) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
          }
        }
      } else {
        if (r > static_cast<U>(0)) {
          if (r > static_cast<U>(std::numeric_limits<T>::min() / l)) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
          }
        } else {
          if (r < static_cast<U>(std::numeric_limits<T>::max() / l)) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
          }
        }
      }
      return static_cast<T>(static_cast<U>(l) * r);
    }
    template <class T, class U> T SafeMul<T, U, SS_UL>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (l == static_cast<T>(1)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(r);
      }
      if (l == static_cast<T>(-1)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max()) + static_cast<U>(1)) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
        }
        if (r == static_cast<U>(std::numeric_limits<T>::max()) + static_cast<U>(1)) {
          return std::numeric_limits<T>::min();
        } else {
          return (static_cast<T>(r) * static_cast<T>(-1));
        }
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max() / l)) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
      } else {
        if (r > static_cast<U>(std::numeric_limits<T>::min() / l)) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
        }
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, SL_SS>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r == static_cast<U>(-1)) {
        if (l == std::numeric_limits<T>::min()) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        return (l * static_cast<T>(r));
      }
      if (l == static_cast<T>(1)) {
        return static_cast<T>(r);
      }
      if (l == static_cast<T>(-1)) {
        return (static_cast<T>(r) * static_cast<T>(-1));
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<T>(0)) {
          if (static_cast<T>(r) > std::numeric_limits<T>::max() / l) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
          }
        } else {
          if (static_cast<T>(r) < std::numeric_limits<T>::min() / l) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
          }
        }
      } else {
        if (r > static_cast<T>(0)) {
          if (static_cast<T>(r) > std::numeric_limits<T>::min() / l) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
          }
        } else {
          if (static_cast<T>(r) < std::numeric_limits<T>::max() / l) {
            throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
          }
        }
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, SL_US>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (l == static_cast<T>(1)) {
        return static_cast<T>(r);
      }
      if (l == static_cast<T>(-1)) {
        return (static_cast<T>(r) * static_cast<T>(-1));
      }
      if (l > static_cast<T>(0)) {
        if (r > std::numeric_limits<T>::max() / l) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
      } else {
        if (r > std::numeric_limits<T>::min() / l) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
        }
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, UE_UE>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (l == static_cast<T>(1)) {
        return static_cast<T>(r);
      }
      if (static_cast<T>(r) > std::numeric_limits<T>::max() / l) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, UE_SE>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
      }
      if (l == static_cast<T>(1)) {
        return static_cast<T>(r);
      }
      if (r > static_cast<U>(std::numeric_limits<T>::max() / l)) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, US_SL>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
      }
      if (l == static_cast<T>(1)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(r);
      }
      if (r > static_cast<U>(std::numeric_limits<T>::max() / l)) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, US_UL>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (l == static_cast<T>(1)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(r);
      }
      if (r > static_cast<U>(std::numeric_limits<T>::max() / l)) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, UL_SS>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::min());
      }
      if (l == static_cast<T>(1)) {
        return static_cast<T>(r);
      }
      if (static_cast<T>(r) > std::numeric_limits<T>::max() / l) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
      }
      return (l * static_cast<T>(r));
    }
    template <class T, class U> T SafeMul<T, U, UL_US>::op (T const l, U const r) {
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(0))) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (l == static_cast<T>(1)) {
        return static_cast<T>(r);
      }
      if (static_cast<T>(r) > std::numeric_limits<T>::max() / l) {
        throw SafeIntErrors::BinOpOverflow(l, "*", r, std::numeric_limits<T>::max());
      }
      return (l * static_cast<T>(r));
    }
    
    // Divide.
    template <class T, class U> T SafeDiv<T, U, SE_SE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if ((l == std::numeric_limits<T>::min()) && (r == static_cast<U>(-1))) {
        throw SafeIntErrors::BinOpOverflow(l, "/", r, std::numeric_limits<T>::max());
      }
      return (l / static_cast<T>(r));
    }
    template <class T, class U> T SafeDiv<T, U, SE_UE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      return (l / static_cast<T>(r));
    }
    template <class T, class U> T SafeDiv<T, U, SS_SL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if ((l == std::numeric_limits<T>::min()) && (r == static_cast<U>(-1))) {
        throw SafeIntErrors::BinOpOverflow(l, "/", r, std::numeric_limits<T>::max());
      }
      return static_cast<T>(static_cast<U>(l) / r);
    }
    template <class T, class U> T SafeDiv<T, U, SS_UL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (l > 0) {
        return static_cast<T>(static_cast<U>(l) / r);
      } else {
        return (static_cast<T>((static_cast<U>((l + 1) * static_cast<T>(-1)) + static_cast<U>(1)) / r) * static_cast<T>(-1));
      }
    }
    template <class T, class U> T SafeDiv<T, U, SL_SS>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if ((l == std::numeric_limits<T>::min()) && (r == static_cast<U>(-1))) {
        throw SafeIntErrors::BinOpOverflow(l, "/", r, std::numeric_limits<T>::max());
      }
      return (l / static_cast<T>(r));
    }
    template <class T, class U> T SafeDiv<T, U, SL_US>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      return (l / static_cast<T>(r));
    }
    template <class T, class U> T SafeDiv<T, U, UE_UE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      return (l / static_cast<T>(r));
    }
    template <class T, class U> T SafeDiv<T, U, UE_SE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpOverflow(l, "/", r, static_cast<T>(0));
      }
      return (l / static_cast<T>(r));
    }
    template <class T, class U> T SafeDiv<T, U, US_SL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpOverflow(l, "/", r, static_cast<T>(0));
      }
      return static_cast<T>(static_cast<U>(l) / r);
    }
    template <class T, class U> T SafeDiv<T, U, US_UL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      return static_cast<T>(static_cast<U>(l) / r);
    }
    template <class T, class U> T SafeDiv<T, U, UL_SS>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpOverflow(l, "/", r, static_cast<T>(0));
      }
      return (l / static_cast<T>(r));
    }
    template <class T, class U> T SafeDiv<T, U, UL_US>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "/", r);
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0);
      }
      if (r == static_cast<U>(1)) {
        return l;
      }
      return (l / static_cast<T>(r));
    }
    
    // Modulo divide
    template <class T, class U> T SafeMod<T, U, SE_SE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return (l % static_cast<T>(r));
    }
    template <class T, class U> T SafeMod<T, U, SE_UE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return (l % static_cast<T>(r));
    }
    template <class T, class U> T SafeMod<T, U, SS_SL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return static_cast<T>(static_cast<U>(l) % r);
    }
    template <class T, class U> T SafeMod<T, U, SS_UL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      if (l > static_cast<T>(0)) {
        return static_cast<T>(static_cast<U>(l) % r);
      } else {
        return (static_cast<T>((static_cast<U>((l + 1) * static_cast<T>(-1)) + static_cast<U>(1)) % r) * static_cast<T>(-1));
      }
    }
    template <class T, class U> T SafeMod<T, U, SL_SS>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return (l % static_cast<T>(r));
    }
    template <class T, class U> T SafeMod<T, U, SL_US>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return (l % static_cast<T>(r));
    }
    template <class T, class U> T SafeMod<T, U, UE_UE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return (l % static_cast<T>(r));
    }
    template <class T, class U> T SafeMod<T, U, UE_SE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return static_cast<T>(static_cast<U>(l) % r);
    }
    template <class T, class U> T SafeMod<T, U, US_SL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return static_cast<T>(static_cast<U>(l) % r);
    }
    template <class T, class U> T SafeMod<T, U, US_UL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return static_cast<T>(static_cast<U>(l) % r);
    }
    template <class T, class U> T SafeMod<T, U, UL_SS>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return (l % static_cast<T>(std::abs(r)));
    }
    template <class T, class U> T SafeMod<T, U, UL_US>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        throw SafeIntErrors::DivByZero(l, "%", r);
      }
      if ((l == static_cast<T>(0)) || (r == static_cast<U>(1))) {
        return static_cast<T>(0);
      }
      return (l % static_cast<T>(r));
    }
    
    // Add.
    template <class T, class U> T SafeAdd<T, U, SE_SE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(r);
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          if (static_cast<T>(r) > std::numeric_limits<T>::max() - l) {
            throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
          }
        }
      } else {
        if (r < static_cast<U>(0)) {
          if (static_cast<T>(r) < std::numeric_limits<T>::min() - l) {
            throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::min());
          }
        }
      }
      return (l + static_cast<T>(r));
    }
    template <class T, class U> T SafeAdd<T, U, SE_UE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(r);
      }
      if (l > static_cast<T>(0)) {
        if (static_cast<T>(r) > std::numeric_limits<T>::max() - l) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
      }
      return (l + static_cast<T>(r));
    }
    template <class T, class U> T SafeAdd<T, U, SS_SL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
        if (r < static_cast<U>(std::numeric_limits<T>::min())) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::min());
        }
        return static_cast<T>(r);
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          if (r > static_cast<U>(std::numeric_limits<T>::max() - l)) {
            throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
          }
        } else {
          if (r < (static_cast<U>(l) * static_cast<U>(-1))) {
            if (r - static_cast<U>(l) < static_cast<U>(std::numeric_limits<T>::min())) {
              throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::min());
            }
          }
        }
      } else {
        if (r > static_cast<U>(0)) {
          if (r > (static_cast<U>(l) * static_cast<U>(-1))) {
            if (r + static_cast<U>(l) > static_cast<U>(std::numeric_limits<T>::max())) {
              throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
            }
          }
        } else {
          if (r < static_cast<U>(std::numeric_limits<T>::min() - l)) {
            throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::min());
          }
        }
      }
      return static_cast<T>(static_cast<U>(l) + r);
    }
    template <class T, class U> T SafeAdd<T, U, SS_UL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(r);
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max() - l)) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(static_cast<U>(l) + r);
      } else {
        U tmp(static_cast<U>((l + static_cast<T>(1)) * static_cast<T>(-1)) + static_cast<U>(1));
        if (r > tmp) {
          if (r - tmp > static_cast<U>(std::numeric_limits<T>::max())) {
            throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
          }
          return static_cast<T>(r - tmp);
        } else {
          return (static_cast<T>(tmp - r - static_cast<U>(1)) * static_cast<T>(-1) - static_cast<T>(1));
        }
      }
    }
    template <class T, class U> T SafeAdd<T, U, SL_SS>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(r);
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          if (static_cast<T>(r) > std::numeric_limits<T>::max() - l) {
            throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
          }
        }
      } else {
        if (r < static_cast<U>(0)) {
          if (static_cast<T>(r) < std::numeric_limits<T>::min() - l) {
            throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::min());
          }
        }
      }
      return (l + static_cast<T>(r));
    }
    template <class T, class U> T SafeAdd<T, U, SL_US>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(r);
      }
      if (l > static_cast<T>(0)) {
        if (static_cast<T>(r) > std::numeric_limits<T>::max() - l) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
      }
      return (l + static_cast<T>(r));
    }
    template <class T, class U> T SafeAdd<T, U, UE_UE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(r);
      }
      if (static_cast<T>(r) > std::numeric_limits<T>::max() - l) {
        throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
      }
      return (l + static_cast<T>(r));
    }
    template <class T, class U> T SafeAdd<T, U, UE_SE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r < static_cast<U>(0)) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, static_cast<T>(0));
        }
        return static_cast<T>(r);
      }
      if (r > static_cast<U>(0)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max() - l)) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
      } else {
        if (r < static_cast<U>(l) * static_cast<U>(-1)) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, static_cast<T>(0));
        }
      }
      return static_cast<T>(static_cast<U>(l) + r);
    }
    template <class T, class U> T SafeAdd<T, U, US_SL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r < static_cast<U>(0)) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, static_cast<T>(0));
        }
        if (r > static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(r);
      }
      if (r > static_cast<U>(0)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max() - l)) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
      } else {
        if (r < static_cast<U>(l) * static_cast<U>(-1)) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, static_cast<T>(0));
        }
      }
      return static_cast<T>(static_cast<U>(l) + r);
    }
    template <class T, class U> T SafeAdd<T, U, US_UL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r > static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(r);
      }
      if (r > static_cast<U>(std::numeric_limits<T>::max() - l)) {
        throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
      }
      return static_cast<T>(static_cast<U>(l) + r);
    }
    template <class T, class U> T SafeAdd<T, U, UL_SS>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r < static_cast<U>(0)) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, static_cast<T>(0));
        }
        return static_cast<T>(r);
      }
      if (r > static_cast<U>(0)) {
        if (static_cast<T>(r) > std::numeric_limits<T>::max() - l) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
        }
        return (l + static_cast<T>(r));
      } else {
        T tmp(static_cast<T>((r + 1) * static_cast<U>(-1)) + static_cast<T>(1));
        if (tmp > l) {
          throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::min());
        }
        return (l - tmp);
      }
    }
    template <class T, class U> T SafeAdd<T, U, UL_US>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(r);
      }
      if (static_cast<T>(r) > std::numeric_limits<T>::max() - l) {
        throw SafeIntErrors::BinOpOverflow(l, "+", r, std::numeric_limits<T>::max());
      }
      return (l + static_cast<T>(r));
    }
    
    // Subtract.
    template <class T, class U> T SafeSub<T, U, SE_SE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r == static_cast<U>(std::numeric_limits<T>::min())) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(static_cast<U>(0) - r);
      }
      if (l > static_cast<T>(0)) {
        if (r < static_cast<U>(0)) {
          if (static_cast<T>(r) < std::numeric_limits<T>::max() - l * static_cast<T>(-1)) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
          }
        }
      } else {
        if (r > static_cast<U>(0)) {
          if (static_cast<T>(r) * static_cast<T>(-1) < std::numeric_limits<T>::min() - l) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
          }
        }
      }
      return (l - static_cast<T>(r));
    }
    template <class T, class U> T SafeSub<T, U, SE_UE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0) - static_cast<T>(r);
      }
      if (l < static_cast<T>(0)) {
        if (static_cast<T>(r) * static_cast<T>(-1) < std::numeric_limits<T>::min() - l) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
        }
      }
      return (l - static_cast<T>(r));
    }
    template <class T, class U> T SafeSub<T, U, SS_SL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r > 0) {
          if (r * static_cast<U>(-1) < static_cast<U>(std::numeric_limits<T>::min())) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
          }
        } else {
          if (r < static_cast<U>(std::numeric_limits<T>::max()) * static_cast<U>(-1)) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
          }
        }
        return static_cast<T>(static_cast<U>(0) - r);
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          if (r > static_cast<U>(l)) {
            if ((r - static_cast<U>(l)) * static_cast<U>(-1) < static_cast<U>(std::numeric_limits<T>::min())) {
              throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
            }
          }
        } else {
          if (r < static_cast<U>(std::numeric_limits<T>::max() - l) * static_cast<U>(-1)) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
          }
        }
      } else {
        if (r > static_cast<U>(0)) {
          if (r * static_cast<U>(-1) < static_cast<U>(std::numeric_limits<T>::min() - l)) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
          }
        } else {
          if (r < static_cast<U>(l)) {
            if ((r - static_cast<U>(l)) < static_cast<U>(std::numeric_limits<T>::max()) * static_cast<U>(-1)) {
              throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
            }
          }
        }
      }
      return static_cast<T>(static_cast<U>(l) - r);
    }
    template <class T, class U> T SafeSub<T, U, SS_UL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r > static_cast<U>((std::numeric_limits<T>::min() + static_cast<T>(1)) * static_cast<T>(-1)) + static_cast<U>(1)) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
        }
        return static_cast<T>(r - static_cast<U>(1)) * static_cast<T>(-1) - static_cast<T>(1);
      }
      if (l > static_cast<T>(0)) {
        if (r > static_cast<U>(l)) {
          if (r - static_cast<U>(l) > static_cast<U>(std::numeric_limits<T>::max()) + static_cast<U>(1)) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
          }
          return (static_cast<T>(r - static_cast<U>(l) - static_cast<U>(1)) * static_cast<T>(-1)) - static_cast<T>(1);
        } else {
          return static_cast<T>(static_cast<U>(l) - r);
        }
      } else {
        if (r > static_cast<U>((std::numeric_limits<T>::min() - l) * static_cast<T>(-1))) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
        }
        return static_cast<T>(static_cast<U>(l * static_cast<T>(-1)) + r - static_cast<U>(1)) * static_cast<T>(-1) - static_cast<T>(1);
      }
    }
    template <class T, class U> T SafeSub<T, U, SL_SS>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0) - static_cast<T>(r);
      }
      if (l > static_cast<T>(0)) {
        if (r < static_cast<U>(0)) {
          if (static_cast<T>(r) * static_cast<T>(-1) > std::numeric_limits<T>::max() - l) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
          }
        }
      } else {
        if (r > static_cast<U>(0)) {
          if (static_cast<T>(r) * static_cast<T>(-1) < std::numeric_limits<T>::min() - l) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
          }
        }
      }
      return l - static_cast<T>(r);
    }
    template <class T, class U> T SafeSub<T, U, SL_US>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        return static_cast<T>(0) - static_cast<T>(r);
      }
      if (l < static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          if (static_cast<T>(r) * static_cast<T>(-1) < std::numeric_limits<T>::min() - l) {
            throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::min());
          }
        }
      }
      return l - static_cast<T>(r);
    }
    template <class T, class U> T SafeSub<T, U, UE_UE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (r > static_cast<U>(l)) {
        throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
      }
      return l - static_cast<T>(r);
    }
    template <class T, class U> T SafeSub<T, U, UE_SE>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
        }
        if (r < static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
        }
        return static_cast<T>(static_cast<U>(0) - r);
      }
      if (r > static_cast<U>(0)) {
        if (r > static_cast<U>(l)) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
        }
      } else {
        if (r < static_cast<U>(std::numeric_limits<T>::max() - l) * static_cast<U>(-1)) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
        }
      }
      return static_cast<T>(static_cast<U>(l) - r);
    }
    template <class T, class U> T SafeSub<T, U, US_SL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
        }
        if (r < static_cast<U>(0) - static_cast<U>(std::numeric_limits<T>::max())) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
        }
        return static_cast<T>(static_cast<U>(0) - r);
      }
      if (r > static_cast<U>(0)) {
        if (r > static_cast<U>(l)) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
        }
      } else {
        if (r < static_cast<U>(0) - static_cast<U>(std::numeric_limits<T>::max() - l)) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
        }
      }
      return static_cast<T>(static_cast<U>(l) - r);
    }
    template <class T, class U> T SafeSub<T, U, US_UL>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (r > static_cast<U>(l)) {
        throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
      }
      return static_cast<T>(static_cast<U>(l) - r);
    }
    template <class T, class U> T SafeSub<T, U, UL_SS>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (l == static_cast<T>(0)) {
        if (r > static_cast<U>(0)) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
        }
        if (r == std::numeric_limits<U>::min()) {
          return static_cast<T>((std::numeric_limits<U>::min() + static_cast<U>(1)) * static_cast<U>(-1)) + static_cast<T>(1);
        } else {
          return static_cast<T>(r * static_cast<U>(-1));
        }
      }
      if (r > static_cast<U>(0)) {
        if (static_cast<T>(r) > l) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
        }
        return l - static_cast<T>(r);
      } else {
        T tmpnum = static_cast<T>((r + static_cast<U>(1)) * static_cast<U>(-1)) + static_cast<T>(1);
        if (tmpnum > std::numeric_limits<T>::max() - l) {
          throw SafeIntErrors::BinOpOverflow(l, "-", r, std::numeric_limits<T>::max());
        }
        return l + tmpnum;
      }
    }
    template <class T, class U> T SafeSub<T, U, UL_US>::op (T const l, U const r) {
      if (r == static_cast<U>(0)) {
        return l;
      }
      if (static_cast<T>(r) > l) {
        throw SafeIntErrors::BinOpOverflow(l, "-", r, static_cast<T>(0));
      }
      return l - static_cast<T>(r);
    }
    
    // Shift left.
    template <class T, class U> T SafeShL<T, U, SE_SE>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, SE_UE>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, SS_SL>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, SS_UL>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, SL_SS>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, SL_US>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, UE_UE>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, UE_SE>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, US_SL>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, US_UL>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, UL_SS>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    template <class T, class U> T SafeShL<T, U, UL_US>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, "<<", r);
      }
      return l << r;
    }
    
    // Shift right.
    template <class T, class U> T SafeShR<T, U, SE_SE>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) == std::numeric_limits<T>::digits + 1) {
        return static_cast<T>(0);
      }
      if (l < static_cast<T>(0)) {
        return (l & std::numeric_limits<T>::max()) >> r | static_cast<T>(1) << std::numeric_limits<T>::digits - r;
      } else {
        return l >> r;
      }
    }
    template <class T, class U> T SafeShR<T, U, SE_UE>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) == std::numeric_limits<T>::digits + 1) {
        return static_cast<T>(0);
      }
      if (l < static_cast<T>(0)) {
        return (l & std::numeric_limits<T>::max()) >> r | static_cast<T>(1) << std::numeric_limits<T>::digits - r;
      } else {
        return l >> r;
      }
    }
    template <class T, class U> T SafeShR<T, U, SS_SL>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) == std::numeric_limits<T>::digits + 1) {
        return static_cast<T>(0);
      }
      if (l < static_cast<T>(0)) {
        return (l & std::numeric_limits<T>::max()) >> r | static_cast<T>(1) << std::numeric_limits<T>::digits - r;
      } else {
        return l >> r;
      }
    }
    template <class T, class U> T SafeShR<T, U, SS_UL>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) == std::numeric_limits<T>::digits + 1) {
        return static_cast<T>(0);
      }
      if (l < static_cast<T>(0)) {
        return (l & std::numeric_limits<T>::max()) >> r | static_cast<T>(1) << std::numeric_limits<T>::digits - r;
      } else {
        return l >> r;
      }
    }
    template <class T, class U> T SafeShR<T, U, SL_SS>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) == std::numeric_limits<T>::digits + 1) {
        return static_cast<T>(0);
      }
      if (l < static_cast<T>(0)) {
        return (l & std::numeric_limits<T>::max()) >> r | static_cast<T>(1) << std::numeric_limits<T>::digits - r;
      } else {
        return l >> r;
      }
    }
    template <class T, class U> T SafeShR<T, U, SL_US>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits + 1) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) == std::numeric_limits<T>::digits + 1) {
        return static_cast<T>(0);
      }
      if (l < static_cast<T>(0)) {
        return (l & std::numeric_limits<T>::max()) >> r | static_cast<T>(1) << std::numeric_limits<T>::digits - r;
      } else {
        return l >> r;
      }
    }
    template <class T, class U> T SafeShR<T, U, UE_UE>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      return l >> r;
    }
    template <class T, class U> T SafeShR<T, U, UE_SE>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      return l >> r;
    }
    template <class T, class U> T SafeShR<T, U, US_SL>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      return l >> r;
    }
    template <class T, class U> T SafeShR<T, U, US_UL>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      return l >> r;
    }
    template <class T, class U> T SafeShR<T, U, UL_SS>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      return l >> r;
    }
    template <class T, class U> T SafeShR<T, U, UL_US>::op (T const l, U const r) {
      if (SafeInt<U>(r) > std::numeric_limits<T>::digits) {
        throw SafeIntErrors::BinOpUndefined(l, ">>", r);
      }
      return l >> r;
    }
    
    // Test for greater than.
    template <class T, class U> inline bool SafeGT<T, U, SE_SE>::op (T const l, U const r) {
      return l > static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeGT<T, U, SE_UE>::op (T const l, U const r) {
      return l > static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeGT<T, U, SS_SL>::op (T const l, U const r) {
      return static_cast<U>(l) > r;
    }
    template <class T, class U> inline bool SafeGT<T, U, SS_UL>::op (T const l, U const r) {
      if (l < static_cast<T>(0)) {
        return false;
      } else {
        return static_cast<U>(l) > r;
      }
    }
    template <class T, class U> inline bool SafeGT<T, U, SL_SS>::op (T const l, U const r) {
      return l > static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeGT<T, U, SL_US>::op (T const l, U const r) {
      return l > static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeGT<T, U, UE_UE>::op (T const l, U const r) {
      return l > static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeGT<T, U, UE_SE>::op (T const l, U const r) {
      return static_cast<U>(l) > r;
    }
    template <class T, class U> inline bool SafeGT<T, U, US_SL>::op (T const l, U const r) {
      return static_cast<U>(l) > r;
    }
    template <class T, class U> inline bool SafeGT<T, U, US_UL>::op (T const l, U const r) {
      return static_cast<U>(l) > r;
    }
    template <class T, class U> inline bool SafeGT<T, U, UL_SS>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        return true;
      } else {
        return l > static_cast<T>(r);
      }
    }
    template <class T, class U> inline bool SafeGT<T, U, UL_US>::op (T const l, U const r) {
      return l > static_cast<T>(r);
    }
    
    // Test for greater than or equal to.
    template <class T, class U> inline bool SafeGE<T, U, SE_SE>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, SE_UE>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, SS_SL>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, SS_UL>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, SL_SS>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, SL_US>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, UE_UE>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, UE_SE>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, US_SL>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, US_UL>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, UL_SS>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeGE<T, U, UL_US>::op (T const l, U const r) { return !SafeLT<T, U, Relationship<T, U>::value>::op(l, r); }
    
    // Test for less than.
    template <class T, class U> inline bool SafeLT<T, U, SE_SE>::op (T const l, U const r) {
      return l < static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeLT<T, U, SE_UE>::op (T const l, U const r) {
      return l < static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeLT<T, U, SS_SL>::op (T const l, U const r) {
      return static_cast<U>(l) < r;
    }
    template <class T, class U> inline bool SafeLT<T, U, SS_UL>::op (T const l, U const r) {
      if (l < static_cast<T>(0)) {
        return true;
      } else {
        return static_cast<U>(l) < r;
      }
    }
    template <class T, class U> inline bool SafeLT<T, U, SL_SS>::op (T const l, U const r) {
      return l < static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeLT<T, U, SL_US>::op (T const l, U const r) {
      return l < static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeLT<T, U, UE_UE>::op (T const l, U const r) {
      return l < static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeLT<T, U, UE_SE>::op (T const l, U const r) {
      return static_cast<U>(l) < r;
    }
    template <class T, class U> inline bool SafeLT<T, U, US_SL>::op (T const l, U const r) {
      return static_cast<U>(l) < r;
    }
    template <class T, class U> inline bool SafeLT<T, U, US_UL>::op (T const l, U const r) {
      return static_cast<U>(l) < r;
    }
    template <class T, class U> inline bool SafeLT<T, U, UL_SS>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        return false;
      } else {
        return l < static_cast<T>(r);
      }
    }
    template <class T, class U> inline bool SafeLT<T, U, UL_US>::op (T const l, U const r) {
      return l < static_cast<T>(r);
    }
    
    // Test for less than or equal to.
    template <class T, class U> inline bool SafeLE<T, U, SE_SE>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, SE_UE>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, SS_SL>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, SS_UL>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, SL_SS>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, SL_US>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, UE_UE>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, UE_SE>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, US_SL>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, US_UL>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, UL_SS>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeLE<T, U, UL_US>::op (T const l, U const r) { return !SafeGT<T, U, Relationship<T, U>::value>::op(l, r); }
    
    // Test for equal to.
    template <class T, class U> inline bool SafeEQ<T, U, SE_SE>::op (T const l, U const r) {
      return l == static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeEQ<T, U, SE_UE>::op (T const l, U const r) {
      return l == static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeEQ<T, U, SS_SL>::op (T const l, U const r) {
      return static_cast<U>(l) == r;
    }
    template <class T, class U> inline bool SafeEQ<T, U, SS_UL>::op (T const l, U const r) {
      if (l < static_cast<T>(0)) {
        return false;
      } else {
        return static_cast<U>(l) == r;
      }
    }
    template <class T, class U> inline bool SafeEQ<T, U, SL_SS>::op (T const l, U const r) {
      return l == static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeEQ<T, U, SL_US>::op (T const l, U const r) {
      return l == static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeEQ<T, U, UE_UE>::op (T const l, U const r) {
      return l == static_cast<T>(r);
    }
    template <class T, class U> inline bool SafeEQ<T, U, UE_SE>::op (T const l, U const r) {
      return static_cast<U>(l) == r;
    }
    template <class T, class U> inline bool SafeEQ<T, U, US_SL>::op (T const l, U const r) {
      return static_cast<U>(l) == r;
    }
    template <class T, class U> inline bool SafeEQ<T, U, US_UL>::op (T const l, U const r) {
      return static_cast<U>(l) == r;
    }
    template <class T, class U> inline bool SafeEQ<T, U, UL_SS>::op (T const l, U const r) {
      if (r < static_cast<U>(0)) {
        return false;
      } else {
        return l == static_cast<T>(r);
      }
    }
    template <class T, class U> inline bool SafeEQ<T, U, UL_US>::op (T const l, U const r) {
      return l == static_cast<T>(r);
    }
    
    // Test for not equal.
    template <class T, class U> inline bool SafeNE<T, U, SE_SE>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, SE_UE>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, SS_SL>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, SS_UL>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, SL_SS>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, SL_US>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, UE_UE>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, UE_SE>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, US_SL>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, US_UL>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, UL_SS>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    template <class T, class U> inline bool SafeNE<T, U, UL_US>::op (T const l, U const r) { return !SafeEQ<T, U, Relationship<T, U>::value>::op(l, r); }
    
    // Bitwise AND.
    template <class T, class U> inline T SafeBitAnd<T, U, SE_SE>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, SE_UE>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, SS_SL>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, SS_UL>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, SL_SS>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, SL_US>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, UE_UE>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, UE_SE>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, US_SL>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, US_UL>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, UL_SS>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitAnd<T, U, UL_US>::op (T const l, U const r) { return l & RawCast<U, T, Relationship<U, T>::value>::op(r); }
    
    // Bitwise inclusive OR.
    template <class T, class U> inline T SafeBitIOr<T, U, SE_SE>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, SE_UE>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, SS_SL>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, SS_UL>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, SL_SS>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, SL_US>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, UE_UE>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, UE_SE>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, US_SL>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, US_UL>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, UL_SS>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitIOr<T, U, UL_US>::op (T const l, U const r) { return l | RawCast<U, T, Relationship<U, T>::value>::op(r); }
    
    // Bitwise exclusive OR.
    template <class T, class U> inline T SafeBitXOr<T, U, SE_SE>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, SE_UE>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, SS_SL>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, SS_UL>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, SL_SS>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, SL_US>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, UE_UE>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, UE_SE>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, US_SL>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, US_UL>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, UL_SS>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    template <class T, class U> inline T SafeBitXOr<T, U, UL_US>::op (T const l, U const r) { return l ^ RawCast<U, T, Relationship<U, T>::value>::op(r); }
    
    /*************************************************************************
     * Class SafeInt::_Relationship.
     *************************************************************************/
    
    /*************************************************************************/
    // Static data members.
    
    // Relationship between T and U.
    template <class T, class U> const RelType Relationship<T, U>::value(
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
    );
    
  }
  
  /***************************************************************************
   * SafeInt Errors.
   ***************************************************************************/
  
  namespace SafeIntErrors {
    
    char const* Overflow::what () const throw() {
      try {
        if (!message.empty()) {
          return message.c_str();
        } else {
          return "Overflow.";
        }
      } catch (...) {
        return "Error returning message string.";
      }
    }
    inline Overflow::~Overflow () throw() {}
    
    template <class T, class U> CastOverflow::CastOverflow (T const num, U const limit) throw () {
      try {
        message = toString(num) + " overflows type limit of " + toString(limit) + " in attempted cast.";
      } catch (...) {
        message.clear();
      }
    }
    
    template <class T, class U> BinOpOverflow::BinOpOverflow (T const l, char const* const op, U const r, T const limit) throw() {
      try {
        message = toString(l) + std::string(" ") + op + std::string(" ") + toString(r) + " overflows type limit of " + toString(limit) + ".";
      } catch (...) {
        message.clear();
      }
    }
    
    template <class T, class U> DivByZero::DivByZero (T const l, char const* const op, U const r) throw() {
      try {
        message = toString(l) + std::string(" ") + op + std::string(" ") + toString(r) + ": Divide by zero.";
      } catch (...) {
        message.clear();
      }
    }
    char const* DivByZero::what () const throw() {
      try {
        if (!message.empty()) {
          return message.c_str();
        } else {
          return "Divide by zero.";
        }
      } catch (...) {
        return "Error returning message string.";
      }
    }
    inline DivByZero::~DivByZero () throw() {}
    
    char const* Undefined::what () const throw() {
      try {
        if (!message.empty()) {
          return message.c_str();
        } else {
          return "Result of operation is undefined.";
        }
      } catch (...) {
        return "Error returning message string.";
      }
    }
    inline Undefined::~Undefined () throw() {}
    
    template <class T, class U> BinOpUndefined::BinOpUndefined (T const l, char const* const op, U const r) throw () {
      try {
        message = toString(l) + std::string(" ") + op + std::string(" ") + toString(r) + ": The result of this operation is undefined.";
      } catch (...) {
        message.clear();
      }
    }
    
  }
  
}

/*****************************************************************************
 * Global operators.
 *****************************************************************************/

// Stream I/O operators.
template <class T> inline std::ostream& operator << (std::ostream& l, DAC::SafeInt<T> const& r) { l << toString(static_cast<T>(r)); return l; }
template <class T> inline std::istream& operator >> (std::istream& l, DAC::SafeInt<T>&       r) { T input; l >> input; r = input; return l;   }

// Arithmetic operators.
template <class T, class U> inline DAC::SafeInt<T> operator * (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_mul(r); }
template <class T, class U> inline DAC::SafeInt<T> operator * (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_mul(r); }
template <class T, class U> inline DAC::SafeInt<T> operator * (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_mul(r); }
template <class T, class U> inline DAC::SafeInt<T> operator / (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_div(r); }
template <class T, class U> inline DAC::SafeInt<T> operator / (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_div(r); }
template <class T, class U> inline DAC::SafeInt<T> operator / (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_div(r); }
template <class T, class U> inline DAC::SafeInt<T> operator % (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_mod(r); }
template <class T, class U> inline DAC::SafeInt<T> operator % (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_mod(r); }
template <class T, class U> inline DAC::SafeInt<T> operator % (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_mod(r); }
template <class T, class U> inline DAC::SafeInt<T> operator + (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_add(r); }
template <class T, class U> inline DAC::SafeInt<T> operator + (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_add(r); }
template <class T, class U> inline DAC::SafeInt<T> operator + (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_add(r); }
template <class T, class U> inline DAC::SafeInt<T> operator - (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_sub(r); }
template <class T, class U> inline DAC::SafeInt<T> operator - (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_sub(r); }
template <class T, class U> inline DAC::SafeInt<T> operator - (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_sub(r); }

// Bit shift operators.
template <class T, class U> inline DAC::SafeInt<T> operator << (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_shl(r); }
template <class T, class U> inline DAC::SafeInt<T> operator << (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_shl(r); }
template <class T, class U> inline DAC::SafeInt<T> operator << (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_shl(r); }
template <class T, class U> inline DAC::SafeInt<T> operator >> (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_shr(r); }
template <class T, class U> inline DAC::SafeInt<T> operator >> (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_shr(r); }
template <class T, class U> inline DAC::SafeInt<T> operator >> (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_shr(r); }

// Comparison operators.
template <class T, class U> inline bool operator >  (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return l.op_gt(r); }
template <class T, class U> inline bool operator >  (DAC::SafeInt<T> const& l, U               const  r) { return l.op_gt(r); }
template <class T, class U> inline bool operator >  (T               const  l, DAC::SafeInt<U> const& r) { return r.op_le(l); }
template <class T, class U> inline bool operator >= (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return l.op_ge(r); }
template <class T, class U> inline bool operator >= (DAC::SafeInt<T> const& l, U               const  r) { return l.op_ge(r); }
template <class T, class U> inline bool operator >= (T               const  l, DAC::SafeInt<U> const& r) { return r.op_lt(l); }
template <class T, class U> inline bool operator <  (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return l.op_lt(r); }
template <class T, class U> inline bool operator <  (DAC::SafeInt<T> const& l, U               const  r) { return l.op_lt(r); }
template <class T, class U> inline bool operator <  (T               const  l, DAC::SafeInt<U> const& r) { return r.op_ge(l); }
template <class T, class U> inline bool operator <= (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return l.op_le(r); }
template <class T, class U> inline bool operator <= (DAC::SafeInt<T> const& l, U               const  r) { return l.op_le(r); }
template <class T, class U> inline bool operator <= (T               const  l, DAC::SafeInt<U> const& r) { return r.op_gt(l); }
template <class T, class U> inline bool operator == (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return l.op_eq(r); }
template <class T, class U> inline bool operator == (DAC::SafeInt<T> const& l, U               const  r) { return l.op_eq(r); }
template <class T, class U> inline bool operator == (T               const  l, DAC::SafeInt<U> const& r) { return r.op_eq(l); }
template <class T, class U> inline bool operator != (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return l.op_ne(r); }
template <class T, class U> inline bool operator != (DAC::SafeInt<T> const& l, U               const  r) { return l.op_ne(r); }
template <class T, class U> inline bool operator != (T               const  l, DAC::SafeInt<U> const& r) { return r.op_ne(l); }

// Bitwise operators.
template <class T, class U> inline DAC::SafeInt<T> operator & (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_bit_and(r); }
template <class T, class U> inline DAC::SafeInt<T> operator & (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_bit_and(r); }
template <class T, class U> inline DAC::SafeInt<T> operator & (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_bit_and(r); }
template <class T, class U> inline DAC::SafeInt<T> operator | (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_bit_ior(r); }
template <class T, class U> inline DAC::SafeInt<T> operator | (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_bit_ior(r); }
template <class T, class U> inline DAC::SafeInt<T> operator | (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_bit_ior(r); }
template <class T, class U> inline DAC::SafeInt<T> operator ^ (DAC::SafeInt<T> const& l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_bit_xor(r); }
template <class T, class U> inline DAC::SafeInt<T> operator ^ (DAC::SafeInt<T> const& l, U               const  r) { return DAC::SafeInt<T>(l).op_bit_xor(r); }
template <class T, class U> inline DAC::SafeInt<T> operator ^ (T               const  l, DAC::SafeInt<U> const& r) { return DAC::SafeInt<T>(l).op_bit_xor(r); }

#endif