#include "XInputEmulator.h"

#include "log.h"
#include "wind.h"

#include <filesystem>
#include <optional>
#include <string_view>

#include <Xinput.h>

extern HMODULE gHModule;

XInputEmulator::XInputEmulator()
{
    auto cfgPath = dllPath() / "d2x.ini";

    ConfigLoader config(cfgPath.u8string().c_str());
    currentMapping_ = config.mapping();
    struct Ctx
    {
        XInputEmulator& self;
        const std::string& deviceName;
    }
    ctx
    {
        *this, config.name(),
    };

    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&dinput_, NULL);
    dinput_->EnumDevices(DI8DEVCLASS_GAMECTRL, [](LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef) 
    {
        Ctx& ctx = *(Ctx*)pvRef;
        XInputEmulator& self = ctx.self;
        std::string_view deviceName = lpddi->tszProductName;
        IPRINTF("Found device '%s'", lpddi->tszProductName);
        if (deviceName == ctx.deviceName)
        {
            IPRINTF("Device matched, opening");
            self.dinput_->CreateDevice(lpddi->guidInstance, &self.gamepad_, NULL);
            self.gamepad_->SetDataFormat(&c_dfDIJoystick2);
            self.gamepad_->Acquire();
            return DIENUM_STOP;
        }

        return DIENUM_CONTINUE;
    }, &ctx, DIEDFL_ATTACHEDONLY);

    if (!gamepad_ || !dinput_)
        WPRINTF("Failed to find a matching device matching '%s'", config.name().c_str());

    logFlush();
}

XInputEmulator::~XInputEmulator()
{
    if (gamepad_)
    {
        gamepad_->Unacquire();
        gamepad_->Release();
    }

    if (dinput_)
    {
        dinput_->Release();
    }
}

DIJOYSTATE2 XInputEmulator::PollDirectInput()
{
    DIJOYSTATE2 state{ };
    if (gamepad_)
    {
        gamepad_->Poll();
        int err = gamepad_->GetDeviceState(sizeof(DIJOYSTATE2), &state);
        int a = 0; 
    }

    return state;
}

DWORD XInputEmulator::GetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
    if (0 != dwUserIndex)
        return ERROR_DEVICE_NOT_CONNECTED;

    pState->dwPacketNumber = seqNum_++;
    pState->Gamepad = PollGamepad();

    if (overrideWithPhonyAmount_)
    {
        overrideWithPhonyAmount_--;
        pState->Gamepad.wButtons = ~0;
    }

    return ERROR_SUCCESS;
}

DWORD XInputEmulator::GetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities)
{
    if (0 != dwUserIndex)
        return ERROR_DEVICE_NOT_CONNECTED;

    pCapabilities->Type = XINPUT_DEVTYPE_GAMEPAD;
    pCapabilities->SubType = XINPUT_DEVSUBTYPE_GAMEPAD;
    pCapabilities->Flags = 0;
    pCapabilities->Gamepad = PollGamepad();
    pCapabilities->Vibration = {};

    return ERROR_SUCCESS;
}

static std::optional<long> readDInputAxis(const DIJOYSTATE2& state, const DInputAxis& axis)
{
    // scaled from 0 to 0xffff
    long value;
    LONG dinputRaw = *(const LONG*)(((const char*)&state) + axis.offset);
    switch (axis.type)
    {
    case DInputAxisType::FULL:
        value = std::clamp(dinputRaw, 0L, 0xffffL);
        break;
    case DInputAxisType::POSITIVE:
        if (dinputRaw < 0x8000)
            return std::nullopt;

        value = (dinputRaw - 0x8000) * 2;
        if (value > 0xffff)
            value = 0xffff;

        break;
    case DInputAxisType::NEGATIVE:
        if (dinputRaw > 0x8000)
            return std::nullopt;

        value = (0x7fff - dinputRaw) * 2;
        if (value > 0xffff)
            value = 0xffff;

        break;
    }

    if (axis.inverted)
    {
        value = 0xffff - value;
    }

    return value;
}

static void writeXInputAxis(long value, XINPUT_GAMEPAD& gamepad, const XInputAxis& axis)
{
    void* xinputRaw = ((char*)&gamepad) + axis.offset;
    switch (axis.type)
    {
    case XInputAxisType::BYTE:
        // Rescale to 0 to 0xff
        *(BYTE*)xinputRaw = (BYTE)(value >> 8);
        break;
    case XInputAxisType::SHORT:
        // Rescale to -0x8000 to 0x7fff
        *(SHORT*)xinputRaw = (SHORT)(value - 0x8000);
        break;
    }
}

XINPUT_GAMEPAD XInputEmulator::PollGamepad()
{
    XINPUT_GAMEPAD gamepad{};

    DIJOYSTATE2 state = PollDirectInput();
    for (const auto& btn : currentMapping_.buttons)
    {
        if (state.rgbButtons[btn.dinputOffset] & 0x80)
            gamepad.wButtons |= btn.xboxMask;
    }
    for (const auto& axis : currentMapping_.axis)
    {
        if (auto value = readDInputAxis(state, axis.dinput))
        {
            writeXInputAxis(*value, gamepad, axis.xbox);
        }
    }
    for (const auto& b2a : currentMapping_.button2axis)
    {
        if (state.rgbButtons[b2a.dinputOffset] & 0x80)
            writeXInputAxis(0xffff, gamepad, b2a.xbox);
    }

    return gamepad;
}
