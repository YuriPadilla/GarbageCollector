// This class defines an element that is stored
// in the garbage collection information list.
//
template <class T>
class PtrDetails {
	public:
        unsigned refcount = 0; // current reference count
        T* memPtr;         // pointer to allocated memory
        bool isArray; // true if pointing to array. isArray is true if memPtr points to an allocated array. It is false otherwise.
        unsigned arraySize; // size of array. If memPtr is pointing to an allocated array, then arraySize contains its size.
                            // Here, mPtr points to the allocated memory. If this is an array, then size specifies the size of the array.
        PtrDetails(T* _memPtr, unsigned _arraySize) {
            memPtr = _memPtr;
            arraySize = _arraySize;
            refcount++;
          
            if(arraySize > 0) {
                //isArray = false; //I don't understand why false
                isArray = true; //That's why I changed it for true...
            } else {
                isArray = false;
            }
        }
};

template <class T> // Overloading operator== allows two class objects to be compared. This is needed by the STL list class.
bool operator==(const PtrDetails<T> &ob1,
                const PtrDetails<T> &ob2) {
     // TODO: Implement operator==
      if(ob1.memPtr == ob2.memPtr) {
          return true;
      } else {
          return false;
      }
}
