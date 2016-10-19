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

struct render_basis
{
    v2 Origin;
    v2 IHat;
    v2 JHat; 
};

#include "xboxonemouse_stb.h"

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
    bool32 IsModeInitialized;
    char CharToType; 
    controller_config Config;
    commands Commands;

    v2 ScreenDim; 
    v2 ClientDim; 

    loaded_bitmap AButton;
    loaded_bitmap BButton;
    loaded_bitmap XButton;
    loaded_bitmap YButton;
    loaded_bitmap StickUp;
    loaded_bitmap StickDown;
    loaded_bitmap StickRight;
    loaded_bitmap StickLeft;
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
