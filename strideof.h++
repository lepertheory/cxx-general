/*****************************************************************************
 * strideof
 *****************************************************************************
 * Get the stride of a given data type. I didn't come up with this, and it's
 * not only novel as hell but looks useful. Hope it's not patented. Anyway,
 * in case the site explaining it goes down, I've copied it here.
 * 
 * Also, this doesn't seem to work. At least, using __alignof() in GCC returns
 * 8 for long long and long double, but this returns 4. Different -O levels
 * have no effect. Maybe I did something wrong, maybe GCC isn't following its
 * own recommended alignments, or maybe I'm misunderstanding this completely.
 * Don't use this header until the question is answered.
 *****************************************************************************
 * Computing Memory Alignment Restrictions
 * of Data Types in C and C++
 * Tom Lokovic
 * http://www.monkeyspeak.com/tom/
 * July 2003
 * ABSTRACT
 * 
 * When implementing a dynamic object system in which structures of
 * heterogenous data are assembled at runtime, one must respect memory
 * alignment restrictions of the hardware. Since alignment restrictions are
 * different from one platform to the next, this imposes a portability problem.
 * This paper describes a portable method for computing alignment restrictions
 * of data types in C and C++.
 * BACKGROUND
 * 
 * Modern computer hardware is byte-addressed: the address space is broken up
 * into bytes, and any pointer may reference any individual byte. A value which
 * consists of one byte may be placed anywhere in memory. However, most
 * hardware architectures impose restrictions on larger data types.
 * Specifically, a multi-byte value may be constrained to appear at only
 * certain memory locations.
 * 
 * This restriction is often called memory alignment: the multi-byte value may
 * only appear at memory locations which are integer multiples of some number.
 * In this paper, we call that number the stride of the data type. The stride
 * may vary between different data types on one platform, and between the same
 * type on different platforms.
 * 
 * Most programs in C and C++ need not worry about alignment restrictions. This
 * is because compilers know enough about the platform to arrange values so
 * that they all align properly. As long as a program restricts itself to
 * simple structs and classes, and as long as it does not attempt to place
 * values at arbitrary memory locations, the programmer need not worry about
 * alignment issues.
 * 
 * However, some programs cannot be so restrictive. In particular, if a program
 * needs to assemble structs dynamically--that is, determine at runtime which
 * types are needed and then arrange them on the fly--then native structs and
 * classes are insufficient. The program must perform its own "packing" of
 * structured data, which also means it must take care to respect alignment.
 * 
 * While C and C++ compilers always know about the alignment restrictions of
 * the underlying platform, there is no formal mechanism to ask the compiler
 * about those restrictions. This paper offers a portable way to "trick" the
 * compiler into revealing what it knows about alignment.
 * THE TECHNIQUE
 * 
 * C and C++ provide a built-in operator sizeof(), which returns the size of a
 * data type in bytes. We seek a similar operator, strideof(), which returns
 * the type's alignment stride in bytes.
 * 
 * Here is such an operator, implemented with a template and a macro:
 * 
 * 
 *    template <typename T>
 *    struct Tchar {
 *        T t;
 *        char c;
 *    };
 * 
 *    #define strideof(T)                             \
 *       ((sizeof(Tchar<T>) > sizeof(T)) ?            \
 *         sizeof(Tchar<T>)-sizeof(T) : sizeof(T))
 * 
 * It may not be immediately obvious why this works, or even what this code has
 * to do with alignment. Before we get into the details, though, let's consider
 * the advantages and disadvantages of this technique.
 * 
 * Advantages:
 * 
 *     * It works for any type, including native types (eg, short, double),
 * struct types, and class types.
 *     * Its computation can be performed at compile-time via constant folding,
 * and thus (like sizeof()) it incurs no runtime cost at all.
 * 
 * Disadvantages:
 * 
 *     * The macro uses C++ templates, which are not available in C. The basic
 * technique (subtracting T's size from the size of a special struct) works
 * equally well in C, but without templates there's no way to provide a general
 * macro like strideof().
 *     * While this technique is always guaranteed to give a result that is
 * safe to use, and in practice does give the right answer, it's not strictly
 * guaranteed to give T's true stride. More on that later.
 *     * Unlike sizeof(),which is a built-in operator, the strideof() macro can
 * only take a type as its argument--not an instance.
 * 
 * HOW IT WORKS:
 * 
 * Consider a type T for which we wish to compute the stride. The stride
 * constrains the memory locations at which an instance of T may begin. (Here,
 * each ^ represents a memory location at which T may appear; the spacing
 * between them is the stride we seek.)
 * 
 *    ^   ^   ^   ^   ^   ^   ^   ^   ^   ^
 * 
 * Now consider a hypothetical instance of T in memory. It has sizeof(T) bytes,
 * and must begin at some multiple of T's stride. We also know (from the proof
 * in Appendix 1 below) that sizeof(T) is an integer multiple of T's stride.
 * Unfortunately, we don't know which multiple it is, and so we still don't
 * know the stride.
 * 
 *        tttttttttttt
 *    ^   ^   ^   ^   ^   ^   ^   ^   ^   ^
 * 
 * Now suppose that, instead of a T, we have a struct Tchar which contains both
 * a T and a char. There are several ways in which the compiler may choose to
 * arrange the struct, and this technique works equally well with all of them.
 * For now, we'll assume it does what most compilers do: place the char
 * immediately after the T:
 * 
 *        ttttttttttttc
 *    ^   ^   ^   ^   ^   ^   ^   ^   ^   ^
 * 
 * However, the compiler cannot leave the struct quite like this. For alignment
 * reasons, the compiler will bump the size of Tchar up to some multiple of T's
 * stride--probably the next multiple. It must do this regardless of the order
 * in which it arranges the elements and the padding. For a proof, See Appendix
 * 2.
 * 
 *        ttttttttttttc###
 *    ^   ^   ^   ^   ^   ^   ^   ^   ^   ^
 * 
 * Now, if we subtract the size of T from the size of Tchar, we get some
 * integer multiple of T's stride. In this case (and in the common case), we
 * get T's stride exactly.
 * 
 *        ttttttttttttc###
 *    ^   ^   ^   ^   ^   ^   ^   ^   ^   ^
 *                    <---
 * 
 * There are two cases in which we might not get T's stride exactly. The first
 * is if the compiler (for some reason) chooses to pad the struct too much. In
 * that case, Tchar's size will be bumped up to some higher multiple of T's
 * stride. The result we get is not optimal, but it's still guaranteed to be
 * safe to use. Such excessive padding should probably be considered a bug in
 * the compiler, but our technique deals safely with that case anyway.
 * 
 * The second case is if T already has some padding in it, and the compiler
 * chooses to reuse some of T's padding to store the extra char. In that case,
 * Tchar will be the same size as T, and our subtraction will yield zero. When
 * that happens, our macro defaults to sizeof(T), which is not optimal but is
 * at least known to be a multiple of T's stride. (There is some dispute as to
 * whether this case can arise--some believe that it is illegal for a compiler
 * to reuse padding like this. Either way, our technique deals with it.)
 * 
 * Note that this technique has more to do with the compiler's alignment
 * strategy than the hardware's actual restrictions. For example, some
 * architectures do not prohibit misaligned values, but instead incur some
 * performance penalty when operating on misaligned values. If a compiler is
 * configured to allow misaligned values (to save space at the cost of some
 * performance), then our technique will reflect that alignment strategy.
 * APPENDIX 1
 * 
 * Claim: For every type T, sizeof(T) is an integer multiple of T's stride S.
 * 
 * Proof:
 * 
 *    1. By definition, instances of T may only begin at memory addresses which
 * are integer multiples of S.
 *    2. C and C++ guarantee that every datatype is "tilable". That is, if an
 * instance of T appears at a legal address a, then it is always legal
 * (alignment-wise) for another instance to appear at a+sizeof(T). This is what
 * makes arrays possible.
 *    3. Given any two instances of T in memory, the difference between their
 * addresses is an integer multiple of S. (This is clearly true since, from i,
 * both addresses are integer multiples of S.)
 *    4. Suppose A is an instance of type T which appears at address a. By ii,
 * it is legal to place another instance at address a+sizeof(T). And by iii, we
 * know (a+sizeof(T))-a is an integer multiple of S.
 *    5. Thus, sizeof(T) is an integer multiple of S.
 * 
 * APPENDIX 2
 * 
 * Claim: For every struct type T, sizeof(T) is an integer multiple of the
 * stride of each of T's members.
 * 
 * Proof:
 * 
 *    6. Suppose A is an instance of type T at address a.
 *    7. From ii, it is legal to place an instance B of type T at address
 * a+sizeof(T).
 *    8. Given any element inside struct T, the difference between its address
 * in B and its address in A is sizeof(T).
 *    9. But from iii, that difference is also an integer multiple of the
 * element's stride.
 *   10. Thus, sizeof(T) is an integer multiple of the element's stride.
 *****************************************************************************/

// Include guard.
#if !defined(STRIDEOF_4kgdurg2k27d)
  #define STRIDEOF4kgdurg2k27d

// Standard includes.
#include <cstdlib>

// Namespace wrapper.
namespace DAC {
  
  /***************************************************************************
   * strideof
   ***************************************************************************
   * This is a class so that we can hide any ugly details from the user.
   ***************************************************************************/
  template <class T> class strideof {
    
    /*
     * Public members.
     */
    public:
      
      /***********************************************************************/
      // Static function members.
      
      // Get the stride of T.
      static size_t get ();
      
    /*
     * Private members.
     */
    private:
      
      /***********************************************************************/
      // Data types.
      
      // Used to calculate the stride, see explanation above.
      struct Tchar {
        T    t;
        char c;
      };
      
      /***********************************************************************/
      // Function members.
      
      // Instantiation of this class is not allowed.
      strideof ();
      strideof (strideof const&);
      strideof& operator = (strideof const&);
    
  };
  
  /***************************************************************************/
  // Functions.
  
  /***************************************************************************
   * Inline and template definitions.
   ***************************************************************************/
  
  /***************************************************************************/
  // Static function members.
  
  /*
   * Get the stride of a particular data type.
   */
  template <class T> inline size_t strideof<T>::get () {
    return (sizeof(Tchar) > sizeof(T)) ? sizeof(Tchar) - sizeof(T) : sizeof(T);
  };
  
}

// End include guard.
#endif

