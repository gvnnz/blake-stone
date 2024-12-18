#ifndef JM_DEBUG_HPP
#define JM_DEBUG_HPP

//-------------------------------------------------------------------------
// defines
//-------------------------------------------------------------------------

#if IN_DEVELOPMENT
#define __MPRINTF__
#endif

//-------------------------------------------------------------------------
//  typedefs
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Function prototypes
//-------------------------------------------------------------------------
void mclear(void);
void mprintf(char* msg, ...);
void fmprint(char* text);

#endif