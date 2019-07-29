#include "gc_pointer.h"
#include "LeakTester.h"

int main()
{
  
    Pointer<int> p = new int(13);
    p = new int(21);
    p = new int(34);
    
    return 0;
}


