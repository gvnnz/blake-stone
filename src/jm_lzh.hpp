#ifndef JM_LZH_HPP
#define JM_LZH_HPP

extern void (*LZH_CompressDisplayVector)(unsigned long, unsigned long);
extern void (*LZH_DecompressDisplayVector)(unsigned long, unsigned long);

//===========================================================================
//
//											PROTOTYPES
//
//===========================================================================

bool LZH_Startup(void);
void LZH_Shutdown(void);
long LZH_Compress(void* infile, void* outfile, unsigned long DataLength, unsigned PtrTypes);
long LZH_Decompress(void* infile, void* outfile, unsigned long OrginalLength, unsigned long CompressLength, unsigned PtrTypes);

#endif