/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Barret Gaylor $
   $Notice: (C) Copyright 2015 by Barret Gaylor. All Rights Reserved. $
   ======================================================================== */
#include "xboxonemouse.h"


internal void
ActionPickCharacter(char *CharToType, character_set Set, game_controller_input *Controller)
{
    if(Controller->ActionDown.EndedDown && Controller->ActionDown.HalfTransitionCount == 1)
    {
        *CharToType = Set.Set[0];                             
    }
    if(Controller->ActionUp.EndedDown && Controller->ActionUp.HalfTransitionCount == 1)
    {
        *CharToType = Set.Set[1];
    }
    if(Controller->ActionRight.EndedDown && Controller->ActionRight.HalfTransitionCount == 1)
    {
        *CharToType = Set.Set[2];
    }
    if(Controller->ActionLeft.EndedDown && Controller->ActionLeft.HalfTransitionCount == 1)
    {
        *CharToType = Set.Set[3];
    } 
}

internal void
MovePickCharacter(char *CharToType, character_set Set, game_controller_input *Controller)
{
    if(Controller->MoveDown.EndedDown && Controller->MoveDown.HalfTransitionCount == 1)
    {
        *CharToType = Set.Set[0];                                
    }
    if(Controller->MoveUp.EndedDown && Controller->MoveUp.HalfTransitionCount == 1)
    {
        *CharToType = Set.Set[1];                               
    }
    if(Controller->MoveRight.EndedDown && Controller->MoveRight.HalfTransitionCount == 1)
    {
        *CharToType = Set.Set[2];                               
    }
    if(Controller->MoveLeft.EndedDown && Controller->MoveLeft.HalfTransitionCount == 1)
    {
        *CharToType = Set.Set[3];                               
    }
}

internal void
PickDirection(char *CharToType, v2 ddP, character_set BigSet[4], game_controller_input *Controller, bool32 LeftStick)
{
    if(ddP.Y == 1.0f)
    {
        character_set Set = {};
        Set.Set = BigSet[0].Set;
            
        if(LeftStick)
            ActionPickCharacter(CharToType, Set, Controller);
    }
                    
    if(ddP.Y == -1.0f)
    {
        character_set Set = {};
        Set.Set = BigSet[1].Set;
            
        if(LeftStick)
            ActionPickCharacter(CharToType, Set, Controller);
            
    }
                                        
    if(ddP.X == 1.0f)
    {
        character_set Set = {};
        Set.Set = BigSet[2].Set;
            
        if(LeftStick)            
            ActionPickCharacter(CharToType, Set, Controller);
            
    }
                                        
    if(ddP.X == -1.0f)
    {
        character_set Set = {};
        Set.Set = BigSet[3].Set;
            
        if(LeftStick)                        
            ActionPickCharacter(CharToType, Set, Controller);
            
    }

    
 
}

internal void
MoveMouse(v2 *NewMousePos, v2 ddP, real32 dt, v2 MousePos, real32 MouseSpeed)
{
     
    v2 Velocity = (ddP * MouseSpeed) * dt;

    *NewMousePos = Velocity + MousePos;

}

internal void
ButtonDown(commands *Commands, game_button_state *State, function Type)
{
    
    if(State->EndedDown && State->HalfTransitionCount == 1)
    {
        Commands->ButtonStates[Type].ButtonData = 1;           
    }


}

internal void
ButtonUp(commands *Commands, game_button_state *State, function Type)
{
    if(!State->EndedDown && State->HalfTransitionCount == 1)
    {
        Commands->ButtonStates[Type].ButtonData = 3;
    }    
}

internal void
DrawRectangle(game_offscreen_buffer *Buffer, v2 Min, v2 Max,
              real32 R, real32 G, real32 B, real32 A = 1.0f)
{

    int32 MinX = RoundReal32ToInt32(Min.X);
    int32 MinY = RoundReal32ToInt32(Min.Y);
    int32 MaxX = RoundReal32ToInt32(Max.X);
    int32 MaxY = RoundReal32ToInt32(Max.Y); 

    int32 SourceOffsetX = 0;
    if(MinX < 0)
    {
        SourceOffsetX = -MinX;
        MinX = 0;
    }

    int32 SourceOffsetY = 0;
    if(MinY < 0)
    {
        SourceOffsetY = -MinY;
        MinY = 0; 
    }

    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height; 
    }

    uint32 Color32 = ((RoundReal32ToUInt32(A * 255.0f) << 24) |
                      (RoundReal32ToUInt32(R * 255.0f) << 16) |
                      (RoundReal32ToUInt32(G * 255.0f) << 8) |
                      (RoundReal32ToUInt32(B * 255.0f) << 0));

    uint8 *Row = ((uint8 *)Buffer->Memory +
        MinX*BITMAP_BYTES_PER_PIXEL +
        MinY*Buffer->Pitch); 
    for(int32 Y = MinY;
        Y < MaxY;
        ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int32 X = MinX;
            X < MaxX;
            ++X)
        {
            *Pixel++ = Color32; 
        }

        Row += Buffer->Pitch; 
    }
}


internal void
DrawBitmap(game_offscreen_buffer *Buffer, loaded_bitmap *Bitmap, real32 RealX, real32 RealY)
{
    //TODO(barret): fill out

    int32 MinX = RoundReal32ToInt32(RealX);
    int32 MinY = RoundReal32ToInt32(RealY);
    int32 MaxX = MinX + Bitmap->Width;
    int32 MaxY = MinY + Bitmap->Height; 

    int32 SourceOffsetX = 0;
    if(MinX < 0)
    {
        SourceOffsetX = -MinX;
        MinX = 0;
    }

    int32 SourceOffsetY = 0;
    if(MinY < 0)
    {
        SourceOffsetY = -MinY;
        MinY = 0; 
    }

    if(MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    if(MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height; 
    }

    uint8 *SourceRow = (uint8 *)Bitmap->Memory +
        SourceOffsetY*Bitmap->Pitch +
        SourceOffsetX*BITMAP_BYTES_PER_PIXEL;
    uint8 *DestRow = (uint8 *)Buffer->Memory +
        MinX*BITMAP_BYTES_PER_PIXEL +
        MinY*Buffer->Pitch;

    for (int32 Y = MinY;
         Y < MaxY;
         ++Y)
    {
        uint32 *Source = (uint32 *) SourceRow;
        uint32 *Dest = (uint32 *) DestRow; 
        for(int32 X = MinX;
            X < MaxX;
            ++X)
        {
            real32 A = (real32)((*Source >> 24) & 0xFF) / 255.0f;  
            real32 SB = (real32)((*Source >> 16) & 0xFF);
            real32 SG = (real32)((*Source >> 8) & 0xFF);
            real32 SR = (real32)((*Source >> 0) & 0xFF);

            real32 DR = (real32)((*Dest >> 16) & 0xFF);
            real32 DG = (real32)((*Dest >> 8) & 0xFF);
            real32 DB = (real32)((*Dest >> 0) & 0xFF);

            real32 R = (1.0f - A)*DR + A*SR;
            real32 G = (1.0f - A)*DG + A*SG;
            real32 B = (1.0f - A)*DB + A*SB;
            
            *Dest = (((uint32)(R + 0.5f) << 16) |
                     ((uint32)(G + 0.5f) << 8) |
                     ((uint32)(B + 0.5f) << 0)); 

            ++Dest;
            ++Source; 
        }
        
        SourceRow += Bitmap->Pitch;
        DestRow += Buffer->Pitch; 
    }
}

internal void
DrawBitmapSlowly(game_offscreen_buffer *Buffer, loaded_bitmap *Texture, v2 Origin, v2 XAxis, v2 YAxis,
                 v4 Color)
{
    uint32 Color32 = ((RoundReal32ToUInt32(Color.a * 255.0f) << 24) |
                      (RoundReal32ToUInt32(Color.r * 255.0f) << 16) |
                      (RoundReal32ToUInt32(Color.g * 255.0f) << 8) |
                      (RoundReal32ToUInt32(Color.b * 255.0f) << 0));
    
    int32 MinX = Buffer->Width;
    int32 MinY = Buffer->Height;
    int32 MaxX = 0;
    int32 MaxY = 0;

    v2 P[4] = {Origin, Origin + XAxis, Origin + YAxis, Origin + XAxis + YAxis};
    for(int32 PIndex = 0;
        PIndex < ArrayCount(P);
        ++PIndex)
    {
        v2 PTest = P[PIndex];
        int32 Test0 = FloorReal32ToInt32(PTest.X);
        int32 Test1 = FloorReal32ToInt32(PTest.Y);
        int32 Test2 = CeilReal32ToInt32(PTest.X);
        int32 Test3 = CeilReal32ToInt32(PTest.Y);

        if(Test0 < MinX){MinX = Test0;}
        if(Test1 < MinY){MinY = Test1;}
        if(Test2 > MaxX){MaxX = Test2;}
        if(Test3 > MaxY){MaxY = Test3;}
    }


    uint8 *Dest = (uint8 *)Buffer->Memory + MinY*Buffer->Pitch + MinX*BITMAP_BYTES_PER_PIXEL;
    for (int32 Y = MinY;
         Y < MaxY;
         ++Y)
    {
        uint32 *Pixel = (uint32 *)Dest; 
        for (int32 X = MinX;
             X < MaxX;
             ++X)
        {
            v2 P = {(real32)X, (real32)Y};
            v2 V = P - Origin;
            v2 Max = {Origin + XAxis + YAxis};
            v2 V2 = P - Max;

            int32 Edge0 = Inner(V, XAxis);
            int32 Edge1 = Inner(V, YAxis);
            int32 Edge2 = Inner(V2, (Origin - XAxis) - Max);
            int32 Edge3 = Inner(V2, (Origin - YAxis) - Max);

            if(Edge0 > 0 &&
               Edge1 > 0 &&
               Edge2 > 0 &&
               Edge3 > 0)
            {
                *Pixel++ = Color32;
            }
            
        }

        Pixel += Buffer->Pitch; 
    }
}

internal void
StartUp(state *State, controller_config *Config, commands *Commands, int32 ScreenWidth, int32 ScreenHeight)
{
    for(int32 ButtonTypeIndex = 0;
        ButtonTypeIndex <= ArrayCount(Commands->ButtonStates);
        ++ButtonTypeIndex)
    {
        Commands->ButtonStates[ButtonTypeIndex].ButtonType = (function)ButtonTypeIndex; 
    }
              
    Config->MoveUp = ARROW_UP;
    Config->MoveDown = ARROW_DOWN;
    Config->MoveLeft = ARROW_LEFT;
    Config->MoveRight = ARROW_RIGHT;
    Config->ActionUp = CTR;
    Config->ActionDown = SPACE;
    Config->ActionLeft = SHIFT;
    Config->ActionRight = ALT;
    Config->LeftShoulder = MIDDLECLICK;
    Config->RightShoulder = SPEED_BOOST;
    Config->LeftTrigger = RIGHTCLICK;
    Config->RightTrigger = LEFTCLICK;
    Config->LeftStick = WINDOWS;
    Config->RightStick = DEL;
    Config->Back = ESC;

    State->ScreenDim = {(real32)ScreenWidth, (real32)ScreenHeight};
    
    State->AButton = STBLoadMap("Abutton.png");
    State->BButton = STBLoadMap("BButton.png");
    State->XButton = STBLoadMap("XButton.png");
    State->YButton = STBLoadMap("YButton.png");
    State->StickUp = STBLoadMap("StickUp.png");
    State->StickDown = STBLoadMap("StickDown.png");
    State->StickRight = STBLoadMap("StickRight.png");
    State->StickLeft = STBLoadMap("StickLeft.png");
}

internal void
Update(state *State, controller_config *Config, game_input *Input, v2 MousePos,
       game_offscreen_buffer *Buffer)
{
    commands *Commands = &State->Commands;
    int32 ButtonComandsIndex = 0;
    Commands->MousePos = MousePos; 
    
    for(int ControllerIndex = 0;
        ControllerIndex < ArrayCount(Input->Controllers);
        ++ControllerIndex)
    {

        game_controller_input *Controller = GetController(Input, ControllerIndex);

        if(Controller->IsConnected)
        {
                
            v2 ddP = {};
            v2 ddPR = {};
            
            if(Controller->IsAnalog)
            {
                // NOTE(casey): Use analog movement tuning
                ddP = v2{Controller->StickAverageX, -Controller->StickAverageY};
                ddPR = v2{Controller->RightStickAverageX, -Controller->RightStickAverageY};
            }

            if(State->Mode == DEFAULT)
            {
                ButtonDown(Commands, &Controller->RightTrigger, Config->RightTrigger);
                ButtonUp(Commands, &Controller->RightTrigger, Config->RightTrigger);

                ButtonDown(Commands, &Controller->LeftTrigger, Config->LeftTrigger);
                ButtonUp(Commands, &Controller->LeftTrigger, Config->LeftTrigger);
            
                ButtonDown(Commands, &Controller->LeftShoulder, Config->LeftShoulder);
                ButtonUp(Commands, &Controller->LeftShoulder, Config->LeftShoulder);

                ButtonDown(Commands, &Controller->RightShoulder, Config->RightShoulder);
                ButtonUp(Commands, &Controller->RightShoulder, Config->RightShoulder);            
            
                ButtonDown(Commands, &Controller->ActionUp, Config->ActionUp);
                ButtonUp(Commands, &Controller->ActionUp, Config->ActionUp);

                ButtonDown(Commands, &Controller->ActionDown, Config->ActionDown);
                ButtonUp(Commands, &Controller->ActionDown, Config->ActionDown);

                ButtonDown(Commands, &Controller->ActionLeft, Config->ActionLeft);
                ButtonUp(Commands, &Controller->ActionLeft, Config->ActionLeft);

                ButtonDown(Commands, &Controller->ActionRight, Config->ActionRight);
                ButtonUp(Commands, &Controller->ActionRight, Config->ActionRight);
            
                ButtonDown(Commands, &Controller->MoveUp, Config->MoveUp);
                ButtonUp(Commands, &Controller->MoveUp, Config->MoveUp);

                ButtonDown(Commands, &Controller->MoveDown, Config->MoveDown);
                ButtonUp(Commands, &Controller->MoveDown, Config->MoveDown);
            
                ButtonDown(Commands, &Controller->MoveLeft, Config->MoveLeft);
                ButtonUp(Commands, &Controller->MoveLeft, Config->MoveLeft);

                ButtonDown(Commands, &Controller->MoveRight, Config->MoveRight);
                ButtonUp(Commands, &Controller->MoveRight, Config->MoveRight);
            
                ButtonDown(Commands, &Controller->LeftStick, Config->LeftStick);
                ButtonUp(Commands, &Controller->LeftStick, Config->LeftStick);

                ButtonDown(Commands, &Controller->RightStick, Config->RightStick);
                ButtonUp(Commands, &Controller->RightStick, Config->RightStick);

                ButtonDown(Commands, &Controller->Back, Config->Back);
                ButtonUp(Commands, &Controller->Back, Config->Back);
            
            
                real32 MouseSpeed = 1536;
                if(Commands->ButtonStates[SPEED_BOOST].ButtonData == DOWN ||
                   Commands->ButtonStates[SPEED_BOOST].ButtonData == HOLD)
                {
                    MouseSpeed *= 2; 
                }
            
                if(ddP.X || ddP.Y)
                {
                    MoveMouse(&Commands->MousePos, ddP, Input->dtForFrame, MousePos, MouseSpeed);
                }
                if(Controller->Start.EndedDown)
                {
                    // NOTE(barret): Menu Mode Here
                }

                if(Controller->RightStickAverageY > 0)
                {
                    Commands->ButtonStates[WHEEL].ButtonData = 1;
                  
                }
            
                if(Controller->RightStickAverageY < 0)
                {
                    Commands->ButtonStates[WHEEL].ButtonData = -1;
                                                
                }

                if(Controller->Start.EndedDown && Controller->Start.HalfTransitionCount == 1)
                {
                    State->Mode = TYPE;
                    State->IsModeInitialized = false; 
                    break;
                }
                
            }
            else if(State->Mode == TYPE)
            {
                if(Controller->RightTrigger.EndedDown && Controller->LeftTrigger.EndedDown)
                {
                    character_set Sets[4];
                    Sets[0].Set = "`[]|\"";
                    Sets[1].Set = "\\,;";
                    Sets[2].Set = "./>?";
                    Sets[3].Set = "    ";
                    PickDirection(&State->CharToType, ddP, Sets, Controller, 1);
                    
                }
                else if(Controller->RightTrigger.EndedDown)
                {
                    character_set Sets[4];
                    Sets[0].Set = "qwer";
                    Sets[1].Set = "asdf";
                    Sets[2].Set = "uiop";
                    Sets[3].Set = "jkl";
                    PickDirection(&State->CharToType, ddP, Sets, Controller, 1);
                }
                else if(Controller->LeftTrigger.EndedDown)
                {
                    character_set Sets[4];
                    Sets[0].Set = "zxcv";
                    Sets[1].Set = "bnm";
                    Sets[2].Set = "tygh";
                    PickDirection(&State->CharToType, ddP, Sets, Controller, 1);
                    
                }
                else if(Controller->RightShoulder.EndedDown)
                {
                    character_set Sets[4];
                    Sets[0].Set = "QWER";
                    Sets[1].Set = "ASDF";
                    Sets[2].Set = "UIOP";
                    Sets[3].Set = "JKL";
                    PickDirection(&State->CharToType, ddP, Sets, Controller, 1);
                    
                }
                else if(Controller->LeftShoulder.EndedDown)
                {
                    character_set Sets[4];
                    Sets[0].Set = "ZXCV";
                    Sets[1].Set = "MBN";
                    Sets[2].Set = "TYGH";
                    PickDirection(&State->CharToType, ddP, Sets, Controller, 1);
                    
                }
                else if(Controller->RightShoulder.EndedDown && Controller->LeftTrigger.EndedDown)
                {
                    character_set Sets[4];
                    Sets[0].Set = "1234";
                    Sets[1].Set = "5678";
                    Sets[2].Set = "90-=";
                    Sets[3].Set = "*+_&";
                    PickDirection(&State->CharToType, ddP, Sets, Controller, 1);
                    
                }
                else if(Controller->LeftShoulder.EndedDown && Controller->RightTrigger.EndedDown)
                {
                    character_set Sets[4];
                    Sets[0].Set = ")(^%";
                    Sets[1].Set = "$#@!";
                    Sets[2].Set = "~<>?";
                    Sets[3].Set = ":{};";
                    PickDirection(&State->CharToType, ddP, Sets, Controller, 1);
                     
                }
                
                if(Controller->Start.EndedDown && Controller->Start.HalfTransitionCount == 1)
                {
                    State->Mode = DEFAULT;
                    State->IsModeInitialized = false; 
                    break;                  
                }
                
                // TODO(barret): draw interface for type mode 
                DrawRectangle(Buffer, v2{0, 0}, v2{(real32)Buffer->Width, (real32)Buffer->Height},
                              1.0f, 1.0f, 1.0f);

                DrawBitmap(Buffer, &State->AButton, 100, 100);
                DrawBitmap(Buffer, &State->BButton, 200, 100);
                DrawBitmap(Buffer, &State->XButton, 300, 100);
                DrawBitmap(Buffer, &State->YButton, 400, 100);
                DrawBitmap(Buffer, &State->StickUp, 500, 100);

#if 1
                State->Time += Input->dtForFrame;
                real32 Angle = 0.1f*State->Time;
                real32 Disp = 100.0f*Cos(5.0f*Angle);
                // NOTE(barret): Testing render stuff
                
                v2 Origin = v2{600, 100};
                v2 XAxis = 100.0f*V2(Cos(Angle), Sin(Angle));
                v2 YAxis = Perp(XAxis);
                v4 Color = {0.5f+0.5f*Sin(Angle), 0.5f+0.5f*Sin(2.9f*Angle), 0.5f+0.5f*Cos(9.9f*Angle), 1};
                DrawBitmapSlowly(Buffer, &State->AButton, Origin, XAxis, YAxis,
                                 Color);
#endif 
            }
            else if(State->Mode == MENU)
            {
                
            }
        }    
    }


}
