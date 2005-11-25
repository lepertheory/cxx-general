/*****************************************************************************
 * AutoArray - Like auto_ptr, except for arrays.
 *****************************************************************************/

#if !defined(AUTOARRAY_9234khti934)
  #define AUTOARRAY_9234khti934
  
  // Contain in a namespace.
  namespace DAC {
    
    // The class.
    template<class T> class AutoArray {
      
      public:
        
        // Constructor. When constructed with a pointer, take owership of the
        // pointer. When constructed with another AutoArray, transfer ownership
        // from the original.
        explicit AutoArray (T*         p = 0) throw();
                 AutoArray (AutoArray& p)     throw();
        
        // Destructor.
        ~AutoArray () throw();
        
        // Assignment operator. See description of the constructor.
        AutoArray& operator= (T*         p) throw();
        AutoArray& operator= (AutoArray& p) throw();
        
        // Dereferencing operators. Works as if AutoArray were a native *.
        T& operator[] (int index) const throw();
        T& operator*  ()          const throw();
        T* operator-> ()          const throw();
        
        // get() returns the pointer without de-allocating. Will still de-
        // allocate on destruction of the object.
        T* get () const throw();
        
        // release() returns the pointer and releases ownership, i.e., the
        // memory pointed to will not be de-allocated on object destruction.
        T* release () throw();
        
        // reset() de-allocates memory at the current pointer and takes
        // ownership of the passed pointer.
        void reset (T* p = 0) throw();
        
      private:
        
        T* _p;
      
    };
    
    /*************************************************************************/
    template<class T> AutoArray<T>::AutoArray (T* p) throw() {
      _p = p;
    }
    
    /*************************************************************************/
    template<class T> AutoArray<T>::AutoArray (AutoArray& p) throw() {
      _p = p.release();
    }
    
    /*************************************************************************/
    template<class T> AutoArray<T>::~AutoArray () throw() {
      delete[] _p;
    }
    
    /*************************************************************************/
    template<class T> AutoArray<T>& AutoArray<T>::operator= (T* p) throw() {
      reset(p);
      return *this;
    }
    
    /*************************************************************************/
    template<class T> AutoArray<T>& AutoArray<T>::operator= (AutoArray<T>& p) throw() {
      reset(p.release());
      return *this;
    }
    
    /*************************************************************************/
    template<class T> T& AutoArray<T>::operator[] (int index) const throw() {
      return _p[index];
    }
    
    /*************************************************************************/
    template<class T> T& AutoArray<T>::operator* () const throw() {
      return *_p;
    }
    
    /*************************************************************************/
    template<class T> T* AutoArray<T>::operator-> () const throw() {
      return _p;
    }
    
    /*************************************************************************/
    template<class T> T* AutoArray<T>::get () const throw() {
      return _p;
    }
    
    /*************************************************************************/
    template<class T> T* AutoArray<T>::release () throw() {
      T* tp = _p;
      _p    = 0;
      return tp;
    }
    
    /*************************************************************************/
    template<class T> void AutoArray<T>::reset (T* p) throw() {
      if (_p != p) {
        delete[] _p;
        _p = p;
      }
    }
    
  }
  
#endif
