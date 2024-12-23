#include "id_heads.hpp"
#include "jm_cio.hpp"
#include "jm_io.hpp"
// #include <MATH.H>
// #include <VALUES.H>
#include <cstdarg>

// #define DEMOS_EXTERN
// #define MYPROFILE
// #define TRACK_ENEMY_COUNT
#define OBJ_RESERV

#define __VERSION__ "V1.01"

#define GOLD_MORPH_LEVEL (19) // Level which Dr. GoldFire Morphs.

#define VERSION_TEXT_COLOR (0x82)
#define VERSION_TEXT_BKCOLOR (0x80)

#define NO_SHADING ((char)0x80)
#define LAMP_ON_SHADING ((char)-12)
#define EXPLOSION_SHADING ((char)-52)

#define PAGEFLIP

#define BETA_TEST (false)
#define LOOK_FOR_DEAD_GUYS (false)

#define BETA_CODE "NEWGAME"

#define MIN_MEM_NEEDED (231568l) // 560K
#define LIMITED_AMMO

#define SHADE_MAX 51
#define SHADE_DIV 6

#define BETA_MONTH 8
#define BETA_DAY 1
#define BETA_YEAR 1994

#define PLAYTEMP_FILE "PLAYTEMP"
#define OLD_PLAYTEMP_FILE "OLDPTEMP"

#define DISK_SPACE_NEEDED (1024l * 1024l)

#define BORDER_HI_COLOR 0x85
#define BORDER_MED_COLOR 0x82
#define BORDER_LO_COLOR 0x80
#define BORDER_TEXT_COLOR 0xaf

// FONT DEFINES
//
#define ID_FONT (STARTFONT)
#define BIG_FONT (STARTFONT + 1)
#define SIXPOINT_FONT (STARTFONT + 2)
#define TALL_FONT (STARTFONT + 3)
#define COAL_FONT (STARTFONT + 4)
#define RADAR_FONT (STARTFONT + 5)

// #define LOCKED_FLOORS

#define ID_CACHE_BRIEFS
#define ID_CACHE_HELP
#define ID_CACHE_LOSE
#define ID_CACHE_CREDITS

// #define DEBUG_STATICS
// #define DEBUG_ACTORS

//==========================================================================
//
//							GAME VERSION DEPENDANT DEFINATIONS
//
//==========================================================================

void jsprintf(char* msg, ...);

/*
=============================================================================

                            MACROS

=============================================================================
*/

#define BASE_GRAY 0x8
#define HIGH_GRAY 0xb
#define LOW_GRAY 0x5
#define BASE_GRN 0x76
#define HIGH_GRN 0x78
#define LOW_GRN 0x74

#define MAX_GUN_DELAY 12

#define MAX_RADAR_ENERGY 14400
#define RADAR_PAK_VALUE 60 * 15

#define MAXKEYS 1

#define COLORBORDER(color)    \
    asm {mov dx,STATUS_REGISTER_1;in al,dx;\
	mov dx,ATR_INDEX;mov al,ATR_OVERSCAN;out dx,al;mov al,color;out dx,al;\
	mov     al,32;out dx,al} \
    ;

#define MAPSPOT(x, y, plane) (*(mapsegs[plane] + farmapylookup[y] + x))

#define SIGN(x) ((x) > 0 ? 1 : -1)
#define ABS(x) ((int)(x) > 0 ? (x) : -(x))
#define LABS(x) ((long)(x) > 0 ? (x) : -(x))

// #define STATUSDRAWPIC(x, y, picnum)			JLatchDrawPic((x),(y+(200-STATUSLINES)),(picnum))

#define FMAPWIDTH ((fixed)mapwidth << TILESHIFT)
#define FMAPHEIGHT ((fixed)mapheight << TILESHIFT)
#define ObjVisable(from_obj, to_obj) PosVisable(from_obj->x, from_obj->y, to_obj->x, to_obj->y, from_obj->angle)

// SmartAnim macro
#define ANIM_INFO(o) ((ofs_anim_t*)&(o)->temp3)

#define DISPLAY_MSG_STD_TIME (5 * 60) // Tics display len
#define DISPLAY_TIMED_MSG(msg, pri, type) DisplayInfoMsg((msg), (pri), DISPLAY_MSG_STD_TIME, (type))
#define DISPLAY_MSG(msg, pri, type) DisplayInfoMsg((msg), (pri), 0, (type))

// SMART_ACTORS is the "case" used for certain switch statements.
//

#define SMART_ACTORS  \
    proguardobj:      \
    case rentacopobj: \
    case gen_scientistobj

#define SECURITY_LAMPS_ALERTED (madenoise)

// Barrier Code Stuff

#define MAX_BARRIER_SWITCHES 40 // max number level wall switches

#define SLIDE_TEMP(obj) ((unsigned)obj->hitpoints)

//
// M_BASE1 - represents 100 percent in 1st base
// M_BASE2 - represents 100 percent in 2nd base
// F_BASE2 - fractional portion of 2nd base
// SCALE - arbitrary scaling value (bigger number means more accurate)
//
// RETURNS: F_BASE1 - represents fractional portion of 1st base
//
// ex: RATIO(320,16,8,7)    returns  160
//
// Make sure values used won't overflow a WORD! In general, if largest number
// to be used (320 in ex: above) * (1<<SCALE) is greater than 65535, use
// LRATIO or a lower SCALE. Using a SCALE of 8 in the example above would
// overflow a WORD in some circumstances!
//
// LRATIO is to be used for larger SCALEs, thus, giving you massive accuracy!
//
#define RATIO(M_BASE1, M_BASE2, F_BASE2, SCALE) ((unsigned)(M_BASE1 * ((F_BASE2 << SCALE) / M_BASE2)) >> SCALE)
#define LRATIO(M_BASE1, M_BASE2, F_BASE2, SCALE) (((long)M_BASE1 * (((long)F_BASE2 << SCALE) / M_BASE2)) >> SCALE)

#define MAX_INF_AREA_MSGS 6
#define MAX_LOCATION_DESC_LEN 45

#define DOOR_RUBBLE_STATNUM 112 // Door Rubble sprite

#define SpawnExplosion(a, b) SpawnCusExplosion((a), (b), SPR_EXPLOSION_1, 4, 5, explosionobj)
#define SpawnFlash(a, b) SpawnCusExplosion((a), (b), SPR_EXPLOSION_1, 4, 5, deadobj)
#define InitSmartSpeedAnim(a, b, c, d, e, f, g) InitAnim((a), (b), (c), (d), (e), (f), (g), (g))

/*
=============================================================================

                         GLOBAL CONSTANTS

=============================================================================
*/

#define OV_ACTORS 0x0001
#define OV_SHOWALL 0x0002
#define OV_KEYS 0x0004
#define OV_PUSHWALLS 0x0008

#define TT_TRAVELED 0x01
#define TT_KEYS 0x02

// Max number of concession reply messages

// #define CON_HINTS															// Un/Comment to support concession hints

#define MAXACTORS 150     // max number of nazis, etc / map
#define MAXSTATS 400      // max number of lamps, bonus, etc
#define MAXDOORS 64       // max number of sliding doors
#define MAXCONCESSIONS 15 // max number of concession machines
#define MAXWALLTILES 64   // max number of wall tiles
#define MAXEAWALLS 12     // max electro-alien wall sockets

#define GS_NEEDCOORD 0
#define GS_FIRSTTIME 1
#define GS_COORDFOUND 2
#define GS_NO_MORE 3

#define GOLDIE_MAX_SPAWNS 10            // Max Number of spawn points for Goldstern
#define MIN_GOLDIE_FIRST_WAIT (5 * 60)  // Min wait time for FIRST goldstern (5 Secs)
#define MAX_GOLDIE_FIRST_WAIT (15 * 60) // Max wait time for FIRST goldstern (15 Secs)
#define MIN_GOLDIE_WAIT (30 * 60)       // Min wait time for next goldstern (30 Secs)
#define MAX_GOLDIE_WAIT (4 * 60 * 60)   // Max wait time for next goldstern (4 Mins)

//
// tile constants
//

#define ICONARROWS 90
#define PUSHABLETILE 98
#define EXITTILE 99        // at end of castle
#define AREATILE 108       // first of NUMAREAS floor tiles
#define HIDDENAREATILE 162 // first of hidden floor tiles
#define NUMAREAS 45
#define DOORTRIGGERTILE 158
#define SMART_OFF_TRIGGER 159
#define SMART_ON_TRIGGER 160
#define ELEVATORTILE2 27 // Space Will Switch
#define TRANSPORTERTILE 21
#define DIRECTTRANSPORTTILE 32 // Wall to use on direct level transporters
#define SODATILE 15
#define TERMINALTILE 17
#define FOODTILE 18
#define AMBUSHTILE 106
#define RKEY_TILE 72
#define YKEY_TILE 73
#define BKEY_TILE 74
#define BFG_TILE 75
#define ION_TILE 76
#define DETONATOR_TILE 77
#define CLOAK_TILE 78
#define LINC_TILE 79
#define CLOAK_AMBUSH_TILE 80
#define EATILE 24
#define ON_SWITCH 45      // ON Wall Switch -
#define OFF_SWITCH 57     // OFF Wall Switch -
#define WINTIGGERTILE 157 // Win Tile
#define NUMBERCHARS 9

#define START_TEXTURES 125 // Start of Textures - (Also LAST_WALL_TILE NUM)
#define NUM_TILES (PMSpriteStart)

//----------------

#define EXTRAPOINTS 400000

#define MAX_EXTRA_LIVES 4

#define PLAYERSPEED 3000
#define RUNSPEED 6000

#define SCREENSEG 0xa000

#define SCREENBWIDE 80

#define HEIGHTRATIO 0.41
#define TOP_STRIP_HEIGHT 16 // Pix height of top strip.

// #define BORDERCOLOR     116
#define FLASHCOLOR 5
#define FLASHTICS 4

#define PLAYERSIZE MINDIST    // player radius
#define MINACTORDIST 0x10000l // minimum dist from player center
                              // to any actor center

#define NUMLATCHPICS 100

#define PI 3.141592657

#define GLOBAL1 (1l << 16)
#define TILEGLOBAL GLOBAL1
#define PIXGLOBAL (GLOBAL1 / 64)
#define TILESHIFT 16l
#define UNSIGNEDSHIFT 8

#define ANGLES 360 // must be divisable by 4
#define ANGLEQUAD (ANGLES / 4)
#define FINEANGLES 3600
#define ANG90 (FINEANGLES / 4)
#define ANG180 (ANG90 * 2)
#define ANG270 (ANG90 * 3)
#define ANG360 (ANG90 * 4)
#define VANG90 (ANGLES / 4)
#define VANG180 (VANG90 * 2)
#define VANG270 (VANG90 * 3)
#define VANG360 (VANG90 * 4)

#define MINDIST (0x5800l)

#define MAX_WVIEW_DIST (44) // Max wrap_view dist in TILES

#define MAXSCALEHEIGHT 256 // largest scale on largest view
#define MAXVIEWWIDTH 320

#define MAPSIZE 64 // maps are 64*64 max
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define STATUSLINES 48

#define SCREENSIZE (SCREENBWIDE * 208)
#define PAGE1START 0
#define PAGE2START (SCREENSIZE)
#define PAGE3START (SCREENSIZE * 2u)
#define FREESTART (SCREENSIZE * 3u)

#define PIXRADIUS 512

#define STARTAMMO 8

// Token Definations

#define MAX_TOKENS 25

// Ammo/Weapon Definations

// #define NUM_AMMO_SEGS				42			// 42 Color segments (OR 21 2-Color segs)
#define MAX_AMMO 100                   // Max ammount of ammo for any weapon
#define AUTOCHARGE_WAIT 50             // Tics wait time for a full charge
#define MAX_PLASMA_DETONATORS 100      // Max number of Plasma Detonators
#define PLASMA_DETONATORS_DELAY 60 * 4 // Number of tics before plasma detonator explodes

// gamestate.flags flag values

#define GS_HEARTB_SOUND 0x0001

#ifdef CEILING_FLOOR_COLORS
#define GS_DRAW_CEILING 0x0002
#endif

#define GS_CLIP_WALLS 0x0004

#ifdef CEILING_FLOOR_COLORS
#define GS_DRAW_FLOOR 0x0008
#endif

#define GS_VIRGIN_LEVEL 0x0010
#define GS_CHECK_STATS_BONUS 0x0020
#define GS_ATTACK_INFOAREA 0x0040
#define GS_KILL_INF_WARN 0x0080
#define GS_SHOW_OVERHEAD 0x0100
#define GS_BAD_DIZ_FILE 0x0200
#define GS_MUSIC_TEST 0x0400
#define GS_LIGHTING 0x0800
#define GS_TICS_FOR_SCORE 0x1000
#define GS_NOWAIT 0x2000
#define GS_STARTLEVEL 0x4000
#define GS_QUICKRUN 0x8000

// object flag values - Oh Shit Longs!

#define FL_SHOOTABLE 0x00000001
#define FL_BONUS 0x00000002
#define FL_NEVERMARK 0x00000004
#define FL_VISABLE 0x00000008
#define FL_ATTACKMODE 0x00000010
#define FL_FIRSTATTACK 0x00000020
#define FL_AMBUSH 0x00000040
#define FL_NONMARK 0x00000080
#define FL_SOLID 0x00000100
#define FL_STATIONARY 0x00000200
#define FL_FRIENDLY 0x00000400
#define FL_DEADGUY 0x00000800
#define FL_RUNAWAY 0x00001000
#define FL_RUNTOSTATIC 0x00002000
#define FL_OFFSET_STATES 0x00004000
#define FL_INFORMANT 0x00008000
#define FL_INTERROGATED 0x00010000
#define FL_RANDOM_TURN 0x00020000
#define FL_NO_SLIDE 0x00040000
#define FL_MUST_ATTACK 0x00080000
#define FL_ALERTED 0x00100000
#define FL_FREEZE 0x00200000
#define FL_HAS_AMMO 0x00400000
#define FL_PROJ_TRANSPARENT 0x00800000
#define FL_PROJ_CHECK_TRANSPARENT 0x01000000
#define FL_HAS_TOKENS 0x02000000
#define FL_LOCKED_STATE 0x04000000
#define FL_BARRIER 0x08000000
#define FL_SHOOTMODE 0x10000000
#define FL_SLIDE_INIT 0x20000000
#define FL_STORED_OBJPTR 0x40000000
#define FL_FAKE_STATIC 0x80000000

#define FL_BARRIER_DAMAGE FL_HAS_TOKENS

// object flags2 values

#define FL2_BFGSHOT_SOLID 0x0001
#define FL2_BFG_SHOOTABLE 0x0002
#define FL2_NOTGUNSHOOTABLE 0x0004
#define FL2_SCARED 0x0008
#define FL2_DROP_RKEY 0x0010
#define FL2_DROP_YKEY 0x0020
#define FL2_DROP_BKEY 0x0040
#define FL2_DROP_BFG 0x0080
#define FL2_DROP_ION 0x0100
#define FL2_DROP_DETONATOR 0x0200
#define FL2_CLOAKED 0x0400
#define FL2_LINC 0x0800
#define FL2_DAMAGE_CLOAK 0x1000

// Run Reason Flags -- Why am I running..Duh..

#define RR_AMMO 0x0001
#define RR_HEALTH 0x0002
#define RR_INTERROGATED 0x0004
#define RR_CASUAL_PICKUP 0x0008
#define RR_SCARED 0x0010

// 0xFFxx Special Tile Flags (Flags in MASKED plane of Ted Maps)

#define TF_STARFIELD 0x01
#define TF_WRAP_VIEW 0x02

//
// Concession Machine Types

#define CT_HINT 0x0
#define CT_FOOD 0x1
#define CT_BEVS 0x2

//
// Radar switches for terminal Floor Cheat

#define RS_GOLDSTERN_TRACKER 0x0001
#define RS_PERSONNEL_TRACKER 0x0002
#define RS_SECURITY_STATUS 0x0004

//
// Door Flags
//
#define DR_BLASTABLE 0x01

//
// Smart Trigger Possiblities
//
#define ST_TURN_OFF 0x00
#define ST_TURN_ON 0x01
#define ST_TOGGLE 0x02

//
//
//

#define LT_GUN_DELAY gamestate.lastammo_leds
#define RT_GUN_DELAY gamestate.ammo_leds
#define GUN_TOGGLE gamestate.ammo

#define CANT_SAVE_GAME_TXT " Can't save this game! \n" \
                           "    Hard Disk FULL!"

typedef enum ss_type
{
    ss_normal,
    ss_quick,
    ss_justcalc,
} ss_type;

typedef enum cds_io_type
{
    cds_dos_print,
    cds_id_print,
    cds_menu_print,
} cds_io_type;

typedef enum sp_type
{
    sp_normal,
    sp_loading,
    sp_saving,
    sp_changeview,
    sp_teleporting,
} sp_type;

//
// sprite constants
//

typedef enum
{
    SPR_DEMO,

    //
    // static sprites
    //
    SPR_STAT_0, // SPR1V
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_1,
#endif
    SPR_STAT_2,
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_3,
    SPR_STAT_4,
    SPR_STAT_5,
    SPR_STAT_6,
    SPR_STAT_7,
#endif

    SPR_STAT_8,
    SPR_STAT_9,
    SPR_STAT_10,
    SPR_STAT_11, // SPR2V
    SPR_STAT_12,
    SPR_STAT_13,
    SPR_STAT_14,
    SPR_STAT_15,

#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_16,
    SPR_STAT_17, // SPR3V
#endif
    SPR_STAT_18,
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_19,
    SPR_STAT_20,
    SPR_STAT_21,
    SPR_STAT_22,
    SPR_STAT_23,
#endif

    SPR_STAT_24, // SPR4V
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_25,
#endif
    SPR_STAT_26,
    SPR_STAT_27,
    SPR_STAT_28,
    SPR_STAT_29,
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_30,
#endif
    SPR_STAT_31,

    SPR_STAT_32,
    SPR_STAT_33,
    SPR_STAT_34,
    SPR_STAT_35, // SPR5V
    SPR_STAT_36,
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_37,
#endif
    SPR_STAT_38,
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_39,
#endif

    SPR_STAT_40,
    SPR_STAT_41,
    SPR_STAT_42,
    SPR_STAT_43, // SPR7V
    SPR_STAT_44,
    SPR_STAT_45,
    SPR_STAT_46,
    SPR_STAT_47,

    SPR_STAT_48,
    SPR_STAT_49, // SPR8V
    SPR_STAT_50,
    SPR_STAT_51,

    SPR_STAT_52,
    SPR_STAT_53,
    SPR_STAT_54,
    SPR_STAT_55, // SPR9V
    SPR_STAT_56,
    SPR_CRATE_1,
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_CRATE_2,
    SPR_CRATE_3,
#endif

    SPR_STAT_57, // SPR10V
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_58,
    SPR_STAT_59,
#endif
    SPR_STAT_60,
    SPR_STAT_61,
    SPR_STAT_62,
    SPR_STAT_63,
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_64,
#endif

    SPR_STAT_65,
    SPR_STAT_66,
    SPR_STAT_67,
    SPR_STAT_68, // SPR11V
    SPR_STAT_69,
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_70,
    SPR_STAT_71,
    SPR_STAT_72,
#endif

#if GAME_VERSION != SHAREWARE_VERSION
    SPR_STAT_73,
    SPR_STAT_74,
    SPR_STAT_75,
    SPR_STAT_76, // SPR12V
#endif
    SPR_STAT_77,
    SPR_STAT_78,
    SPR_STAT_79,

    SPR_DOORBOMB,
    SPR_ALT_DOORBOMB,
    SPR_RUBBLE,
    SPR_BONZI_TREE, // OBJECTS1V
    SPR_AUTOMAPPER,
    SPR_POT_PLANT,
    SPR_TUBE_PLANT,
    SPR_HITECH_CHAIR,

    //
    // aesthetics
    //
    SPR_AIR_VENT,
    SPR_BLOOD_DRIP1,
    SPR_BLOOD_DRIP2,
    SPR_BLOOD_DRIP3,
    SPR_BLOOD_DRIP4,
    SPR_WATER_DRIP1,
    SPR_WATER_DRIP2,
    SPR_WATER_DRIP3,
    SPR_WATER_DRIP4,

    SPR_DECO_ARC_1,
    SPR_DECO_ARC_2,
    SPR_DECO_ARC_3,

    SPR_GRATE,
    SPR_STEAM_1,
    SPR_STEAM_2,
    SPR_STEAM_3,
    SPR_STEAM_4,

    SPR_STEAM_PIPE,
    SPR_PIPE_STEAM_1,
    SPR_PIPE_STEAM_2,
    SPR_PIPE_STEAM_3,
    SPR_PIPE_STEAM_4,

    //
    // Dead Actors (from Blake Stone: AOG)
    //
    SPR_DEAD_RENT,
    SPR_DEAD_PRO,
    SPR_DEAD_SWAT,

    //
    // Rent-A-Cop
    //
    SPR_RENT_S_1,
    SPR_RENT_S_2,
    SPR_RENT_S_3,
    SPR_RENT_S_4,
    SPR_RENT_S_5,
    SPR_RENT_S_6,
    SPR_RENT_S_7,
    SPR_RENT_S_8,

    SPR_RENT_W1_1,
    SPR_RENT_W1_2,
    SPR_RENT_W1_3,
    SPR_RENT_W1_4,
    SPR_RENT_W1_5,
    SPR_RENT_W1_6,
    SPR_RENT_W1_7,
    SPR_RENT_W1_8,

    SPR_RENT_W2_1,
    SPR_RENT_W2_2,
    SPR_RENT_W2_3,
    SPR_RENT_W2_4,
    SPR_RENT_W2_5,
    SPR_RENT_W2_6,
    SPR_RENT_W2_7,
    SPR_RENT_W2_8,

    SPR_RENT_W3_1,
    SPR_RENT_W3_2,
    SPR_RENT_W3_3,
    SPR_RENT_W3_4,
    SPR_RENT_W3_5,
    SPR_RENT_W3_6,
    SPR_RENT_W3_7,
    SPR_RENT_W3_8,

    SPR_RENT_W4_1,
    SPR_RENT_W4_2,
    SPR_RENT_W4_3,
    SPR_RENT_W4_4,
    SPR_RENT_W4_5,
    SPR_RENT_W4_6,
    SPR_RENT_W4_7,
    SPR_RENT_W4_8,

    SPR_RENT_DIE_1,
    SPR_RENT_DIE_2,
    SPR_RENT_DIE_3,
    SPR_RENT_DIE_4,
    SPR_RENT_PAIN_1,
    SPR_RENT_DEAD,

    SPR_RENT_SHOOT1,
    SPR_RENT_SHOOT2,
    SPR_RENT_SHOOT3,

    //
    // PRO-GUARD
    //
    SPR_PRO_S_1,
    SPR_PRO_S_2,
    SPR_PRO_S_3,
    SPR_PRO_S_4,
    SPR_PRO_S_5,
    SPR_PRO_S_6,
    SPR_PRO_S_7,
    SPR_PRO_S_8,

    SPR_PRO_W1_1,
    SPR_PRO_W1_2,
    SPR_PRO_W1_3,
    SPR_PRO_W1_4,
    SPR_PRO_W1_5,
    SPR_PRO_W1_6,
    SPR_PRO_W1_7,
    SPR_PRO_W1_8,

    SPR_PRO_W2_1,
    SPR_PRO_W2_2,
    SPR_PRO_W2_3,
    SPR_PRO_W2_4,
    SPR_PRO_W2_5,
    SPR_PRO_W2_6,
    SPR_PRO_W2_7,
    SPR_PRO_W2_8,

    SPR_PRO_W3_1,
    SPR_PRO_W3_2,
    SPR_PRO_W3_3,
    SPR_PRO_W3_4,
    SPR_PRO_W3_5,
    SPR_PRO_W3_6,
    SPR_PRO_W3_7,
    SPR_PRO_W3_8,

    SPR_PRO_W4_1,
    SPR_PRO_W4_2,
    SPR_PRO_W4_3,
    SPR_PRO_W4_4,
    SPR_PRO_W4_5,
    SPR_PRO_W4_6,
    SPR_PRO_W4_7,
    SPR_PRO_W4_8,

    SPR_PRO_PAIN_1,
    SPR_PRO_DIE_1,
    SPR_PRO_DIE_2,
    SPR_PRO_DIE_3,
    SPR_PRO_PAIN_2,
    SPR_PRO_DIE_4,
    SPR_PRO_DEAD,

    SPR_PRO_SHOOT1,
    SPR_PRO_SHOOT2,
    SPR_PRO_SHOOT3,

    //
    // swat
    //
    SPR_SWAT_S_1,
    SPR_SWAT_S_2,
    SPR_SWAT_S_3,
    SPR_SWAT_S_4,
    SPR_SWAT_S_5,
    SPR_SWAT_S_6,
    SPR_SWAT_S_7,
    SPR_SWAT_S_8,

    SPR_SWAT_W1_1,
    SPR_SWAT_W1_2,
    SPR_SWAT_W1_3,
    SPR_SWAT_W1_4,
    SPR_SWAT_W1_5,
    SPR_SWAT_W1_6,
    SPR_SWAT_W1_7,
    SPR_SWAT_W1_8,

    SPR_SWAT_W2_1,
    SPR_SWAT_W2_2,
    SPR_SWAT_W2_3,
    SPR_SWAT_W2_4,
    SPR_SWAT_W2_5,
    SPR_SWAT_W2_6,
    SPR_SWAT_W2_7,
    SPR_SWAT_W2_8,

    SPR_SWAT_W3_1,
    SPR_SWAT_W3_2,
    SPR_SWAT_W3_3,
    SPR_SWAT_W3_4,
    SPR_SWAT_W3_5,
    SPR_SWAT_W3_6,
    SPR_SWAT_W3_7,
    SPR_SWAT_W3_8,

    SPR_SWAT_W4_1,
    SPR_SWAT_W4_2,
    SPR_SWAT_W4_3,
    SPR_SWAT_W4_4,
    SPR_SWAT_W4_5,
    SPR_SWAT_W4_6,
    SPR_SWAT_W4_7,
    SPR_SWAT_W4_8,

    SPR_SWAT_PAIN_1,
    SPR_SWAT_DIE_1,
    SPR_SWAT_DIE_2,
    SPR_SWAT_DIE_3,
    SPR_SWAT_PAIN_2,
    SPR_SWAT_DIE_4,
    SPR_SWAT_DEAD,

    SPR_SWAT_SHOOT1,
    SPR_SWAT_SHOOT2,
    SPR_SWAT_SHOOT3,

    SPR_SWAT_WOUNDED1,
    SPR_SWAT_WOUNDED2,
    SPR_SWAT_WOUNDED3,
    SPR_SWAT_WOUNDED4,

    //
    // GENERAL SCIENTIST
    //
    SPR_OFC_S_1,
    SPR_OFC_S_2,
    SPR_OFC_S_3,
    SPR_OFC_S_4,
    SPR_OFC_S_5,
    SPR_OFC_S_6,
    SPR_OFC_S_7,
    SPR_OFC_S_8,

    SPR_OFC_W1_1,
    SPR_OFC_W1_2,
    SPR_OFC_W1_3,
    SPR_OFC_W1_4,
    SPR_OFC_W1_5,
    SPR_OFC_W1_6,
    SPR_OFC_W1_7,
    SPR_OFC_W1_8,

    SPR_OFC_W2_1,
    SPR_OFC_W2_2,
    SPR_OFC_W2_3,
    SPR_OFC_W2_4,
    SPR_OFC_W2_5,
    SPR_OFC_W2_6,
    SPR_OFC_W2_7,
    SPR_OFC_W2_8,

    SPR_OFC_W3_1,
    SPR_OFC_W3_2,
    SPR_OFC_W3_3,
    SPR_OFC_W3_4,
    SPR_OFC_W3_5,
    SPR_OFC_W3_6,
    SPR_OFC_W3_7,
    SPR_OFC_W3_8,

    SPR_OFC_W4_1,
    SPR_OFC_W4_2,
    SPR_OFC_W4_3,
    SPR_OFC_W4_4,
    SPR_OFC_W4_5,
    SPR_OFC_W4_6,
    SPR_OFC_W4_7,
    SPR_OFC_W4_8,

    SPR_OFC_PAIN_1,
    SPR_OFC_DIE_1,
    SPR_OFC_DIE_2,
    SPR_OFC_DIE_3,
    SPR_OFC_PAIN_2,
    SPR_OFC_DIE_4,
    SPR_OFC_DEAD,

    SPR_OFC_SHOOT1,
    SPR_OFC_SHOOT2,
    SPR_OFC_SHOOT3,

    //
    // Bad Boy Dr. Goldstern
    //
    SPR_GOLD_S_1,
    SPR_GOLD_S_2,
    SPR_GOLD_S_3,
    SPR_GOLD_S_4,
    SPR_GOLD_S_5,
    SPR_GOLD_S_6,
    SPR_GOLD_S_7,
    SPR_GOLD_S_8,

    SPR_GOLD_W1_1,
    SPR_GOLD_W1_2,
    SPR_GOLD_W1_3,
    SPR_GOLD_W1_4,
    SPR_GOLD_W1_5,
    SPR_GOLD_W1_6,
    SPR_GOLD_W1_7,
    SPR_GOLD_W1_8,

    SPR_GOLD_W2_1,
    SPR_GOLD_W2_2,
    SPR_GOLD_W2_3,
    SPR_GOLD_W2_4,
    SPR_GOLD_W2_5,
    SPR_GOLD_W2_6,
    SPR_GOLD_W2_7,
    SPR_GOLD_W2_8,

    SPR_GOLD_W3_1,
    SPR_GOLD_W3_2,
    SPR_GOLD_W3_3,
    SPR_GOLD_W3_4,
    SPR_GOLD_W3_5,
    SPR_GOLD_W3_6,
    SPR_GOLD_W3_7,
    SPR_GOLD_W3_8,

    SPR_GOLD_W4_1,
    SPR_GOLD_W4_2,
    SPR_GOLD_W4_3,
    SPR_GOLD_W4_4,
    SPR_GOLD_W4_5,
    SPR_GOLD_W4_6,
    SPR_GOLD_W4_7,
    SPR_GOLD_W4_8,

    SPR_GOLD_PAIN_1,

    SPR_GOLD_WRIST_1,
    SPR_GOLD_WRIST_2,

    SPR_GOLD_SHOOT1,
    SPR_GOLD_SHOOT2,
    SPR_GOLD_SHOOT3,

    SPR_GOLD_WARP1,
    SPR_GOLD_WARP2,
    SPR_GOLD_WARP3,
    SPR_GOLD_WARP4,
    SPR_GOLD_WARP5,

    SPR_GOLD_DEATH1,
    SPR_GOLD_DEATH2,
    SPR_GOLD_DEATH3,
    SPR_GOLD_DEATH4,
    SPR_GOLD_DEATH5,
    SPR_MGOLD_OUCH,

    SPR_GOLD_MORPH1,
    SPR_GOLD_MORPH2,
    SPR_GOLD_MORPH3,
    SPR_GOLD_MORPH4,
    SPR_GOLD_MORPH5,
    SPR_GOLD_MORPH6,
    SPR_GOLD_MORPH7,
    SPR_GOLD_MORPH8,

    SPR_MGOLD_WALK1,
    SPR_MGOLD_WALK2,
    SPR_MGOLD_WALK3,
    SPR_MGOLD_WALK4,
    SPR_MGOLD_ATTACK1,
    SPR_MGOLD_ATTACK2,
    SPR_MGOLD_ATTACK3,
    SPR_MGOLD_ATTACK4,

    SPR_MGOLD_SHOT1,
    SPR_MGOLD_SHOT2,
    SPR_MGOLD_SHOT3,
    SPR_MGOLD_SHOT_EXP1,
    SPR_MGOLD_SHOT_EXP2,
    SPR_MGOLD_SHOT_EXP3,

//
//		GROUND SCOUT
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_GSCOUT_W1_1,
    SPR_GSCOUT_W1_2,
    SPR_GSCOUT_W1_3,
    SPR_GSCOUT_W1_4,
    SPR_GSCOUT_W1_5,
    SPR_GSCOUT_W1_6,
    SPR_GSCOUT_W1_7,
    SPR_GSCOUT_W1_8,

    SPR_GSCOUT_W2_1,
    SPR_GSCOUT_W2_2,
    SPR_GSCOUT_W2_3,
    SPR_GSCOUT_W2_4,
    SPR_GSCOUT_W2_5,
    SPR_GSCOUT_W2_6,
    SPR_GSCOUT_W2_7,
    SPR_GSCOUT_W2_8,

    SPR_GSCOUT_W3_1,
    SPR_GSCOUT_W3_2,
    SPR_GSCOUT_W3_3,
    SPR_GSCOUT_W3_4,
    SPR_GSCOUT_W3_5,
    SPR_GSCOUT_W3_6,
    SPR_GSCOUT_W3_7,
    SPR_GSCOUT_W3_8,

    SPR_GSCOUT_W4_1,
    SPR_GSCOUT_W4_2,
    SPR_GSCOUT_W4_3,
    SPR_GSCOUT_W4_4,
    SPR_GSCOUT_W4_5,
    SPR_GSCOUT_W4_6,
    SPR_GSCOUT_W4_7,
    SPR_GSCOUT_W4_8,

    SPR_GSCOUT_DIE1,
    SPR_GSCOUT_DIE2,
    SPR_GSCOUT_DIE3,
    SPR_GSCOUT_DIE4,
    SPR_GSCOUT_DIE5,
    SPR_GSCOUT_DIE6,
    SPR_GSCOUT_DIE7,
    SPR_GSCOUT_DIE8,

    SPR_GSCOUT_DEAD,
#endif

    //
    //		FLOATING SCOUT
    //
    SPR_FSCOUT_W1_1,
    SPR_FSCOUT_W1_2,
    SPR_FSCOUT_W1_3,
    SPR_FSCOUT_W1_4,
    SPR_FSCOUT_W1_5,
    SPR_FSCOUT_W1_6,
    SPR_FSCOUT_W1_7,
    SPR_FSCOUT_W1_8,

#if GAME_VERSION != SHAREWARE_VERSION
    SPR_FSCOUT_W2_1,
    SPR_FSCOUT_W2_2,
    SPR_FSCOUT_W2_3,
    SPR_FSCOUT_W2_4,
    SPR_FSCOUT_W2_5,
    SPR_FSCOUT_W2_6,
    SPR_FSCOUT_W2_7,
    SPR_FSCOUT_W2_8,

    SPR_FSCOUT_W3_1,
    SPR_FSCOUT_W3_2,
    SPR_FSCOUT_W3_3,
    SPR_FSCOUT_W3_4,
    SPR_FSCOUT_W3_5,
    SPR_FSCOUT_W3_6,
    SPR_FSCOUT_W3_7,
    SPR_FSCOUT_W3_8,

    SPR_FSCOUT_W4_1,
    SPR_FSCOUT_W4_2,
    SPR_FSCOUT_W4_3,
    SPR_FSCOUT_W4_4,
    SPR_FSCOUT_W4_5,
    SPR_FSCOUT_W4_6,
    SPR_FSCOUT_W4_7,
    SPR_FSCOUT_W4_8,
#endif

    SPR_FSCOUT_DIE1,
    SPR_FSCOUT_DIE2,
    SPR_FSCOUT_DIE3,
    SPR_FSCOUT_DIE4,
    SPR_FSCOUT_DIE5,
    SPR_FSCOUT_DIE6,
    SPR_FSCOUT_DIE7,
    SPR_FSCOUT_DEAD,

    //
    // 	GENERAL EXPLOSION ANIM
    //
    SPR_EXPLOSION_1,
    SPR_EXPLOSION_2,
    SPR_EXPLOSION_3,
    SPR_EXPLOSION_4,
    SPR_EXPLOSION_5,

    //
    //		VITAL DEFENCE OBJECT
    //
    // #if GAME_VERSION != SHAREWARE_VERSION
    //		SPR_VITAL_STAND,
    //		SPR_VITAL_DIE_1, SPR_VITAL_DIE_2, SPR_VITAL_DIE_3,SPR_VITAL_DIE_4,
    //		SPR_VITAL_DIE_5, SPR_VITAL_DIE_6, SPR_VITAL_DIE_7, SPR_VITAL_DIE_8,
    //		SPR_VITAL_DEAD_1,SPR_VITAL_DEAD_2,SPR_VITAL_DEAD_3,
    //		SPR_VITAL_OUCH,
    // #endif

    //
    //		ROTATING CUBE
    //
    SPR_CUBE1,
    SPR_CUBE2,
    SPR_CUBE3,
    SPR_CUBE4,
    SPR_CUBE5,
    SPR_CUBE6,
    SPR_CUBE7,
    SPR_CUBE8,
    SPR_CUBE9,
    SPR_CUBE10,
    SPR_CUBE_EXP1,
    SPR_CUBE_EXP2,
    SPR_CUBE_EXP3,
    SPR_CUBE_EXP4,
    SPR_CUBE_EXP5,
    SPR_CUBE_EXP6,
    SPR_CUBE_EXP7,
    SPR_CUBE_EXP8,
    SPR_DEAD_CUBE,

    //
    //    RED SECURITY LIGHTS
    //
    SPR_SECURITY_NORMAL,
    SPR_SECURITY_ALERT,

    //
    // P.O.D. Alien
    //
    SPR_POD_EGG,
    SPR_POD_HATCH1,
    SPR_POD_HATCH2,
    SPR_POD_HATCH3,
    SPR_POD_WALK1,
    SPR_POD_WALK2,
    SPR_POD_WALK3,
    SPR_POD_WALK4,
    SPR_POD_ATTACK1,
    SPR_POD_ATTACK2,
    SPR_POD_ATTACK3,
    SPR_POD_OUCH,
    SPR_POD_DIE1,
    SPR_POD_DIE2,
    SPR_POD_DIE3,
    SPR_POD_SPIT1,
    SPR_POD_SPIT2,
    SPR_POD_SPIT3,

    //
    // Electro-Alien
    //
    SPR_ELEC_APPEAR1,
    SPR_ELEC_APPEAR2,
    SPR_ELEC_APPEAR3,
    SPR_ELEC_WALK1,
    SPR_ELEC_WALK2,
    SPR_ELEC_WALK3,
    SPR_ELEC_WALK4,
    SPR_ELEC_OUCH,
    SPR_ELEC_SHOOT1,
    SPR_ELEC_SHOOT2,
    SPR_ELEC_SHOOT3,
    SPR_ELEC_DIE1,
    SPR_ELEC_DIE2,
    SPR_ELEC_DIE3,
    SPR_ELEC_SHOT1,
    SPR_ELEC_SHOT2,
    SPR_ELEC_SHOT_EXP1,
    SPR_ELEC_SHOT_EXP2,

    //
    // ElectroSphere
    //
    SPR_ELECTRO_SPHERE_ROAM1,
    SPR_ELECTRO_SPHERE_ROAM2,
    SPR_ELECTRO_SPHERE_ROAM3,
    SPR_ELECTRO_SPHERE_OUCH,
    SPR_ELECTRO_SPHERE_DIE1,
    SPR_ELECTRO_SPHERE_DIE2,
    SPR_ELECTRO_SPHERE_DIE3,
    SPR_ELECTRO_SPHERE_DIE4,

    //
    // Genetic Guard
    //
    SPR_GENETIC_W1,
    SPR_GENETIC_W2,
    SPR_GENETIC_W3,
    SPR_GENETIC_W4,
    SPR_GENETIC_SWING1,
    SPR_GENETIC_SWING2,
    SPR_GENETIC_SWING3,
    SPR_GENETIC_DEAD,
    SPR_GENETIC_DIE1,
    SPR_GENETIC_DIE2,
    SPR_GENETIC_DIE3,
    SPR_GENETIC_DIE4,
    SPR_GENETIC_OUCH,
    SPR_GENETIC_SHOOT1,
    SPR_GENETIC_SHOOT2,
    SPR_GENETIC_SHOOT3,

    //
    // Muntant human type 1
    //
    SPR_MUTHUM1_W1,
    SPR_MUTHUM1_W2,
    SPR_MUTHUM1_W3,
    SPR_MUTHUM1_W4,
    SPR_MUTHUM1_SWING1,
    SPR_MUTHUM1_SWING2,
    SPR_MUTHUM1_SWING3,
    SPR_MUTHUM1_DEAD,
    SPR_MUTHUM1_DIE1,
    SPR_MUTHUM1_DIE2,
    SPR_MUTHUM1_DIE3,
    SPR_MUTHUM1_DIE4,
    SPR_MUTHUM1_OUCH,
    SPR_MUTHUM1_SPIT1,
    SPR_MUTHUM1_SPIT2,
    SPR_MUTHUM1_SPIT3,

//
// Muntant human type 2
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_MUTHUM2_W1,
    SPR_MUTHUM2_W2,
    SPR_MUTHUM2_W3,
    SPR_MUTHUM2_W4,
    SPR_MUTHUM2_SWING1,
    SPR_MUTHUM2_SWING2,
    SPR_MUTHUM2_SWING3,
    SPR_MUTHUM2_DEAD,
    SPR_MUTHUM2_DIE1,
    SPR_MUTHUM2_DIE2,
    SPR_MUTHUM2_DIE3,
    SPR_MUTHUM2_DIE4,
    SPR_MUTHUM2_OUCH,
    SPR_MUTHUM2_SPIT1,
    SPR_MUTHUM2_SPIT2,
    SPR_MUTHUM2_SPIT3,

    SPR_MUTHUM2_MORPH1,
    SPR_MUTHUM2_MORPH2,
    SPR_MUTHUM2_MORPH3,
    SPR_MUTHUM2_MORPH4,
    SPR_MUTHUM2_MORPH5,
    SPR_MUTHUM2_MORPH6,
    SPR_MUTHUM2_MORPH7,
    SPR_MUTHUM2_MORPH8,
    SPR_MUTHUM2_MORPH9,
#endif

//
// Large Cantained Alien
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_LCAN_ALIEN_READY,
    SPR_LCAN_ALIEN_B1,
    SPR_LCAN_ALIEN_B2,
    SPR_LCAN_ALIEN_B3,
    SPR_LCAN_ALIEN_EMPTY,

    SPR_LCAN_ALIEN_W1,
    SPR_LCAN_ALIEN_W2,
    SPR_LCAN_ALIEN_W3,
    SPR_LCAN_ALIEN_W4,
    SPR_LCAN_ALIEN_SWING1,
    SPR_LCAN_ALIEN_SWING2,
    SPR_LCAN_ALIEN_SWING3,
    SPR_LCAN_ALIEN_DEAD,
    SPR_LCAN_ALIEN_DIE1,
    SPR_LCAN_ALIEN_DIE2,
    SPR_LCAN_ALIEN_DIE3,
    SPR_LCAN_ALIEN_DIE4,
    SPR_LCAN_ALIEN_OUCH,
    SPR_LCAN_ALIEN_SPIT1,
    SPR_LCAN_ALIEN_SPIT2,
    SPR_LCAN_ALIEN_SPIT3,
#endif

    //
    // Small Canister Alien
    //
    SPR_SCAN_ALIEN_READY,
    SPR_SCAN_ALIEN_B1,
    SPR_SCAN_ALIEN_B2,
    SPR_SCAN_ALIEN_B3,
    SPR_SCAN_ALIEN_EMPTY,

    SPR_SCAN_ALIEN_W1,
    SPR_SCAN_ALIEN_W2,
    SPR_SCAN_ALIEN_W3,
    SPR_SCAN_ALIEN_W4,
    SPR_SCAN_ALIEN_SWING1,
    SPR_SCAN_ALIEN_SWING2,
    SPR_SCAN_ALIEN_SWING3,
    SPR_SCAN_ALIEN_DEAD,
    SPR_SCAN_ALIEN_DIE1,
    SPR_SCAN_ALIEN_DIE2,
    SPR_SCAN_ALIEN_DIE3,
    SPR_SCAN_ALIEN_DIE4,
    SPR_SCAN_ALIEN_OUCH,

    SPR_SCAN_ALIEN_SPIT1,
    SPR_SCAN_ALIEN_SPIT2,
    SPR_SCAN_ALIEN_SPIT3,

//
// Gurney Mutant
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_GURNEY_MUT_READY,
    SPR_GURNEY_MUT_B1,
    SPR_GURNEY_MUT_B2,
    SPR_GURNEY_MUT_B3,
    SPR_GURNEY_MUT_EMPTY,

    SPR_GURNEY_MUT_W1,
    SPR_GURNEY_MUT_W2,
    SPR_GURNEY_MUT_W3,
    SPR_GURNEY_MUT_W4,
    SPR_GURNEY_MUT_SWING1,
    SPR_GURNEY_MUT_SWING2,
    SPR_GURNEY_MUT_SWING3,
    SPR_GURNEY_MUT_DEAD,
    SPR_GURNEY_MUT_DIE1,
    SPR_GURNEY_MUT_DIE2,
    SPR_GURNEY_MUT_DIE3,
    SPR_GURNEY_MUT_DIE4,
    SPR_GURNEY_MUT_OUCH,
#endif

//
// Liquid Alien
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_LIQUID_M1,
    SPR_LIQUID_M2,
    SPR_LIQUID_M3,
    SPR_LIQUID_R1,
    SPR_LIQUID_R2,
    SPR_LIQUID_R3,
    SPR_LIQUID_R4,
    SPR_LIQUID_S1,
    SPR_LIQUID_S2,
    SPR_LIQUID_S3,
    SPR_LIQUID_OUCH,
    SPR_LIQUID_DIE_1,
    SPR_LIQUID_DIE_2,
    SPR_LIQUID_DIE_3,
    SPR_LIQUID_DIE_4,
    SPR_LIQUID_DEAD,
    SPR_LIQUID_SHOT_FLY_1,
    SPR_LIQUID_SHOT_FLY_2,
    SPR_LIQUID_SHOT_FLY_3,
    SPR_LIQUID_SHOT_BURST_1,
    SPR_LIQUID_SHOT_BURST_2,
    SPR_LIQUID_SHOT_BURST_3,
#endif

    //
    //  SPIT SHOTS
    //
    SPR_SPIT1_1,
    SPR_SPIT1_2,
    SPR_SPIT1_3,
    SPR_SPIT_EXP1_1,
    SPR_SPIT_EXP1_2,
    SPR_SPIT_EXP1_3,

    SPR_SPIT2_1,
    SPR_SPIT2_2,
    SPR_SPIT2_3,
    SPR_SPIT_EXP2_1,
    SPR_SPIT_EXP2_2,
    SPR_SPIT_EXP2_3,

    SPR_SPIT3_1,
    SPR_SPIT3_2,
    SPR_SPIT3_3,
    SPR_SPIT_EXP3_1,
    SPR_SPIT_EXP3_2,
    SPR_SPIT_EXP3_3,

    //
    // Hanging Terrot
    //

    SPR_TERROT_1,
    SPR_TERROT_2,
    SPR_TERROT_3,
    SPR_TERROT_4,
    SPR_TERROT_5,
    SPR_TERROT_6,
    SPR_TERROT_7,
    SPR_TERROT_8,

    SPR_TERROT_FIRE_1,
    SPR_TERROT_FIRE_2,
    SPR_TERROT_DIE_1,
    SPR_TERROT_DIE_2,
    SPR_TERROT_DIE_3,
    SPR_TERROT_DIE_4,
    SPR_TERROT_DEAD,

    //
    // player attack frames
    //
    SPR_KNIFEREADY,
    SPR_KNIFEATK1,
    SPR_KNIFEATK2,
    SPR_KNIFEATK3,
    SPR_KNIFEATK4,

    SPR_PISTOLREADY,
    SPR_PISTOLATK1,
    SPR_PISTOLATK2,
    SPR_PISTOLATK3,
    SPR_PISTOLATK4,

    SPR_MACHINEGUNREADY,
    SPR_MACHINEGUNATK1,
    SPR_MACHINEGUNATK2,
    MACHINEGUNATK3,
    SPR_MACHINEGUNATK4,

    SPR_CHAINREADY,
    SPR_CHAINATK1,
    SPR_CHAINATK2,
    SPR_CHAINATK3,
    SPR_CHAINATK4,

    SPR_GRENADEREADY,
    SPR_GRENADEATK1,
    SPR_GRENADEATK2,
    SPR_GRENADEATK3,
    SPR_GRENADEATK4,

    SPR_GRENADE_FLY1,
    SPR_GRENADE_FLY2,
    SPR_GRENADE_FLY3,
    SPR_GRENADE_FLY4,
    SPR_GRENADE_EXPLODE1,
    SPR_GRENADE_EXPLODE2,
    SPR_GRENADE_EXPLODE3,
    SPR_GRENADE_EXPLODE4,
    SPR_GRENADE_EXPLODE5,

    SPR_ELEC_ARC1,
    SPR_ELEC_ARC2,
    SPR_ELEC_ARC3,
    SPR_ELEC_ARC4,
    SPR_ELEC_POST1,
    SPR_ELEC_POST2,
    SPR_ELEC_POST3,
    SPR_ELEC_POST4,

    SPR_VPOST1,
    SPR_VPOST2,
    SPR_VPOST3,
    SPR_VPOST4,
    SPR_VPOST5,
    SPR_VPOST6,
    SPR_VPOST7,
    SPR_VPOST8,

    SPR_VSPIKE1,
    SPR_VSPIKE2,
    SPR_VSPIKE3,
    SPR_VSPIKE4,
    SPR_VSPIKE5,
    SPR_VSPIKE6,
    SPR_VSPIKE7,
    SPR_VSPIKE8,

    SPR_GREEN_OOZE1,
    SPR_GREEN_OOZE2,
    SPR_GREEN_OOZE3,
    SPR_BLACK_OOZE1,
    SPR_BLACK_OOZE2,
    SPR_BLACK_OOZE3,

    SPR_GREEN2_OOZE1,
    SPR_GREEN2_OOZE2,
    SPR_GREEN2_OOZE3,
    SPR_BLACK2_OOZE1,
    SPR_BLACK2_OOZE2,
    SPR_BLACK2_OOZE3,

    SPR_CANDY_BAR,
    SPR_CANDY_WRAPER,
    SPR_SANDWICH,
    SPR_SANDWICH_WRAPER,

    SPR_BLAKE_W1,
    SPR_BLAKE_W2,
    SPR_BLAKE_W3,
    SPR_BLAKE_W4,

    //
    // BOSS 1
    //
    SPR_BOSS1_W1,
    SPR_BOSS1_W2,
    SPR_BOSS1_W3,
    SPR_BOSS1_W4,
    SPR_BOSS1_SWING1,
    SPR_BOSS1_SWING2,
    SPR_BOSS1_SWING3,
    SPR_BOSS1_DEAD,
    SPR_BOSS1_DIE1,
    SPR_BOSS1_DIE2,
    SPR_BOSS1_DIE3,
    SPR_BOSS1_DIE4,
    SPR_BOSS1_OUCH,
    SPR_BOSS1_PROJ1,
    SPR_BOSS1_PROJ2,
    SPR_BOSS1_PROJ3,
    SPR_BOSS1_EXP1,
    SPR_BOSS1_EXP2,
    SPR_BOSS1_EXP3,
    SPR_BOSS1_MORPH1,
    SPR_BOSS1_MORPH2,
    SPR_BOSS1_MORPH3,
    SPR_BOSS1_MORPH4,
    SPR_BOSS1_MORPH5,
    SPR_BOSS1_MORPH6,
    SPR_BOSS1_MORPH7,
    SPR_BOSS1_MORPH8,
    SPR_BOSS1_MORPH9,

//
// BOSS 2
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS2_W1,
    SPR_BOSS2_W2,
    SPR_BOSS2_W3,
    SPR_BOSS2_W4,
    SPR_BOSS2_SWING1,
    SPR_BOSS2_SWING2,
    SPR_BOSS2_SWING3,
    SPR_BOSS2_DEAD,
    SPR_BOSS2_DIE1,
    SPR_BOSS2_DIE2,
    SPR_BOSS2_DIE3,
    SPR_BOSS2_DIE4,
    SPR_BOSS2_OUCH,
#endif

//
// BOSS 3
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS3_W1,
    SPR_BOSS3_W2,
    SPR_BOSS3_W3,
    SPR_BOSS3_W4,
    SPR_BOSS3_SWING1,
    SPR_BOSS3_SWING2,
    SPR_BOSS3_SWING3,
    SPR_BOSS3_DEAD,
    SPR_BOSS3_DIE1,
    SPR_BOSS3_DIE2,
    SPR_BOSS3_DIE3,
    SPR_BOSS3_DIE4,
    SPR_BOSS3_OUCH,
#endif

//
// BOSS 4
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS4_W1,
    SPR_BOSS4_W2,
    SPR_BOSS4_W3,
    SPR_BOSS4_W4,
    SPR_BOSS4_SWING1,
    SPR_BOSS4_SWING2,
    SPR_BOSS4_SWING3,
    SPR_BOSS4_DEAD,
    SPR_BOSS4_DIE1,
    SPR_BOSS4_DIE2,
    SPR_BOSS4_DIE3,
    SPR_BOSS4_DIE4,
    SPR_BOSS4_OUCH,
    SPR_BOSS4_MORPH1,
    SPR_BOSS4_MORPH2,
    SPR_BOSS4_MORPH3,
    SPR_BOSS4_MORPH4,
    SPR_BOSS4_MORPH5,
    SPR_BOSS4_MORPH6,
    SPR_BOSS4_MORPH7,
    SPR_BOSS4_MORPH8,
    SPR_BOSS4_MORPH9,
#endif

//
// BOSS 5
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS5_W1,
    SPR_BOSS5_W2,
    SPR_BOSS5_W3,
    SPR_BOSS5_W4,
    SPR_BOSS5_SWING1,
    SPR_BOSS5_SWING2,
    SPR_BOSS5_SWING3,
    SPR_BOSS5_DEAD,
    SPR_BOSS5_DIE1,
    SPR_BOSS5_DIE2,
    SPR_BOSS5_DIE3,
    SPR_BOSS5_DIE4,
    SPR_BOSS5_OUCH,
    SPR_BOSS5_PROJ1,
    SPR_BOSS5_PROJ2,
    SPR_BOSS5_PROJ3,
    SPR_BOSS5_EXP1,
    SPR_BOSS5_EXP2,
    SPR_BOSS5_EXP3,
#endif

//
// BOSS 6
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS6_W1,
    SPR_BOSS6_W2,
    SPR_BOSS6_W3,
    SPR_BOSS6_W4,
    SPR_BOSS6_SWING1,
    SPR_BOSS6_SWING2,
    SPR_BOSS6_SWING3,
    SPR_BOSS6_DEAD,
    SPR_BOSS6_DIE1,
    SPR_BOSS6_DIE2,
    SPR_BOSS6_DIE3,
    SPR_BOSS6_DIE4,
    SPR_BOSS6_OUCH,
#endif

//
// BOSS 7
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS7_W1,
    SPR_BOSS7_W2,
    SPR_BOSS7_W3,
    SPR_BOSS7_W4,
    SPR_BOSS7_SHOOT1,
    SPR_BOSS7_SHOOT2,
    SPR_BOSS7_SHOOT3,
    SPR_BOSS7_DEAD,
    SPR_BOSS7_DIE1,
    SPR_BOSS7_DIE2,
    SPR_BOSS7_DIE3,
    SPR_BOSS7_DIE4,
    SPR_BOSS7_OUCH,
#endif

//
// BOSS 8
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS8_W1,
    SPR_BOSS8_W2,
    SPR_BOSS8_W3,
    SPR_BOSS8_W4,
    SPR_BOSS8_SHOOT1,
    SPR_BOSS8_SHOOT2,
    SPR_BOSS8_SHOOT3,
    SPR_BOSS8_DIE1,
    SPR_BOSS8_DIE2,
    SPR_BOSS8_DIE3,
    SPR_BOSS8_DIE4,
    SPR_BOSS8_DEAD,
    SPR_BOSS8_OUCH,
#endif

//
// BOSS 9
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS9_W1,
    SPR_BOSS9_W2,
    SPR_BOSS9_W3,
    SPR_BOSS9_W4,
    SPR_BOSS9_SHOOT1,
    SPR_BOSS9_SHOOT2,
    SPR_BOSS9_SHOOT3,
    SPR_BOSS9_DIE1,
    SPR_BOSS9_DIE2,
    SPR_BOSS9_DIE3,
    SPR_BOSS9_DIE4,
    SPR_BOSS9_DEAD,
    SPR_BOSS9_OUCH,
#endif

//
// BOSS 10
//
#if GAME_VERSION != SHAREWARE_VERSION
    SPR_BOSS10_W1,
    SPR_BOSS10_W2,
    SPR_BOSS10_W3,
    SPR_BOSS10_W4,
    SPR_BOSS10_SHOOT1,
    SPR_BOSS10_SHOOT2,
    SPR_BOSS10_SHOOT3,
    SPR_BOSS10_DEAD,
    SPR_BOSS10_DIE1,
    SPR_BOSS10_DIE2,
    SPR_BOSS10_DIE3,
    SPR_BOSS10_DIE4,
    SPR_BOSS10_OUCH,

    SPR_BOSS10_SPIT1,
    SPR_BOSS10_SPIT2,
    SPR_BOSS10_SPIT3,
    SPR_BOSS10_SPIT_EXP1,
    SPR_BOSS10_SPIT_EXP2,
    SPR_BOSS10_SPIT_EXP3,
#endif

    //
    // PLASMA DETONATOR EXPLOSION
    //
    SPR_DETONATOR_EXP1,
    SPR_DETONATOR_EXP2,
    SPR_DETONATOR_EXP3,
    SPR_DETONATOR_EXP4,
    SPR_DETONATOR_EXP5,
    SPR_DETONATOR_EXP6,
    SPR_DETONATOR_EXP7,
    SPR_DETONATOR_EXP8,

    //
    // CLIP EXPLOSION
    //
    SPR_CLIP_EXP1,
    SPR_CLIP_EXP2,
    SPR_CLIP_EXP3,
    SPR_CLIP_EXP4,
    SPR_CLIP_EXP5,
    SPR_CLIP_EXP6,
    SPR_CLIP_EXP7,
    SPR_CLIP_EXP8,

    //
    // BFG CANNON WEAPON
    //
    SPR_BFG_WEAPON1,
    SPR_BFG_WEAPON2,
    SPR_BFG_WEAPON3,
    SPR_BFG_WEAPON4,
    SPR_BFG_WEAPON5,

    //
    // BFG CANNON WEAPON SHOTS
    //
    SPR_BFG_WEAPON_SHOT1,
    SPR_BFG_WEAPON_SHOT2,
    SPR_BFG_WEAPON_SHOT3,

    //
    // BFG SHOT EXPLOSION
    //
    SPR_BFG_EXP1,
    SPR_BFG_EXP2,
    SPR_BFG_EXP3,
    SPR_BFG_EXP4,
    SPR_BFG_EXP5,
    SPR_BFG_EXP6,
    SPR_BFG_EXP7,
    SPR_BFG_EXP8,

} sprite_t;

#if GAME_VERSION == SHAREWARE_VERSION
typedef enum
{

    SPR_STAT_1  = 0,
    SPR_STAT_3  = 0,
    SPR_STAT_4  = 0,
    SPR_STAT_5  = 0,
    SPR_STAT_6  = 0,
    SPR_STAT_7  = 0,
    SPR_STAT_16 = 0,
    SPR_STAT_17 = 0,
    SPR_STAT_19 = 0,
    SPR_STAT_20 = 0,
    SPR_STAT_21 = 0,
    SPR_STAT_22 = 0,
    SPR_STAT_23 = 0,
    SPR_STAT_25 = 0,
    SPR_STAT_30 = 0,
    SPR_STAT_37 = 0,
    SPR_STAT_39 = 0,
    SPR_CRATE_2 = 0,
    SPR_CRATE_3 = 0,
    SPR_STAT_58 = 0,
    SPR_STAT_59 = 0,
    SPR_STAT_64 = 0,
    SPR_STAT_70 = 0,
    SPR_STAT_71 = 0,
    SPR_STAT_72 = 0,
    SPR_STAT_73 = 0,
    SPR_STAT_74 = 0,
    SPR_STAT_75 = 0,
    SPR_STAT_76 = 0,

    SPR_GSCOUT_W1_1,
    SPR_GSCOUT_W1_2,
    SPR_GSCOUT_W1_3,
    SPR_GSCOUT_W1_4,
    SPR_GSCOUT_W1_5,
    SPR_GSCOUT_W1_6,
    SPR_GSCOUT_W1_7,
    SPR_GSCOUT_W1_8,

    SPR_GSCOUT_W2_1,
    SPR_GSCOUT_W2_2,
    SPR_GSCOUT_W2_3,
    SPR_GSCOUT_W2_4,
    SPR_GSCOUT_W2_5,
    SPR_GSCOUT_W2_6,
    SPR_GSCOUT_W2_7,
    SPR_GSCOUT_W2_8,

    SPR_GSCOUT_W3_1,
    SPR_GSCOUT_W3_2,
    SPR_GSCOUT_W3_3,
    SPR_GSCOUT_W3_4,
    SPR_GSCOUT_W3_5,
    SPR_GSCOUT_W3_6,
    SPR_GSCOUT_W3_7,
    SPR_GSCOUT_W3_8,

    SPR_GSCOUT_W4_1,
    SPR_GSCOUT_W4_2,
    SPR_GSCOUT_W4_3,
    SPR_GSCOUT_W4_4,
    SPR_GSCOUT_W4_5,
    SPR_GSCOUT_W4_6,
    SPR_GSCOUT_W4_7,
    SPR_GSCOUT_W4_8,

    SPR_GSCOUT_DIE1,
    SPR_GSCOUT_DIE2,
    SPR_GSCOUT_DIE3,
    SPR_GSCOUT_DIE4,
    SPR_GSCOUT_DIE5,
    SPR_GSCOUT_DIE6,
    SPR_GSCOUT_DIE7,
    SPR_GSCOUT_DIE8,

    SPR_GSCOUT_DEAD,

    SPR_FSCOUT_W2_1,
    SPR_FSCOUT_W2_2,
    SPR_FSCOUT_W2_3,
    SPR_FSCOUT_W2_4,
    SPR_FSCOUT_W2_5,
    SPR_FSCOUT_W2_6,
    SPR_FSCOUT_W2_7,
    SPR_FSCOUT_W2_8,

    SPR_FSCOUT_W3_1,
    SPR_FSCOUT_W3_2,
    SPR_FSCOUT_W3_3,
    SPR_FSCOUT_W3_4,
    SPR_FSCOUT_W3_5,
    SPR_FSCOUT_W3_6,
    SPR_FSCOUT_W3_7,
    SPR_FSCOUT_W3_8,

    SPR_FSCOUT_W4_1,
    SPR_FSCOUT_W4_2,
    SPR_FSCOUT_W4_3,
    SPR_FSCOUT_W4_4,
    SPR_FSCOUT_W4_5,
    SPR_FSCOUT_W4_6,
    SPR_FSCOUT_W4_7,
    SPR_FSCOUT_W4_8,

    SPR_VITAL_STAND,
    SPR_VITAL_DIE_1,
    SPR_VITAL_DIE_2,
    SPR_VITAL_DIE_3,
    SPR_VITAL_DIE_4,
    SPR_VITAL_DIE_5,
    SPR_VITAL_DIE_6,
    SPR_VITAL_DIE_7,
    SPR_VITAL_DIE_8,
    SPR_VITAL_DEAD_1,
    SPR_VITAL_DEAD_2,
    SPR_VITAL_DEAD_3,
    SPR_VITAL_OUCH,

    SPR_MUTHUM2_W1,
    SPR_MUTHUM2_W2,
    SPR_MUTHUM2_W3,
    SPR_MUTHUM2_W4,
    SPR_MUTHUM2_SWING1,
    SPR_MUTHUM2_SWING2,
    SPR_MUTHUM2_SWING3,
    SPR_MUTHUM2_DEAD,
    SPR_MUTHUM2_DIE1,
    SPR_MUTHUM2_DIE2,
    SPR_MUTHUM2_DIE3,
    SPR_MUTHUM2_DIE4,
    SPR_MUTHUM2_OUCH,
    SPR_MUTHUM2_SPIT1,
    SPR_MUTHUM2_SPIT2,
    SPR_MUTHUM2_SPIT3,

    SPR_LCAN_ALIEN_READY,
    SPR_LCAN_ALIEN_B1,
    SPR_LCAN_ALIEN_B2,
    SPR_LCAN_ALIEN_B3,
    SPR_LCAN_ALIEN_EMPTY,

    SPR_LCAN_ALIEN_W1,
    SPR_LCAN_ALIEN_W2,
    SPR_LCAN_ALIEN_W3,
    SPR_LCAN_ALIEN_W4,
    SPR_LCAN_ALIEN_SWING1,
    SPR_LCAN_ALIEN_SWING2,
    SPR_LCAN_ALIEN_SWING3,
    SPR_LCAN_ALIEN_DEAD,
    SPR_LCAN_ALIEN_DIE1,
    SPR_LCAN_ALIEN_DIE2,
    SPR_LCAN_ALIEN_DIE3,
    SPR_LCAN_ALIEN_DIE4,
    SPR_LCAN_ALIEN_OUCH,
    SPR_LCAN_ALIEN_SPIT1,
    SPR_LCAN_ALIEN_SPIT2,
    SPR_LCAN_ALIEN_SPIT3,

    SPR_GURNEY_MUT_READY,
    SPR_GURNEY_MUT_B1,
    SPR_GURNEY_MUT_B2,
    SPR_GURNEY_MUT_B3,
    SPR_GURNEY_MUT_EMPTY,

    SPR_GURNEY_MUT_W1,
    SPR_GURNEY_MUT_W2,
    SPR_GURNEY_MUT_W3,
    SPR_GURNEY_MUT_W4,
    SPR_GURNEY_MUT_SWING1,
    SPR_GURNEY_MUT_SWING2,
    SPR_GURNEY_MUT_SWING3,
    SPR_GURNEY_MUT_DEAD,
    SPR_GURNEY_MUT_DIE1,
    SPR_GURNEY_MUT_DIE2,
    SPR_GURNEY_MUT_DIE3,
    SPR_GURNEY_MUT_DIE4,
    SPR_GURNEY_MUT_OUCH,

    SPR_LIQUID_M1,
    SPR_LIQUID_M2,
    SPR_LIQUID_M3,
    SPR_LIQUID_R1,
    SPR_LIQUID_R2,
    SPR_LIQUID_R3,
    SPR_LIQUID_R4,
    SPR_LIQUID_S1,
    SPR_LIQUID_S2,
    SPR_LIQUID_S3,
    SPR_LIQUID_OUCH,
    SPR_LIQUID_DIE_1,
    SPR_LIQUID_DIE_2,
    SPR_LIQUID_DIE_3,
    SPR_LIQUID_DIE_4,
    SPR_LIQUID_DEAD,
    SPR_LIQUID_SHOT_FLY_1,
    SPR_LIQUID_SHOT_FLY_2,
    SPR_LIQUID_SHOT_FLY_3,
    SPR_LIQUID_SHOT_BURST_1,
    SPR_LIQUID_SHOT_BURST_2,
    SPR_LIQUID_SHOT_BURST_3,

    SPR_GREEN_OOZE1,
    SPR_GREEN_OOZE2,
    SPR_GREEN_OOZE3,
    SPR_BLACK_OOZE1,
    SPR_BLACK_OOZE2,
    SPR_BLACK_OOZE3,

    SPR_BOSS2_W1,
    SPR_BOSS2_W2,
    SPR_BOSS2_W3,
    SPR_BOSS2_W4,
    SPR_BOSS2_SWING1,
    SPR_BOSS2_SWING2,
    SPR_BOSS2_SWING3,
    SPR_BOSS2_DEAD,
    SPR_BOSS2_DIE1,
    SPR_BOSS2_DIE2,
    SPR_BOSS2_DIE3,
    SPR_BOSS2_DIE4,
    SPR_BOSS2_OUCH,
    SPR_BOSS2_SHOOT1,
    SPR_BOSS2_SHOOT2,
    SPR_BOSS2_SHOOT3,
    SPR_BOSS2_PROJ1,
    SPR_BOSS2_PROJ2,
    SPR_BOSS2_PROJ3,
    SPR_BOSS2_EXP1,
    SPR_BOSS2_EXP2,
    SPR_BOSS2_EXP3,

    SPR_BOSS3_W1,
    SPR_BOSS3_W2,
    SPR_BOSS3_W3,
    SPR_BOSS3_W4,
    SPR_BOSS3_SWING1,
    SPR_BOSS3_SWING2,
    SPR_BOSS3_SWING3,
    SPR_BOSS3_DEAD,
    SPR_BOSS3_DIE1,
    SPR_BOSS3_DIE2,
    SPR_BOSS3_DIE3,
    SPR_BOSS3_DIE4,
    SPR_BOSS3_OUCH,
    SPR_BOSS3_SHOOT1,
    SPR_BOSS3_SHOOT2,
    SPR_BOSS3_SHOOT3,
    SPR_BOSS3_PROJ1,
    SPR_BOSS3_PROJ2,
    SPR_BOSS3_PROJ3,
    SPR_BOSS3_EXP1,
    SPR_BOSS3_EXP2,
    SPR_BOSS3_EXP3,

    SPR_BOSS4_W1,
    SPR_BOSS4_W2,
    SPR_BOSS4_W3,
    SPR_BOSS4_W4,
    SPR_BOSS4_SWING1,
    SPR_BOSS4_SWING2,
    SPR_BOSS4_SWING3,
    SPR_BOSS4_DEAD,
    SPR_BOSS4_DIE1,
    SPR_BOSS4_DIE2,
    SPR_BOSS4_DIE3,
    SPR_BOSS4_DIE4,
    SPR_BOSS4_OUCH,
    SPR_BOSS4_SHOOT1,
    SPR_BOSS4_SHOOT2,
    SPR_BOSS4_SHOOT3,
    SPR_BOSS4_PROJ1,
    SPR_BOSS4_PROJ2,
    SPR_BOSS4_PROJ3,
    SPR_BOSS4_EXP1,
    SPR_BOSS4_EXP2,
    SPR_BOSS4_EXP3,

    SPR_BOSS5_W1,
    SPR_BOSS5_W2,
    SPR_BOSS5_W3,
    SPR_BOSS5_W4,
    SPR_BOSS5_SWING1,
    SPR_BOSS5_SWING2,
    SPR_BOSS5_SWING3,
    SPR_BOSS5_DEAD,
    SPR_BOSS5_DIE1,
    SPR_BOSS5_DIE2,
    SPR_BOSS5_DIE3,
    SPR_BOSS5_DIE4,
    SPR_BOSS5_OUCH,
    SPR_BOSS5_SHOOT1,
    SPR_BOSS5_SHOOT2,
    SPR_BOSS5_SHOOT3,
    SPR_BOSS5_PROJ1,
    SPR_BOSS5_PROJ2,
    SPR_BOSS5_PROJ3,
    SPR_BOSS5_EXP1,
    SPR_BOSS5_EXP2,
    SPR_BOSS5_EXP3,

    SPR_BOSS6_W1,
    SPR_BOSS6_W2,
    SPR_BOSS6_W3,
    SPR_BOSS6_W4,
    SPR_BOSS6_SWING1,
    SPR_BOSS6_SWING2,
    SPR_BOSS6_SWING3,
    SPR_BOSS6_DEAD,
    SPR_BOSS6_DIE1,
    SPR_BOSS6_DIE2,
    SPR_BOSS6_DIE3,
    SPR_BOSS6_DIE4,
    SPR_BOSS6_OUCH,
    SPR_BOSS6_SHOOT1,
    SPR_BOSS6_SHOOT2,
    SPR_BOSS6_SHOOT3,
    SPR_BOSS6_PROJ1,
    SPR_BOSS6_PROJ2,
    SPR_BOSS6_PROJ3,
    SPR_BOSS6_EXP1,
    SPR_BOSS6_EXP2,
    SPR_BOSS6_EXP3,
} dummy_sprite_t;
#endif

//
// Door Objects
//

typedef enum
{
    // LOCKED DOORS

    L_METAL,
    L_METAL_SHADE,

    L_BIO,
    L_BIO_SHADE,

    L_ELEVATOR,
    L_ELEVATOR_SHADE,

    L_SPACE,
    L_SPACE_SHADE,

    L_PRISON,
    L_PRISON_SHADE,

    L_HIGH_SECURITY,
    L_HIGH_SECURITY_SHADE,

    L_ENTER_ONLY,
    L_ENTER_ONLY_SHADE,

    L_HIGH_TECH,
    L_HIGH_TECH_SHADE,

    // UNLOCKED DOORS

    UL_METAL,
    UL_METAL_SHADE,

    UL_BIO,
    UL_BIO_SHADE,

    UL_ELEVATOR,
    UL_ELEVATOR_SHADE,

    UL_SPACE,
    UL_SPACE_SHADE,

    UL_PRISON,
    UL_PRISON_SHADE,

    UL_HIGH_SECURITY,
    UL_HIGH_SECURITY_SHADE,

    UL_ENTER_ONLY,
    UL_ENTER_ONLY_SHADE,

    UL_HIGH_TECH,
    UL_HIGH_TECH_SHADE,

    // MISC DOORS

    NOEXIT,
    NOEXIT_SHADE,

    STEEL_JAM,
    STEEL_JAM_SHADE,

    SPACE_JAM,
    SPACE_JAM_SHADE,

    OFFICE_JAM,
    OFFICE_JAM_SHADE,

    BIO_JAM,
    BIO_JAM_SHADE,

    SPACE_JAM_2,
    SPACE_JAM_2_SHADE,

    // END OF DOOR LIST

    NUMDOORTYPES,

} doortype;

//
// Breifing types - Note these are ordered to an char array in Breifing().
//

typedef enum
{
    BT_LOSE,
    BT_WIN,
    BT_INTRO,

} breifing_type;

// Terminal Messages - These correspond to the order in which they are
//							  grabbed in VGAGRAPH.BS?.  See BSTONEV.I
//

typedef enum
{
    TM_HINT,
    TM_JAM,
    TM_HELP,
    TM_APOGEE,
    TM_ID,
    TM_GOOBERS,
    TM_MIKE,
    TM_JIM,
    TM_JERRY,

    TM_JOSHUA,

    TM_STAR,

    TM_VITALS1,
    TM_VITALS2,

    TM_DEACTIVATE_TURRET,
    TM_TURRETS_ACTIVATED,
    TM_TURRETS_DEACTIVATED,

    TM_LINK,
    TM_LINK_OK,
    TM_LINK_BAD,

    TM_RADAR_OFF,
    TM_RADAR_ON,

    TM_SOUND_OFF,
    TM_SOUND_ON,

    TM_GOLDSTERN_TRACK_OFF,
    TM_GOLDSTERN_TRACK_ON,
    TM_GOLDSTERN_ARRIVED,
    TM_GOLDSTERN_WILL_AR,
    TM_GOLDSTERN_NO_PICK,
    TM_GOLDSTERN_NO_INFO,

    TM_RESET_SECURITY,
    TM_SECURITY_STATUS_OFF,
    TM_SECURITY_STATUS_ON,

    TM_TURRET_DEACTIVATE_BAD,
    TM_TURRET_DEACTIVATE_GOOD,

    TM_UNRECOGNIZED_COMMAND,
    TM_READY,
    TM_RETURN,
    TM_SECONDS,
    TM_CHEATER,
    TM_BLUEPRINTS,

    TM_PROFILE_WHO,
    TM_PROFILE_SARA,
    TM_PROFILE_BLAKE,
    TM_PROFILE_GOLDSTERN,
    TM_PROFILE_UNKNOWN,
    TM_DEACTIVATE_SOCKETS,
    TM_ACTIVATE_SOCKETS,
    TM_UNABLE_TO_PERFORM,
    TM_NO_SOCKETS,
    TM_ALREADY_ACTIVATED,
    TM_ALREADY_DEACTIVATED,
    TM_LAST,
} term_msg_type;

// Terminal Commands - The first set of commands TC_HINT - TC_end_of_1to1
//							  are directly mapped 1 to 1 to the terminal msgs.
//

typedef enum
{
    TC_HINT,
    TC_JAM,
    TC_HELP,
    TC_APOGEE,
    TC_THANKS,
    TC_GOOBERS,
    TC_MIKE,
    TC_JIM,
    TC_JERRY,

    // END OF ONE TO ONE LIST

    TC_JOSHUA,
    TC_STAR,
    TC_BLUEPRINT,
    TC_EXIT,
    TC_QUIT,
    TC_OFF,
    TC_BYE,
    TC_DISPLAY_PERSONNEL,
    TC_SOUND,
    TC_DISPLAY_GOLDSTERN,
    TC_ARRIVAL_GOLDSTERN,
    TC_DEACTIVATE_SECURITY,
    TC_DISPLAY_SECURITY,
    TC_SATALITE_STATUS,
    TC_DEACTIVATE_TURRETS,
    TC_TURRET_STATUS,
    TC_PROFILE,
    TC_SSTONE,
    TC_BSTONE,
    TC_GOLDSTERN,
    TC_DEACTIVATE_SOCKETS,
    TC_ACTIVATE_SOCKETS,

    TC_LAST,
} term_cmd_type;

//
// Barrier State Transistions
//

typedef enum
{
    bt_OFF,
    bt_ON,
    bt_DISABLING,
    bt_DISABLED,

    bt_OPENING, // For physical barriers
    bt_CLOSING, //	      " " "

} barrier_state_type;

/*
=============================================================================

                            GLOBAL TYPES

=============================================================================
*/

typedef long fixed;

// Display priority is determined by the order of these bits!
// And, this order must match the PinballBonus table in AGENT.C!
//
#define B_GALIEN_DESTROYED 0x0001
#define B_SCORE_ROLLED 0x0002
#define B_ONE_MILLION 0x0004
#define B_EXTRA_MAN 0x0008
#define B_ENEMY_DESTROYED 0x0010
#define B_TOTAL_POINTS 0x0020
#define B_INFORMANTS_ALIVE 0x0040

#define BONUS_QUEUE gamestuff.level[gamestate.mapon].bonus_queue
#define BONUS_SHOWN gamestuff.level[gamestate.mapon].bonus_shown

#define PinballBonusShown(bonus) (BONUS_SHOWN & bonus)
#define ActivatePinballBonus(bonus) \
    if (!PinballBonusShown(bonus))  \
    BONUS_QUEUE |= bonus

typedef struct
{
    char* BonusText; // REBA text pointer
    long  Points;    // Score for this bonus
    bool  Recurring; // Appear multiple times in a single level?
    void (*func)();  // Code to execute when you get this bonus.
} PinballBonusInfo;

typedef struct
{
    char tics;
    char attack;
    char frame; // attack is 1 for gun, 2 for knife
} atkinf_t;

typedef enum // NOTE - This enum list is ORDERED!
{
    mv_intro,
    mv_final,
    mv_NUM_MOVIES,

} movie_t;

typedef enum
{
    di_north,
    di_east,
    di_south,
    di_west
} controldir_t;

typedef enum
{ // NOTE - This enum list is ORDERED!
    dr_bio,
    dr_normal,
    dr_prison,
    dr_elevator,
    dr_high_security,
    dr_office,
    dr_oneway_left,
    dr_oneway_up,
    dr_oneway_right,
    dr_oneway_down,
    dr_space,
} door_t;

typedef enum
{
    kt_none = -1,
    kt_red,
    kt_yellow,
    kt_blue,
    NUMKEYS,
} keytype;

typedef enum
{
    ac_badobject = -1,
    ac_no,
    ac_yes,
    ac_allways
} activetype;

typedef enum
{
    nothing,
    playerobj,
    inertobj,
    fixup_inertobj,
    deadobj,

    // BEGIN - Start of ordered list for ActorInfoMsg[] for attacking
    //         actor REBA messages

    rentacopobj,
    hang_terrotobj,
    gen_scientistobj,
    podobj,
    electroobj,
    electrosphereobj,
    proguardobj,
    genetic_guardobj,
    mutant_human1obj,
    mutant_human2obj,
    lcan_wait_alienobj,
    lcan_alienobj,
    scan_wait_alienobj,
    scan_alienobj,
    gurney_waitobj,
    gurneyobj,
    liquidobj,
    swatobj,
    goldsternobj,
    gold_morphobj,
    volatiletransportobj,
    floatingbombobj,
    rotating_cubeobj,

    spider_mutantobj,
    breather_beastobj,
    cyborg_warriorobj,
    reptilian_warriorobj,
    acid_dragonobj,
    mech_guardianobj,

    final_boss1obj,
    final_boss2obj,
    final_boss3obj,
    final_boss4obj,

    blakeobj,

    crate1obj,
    crate2obj,
    crate3obj,

    green_oozeobj,
    black_oozeobj,
    green2_oozeobj,
    black2_oozeobj,
    podeggobj,

    morphing_spider_mutantobj,
    morphing_reptilian_warriorobj,
    morphing_mutanthuman2obj,

    SPACER1_OBJ,
    electroshotobj, // NON-HITPOINT objects...
    post_barrierobj,
    arc_barrierobj,
    vpost_barrierobj,
    vspike_barrierobj,
    goldmorphshotobj,

    security_lightobj,
    explosionobj,
    steamgrateobj,
    steampipeobj,

    liquidshotobj,

    lcanshotobj,
    podshotobj,
    scanshotobj,
    dogshotobj,
    mut_hum1shotobj,

    ventdripobj,
    playerspshotobj,
    flickerlightobj,

    plasma_detonatorobj,
    plasma_detonator_reserveobj,

    grenadeobj,
    bfg_shotobj,
    bfg_explosionobj,
    pd_explosionobj,

    spider_mutantshotobj,
    breather_beastshotobj,
    cyborg_warriorshotobj,
    reptilian_warriorshotobj,
    acid_dragonshotobj,
    mech_guardianshotobj,
    final_boss2shotobj,
    final_boss4shotobj,

    doorexplodeobj, // Door explosion_anim acto
    gr_explosionobj,
    gold_morphingobj,

} classtype;

//
// NOTE: When adding bonus objects - Make sure that they are added
//	-----	at the bottom of the list or that BonusMsg[] is correctly
//			updated.
//

typedef enum
{
    dressing,

    bo_red_key,
    bo_yellow_key,
    bo_blue_key,

    bo_clip,
    bo_clip2,
    bo_pistol,
    bo_burst_rifle,
    bo_ion_cannon,
    bo_grenade,
    bo_bfg_cannon,

    // START of Bonus Health Ordered list

    bo_fullheal,
    bo_firstaid,
    bo_ham,
    bo_chicken,
    bo_sandwich,
    bo_candybar,
    bo_water,
    bo_water_puddle,

    // END of ordered ...

    bo_money_bag,
    bo_loot,

    bo_gold1,
    bo_gold2,
    bo_gold3,

    bo_gold,
    bo_bonus,

    bo_plainvent,
    bo_bloodvent,
    bo_watervent,

    bo_coin,
    bo_coin5,

    bo_plasma_detonator,
    bo_automapper1,

    bo_nothing,

    block,

} stat_t;

typedef struct
{
    int    picnum;
    stat_t type;
} stattype;

typedef enum
{
    east,
    northeast,
    north,
    northwest,
    west,
    southwest,
    south,
    southeast,
    nodir
} dirtype;

typedef enum
{
    en_rentacop, // Actors with hitpoints (normal actors)
    en_hang_terrot,
    en_gen_scientist,
    en_pod,
    en_electro_alien,
    en_electrosphere,
    en_proguard,
    en_genetic_guard,
    en_mutant_human1,
    en_mutant_human2,
    en_lcan_wait_alien,
    en_lcan_alien,
    en_scan_wait_alien,
    en_scan_alien,
    en_gurney_wait,
    en_gurney,
    en_liquid,
    en_swat,
    en_goldstern,
    en_gold_morph,
    en_volatiletransport,
    en_floatingbomb,
    en_rotating_cube,

    en_spider_mutant,
    en_breather_beast,
    en_cyborg_warrior,
    en_reptilian_warrior,
    en_acid_dragon,
    en_mech_guardian,

    en_final_boss1,
    en_final_boss2,
    en_final_boss3,
    en_final_boss4,

    en_blake,

    en_crate1,
    en_crate2,
    en_crate3,

    en_green_ooze,
    en_black_ooze,
    en_green2_ooze,
    en_black2_ooze,
    en_podegg,

    en_morphing_spider_mutant,
    en_morphing_reptilian_warrior,
    en_morphing_mutanthuman2,

    NUMHITENEMIES,

    en_electro_shot, // Actors WITHOUT hitpoints (abnormal actors?)
    en_post_barrier,
    en_arc_barrier,
    en_vpost_barrier,
    en_vspike_barrier,
    en_goldmorphshot,

    en_security_light,
    en_explosion,
    en_steamgrate,
    en_steampipe,

    en_liquidshot,

    en_lcanshot,
    en_podshot,
    en_scanshot,
    en_dogshot,
    en_mut_hum1shot,

    en_ventdrip,
    en_playerspshotobj,
    en_flickerlight,

    en_plasma_detonator,
    en_plasma_detonator_reserve,

    en_vertsphere, // Actor types only used for spawning.
    en_horzsphere,
    en_diagsphere,
    en_bloodvent,
    en_watervent,
    NUMENEMIES
} enemy_t;

#define SF_ROTATE 0x01
#define SF_PAINFRAME 0x02

typedef struct statestruct
{
    byte flags;
    int  shapenum; // a shapenum of -1 means get from ob->temp1
    int  tictime;
    void (*think)(), (*action)();
    struct statestruct* next;
} statetype;

//---------------------
//
// trivial actor structure
//
//---------------------

typedef struct statstruct
{
    byte tilex, tiley;
    byte areanumber;

    byte*    visspot;
    int      shapenum; // if shapenum == -1 the obj has been removed
    unsigned flags;
    byte     itemnumber;
    char     lighting;
} statobj_t;

//---------------------
//
// door actor structure
//
//---------------------

typedef struct doorstruct
{
    byte    tilex, tiley;
    bool    vertical;
    char    flags;
    keytype lock;
    door_t  type;
    enum
    {
        dr_open,
        dr_closed,
        dr_opening,
        dr_closing,
        dr_jammed
    } action;
    int  ticcount;
    byte areanumber[2];
} doorobj_t;

//--------------------
//
// thinking actor structure
//
//--------------------

typedef struct objstruct
{
    byte tilex, tiley;
    byte areanumber;

    activetype active;
    int        ticcount;
    classtype  obclass;
    statetype* state;

    unsigned long flags;
    unsigned      flags2; // Aux flags

    long    distance; // if negative, wait for that door to open
    dirtype dir;
    dirtype trydir; // "bit 7" == "direction to turn" flag

    fixed         x, y;
    unsigned char s_tilex, s_tiley; // s_tilex==0, running for corner

    int      viewx;
    unsigned viewheight;
    fixed    transx, transy; // in global coord

    int           hitpoints;
    unsigned char ammo;
    char          lighting;
    unsigned      linc;
    int           angle;
    long          speed;

    int      temp1;
    int      temp2;
    unsigned temp3; // holds 'last door used' by 'smart' actors

    struct objstruct *next, *prev;
} objtype;

enum
{
    bt_nobutton = -1,
    bt_attack   = 0,
    bt_strafe,
    bt_run,
    bt_use,
    bt_ready_autocharge,
    bt_ready_pistol,
    bt_ready_burst_rifle,
    bt_ready_ion_cannon,
    bt_ready_grenade,
    bt_ready_bfg_cannon,
    bt_ready_plasma_detonators,

    bt_SPACER,

    NUMBUTTONS,
};

typedef enum
{
    wp_autocharge,
    wp_pistol,
    wp_burst_rifle,
    wp_ion_cannon,
    wp_grenade,
    wp_bfg_cannon,
    //	wp_plasma_detonators,

    wp_SPACER,
    NUMWEAPONS,
} weapontype;

typedef enum
{
    gd_baby,
    gd_easy,
    gd_medium,
    gd_hard
};

typedef enum
{
    ELEVATOR_BACK,
    TRANSPORTER_BACK

} backgroundtype;

//
// General Coord (tile) structure
//
typedef struct
{
    unsigned char tilex, tiley;
} tilecoord_t;

//-----------------------------------
//
// barrier coord/table structure
//
//-----------------------------------

typedef struct
{
    tilecoord_t   coord;
    unsigned char on;
} barrier_type;

//---------------
//
// gamestate structure
//
//---------------

typedef struct statsInfoType
{
    long  total_points, accum_points;
    byte  total_enemy, accum_enemy;
    byte  total_inf, accum_inf;
    short overall_floor;
} statsInfoType;

typedef struct
{
    unsigned      bonus_queue; // bonuses that need to be shown
    unsigned      bonus_shown; // bonuses that have been shown
    bool          locked;
    statsInfoType stats;
    byte          ptilex, ptiley;
    int           pangle;
} levelinfo;

typedef struct
{
    levelinfo old_levelinfo[MAPS_PER_EPISODE];
    levelinfo level[MAPS_PER_EPISODE];
} fargametype;

typedef struct
{
    int      turn_around, turn_angle;
    unsigned flags;
    int      lastmapon;
    int      difficulty;
    int      mapon;
    int      status_refresh;
    long     oldscore, tic_score, score, nextextra;
    int      score_roll_wait;
    int      lives;
    int      health;
    int      health_delay;
    char     health_str[4];

    int  rpower, old_rpower;
    char rzoom;
    char radar_leds, lastradar_leds;

    char lastammo_leds;
    char ammo_leds;
    int  ammo, old_ammo;

    int plasma_detonators, old_plasma_detonators;

    char useable_weapons;
    char weapons, weapon, chosenweapon, old_weapons[4];
    char key_floor;

    char         weapon_wait;
    int          attackframe, attackcount, weaponframe;
    int          episode;
    long         TimeCount;
    long         killx, killy;
    char*        msg; // InfoArea msg...
    char         numkeys[NUMKEYS], old_numkeys[NUMKEYS];
    barrier_type barrier_table[MAX_BARRIER_SWITCHES];
    barrier_type old_barrier_table[MAX_BARRIER_SWITCHES];
    unsigned     tokens, old_tokens;
    bool         boss_key_dropped, old_boss_key_dropped;
    int          wintilex, wintiley;
} gametype;

typedef enum
{
    ex_stillplaying,
    ex_completed,
    ex_transported,
    ex_died,
    ex_warped,
    ex_resetgame,
    ex_loadedgame,
    ex_victorious,
    ex_abort,
    ex_demodone,
    ex_secretlevel,
    ex_title,
} exit_t;

typedef struct
{
    unsigned char init_delay;
    unsigned char delay_count;
    unsigned char firstreg;
    unsigned char lastreg;
} CycleInfo;

typedef struct
{
    int viewx,
        viewheight,
        shapenum;
    char lighting;
    char cloaked;
} visobj_t;

typedef enum
{
    at_NONE = 0,
    at_CYCLE,
    at_REBOUND,
    at_ONCE
} animtype_t;
typedef enum
{
    ad_FWD = 0,
    ad_REV
} animdir_t;

typedef struct ofs_anim_struct
{
    unsigned animtype : 2; // animtype_t
    unsigned curframe : 5;
    unsigned maxframe : 5;
    unsigned animdir : 1; // animdir_t
    unsigned extra : 3;

} ofs_anim_t;

//
// DisplayInfoMsg Priorities - Duplicate Values are Allowed.
//
// ("enum" list used simply for debuging use like Object Classes)
//

//
// Msg_Priorities - Hell.. Lets just make them all the same...

typedef enum
{
    MP_min_val = 0,

    MP_HEARTB_SND         = 0x0200,
    MP_WALLSWITCH_OPERATE = 0x0200,

    MP_DOOR_OPERATE       = 0x0200,
    MP_CONCESSION_OPERATE = 0x0200,
    MP_WEAPON_AVAIL       = 0x0200,

    MP_ATTACK_INFO        = 0x0200,
    MP_NO_MORE_AMMO       = 0x0200,
    MP_WEAPON_MALFUNCTION = 0x0200,

    MP_INTERROGATE          = 0x0200,
    MP_CONCESSION_HINT      = 0x0200,
    MP_NO_MORE_TOKENS       = 0x0200,
    MP_CONCESSION_OUT_ORDER = 0x0200,

    MP_BONUS = 0x0200,

    MP_TAKE_DAMAGE = 0x0200,
    MP_DETONATOR   = 0x0200,

    MP_PINBALL_BONUS  = 0x3000,
    MP_FLOOR_UNLOCKED = 0x3000,

    MP_POWERUP = 0x7000, // Power-Up/Game-Start Value
    MP_max_val = 0x7FFF, // DO NOT USE/EXCEED - MAX Val
} msg_priorities;

typedef enum
{
    MT_NOTHING,
    MT_CLEAR,
    MT_ATTACK,
    MT_GENERAL,
    MT_OUT_OF_AMMO,
    MT_MALFUNCTION,
    MT_NO_MO_FOOD_TOKENS,
    MT_BONUS,

} infomsg_type;

//
// Menu Instruction Text types...
//
typedef enum
{
    IT_STANDARD,
    IT_HIGHSCORES,
    IT_ENTER_HIGHSCORE,
    IT_MOUSE_SEN,
    MAX_INSTRUCTIONS,

} inst_type;

#define MAX_CACHE_MSGS 30
#define MAX_CACHE_MSG_LEN 190

//------------------------- BASIC STRUCTURES -----------------------------

// Basic 'message info' structure
//
typedef struct
{
    unsigned char local_val;  // where msg is in 'local' list
    unsigned char global_val; // where msg was in 'global' list
    memptr        mSeg;       // pointer to message
} mCacheInfo;

// Basic 'message list' structure
//
typedef struct
{
    short      NumMsgs;               // number of messages
    mCacheInfo mInfo[MAX_CACHE_MSGS]; // table of message 'info'
} mCacheList;

//----------------------- CONCESSION STRUCTURES --------------------------

// Concession 'message info' structure
//
typedef struct
{
    mCacheInfo    mInfo;
    unsigned char type;        // type of concession
    unsigned char operate_cnt; // # of times req'd to operate
} con_mCacheInfo;

// Concession 'message list' structure
//
typedef struct
{
    short          NumMsgs; // also, num concessions
    con_mCacheInfo cmInfo[MAX_CACHE_MSGS];
} concession_t;

//------------------------ INFORMANT STRUCTURES --------------------------

// Informant 'message info' structure
//
typedef struct
{
    mCacheInfo mInfo;
    byte       areanumber; // 'where' msg can be used
} sci_mCacheInfo;

// Informant 'message list' structure
//
typedef struct
{
    short          NumMsgs;
    sci_mCacheInfo smInfo[MAX_CACHE_MSGS];
} scientist_t;

//------------------------------------------------------------------------

// Electro-Alien controller structer
//
typedef struct
{
    char  tilex, tiley; // where this controller is in the map.
    char  aliens_out;   // aliens spawned by this controller.
    short delay;        // delay before spawning another alien.
} eaWallInfo;

// General Structure to hold goldstern specific stuff...
//

typedef struct
{
    unsigned char LastIndex;   // Last Spawn Coord Index
    unsigned char SpawnCnt;    // Num of Spawnpoints for Goldstern
    unsigned      flags;       // What type of command/operation is needed...
    unsigned      WaitTime;    // Wait time for Goldstern Spawn (current & Next)
    bool          GoldSpawned; // Has Goldstern been spawned?
} GoldsternInfo_t;

typedef struct
{
    long x, y, z;
    byte color;
} star_t;

#define MAX_SPACE_THRUST ((long)0x4fff)

#define MAX_STARS 128
#define MAX_SPACE_STATS (((MAXSTATS * sizeof(statobj_t)) - ((MAX_STARS + 1) * sizeof(star_t))) / sizeof(statobj_t))

/*
=============================================================================

                         3D_MAIN DEFINITIONS

=============================================================================
*/

#define TITLE_LOOP_MUSIC PLOT_MUS

#define CANT_PLAY_TXT "\n"                                \
                      "ERROR: Insufficient disk space.\n" \
                      "Try deleting some files from your hard disk.\n\n"

extern char tempPath[];

extern const float radtoint; // = (float)FINEANGLES/2/PI;

extern levelinfo default_level[MAPS_PER_EPISODE];
extern short     view_xl, view_xh, view_yl, view_yh;
extern short     starting_level, debug_value, starting_episode, starting_difficulty;

extern bool MS_CheckParm(char* string);

extern signed char lastmap_tilex, lastmap_tiley;
extern unsigned    TopColor, BottomColor;
extern char        str[80], str2[20];
// extern  unsigned        tedlevelnum;
// extern  bool         tedlevel;
extern bool nospr;
extern bool IsA386;

extern fixed    focallength;
extern unsigned viewangles;
extern unsigned screenofs;
extern int      viewwidth;
extern int      viewheight;
extern int      centerx;
extern int      shootdelta;

extern int dirangle[9];

extern bool startgame, loadedgame;
extern int  mouseadjustment;
//
// math tables
//
extern int   pixelangle[MAXVIEWWIDTH];
extern long  finetangent[FINEANGLES / 4];
extern fixed sintable[], *costable;

//
// derived constants
//
extern fixed scale, maxslope;
extern long  heightnumerator;
extern int   minheightdiv;

extern char configname[13];

extern bool ShowQuickMsg;

long DeleteChunk(int handle, char* chunk);

void     LoadFonts(void);
void     ClearNClose(void);
void     CycleColors(void);
void     LoadAccessCodes(void);
void     AlignPlayerInElevator(void);
void     HelpScreens(void);
void     OrderingInfo(void);
void     TEDDeath(void);
void     CalcProjection(long focal);
bool     SetViewSize(unsigned width, unsigned height);
void     NewGame(int difficulty, int episode);
void     NewViewSize(int width);
unsigned scan_atoi(char* s);
void     AlignPlayerOnTransporter(void);

unsigned UseFunc(char* first, char* next);
bool     DoMovie(movie_t movie, memptr palette);
bool     CheckDiskSpace(long needed, char* text, cds_io_type io_type);
bool     SaveTheGame(int handle, char* description);
long     ChecksumFile(char* file, long checksum);
void     BadChecksum(void);
void     InvalidLevels(void);
void     CheckValidity(char* file, long valid_checksum);
void     UnauthorizedLevels(void);
void     ShowChecksums(void);
void     fprint(char* text);

void SetupWalls(void);
void InitDigiMap(void);

void CleanUpDoors_N_Actors(void);

void MakeDestPath(char* file);
void InitDestPath(void);

extern long FindChunk(int file, char* chunk);
extern long NextChunk(int file);

/*
=============================================================================

                         3D_GAME DEFINITIONS

=============================================================================
*/

extern int             db_count;
extern classtype       debug_bonus[2][800];
extern fargametype     gamestuff;
extern tilecoord_t     GoldieList[GOLDIE_MAX_SPAWNS];
extern GoldsternInfo_t GoldsternInfo;

extern unsigned char VitalsRemain, VitalsOnFloor;

extern eaWallInfo eaList[];
extern char       NumEAWalls, NumEASpawned;
extern bool       ingame, fizzlein, screensplit;
extern unsigned   latchpics[NUMLATCHPICS];
extern gametype   gamestate;
extern int        doornum;

extern char demoname[13];

void DrawPlayBorder(void);
void ScanInfoPlane(void);
void SetupGameLevel(void);
void NormalScreen(void);
void DrawPlayScreen(bool);
void FizzleOut(void);
void GameLoop(void);

// JAB
#define PlaySoundLocTile(s, tx, ty) PlaySoundLocGlobal(s, (((long)(tx) << TILESHIFT) + (1L << (TILESHIFT - 1))), (((long)ty << TILESHIFT) + (1L << (TILESHIFT - 1))))
#define PlaySoundLocActor(s, ob) PlaySoundLocGlobal(s, (ob)->x, (ob)->y)
void PlaySoundLocGlobal(word s, fixed gx, fixed gy);

void Warped(void);
void RotateView(int DestAngle, unsigned char RotSpeed);
void DrawWarpIn(void);
void BMAmsg(char* msg);
void CacheBMAmsg(unsigned MsgNum);
void BevelBox(short xl, short yl, short w, short h, byte hi, byte med, byte lo);

void AddTotalPoints(unsigned points);
void AddTotalInformants(char informants);
void AddTotalEnemy(unsigned enemies);

void ShadowPrintLocationText(sp_type type);
void LoseScreen(void);
void LoadLocationText(short textNum);

/*
=============================================================================

                         3D_PLAY DEFINITIONS

=============================================================================
*/

extern int objcount;

extern objtype *     DeadGuys[], dummyobj;
extern unsigned char NumDeadGuys;

extern exit_t playstate;

extern int bordertime;

extern bool          madenoise, usedummy, nevermark;
extern unsigned char alerted, alerted_areanum;

extern objtype objlist[MAXACTORS], *new_, *player, *lastobj,
    *objfreelist, *killerobj;
extern statobj_t statobjlist[MAXSTATS], *laststatobj;
extern doorobj_t doorobjlist[MAXDOORS], *lastdoorobj;

extern unsigned farmapylookup[MAPSIZE];
extern byte*    nearmapylookup[MAPSIZE];

extern byte     tilemap[MAPSIZE][MAPSIZE]; // wall values only
extern byte     spotvis[MAPSIZE][MAPSIZE];
extern objtype* actorat[MAPSIZE][MAPSIZE];

#define UPDATESIZE (UPDATEWIDE * UPDATEHIGH)
extern byte update[UPDATESIZE];

extern bool     singlestep, godmode, noclip;
extern int      extravbls;
extern int      DebugOk;
extern int      InstantWin, InstantQuit;
extern bool     PowerBall;
extern int      TestQuickSave, TestAutoMapper;
extern unsigned ExtraRadarFlags;

//
// control info
//
extern bool mouseenabled, joystickenabled, joypadenabled, joystickprogressive;
extern int  joystickport;
extern int  dirscan[4];
extern int  buttonscan[NUMBUTTONS];
extern int  buttonmouse[4];
extern int  buttonjoy[4];

extern bool buttonheld[NUMBUTTONS];

extern int viewsize;

//
// curent user input
//
extern int  controlx, controly; // range from -100 to 100
extern bool buttonstate[NUMBUTTONS];

extern bool   demorecord, demoplayback;
extern char * demoptr, *lastdemoptr;
extern memptr demobuffer;

extern char Computing[];

void CenterWindow(word w, word h);
void InitActorList(void);
void GetNewActor(void);
void RemoveObj(objtype* gone);
void PollControls(void);
void StopMusic(void);
void StartMusic(bool startit);
void PlayLoop(void);

void ChangeSwapFiles(bool display);
void OpenPageFile(void);
void CheckMusicToggle(void);

/*
=============================================================================

                            3D_DEBUG

=============================================================================
*/

int  DebugKeys(void);
void CalcMemFree(void);

/*
=============================================================================

                         3D_DRAW DEFINITIONS

=============================================================================
*/

extern byte TravelTable[MAPSIZE][MAPSIZE];

extern int weaponchangetics, itemchangetics, bodychangetics;
extern int plaqueon, plaquetime, plaquetimefrac, getpic;

extern statobj_t* firststarobj;

extern unsigned screenloc[3];
extern unsigned freelatch;

extern long space_xmove, space_ymove;

extern long lasttimecount;
extern long framecount;
extern long frameon;
extern bool fizzlein;

extern unsigned wallheight[MAXVIEWWIDTH];

extern fixed focallength;
extern fixed mindist;

//
// math tables
//
extern int   pixelangle[MAXVIEWWIDTH];
extern long  finetangent[FINEANGLES / 4];
extern fixed sintable[], *costable;

//
// derived constants
//
extern fixed scale;
extern long  heightnumerator, mindist;

//
// refresh variables
//
extern fixed viewx, viewy; // the focal point
extern int   viewangle;
extern fixed viewsin, viewcos;

extern long     postsource;
extern unsigned postx;
extern unsigned postwidth;

extern int horizwall[], vertwall[];

extern unsigned pwallpos;

extern bool cloaked_shape;

fixed FixedByFrac(fixed a, fixed b);
void  TransformActor(objtype* ob);
void  BuildTables(void);
void  ClearScreen(void);
int   CalcRotate(objtype* ob);
void  DrawScaleds(void);
void  CalcTics(void);
void  FixOfs(void);
void  ThreeDRefresh(void);
void  FarScalePost(void);
void  DrawStars(void);

bool TransformTile(int tx, int ty, int* dispx, int* dispheight);
void WrapTransformActor(objtype* ob);
void ComputeActorPosition(objtype* ob, char adjust_x, char adjust_y);
void WrapDrawScaleds(void);
bool WrapActorPosition(objtype* obj);
void WrapStaticPosition(statobj_t* statptr, visobj_t* visptr);
void ShowOverhead(short bx, short by, short radius, short zoom, unsigned flags);
void UpdateTravelTable(void);

/*
=============================================================================

                         3D_DRAW2 DEFINITIONS

=============================================================================
*/
extern unsigned CeilingTile, FloorTile;
extern void (*MapRowPtr)();

void DrawPlanes(void);

void MapRow();
void C_MapRow();
void F_MapRow();

/*
=============================================================================

                         3D_STATE DEFINITIONS

=============================================================================
*/

extern unsigned actor_points[];
extern dirtype  opposite[9];
extern dirtype  diagonal[9][9];

void     SeekPlayerOrStatic(objtype* ob, int* deltax, int* deltay);
unsigned CheckRunChase(objtype* ob);
void     GetCornerSeek(objtype* ob);
bool     LookForGoodies(objtype* ob, unsigned RunReason);
void     InitHitRect(objtype* ob, unsigned radius);
void     SpawnNewObj(unsigned tilex, unsigned tiley, statetype* state);
void     NewState(objtype* ob, statetype* state);

bool TryWalk(objtype* ob, bool moveit);
void SelectChaseDir(objtype* ob);
void SelectDodgeDir(objtype* ob);
void MoveObj(objtype* ob, long move);

void KillActor(objtype* ob);
void DamageActor(objtype* ob, unsigned damage, objtype* attacker);

bool CheckLine(objtype* from_obj, objtype* to_obj);
bool CheckSight(objtype* from_obj, objtype* to_obj);

bool PosVisable(fixed from_x, fixed from_y, fixed to_x, fixed to_y, int from_angle);
bool PlayerIsBlocking(objtype* ob);

void     MakeAlertNoise(objtype* obj);
objtype* CheckAndReserve(void);

/*
=============================================================================

                         3D_SCALE DEFINITIONS

=============================================================================
*/

#define COMPSCALECODESTART (65 * 4) // offset to start of code in comp scaler

typedef struct
{
    unsigned codeofs[65];
    unsigned width[65];
    byte     code[];
} t_compscale;

typedef struct
{
    unsigned leftpix, rightpix;
    unsigned dataofs[64];
    // table data after dataofs[rightpix-leftpix+1]
} t_compshape;

extern t_compscale* scaledirectory[MAXSCALEHEIGHT + 1];
extern long         fullscalefarcall[MAXSCALEHEIGHT + 1];

extern byte     bitmasks1[8][8];
extern byte     bitmasks2[8][8];
extern unsigned wordmasks[8][8];

extern byte mapmasks1[4][8];
extern byte mapmasks2[4][8];
extern byte mapmasks3[4][8];

extern int  maxscale, maxscaleshl2;
extern bool scaledir_avail;

extern int normalshade;
extern int normalshade_div;
extern int shade_max;

void FreeScaleDirectory(void);
void SetupScaling(int maxscaleheight);
void ScaleShape(int xcenter, int shapenum, unsigned height);
void SimpleScaleShape(int xcenter, int shapenum, unsigned height);
void MegaSimpleScaleShape(int xcenter, int ycenter, int shapenum, unsigned height, unsigned shade);

/*
=============================================================================

                         3D_AGENT DEFINITIONS

=============================================================================
*/

extern scientist_t InfHintList;
extern scientist_t NiceSciList;
extern scientist_t MeanSciList;

extern unsigned  static_points[];
extern bool      GAN_HiddenArea;
extern memptr    InfAreaMsgs[];
extern byte      NumAreaMsgs, LastInfArea;
extern short     FirstGenInfMsg, TotalGenInfMsgs;
extern classtype LastInfoAttacker;
extern int       LastInfoAttacker_Cloaked;

extern char term_com_name[];
extern char term_msg_name[];

extern atkinf_t attackinfo[7][14];

//
// player state info
//
extern bool     commandmode;
extern long     thrustspeed;
extern unsigned plux, pluy; // player coordinates scaled to unsigned
extern bool     PlayerInvisable;
extern char     DrawInfoArea_COUNT;
extern char     InitInfoArea_COUNT;

extern unsigned player_oldtilex;
extern unsigned player_oldtiley;

// Terminal variables

extern unsigned RadarSw;

// Location Bar message string...

extern char LocationText[MAX_LOCATION_DESC_LEN];

void JLatchDrawPic(unsigned x, unsigned y, unsigned picnum);

//
// search / move info
//
extern unsigned searchon;   // held object number looking at
extern int      searchtics; // when it reaches SEARCHTICS, get an obj
extern objtype* searchobj;  // current object being searched
extern unsigned foundflag;  // only show NOTHING if nothing was found
extern objtype* moveobj;    // current object being draged

extern int anglefrac;
extern int facecount;

extern unsigned LastMsgPri;
extern short    MsgTicsRemain;

void GivePoints(long score, bool add_to_stats);
void SpawnPlayer(int tilex, int tiley, int dir);
void DrawCash(void);
void UpdateHealth(void);

void DrawAmmoGuage(void);
void DrawAmmoMsg(void);
void DrawAmmo(bool ForceRefresh);
bool DisplayInfoMsg(char* Msg, msg_priorities Priority, short DisplayTime, short MessageType);
void UpdateInfoAreaClock(void);
void UpdateInfoArea(void);
void DrawHealthMonitor(void);
void CalcHealthDisplay(void);
void UpdateScore(void);

byte  ValidAreaTile(unsigned* ptr);
char  GetAreaNumber(char tilex, char tiley);
short InputFloor(void);

void  RestoreInfoArea(void);
void  DrawHeartPic(void);
void  DrawInfoArea(void);
short DrawShape(short x, short y, short shapenum, pisType shapetype);

void AnimatePage(void);

void ActivateTerminal(bool);
void TerminalPrint(char* msg, bool FastPrint);
void FloorCheat(unsigned RadarFlags);
bool Interrogate(objtype* ob);

void GiveKey(int key);
void TakeKey(int key);
void GiveToken(int tokens);

void TakePlasmaDetonator(int count);
void GivePlasmaDetonator(int count);

void CacheDrawPic(short x, short y, short pic);
void LoadTerminalCommands(void);

void     ActivateWallSwitch(unsigned iconnum, short x, short y);
unsigned UpdateBarrierTable(unsigned char x, unsigned char y, bool OnOff);
unsigned ScanBarrierTable(unsigned char x, unsigned char y);
void     DisplaySwitchOperateMsg(unsigned coords);

void  DisplayNoMoMsgs(void);
void  PrintStatPercent(short nx, short ny, char percentage);
short ShowStats(short bx, short by, ss_type type, statsInfoType* stats);
bool  PerfectStats(void);
bool  CheckPerfectStats(void);
bool  OperateSmartSwitch(unsigned tilex, unsigned tiley, char Operation, bool Force);

/*
=============================================================================

                         3D_ACT1 DEFINITIONS

=============================================================================
*/
extern char         xy_offset[8][2];
extern stattype     statinfo[];
extern concession_t ConHintList;

extern doorobj_t doorobjlist[MAXDOORS], *lastdoorobj;
extern int       doornum;

extern unsigned doorposition[MAXDOORS], pwallstate;

extern byte areaconnect[NUMAREAS][NUMAREAS];

extern bool areabyplayer[NUMAREAS];

extern unsigned pwallstate;
extern unsigned pwallpos; // amount a pushable wall has been moved (0-63)
extern unsigned pwallx, pwally;
extern int      pwalldir, pwalldist;

statobj_t* ReserveStatic(void);
void       SpawnStatic(int tilex, int tiley, int type);
void       SpawnDoor(int tilex, int tiley, bool vertical, keytype lock, door_t type);

void     OperateConcession(unsigned concession);
void     SpawnConcession(int tilex, int tiley, unsigned credits, unsigned machinetype);
unsigned LoadConcessionHint(unsigned MsgNum);
void     CacheInfoAreaMsg(unsigned block, unsigned MsgNum, char* hint_buffer, unsigned MaxBufferLen);
void     CheckSpawnEA(void);

int TransformAreas(char tilex, char tiley, char xform);

void CheckSpawnGoldstern(void);
void FindNewGoldieSpawnSite(void);

void  InitMsgCache(mCacheList* mList, unsigned listSize, unsigned infoSize);
void  FreeMsgCache(mCacheList* mList, unsigned listSize);
void  CacheMsg(mCacheInfo* ci, unsigned SegNum, unsigned MsgNum);
short LoadMsg(char* hint_buffer, unsigned SegNum, unsigned MsgNum, unsigned MaxMsgLen);
void  CacheConcessionMsg(void);
bool  ReuseMsg(mCacheInfo* ci, short count, short struct_size);

void DropPlasmaDetonator(void);
void BlockDoorOpen(int door);
void BlastNearDoors(int tilex, int tiley);
void TryBlastDoor(char door);

statobj_t* FindStatic(unsigned tilex, unsigned tiley);
statobj_t* UseReservedStatic(int itemtype, int tilex, int tiley);
void       PlaceReservedItemNearTile(int itemtype, int tilex, int tiley);
void       ExplodeStatics(int tilex, int tiley);

/*
=============================================================================

                         3D_ACT2 DEFINITIONS

=============================================================================
*/

#define s_nakedbody s_static10

#define BARRIER_STATE(obj) ((obj)->ammo)

#define InitSmartAnim(obj, ShapeNum, StartOfs, MaxOfs, AnimType, AnimDir) \
    InitSmartSpeedAnim(obj, ShapeNum, StartOfs, MaxOfs, AnimType, AnimDir, 7)

void MakeFakeStatic(objtype* ob);
void UnmakeFakeStatic(objtype* ob);

extern char     detonators_spawned;
extern int      starthitpoints[][NUMHITENEMIES];
extern unsigned MorphClass[];

extern statetype s_ofs_bounce;

extern statetype s_ofs_esphere_death1;
extern statetype s_ofs_ouch;

extern statetype s_ofs_static;

extern statetype s_rent_die1;
extern statetype s_ofcdie1;
extern statetype s_swatdie1;
extern statetype s_prodie1;
extern statetype s_proshoot1;

extern statetype s_rent_chase1;
extern statetype s_ofcchase1;
extern statetype s_prochase1;
extern statetype s_swatchase1;

extern statetype s_rent_pain;
extern statetype s_ofcpain;
extern statetype s_propain;
extern statetype s_swatpain;

extern statetype s_hold;

extern statetype s_swatwounded1;

extern statetype s_deathcam;

extern statetype s_terrot_wait;
extern statetype s_terrot_seek1;

extern statetype s_ofs_stand;
extern statetype s_ofs_chase1;
extern statetype s_ofs_pain;
extern statetype s_ofs_die1;
extern statetype s_ofs_pod_death1;
extern statetype s_ofs_pod_ouch;
extern statetype s_ofs_attack1;

extern statetype s_electro_appear1;
extern statetype s_electro_chase1;
extern statetype s_electro_ouch;
extern statetype s_electro_shoot1;
extern statetype s_electro_die1;

extern statetype s_liquid_wait;
extern statetype s_liquid_move;
extern statetype s_liquid_rise1;
extern statetype s_liquid_shoot1;
extern statetype s_liquid_ouch;
extern statetype s_liquid_shot;
extern statetype s_liquid_die1;
extern statetype s_liquid_shot_exp1;

extern statetype s_goldstand;
extern statetype s_goldpath1;
extern statetype s_goldpain;
extern statetype s_goldshoot1;
extern statetype s_goldchase1;
extern statetype s_goldwarp_it; // Warp In w/button pressing
extern statetype s_goldwarp_out1;
extern statetype s_goldwarp_in1;

extern statetype s_goldmorph1;
extern statetype s_goldmorph2;
extern statetype s_goldmorph3;
extern statetype s_goldmorph4;
extern statetype s_goldmorph5;
extern statetype s_goldmorph6;
extern statetype s_goldmorph7;
extern statetype s_goldmorph8;

extern statetype s_mgold_chase1;
extern statetype s_mgold_chase2;
extern statetype s_mgold_chase3;
extern statetype s_mgold_chase4;

extern statetype s_mgold_shoot1;
extern statetype s_mgold_shoot2;
extern statetype s_mgold_shoot3;
extern statetype s_mgold_shoot4;

extern statetype s_mgold_pain;

extern statetype s_security_light;

extern statetype s_scout_path1;
extern statetype s_scout_path2;
extern statetype s_scout_path3;
extern statetype s_scout_path4;
extern statetype s_scout_pain;
extern statetype s_scout_run;
extern statetype s_scout_run2;
extern statetype s_scout_run3;
extern statetype s_scout_run4;
extern statetype s_scout_die1;
extern statetype s_scout_stand;
extern statetype s_scout_dead;

extern statetype s_explosion1;

extern statetype s_steamgrate;
extern statetype s_vital;

extern statetype s_vital_ouch;
extern statetype s_vital_die;
extern statetype s_vital_die1;
extern statetype s_vital_die2;
extern statetype s_vital_die3;
extern statetype s_vital_die4;
extern statetype s_vital_die5;
extern statetype s_vital_die6;
extern statetype s_vital_die7;
extern statetype s_vital_die8;

extern statetype s_ooze_chase;
extern statetype s_vpost_barrier;
extern statetype s_spike_barrier;

void T_PainThink(objtype* obj);
void T_ExplodeScout(objtype* obj);

void T_Security(objtype* obj);

void T_ChangeShape(objtype* obj);
void T_MakeOffset(objtype* obj);
void T_LiquidStand(objtype* obj);

void PlaceTowardPlayer(objtype* obj);

void T_Seek(objtype* ob);

void SpawnProjectile(objtype* shooter, classtype);
void SpawnStand(enemy_t which, int tilex, int tiley, int dir);
void SpawnPatrol(enemy_t which, int tilex, int tiley, int dir);
void KillActor(objtype* ob);

void US_ControlPanel(byte);

int      IntSqrt(long va);
unsigned CalcDistance(unsigned x1, unsigned y1, unsigned x2, unsigned y2);

void T_Hit(objtype* ob);
void SpawnOffsetObj(enemy_t which, int tilex, int tiley);

void InitSmartAnimStruct(objtype* obj, unsigned ShapeNum, unsigned char StartOfs, unsigned char MaxOfs, animtype_t AnimType, animdir_t AnimDir);
bool AnimateOfsObj(objtype* obj);
void AdvanceAnimREV(objtype* obj);
void AdvanceAnimFWD(objtype* obj);

void SpawnCusExplosion(fixed x, fixed y, unsigned StartFrame, unsigned NumFrames, unsigned Delay, unsigned Class);
void T_SpawnExplosion(objtype* obj);
void T_ExplodeDamage(objtype* obj);

void ExplodeRadius(objtype* obj, short damage, bool damageplayer);

extern statetype s_barrier_transition;
extern statetype s_barrier_shutdown;

void SpawnBarrier(enemy_t which, int tilex, int tiley, bool OnOff);
void ToggleBarrier(objtype* obj);

void InitAnim(objtype* obj, unsigned ShapeNum, unsigned char StartOfs, unsigned char MaxOfs, animtype_t AnimType, animdir_t AnimDir, unsigned Delay, unsigned WaitDelay);

objtype* FindObj(classtype which, short tilex, short tiley);
objtype* FindHiddenOfs(classtype which);
void     SpawnHiddenOfs(enemy_t which, int tilex, int tiley);
objtype* MoveHiddenOfs(classtype which_class, classtype new_, fixed x, fixed y);

void CheckForSpecialTile(objtype* obj, unsigned tilex, unsigned tiley);
void DropCargo(objtype* obj);

/*
=============================================================================

                         3D_TEXT DEFINITIONS

=============================================================================
*/

extern char helpfilename[], endfilename[];

extern void HelpScreens(void);
extern void EndText(void);

/*
=============================================================================

                         3D_MSGS TEXT DEFINITIONS

=============================================================================
*/

extern char noeat_msg1[];
extern char bevs_msg1[];
extern char food_msg1[];

extern char bonus_msg7[];
extern char bonus_msg26[];

extern char* BonusMsg[];
extern char* ActorInfoMsg[];
extern char  ekg_heartbeat_enabled[];
extern char  ekg_heartbeat_disabled[];
extern char  attacker_info_enabled[];
extern char  attacker_info_disabled[];
extern char  WeaponNotAvailMsg[];
extern char  WeaponAvailMsg[];
extern char  RadarAvailMsg[];
extern char  RadarEnergyGoneMsg[];
extern char  WeaponAutoSelectMsg[];
extern char  EnergyPackDepleted[];
extern char  NotEnoughEnergyForWeapon[];

extern char WeaponMalfunction[];

extern char SwitchNotActivateMsg[];
extern char NoFoodTokens[];
extern char ExtraMan[];
extern char OneMillion[];
extern char TenMillion[];

extern char NoAdLibCard[];
extern char MusicOn[];
extern char MusicOff[];
extern char SoundOn[];
extern char SoundOff[];

extern char pd_dropped[];
extern char pd_nomore[];
extern char pd_switching[];
extern char pd_notnear[];
extern char pd_getcloser[];
extern char pd_floorunlocked[];
extern char pd_donthaveany[];
extern char pd_no_computer[];
extern char pd_floornotlocked[];

/*
=============================================================================

                         3D_INTER DEFINITIONS

=============================================================================
*/

extern char BreifingText[];

void     UpdateScreenPic(void);
void     DisplayPrepingMsg(char* text);
bool     Breifing(breifing_type BreifingType, unsigned episode);
void     ShPrint(char* text, char shadow_color, bool single_char);
unsigned Random(unsigned Max);

//===========================================================================
//
//					3D_MENU DEFINATIONS - WHICH NEED TO BE GLOBAL
//
//===========================================================================

extern bool EscPressed;

void          DrawInstructions(inst_type Type);
void          CacheMessage(unsigned MessageNum);
void          TerminateStr(char* pos);
unsigned long CacheCompData(unsigned ItemNum, memptr* dest_loc);
bool          CheckForSpecialCode(unsigned ItemNum);

//===========================================================================
//
//					3D_FREE DEFINATIONS - WHICH NEED TO BE GLOBAL
//
//===========================================================================

extern char JM_FREE_DATA_END[];
extern char JM_FREE_DATA_START[];
