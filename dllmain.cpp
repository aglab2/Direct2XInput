#include <windows.h>
#include "XInputEmulator.h"
#include "wind.h"

#define EXPORT_API extern "C" __declspec(dllexport)

HMODULE gHModule;

#pragma comment(linker, "/EXPORT:XInputGetState=_XInputGetState@8")
#pragma comment(linker, "/EXPORT:XInputCancelGuideButtonWait=_XInputCancelGuideButtonWait@4")
#pragma comment(linker, "/EXPORT:XInputEnable=_XInputEnable@4")
#pragma comment(linker, "/EXPORT:XInputGetAudioDeviceIds=_XInputGetAudioDeviceIds@20")
#pragma comment(linker, "/EXPORT:XInputGetBaseBusInformation=_XInputGetBaseBusInformation@8")
#pragma comment(linker, "/EXPORT:XInputGetBatteryInformation=_XInputGetBatteryInformation@12")
#pragma comment(linker, "/EXPORT:XInputGetCapabilities=_XInputGetCapabilities@12")
#pragma comment(linker, "/EXPORT:XInputGetCapabilitiesEx=_XInputGetCapabilitiesEx@16")
#pragma comment(linker, "/EXPORT:XInputGetDSoundAudioDeviceGuids=_XInputGetDSoundAudioDeviceGuids@12")
#pragma comment(linker, "/EXPORT:XInputGetKeystroke=_XInputGetKeystroke@12")
#pragma comment(linker, "/EXPORT:XInputGetState=_XInputGetState@8")
#pragma comment(linker, "/EXPORT:XInputGetStateEx=_XInputGetStateEx@8")
#pragma comment(linker, "/EXPORT:XInputPowerOffController=_XInputPowerOffController@4")
#pragma comment(linker, "/EXPORT:XInputSetState=_XInputSetState@8")
#pragma comment(linker, "/EXPORT:XInputSetStateEx=_XInputSetStateEx@8")
#pragma comment(linker, "/EXPORT:XInputWaitForGuideButton=_XInputWaitForGuideButton@12")

EXPORT_API DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) WIN_NOEXCEPT
try
{
    return XInputEmulator::GetInstance().GetState(dwUserIndex, pState);
}
catch (...)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

EXPORT_API DWORD WINAPI XInputGetStateEx(DWORD dwUserIndex, XINPUT_STATE* pState) WIN_NOEXCEPT
try
{
    abort();
}
catch (...)
{

}

typedef struct _XINPUT_VIBRATION XINPUT_VIBRATION;
EXPORT_API DWORD WINAPI XInputSetState(DWORD dwUserIndex, const XINPUT_VIBRATION* pVibration) WIN_NOEXCEPT
{
    return 0;
}

EXPORT_API DWORD WINAPI XInputSetStateEx(DWORD dwUserIndex, const XINPUT_VIBRATION* pVibration) WIN_NOEXCEPT
{
    return 0;
}

EXPORT_API DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) WIN_NOEXCEPT
try
{
    return XInputEmulator::GetInstance().GetCapabilities(dwUserIndex, dwFlags, pCapabilities);
}
catch (...)
{
    return ERROR_DEVICE_NOT_CONNECTED;
}

typedef struct _XINPUT_CAPABILITIES_EX XINPUT_CAPABILITIES_EX;
EXPORT_API DWORD WINAPI XInputGetCapabilitiesEx(DWORD a1, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES_EX* pCapabilities) WIN_NOEXCEPT
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

typedef struct _XINPUT_BATTERY_INFORMATION XINPUT_BATTERY_INFORMATION;
EXPORT_API DWORD WINAPI XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation) WIN_NOEXCEPT
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

typedef struct _XINPUT_KEYSTROKE XINPUT_KEYSTROKE, * PXINPUT_KEYSTROKE;
EXPORT_API DWORD WINAPI XInputGetKeystroke(DWORD dwUserIndex,DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke) WIN_NOEXCEPT
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

typedef int XINPUT_LISTEN_STATE;
EXPORT_API DWORD WINAPI XInputWaitForGuideButton(DWORD dwUserIndex, DWORD dwFlags, XINPUT_LISTEN_STATE* pState)
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

EXPORT_API DWORD WINAPI XInputCancelGuideButtonWait(DWORD dwUserIndex)
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

EXPORT_API DWORD WINAPI XInputPowerOffController(DWORD dwUserIndex)
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

EXPORT_API void WINAPI XInputEnable(BOOL enable)
try
{
    abort();
}
catch (...)
{
}

struct XINPUT_BASE_BUS_INFORMATION
{
    WORD VendorId; //unknown
    WORD ProductId; //unknown
    WORD InputId; //unknown
    WORD Field_6; //unknown
    DWORD Field_8; //unknown
    BYTE Field_C; //unknown
    BYTE Field_D; //unknown
    BYTE Field_E; //unknown
    BYTE Field_F;//unknown
};

EXPORT_API DWORD WINAPI XInputGetBaseBusInformation(DWORD dwUserIndex, XINPUT_BASE_BUS_INFORMATION* pInfo)
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

EXPORT_API DWORD WINAPI XInputGetAudioDeviceIds(DWORD dwUserIndex, LPWSTR pRenderDeviceId, UINT* pRenderCount, LPWSTR pCaptureDeviceId, UINT* pCaptureCount) WIN_NOEXCEPT
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

EXPORT_API DWORD WINAPI XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid)
try
{
    abort();
}
catch (...)
{
    return ERROR_CALL_NOT_IMPLEMENTED;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        gHModule = hModule;
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
