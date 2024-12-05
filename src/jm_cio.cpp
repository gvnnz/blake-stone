#include "jm_cio.hpp"
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>

//----------------------------------------------------------------------------
//
//	 						PTR/PTR COMPRESSION ROUTINES
//
//
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// CIO_WritePtr()  -- Outputs data to a particular ptr type
//
//	PtrType MUST be of type DEST_TYPE.
//
// NOTE : For PtrTypes DEST_MEM a ZERO (0) is always returned.
//
//---------------------------------------------------------------------------
char CIO_WritePtr(long outfile, unsigned char data, unsigned PtrType)
{
    assert(false);

    int returnval = 0;

#if 0
    switch (PtrType & DEST_TYPES)
    {
    case DEST_FILE:
        write(*(int far*)outfile, (char*)&data, 1);
        break;

    case DEST_FFILE:
        returnval = putc(data, *(FILE**)outfile);
        break;

    case DEST_IMEM:
        printf("CIO_WritePtr - unsupported ptr type\n");
        exit(0);
        break;

    case DEST_MEM:
        *((char far*)*(char far**)outfile)++ = data;
        break;
    }
#endif

    return (returnval);
}

//---------------------------------------------------------------------------
// CIO_ReadPtr()  -- Reads data from a particular ptr type
//
//	PtrType MUST be of type SRC_TYPE.
//
// RETURNS :
//		The char read in or EOF for SRC_FFILE type of reads.
//
//
//---------------------------------------------------------------------------
int CIO_ReadPtr(long infile, unsigned PtrType)
{
    assert(false);

    int returnval = 0;

#if 0
    switch (PtrType & SRC_TYPES)
    {
    case SRC_FILE:
        read(*(int far*)infile, (char*)&returnval, 1);
        break;

    case SRC_FFILE:
        returnval = getc(*(FILE**)infile);
        break;

#if 0
		case SRC_BFILE:
			returnval = bio_readch((BufferedIO *)*(void far **)infile);
		break;
#endif

        //		case SRC_IMEM:
        //			printf("CIO_WritePtr - unsupported ptr type\n");
        //			exit(0);
        //		break;

    case SRC_MEM:
        returnval = (unsigned char)*((char far*)*(char far**)infile)++;
        break;
    }
#endif

    return (returnval);
}
