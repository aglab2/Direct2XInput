#pragma once
#include <windows.h>
#include <dinput.h>
#include "ConfigLoader.h"


typedef struct _XINPUT_CAPABILITIES XINPUT_CAPABILITIES;
typedef struct _XINPUT_GAMEPAD XINPUT_GAMEPAD;
typedef struct _XINPUT_STATE XINPUT_STATE;

class XInputEmulator {
public:
    static XInputEmulator& GetInstance()
    {
        static XInputEmulator instance;
        return instance;
    }

    DWORD GetState(DWORD dwUserIndex, XINPUT_STATE* pState);
    DWORD GetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities);

private:
    XInputEmulator();
    ~XInputEmulator();

    XINPUT_GAMEPAD PollGamepad();

    LPDIRECTINPUT8 dinput_ = nullptr;
    LPDIRECTINPUTDEVICE8 gamepad_ = nullptr;
    Mapping currentMapping_;
    DWORD seqNum_ = 0;
    int overrideWithPhonyAmount_ = 2;

    DIJOYSTATE2 PollDirectInput();
};
