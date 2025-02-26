#pragma once
#include <Windows.h>
#include <map>
#include <string>
#include <vector>

struct ButtonMapping
{
    int dinputOffset;
    WORD xboxMask;
};

enum class DInputAxisType : BYTE
{
    // from 0 to 0x10000, clamped
    FULL,
    // from 0x8000 to 0x10000, clamped
    POSITIVE,
    // from 0 to 0x8000, clamped
    NEGATIVE,
};

enum class XInputAxisType
{
    // 0 to 0xff
    BYTE,
    // -0x8000 to 0x7fff
    SHORT,
};

struct DInputAxis
{
    int offset;
    DInputAxisType type;
    bool inverted;
};

struct XInputAxis
{
    int offset;
    XInputAxisType type;
};

struct AxisMapping
{
    DInputAxis dinput;
    XInputAxis xbox;
};

struct Mapping
{
    std::vector<ButtonMapping> buttons;
    std::vector<AxisMapping> axis;
};

class ConfigLoader
{
public:
    ConfigLoader(const char* filePath);
    
    const std::string& name() const
    { return name_; }
    const Mapping& mapping() const
    { return mapping_; }

private:
    static int iniHandler(void* user, const char* section, const char* name, const char* value);
    void onValue(const char* section, const char* name, const char* value);

    std::string name_;
    Mapping mapping_;
};
