#ifndef BaseItemArray_hhhhh
#define BaseItemArray_hhhhh

#include <ios>
#include <sstream>

template<
    class T, // The internal class
    int MAX_SIZE_VAL, // The maximum size of the array
    const std::uintptr_t ARRAY_VAL_PTR, // The ptr of the actual array
    const int OFFSET_VAL = 0 // (Optional) Offset added to the index
>
struct SMBX_StaticBaseItemArray
{
    static inline T* Get(unsigned short index) {
        if (index >= MAX_SIZE_VAL) return NULL;
        return &(*(T**)ARRAY_VAL_PTR)[index + OFFSET_VAL];
    }
};

template<
    class T, // The internal class
    const short MAX_ID_VAL, // The ptr to the max id
    const std::uintptr_t COUNT_VAL_PTR, // The ptr to the count value (how many objects of type T are in that array)
    const std::uintptr_t ARRAY_VAL_PTR, // The ptr of the actual array
    const int OFFSET_VAL = 0 // (Optional) Offset added to the index
>
struct SMBX_FullBaseItemArray
{
    static inline T* Get(unsigned short index) {
        if (index > Count()) return NULL;
        return &(*(T**)ARRAY_VAL_PTR)[index + OFFSET_VAL];
    }

    static inline T* GetRaw(unsigned short index) {
        return &(*(T**)ARRAY_VAL_PTR)[index + OFFSET_VAL];
    }

    static inline unsigned short Count() {
        return *(unsigned short*)COUNT_VAL_PTR;
    }

    static const short MAX_ID = MAX_ID_VAL;
};

#endif
