// ID_HEAD.H

#define WOLF
#define FREE_FUNCTIONS (true)

#define TEXTGR 0
#define CGAGR 1
#define EGAGR 2
#define VGAGR 3

#define GRMODE VGAGR

typedef unsigned char byte;
typedef unsigned int  word;
typedef unsigned long longword;
typedef byte*         Ptr;

/*
typedef struct
{
    int x, y;
} Point;
*/
/*
typedef struct
{
    Point ul, lr;
} Rect;
*/
void Quit(char* error, ...); // defined in user program
