#ifndef _ID_HEADS_H_
#define _ID_HEADS_H_

// ID_GLOB.H

#include <ALLOC.H>
#include <CTYPE.H>
#include <DIR.H>
#include <DOS.H>
#include <ERRNO.H>
#include <FCNTL.H>
#include <IO.H>
#include <MEM.H>
#include <PROCESS.H>
#include <STDIO.H>
#include <STDLIB.H>
#include <STRING.H>
#include <SYS\STAT.H>
#include <VALUES.H>
#define __ID_GLOB__

//--------------------------------------------------------------------------

#define DEBUG_VALUE
#define CEILING_FLOOR_COLORS

// #define CARMACIZED
#define WOLF
#define FREE_FUNCTIONS (false)
#define FREE_DATA (false)
#define DEMOS_ENABLED (true)
#define RESTART_PICTURE_PAUSE (false)
#define GEORGE_CHEAT (false)

#define FORCE_FILE_CLOSE (true) // true - forces all files closed once they are used

//
// GAME VERSION TYPES
//

#define SHAREWARE_VERSION 0x0001
#define MISSIONS_1_THR_3 0x0003
#define MISSIONS_4_THR_6 0x0004
#define MISSIONS_1_THR_6 0x0007

//
// CURRENT GAME VERSION DEFINE - Change this define according to the
//											game release versions 1,1-3,4-6, or 1-6.

#define GAME_VERSION (MISSIONS_1_THR_6)
// #define GAME_VERSION      			(MISSIONS_1_THR_3)
// #define GAME_VERSION					(SHAREWARE_VERSION)

#define TECH_SUPPORT_VERSION (false)
#define IN_DEVELOPMENT (false)

#define ERROR_LOG "ERROR.LOG"   // Text filename for critical memory errors
#define DUAL_SWAP_FILES (false) //(GAME_VERSION != SHAREWARE_VERSION)				// Support for Shadowed and NonShadowed page files

extern char signonv1;
#define introscn signonv1

#ifndef SPEAR

#include "GFXV_vsi.H"
#include "MAPSvsi.H"
#include "audiovsi.hpp"

#else

#include "AUDIOSOD.H"
#include "GFXV_SOD.H"
#include "MAPSSOD.H"

#endif

//-----------------

#define GREXT "VGA"

//
//      ID Engine
//      Types.h - Generic types, #defines, etc.
//      v1.0d1
//

typedef unsigned char byte;
typedef unsigned int  word;
typedef unsigned long longword;
typedef byte*         Ptr;

typedef struct
{
    int x, y;
} Point;
typedef struct
{
    Point ul, lr;
} Rect;

#define nil (0l)

#include "ID_CA.H"
#include "ID_IN.H"
#include "ID_MM.H"
#include "ID_PM.H"
#include "ID_SD.H"
#include "ID_US.H"
#include "ID_VH.H"
#include "ID_VL.H"

#include "jm_debug.hpp"
#include "jm_error.hpp"
#include "jm_tp.h"

#include "movie.h"

void Quit(char* error, ...); // defined in user program

extern void CalcMemFree(void);

//
// replacing refresh manager with custom routines
//

#define PORTTILESWIDE 20 // all drawing takes place inside a
#define PORTTILESHIGH 13 // non displayed port of this size

#define UPDATEWIDE PORTTILESWIDE
#define UPDATEHIGH PORTTILESHIGH

#define MAXTICS 10
#define DEMOTICS 4

#define UPDATETERMINATE 0x0301

extern unsigned mapwidth, mapheight, tics, realtics;
extern bool     compatability;

extern byte*    updateptr;
extern unsigned uwidthtable[UPDATEHIGH];
extern unsigned blockstarts[UPDATEWIDE * UPDATEHIGH];

extern byte fontcolor, backcolor;

#define SETFONTCOLOR(f, b) \
    fontcolor = f;         \
    backcolor = b;

#include "3D_MENU.H"

#define CA_FarRead(h, d, s) IO_FarRead((int)h, (byte*)d, (long)s)

#endif