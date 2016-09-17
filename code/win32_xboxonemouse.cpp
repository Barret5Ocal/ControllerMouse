/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Barret Gaylor $
   $Notice: (C) Copyright 2015 by Barret Gaylor. All Rights Reserved. $
   ======================================================================== */
#include <windows.h>
#include <xinput.h>
#include <stdio.h>
#include "win32_xboxonemouse.h"
#include "xboxonemouse.cpp"
#include "win32_xboxonemouse_simulate.cpp"

global_variable bool GlobalRunning;
global_variable int64 GlobalPerfCountFrequency;

// NOTE(casey): XInputGetState
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return(ERROR_DEVICE_NOT_CONNECTED);
}
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

// NOTE(casey): XInputSetState
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
    return(ERROR_DEVICE_NOT_CONNECTED);
}
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

internal void
Win32LoadXInput(void)    
{
    // TODO(casey): Test this on Windows 8
    HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");
    if(!XInputLibrary)
    {
        // TODO(casey): Diagnostic
        XInputLibrary = LoadLibraryA("xinput9_1_0.dll");
    }
    
    if(!XInputLibrary)
    {
        // TODO(casey): Diagnostic
        XInputLibrary = LoadLibraryA("xinput1_3.dll");
    }
    
    if(XInputLibrary)
    {
        XInputGetState = (x_input_get_state *)GetProcAddress(XInputLibrary, "XInputGetState");
        if(!XInputGetState) {XInputGetState = XInputGetStateStub;}

        XInputSetState = (x_input_set_state *)GetProcAddress(XInputLibrary, "XInputSetState");
        if(!XInputSetState) {XInputSetState = XInputSetStateStub;}

        // TODO(casey): Diagnostic

    }
    else
    {
        // TODO(casey): Diagnostic
    }
}

internal void
Win32ProcessXInputDigitalButton(DWORD XInputButtonState,
                                game_button_state *OldState, DWORD ButtonBit,
                                game_button_state *NewState)
{
    NewState->EndedDown = ((XInputButtonState & ButtonBit) == ButtonBit);
    NewState->HalfTransitionCount = (OldState->EndedDown != NewState->EndedDown) ? 1 : 0;
}

internal void
Win32ProcessXInputTriggerButton(BYTE ButtonValue, game_button_state *OldState,
                                game_button_state *NewState)
{
    NewState->EndedDown = (ButtonValue > 0) ? 1 : 0;
    NewState->HalfTransitionCount = (OldState->EndedDown != NewState->EndedDown) ? 1 : 0;
}

internal real32
Win32ProcessXInputStickValue(SHORT Value, SHORT DeadZoneThreshold)
{
    real32 Result = 0;

    if(Value < -DeadZoneThreshold)
    {
        Result = (real32)((Value + DeadZoneThreshold) / (32768.0f - DeadZoneThreshold));
    }
    else if(Value > DeadZoneThreshold)
    {
        Result = (real32)((Value - DeadZoneThreshold) / (32767.0f - DeadZoneThreshold));
    }

    return(Result);
}

inline LARGE_INTEGER
Win32GetWallClock(void)
{    
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return(Result);
}

inline real32
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
    real32 Result = ((real32)(End.QuadPart - Start.QuadPart) /
                     (real32)GlobalPerfCountFrequency);
    return(Result);
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
                        UINT Message,
                        WPARAM WParam,
                        LPARAM LParam)
{
    LRESULT Result = 0;

    switch(Message)
    {
        case WM_CLOSE:
        {
            // TODO(casey): Handle this with a message to the user?
            GlobalRunning = false;
        } break;

        case WM_SETCURSOR:
        {
            
        }break;
        
        case WM_ACTIVATEAPP:
        {
#if 0
            if(WParam == TRUE)
            {
                SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 255, LWA_ALPHA);
            }
            else
            {
                SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 64, LWA_ALPHA);
            }
#endif
        } break;

        case WM_DESTROY:
        {
            // TODO(casey): Handle this as an error - recreate window?
            GlobalRunning = false;
        } break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32 VKCode = (uint32)WParam;

            // NOTE(casey): Since we are comparing WasDown to IsDown,
            // we MUST use == and != to convert these bit tests to actual
            // 0 or 1 values.
            bool32 WasDown = ((LParam & (1 << 30)) != 0);
            bool32 IsDown = ((LParam & (1 << 31)) == 0);
            if(WasDown != IsDown)
            {

                if(IsDown)
                {
                    bool32 AltKeyWasDown = (LParam & (1 << 29));
                    if((VKCode == VK_F4) && AltKeyWasDown)
                    {
                        GlobalRunning = false;
                    }
                   
                }
            }

        } break;
       
        default:
        {
//            OutputDebugStringA("default\n");
            Result = DefWindowProcA(Window, Message, WParam, LParam);
        } break;
    }

    return(Result);
}

internal void
Win32InitializeMenu(HWND Window)
{
    SetWindowLong(Window, GWL_EXSTYLE, 0);//WS_EX_TOPMOST|WS_EX_LAYERED);
    //SetWindowPos(Window, HWND_TOPMOST, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, SWP_SHOWWINDOW);
    ShowWindow(Window, SW_SHOW);
}

int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CmdLine,
        int ShowCode)
{
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;

    UINT DesiredSchedulerMS = 1;
    bool32 SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);
    
    WNDCLASSA WindowClass = {};
    
    WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "XboxOneMouse";
    if(RegisterClassA(&WindowClass))
    {
        HWND Window =
            CreateWindowExA(
                WS_EX_TOPMOST|WS_EX_LAYERED|WS_EX_TRANSPARENT,
                WindowClass.lpszClassName,
                "XboxOneMouse",
                //WS_OVERLAPPEDWINDOW|
                WS_VISIBLE|WS_POPUP,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                Instance,
                0);
        if(Window)
        {
            int MonitorRefreshHz = 60;
            HDC RefreshDC = GetDC(Window);
            int Win32RefreshRate = GetDeviceCaps(RefreshDC, VREFRESH);
            ReleaseDC(Window, RefreshDC);
            if(Win32RefreshRate > 1)
            {
                MonitorRefreshHz = Win32RefreshRate;
            }
            real32 GameUpdateHz = (MonitorRefreshHz / 2.0f);
            real32 TargetSecondsPerFrame = 1.0f / (real32)GameUpdateHz;
            
            Win32LoadXInput();
            GlobalRunning = true;
 
            game_input Input[2] = {};
            game_input *NewInput = &Input[0];
            game_input *OldInput = &Input[1];

            state State = {};
            commands *Commands = &State.Commands;
            for(int32 ButtonTypeIndex = 0;
                ButtonTypeIndex <= ArrayCount(Commands->ButtonStates);
                ++ButtonTypeIndex)
            {
                Commands->ButtonStates[ButtonTypeIndex].ButtonType = (function)ButtonTypeIndex; 
            }

            controller_config *Config = &State.Config;            
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
            
            //ShowWindow(Window, SW_SHOW);
            
            LARGE_INTEGER LastCounter = Win32GetWallClock();
            LARGE_INTEGER FlipWallClock = Win32GetWallClock();

            uint64 LastCycleCount = __rdtsc();

            
            while(GlobalRunning)
            {
                NewInput->dtForFrame = TargetSecondsPerFrame;
                
                MSG Message; 
                while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }
                
                // TODO(casey): Need to not poll disconnected controllers to avoid
                // xinput frame rate hit on older libraries...
                // TODO(casey): Should we poll this more frequently
                DWORD MaxControllerCount = XUSER_MAX_COUNT;
                if(MaxControllerCount > (ArrayCount(NewInput->Controllers) - 1))
                {
                    MaxControllerCount = (ArrayCount(NewInput->Controllers) - 1);
                }
                
                for (DWORD ControllerIndex = 0;
                     ControllerIndex < MaxControllerCount;
                     ++ControllerIndex)
                {
                    DWORD OurControllerIndex = ControllerIndex + 1;
                    game_controller_input *OldController = GetController(OldInput, OurControllerIndex);
                    game_controller_input *NewController = GetController(NewInput, OurControllerIndex);
                    
                    XINPUT_STATE ControllerState;
                    if(XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
                    {
                        NewController->IsConnected = true;
                        NewController->IsAnalog = OldController->IsAnalog;
                           
                        // NOTE(casey): This controller is plugged in
                        // TODO(casey): See if ControllerState.dwPacketNumber increments too rapidly
                        XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

                        // TODO(casey): This is a square deadzone, check XInput to
                        // verify that the deadzone is "round" and show how to do
                        // round deadzone processing.
                        NewController->StickAverageX = Win32ProcessXInputStickValue(
                            Pad->sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                        NewController->StickAverageY = Win32ProcessXInputStickValue(
                            Pad->sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                        NewController->RightStickAverageX = Win32ProcessXInputStickValue(
                            Pad->sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
                        NewController->RightStickAverageY = Win32ProcessXInputStickValue(
                            Pad->sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

                        
                        if((NewController->StickAverageX != 0.0f) ||
                           (NewController->StickAverageY != 0.0f))
                        {
                            NewController->IsAnalog = true;
                        }
#if 0
                        if(Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP)
                        {
                            NewController->StickAverageY = 1.0f;
                            NewController->IsAnalog = false;
                        }
                            
                        if(Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
                        {
                            NewController->StickAverageY = -1.0f;
                            NewController->IsAnalog = false;
                        }
                            
                        if(Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
                        {
                            NewController->StickAverageX = -1.0f;
                            NewController->IsAnalog = false;
                        }
                            
                        if(Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
                        {
                            NewController->StickAverageX = 1.0f;
                            NewController->IsAnalog = false;
                        }

                        
                        real32 Threshold = 0.5f;
                        Win32ProcessXInputDigitalButton(
                            (NewController->StickAverageX < -Threshold) ? 1 : 0,
                            &OldController->MoveLeft, 1,
                            &NewController->MoveLeft);
                        Win32ProcessXInputDigitalButton(
                            (NewController->StickAverageX > Threshold) ? 1 : 0,
                            &OldController->MoveRight, 1,
                            &NewController->MoveRight);
                        Win32ProcessXInputDigitalButton(
                            (NewController->StickAverageY < -Threshold) ? 1 : 0,
                            &OldController->MoveDown, 1,
                            &NewController->MoveDown);
                        Win32ProcessXInputDigitalButton(
                            (NewController->StickAverageY > Threshold) ? 1 : 0,
                            &OldController->MoveUp, 1,
                            &NewController->MoveUp);
#endif
                        
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->MoveDown, XINPUT_GAMEPAD_DPAD_DOWN,
                                                        &NewController->MoveDown);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->MoveRight, XINPUT_GAMEPAD_DPAD_RIGHT,
                                                        &NewController->MoveRight);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->MoveLeft, XINPUT_GAMEPAD_DPAD_LEFT,
                                                        &NewController->MoveLeft);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->MoveUp, XINPUT_GAMEPAD_DPAD_UP,
                                                        &NewController->MoveUp);
                        
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->ActionDown, XINPUT_GAMEPAD_A,
                                                        &NewController->ActionDown);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->ActionRight, XINPUT_GAMEPAD_B,
                                                        &NewController->ActionRight);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->ActionLeft, XINPUT_GAMEPAD_X,
                                                        &NewController->ActionLeft);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->ActionUp, XINPUT_GAMEPAD_Y,
                                                        &NewController->ActionUp);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->LeftShoulder, XINPUT_GAMEPAD_LEFT_SHOULDER,
                                                        &NewController->LeftShoulder);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->RightShoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER,
                                                        &NewController->RightShoulder);

                        Win32ProcessXInputTriggerButton(Pad->bRightTrigger, &OldController->RightTrigger,
                                                        &NewController->RightTrigger);
                        Win32ProcessXInputTriggerButton(Pad->bLeftTrigger, &OldController->LeftTrigger,
                                                        &NewController->LeftTrigger);

                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->LeftStick, XINPUT_GAMEPAD_LEFT_THUMB,
                                                        &NewController->LeftStick);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->RightStick, XINPUT_GAMEPAD_RIGHT_THUMB,
                                                        &NewController->RightStick);

                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->Start, XINPUT_GAMEPAD_START,
                                                        &NewController->Start);
                        Win32ProcessXInputDigitalButton(Pad->wButtons,
                                                        &OldController->Back, XINPUT_GAMEPAD_BACK,
                                                        &NewController->Back);
                    }
                    else
                    {
                        // NOTE(casey): The controller is not available
                        NewController->IsConnected = false;
                    }
                }
                
                POINT Point = {};
                GetCursorPos(&Point);

                v2 MousePos = {(real32)Point.x, (real32)Point.y};
                
                Update(&State, Config, NewInput, MousePos);
                
                SetCursorPos(Commands->MousePos.X, Commands->MousePos.Y);
                GetCursorPos(&Point);

                if(State.Mode == DEFAULT)
                {
                    Win32RunDefaultSims(Commands);
                }

                if(State.Mode == TYPE)
                {
                    if(State.CharToType != 0)
                        Win32RunTypeSims(State.CharToType);

                    State.CharToType = 0;
                }
                
                if(State.Mode == MENU)
                {
                    if(State.ModeData == 1)
                    {
                        Win32InitializeMenu(Window);
                        State.ModeData = 0;
                    }

                    
                }

                LARGE_INTEGER WorkCounter = Win32GetWallClock();
                real32 WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);

                real32 SecondsElapsedForFrame = WorkSecondsElapsed;
                if(SecondsElapsedForFrame < TargetSecondsPerFrame)
                {                        
                    if(SleepIsGranular)
                    {
                        DWORD SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame -
                                                           SecondsElapsedForFrame));
                        if(SleepMS > 0)
                        {
                            Sleep(SleepMS);
                        }
                    }
                        
                    real32 TestSecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter,
                                                                               Win32GetWallClock());
                    if(TestSecondsElapsedForFrame < TargetSecondsPerFrame)
                    {
                        // TODO(casey): LOG MISSED SLEEP HERE
                    }
                        
                    while(SecondsElapsedForFrame < TargetSecondsPerFrame)
                    {                            
                        SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter,
                                                                        Win32GetWallClock());
                    }
                }
                else
                {
                    // TODO(casey): MISSED FRAME RATE!
                    // TODO(casey): Logging
                }
                
                LARGE_INTEGER EndCounter = Win32GetWallClock();
                real32 MSPerFrame = 1000.0f*Win32GetSecondsElapsed(LastCounter, EndCounter);                    
                LastCounter = EndCounter;
                
                FlipWallClock = Win32GetWallClock();
                  
                game_input *Temp = NewInput;
                NewInput = OldInput;
                OldInput = Temp;

                uint64 EndCycleCount = __rdtsc();
                uint64 CyclesElapsed = EndCycleCount - LastCycleCount;
                LastCycleCount = EndCycleCount;

                real64 FPS = 0.0f;
                real64 MCPF = ((real64)CyclesElapsed / (1000.0f * 1000.0f));

                char FPSBuffer[256];
                _snprintf_s(FPSBuffer, sizeof(FPSBuffer),
                            "%.02fms/f,  %.02ff/s,  %.02fmc/f\n", MSPerFrame, FPS, MCPF);
                OutputDebugStringA(FPSBuffer);
            }
        }
    }
    return(0);
    
}
