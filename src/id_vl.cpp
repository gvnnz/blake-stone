// ID_VL.C

#include "id_vl.hpp"
#include "id_head.hpp"
#include <cassert>
#pragma hdrstop

//
// SC_INDEX is expected to stay at SC_MAPMASK for proper operation
//

unsigned bufferofs;
unsigned displayofs, pelpan;

unsigned screenseg = SCREENSEG; // set to 0xa000 for asm convenience

unsigned linewidth;
unsigned ylookup[MAXSCANLINES];

bool     screenfaded;
unsigned bordercolor;

// bool		fastpalette;				// if true, use outsb to set

byte palette1[256][3], palette2[256][3];

//===========================================================================

// asm

int  VL_VideoID(void);
void VL_SetCRTC(int crtc);
void VL_SetScreen(int crtc, int pelpan);
void VL_WaitVBL(int vbls);

//===========================================================================

/*
=======================
=
= VL_Startup
=
=======================
*/

#if 0
void	VL_Startup (void)
{
	if ( !MS_CheckParm ("HIDDENCARD") && VL_VideoID () != 5)
		MS_Quit ("You need a VGA graphics card to run this!");

	asm	cld;				// all string instructions assume forward
}

#endif

/*
=======================
=
= VL_Shutdown
=
=======================
*/

void VL_Shutdown(void)
{
    VL_SetTextMode();
}

#if RESTART_PICTURE_PAUSE

/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void VL_SetVGAPlaneMode(void)
{
    asm mov ax, 0x13 asm int 0x10 VL_DePlaneVGA();
    VGAMAPMASK(15);
    VL_SetLineWidth(40);
}

#endif

/*
=======================
=
= VL_SetTextMode
=
=======================
*/

void VL_SetTextMode(void)
{
    assert(false);
#if 0
    asm mov ax, 3 asm int 0x10
#endif
}

//===========================================================================

#if RESTART_PICTURE_PAUSE

/*
=================
=
= VL_ClearVideo
=
= Fill the entire video buffer with a given color
=
=================
*/

void VL_ClearVideo(byte color)
{
    asm mov dx, GC_INDEX asm mov al, GC_MODE asm out dx, al asm inc dx asm in al, dx asm and al, 0xfc // write mode 0 to store directly to video
        asm out dx,
        al

        asm mov          dx,
        SC_INDEX asm mov ax, SC_MAPMASK + 15 * 256 asm out dx, ax // write through all four planes

        asm mov           ax,
        SCREENSEG asm mov es, ax asm mov al, [color] asm mov ah, al asm mov cx, 0x8000 // 0x8000 words, clearing 8 video bytes/word
                                                                                    asm xor
                                                                                    di,
        di asm rep stosw
}

/*
=============================================================================

            VGA REGISTER MANAGEMENT ROUTINES

=============================================================================
*/

/*
=================
=
= VL_DePlaneVGA
=
=================
*/

void VL_DePlaneVGA(void)
{

    //
    // change CPU addressing to non linear mode
    //

    //
    // turn off chain 4 and odd/even
    //
    outportb(SC_INDEX, SC_MEMMODE);
    outportb(SC_INDEX + 1, (inportb(SC_INDEX + 1) & ~8) | 4);

    outportb(SC_INDEX, SC_MAPMASK); // leave this set throughought

    //
    // turn off odd/even and set write mode 0
    //
    outportb(GC_INDEX, GC_MODE);
    outportb(GC_INDEX + 1, inportb(GC_INDEX + 1) & ~0x13);

    //
    // turn off chain
    //
    outportb(GC_INDEX, GC_MISCELLANEOUS);
    outportb(GC_INDEX + 1, inportb(GC_INDEX + 1) & ~2);

    //
    // clear the entire buffer space, because int 10h only did 16 k / plane
    //
    VL_ClearVideo(0);

    //
    // change CRTC scanning from doubleword to byte mode, allowing >64k scans
    //
    outportb(CRTC_INDEX, CRTC_UNDERLINE);
    outportb(CRTC_INDEX + 1, inportb(CRTC_INDEX + 1) & ~0x40);

    outportb(CRTC_INDEX, CRTC_MODE);
    outportb(CRTC_INDEX + 1, inportb(CRTC_INDEX + 1) | 0x40);
}

//===========================================================================

/*
====================
=
= VL_SetLineWidth
=
= Line witdh is in WORDS, 40 words is normal width for vgaplanegr
=
====================
*/

void VL_SetLineWidth(unsigned width)
{
    int i, offset;

    //
    // set wide virtual screen
    //
    outport(CRTC_INDEX, CRTC_OFFSET + width * 256);

    //
    // set up lookup tables
    //
    linewidth = width * 2;

    offset = 0;

    for (i = 0; i < MAXSCANLINES; i++)
    {
        ylookup[i] = offset;
        offset += linewidth;
    }
}

#endif

#if 0

/*
====================
=
= VL_SetSplitScreen
=
====================
*/

void VL_SetSplitScreen (int linenum)
{
	VL_WaitVBL (1);
	linenum=linenum*2-1;
	outportb (CRTC_INDEX,CRTC_LINECOMPARE);
	outportb (CRTC_INDEX+1,linenum % 256);
	outportb (CRTC_INDEX,CRTC_OVERFLOW);
	outportb (CRTC_INDEX+1, 1+16*(linenum/256));
	outportb (CRTC_INDEX,CRTC_MAXSCANLINE);
	outportb (CRTC_INDEX+1,inportb(CRTC_INDEX+1) & (255-64));
}

#endif

/*
=============================================================================

                        PALETTE OPS

        To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/

/*
=================
=
= VL_FillPalette
=
=================
*/

void VL_FillPalette(int red, int green, int blue)
{
    assert(false);
#if 0
    int i;

    outportb(PEL_WRITE_ADR, 0);
    for (i = 0; i < 256; i++)
    {
        outportb(PEL_DATA, red);
        outportb(PEL_DATA, green);
        outportb(PEL_DATA, blue);
    }
#endif
}

//===========================================================================

#if 0		
/*
=================
=
= VL_SetColor
=
=================
*/

void VL_SetColor	(int color, int red, int green, int blue)
{
	outportb (PEL_WRITE_ADR,color);
	outportb (PEL_DATA,red);
	outportb (PEL_DATA,green);
	outportb (PEL_DATA,blue);
}
#endif

//===========================================================================

#if 0	  

/*
=================
=
= VL_GetColor
=
=================
*/

void VL_GetColor	(int color, int *red, int *green, int *blue)
{
	outportb (PEL_READ_ADR,color);
	*red = inportb (PEL_DATA);
	*green = inportb (PEL_DATA);
	*blue = inportb (PEL_DATA);
}

#endif

//===========================================================================

/*
=================
=
= VL_SetPalette
=
= If fast palette setting has been tested for, it is used
= (some cards don't like outsb palette setting)
=
=================
*/

void VL_SetPalette(byte firstreg, unsigned numregs, byte* palette)
{
    assert(false);
#if 0
    int i; //,three=3;

    VL_WaitVBL(1);

    asm mov dx, PEL_WRITE_ADR asm mov al, [firstreg] asm out dx, al asm mov dx, PEL_DATA asm lds si, [palette]

#if 0
	asm	test	[ss:fastpalette],1
	asm	jz	slowset

// set palette fast for cards that can take it
//
	asm	mov	ax,[numregs]
	asm	mul	[three]
	asm	mov	cx,ax
	asm	rep 	outsb
	asm	jmp	done
#endif

        // set palette slowly for some video cards
        //
        slowset : asm mov                                  cx,
                  [numregs] setloop : asm lodsb asm out    dx,
                                      al asm lodsb asm out dx,
                                      al asm lodsb asm out dx,
                                      al asm loop setloop

                                          done : asm mov    ax,
                                                 ss asm mov ds,
                                                 ax
#endif
}

//===========================================================================

/*
=================
=
= VL_GetPalette
=
= This does not use the port string instructions,
= due to some incompatabilities
=
=================
*/

void VL_GetPalette(byte firstreg, unsigned numregs, byte* palette)
{
    assert(false);
#if 0
    int i;

    numregs *= 3;

    VL_WaitVBL(1);
    outportb(PEL_READ_ADR, firstreg);
    for (i = 0; i < numregs; i++)
        *palette++ = inportb(PEL_DATA);
#endif
}

//===========================================================================

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut(int start, int end, int red, int green, int blue, int steps)
{
    assert(false);
#if 0
    int   i, j, orig, delta;
    byte *origptr, *newptr;

    VL_GetPalette(0, 256, &palette1[0][0]);
    _fmemcpy(palette2, palette1, 768);

    //
    // fade through intermediate frames
    //
    for (i = 0; i < steps; i++)
    {
        origptr = &palette1[start][0];
        newptr  = &palette2[start][0];
        for (j = start; j <= end; j++)
        {
            orig      = *origptr++;
            delta     = red - orig;
            *newptr++ = orig + delta * i / steps;
            orig      = *origptr++;
            delta     = green - orig;
            *newptr++ = orig + delta * i / steps;
            orig      = *origptr++;
            delta     = blue - orig;
            *newptr++ = orig + delta * i / steps;
        }

        VL_SetPalette(0, 256, &palette2[0][0]);
    }

    //
    // final color
    //
    VL_FillPalette(red, green, blue);

    screenfaded = true;
#endif
}

/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn(int start, int end, byte* palette, int steps)
{
    assert(false);
#if 0
    int i, j, delta;

    VL_GetPalette(0, 256, &palette1[0][0]);
    _fmemcpy(&palette2[0][0], &palette1[0][0], sizeof(palette1));

    start *= 3;
    end = end * 3 + 2;

    //
    // fade through intermediate frames
    //
    for (i = 0; i < steps; i++)
    {
        for (j = start; j <= end; j++)
        {
            delta          = palette[j] - palette1[0][j];
            palette2[0][j] = palette1[0][j] + delta * i / steps;
        }

        VL_SetPalette(0, 256, &palette2[0][0]);
    }

    //
    // final color
    //
    VL_SetPalette(0, 256, palette);
    screenfaded = false;
#endif
}

//------------------------------------------------------------------------
// VL_SetPaletteIntensity()
//------------------------------------------------------------------------
void VL_SetPaletteIntensity(int start, int end, byte* palette, char intensity)
{
    short loop;
    char  red, green, blue;
    byte* cmap = &palette1[0][0] + start * 3;

    intensity = 63 - intensity;
    for (loop = start; loop <= end; loop++)
    {
        red = *palette++ - intensity;
        if (red < 0)
            red = 0;
        *cmap++ = red;

        green = *palette++ - intensity;
        if (green < 0)
            green = 0;
        *cmap++ = green;

        blue = *palette++ - intensity;
        if (blue < 0)
            blue = 0;
        *cmap++ = blue;
    }

    VL_SetPalette(start, end - start + 1, &palette1[0][0]);
}

#if 0
//------------------------------------------------------------------------
// FadeOut()
//------------------------------------------------------------------------
void FadeOut(char *colormap, short numcolors, char delay)
{
	short loop;

	for (loop=63; loop>=0; loop--)
	{
		SetPaletteIntensity(colormap,numcolors,loop);
		if (delay)
			WaitVBL(delay);
	}
}

//------------------------------------------------------------------------
// FadeIn()
//------------------------------------------------------------------------
void FadeIn(char *colormap, short numcolors, char delay)
{
	short loop;

	for (loop=0; loop<64; loop++)
	{
		SetPaletteIntensity(colormap,numcolors,loop);
		if (delay)
			WaitVBL(delay);
	}
}
#endif

#if 0
/*
=================
=
= VL_TestPaletteSet
=
= Sets the palette with outsb, then reads it in and compares
= If it compares ok, fastpalette is set to true.
=
=================
*/

void VL_TestPaletteSet (void)
{
	int	i;

	for (i=0;i<768;i++)
		palette1[0][i] = i;

	fastpalette = true;
	VL_SetPalette (0,256,&palette1[0][0]);
	VL_GetPalette (0,256,&palette2[0][0]);
	if (_fmemcmp (&palette1[0][0],&palette2[0][0],768))
		fastpalette = false;
}
#endif

/*
==================
=
= VL_ColorBorder
=
==================
*/

void VL_ColorBorder(int color)
{
    assert(false);
#if 0
    _AH = 0x10;
    _AL = 1;
    _BH = color;
    geninterrupt(0x10);
    bordercolor = color;
#endif
}

/*
=============================================================================

                            PIXEL OPS

=============================================================================
*/

byte pixmasks[4]   = {1, 2, 4, 8};
byte leftmasks[4]  = {15, 14, 12, 8};
byte rightmasks[4] = {1, 3, 7, 15};

/*
=================
=
= VL_Plot
=
=================
*/

void VL_Plot(int x, int y, int color)
{
    assert(false);
#if 0
    byte mask;

    mask = pixmasks[x & 3];
    VGAMAPMASK(mask);
    *(byte*)MK_FP(SCREENSEG, bufferofs + (ylookup[y] + (x >> 2))) = color;
    VGAMAPMASK(15);
#endif
}

/*
=================
=
= VL_Hlin
=
=================
*/

void VL_Hlin(unsigned x, unsigned y, unsigned width, unsigned color)
{
    assert(false);
#if 0
    unsigned xbyte;
    byte*    dest;
    byte     leftmask, rightmask;
    int      midbytes;

    xbyte     = x >> 2;
    leftmask  = leftmasks[x & 3];
    rightmask = rightmasks[(x + width - 1) & 3];
    midbytes  = ((x + width + 3) >> 2) - xbyte - 2;

    dest = MK_FP(SCREENSEG, bufferofs + ylookup[y] + xbyte);

    if (midbytes < 0)
    {
        // all in one byte
        VGAMAPMASK(leftmask & rightmask);
        *dest = color;
        VGAMAPMASK(15);
        return;
    }

    VGAMAPMASK(leftmask);
    *dest++ = color;

    VGAMAPMASK(15);
    _fmemset(dest, color, midbytes);
    dest += midbytes;

    VGAMAPMASK(rightmask);
    *dest = color;

    VGAMAPMASK(15);
#endif
}

/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin(int x, int y, int height, int color)
{
    assert(false);
#if 0
    byte *dest, mask;

    mask = pixmasks[x & 3];
    VGAMAPMASK(mask);

    dest = MK_FP(SCREENSEG, bufferofs + ylookup[y] + (x >> 2));

    while (height--)
    {
        *dest = color;
        dest += linewidth;
    }

    VGAMAPMASK(15);
#endif
}

/*
=================
=
= VL_Bar
=
=================
*/

void VL_Bar(int x, int y, int width, int height, int color)
{
    assert(false);
#if 0
    byte* dest;
    byte  leftmask, rightmask;
    int   midbytes, linedelta;

    leftmask  = leftmasks[x & 3];
    rightmask = rightmasks[(x + width - 1) & 3];
    midbytes  = ((x + width + 3) >> 2) - (x >> 2) - 2;
    linedelta = linewidth - (midbytes + 1);

    dest = MK_FP(SCREENSEG, bufferofs + ylookup[y] + (x >> 2));

    if (midbytes < 0)
    {
        // all in one byte
        VGAMAPMASK(leftmask & rightmask);
        while (height--)
        {
            *dest = color;
            dest += linewidth;
        }
        VGAMAPMASK(15);
        return;
    }

    while (height--)
    {
        VGAMAPMASK(leftmask);
        *dest++ = color;

        VGAMAPMASK(15);
        _fmemset(dest, color, midbytes);
        dest += midbytes;

        VGAMAPMASK(rightmask);
        *dest = color;

        dest += linedelta;
    }

    VGAMAPMASK(15);
#endif
}

/*
============================================================================

                            MEMORY OPS

============================================================================
*/

/*
=================
=
= VL_MemToLatch
=
=================
*/

void VL_MemToLatch(byte* source, int width, int height, unsigned dest)
{
    unsigned count;
    byte     plane, mask;

    count = ((width + 3) / 4) * height;
    mask  = 1;
    for (plane = 0; plane < 4; plane++)
    {
        assert(false);
#if 0
        VGAMAPMASK(mask);
        mask <<= 1;

        asm mov cx, count asm mov ax, SCREENSEG asm mov es, ax asm mov di, [dest] asm lds si, [source] asm rep movsb asm mov ax, ss asm mov ds, ax

#endif source += count;
    }
}

//===========================================================================

/*
=================
=
= VL_MemToScreen
=
= Draws a block of data to the screen.
=
=================
*/

void VL_MemToScreen(byte* source, int width, int height, int x, int y)
{
    assert(false);
#if 0
    byte *screen, *dest, mask;
    int   plane;

    width >>= 2;
    dest = MK_FP(SCREENSEG, bufferofs + ylookup[y] + (x >> 2));
    mask = 1 << (x & 3);

    for (plane = 0; plane < 4; plane++)
    {
        VGAMAPMASK(mask);
        mask <<= 1;
        if (mask == 16)
            mask = 1;

        screen = dest;
        for (y = 0; y < height; y++, screen += linewidth, source += width)
            _fmemcpy(screen, source, width);
    }
#endif
}

//==========================================================================

//------------------------------------------------------------------------
// VL_MaskMemToScreen()
//------------------------------------------------------------------------
void VL_MaskMemToScreen(byte* source, int width, int height, int x, int y, byte mask)
{
    assert(false);
#if 0
    byte *screen, *dest, bmask;
    int   plane, w, h, mod;

    width >>= 2;
    dest  = MK_FP(SCREENSEG, bufferofs + ylookup[y] + (x >> 2));
    bmask = 1 << (x & 3);
    mod   = linewidth - width;

    for (plane = 0; plane < 4; plane++)
    {
        VGAMAPMASK(bmask);

        screen = dest;
        h      = height;
        while (h--)
        {
            w = width;
            while (w--)
            {
                if (*source != mask)
                    *screen = *source;
                source++;
                screen++;
            }
            screen += mod;
        }

        bmask <<= 1;
        if (bmask == 16)
        {
            bmask = 1;
            dest++;
        }
    }
#endif
}

//------------------------------------------------------------------------
// VL_ScreenToMem()
//------------------------------------------------------------------------
void VL_ScreenToMem(byte* dest, int width, int height, int x, int y)
{
    assert(false);
#if 0
    byte *screen, *source, mask;
    int   plane;

    width >>= 2;
    source = MK_FP(SCREENSEG, bufferofs + ylookup[y] + (x >> 2));
    mask   = 0;

    for (plane = 0; plane < 4; plane++)
    {
        VGAREADMAP(mask);

        screen = source;
        for (y = 0; y < height; y++, screen += linewidth, dest += width)
            _fmemcpy(dest, screen, width);

        mask++;
        if (mask == 4)
        {
            mask = 0;
            source++;
        }
    }
#endif
}

//==========================================================================

/*
=================
=
= VL_LatchToScreen
=
=================
*/

void VL_LatchToScreen(unsigned source, int width, int height, int x, int y)
{
    assert(false);
#if 0
    VGAWRITEMODE(1);
    VGAMAPMASK(15);

    asm mov       di, [y] // dest = bufferofs+ylookup[y]+(x>>2)
        asm shl   di,
        1 asm mov di, [WORD PTR ylookup + di] asm add di, [bufferofs] asm mov ax, [x] asm shr ax, 2 asm add di, ax

        asm mov          si,
        [source] asm mov ax, [width] asm mov bx, [linewidth] asm sub bx, ax asm mov dx, [height] asm mov cx, SCREENSEG asm mov ds, cx asm mov es, cx

                                                                                                                                                      drawline : asm mov                  cx,
                                                                                                                                                                 ax asm rep movsb asm add di,
                                                                                                                                                                 bx asm dec dx asm jnz drawline

                                                                                                                                                                 asm mov    ax,
                                                                                                                                                                 ss asm mov ds,
                                                                                                                                                                 ax

                                                                                                                                                                     VGAWRITEMODE(0);
#endif
}

//===========================================================================

#if 0

/*
=================
=
= VL_ScreenToScreen
=
=================
*/

void VL_ScreenToScreen (unsigned source, unsigned dest,int width, int height)
{
	VGAWRITEMODE(1);
	VGAMAPMASK(15);

asm	mov	si,[source]
asm	mov	di,[dest]
asm	mov	ax,[width]
asm	mov	bx,[linewidth]
asm	sub	bx,ax
asm	mov	dx,[height]
asm	mov	cx,SCREENSEG
asm	mov	ds,cx
asm	mov	es,cx

drawline:
asm	mov	cx,ax
asm	rep movsb
asm	add	si,bx
asm	add	di,bx
asm	dec	dx
asm	jnz	drawline

asm	mov	ax,ss
asm	mov	ds,ax

	VGAWRITEMODE(0);
}

#endif

/*
=============================================================================

                        STRING OUTPUT ROUTINES

=============================================================================
*/

#if 0	
/*
===================
=
= VL_DrawTile8String
=
===================
*/

void VL_DrawTile8String (char *str, char  *tile8ptr, int printx, int printy)
{
	int		i;
	unsigned	 *dest, *screen, *src;

	dest = MK_FP(SCREENSEG,bufferofs+ylookup[printy]+(printx>>2));

	while (*str)
	{
		src = (unsigned  *)(tile8ptr + (*str<<6));
		// each character is 64 bytes

		VGAMAPMASK(1);
		screen = dest;
		for (i=0;i<8;i++,screen+=linewidth)
			*screen = *src++;
		VGAMAPMASK(2);
		screen = dest;
		for (i=0;i<8;i++,screen+=linewidth)
			*screen = *src++;
		VGAMAPMASK(4);
		screen = dest;
		for (i=0;i<8;i++,screen+=linewidth)
			*screen = *src++;
		VGAMAPMASK(8);
		screen = dest;
		for (i=0;i<8;i++,screen+=linewidth)
			*screen = *src++;

		str++;
		printx += 8;
		dest+=2;
	}
}
#endif

#if 0
/*
===================
=
= VL_DrawLatch8String
=
===================
*/

void VL_DrawLatch8String (char *str, unsigned tile8ptr, int printx, int printy)
{
	int		i;
	unsigned	src,dest;

	dest = bufferofs+ylookup[printy]+(printx>>2);

	VGAWRITEMODE(1);
	VGAMAPMASK(15);

	while (*str)
	{
		src = tile8ptr + (*str<<4);		// each character is 16 latch bytes

asm	mov	si,[src]
asm	mov	di,[dest]
asm	mov	dx,[linewidth]

asm	mov	ax,SCREENSEG
asm	mov	ds,ax

asm	lodsw
asm	mov	[di],ax
asm	add	di,dx
asm	lodsw
asm	mov	[di],ax
asm	add	di,dx
asm	lodsw
asm	mov	[di],ax
asm	add	di,dx
asm	lodsw
asm	mov	[di],ax
asm	add	di,dx
asm	lodsw
asm	mov	[di],ax
asm	add	di,dx
asm	lodsw
asm	mov	[di],ax
asm	add	di,dx
asm	lodsw
asm	mov	[di],ax
asm	add	di,dx
asm	lodsw
asm	mov	[di],ax
asm	add	di,dx

asm	mov	ax,ss
asm	mov	ds,ax

		str++;
		printx += 8;
		dest+=2;
	}

	VGAWRITEMODE(0);
}

#endif

#if 0

/*
===================
=
= VL_SizeTile8String
=
===================
*/

void VL_SizeTile8String (char *str, int *width, int *height)
{
	*height = 8;
	*width = 8*strlen(str);
}

#endif
