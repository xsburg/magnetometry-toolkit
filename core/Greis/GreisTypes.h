#ifndef GreisTypes_h__
#define GreisTypes_h__

namespace Greis
{
    namespace Types
    {
                                    // Type Name | Meaning | Length in Bytes
        typedef char a1;            // a1 | ASCII character | 1
        typedef char i1;            // i1 | signed integer | 1
        typedef short i2;           // i2 | signed integer | 2
        typedef int i4;             // i4 | signed integer | 4
        typedef unsigned char u1;   // u1 | unsigned integer | 1
        typedef unsigned short u2;  // u2 | unsigned integer | 2
        typedef unsigned int u4;    // u4 | unsigned integer | 4
        typedef float f4;           // f4 | IEEE-754 single precision floating point | 4
        typedef double f8;          // f8 | IEEE-754 double precision floating point | 8
    }
}

#endif // GreisTypes_h__