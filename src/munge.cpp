
#include <cassert>
/*
=================
=
= VL_MungePic
=
=================
*/

void VL_MungePic(unsigned char* source, unsigned width, unsigned height)
{
    assert(false);
#if 0
    unsigned       x, y, plane, size, pwidth;
    unsigned char *temp, *dest, *srcline;

    size = width * height;

    if (width & 3)
        errout("VL_MungePic: Not divisable by 4!\n");

    //
    // copy the pic to a temp buffer
    //
    temp = (unsigned char*)farmalloc(size);
    if (!temp)
        errout("Non enough memory for munge buffer!\n");

    _fmemcpy(temp, source, size);

    //
    // munge it back into the original buffer
    //
    dest   = source;
    pwidth = width / 4;

    for (plane = 0; plane < 4; plane++)
    {
        srcline = temp;
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < pwidth; x++)
                *dest++ = *(srcline + x * 4 + plane);
            srcline += width;
        }
    }

    free(temp);
#endif
}
