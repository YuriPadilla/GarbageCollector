#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>
#include "gc_details.h"
#include "gc_iterator.h"
/* Pointer implements a pointer type that uses garbage collection to release unused memory. A Pointer must only be used to point to memory that was
   dynamically allocated using new. When used to refer to an allocated array, specify the array size. */
template <class T, int size = 0>
class Pointer {
	private:
    	static std::list<PtrDetails<T> > refContainer;//refContainer maintains the garbage collection list.
                                                      //Yo... refContainer es una lista que recolecta la basura. Los Pointers que ya no se usan se guardan aquí.
                                                      //Yo... Esta lista contiene objetos de la clase PtrDetails<T>.
    	T* addr;// addr points to the allocated memory to which Pointer pointer actually points. 
    	bool isArray;//isArray is true if this Pointer points to an allocated array. It is false otherwise. 
    	unsigned arraySize;//true if pointing to array. If this Pointer is pointing to an allocated array, then arraySize contains its size.
                           //Yo... size of the array
    	static bool first; // true when first Pointer is created
    	typename std::list<PtrDetails<T> >::iterator findPtrInfo(T* ptr);// Return an iterator to pointer details in refContainer.
	public:
    	typedef Iter<T> GCiterator;// Define an iterator type for Pointer<T>.
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    	Pointer(){ // Empty constructor. NOTE: templates aren't able to have prototypes with default...          +++
        	Pointer(NULL);                //...arguments, this is why constructor is designed like this:          ++++
    	}                                                                                           //             ++++++  Constructors and
    	Pointer(T*);    //Constructor normal                                                         //            ++++    Destructors
    	Pointer(const Pointer &); // Copy constructor.                                                            +++
    	~Pointer(); // Destructor for Pointer.                                                                  +++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    	static bool collect(); //++++++++++++++++++++++++++++++++++ Collect garbage. Returns true if at least one object was freed. 
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    	//T* operator=(T* t); // Overload assignment of pointer to Pointer.                                    +++
        Pointer& operator=(T* t);// Made for mi...                                                               +++
        Pointer& operator=(Pointer &rv); // Overload assignment of Pointer to Pointer.                            ++++
    	T& operator*() { return *addr;} // Return a reference to the object pointed to by this Pointer.            +++++  Operator overloading
    	T* operator->(){ return addr; } // Return the address being pointed to.                                   ++++
    	T &operator[](int i){ return addr[i];} // Return a reference to the object at the index specified by i.  +++
    	operator T *() { return addr; } // Conversion function to T *.                                          +++
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    	Iter<T> begin(){ // Return an Iter to the start of the allocated memory.
        	int _size;
        	if (isArray)
        	_size = arraySize;
        	else
            	_size = 1;
        	return Iter<T>(addr, addr, addr + _size);
    	}
    	Iter<T> end(){ // Return an Iter to one past the end of an allocated array.
        	int _size;
        	if (isArray)
            	_size = arraySize;
        	else
            	_size = 1;
        	return Iter<T>(addr + _size, addr, addr + _size);
    	}
    	static int refContainerSize() { return refContainer.size(); } // Return the size of refContainer for this type of Pointer.
    	static void showlist(); // A utility function that displays refContainer.
    	static void shutdown(); // Clear refContainer when program exits.
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template <class T, int size>                                 //             +++
std::list<PtrDetails<T> > Pointer<T, size>::refContainer;     //              +++++      STATIC INITIALIZATION
//++++++++++++++++++++++++++++++++++++++++++++++                               +++++++   Creates storage for the static variables
template <class T, int size>                                  //              +++++
bool Pointer<T, size>::first = true;                         //             +++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ************************** Constructor for both initialized and uninitialized...  //            ++++
template<class T, int size>                                           // ...objects.  //            ++++
Pointer<T,size>::Pointer(T* t) {                                                       //            ++++
    // Register shutdown() as an exit function.                                         //            ++++
    if (first)                                                                           //            ++++
        atexit(shutdown);                                                                 //            ++++
    first = false; //I don't understand why false                                          //            ++++
    //first = true; //That's why I changed it for true...                                   //            ++++
    // TODO: Implement Pointer constructor // Lab: Smart Pointer Project Lab                 //            ++++
    addr = t;                                                                                 //            ++++
    arraySize = size;                                                                          //            ++++
    refContainer.push_back(PtrDetails<T>(addr, arraySize));                                     //            ++++
}                                                                                                //            ++++
// ************************** Copy constructor.                                                   //            ++++
template< class T, int size>                                                                       //            ++++
Pointer<T,size>::Pointer(const Pointer &ob){                                                        //            ++++    Implementing
    // TODO: Implement Pointer constructor. Lab: Smart Pointer Project Lab                           //            ++++   Constructors and
    addr = ob.addr;                                                                                   //            ++++    Destructors
    arraySize = ob.arraySize;                                                                        //            ++++
                                                                                                    //            ++++
    typename std::list<PtrDetails<T> >::iterator p;                                                //            ++++
    p = findPtrInfo(addr);                                                                        //            ++++
    if(p->refcount);                                                                             //            ++++
    p->refcount++;                                                                              //            ++++
}                                                                                              //            ++++
// ************************** Destructor for Pointer.                                         //            ++++
template <class T, int size>                                                                 //            ++++
Pointer<T, size>::~Pointer(){                                                               //            ++++
                                                                                           //            ++++
    typename std::list<PtrDetails<T> >::iterator p;                                       //            ++++
    p = findPtrInfo(addr);                                                               //            ++++
    // decrement ref count                                                              //            ++++
    if(p->refcount) {                                                                  //            ++++
        p->refcount--;                                                                //            ++++
    }                                                                                //            ++++
    collect();// Collect garbage when a pointer goes out of scope.                  //            ++++  
}                                                                                  //            ++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Collect garbage. Returns true if at least one object was freed.                    //            ++++
template <class T, int size>                                                           //            ++++
bool Pointer<T, size>::collect(){                                                       //            ++++
    // TODO: Implement collect function. LAB: New and Delete Project Lab.                //            ++++
    // Note: collect() will be called in the destructor                                   //            ++++
    bool memfreed = false;                                                                 //            ++++
    typename std::list<PtrDetails<T> >::iterator p;                                         //            ++++
    do{                                                                                      //            ++++
        // Scan refContainer looking for unreferenced pointers.                               //            ++++
        for (p = refContainer.begin(); p != refContainer.end(); p++){                          //            ++++
            // TODO: Implement collect()                                                        //            ++++
            // If in-use, skip.                                                                  //            ++++
            if(p->refcount > 0)                                                                   //            ++++
                continue;                                                                          //            ++++
            memfreed = true;                                                                        //            ++++
            //Remove unused entry from refContainer.                                                 //            ++++
            refContainer.remove(*p);                                                                  //            ++++
            // Free memory unless the Pointer is null.                                                 //            ++++    Function Collect()
            if(p->memPtr) {                                                                           //            ++++
                if(p->isArray) {                                                                     //            ++++
                    delete[] p->memPtr;                                                             //            ++++
                } else {                                                                           //            ++++
                    delete p->memPtr;                                                             //            ++++
                }                                                                                //            ++++
            }                                                                                   //            ++++
            // Restart the search.                                                             //            ++++
            break;                                                                            //            ++++
        }                                                                                    //            ++++
    } while (p != refContainer.end());                                                      //            ++++
    //return false;                                                                        //            ++++
    return memfreed;                                                                      //            ++++
}                                                                                        //            ++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Overload assignment of pointer to Pointer.                                            //            ++++
template <class T, int size>                                                              //            ++++
//T* Pointer<T, size>::operator=(T* t){                                                    //            ++++
Pointer<T, size>& Pointer<T, size>::operator=(T* t) {                                       //            ++++
                                                                                             //            ++++
    typename std::list<PtrDetails<T> >::iterator p;                                           //            ++++
    p = findPtrInfo(this->addr);                                                               //            ++++
    if(p->refcount);                                                                            //            ++++
    p->refcount--;                                                                               //            ++++
                                                                                                  //            ++++
    this->addr = t;                                                                                //            ++++
    this->arraySize = size;                                                                         //            ++++
    refContainer.push_back(PtrDetails<T>(this->addr, this->arraySize));                              //            ++++
    return *this;                                                                                     //            ++++
}                                                                                                      //            ++++   Overloading
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//            ++++  operator =
// Overload assignment of Pointer to Pointer.                                                          //            ++++   twice...
template <class T, int size>                                                                          //            ++++
Pointer<T, size>& Pointer<T, size>::operator=(Pointer& rv) {                                         //            ++++
                                                                                                    //            ++++
    typename std::list<PtrDetails<T> >::iterator p;                                                //            ++++
    p = findPtrInfo(this->addr);                                                                  //            ++++
    if(p->refcount);                                                                             //            ++++
    p->refcount--;                                                                              //            ++++
                                                                                               //            ++++
    this->addr = rv.addr;                                                                     //            ++++
    this->arraySize = rv.arraySize;                                                          //            ++++
    p = findPtrInfo(this->addr);                                                            //            ++++
    if(p->refcount);                                                                       //            ++++
    p->refcount++;                                                                        //            ++++
    return *this;                                                                        //            ++++
}                                                                                       //            ++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// A utility function that displays refContainer.
template <class T, int size>
void Pointer<T, size>::showlist(){
    typename std::list<PtrDetails<T> >::iterator p;
    std::cout << "refContainer<" << typeid(T).name() << ", " << size << ">:\n";
    std::cout << "  memPtr      refcount       value" << "\n";
    if (refContainer.begin() == refContainer.end())
    {
        std::cout << " Container is empty!\n\n ";
    }
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        std::cout << "[" << (void *)p->memPtr << "]"
             << "       " << p->refcount << "           ";
        if (p->memPtr)
            std::cout << " " << *p->memPtr;
        else
            std::cout << "---";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Find a pointer in refContainer.
template <class T, int size>
typename std::list<PtrDetails<T> >::iterator
Pointer<T, size>::findPtrInfo(T* ptr) {
    typename std::list<PtrDetails<T> >::iterator p;
    // Find ptr in refContainer.
    for (p = refContainer.begin(); p != refContainer.end(); p++)
        if (p->memPtr == ptr)
            return p;
    return p;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Clear refContainer when program exits.
template <class T, int size>
void Pointer<T, size>::shutdown(){
    if (refContainerSize() == 0)
        return; // list is empty
    typename std::list<PtrDetails<T> >::iterator p;
    for (p = refContainer.begin(); p != refContainer.end(); p++)
    {
        // Set all reference counts to zero
        p->refcount = 0;
    }
    collect();
}
