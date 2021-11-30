// RVCOS.c file COPYRIGHT MITUL GANGER
// PROJECT 3
#include "RVCOS.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
//#include "queue.h"

#define CONTROLLER      (*(volatile uint32_t*)0x40000018) // https://piazza.com/class/ktlxmp9hob46o0?cid=410
#define MODE_CONTROL            (*(volatile uint32_t*)0x500FF214)
typedef struct{ // holds Thread TCB
    int32_t entry; 
    void* param;
    uint32_t memsize;
    uint32_t priority;
    uint32_t id;
    uint32_t thread_state;
    uint8_t *stackbase;
    uint32_t *savedsp;
    uint32_t returnval;
    uint32_t sleepticks;
} TCB;
SColor RVCOPaletteDefaultColors[] = {
{0x00, 0x00, 0x00, 0x00}, // Transparent
{0x00, 0x00, 0x80, 0xFF}, // Maroon (SYSTEM)
{0x00, 0x80, 0x00, 0xFF}, // Green (SYSTEM)
{0x00, 0x80, 0x80, 0xFF}, // Olive (SYSTEM)
{0x80, 0x00, 0x00, 0xFF}, // Navy (SYSTEM)
{0x80, 0x00, 0x80, 0xFF}, // Purple (SYSTEM)
{0x80, 0x80, 0x00, 0xFF}, // Teal (SYSTEM)
{0xC0, 0xC0, 0xC0, 0xFF}, // Silver (SYSTEM)
{0x80, 0x80, 0x80, 0xFF}, // Grey (SYSTEM)
{0x00, 0x00, 0xFF, 0xFF}, // Red (SYSTEM)
{0x00, 0xFF, 0x00, 0xFF}, // Lime (SYSTEM)
{0x00, 0xFF, 0xFF, 0xFF}, // Yellow (SYSTEM)
{0xFF, 0x00, 0x00, 0xFF}, // Blue (SYSTEM)
{0xFF, 0x00, 0xFF, 0xFF}, // Fuchsia (SYSTEM)
{0xFF, 0xFF, 0x00, 0xFF}, // Aqua (SYSTEM)
{0xFF, 0xFF, 0xFF, 0xFF}, // White (SYSTEM)
{0x00, 0x00, 0x00, 0xFF}, // Black (SYSTEM)
{0x5F, 0x00, 0x00, 0xFF}, // NavyBlue
{0x87, 0x00, 0x00, 0xFF}, // DarkBlue
{0xAF, 0x00, 0x00, 0xFF}, // Blue3
{0xD7, 0x00, 0x00, 0xFF}, // Blue3
{0xFF, 0x00, 0x00, 0xFF}, // Blue1
{0x00, 0x5F, 0x00, 0xFF}, // DarkGreen
{0x5F, 0x5F, 0x00, 0xFF}, // DeepSkyBlue4
{0x87, 0x5F, 0x00, 0xFF}, // DeepSkyBlue4
{0xAF, 0x5F, 0x00, 0xFF}, // DeepSkyBlue4
{0xD7, 0x5F, 0x00, 0xFF}, // DodgerBlue3
{0xFF, 0x5F, 0x00, 0xFF}, // DodgerBlue2
{0x00, 0x87, 0x00, 0xFF}, // Green4
{0x5F, 0x87, 0x00, 0xFF}, // SpringGreen4
{0x87, 0x87, 0x00, 0xFF}, // Turquoise4
{0xAF, 0x87, 0x00, 0xFF}, // DeepSkyBlue3
{0xD7, 0x87, 0x00, 0xFF}, // DeepSkyBlue3
{0xFF, 0x87, 0x00, 0xFF}, // DodgerBlue1
{0x00, 0xAF, 0x00, 0xFF}, // Green3
{0x5F, 0xAF, 0x00, 0xFF}, // SpringGreen3
{0x87, 0xAF, 0x00, 0xFF}, // DarkCyan
{0xAF, 0xAF, 0x00, 0xFF}, // LightSeaGreen
{0xD7, 0xAF, 0x00, 0xFF}, // DeepSkyBlue2
{0xFF, 0xAF, 0x00, 0xFF}, // DeepSkyBlue1
{0x00, 0xD7, 0x00, 0xFF}, // Green3
{0x5F, 0xD7, 0x00, 0xFF}, // SpringGreen3
{0x87, 0xD7, 0x00, 0xFF}, // SpringGreen2
{0xAF, 0xD7, 0x00, 0xFF}, // Cyan3
{0xD7, 0xD7, 0x00, 0xFF}, // DarkTurquoise
{0xFF, 0xD7, 0x00, 0xFF}, // Turquoise2
{0x00, 0xFF, 0x00, 0xFF}, // Green1
{0x5F, 0xFF, 0x00, 0xFF}, // SpringGreen2
{0x87, 0xFF, 0x00, 0xFF}, // SpringGreen1
{0xAF, 0xFF, 0x00, 0xFF}, // MediumSpringGreen
{0xD7, 0xFF, 0x00, 0xFF}, // Cyan2
{0xFF, 0xFF, 0x00, 0xFF}, // Cyan1
{0x00, 0x00, 0x5F, 0xFF}, // DarkRed
{0x5F, 0x00, 0x5F, 0xFF}, // DeepPink4
{0x87, 0x00, 0x5F, 0xFF}, // Purple4
{0xAF, 0x00, 0x5F, 0xFF}, // Purple4
{0xD7, 0x00, 0x5F, 0xFF}, // Purple3
{0xFF, 0x00, 0x5F, 0xFF}, // BlueViolet
{0x00, 0x5F, 0x5F, 0xFF}, // Orange4
{0x5F, 0x5F, 0x5F, 0xFF}, // Grey37
{0x87, 0x5F, 0x5F, 0xFF}, // MediumPurple4
{0xAF, 0x5F, 0x5F, 0xFF}, // SlateBlue3
{0xD7, 0x5F, 0x5F, 0xFF}, // SlateBlue3
{0xFF, 0x5F, 0x5F, 0xFF}, // RoyalBlue1
{0x00, 0x87, 0x5F, 0xFF}, // Chartreuse4
{0x5F, 0x87, 0x5F, 0xFF}, // DarkSeaGreen4
{0x87, 0x87, 0x5F, 0xFF}, // PaleTurquoise4
{0xAF, 0x87, 0x5F, 0xFF}, // SteelBlue
{0xD7, 0x87, 0x5F, 0xFF}, // SteelBlue3
{0xFF, 0x87, 0x5F, 0xFF}, // CornflowerBlue
{0x00, 0xAF, 0x5F, 0xFF}, // Chartreuse3
{0x5F, 0xAF, 0x5F, 0xFF}, // DarkSeaGreen4
{0x87, 0xAF, 0x5F, 0xFF}, // CadetBlue
{0xAF, 0xAF, 0x5F, 0xFF}, // CadetBlue
{0xD7, 0xAF, 0x5F, 0xFF}, // SkyBlue3
{0xFF, 0xAF, 0x5F, 0xFF}, // SteelBlue1
{0x00, 0xD7, 0x5F, 0xFF}, // Chartreuse3
{0x5F, 0xD7, 0x5F, 0xFF}, // PaleGreen3
{0x87, 0xD7, 0x5F, 0xFF}, // SeaGreen3
{0xAF, 0xD7, 0x5F, 0xFF}, // Aquamarine3
{0xD7, 0xD7, 0x5F, 0xFF}, // MediumTurquoise
{0xFF, 0xD7, 0x5F, 0xFF}, // SteelBlue1
{0x00, 0xFF, 0x5F, 0xFF}, // Chartreuse2
{0x5F, 0xFF, 0x5F, 0xFF}, // SeaGreen2
{0x87, 0xFF, 0x5F, 0xFF}, // SeaGreen1
{0xAF, 0xFF, 0x5F, 0xFF}, // SeaGreen1
{0xD7, 0xFF, 0x5F, 0xFF}, // Aquamarine1
{0xFF, 0xFF, 0x5F, 0xFF}, // DarkSlateGray2
{0x00, 0x00, 0x87, 0xFF}, // DarkRed
{0x5F, 0x00, 0x87, 0xFF}, // DeepPink4
{0x87, 0x00, 0x87, 0xFF}, // DarkMagenta
{0xAF, 0x00, 0x87, 0xFF}, // DarkMagenta
{0xD7, 0x00, 0x87, 0xFF}, // DarkViolet
{0xFF, 0x00, 0x87, 0xFF}, // Purple
{0x00, 0x5F, 0x87, 0xFF}, // Orange4
{0x5F, 0x5F, 0x87, 0xFF}, // LightPink4
{0x87, 0x5F, 0x87, 0xFF}, // Plum4
{0xAF, 0x5F, 0x87, 0xFF}, // MediumPurple3
{0xD7, 0x5F, 0x87, 0xFF}, // MediumPurple3
{0xFF, 0x5F, 0x87, 0xFF}, // SlateBlue1
{0x00, 0x87, 0x87, 0xFF}, // Yellow4
{0x5F, 0x87, 0x87, 0xFF}, // Wheat4
{0x87, 0x87, 0x87, 0xFF}, // Grey53
{0xAF, 0x87, 0x87, 0xFF}, // LightSlateGrey
{0xD7, 0x87, 0x87, 0xFF}, // MediumPurple
{0xFF, 0x87, 0x87, 0xFF}, // LightSlateBlue
{0x00, 0xAF, 0x87, 0xFF}, // Yellow4
{0x5F, 0xAF, 0x87, 0xFF}, // DarkOliveGreen3
{0x87, 0xAF, 0x87, 0xFF}, // DarkSeaGreen
{0xAF, 0xAF, 0x87, 0xFF}, // LightSkyBlue3
{0xD7, 0xAF, 0x87, 0xFF}, // LightSkyBlue3
{0xFF, 0xAF, 0x87, 0xFF}, // SkyBlue2
{0x00, 0xD7, 0x87, 0xFF}, // Chartreuse2
{0x5F, 0xD7, 0x87, 0xFF}, // DarkOliveGreen3
{0x87, 0xD7, 0x87, 0xFF}, // PaleGreen3
{0xAF, 0xD7, 0x87, 0xFF}, // DarkSeaGreen3
{0xD7, 0xD7, 0x87, 0xFF}, // DarkSlateGray3
{0xFF, 0xD7, 0x87, 0xFF}, // SkyBlue1
{0x00, 0xFF, 0x87, 0xFF}, // Chartreuse1
{0x5F, 0xFF, 0x87, 0xFF}, // LightGreen
{0x87, 0xFF, 0x87, 0xFF}, // LightGreen
{0xAF, 0xFF, 0x87, 0xFF}, // PaleGreen1
{0xD7, 0xFF, 0x87, 0xFF}, // Aquamarine1
{0xFF, 0xFF, 0x87, 0xFF}, // DarkSlateGray1
{0x00, 0x00, 0xAF, 0xFF}, // Red3
{0x5F, 0x00, 0xAF, 0xFF}, // DeepPink4
{0x87, 0x00, 0xAF, 0xFF}, // MediumVioletRed
{0xAF, 0x00, 0xAF, 0xFF}, // Magenta3
{0xD7, 0x00, 0xAF, 0xFF}, // DarkViolet
{0xFF, 0x00, 0xAF, 0xFF}, // Purple
{0x00, 0x5F, 0xAF, 0xFF}, // DarkOrange3
{0x5F, 0x5F, 0xAF, 0xFF}, // IndianRed
{0x87, 0x5F, 0xAF, 0xFF}, // HotPink3
{0xAF, 0x5F, 0xAF, 0xFF}, // MediumOrchid3
{0xD7, 0x5F, 0xAF, 0xFF}, // MediumOrchid
{0xFF, 0x5F, 0xAF, 0xFF}, // MediumPurple2
{0x00, 0x87, 0xAF, 0xFF}, // DarkGoldenrod
{0x5F, 0x87, 0xAF, 0xFF}, // LightSalmon3
{0x87, 0x87, 0xAF, 0xFF}, // RosyBrown
{0xAF, 0x87, 0xAF, 0xFF}, // Grey63
{0xD7, 0x87, 0xAF, 0xFF}, // MediumPurple2
{0xFF, 0x87, 0xAF, 0xFF}, // MediumPurple1
{0x00, 0xAF, 0xAF, 0xFF}, // Gold3
{0x5F, 0xAF, 0xAF, 0xFF}, // DarkKhaki
{0x87, 0xAF, 0xAF, 0xFF}, // NavajoWhite3
{0xAF, 0xAF, 0xAF, 0xFF}, // Grey69
{0xD7, 0xAF, 0xAF, 0xFF}, // LightSteelBlue3
{0xFF, 0xAF, 0xAF, 0xFF}, // LightSteelBlue
{0x00, 0xD7, 0xAF, 0xFF}, // Yellow3
{0x5F, 0xD7, 0xAF, 0xFF}, // DarkOliveGreen3
{0x87, 0xD7, 0xAF, 0xFF}, // DarkSeaGreen3
{0xAF, 0xD7, 0xAF, 0xFF}, // DarkSeaGreen2
{0xD7, 0xD7, 0xAF, 0xFF}, // LightCyan3
{0xFF, 0xD7, 0xAF, 0xFF}, // LightSkyBlue1
{0x00, 0xFF, 0xAF, 0xFF}, // GreenYellow
{0x5F, 0xFF, 0xAF, 0xFF}, // DarkOliveGreen2
{0x87, 0xFF, 0xAF, 0xFF}, // PaleGreen1
{0xAF, 0xFF, 0xAF, 0xFF}, // DarkSeaGreen2
{0xD7, 0xFF, 0xAF, 0xFF}, // DarkSeaGreen1
{0xFF, 0xFF, 0xAF, 0xFF}, // PaleTurquoise1
{0x00, 0x00, 0xD7, 0xFF}, // Red3
{0x5F, 0x00, 0xD7, 0xFF}, // DeepPink3
{0x87, 0x00, 0xD7, 0xFF}, // DeepPink3
{0xAF, 0x00, 0xD7, 0xFF}, // Magenta3
{0xD7, 0x00, 0xD7, 0xFF}, // Magenta3
{0xFF, 0x00, 0xD7, 0xFF}, // Magenta2
{0x00, 0x5F, 0xD7, 0xFF}, // DarkOrange3
{0x5F, 0x5F, 0xD7, 0xFF}, // IndianRed
{0x87, 0x5F, 0xD7, 0xFF}, // HotPink3
{0xAF, 0x5F, 0xD7, 0xFF}, // HotPink2
{0xD7, 0x5F, 0xD7, 0xFF}, // Orchid
{0xFF, 0x5F, 0xD7, 0xFF}, // MediumOrchid1
{0x00, 0x87, 0xD7, 0xFF}, // Orange3
{0x5F, 0x87, 0xD7, 0xFF}, // LightSalmon3
{0x87, 0x87, 0xD7, 0xFF}, // LightPink3
{0xAF, 0x87, 0xD7, 0xFF}, // Pink3
{0xD7, 0x87, 0xD7, 0xFF}, // Plum3
{0xFF, 0x87, 0xD7, 0xFF}, // Violet
{0x00, 0xAF, 0xD7, 0xFF}, // Gold3
{0x5F, 0xAF, 0xD7, 0xFF}, // LightGoldenrod3
{0x87, 0xAF, 0xD7, 0xFF}, // Tan
{0xAF, 0xAF, 0xD7, 0xFF}, // MistyRose3
{0xD7, 0xAF, 0xD7, 0xFF}, // Thistle3
{0xFF, 0xAF, 0xD7, 0xFF}, // Plum2
{0x00, 0xD7, 0xD7, 0xFF}, // Yellow3
{0x5F, 0xD7, 0xD7, 0xFF}, // Khaki3
{0x87, 0xD7, 0xD7, 0xFF}, // LightGoldenrod2
{0xAF, 0xD7, 0xD7, 0xFF}, // LightYellow3
{0xD7, 0xD7, 0xD7, 0xFF}, // Grey84
{0xFF, 0xD7, 0xD7, 0xFF}, // LightSteelBlue1
{0x00, 0xFF, 0xD7, 0xFF}, // Yellow2
{0x5F, 0xFF, 0xD7, 0xFF}, // DarkOliveGreen1
{0x87, 0xFF, 0xD7, 0xFF}, // DarkOliveGreen1
{0xAF, 0xFF, 0xD7, 0xFF}, // DarkSeaGreen1
{0xD7, 0xFF, 0xD7, 0xFF}, // Honeydew2
{0xFF, 0xFF, 0xD7, 0xFF}, // LightCyan1
{0x00, 0x00, 0xFF, 0xFF}, // Red1
{0x5F, 0x00, 0xFF, 0xFF}, // DeepPink2
{0x87, 0x00, 0xFF, 0xFF}, // DeepPink1
{0xAF, 0x00, 0xFF, 0xFF}, // DeepPink1
{0xD7, 0x00, 0xFF, 0xFF}, // Magenta2
{0xFF, 0x00, 0xFF, 0xFF}, // Magenta1
{0x00, 0x5F, 0xFF, 0xFF}, // OrangeRed1
{0x5F, 0x5F, 0xFF, 0xFF}, // IndianRed1
{0x87, 0x5F, 0xFF, 0xFF}, // IndianRed1
{0xAF, 0x5F, 0xFF, 0xFF}, // HotPink
{0xD7, 0x5F, 0xFF, 0xFF}, // HotPink
{0xFF, 0x5F, 0xFF, 0xFF}, // MediumOrchid1
{0x00, 0x87, 0xFF, 0xFF}, // DarkOrange
{0x5F, 0x87, 0xFF, 0xFF}, // Salmon1
{0x87, 0x87, 0xFF, 0xFF}, // LightCoral
{0xAF, 0x87, 0xFF, 0xFF}, // PaleVioletRed1
{0xD7, 0x87, 0xFF, 0xFF}, // Orchid2
{0xFF, 0x87, 0xFF, 0xFF}, // Orchid1
{0x00, 0xAF, 0xFF, 0xFF}, // Orange1
{0x5F, 0xAF, 0xFF, 0xFF}, // SandyBrown
{0x87, 0xAF, 0xFF, 0xFF}, // LightSalmon1
{0xAF, 0xAF, 0xFF, 0xFF}, // LightPink1
{0xD7, 0xAF, 0xFF, 0xFF}, // Pink1
{0xFF, 0xAF, 0xFF, 0xFF}, // Plum1
{0x00, 0xD7, 0xFF, 0xFF}, // Gold1
{0x5F, 0xD7, 0xFF, 0xFF}, // LightGoldenrod2
{0x87, 0xD7, 0xFF, 0xFF}, // LightGoldenrod2
{0xAF, 0xD7, 0xFF, 0xFF}, // NavajoWhite1
{0xD7, 0xD7, 0xFF, 0xFF}, // MistyRose1
{0xFF, 0xD7, 0xFF, 0xFF}, // Thistle1
{0x00, 0xFF, 0xFF, 0xFF}, // Yellow1
{0x5F, 0xFF, 0xFF, 0xFF}, // LightGoldenrod1
{0x87, 0xFF, 0xFF, 0xFF}, // Khaki1
{0xAF, 0xFF, 0xFF, 0xFF}, // Wheat1
{0xD7, 0xFF, 0xFF, 0xFF}, // Cornsilk1
{0xFF, 0xFF, 0xFF, 0xFF}, // Grey100
{0x08, 0x08, 0x08, 0xFF}, // Grey3
{0x12, 0x12, 0x12, 0xFF}, // Grey7
{0x1C, 0x1C, 0x1C, 0xFF}, // Grey11
{0x26, 0x26, 0x26, 0xFF}, // Grey15
{0x30, 0x30, 0x30, 0xFF}, // Grey19
{0x3A, 0x3A, 0x3A, 0xFF}, // Grey23
{0x44, 0x44, 0x44, 0xFF}, // Grey27
{0x4E, 0x4E, 0x4E, 0xFF}, // Grey30
{0x58, 0x58, 0x58, 0xFF}, // Grey35
{0x62, 0x62, 0x62, 0xFF}, // Grey39
{0x6C, 0x6C, 0x6C, 0xFF}, // Grey42
{0x76, 0x76, 0x76, 0xFF}, // Grey46
{0x80, 0x80, 0x80, 0xFF}, // Grey50
{0x8A, 0x8A, 0x8A, 0xFF}, // Grey54
{0x94, 0x94, 0x94, 0xFF}, // Grey58
{0x9E, 0x9E, 0x9E, 0xFF}, // Grey62
{0xA8, 0xA8, 0xA8, 0xFF}, // Grey66
{0xB2, 0xB2, 0xB2, 0xFF}, // Grey70
{0xBC, 0xBC, 0xBC, 0xFF}, // Grey74
{0xC6, 0xC6, 0xC6, 0xFF}, // Grey78
{0xD0, 0xD0, 0xD0, 0xFF}, // Grey82
{0xDA, 0xDA, 0xDA, 0xFF}, // Grey85
{0xE4, 0xE4, 0xE4, 0xFF}, // Grey89
{0xEE, 0xEE, 0xEE, 0xFF}  // Grey93
};
void ContextSwitch(volatile uint32_t **oldctx, volatile uint32_t *newctx);      // from office hours, garett
void set_tp(TCB* tp);
TCB* get_tp();
uint32_t call_the_gp(void *param, TThreadEntry entry, uint32_t *gp);
uint32_t *init_stack(uint32_t *sp, TThreadEntry fun, uint32_t param, uint32_t tp);

typedef struct {
    TGraphicID id;
    TGraphicType type;
    TPaletteIndex buffer[512];
    uint32_t status;
    TPaletteID palette;
} OffScreenBufferControl;

//struct for the palette Controls
typedef struct {
    TGraphicID id;
    SColor* palette; 
} PaletteControl;

typedef struct {
    // memory location of graphic data  <- pointer
    // memory location of graphic controller <- pointer
    TGraphicType type;
    TGraphicID id;
    uint32_t* graphicData;
    uint32_t* graphicController;
    uint32_t status;
    TPaletteID palette;
    int32_t DXPosition;
    int32_t DYPosition;
    uint32_t DZPosition;
    uint32_t DWidth;
    uint32_t DHeight;
    uint32_t inMemPos;
    TPaletteIndex* buffer;
} OnScreenBuffer;

OnScreenBuffer* BackgroundGraphics[5], *LargeGraphics[64], *SmallGraphics[128];
OffScreenBufferControl** graphicsBuffer;

PaletteControl** palettes;
volatile int buffpos = 0, bgpos = 0, lgpos = 0, sgpos = 0, palettepos;  // buffpos is for offscreenbuffercontrol[]
OnScreenBuffer* FindOnScreen(TGraphicID gid);
//structs for the diffrent types of graphic controls
typedef struct { 
    //TGraphicID ID;
    uint32_t DPalette : 2;
    uint32_t DXOffset : 10;
    uint32_t DYOffset : 9;
    uint32_t DWidth : 5;
    uint32_t DHeight : 5;
    uint32_t DReserved : 1;
} SLargeSpriteControl, *SLargeSpriteControlRef;

typedef struct {
    //TGraphicID ID;
    uint32_t DPalette : 2;
    uint32_t DXOffset : 10;
    uint32_t DYOffset : 9;
    uint32_t DWidth : 4;
    uint32_t DHeight : 4;
    uint32_t DZ : 3;
} SSmallSpriteControl, * SSmallSpriteControlRef;
typedef struct {
    //TGraphicID ID;
    uint32_t DPalette : 2;
    uint32_t DXOffset : 10;
    uint32_t DYOffset : 10;
    uint32_t DZ : 3;
    uint32_t DReserved : 7;
} SBackgroundControl, *SBackgroundControlRef;

typedef struct {
    uint32_t DMode : 1;
    uint32_t DRefresh : 7;
    uint32_t DReserved : 24;
} SVideoControllerMode, *SVideoControllerModeRef;

//graphics data
volatile uint8_t *BackgroundData[5];
volatile uint8_t *LargeSpriteData[64];
volatile uint8_t *SmallSpriteData[128];

volatile SColor *BackgroundPalettes[10];
volatile SColor *SpritePalettes[10];

//Positions of each of the spritedata
//create off screen buffer and respective structs with arrays
//for the time being use a button to switch to graphics mode 


//Pointers for the video controllers of the graphics
volatile SBackgroundControl *BackgroundControls = (volatile SBackgroundControl *)0x500FF100;
volatile SLargeSpriteControl *LargeSpriteControls = (volatile SLargeSpriteControl *)0x500FF114;
volatile SSmallSpriteControl *SmallSpriteControls = (volatile SSmallSpriteControl *)0x500FF214;
volatile SVideoControllerMode *ModeControl = (volatile SVideoControllerMode *)0x500FF414;
//struct for the offscreen buffer

void InitPointers(void);

//Function which fills in the array data for each of the different sprites and backgrounds

void InitPointers(void){
    for(int Index = 0; Index < 4; Index++){
        BackgroundPalettes[Index] = (volatile SColor *)(0x500FC000 + 256 * sizeof(SColor) * Index);
        SpritePalettes[Index] = (volatile SColor *)(0x500FD000 + 256 * sizeof(SColor) * Index);
    }
    for(int Index = 0; Index < 5; Index++){
        BackgroundData[Index] = (volatile uint8_t *)(0x50000000 + 512 * 288 * Index);
    }
    for(int Index = 0; Index < 64; Index++){
        LargeSpriteData[Index] = (volatile uint8_t *)(0x500B4000 + 64 * 64 * Index);
    }
    for(int Index = 0; Index < 128; Index++){
        SmallSpriteData[Index] = (volatile uint8_t *)(0x500F4000 + 16 * 16 * Index);
         
    }
    /*
    RVCMemoryAllocate(300*sizeof(OffScreenBufferControl*), &graphicsBuffer);
    RVCMemoryAllocate(256*sizeof(PaletteControl*), &palettes);
    */
}
__attribute__((always_inline)) inline uint32_t csr_mstatus_read(void){
    uint32_t result;
    asm volatile ("csrr %0, mstatus" : "=r"(result));
    return result;
}

__attribute__((always_inline)) inline void csr_mstatus_write(uint32_t val){
    asm volatile ("csrw mstatus, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_write_mie(uint32_t val){
    asm volatile ("csrw mie, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_enable_interrupts(void){
    asm volatile ("csrsi mstatus, 0x8");
}

__attribute__((always_inline)) inline void csr_disable_interrupts(void){
    asm volatile ("csrci mstatus, 0x8");
}

// ======================================================================================================================================================================================================

// https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm
struct node {   // https://stackoverflow.com/questions/35579713/declare-structure-in-header-file
    TCB *thread;     // holds a tcb thread
     // https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm
    struct node *next;  // the next node from the current node
    struct node *prev;  // the previous node from the current one
};
typedef struct node node;


struct queue {  // https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/
    int length;
    node *head; // the front of the queue
    node *tail; // the back of the queue
};
typedef struct queue queue;


bool isEmpty(queue *q) {  // https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm
    return (q->length == 0);
}


void insertHead(queue *q, TCB *thread) {   // https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm

    node *n;
    RVCMemoryAllocate(sizeof(node), &n);
    n->thread = thread;   

    if(!isEmpty(q)) {
        n->next = q->head;
        n->prev = NULL;
        q->head = n;
    } 
    else {
        q->head = n;
        q->tail = n;
        q->tail->prev = NULL, q->head->prev= NULL, q->tail->next = NULL, q->head->next= NULL;
    }
    q->length ++;
}

void insertTail(queue *q, TCB * thread) { // insert node to the tail of the function
    node *n;            // https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c
    RVCMemoryAllocate(sizeof(node), &n);
    n->thread = thread;   


   // RVCWriteText("got into insertTail function\n", 30);

    if(!isEmpty(q)) {
       // RVCWriteText("got into insertTail function if statement\n", 43);
        n->prev = q->tail;
        n->next = NULL;
        q->tail = n;
    } 
    else {
        //RVCWriteText("got into insertTail function else statement\n", 45);
        q->head = n;
        q->tail = n;
        q->tail->prev = NULL, q->head->prev= NULL, q->tail->next = NULL, q->head->next= NULL;
    }
    q->length ++;

}



// erase the thread in the queue with the matching ID
// https://prepinsta.com/c-program/deletion-in-linked-list/

void eraseElement(queue *q, TThreadID threadid) {
    node *n;
    node *element = q->head;    // start iterations at head, our traversal node

    if(q->length == 0) return;

    // 1 element in queue case
    if (q->length == 1 && q->head->thread->id == threadid) {    // https://prepinsta.com/c-program/deletion-in-linked-list/
        RVCMemoryDeallocate(element);
        q->head = NULL;
        q->length--;    // size back to 0
        return;
    }
    // head is the one that needs to go then --> 
    if (element!=NULL && element->thread->id == threadid) {  // https://prepinsta.com/c-program/deletion-in-linked-list/
        q->head = element->next;
        q->head->prev = NULL; // set the new head's previous element to NULL
        RVCMemoryDeallocate(element);
        q->length--;
        return;
    }

    while(element != NULL && element->thread->id != threadid) {
        n = element;        // keep track of element that would have been the right value to delete
        element = element->next;
    }

    if (element == NULL) return; // did not find the element thread id

    // when we get here, we know the correct thread to delete is element so we set the nexts and prevs to the correct values
    if (element == q->tail) q->tail = n;    // set tail
    n->next = element->next;

    if (n->next != NULL) n->next->prev = n; 
    RVCMemoryDeallocate(element);  // https://prepinsta.com/c-program/deletion-in-linked-list/
    q->length--;
}

TCB popFront(queue *q) {   // https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm
   // https://www.codesdope.com/blog/article/making-a-aqueue-using-linked-list-in-c/
    node *n;
    TCB temp = *(q->head->thread); // holds the thread TCB
    n = q->head;
    q->head = q->head->next;
    q->length--;
    RVCMemoryDeallocate(n);
    return(temp);
}

TCB popBack(queue *q) {    // https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm
    // https://www.codesdope.com/blog/article/making-a-aqueue-using-linked-list-in-c/
    node* temp = q->tail;
    eraseElement(q, temp->thread->thread_state);
    return *(temp->thread);
}

//   =================================================================================================================================================================================

                    // ---------------------------------------------------------MUTEX
typedef struct {
     TMutexID id;
     TThreadID threadid;
     queue* waitingToAcquire;
     bool locked;
} Mutex;    // id, threadid, waitingtoacquire, locked?

typedef struct {
    void* base;
    TMemoryPoolID ID;
    TMemorySize size;
    TMemorySize bytesleft;
} MemoryPool;

// extern uint32_t __global_pointer$;   // points to globabl pointer in assembly
volatile uint32_t* current_gp;
extern volatile int global;
volatile int tickcount = 0, counter = 2;
volatile int pos = 0, mutexpos = 0, poolpos = 0;    // position of last element in the TCB threads[256] array
volatile int cursorPos = 0; // position of cursor in video memory1
volatile TCB** threads;   // store all threads in order of creation
volatile Mutex* locks[256];   // stores all our locks in order of creation
volatile MemoryPool* pools[256];

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);  // https://github.com/UCDClassNitta/riscv-console/blob/main/riscv-example/src/main.c
 
// volatile uint8_t idle_stack_base;   // idle threads stack base location
queue *hQueue, *nQueue, *lQueue, *sleepQueue, *idleQueue;

 
void initQueue(queue *q) { // https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/
    q->length = 0;
    if(q->length != 0) RVCWriteText("initqueue is the problem\n", 26);
    q-> head = NULL;
    q-> tail = NULL;
}
// _heapbase = MAX(_edata,MAX(_esdata,_ebss));

void initializeQueues() {   // 
    RVCMemoryAllocate(sizeof(queue), &hQueue);
    RVCMemoryAllocate(sizeof(queue), &nQueue);
    RVCMemoryAllocate(sizeof(queue), &lQueue);
    RVCMemoryAllocate(sizeof(queue), &sleepQueue);
    RVCMemoryAllocate(sizeof(queue), &idleQueue);
    initQueue(idleQueue);
    initQueue(sleepQueue);
    initQueue(hQueue);
    initQueue(nQueue);
    initQueue(lQueue);
}

// THIS FUNCTION IS ONLY CALLED WHEN A TIMER INTERRUPT HAPPENS
void decrementSleepTicks(queue* sq) {   // function that loops through sleeping queue 
    node* n = sq->head;
    for (int i = 0; i < sq->length; i++) {
        if(n == NULL) return;   // incase the bounds mess up
        n->thread->sleepticks-=1;   // subtract ticks to wait for from the sleep queue element
        if(n->thread->sleepticks <= 0) {
            n->thread->thread_state = RVCOS_THREAD_STATE_READY; //set sleeping thread state to ready
            eraseElement(sq, n->thread->id);                    // remove element from sleep queue
        }
        n = n->next;    // keep looping through sq
    }
}


//Helper Function from 10/20/21 Discussion
void skeleton(TThreadID thread_id){
    //RVCWriteText("got to skeleton\n", 16);
    int32_t entry;
    void *param;
    for(int i = 0; i <= 256; i++){
        if(threads[i]->id == thread_id){
            entry = threads[i]->entry;
            param = threads[i]->param;
        }
    }
    enableInterrupts();
    uint32_t rv = call_the_gp(param, entry, current_gp);
    //uint32_t rv = 1;
    disableInterrupts();
    RVCThreadTerminate(thread_id, rv);
}

// figure out vt code, figure out scheduling bs with the mutex cartridge, double check if memory pool even works
void schedule() {
    TCB *previouslyRunning;     // currently running thread
    for(int i = 0; i < pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->thread_state == RVCOS_THREAD_STATE_RUNNING) {
            //RVCWriteText("\nfound running thread in schedule function\n", 44);
            previouslyRunning = threads[i];
            break;
        }
    }
    //TCB *threadptr = &t;    // the pointer to the current thread we found that is running
    

    if(!isEmpty(hQueue)) {      // check if the high priortiy queue has threads in it
        //RVCWriteText("hqueue is not empty you hoe\n", 29);
        node *nextInQueue = hQueue->head;
        //if (previouslyRunning->id == nextInQueue->thread->id && hQueue->length == 1) return;
        while(nextInQueue != NULL) {
            //RVCWriteText("got into while\n", 16);
            if (nextInQueue->thread->thread_state == RVCOS_THREAD_STATE_READY && previouslyRunning->id != nextInQueue->thread->id) { // if we find next ready thread and it is not our running thread
                previouslyRunning->thread_state = RVCOS_THREAD_STATE_READY, nextInQueue->thread->thread_state = RVCOS_THREAD_STATE_RUNNING;  // change the states of the current running and ready thread
                
                if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_HIGH) { // if the thread is high priority we switch it back
                    //RVCWriteText("previous running thread is high priority my guy\n", 49);
                    eraseElement(hQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(hQueue, previouslyRunning);
                    
                }
                else if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_NORMAL) { // if the thread is high priority we switch it back
                    //RVCWriteText("previous running thread is normal priority my guy\n", 51);
                    eraseElement(nQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(nQueue, previouslyRunning);
                    
                }
                else if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_LOW) { // if the thread is high priority we switch it back
                    //RVCWriteText("previous running thread is low priority my guy\n", 48);
                    eraseElement(lQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(lQueue, previouslyRunning);
                    
                }

                //RVCWriteText("got to the part where I should be context switching\n", 53);
                //set_tp(nextInQueue);
                RVCTickCount(&tickcount);
                decrementSleepTicks(sleepQueue);
                ContextSwitch(&(previouslyRunning->savedsp), nextInQueue->thread->savedsp);
                //RVCWriteText("context switched\n", 18);
            }

            nextInQueue = nextInQueue->next;
        }
    }
    if(!isEmpty(nQueue)) {      // check if the high priortiy queue has threads in it
        //RVCWriteText("nqueue is not empty you hoeeee\n", 29);
        node *nextInQueue = nQueue->head;
        //if (previouslyRunning->id == nextInQueue->thread->id && nQueue->length == 1) return;
        while(nextInQueue != NULL) {
           // RVCWriteText("got into while i nkedium\n", 16);
            if (nextInQueue->thread->thread_state == RVCOS_THREAD_STATE_READY && previouslyRunning->id != nextInQueue->thread->id) { // if we find next ready thread and it is not our running thread
                //RVCWriteText("got past if statement les go\n", 30);
                previouslyRunning->thread_state = RVCOS_THREAD_STATE_READY, nextInQueue->thread->thread_state = RVCOS_THREAD_STATE_RUNNING;  // change the states of the current running and ready thread
                
                if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_HIGH) { // if the thread is high priority we switch it back
                    //RVCWriteText("previous running thread is high priority my guy\n", 49);
                    eraseElement(hQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(hQueue, previouslyRunning);
                    
                }
                else if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_NORMAL) { // if the thread is high priority we switch it back
                    //RVCWriteText("previous running thread is normal priority my guy\n", 51);
                    eraseElement(nQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(nQueue, previouslyRunning);
                    
                }
                else if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_LOW) { // if the thread is high priority we switch it back
                    //RVCWriteText("previous running thread is low priority my guy\n", 48);
                    eraseElement(lQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(lQueue, previouslyRunning);
                    
                }

                //RVCWriteText("got to the part where I should be context switching\n", 53);
                RVCTickCount(&tickcount);
                decrementSleepTicks(sleepQueue);
                ContextSwitch(&(previouslyRunning->savedsp), nextInQueue->thread->savedsp);
                //RVCWriteText("context switched\n", 18);
            }

            nextInQueue = nextInQueue->next;
        }
    }
    if(!isEmpty(lQueue)) {      // check if the high priortiy queue has threads in it
        //RVCWriteText("lqueue is not empty you hoe\n", 29);
        node *nextInQueue = lQueue->head;
        //if (previouslyRunning->id == nextInQueue->thread->id && lQueue->length == 1) return;
        while(nextInQueue != NULL) {
            //RVCWriteText("got into while\n", 16);
            if (nextInQueue->thread->thread_state == RVCOS_THREAD_STATE_READY && previouslyRunning->id != nextInQueue->thread->id) { // if we find next ready thread and it is not our running thread
                //RVCWriteText("got past if statement les go\n", 30);
                previouslyRunning->thread_state = RVCOS_THREAD_STATE_READY, nextInQueue->thread->thread_state = RVCOS_THREAD_STATE_RUNNING;  // change the states of the current running and ready thread
                //RVCWriteText("got into lqueue scheduling part lets go my boi\n", 48);
                if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_HIGH) { // if the thread is high priority we switch it back
                    //RVCWriteText("previous running thread is high priority my guy\n", 49);
                    eraseElement(hQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(hQueue, previouslyRunning);
                    
                }
                else if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_NORMAL) { // if the thread is high priority we switch it back
                   // RVCWriteText("previous running thread is normal priority my guy\n", 51);
                    eraseElement(nQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(nQueue, previouslyRunning);
                    
                }
                else if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_LOW) { // if the thread is high priority we switch it back
                    //RVCWriteText("previous running thread is low priority my guy\n", 48);
                    eraseElement(lQueue, previouslyRunning->id);   // erase the previous running thread from hqueue
                    insertTail(lQueue, previouslyRunning);
                    
                }

                //RVCWriteText("got to the part where I should be context switching\n", 53);
                RVCTickCount(&tickcount);
                decrementSleepTicks(sleepQueue);
                ContextSwitch(&(previouslyRunning->savedsp), nextInQueue->thread->savedsp);
                //RVCWriteText("context switched\n", 18);
            }

            nextInQueue = nextInQueue->next;
        }
    }

}


void enableInterrupts() {
    csr_enable_interrupts();
}
void disableInterrupts() {
    csr_disable_interrupts();
}

//idle thread init, innit
void idle(){
    enableInterrupts();
    while(1);
}


//helper function for initilzing the stack from 10/22/21 Discussion
// COPYRIGHT CHRISTOPHER JOHNSON NITTA, UC DAVIS, 2021, ECS150
uint32_t *init_stack(uint32_t *sp, TThreadEntry fun, TThreadID thread_id, uint32_t tp){  // https://piazza.com/class/ktlxmp9hob46o0?cid=426    
    sp--; 
    *sp = (uint32_t)fun; //sw      ra,48(sp)
    sp--;
    *sp = tp;//sw      tp,44(sp)
    sp--;
    *sp = 0;//sw      t0,40(sp)
    sp--;
    *sp = 0;//sw      t1,36(sp)
    sp--;
    *sp = 0;//sw      t2,32(sp)
    sp--;
    *sp = 0;//sw      s0,28(sp)
    sp--;
    *sp = 0;//sw      s1,24(sp)
    sp--;
    *sp = thread_id;//sw      a0,20(sp)
    sp--;
    *sp = 0;//sw      a1,16(sp)
    sp--;
    *sp = 0;//sw      a2,12(sp)
    sp--;
    *sp = 0;//sw      a3,8(sp)
    sp--;
    *sp = 0;//sw      a4,4(sp)
    sp--;
    *sp = 0;//sw      a5,0(sp)
    return sp;
}

TStatus RVCMemoryPoolAllocate(TMemoryPoolID memory, TMemorySize size, void **pointer) {
    MemoryPool* p;
    for(int i = 0; i < poolpos; i++){
        if(pools[i]->ID == memory){
            p = pools[i];
        }
    }
    *pointer = malloc(size);
    p->base = *pointer;
    return RVCOS_STATUS_SUCCESS;
}

// https://piazza.com/class/ktlxmp9hob46o0?cid=352
TStatus RVCInitialize(uint32_t *gp) { // https://piazza.com/class/ktlxmp9hob46o0?cid=266
    RVCMemoryAllocate(sizeof(TCB*)*64, &threads);   // dynamically allocated
    if (threads[1]->memsize == 2048) return RVCOS_STATUS_ERROR_INVALID_STATE;        // check if this function has been called before
    initializeQueues();     // initialize queues high normal low queueus
    //RVCWriteText("Initialized queues\n", 20);
    pos = 0, tickcount = 0;
    current_gp = gp;  // set global variable

    TCB* Main;
    RVCMemoryAllocate(sizeof(TCB), &Main);
    TCB *Idle;
    RVCMemoryAllocate(sizeof(TCB), &Idle);

    Main->priority = RVCOS_THREAD_PRIORITY_NORMAL, Main->thread_state = RVCOS_THREAD_STATE_RUNNING;
    set_tp(Main);
    threads[pos++] = Main;
    insertTail(nQueue, Main);

    //if(nQueue->tail->thread->thread_state==RVCOS_THREAD_STATE_RUNNING)RVCWriteText("main is indeed running on nqueue\n", 34);

    uint8_t *Idle_stackbase;
    RVCMemoryAllocate(2048, &(Idle_stackbase));
    uint32_t k = 0; // for type matching

    Idle->savedsp = init_stack((uint32_t*)(Idle_stackbase + 2048), (TThreadEntry*)idle, k, get_tp());        // initializing stack at idle thread, from gp pointer, tp = 0 for idle stack
    Idle->thread_state = RVCOS_THREAD_STATE_READY, Idle->priority = RVCOS_THREAD_PRIORITY_IDLE, Idle->memsize = 2048;
    insertTail(idleQueue, Idle);

    threads[pos++] = Idle;
    //RVCWriteText("RVCinitialize is complete\n\n", 28);
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCTickMS(uint32_t *tickmsref) {    // set quantum aka tick time, 2ms
    if(tickmsref == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    *tickmsref = 5;
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCTickCount(TTickRef tickref) {    // change global variable and tickcount
    *tickref+=1;
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCThreadCreate(TThreadEntry entry, void *param, TMemorySize memsize, TThreadPriority prio, TThreadIDRef tid) {
    if(entry == NULL || tid == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;

    TCB *CurTCB ;
    RVCMemoryAllocate(sizeof(TCB), &CurTCB);

    CurTCB->priority = prio, CurTCB->id = *tid, CurTCB->thread_state = RVCOS_THREAD_STATE_CREATED, CurTCB->memsize = memsize, CurTCB->param = param, CurTCB->entry = entry;

     //{entry, param, memsize, prio, tid, 0};
    // RVCWriteText("Creating threads......\n", 24);

    // put the thread in the respective priority queue
    if(prio == RVCOS_THREAD_PRIORITY_LOW) insertTail(lQueue, CurTCB);
    if(prio == RVCOS_THREAD_PRIORITY_NORMAL) insertTail(nQueue, CurTCB);
    if(prio == RVCOS_THREAD_PRIORITY_HIGH) insertTail(hQueue, CurTCB);

    //CurTCB->thread_state = RVCOS_THREAD_STATE_DEAD;
    //if(hQueue->head->thread->thread_state == RVCOS_THREAD_STATE_DEAD)  RVCWriteText("lets goooooooooooo0000\n", 24);

    //RVCWriteText("\nfinished create\n", 16);
    threads[pos++] = CurTCB;
    return RVCOS_STATUS_SUCCESS;
}


TStatus RVCThreadDelete(TThreadID thread) {
    TCB t; 
    int temppos = 0;
    for(int i = 0; i <= pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->id == thread) {
            t = *threads[i];
            if (t.thread_state != RVCOS_THREAD_STATE_DEAD) return RVCOS_STATUS_ERROR_INVALID_STATE;
            temppos = i;
            TCB *threadToFree = threads[i];
            RVCMemoryDeallocate(threadToFree);

            for(int x = i; x <= pos; x++) { // shifts down the threads in the array
                threads[i] = threads[i + 1];
            }
            pos--;                          // keeps track of the pos of last thread 
            return RVCOS_STATUS_SUCCESS;
        }
    }
    // thread not found
    return RVCOS_STATUS_ERROR_INVALID_ID;
}

// sprint(VIDEO_MEMORY[64 * 10], "%p", &main->sp);

TStatus RVCThreadActivate(TThreadID thread) {
    TCB *t;
    for(int i = 0; i <= pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->id == thread) {
           //RVCWriteText("FOUND THREAD in RVCACTIVATE \n", 16);
            t = threads[i];
            if (t->thread_state != RVCOS_THREAD_STATE_CREATED && t->thread_state != RVCOS_THREAD_STATE_DEAD) return RVCOS_STATUS_ERROR_INVALID_STATE;
            //RVCWriteText("got past if statements in activate \n", 37);
             // figure out where savedsp comes from using assembly function
            
            //TCB * threadptr = &t;
            RVCMemoryAllocate(t->memsize, &(t->stackbase));
            
            t->savedsp = init_stack((uint32_t*)(t->stackbase + t->memsize), (TThreadEntry*)skeleton, thread, get_tp());

            t->thread_state = RVCOS_THREAD_STATE_READY;  // check the order of this line and line below <---------------------------
            // if(hQueue->tail->thread->thread_state == RVCOS_THREAD_STATE_READY) RVCWriteText("you have edited the queue element as well\n", 37);
            schedule();
            return RVCOS_STATUS_SUCCESS;
        }
    }

    return RVCOS_STATUS_ERROR_INVALID_ID;

}

// function to change the owner of a lock based on lock queue after termination or release
void changeOwner(Mutex *m) {
    queue* q = m->waitingToAcquire; // q = the queue of threads waiting to acquire lock m
    if(q->length == 0) {    // base case no threads waiting
        m->locked = false;
        m->threadid = NULL;
        return;
    }
    
    m->locked = true;

    TCB* highest = q->head->thread; // first element in queue acquires the lock
    node * n = q->head;
    for (int i = 0; i < q->length; i++) {   // loop though entire waitingtoacquire array
        if (highest->priority < n->thread->priority) highest = n->thread;
        n = n->next;
    }
    m->threadid = highest->id;
    eraseElement(q, highest);    // delete the highest priority thread we found since it is now acquired

    for (int i = 0; i < pos; i++) {
        if(threads[i]->id == m->threadid) {
            threads[i]->thread_state = RVCOS_THREAD_STATE_READY;
        }
    }
}

TStatus RVCThreadTerminate(TThreadID thread, TThreadReturn returnval) {
    TCB *t;
    for(int i = 0; i <= pos; i++) {    // for loop: find matching thread w id
        if (threads[i] != NULL && threads[i]->id == thread) {
            t = threads[i];
            if (t->thread_state == RVCOS_THREAD_STATE_DEAD || t->thread_state == RVCOS_THREAD_STATE_CREATED) return RVCOS_STATUS_ERROR_INVALID_STATE;

            t->returnval = returnval;

            t->thread_state = RVCOS_THREAD_STATE_DEAD;
            
            for(int i = 0; i < mutexpos; i++) {        // check if there is a lock held with this thread and if so release it
                if (locks[i]->threadid == thread) {
                    changeOwner(locks[i]);  // changes the owner of lock that had its thread terminated
                }
            }

            schedule();
            return RVCOS_STATUS_SUCCESS;
        }
    }   

    return RVCOS_STATUS_ERROR_INVALID_ID;
}

TStatus RVCThreadWait(TThreadID thread, TThreadReturnRef returnref, TTick timeout) {
    TTick start = tickcount;
    TCB *t;
    int found = 0;  // variable for making sure we find the thread
    for(int i = 0; i <= pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->id == thread) {
            t = threads[i];
            found = 1;
            break;
        }
    }
    if (found == 0) return RVCOS_STATUS_ERROR_INVALID_ID;
    if (returnref == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    if (timeout == RVCOS_TIMEOUT_IMMEDIATE) schedule(); // return immediate

    if (timeout == RVCOS_TIMEOUT_INFINITE) {
        while(t->thread_state != RVCOS_THREAD_STATE_DEAD); // wait for thread to be terminated timeout infinite
        *returnref = t->returnval;   // set the value of returnref's location variable
        schedule();
        return RVCOS_STATUS_SUCCESS;
    } 

    // case for timeout not infinite or immediate
    while (timeout > (tickcount - start)) {
        if (t->thread_state == RVCOS_THREAD_STATE_DEAD) {
            *returnref = t->returnval;   // set the value of returnref's location variable
            schedule();
            return RVCOS_STATUS_SUCCESS;
        }
    }

    return RVCOS_STATUS_FAILURE;
}

TStatus RVCThreadID(TThreadIDRef threadref){
    TCB* t;
    for(int i = 0; i <= pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->thread_state == RVCOS_THREAD_STATE_RUNNING) {
            t = threads[i];
            break;
        }
    }
    if(threadref == NULL){
        return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    }
    else{
        *threadref = t->id;
        return RVCOS_STATUS_SUCCESS;
    }
}
//Loop through the thread array and find the thread that has the specificed thread ID, and retrieve its state
//Then place that state into where stateref is pointing to
TStatus RVCThreadState(TThreadID thread, TThreadStateRef stateref){
    if(stateref == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;

    for(int i = 0; i <= pos; i++){
        if (threads[i]->id == thread){
            *stateref = threads[i]->thread_state;
            return RVCOS_STATUS_SUCCESS;
        }
    }
    return RVCOS_STATUS_ERROR_INVALID_ID;
}

//we need to put the thread to sleep for tick amount of time
//if tick is specifically: , we yield the time from one thread to the other of equal priority
TStatus RVCThreadSleep(TTick tick){
    if (tick == RVCOS_TIMEOUT_INFINITE) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    if (tick == RVCOS_TIMEOUT_IMMEDIATE) schedule();    // slightly wrong, needs to schedule to next thread of SAME priority

    TCB *previouslyRunning;     // currently running thread
    for(int i = 0; i < pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->thread_state == RVCOS_THREAD_STATE_RUNNING) {
            //RVCWriteText("\nfound running thread in schedule function\n", 44);
            previouslyRunning = threads[i];
            break;
        }
    }

    // add scheduler bs

    node* n;    // node contains the running thread that we will now put to sleep,
    n->thread = previouslyRunning;
    n->thread->sleepticks = tick;
    n->thread->thread_state = RVCOS_THREAD_STATE_WAITING;   // not sure if this is correct but imma keep it for now, idk what to change state to
    insertTail(sleepQueue, n);

    return RVCOS_STATUS_SUCCESS;
}

//  ==========================================================================================================PROJECT 3 API STARTS HERE (MAINLY)================================    

// ricardo wrote these memory pool fucntions
TStatus RVCMemoryPoolCreate(void *base, TMemorySize size, TMemoryPoolIDRef memoryref) {
    //if(base == NULL || size )
    MemoryPool* p;
    RVCMemoryAllocate(sizeof(MemoryPool), &p);
    p->base = base, p->ID = *memoryref, p->size = size;
    pools[poolpos++] = p;
    return RVCOS_STATUS_SUCCESS;
}
//remove the memory pool from the array
TStatus RVCMemoryPoolDelete(TMemoryPoolID memory){
    MemoryPool *p;
    int temppos = 0;
    for(int i = 0; i <= poolpos; i++) {    // for loop: find matching thread w id
        if (pools[i]->ID == memory) {
            p = pools[i];
            //if (p->ID != threads[i]) return RVCOS_STATUS_ERROR_INVALID_STATE;
            temppos = i;
            MemoryPool *memoryToFree = pools[i];
            RVCMemoryDeallocate(memoryToFree);
            //free(&memoryToFree);

            for(int x = i; x <= poolpos; x++) { // shifts down the threads in the array
                pools[i] = pools[i + 1];
            }
            poolpos--;                          // keeps track of the pos of last thread 
            return RVCOS_STATUS_SUCCESS;
        }
    }
    // thread not found
    return RVCOS_STATUS_ERROR_INVALID_ID; 
}

TStatus RVCMemoryPoolQuery(TMemoryPoolID memory, TMemorySizeRef bytesleft) {
    if(memory == NULL || bytesleft == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    for (int i = 0; i < poolpos; i++) {
        if((pools[i])->ID == memory) {
            MemoryPool *p = pools[i];

            *bytesleft = p->size;
            
            return RVCOS_STATUS_SUCCESS;
        }
    }
    return RVCOS_STATUS_ERROR_INVALID_ID;
    
}

TStatus RVCMemoryPoolDeallocate(TMemoryPoolID memory, void *pointer) {
    MemoryPool* p;
    for (int i = 0; i < poolpos; i++){
        if(pools[i] == memory){
            p = pools[i];
            free(p->base);
            return RVCOS_STATUS_SUCCESS;
        }
    }

    free(pointer);
    return RVCOS_STATUS_SUCCESS;
}


TStatus RVCMutexCreate(TMutexIDRef mutexref) {
    if(mutexref == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    // if(mem allocation invalid) return invalid memory
    Mutex* m;
    RVCMemoryAllocate(sizeof(Mutex), &m);
    RVCMemoryAllocate(sizeof(queue), &(m->waitingToAcquire));
    m->id = *mutexref, m->threadid = NULL, m->locked = false; // set mutex id, and allocate space for the array of threads waiting to acquire the lock
    locks[mutexpos++] = m;

    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCMutexDelete(TMutexID mutex) {
    for (int i = 0; i < mutexpos; i++) {
        if(locks[i]->id == mutex) {
            Mutex *m = locks[i];
            if(m->locked == true) return RVCOS_STATUS_ERROR_INVALID_STATE;
            RVCMemoryDeallocate(m);

            for(int x = i; x <= mutexpos; x++) { // shifts down the threads in the array
                locks[i] = locks[i + 1];
            }
            mutexpos--;                          // keeps track of the pos of last thread 
            return RVCOS_STATUS_SUCCESS;
        }
    }
    return RVCOS_STATUS_ERROR_INVALID_ID;
}

TStatus RVCMutexQuery(TMutexID mutex, TThreadIDRef ownerref) {
    if(ownerref == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    for (int i = 0; i < mutexpos; i++) {
        if((locks[i])->id == mutex) {
            Mutex *m = locks[i];
            if(m->locked == false) return RVCOS_THREAD_ID_INVALID;

            *ownerref = m->threadid;
            
            return RVCOS_STATUS_SUCCESS;
        }
    }
    return RVCOS_STATUS_ERROR_INVALID_ID;
}

TStatus RVCMutexAcquire(TMutexID mutex, TTick timeout) {
    // RVCWriteText("got into acquire\n", 18);
    TTick start = tickcount;
    TCB *previouslyRunning;     // currently running thread
    for(int i = 0; i < pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->thread_state == RVCOS_THREAD_STATE_RUNNING) {
             //RVCWriteText("found running thread in acquire function\n", 44);
            previouslyRunning = threads[i];
            break;
        }
    }
    
    // loop finds the specified lock, then tries to acquire it based on timeout
    for (int i = 0; i < mutexpos; i++) {
        if((locks[i])->id == mutex) {
            Mutex *m = locks[i];
             //RVCWriteText("found lock\n", 12);

            if (timeout == RVCOS_TIMEOUT_IMMEDIATE) {   // if timeout immediate we only get one chance to acquire lock
                if(m->locked == true) return RVCOS_STATUS_FAILURE;
                else {
                    m->locked = true;
                    m->threadid = previouslyRunning->id;
                    return RVCOS_STATUS_SUCCESS;
                }
            }
            if (timeout == RVCOS_TIMEOUT_INFINITE) {
                // RVCWriteText("got into infinite if\n", 22);
                previouslyRunning->thread_state = RVCOS_THREAD_STATE_WAITING;
                insertTail(m->waitingToAcquire, previouslyRunning);
                while(m->locked == true);
                // RVCWriteText("it is done now\n", 16);
                m->threadid = previouslyRunning->id;
                m->locked = true;
                return RVCOS_STATUS_SUCCESS;
            }
            else {
                while (timeout > (tickcount - start)) {
                    if (m->locked != true) {            // check if lock is still locked
                        m->threadid = previouslyRunning->id;        // the running thread has acquired lock
                        m->locked = true;
                        return RVCOS_STATUS_SUCCESS;
                    }
                }
            }
            return RVCOS_STATUS_FAILURE;    // all cases failed to acquire lock
        }
    }
    return RVCOS_STATUS_ERROR_INVALID_ID;
}

TStatus RVCMutexRelease(TMutexID mutex) {
    TCB *currentlyRunning;     // currently running thread
    for(int i = 0; i < pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->thread_state == RVCOS_THREAD_STATE_RUNNING) {
            //RVCWriteText("found running thread in mutexrelease function\n", 44);
            currentlyRunning = threads[i];
            break;
        }
    }

    for (int i = 0; i < mutexpos; i++) {    // finds the lock specified by mutex
        if((locks[i])->id == mutex) {
            Mutex *m = locks[i];
            if(m->threadid != currentlyRunning->id) return RVCOS_STATUS_ERROR_INVALID_STATE;
            changeOwner(m);
            m->locked = false;
            m->threadid = NULL;
            return RVCOS_STATUS_SUCCESS;
        }
    }
    return RVCOS_STATUS_ERROR_INVALID_ID;   // did not find lock
}

// *buffer: character array which points to the start position
// writeSize: how many characters we have in the buffer;
TStatus RVCWriteText(const TTextCharacter *buffer, TMemorySize writesize){
    if (buffer == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    int temp = 0;
    char newline = '\n', backspace = '\b', carriage = '\r'; 

    /*
    TCB *previouslyRunning;     // currently running thread
    for(int i = 0; i < pos; i++) {    // for loop: find matching thread w id
        if (threads[i]->thread_state == RVCOS_THREAD_STATE_RUNNING) {
            previouslyRunning = threads[i];
            previouslyRunning->thread_state = RVCOS_THREAD_STATE_READY;
            if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_HIGH) {
                eraseElement(hQueue, previouslyRunning->id);
                insertTail(hQueue, previouslyRunning);
            }
            if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_NORMAL) {
                eraseElement(nQueue, previouslyRunning->id);
                insertTail(nQueue, previouslyRunning);
            }
            if (previouslyRunning->priority == RVCOS_THREAD_PRIORITY_LOW) {
                eraseElement(lQueue, previouslyRunning->id);
                insertTail(lQueue, previouslyRunning);
            }
            break;
        }
    }  */

    while(temp < writesize ) {      // copyright us #works
        char c = buffer[temp];
        if (cursorPos >= 36*64-1) {      // checking if we need to wrap based on position
            for (int i = 64; i <= cursorPos; i++) {       // first shift all values in video memory -64 to simulate rows going back by 1
                VIDEO_MEMORY[i-64] = VIDEO_MEMORY[i];   // sets all characters a row back
            }
            cursorPos -= 64;     // reset position to a row back
        }

    // VT 100 codes
        if (((temp+2) < writesize) && buffer[temp] == '\x1B' && buffer[temp+1] == '[' && buffer[temp+2] == 'A') {   // up
            if (cursorPos <= 63) {
                temp+=3;
                 continue;
            }
            cursorPos-=64;
            temp+=3;
            continue;
        } 
        if (((temp+2) < writesize) && buffer[temp] == '\x1B' && buffer[temp+1] == '[' && buffer[temp+2] == 'B') {   // down
            cursorPos+=64;
            
            temp+=3;
            continue;
        } 
        if (((temp+2) < writesize) && buffer[temp] == '\x1B' && buffer[temp+1] == '[' && buffer[temp+2] == 'C') {   // right
            cursorPos++;
            temp+=3;
            continue;
        } 
        if (((temp+2) < writesize) &&  buffer[temp] == '\x1B' && buffer[temp+1] == '[' && buffer[temp+2] == 'D') {   // left
            cursorPos--;
            temp+=3;
            continue;
        } // https://stackoverflow.com/questions/9593097/convert-a-character-to-an-integer-in-c
        if (((temp+5) < writesize) && buffer[temp] == '\x1B' && buffer[temp+1] == '[' && buffer[temp+3] == ';' && buffer[temp+5] == 'H' ) {   // move cursor ln col
            int col = 0, ln = 0;
            VIDEO_MEMORY[cursorPos] = ' ';  // set previous marker to empty
            ln += buffer[temp+2]-'0'; // https://stackoverflow.com/questions/9593097/convert-a-character-to-an-integer-in-c
            col += buffer[temp+4]-'0';
            cursorPos = ln; // set cursorpos to first char in line set
            cursorPos += col;   // then add the col to cursorpos
            temp+=6;
        } 
        if (((temp+2) < writesize) && buffer[temp] == '\x1B' && buffer[temp+1] == '[' && buffer[temp+2] == 'H') {   // upper left
            VIDEO_MEMORY[cursorPos] = ' ';  // set previous marker to empty
            cursorPos = 0;
            temp+=3;
        }
        if (((temp+3) < writesize) &&  buffer[temp] == '\x1B' && buffer[temp+1] == '[' && buffer[temp+2] == '2' && buffer[temp+3] == 'J') { // erase screen
            for (int i = 0; i < 36*64; i++) {       // loop through all video memory thiings possible
                VIDEO_MEMORY[i] = ' ';   // sets all characters a row back
            }
            temp+=4;
        }

    // everything else
        if (c == backspace) {
            VIDEO_MEMORY[cursorPos--] = buffer[temp++];
            continue;
        }
        else if (c == newline || c == carriage) { 
            VIDEO_MEMORY[cursorPos] = '\n';
            temp++;
            int charLeftTillEndRow = 63 - (cursorPos % 64);
            cursorPos += (++charLeftTillEndRow);
            continue;
        }
        else if (cursorPos % 64 == 0) {  // check if end of line
            VIDEO_MEMORY[cursorPos++] = newline;
        }
        else VIDEO_MEMORY[cursorPos++] = buffer[temp++];
    }

    //schedule();
    return RVCOS_STATUS_SUCCESS;
}


TStatus RVCReadController(SControllerStatusRef statusref) {

    if(statusref == NULL) return RVCOS_STATUS_FAILURE;

    *(uint32_t *)statusref = CONTROLLER;  // https://piazza.com/class/ktlxmp9hob46o0?cid=410
    return RVCOS_STATUS_SUCCESS;        // 
}





//Changes the value of the mode register between text and graphics mode
//mode can either be a 0 or 1, Text_mode = 0 and Graphics_mode = 1
TStatus RVCChangeVideoMode(TVideoMode mode){
    //if(mode != RVCOS_VIDEO_MODE_GRAPHICS || mode != RVCOS_VIDEO_MODE_TEXT) return RVCOS_STATUS_FAILURE;
    ModeControl->DMode ^= 1;
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCSetVideoUpcall(TThreadEntry upcall, void *param){

}

// Create a graphic specified by the type: FULL: 512x288, LARGE: 33x33, SMALL: 1x1 - 16x16
// creating a buffer with a specified id the buffer reflecting the side of the graphic
TStatus RVCGraphicCreate(TGraphicType type, TGraphicIDRef gidref){
    // if(gidref == NULL) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    RVCWriteText("please work\n",13);
	OffScreenBufferControl *buff;   // create the offscreen buffer
    RVCMemoryAllocate(sizeof(OffScreenBufferControl), &buff); 
	buff->type = type;
	buff->id = *gidref;
    buff->status = RVCOS_GRAPHIC_STATUS_DEACTIVATED;

    // we allocate enough memory for a full sized graphic in the 1d array
    if (type == RVCOS_GRAPHIC_TYPE_FULL) RVCMemoryAllocate(512*sizeof(TPaletteIndex), &(buff->buffer));
    if (type == RVCOS_GRAPHIC_TYPE_LARGE) RVCMemoryAllocate(64*sizeof(TPaletteIndex), &(buff->buffer));
    if (type == RVCOS_GRAPHIC_TYPE_SMALL) RVCMemoryAllocate(16*sizeof(TPaletteIndex), &(buff->buffer));
    
    graphicsBuffer[buffpos++] = buff;
	return RVCOS_STATUS_SUCCESS;
    
}
//Loop through the array of graphics and remove the specified one
TStatus RVCGraphicDelete(TGraphicID gid){
    for (int i = 0; i < buffpos; i++) {
        if (graphicsBuffer[i]->id == gid) {
            OffScreenBufferControl* buff = graphicsBuffer[i];
            if (buff->status == RVCOS_GRAPHIC_STATUS_ACTIVATED || buff->status == RVCOS_GRAPHIC_STATUS_PENDING) return RVCOS_STATUS_ERROR_INVALID_STATE;

            for(int x = i; x <= buffpos; x++) { // shifts down the threads in the array
                graphicsBuffer[i] = graphicsBuffer[i + 1];
            }
            buffpos--;
            RVCMemoryDeallocate(buff);
            RVCGraphicDeactivate(gid);
            return RVCOS_STATUS_SUCCESS;
        }
    }
    return RVCOS_STATUS_ERROR_INVALID_ID;
}

//we activate a graphics buffer which gets placed on the screen at a given X and Y position and we only display
//a portion of that grpahic based on the dim paramater with the given palette if pid =
TStatus RVCGraphicActivate(TGraphicID gid, SGraphicPositionRef pos, SGraphicDimensionsRef dim, TPaletteID pid){
    
    RVCWriteText("got into activate\n",19);
    OffScreenBufferControl* Graphic = graphicsBuffer[0]; 
    Graphic->status = RVCOS_GRAPHIC_STATUS_PENDING;
    RVCWriteText("about to create background\n",28);
    TPaletteIndex buffer[512];
    for(int i= 0; i< 512; i++){
        buffer[i] = 0x0F;
    }
    if(Graphic->type == RVCOS_GRAPHIC_TYPE_FULL){
        RVCWriteText("got into if\n",13);
        memset((void *)BackgroundData[0],13,512*288);
        BackgroundControls[0].DPalette = 0;
        BackgroundControls[0].DXOffset = 512;
        BackgroundControls[0].DYOffset = 288;
        BackgroundControls[0].DZ = 0;
        /*
        RVCWriteText("created background\n",20);
        Graphic->status = RVCOS_GRAPHIC_STATUS_ACTIVATED;   // i think this should
        memcpy((void *)BackgroundPalettes[0],RVCOPaletteDefaultColors,256 * sizeof(SColor));
        memcpy((void *)BackgroundData[0],(graphicsBuffer[0]),512* 288); //copy over background sprite data
        RVCWriteText("WROTE SPRITE TO SCREEN\n",24);
        BackgroundControls[0].DPalette = 0;
        BackgroundControls[0].DZ = pos->DZPosition;
        BackgroundControls[0].DXOffset = pos->DXPosition - 512;
        BackgroundControls[0].DYOffset = pos->DYPosition - 288;
        */
    }
    if(Graphic->type == RVCOS_GRAPHIC_TYPE_LARGE){
        Graphic->status = RVCOS_GRAPHIC_STATUS_ACTIVATED;
        memcpy((void *)LargeSpriteData[lgpos],Graphic->buffer,dim->DHeight * dim->DWidth); //copy over the large sprite data
        LargeSpriteControls[lgpos].DPalette = pid;
        LargeSpriteControls[lgpos].DWidth = dim->DWidth +33;
        LargeSpriteControls[lgpos].DHeight = dim->DHeight +33;
        LargeSpriteControls[lgpos].DXOffset = pos->DXPosition - 64;
        LargeSpriteControls[lgpos].DYOffset = pos->DYPosition - 64;
    }
    if(Graphic->type == RVCOS_GRAPHIC_TYPE_SMALL){
        Graphic->status = RVCOS_GRAPHIC_STATUS_ACTIVATED;
        memcpy((void *)SmallSpriteData[sgpos],(Graphic->buffer),dim->DHeight * dim->DWidth);
        SmallSpriteControls[sgpos].DPalette = pid;
        SmallSpriteControls[sgpos].DWidth = dim->DWidth +1 ;
        SmallSpriteControls[sgpos].DHeight = dim->DHeight +1;
        SmallSpriteControls[sgpos].DXOffset = pos->DXPosition - 16;
        SmallSpriteControls[sgpos].DYOffset = pos->DYPosition - 16;
        SmallSpriteControls[sgpos].DZ = pos->DZPosition;
    }
    return RVCOS_STATUS_SUCCESS;  

}

TStatus RVCGraphicDeactivate(TGraphicID gid){
    if(FindOnScreen(gid) == NULL) return RVCOS_STATUS_ERROR_INVALID_ID;
    OnScreenBuffer* buf = FindOnScreen(gid);    // since its not null, buf is the 
    if(buf->status == RVCOS_GRAPHIC_STATUS_PENDING || buf->status != RVCOS_GRAPHIC_STATUS_ACTIVATED) return RVCOS_STATUS_ERROR_INVALID_STATE;

    // we check buf type to get into the correct onscreenbuffer to remove data and control block from video memory
    // remove from data and control memory but pushing everything forward one from index, setting last one to 0
    if(buf->type == RVCOS_GRAPHIC_TYPE_FULL) {
        for(int i = buf->inMemPos; i < bgpos; i++) {
            BackgroundData[i] = BackgroundData[i+1];
            BackgroundControls[i] = BackgroundControls[i+1];
        }
        BackgroundData[--bgpos] = 0;    // since bgpos points to the next to add position in array, we minus it first to get last element in array
    }
    if(buf->type == RVCOS_GRAPHIC_TYPE_LARGE) {
        for(int i = buf->inMemPos; i < lgpos; i++) {
            LargeSpriteData[i] = LargeSpriteData[i+1];
            LargeSpriteControls[i] = LargeSpriteControls[i+1];
        }
        LargeSpriteData[--lgpos] = 0;
    }
    if(buf->type == RVCOS_GRAPHIC_TYPE_SMALL) {
        for(int i = buf->inMemPos; i < sgpos; i++) {
            SmallSpriteData[i] = SmallSpriteData[i+1];
            SmallSpriteControls[i] = SmallSpriteControls[i+1];
        }
        BackgroundData[--sgpos] = 0;
    }

    return RVCOS_STATUS_SUCCESS;
}

OnScreenBuffer* FindOnScreen(TGraphicID gid){
    //Helper function to find the onscreen buffer for use in draw
    OnScreenBuffer* tempBuff = NULL;
    for(int i = 0; i < bgpos; i++){
        if(BackgroundGraphics[i]->id == gid){
            tempBuff = BackgroundGraphics[i];
        }
    }
    for(int i = 0; i < lgpos; i++){
        if(LargeGraphics[i]->id == gid){
            tempBuff = LargeGraphics[i];
        }
    }
    for(int i = 0; i < sgpos; i++){
        if(SmallGraphics[i]->id == gid){
            tempBuff = SmallGraphics[i];
        }
    }
    return tempBuff;
}
TStatus RVCGraphicDraw(TGraphicID gid, SGraphicPositionRef pos, SGraphicDimensionsRef dim, TPaletteIndexRef src, uint32_t srcwidth){
    // FIND graphic with gid
    // update buffer in offscreenbuffer struct using dimensions and shi, src, srcwidth
    //if(pos == NULL || dim == NULL || src == NULL || srcwidth < dim->DWidth) return RVCOS_STATUS_ERROR_INVALID_PARAMETER;
    /*
    for (int i = 0; i < buffpos; i++) {
        if (graphicsBuffer[i]->id == gid) {
            OffScreenBufferControl* graphic = graphicsBuffer[i];
            OnScreenBuffer* buf;    // declare on screen buffer and put it into array
            RVCMemoryAllocate(sizeof(OnScreenBuffer), &buf);

            if (graphic->type == RVCOS_GRAPHIC_TYPE_FULL) { 
                int startingpos = (pos->DYPosition - 1)*512 + pos->DXPosition - 1;  // starting position in background graphic array space
                // https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c for sizeof
                for (int j = 0; j < dim->DHeight * srcwidth; j++) {     // j is the iterator of src[] array in src space
                    if (j % (dim->DWidth) == 0 && j != 0 ) { // check if we need to go to new line for graphic
                        j+= (srcwidth - dim->DWidth + 1); // go to next line in src array
                        startingpos += (512 - startingpos + pos->DXPosition - 1);               // go to next line in starting pos
                    }
                    graphic->buffer[startingpos++] = src[j];  
                }
                RVCMemoryAllocate(512*288*sizeof(TPaletteIndex), &(buf->buffer));
                buf->buffer = graphic->buffer, buf->id = graphic->id, buf->type = graphic->type, buf->status = graphic->status; // pass these into the onscreenbuffer array
                BackgroundGraphics[bgpos++] = buf;
                return RVCOS_STATUS_SUCCESS;
            }

            if (graphic->type == RVCOS_GRAPHIC_TYPE_LARGE) { 
                int startingpos = (pos->DYPosition - 1)*64 + pos->DXPosition - 1;  // starting position in background graphic array space
                // https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c for sizeof
                for (int j = 0; j < dim->DHeight * srcwidth; j++) {     // j is the iterator of src[] array in src space
                    if (j % (dim->DWidth) == 0 && j != 0 ) { // check if we need to go to new line for graphic
                        j+= (srcwidth - dim->DWidth + 1); // go to next line in src array
                        startingpos += (64 - startingpos + pos->DXPosition - 1);               // go to next line in starting pos
                    }
                    graphic->buffer[startingpos++] = src[j];  
                }
                RVCMemoryAllocate(64*64*sizeof(TPaletteIndex), &(buf->buffer));
                buf->buffer = graphic->buffer, buf->id = graphic->id, buf->type = graphic->type, buf->status = graphic->status; // pass these into the onscreenbuffer array
                LargeGraphics[lgpos++] = buf;
                return RVCOS_STATUS_SUCCESS;
            }

            if (graphic->type == RVCOS_GRAPHIC_TYPE_SMALL) { 
                int startingpos = (pos->DYPosition - 1)*16 + pos->DXPosition - 1;  // starting position in background graphic array space
                // https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c for sizeof
                for (int j = 0; j < dim->DHeight * srcwidth; j++) {     // j is the iterator of src[] array in src space
                    if (j % (dim->DWidth) == 0 && j != 0) { // check if we need to go to new line for graphic
                        j+= (srcwidth - dim->DWidth + 1); // go to next line in src array
                        startingpos += (16 - startingpos + pos->DXPosition - 1);               // go to next line in starting pos
                    }
                    graphic->buffer[startingpos++] = src[j];  
                }
                RVCMemoryAllocate(16*16*sizeof(TPaletteIndex), &(buf->buffer));
                buf->buffer = graphic->buffer, buf->id = graphic->id, buf->type = graphic->type, buf->status = graphic->status; // pass these into the onscreenbuffer array
                SmallGraphics[sgpos++] = buf;
                return RVCOS_STATUS_SUCCESS;
            }
        }
    }
    */
    return RVCOS_STATUS_ERROR_INVALID_ID;   // end function
}

//Allocate space for a palette of 256 and assign it an id
TStatus RVCPaletteCreate(TPaletteIDRef pidref){
    PaletteControl* NewPalette;
    RVCMemoryAllocate(sizeof(PaletteControl),&NewPalette);
    NewPalette->id = *pidref;
    RVCMemoryAllocate(sizeof(SColor)*256, &(NewPalette->palette));
    palettes[palettepos++] = NewPalette;
    return RVCOS_STATUS_SUCCESS;
}

TStatus RVCPaletteDelete(TPaletteID pid){
    for (int i = 0; i < buffpos; i++) {
        if (palettes[i]->id == pid) {
            PaletteControl* palette = palettes[i];

            for(int x = i; x <= palettepos; x++) { // shifts down the threads in the array
                palettes[i] = palettes[i + 1];
            }
            palettepos--;
            RVCMemoryDeallocate(palette->palette);
            RVCMemoryDeallocate(palette);
            return RVCOS_STATUS_SUCCESS;
        }
    }
    return RVCOS_STATUS_ERROR_INVALID_ID;
}

TStatus RVCPaletteUpdate(TPaletteID pid, SColorRef cols, TPaletteIndex offset, uint32_t count){
    PaletteControl* p;
    for(int i = 0; i< palettepos; i++ ){
        if(palettes[i]->id == pid){
            p = palettes[i];
        }     
    }
    for(int i = 0; i < count; i++){
        SColor defaultpal = {8,8,8,8};
        p->palette[i] = defaultpal;
    }
    
    return RVCOS_STATUS_SUCCESS;
}

