#pragma once

#include <Windows.h>

#include <filesystem>

extern HMODULE gHModule;

static inline std::filesystem::path dllPath()
{
    char path[MAX_PATH];
    DWORD len = GetModuleFileName(gHModule, path, sizeof(path));
    std::filesystem::path dllPath{ std::string_view{ path, len } };
    return dllPath.parent_path();
}
