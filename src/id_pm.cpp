//
//	ID_PM.C
//	Id Engine's Page Manager v1.0
//	Primary coder: Jason Blochowiak
//

// #define FLASH_LOAD			// Flashes border during Disk Hits

#include "id_heads.hpp"
#include <cassert>
#pragma hdrstop

//	Main Mem specific variables
bool        MainPresent, PageManagerInstalled = false;
memptr      MainMemPages[PMMaxMainMem];
PMBlockAttr MainMemUsed[PMMaxMainMem];
int         MainPagesAvail;

//	EMS specific variables
bool EMSPresent;
word EMSAvail, EMSPagesAvail, EMSHandle,
    EMSPageFrame, EMSPhysicalPage;
EMSListStruct EMSList[EMSFrameCount];

//	XMS specific variables
bool     XMSPresent;
word     XMSAvail, XMSPagesAvail, XMSHandle;
longword XMSDriver;
int      XMSProtectPage = -1;

//	File specific variables
char PageFileName[13] = {"VSWAP."};

shadfile_t FileUsed = sd_NONE;

#if DUAL_SWAP_FILES
char AltPageFileName[13] = {"SVSWAP."};
bool ShadowsAvail        = false;
#endif

int  PageFile = -1;
word ChunksInFile;
word PMSpriteStart, PMSoundStart;

//	General usage variables
bool PMStarted,
    PMPanicMode,
    PMThrashing;
word XMSPagesUsed,
    EMSPagesUsed,
    MainPagesUsed,
    PMNumBlocks;
long            PMFrameCount;
PageListStruct *PMPages,
    *PMSegPages;

char* PM_ParmStrings[] = {"nomain", "noems", "noxms", nil};

/////////////////////////////////////////////////////////////////////////////
//
//	EMS Management code
//
/////////////////////////////////////////////////////////////////////////////

//
//	PML_MapEMS() - Maps a logical page to a physical page
//
void PML_MapEMS(word logical, word physical)
{
    assert(false);
#if 0
    _AL = physical;
    _BX = logical;
    _DX = EMSHandle;
    _AH = EMS_MAPPAGE;
    asm int EMS_INT

        if (_AH)
            PM_ERROR(PML_MAPEMS_MAPPING_FAIL);
#endif
}

#if DUAL_SWAP_FILES

//
//	PML_StartupEMS() - Sets up EMS for Page Mgr's use
//		Checks to see if EMS driver is present
//      Verifies that EMS hardware is present
//		Make sure that EMS version is 3.2 or later
//		If there's more than our minimum (2 pages) available, allocate it (up
//			to the maximum we need)
//

char EMMDriverName[9] = "EMMXXXX0";

bool PML_StartupEMS(void)
{
    int  i;
    long size;

    EMSPresent = false; // Assume that we'll fail
    EMSAvail   = 0;

    _DX = (word)EMMDriverName;
    _AX = 0x3d00;
    geninterrupt(0x21); // try to open EMMXXXX0 device
    asm jnc gothandle goto error;

gothandle:
    _BX = _AX;
    _AX = 0x4400;
    geninterrupt(0x21); // get device info
    asm jnc gotinfo;
    goto error;

gotinfo:
    asm and dx, 0x80 if (!_DX) goto error;

    _AX = 0x4407;
    geninterrupt(0x21); // get status
    asm jc error if (!_AL) goto error;

    _AH = 0x3e;
    geninterrupt(0x21); // close handle

    _AH = EMS_STATUS;
    geninterrupt(EMS_INT);
    if (_AH)
        goto error; // make sure EMS hardware is present

    _AH = EMS_VERSION;
    geninterrupt(EMS_INT);
    if (_AH || (_AL < 0x32)) // only work on EMS 3.2 or greater (silly, but...)
        goto error;

    _AH = EMS_GETFRAME;
    geninterrupt(EMS_INT);
    if (_AH)
        goto error; // find the page frame address
    EMSPageFrame = _BX;

    _AH = EMS_GETPAGES;
    geninterrupt(EMS_INT);
    if (_AH)
        goto error;
    if (_BX < 2)
        goto error; // Require at least 2 pages (32k)
    EMSAvail = _BX;

    // Don't hog all available EMS
    size = EMSAvail * (long)EMSPageSize;
    if (size - (EMSPageSize * 2) > (ChunksInFile * (long)PMPageSize))
    {
        size     = (ChunksInFile * (long)PMPageSize) + EMSPageSize;
        EMSAvail = size / EMSPageSize;
    }

    _AH = EMS_ALLOCPAGES;
    _BX = EMSAvail;
    geninterrupt(EMS_INT);
    if (_AH)
        goto error;
    EMSHandle = _DX;

    mminfo.EMSmem += EMSAvail * (long)EMSPageSize;

    // Initialize EMS mapping cache
    for (i = 0; i < EMSFrameCount; i++)
        EMSList[i].baseEMSPage = -1;

    EMSPresent = true; // We have EMS

error:
    return (EMSPresent);
}

#endif

//
//	PML_ShutdownEMS() - If EMS was used, deallocate it
//
void PML_ShutdownEMS(void)
{
    if (EMSPresent)
    {
        assert(false);
#if 0
        asm mov ah, EMS_FREEPAGES asm mov dx, [EMSHandle] asm int EMS_INT if (_AH) PM_ERROR(PML_SHUTDOWNEMS_FREE);
#endif
    }
}

/////////////////////////////////////////////////////////////////////////////
//
//	XMS Management code
//
/////////////////////////////////////////////////////////////////////////////

#if DUAL_SWAP_FILES

//
//	PML_StartupXMS() - Starts up XMS for the Page Mgr's use
//		Checks for presence of an XMS driver
//		Makes sure that there's at least a page of XMS available
//		Allocates any remaining XMS (rounded down to the nearest page size)
//
bool PML_StartupXMS(void)
{
    XMSPresent = false; // Assume failure
    XMSAvail   = 0;

    asm mov ax, 0x4300 asm int XMS_INT // Check for presence of XMS driver
        if (_AL != 0x80) goto error;

    asm mov ax, 0x4310 asm int XMS_INT // Get address of XMS driver
        asm mov[WORD PTR XMSDriver],
        bx asm mov[WORD PTR XMSDriver + 2], es // function pointer to XMS driver

                                                XMS_CALL(XMS_QUERYFREE); // Find out how much XMS is available
    XMSAvail = _AX;
    if (!_AX)
        goto error;

    XMSAvail &= ~(PMPageSizeKB - 1);   // Round off to nearest page size
    if (XMSAvail < (PMPageSizeKB * 2)) // Need at least 2 pages
        goto error;

    _DX = XMSAvail;
    XMS_CALL(XMS_ALLOC); // And do the allocation
    XMSHandle = _DX;
    if (!_AX)
    {
        XMSAvail = 0;
        goto error;
    }

    mminfo.XMSmem += XMSAvail * 1024;

    XMSPresent = true;
error:

    return (XMSPresent);
}

#endif

//
//	PML_XMSCopy() - Copies a main/EMS page to or from XMS
//		Will round an odd-length request up to the next even value
//
void PML_XMSCopy(bool toxms, byte* addr, word xmspage, word length)
{
    longword xoffset;
    struct
    {
        longword length;
        word     source_handle;
        longword source_offset;
        word     target_handle;
        longword target_offset;
    } copy;

    if (!addr)
        PM_ERROR(PML_XMSCOPY_ZERO_ADRS);

    xoffset = (longword)xmspage * PMPageSize;

    copy.length        = (length + 1) & ~1;
    copy.source_handle = toxms ? 0 : XMSHandle;
    copy.source_offset = toxms ? (long)addr : xoffset;
    copy.target_handle = toxms ? XMSHandle : 0;
    copy.target_offset = toxms ? xoffset : (long)addr;

    assert(false);
#if 0
    asm push si
        _SI = (word)&copy;
    XMS_CALL(XMS_MOVE);
    asm pop si if (!_AX)
        PM_ERROR(PML_XMSCOPY_COPY_ERROR);
#endif
}

#if 1
#define PML_CopyToXMS(s, t, l) PML_XMSCopy(true, (s), (t), (l))
#define PML_CopyFromXMS(t, s, l) PML_XMSCopy(false, (t), (s), (l))
#else
//
//	PML_CopyToXMS() - Copies the specified number of bytes from the real mode
//		segment address to the specified XMS page
//
void PML_CopyToXMS(byte* source, int targetpage, word length)
{
    PML_XMSCopy(true, source, targetpage, length);
}

//
//	PML_CopyFromXMS() - Copies the specified number of bytes from an XMS
//		page to the specified real mode address
//
void PML_CopyFromXMS(byte* target, int sourcepage, word length)
{
    PML_XMSCopy(false, target, sourcepage, length);
}
#endif

//
//	PML_ShutdownXMS()
//
void PML_ShutdownXMS(void)
{
    if (XMSPresent)
    {
        assert(false);
#if 0
        XMSPresent = false;
        _DX        = XMSHandle;
        XMS_CALL(XMS_FREE);
        if (!_AX)
            PM_ERROR(PML_SHUTDOWNXMS_FREE);
#endif
    }
}

/////////////////////////////////////////////////////////////////////////////
//
//	Main memory code
//
/////////////////////////////////////////////////////////////////////////////

//
//	PM_SetMainMemPurge() - Sets the purge level for all allocated main memory
//		blocks. This shouldn't be called directly - the PM_LockMainMem() and
//		PM_UnlockMainMem() macros should be used instead.
//
void PM_SetMainMemPurge(int level)
{
    int i;

    for (i = 0; i < PMMaxMainMem; i++)
        if (MainMemPages[i])
            MM_SetPurge(&MainMemPages[i], level);
}

//
//	PM_CheckMainMem() - If something besides the Page Mgr makes requests of
//		the Memory Mgr, some of the Page Mgr's blocks may have been purged,
//		so this function runs through the block list and checks to see if
//		any of the blocks have been purged. If so, it marks the corresponding
//		page as purged & unlocked, then goes through the block list and
//		tries to reallocate any blocks that have been purged.
//	This routine now calls PM_LockMainMem() to make sure that any allocation
//		attempts made during the block reallocation sweep don't purge any
//		of the other blocks. Because PM_LockMainMem() is called,
//		PM_UnlockMainMem() needs to be called before any other part of the
//		program makes allocation requests of the Memory Mgr.
//

#if IN_DEVELOPMENT
// char  cmm_fartext[]="PM_CHECKMAINMEM():";
#endif

void PM_CheckMainMem(void)
{
    assert(false);
#if 0
    bool            allocfailed;
    int             i, n;
    memptr*         p;
    PMBlockAttr*    used;
    PageListStruct* page;

    if (!MainPresent)
        return;

#if IN_DEVELOPMENT
//	fmprint("----------------- PM_CheckMainMem() -----------------\n");	// mdebug
#endif

    for (i = 0, page = PMPages; i < ChunksInFile; i++, page++)
    {
        n = page->mainPage;
        if (n != -1) // Is the page using main memory?
        {
            if (!MainMemPages[n]) // Yep, was the block purged?
            {
                page->mainPage = -1; // Yes, mark page as purged & unlocked
                page->locked   = pml_Unlocked;
            }
        }
    }

    // Prevent allocation attempts from purging any of our other blocks
    PM_LockMainMem();
    allocfailed = false;
    for (i = 0, p = MainMemPages, used = MainMemUsed; i < PMMaxMainMem; i++, p++, used++)
    {
        if (!*p) // If the page got purged
        {
            if (*used & pmba_Allocated) // If it was allocated
            {
                *used &= ~pmba_Allocated; // Mark as unallocated
                MainPagesAvail--;         // and decrease available count
            }

            if (*used & pmba_Used) // If it was used
            {
                *used &= ~pmba_Used; // Mark as unused
                MainPagesUsed--;     // and decrease used count
            }

            if (!allocfailed)
            {
                MM_BombOnError(false);
#if IN_DEVELOPMENT
//				gp_fartext=cmm_fartext;		// mdebug
#endif
                MM_GetPtr(p, PMPageSize); // Try to reallocate
                if (mmerror)              // If it failed,
                    allocfailed = true;   //  don't try any more allocations
                else                      // If it worked,
                {
                    *used |= pmba_Allocated; // Mark as allocated
                    MainPagesAvail++;        // and increase available count
                }
                MM_BombOnError(true);
            }
        }
    }
    if (mmerror)
        mmerror = false;
#endif
}

#if IN_DEVELOPMENT
char smm_fartext[] = "PM_STARTUPMAINMEM():";
#endif

#if DUAL_SWAP_FILES

//
//	PML_StartupMainMem() - Allocates as much main memory as is possible for
//		the Page Mgr. The memory is allocated as non-purgeable, so if it's
//		necessary to make requests of the Memory Mgr, PM_UnlockMainMem()
//		needs to be called.
//

void PML_StartupMainMem(void)
{
    int     i, n;
    memptr* p;

    MainPagesAvail = MainPagesUsed = 0;

    MM_BombOnError(false);
    for (i = 0, p = MainMemPages; i < PMMaxMainMem; i++, p++)
    {
#if IN_DEVELOPMENT
//		gp_fartext=smm_fartext;		// mdebug
#endif
        MM_GetPtr(p, PMPageSize);
        if (mmerror)
            break;

        MainPagesAvail++;
        MainMemUsed[i] = pmba_Allocated;
    }
    MM_BombOnError(true);
    if (mmerror)
        mmerror = false;
    if (MainPagesAvail < PMMinMainMem)
        PM_ERROR(PML_STARTUPMAINMEM_LOW);
    MainPresent = true;
}

#endif

//
//	PML_ShutdownMainMem() - Frees all of the main memory blocks used by the
//		Page Mgr.
//

void PML_ShutdownMainMem(void)
{
    assert(false);
#if 0
    int          i;
    memptr*      p;
    PMBlockAttr* used;

    for (i = 0, p = MainMemPages, used = MainMemUsed; i < PMMaxMainMem; used++, i++, p++)
    {
        if (*p)
            MM_FreePtr(p);

        if (*used & pmba_Allocated) // Was this puppy marked as allocated
        {
            *used &= ~pmba_Allocated; // Mark as unallocated
            MainPagesAvail--;         // and decrease available count
        }

        if (*used & pmba_Used) // Was this puppy marked as Used?
        {
            *used &= ~pmba_Used; // Mark as unused
            MainPagesUsed--;     // and decrease used count
        }
    }
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	File management code
//
/////////////////////////////////////////////////////////////////////////////

//
//	PML_ReadFromFile() - Reads some data in from the page file
//

void PML_ReadFromFile(byte* buf, long offset, word length)
{
    assert(false);
#if 0
#ifdef FLASH_LOAD
    asm mov dx, STATUS_REGISTER_1 asm in al, dx asm mov dx, ATR_INDEX asm mov al, ATR_OVERSCAN asm out dx, al asm mov al, 10 // bright green
        asm out dx,
        al
#endif

        if (!buf)
            PM_ERROR(PML_READFROMFILE_NULL);
    if (!offset)
        PM_ERROR(PML_READFROMFILE_ZERO);
    if (lseek(PageFile, offset, SEEK_SET) != offset)
        PM_ERROR(PML_READFROMFILE_SEEK);
    if (!CA_FarRead(PageFile, buf, length))
        PM_ERROR(PML_READFROMFILE_READ);

#ifdef FLASH_LOAD
    asm mov dx, STATUS_REGISTER_1 asm in al, dx asm mov dx, ATR_INDEX asm mov al, ATR_OVERSCAN asm out dx, al asm mov al, 3 // blue
        asm out    dx,
        al asm mov al, 0x20 // normal
        asm out    dx,
        al
#endif
#endif
}

//
//	PML_OpenPageFile() - Opens the page file and sets up the page info
//
void PML_OpenPageFile(char* filename)
{
    assert(false);
#if 0
    int             i;
    long            size;
    void*           buf;
    longword*       offsetptr;
    word*           lengthptr;
    PageListStruct* page;

    PageFile = open(filename, O_RDONLY + O_BINARY);

    if (PageFile == -1)
        PM_ERROR(PML_OPENPAGEFILE_OPEN);

    // Read in header variables
    read(PageFile, &ChunksInFile, sizeof(ChunksInFile));
    read(PageFile, &PMSpriteStart, sizeof(PMSpriteStart));
    read(PageFile, &PMSoundStart, sizeof(PMSoundStart));

    // Allocate and clear the page list

    PMNumBlocks = ChunksInFile;
    MM_GetPtr(&(memptr)PMSegPages, sizeof(PageListStruct) * PMNumBlocks);
    MM_SetLock(&(memptr)PMSegPages, true);
    PMPages = (PageListStruct*)PMSegPages;
    _fmemset(PMPages, 0, sizeof(PageListStruct) * PMNumBlocks);

    // Read in the chunk offsets

    size = sizeof(longword) * ChunksInFile;
    MM_GetPtr(&buf, size);
    if (!CA_FarRead(PageFile, (byte*)buf, size))
        PM_ERROR(PML_OPENPAGEFILE_OFF);
    offsetptr = (longword*)buf;
    for (i = 0, page = PMPages; i < ChunksInFile; i++, page++)
        page->offset = *offsetptr++;
    MM_FreePtr(&buf);

    // Read in the chunk lengths

    size = sizeof(word) * ChunksInFile;
    MM_GetPtr(&buf, size);
    if (!CA_FarRead(PageFile, (byte*)buf, size))
        PM_ERROR(PML_OPENPAGEFILE_LEN);
    lengthptr = (word*)buf;
    for (i = 0, page = PMPages; i < ChunksInFile; i++, page++)
        page->length = *lengthptr++;
    MM_FreePtr(&buf);
#endif
}

//
//  PML_ClosePageFile() - Closes the page file
//
void PML_ClosePageFile(void)
{
    assert(false);
#if 0
    if (PageFile != -1)
        close(PageFile);
    if (PMSegPages)
    {
        MM_SetLock(&(memptr)PMSegPages, false);
        MM_FreePtr(&(void*)PMSegPages);
    }
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Allocation, etc., code
//
/////////////////////////////////////////////////////////////////////////////

//
//	PML_GetEMSAddress()
//
// 		Page is in EMS, so figure out which EMS physical page should be used
//  		to map our page in. If normal page, use EMS physical page 3, else
//  		use the physical page specified by the lock type
//
#if 1
#pragma argsused // DEBUG - remove lock parameter
memptr
PML_GetEMSAddress(int page, PMLockType lock)
{
    assert(false);
#if 0
    int  i, emspage;
    word emsoff, emsbase, offset;

    emsoff  = page & (PMEMSSubPage - 1);
    emsbase = page - emsoff;

    emspage = -1;
    // See if this page is already mapped in
    for (i = 0; i < EMSFrameCount; i++)
    {
        if (EMSList[i].baseEMSPage == emsbase)
        {
            emspage = i; // Yep - don't do a redundant remapping
            break;
        }
    }

    // If page isn't already mapped in, find LRU EMS frame, and use it
    if (emspage == -1)
    {
        longword last = MAXLONG;
        for (i = 0; i < EMSFrameCount; i++)
        {
            if (EMSList[i].lastHit < last)
            {
                emspage = i;
                last    = EMSList[i].lastHit;
            }
        }

        EMSList[emspage].baseEMSPage = emsbase;
        PML_MapEMS(page / PMEMSSubPage, emspage);
    }

    if (emspage == -1)
        PM_ERROR(PML_GETEMSADDRESS_FIND);

    EMSList[emspage].lastHit = PMFrameCount;
    offset                   = emspage * EMSPageSizeSeg;
    offset += emsoff * PMPageSizeSeg;
    return ((memptr)(EMSPageFrame + offset));
#endif
}
#else
memptr
PML_GetEMSAddress(int page, PMLockType lock)
{
    word emspage;

    emspage = (lock < pml_EMSLock) ? 3 : (lock - pml_EMSLock);

    PML_MapEMS(page / PMEMSSubPage, emspage);

    return ((memptr)(EMSPageFrame + (emspage * EMSPageSizeSeg) + ((page & (PMEMSSubPage - 1)) * PMPageSizeSeg)));
}
#endif

//
//	PM_GetPageAddress() - Returns the address of a given page
//		Maps in EMS if necessary
//		Returns nil if block isn't cached into Main Memory or EMS
//
//
memptr
PM_GetPageAddress(int pagenum)
{
    PageListStruct* page;

    page = &PMPages[pagenum];
    if (page->mainPage != -1)
        return (MainMemPages[page->mainPage]);
    else if (page->emsPage != -1)
        return (PML_GetEMSAddress(page->emsPage, page->locked));
    else
        return (nil);
}

//
//	PML_GiveLRUPage() - Returns the page # of the least recently used
//		present & unlocked main/EMS page (or main page if mainonly is true)
//
int PML_GiveLRUPage(bool mainonly)
{
    assert(false);
#if 0
    int             i, lru;
    long            last;
    PageListStruct* page;

    for (i = 0, page = PMPages, lru = -1, last = MAXLONG; i < ChunksInFile; i++, page++)
    {
        if (
            (page->lastHit < last) && ((page->emsPage != -1) || (page->mainPage != -1)) && (page->locked == pml_Unlocked) && (!(mainonly && (page->mainPage == -1))))
        {
            last = page->lastHit;
            lru  = i;
        }
    }

    if (lru == -1)
        PM_ERROR(PML_GIVELRUPAGE_SEARCH);
    return (lru);
#endif
}

//
//	PML_GiveLRUXMSPage() - Returns the page # of the least recently used
//		(and present) XMS page.
//	This routine won't return the XMS page protected (by XMSProtectPage)
//
int PML_GiveLRUXMSPage(void)
{
    assert(false);
#if 0
    int             i, lru;
    long            last;
    PageListStruct* page;

    for (i = 0, page = PMPages, lru = -1, last = MAXLONG; i < ChunksInFile; i++, page++)
    {
        if (
            (page->xmsPage != -1) && (page->lastHit < last) && (i != XMSProtectPage))
        {
            last = page->lastHit;
            lru  = i;
        }
    }
    return (lru);
#endif
}

//
//	PML_PutPageInXMS() - If page isn't in XMS, find LRU XMS page and replace
//		it with the main/EMS page
//
void PML_PutPageInXMS(int pagenum)
{
    assert(false);
#if 0
    int             usexms;
    PageListStruct* page;

    if (!XMSPresent)
        return;

    page = &PMPages[pagenum];
    if (page->xmsPage != -1)
        return; // Already in XMS

    if (XMSPagesUsed < XMSPagesAvail)
        page->xmsPage = XMSPagesUsed++;
    else
    {
        usexms = PML_GiveLRUXMSPage();
        if (usexms == -1)
            PM_ERROR(PML_PUTPAGEINXMS_NO_LRU);
        page->xmsPage           = PMPages[usexms].xmsPage;
        PMPages[usexms].xmsPage = -1;
    }
    PML_CopyToXMS(PM_GetPageAddress(pagenum), page->xmsPage, page->length);
#endif
}

//
//	PML_TransferPageSpace() - A page is being replaced, so give the new page
//		the old one's address space. Returns the address of the new page.
//
memptr
PML_TransferPageSpace(int orig, int new_)
{
    memptr          addr;
    PageListStruct *origpage, *newpage;

    if (orig == new_)
        PM_ERROR(PML_TRANSFERPAGE_IDENT);

    origpage = &PMPages[orig];
    newpage  = &PMPages[new_];

    if (origpage->locked != pml_Unlocked)
        PM_ERROR(PML_TRANSFERPAGE_KILL);

    if ((origpage->emsPage == -1) && (origpage->mainPage == -1))
        PM_ERROR(PML_TRANSFERPAGE_REUSE);

    // Copy page that's about to be purged into XMS
    PML_PutPageInXMS(orig);

    // Get the address, and force EMS into a physical page if necessary
    addr = PM_GetPageAddress(orig);

    // Steal the address
    newpage->emsPage  = origpage->emsPage;
    newpage->mainPage = origpage->mainPage;

    // Mark replaced page as purged
    origpage->mainPage = origpage->emsPage = -1;

    if (!addr)
    {
        PM_ERROR(PML_TRANSFERPAGE_ZERO);
        //		addr = 1;
    }

    return (addr);
}

//
//	PML_GetAPageBuffer() - A page buffer is needed. Either get it from the
//		main/EMS free pool, or use PML_GiveLRUPage() to find which page to
//		steal the buffer from. Returns a  pointer to the page buffer, and
//		sets the fields inside the given page structure appropriately.
//		If mainonly is true, free EMS will be ignored, and only main pages
//		will be looked at by PML_GiveLRUPage().
//
byte* PML_GetAPageBuffer(int pagenum, bool mainonly)
{
    assert(false);
#if 0
    byte*           addr = nil;
    int             i, n;
    PMBlockAttr*    used;
    PageListStruct* page;

    page = &PMPages[pagenum];
    if ((EMSPagesUsed < EMSPagesAvail) && !mainonly)
    {
        // There's remaining EMS - use it
        page->emsPage = EMSPagesUsed++;
        addr          = PML_GetEMSAddress(page->emsPage, page->locked);
    }
    else if (MainPagesUsed < MainPagesAvail)
    {
        // There's remaining main memory - use it
        for (i = 0, n = -1, used = MainMemUsed; i < PMMaxMainMem; i++, used++)
        {
            if ((*used & pmba_Allocated) && !(*used & pmba_Used))
            {
                n = i;
                *used |= pmba_Used;
                break;
            }
        }
        if (n == -1)
            PM_ERROR(PML_GETPAGEBUFFER_LIED);
        addr = MainMemPages[n];
        if (!addr)
            PM_ERROR(PML_GETPAGEBUFFER_PURGE);
        page->mainPage = n;
        MainPagesUsed++;
    }
    else
        addr = PML_TransferPageSpace(PML_GiveLRUPage(mainonly), pagenum);

    if (!addr)
        PM_ERROR(PML_GETPAGEBUFFER_SEARC);
    return (addr);
#endif
}

//
//	PML_GetPageFromXMS() - If page is in XMS, find LRU main/EMS page and
//		replace it with the page from XMS. If mainonly is true, will only
//		search for LRU main page.
//	XMSProtectPage is set to the page to be retrieved from XMS, so that if
//		the page from which we're stealing the main/EMS from isn't in XMS,
//		it won't copy over the page that we're trying to get from XMS.
//		(pages that are being purged are copied into XMS, if possible)
//
memptr
PML_GetPageFromXMS(int pagenum, bool mainonly)
{
    assert(false);
#if 0
    byte*           checkaddr;
    memptr          addr = nil;
    PageListStruct* page;

    page = &PMPages[pagenum];
    if (XMSPresent && (page->xmsPage != -1))
    {
        XMSProtectPage = pagenum;
        checkaddr      = PML_GetAPageBuffer(pagenum, mainonly);
        if (FP_OFF(checkaddr))
            PM_ERROR(PML_GETPAGEFROMXMS_SEG);
        addr = (memptr)FP_SEG(checkaddr);
        PML_CopyFromXMS(addr, page->xmsPage, page->length);
        XMSProtectPage = -1;
    }

    return (addr);
#endif
}

//
//	PML_LoadPage() - A page is not in main/EMS memory, and it's not in XMS.
//		Load it into either main or EMS. If mainonly is true, the page will
//		only be loaded into main.
//
void PML_LoadPage(int pagenum, bool mainonly)
{
    byte*           addr;
    PageListStruct* page;

    addr = PML_GetAPageBuffer(pagenum, mainonly);
    page = &PMPages[pagenum];
    PML_ReadFromFile(addr, page->offset, page->length);
}

//
//	PM_GetPage() - Returns the address of the page, loading it if necessary
//		First, check if in Main Memory or EMS
//		Then, check XMS
//		If not in XMS, load into Main Memory or EMS
//
#pragma warn - pia
memptr
PM_GetPage(int pagenum)
{
    assert(false);
#if 0
    memptr result;

    if (pagenum >= ChunksInFile)
        PM_ERROR(PM_GETPAGE_BAD_PAGE);

#if 0 // for debugging
asm	mov	dx,STATUS_REGISTER_1
asm	in	al,dx
asm	mov	dx,ATR_INDEX
asm	mov	al,ATR_OVERSCAN
asm	out	dx,al
asm	mov	al,10	// bright green
asm	out	dx,al
#endif

    if (!(result = PM_GetPageAddress(pagenum)))
    {
        bool mainonly = (pagenum >= PMSoundStart);
        if (!PMPages[pagenum].offset) // JDC: sparse page
            PM_ERROR(PM_GETPAGE_SPARSE_PAGE);
        if (!(result = PML_GetPageFromXMS(pagenum, mainonly)))
        {
            if (PMPages[pagenum].lastHit == PMFrameCount)
                PMThrashing++;

            PML_LoadPage(pagenum, mainonly);
            result = PM_GetPageAddress(pagenum);
        }
    }
    PMPages[pagenum].lastHit = PMFrameCount;

#if 0 // for debugging
asm	mov	dx,STATUS_REGISTER_1
asm	in	al,dx
asm	mov	dx,ATR_INDEX
asm	mov	al,ATR_OVERSCAN
asm	out	dx,al
asm	mov	al,3	// blue
asm	out	dx,al
asm	mov	al,0x20	// normal
asm	out	dx,al
#endif

    return (result);
#endif
}
#pragma warn + pia

//
//	PM_SetPageLock() - Sets the lock type on a given page
//		pml_Unlocked: Normal, page can be purged
//		pml_Locked: Cannot be purged
//		pml_EMS?: Same as pml_Locked, but if in EMS, use the physical page
//					specified when returning the address. For sound stuff.
//
void PM_SetPageLock(int pagenum, PMLockType lock)
{
    if (pagenum < PMSoundStart)
        PM_ERROR(PM_SETPAGELOCK_NON_SND);

    PMPages[pagenum].locked = lock;
}

//
//	PM_Preload() - Loads as many pages as possible into all types of memory.
//		Calls the update function after each load, indicating the current
//		page, and the total pages that need to be loaded (for thermometer).
//
void PM_Preload(void (*update)(word current, word total))
{
    int i, j,
        page, oogypage;
    word current, total,
        totalnonxms, totalxms,
        mainfree, maintotal,
        emsfree, emstotal,
        xmsfree, xmstotal;
    memptr          addr;
    PageListStruct* p;

    mainfree = (MainPagesAvail - MainPagesUsed) + (EMSPagesAvail - EMSPagesUsed);
    xmsfree  = (XMSPagesAvail - XMSPagesUsed);

    xmstotal = maintotal = 0;

    for (i = 0; i < ChunksInFile; i++)
    {
        if (!PMPages[i].offset)
            continue; // sparse

        if (PMPages[i].emsPage != -1 || PMPages[i].mainPage != -1)
            continue; // already in main mem

        if (mainfree)
        {
            maintotal++;
            mainfree--;
        }
        else if (xmsfree && (PMPages[i].xmsPage == -1))
        {
            xmstotal++;
            xmsfree--;
        }
    }

    total = maintotal + xmstotal;

    if (!total)
    {
        update(1, 1);
        return;
    }

    page    = 0;
    current = 0;

    //
    // cache main/ems blocks
    //
    while (maintotal)
    {
        while (!PMPages[page].offset || PMPages[page].mainPage != -1 || PMPages[page].emsPage != -1)
            page++;

        if (page >= ChunksInFile)
            PM_ERROR(PM_PRELOAD_PAGES_OV);

        PM_GetPage(page);

        page++;
        current++;
        maintotal--;
        update(current, total);
    }

    //
    // load stuff to XMS
    //
    if (xmstotal)
    {
        for (oogypage = 0; PMPages[oogypage].mainPage == -1; oogypage++)
            ;
        addr = PM_GetPage(oogypage);
        if (!addr)
            PM_ERROR(PM_PRELOAD_XMS_FAIL);

        while (xmstotal)
        {
            while (!PMPages[page].offset || PMPages[page].xmsPage != -1)
                page++;

            if (page >= ChunksInFile)
                PM_ERROR(PM_PRELOAD_PAGES_OV);

            p = &PMPages[page];

            p->xmsPage = XMSPagesUsed++;
            if (XMSPagesUsed > XMSPagesAvail)
                PM_ERROR(PM_PRELOAD_EXCEED);
            if (p->length > PMPageSize)
                PM_ERROR(PM_PRELOAD_TOO_LONG);

            PML_ReadFromFile((byte*)addr, p->offset, p->length);
            PML_CopyToXMS((byte*)addr, p->xmsPage, p->length);

            page++;
            current++;
            xmstotal--;
            update(current, total);
        }

        p = &PMPages[oogypage];
        PML_ReadFromFile((byte*)addr, p->offset, p->length);
    }

    update(total, total);
}

/////////////////////////////////////////////////////////////////////////////
//
//	General code
//
/////////////////////////////////////////////////////////////////////////////

//
//	PM_NextFrame() - Increments the frame counter and adjusts the thrash
//		avoidence variables
//
//		If currently in panic mode (to avoid thrashing), check to see if the
//			appropriate number of frames have passed since the last time that
//			we would have thrashed. If so, take us out of panic mode.
//
//
void PM_NextFrame(void)
{
    assert(false);
#if 0
    int i;

    // Frame count overrun - kill the LRU hit entries & reset frame count
    if (++PMFrameCount >= MAXLONG - 4)
    {
        for (i = 0; i < PMNumBlocks; i++)
            PMPages[i].lastHit = 0;
        PMFrameCount = 0;
    }

#if 0
	for (i = 0;i < PMSoundStart;i++)
	{
		if (PMPages[i].locked)
		{
			char buf[40];
			sprintf(buf,"PM_NextFrame: Page %d is locked",i);
			Quit(buf);
		}
	}
#endif

    if (PMPanicMode)
    {
        // DEBUG - set border color
        if ((!PMThrashing) && (!--PMPanicMode))
        {
            // DEBUG - reset border color
        }
    }
    if (PMThrashing >= PMThrashThreshold)
        PMPanicMode = PMUnThrashThreshold;
    PMThrashing = false;
#endif
}

//
//	PM_Reset() - Sets up caching structures
//
void PM_Reset(void)
{
    assert(false);
#if 0
    int             i;
    PageListStruct* page;

    XMSPagesAvail = XMSAvail / PMPageSizeKB;

    EMSPagesAvail   = EMSAvail * (EMSPageSizeKB / PMPageSizeKB);
    EMSPhysicalPage = 0;

    MainPagesUsed = EMSPagesUsed = XMSPagesUsed = 0;

    PMPanicMode = false;

    // Initialize page list
    for (i = 0, page = PMPages; i < PMNumBlocks; i++, page++)
    {
        page->mainPage = -1;
        page->emsPage  = -1;
        page->xmsPage  = -1;
        page->locked   = false;
    }
#endif
}

#if DUAL_SWAP_FILES
//
//	PM_Startup() - Start up the Page Mgr
//
void PM_Startup(void)
{
    bool nomain, noems, noxms;
    int  i;

    if (PMStarted)
        return;

    nomain = noems = noxms = false;
    for (i = 1; i < _argc; i++)
    {
        switch (US_CheckParm(_argv[i], PM_ParmStrings))
        {
        case 0:
            nomain = true;
            break;
        case 1:
            noems = true;
            break;
        case 2:
            noxms = true;
            break;
        }
    }

    OpenPageFile();

    if (!noems)
        PML_StartupEMS();
    if (!noxms)
        PML_StartupXMS();

    if (nomain && !EMSPresent)
        PM_ERROR(PM_STARTUP_NO_MAIN_EMS);
    else
        PML_StartupMainMem();

    PM_Reset();

    PMStarted = true;
}

#endif

//
//	PM_Shutdown() - Shut down the Page Mgr
//
void PM_Shutdown(void)
{
    PML_ShutdownXMS();
    PML_ShutdownEMS();

    if (!PMStarted)
        return;

    PMStarted = false;

    PML_ClosePageFile();

    PML_ShutdownMainMem();
}
