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
Update(state *State, controller_config *Config, game_input *Input, v2 MousePos)
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
                    //State->ModeData = 1;
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
                    //State->ModeData = 1;                     
                }
            }
            else if(State->Mode == MENU)
            {
                
            }
        }    
    }
}
