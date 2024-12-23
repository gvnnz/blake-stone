#ifndef ID_MM_HPP
#define ID_MM_HPP

// ID_MM.H

#ifndef __ID_CA__

#define __ID_CA__

#define SAVENEARHEAP 0x400 // space to leave in data segment
#define SAVEFARHEAP 0      // space to leave in  heap

#define BUFFERSIZE 0x1000 // miscelanious, allways available buffer

#define MAXBLOCKS 600

//--------

#define EMS_INT 0x67

#define EMS_STATUS 0x40
#define EMS_GETFRAME 0x41
#define EMS_GETPAGES 0x42
#define EMS_ALLOCPAGES 0x43
#define EMS_MAPPAGE 0x44
#define EMS_FREEPAGES 0x45
#define EMS_VERSION 0x46

//--------

#define XMS_INT 0x2f
#define XMS_CALL(v) \
    _AH = (v);      \
    asm call[DWORD PTR XMSDriver]

#define XMS_VERSION 0x00

#define XMS_ALLOCHMA 0x01
#define XMS_FREEHMA 0x02

#define XMS_GENABLEA20 0x03
#define XMS_GDISABLEA20 0x04
#define XMS_LENABLEA20 0x05
#define XMS_LDISABLEA20 0x06
#define XMS_QUERYA20 0x07

#define XMS_QUERYFREE 0x08
#define XMS_ALLOC 0x09
#define XMS_FREE 0x0A
#define XMS_MOVE 0x0B
#define XMS_LOCK 0x0C
#define XMS_UNLOCK 0x0D
#define XMS_GETINFO 0x0E
#define XMS_RESIZE 0x0F

#define XMS_ALLOCUMB 0x10
#define XMS_FREEUMB 0x11

//==========================================================================

typedef void* memptr;

typedef struct
{
    long nearheap, farheap, EMSmem, XMSmem, mainmem;
} mminfotype;

//==========================================================================

extern unsigned   blockcount;
extern mminfotype mminfo;
extern memptr     bufferseg;
extern bool       mmerror;
// unsigned __SEGS_AVAILABLE__;

extern void (*beforesort)(void);
extern void (*aftersort)(void);

extern char* gp_fartext;

//==========================================================================

void MM_Startup(void);
void MM_Shutdown(void);
void MM_MapEMS(void);

void MM_GetPtr(memptr* baseptr, unsigned long size);
void MM_FreePtr(memptr* baseptr);

void MM_SetPurge(memptr* baseptr, int purge);
void MM_SetLock(memptr* baseptr, bool locked);
void MM_SortMem(void);

void MM_ShowMemory(void);

long MM_UnusedMemory(void);
long MM_TotalFree(void);
long MM_LargestAvail(void);

void MM_BombOnError(bool bomb);

void MML_UseSpace(unsigned segstart, unsigned seglength);

#define LOCKBIT 0x80 // if set in attributes, block cannot be moved

#if IN_DEVELOPMENT
#define GETNEWBLOCK            \
    {                          \
        if (!mmfree)           \
            MML_ClearBlock();  \
        mmnew  = mmfree;       \
        mmfree = mmfree->next; \
        blockcount++;          \
    }
#else
#define GETNEWBLOCK            \
    {                          \
        if (!mmfree)           \
            MML_ClearBlock();  \
        mmnew  = mmfree;       \
        mmfree = mmfree->next; \
    }
#endif

typedef struct mmblockstruct
{
    unsigned              start, length;
    unsigned              attributes;
    memptr*               useptr; // pointer to the segment start
    struct mmblockstruct* next;
} mmblocktype;

#endif
#endif