/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Barret Gaylor $
   $Notice: (C) Copyright 2015 by Barret Gaylor. All Rights Reserved. $
   ======================================================================== */

internal void
Win32SimulateMouseInput(DWORD Flags, button_state *BC, button_setting Set)
{
    INPUT Input={};                                         
    Input.type = INPUT_MOUSE;                               
    Input.mi.dwFlags = Flags;                
    SendInput(1, &Input, sizeof(INPUT));
    BC->ButtonData = Set;                            
}

internal void
Win32SimulateKeyboardInput(WORD VK, DWORD Flags, button_state *BC, button_setting Set)
{
    
    INPUT Input = {};
    Input.type = INPUT_KEYBOARD;
    Input.ki.wVk = VK;
    Input.ki.dwFlags = Flags;
    SendInput(1, &Input, sizeof(INPUT));
    BC->ButtonData = Set;

}

internal void
Win32DoMouseButton(button_state *BC, DWORD FlagDown, DWORD FlagUp)
{
    if(BC->ButtonData == 1)
    {
        Win32SimulateMouseInput(FlagDown, BC, HOLD);
    }
    else if(BC->ButtonData == 3)
    {
        Win32SimulateMouseInput(FlagUp, BC, NONE);
    } 
    
}

global_variable int32 GlobalKeyHoldCount = 0;

internal
void Win32DoKeyboardButton(button_state *BC, WORD VKCode)
{
    if(BC->ButtonData == 1 )
    {
        Win32SimulateKeyboardInput(VKCode, 0, BC, HOLD);
        //OutputDebugStringA("pressed\n");
        ++GlobalKeyHoldCount; 

    }
    else if(BC->ButtonData == 2)
    {
        ++GlobalKeyHoldCount;
        if(GlobalKeyHoldCount >= 15)
        {
            Win32SimulateKeyboardInput(VKCode, 0, BC, HOLD);            
        }
    }
    else if(BC->ButtonData == 3)
    {
        Win32SimulateKeyboardInput(VKCode, KEYEVENTF_KEYUP, BC, NONE);
        //OutputDebugStringA("released\n");
        GlobalKeyHoldCount = 0; 
    }

}

internal void
Win32RunDefaultSims(commands *Commands)
{
    //commands *Commands = *Commands2; 
    for(int ButtonCommandIndex = 0;
        ButtonCommandIndex <= ArrayCount(Commands->ButtonStates);
        ButtonCommandIndex++)
    {
        button_state *BC = &Commands->ButtonStates[ButtonCommandIndex];

        switch(BC->ButtonType)
        {
            case LEFTCLICK:
            {
                Win32DoMouseButton(BC, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);

            }break;
            case RIGHTCLICK:
            {
                Win32DoMouseButton(BC, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
            }break;
            case MIDDLECLICK:
            {
                Win32DoMouseButton(BC, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);
            }break;
            case WHEEL:
            {
                if(BC->ButtonData > 0)
                {
                    INPUT Input={};                                         
                    Input.type = INPUT_MOUSE;                               
                    Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
                    Input.mi.mouseData = 120;
                    SendInput(1, &Input, sizeof(INPUT));
                    BC->ButtonData = 0;
                }
                else if(BC->ButtonData < 0)
                {
                    INPUT Input={};                                         
                    Input.type = INPUT_MOUSE;                               
                    Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
                    Input.mi.mouseData = -120;
                    SendInput(1, &Input, sizeof(INPUT));
                    BC->ButtonData = 0; 
                }
            }break;
            case CTR:
            {
                Win32DoKeyboardButton(BC, VK_CONTROL);
                
            }break;
            case SPACE:
            {
                Win32DoKeyboardButton(BC, VK_SPACE);
            }break;
            case SHIFT:
            {
                Win32DoKeyboardButton(BC, VK_LSHIFT);
            }break;
            case ALT:
            {
                Win32DoKeyboardButton(BC, VK_MENU);
            }break;
            case ESC:
            {
                Win32DoKeyboardButton(BC, VK_ESCAPE);
                
            }break;
            case TAB:
            {
                Win32DoKeyboardButton(BC, VK_TAB);
                
            }break;
            case CAPS_LOCK:
            {
                Win32DoKeyboardButton(BC, VK_CAPITAL);
                
            }break;
            case RETURN:
            {
                Win32DoKeyboardButton(BC, VK_RETURN);

            }break;
            case BACKSPACE:
            {
                Win32DoKeyboardButton(BC, VK_BACK);
                
            }break;
            case MUTE:
            {
                Win32DoKeyboardButton(BC, VK_VOLUME_MUTE);
            }break;
            case VOLUME_UP:
            {
                Win32DoKeyboardButton(BC, VK_VOLUME_UP);
                
            }break;
            case VOLUME_DOWN:
            {
                Win32DoKeyboardButton(BC, VK_VOLUME_DOWN);
              
            }break;
            case WINDOWS:
            {
                Win32DoKeyboardButton(BC, VK_LWIN);

            }break;
            case DEL:
            {
                Win32DoKeyboardButton(BC, VK_DELETE);
              
            }break;
            case HOME:
            {
                Win32DoKeyboardButton(BC, VK_HOME);
                
            }break;
            case END:
            {
                Win32DoKeyboardButton(BC, VK_END);
                
            }break;
            case PG_UP:
            {
                Win32DoKeyboardButton(BC, VK_PRIOR);
                
            }break;
            case PG_DOWN:
            {
                Win32DoKeyboardButton(BC, VK_NEXT);
                
            }break;
            case INSERT:
            {
                Win32DoKeyboardButton(BC, VK_INSERT);
                
            }break;
            case ARROW_UP:
            {
                Win32DoKeyboardButton(BC, VK_UP);
                
            }break;
            case ARROW_DOWN:
            {
                Win32DoKeyboardButton(BC, VK_DOWN);
                
            }break;
            case ARROW_RIGHT:
            {
                Win32DoKeyboardButton(BC, VK_RIGHT);
                
            }break;
            case ARROW_LEFT:
            {
                Win32DoKeyboardButton(BC, VK_LEFT);
                
            }break;
            default:
            {
                            
            }break;
        }
    }
}

internal void 
Win32RunTypeSims(char CharToType)
{
    uint32 VKCode = CharToType; 

    INPUT Input = {};
    Input.type = INPUT_KEYBOARD;
    Input.ki.wVk = 0;
    Input.ki.wScan = CharToType; 
    Input.ki.dwFlags = KEYEVENTF_UNICODE;
    SendInput(1, &Input, sizeof(INPUT));

    /*
    INPUT Input = {};
    Input.type = INPUT_KEYBOARD;
    Input.ki.wVk = VKCode;
    Input.ki.dwFlags = 0;
    SendInput(1, &Input, sizeof(INPUT));
    
    Input = {}; 
    Input.type = INPUT_KEYBOARD;
    Input.ki.wVk = VKCode;
    Input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &Input, sizeof(INPUT));
    */    
}
