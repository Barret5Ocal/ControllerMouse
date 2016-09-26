#if !defined(XBOXONEMOUSE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Barret Gaylor $
   $Notice: (C) Copyright 2015 by Barret Gaylor. All Rights Reserved. $
   ======================================================================== */

#include "xboxonemouse_intrinsics.h"
#include "xboxonemouse_math.h"

#define BITMAP_BYTES_PER_PIXEL 4

struct loaded_bitmap
{
    int32 Width;
    int32 Height;
    int32 Pitch;
    void *Memory;
};


#include "xboxonemouse_stb.h"

enum character
{
    A = 0x41,
    B = 0x42,
    C = 0x43,
    D = 0x44,
    E = 0x45,
    F = 0x46,
    G = 0x47,
    H = 0x48,
    I = 0x49,
    J = 0x4A,
    K = 0x4B,
    L = 0x4C,
    M = 0x4D,
    N = 0x4E,
    O = 0x4F,
    P = 0x50,
    Q = 0x51,
    R = 0x52,
    S = 0x53,
    T = 0x54,
    U = 0x55,
    V = 0x56,
    W = 0x57,
    X = 0x58,
    Y = 0x59,
    Z = 0x5A,
    ONE = 0x31,
    TWO = 0x32,
    THREE = 0x33,
    FOUR = 0x34,
    FIVE = 0x35,
    SIX = 0x36,
    SEVEN = 0x37,
    EIGHT = 0x38,
    NINE = 0x39,
    ZERO = 0x30,
};

struct character_set
{
    char *Set;
};

enum controller_button
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ACTION_UP,
    ACTION_DOWN,
    ACTION_LEFT,
    ACTION_RIGHT,
    LEFT_SHOULDER,
    RIGHT_SHOULDER,
    LEFT_TRIGGER,
    RIGHT_TRIGGER,
    LEFT_STICK,
    RIGHT_STICK,
    BACK,
    START
};

enum function
{
    LEFTCLICK,
    RIGHTCLICK,
    MIDDLECLICK,
    WHEEL,
    CTR,
    SPACE,
    SHIFT,
    ALT,
    ESC,
    TAB,
    CAPS_LOCK,
    RETURN,
    BACKSPACE,
    MUTE,
    VOLUME_UP,
    VOLUME_DOWN,
    WINDOWS,
    DEL,
    HOME,
    END,
    PG_UP,
    PG_DOWN,
    INSERT,
    ARROW_UP,
    ARROW_DOWN,
    ARROW_RIGHT,
    ARROW_LEFT,
    SPEED_BOOST   
};

struct controller_config
{
    function MoveUp;
    function MoveDown;
    function MoveLeft;
    function MoveRight;
    function ActionUp;
    function ActionDown;
    function ActionLeft;
    function ActionRight;
    function LeftShoulder;
    function RightShoulder;
    function LeftTrigger;
    function RightTrigger;
    function LeftStick;
    function RightStick;
    function Back;
};

enum button_setting
{
    NONE, 
    DOWN,
    HOLD,
    UP
};

struct button_state
{
    function ButtonType;
    int32 ButtonData; 
};

struct commands
{
    v2 MousePos;
    button_state ButtonStates[27];
};

enum mode
{
    DEFAULT,
    TYPE,
    MENU
};

struct state
{
    mode Mode;
    int32 ModeData;
    char CharToType; 
    controller_config Config;
    commands Commands;


    loaded_bitmap AButton; 
};


typedef struct game_offscreen_buffer
{
    // NOTE(casey): Pixels are alwasy 32-bits wide, Memory Order BB GG RR XX
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
} game_offscreen_buffer;

#define XBOXONEMOUSE_H
#endif
