#include "ConfigLoader.h"

#include <iostream>
#include <fstream>
#include <optional>

#include "ini.h"
#include "log.h"

#include <Xinput.h>
#include <dinput.h>

int ConfigLoader::iniHandler(void* user, const char* section, const char* name, const char* value)
{
	if (!name)  // Happens when INI_CALL_HANDLER_ON_NEW_SECTION enabled
		return 1;

	ConfigLoader* self = static_cast<ConfigLoader*>(user);
	self->onValue(section, name, value);
	return 1;
}

static std::optional<WORD> decodeAsXInputButton(const std::string_view& name)
{
	if ("A"				 == name) return XINPUT_GAMEPAD_A;
	if ("B"			     == name) return XINPUT_GAMEPAD_B;
	if ("X"              == name) return XINPUT_GAMEPAD_X;
	if ("Y"              == name) return XINPUT_GAMEPAD_Y;
	if ("DPad Up"        == name) return XINPUT_GAMEPAD_DPAD_UP;
	if ("DPad Down"      == name) return XINPUT_GAMEPAD_DPAD_DOWN;
	if ("DPad Left"	     == name) return XINPUT_GAMEPAD_DPAD_LEFT;
	if ("DPad Right"	 == name) return XINPUT_GAMEPAD_DPAD_RIGHT;
	if ("Start"		     == name) return XINPUT_GAMEPAD_START;
	if ("Back"			 == name) return XINPUT_GAMEPAD_BACK;
	if ("Left Thumb"     == name) return XINPUT_GAMEPAD_LEFT_THUMB;
	if ("Right Thumb"	 == name) return XINPUT_GAMEPAD_RIGHT_THUMB;
	if ("Left Shoulder"  == name) return XINPUT_GAMEPAD_LEFT_SHOULDER;
	if ("Right Shoulder" == name) return XINPUT_GAMEPAD_RIGHT_SHOULDER;

	return std::nullopt;
}

static std::optional<XInputAxis> decodeAsXInputAxis(const std::string_view& name)
{
	if ("Left Trigger"  == name) return XInputAxis{ offsetof(XINPUT_GAMEPAD, bLeftTrigger) , XInputAxisType::BYTE };
	if ("Right Trigger" == name) return XInputAxis{ offsetof(XINPUT_GAMEPAD, bRightTrigger), XInputAxisType::BYTE };
	if ("Left Thumb X"  == name) return XInputAxis{ offsetof(XINPUT_GAMEPAD, sThumbLX)     , XInputAxisType::SHORT };
	if ("Left Thumb Y"  == name) return XInputAxis{ offsetof(XINPUT_GAMEPAD, sThumbLY)     , XInputAxisType::SHORT };
	if ("Right Thumb X" == name) return XInputAxis{ offsetof(XINPUT_GAMEPAD, sThumbRX)     , XInputAxisType::SHORT };
	if ("Right Thumb Y" == name) return XInputAxis{ offsetof(XINPUT_GAMEPAD, sThumbRY)	   , XInputAxisType::SHORT };

	return std::nullopt;
}

static std::optional<int> decodeAsDInputOffset(const std::string_view& name)
{
	if ("X Axis" == name)     return offsetof(DIJOYSTATE2, lX);
	if ("Y Axis" == name)	  return offsetof(DIJOYSTATE2, lY);
	if ("Z Axis" == name)	  return offsetof(DIJOYSTATE2, lZ);
	if ("X Rotation" == name) return offsetof(DIJOYSTATE2, lRx);
	if ("Y Rotation" == name) return offsetof(DIJOYSTATE2, lRy);
	if ("Z Rotation" == name) return offsetof(DIJOYSTATE2, lRz);

	return std::nullopt;
}

static std::optional<DInputAxisType> decodeAsDInputAxisType(const std::string_view& name)
{
	if ("Up" == name)			 return DInputAxisType::POSITIVE;
	if ("Down" == name)			 return DInputAxisType::NEGATIVE;

	return std::nullopt;
}

static bool decodeAsInverted(const std::string_view& name)
{
	return "Inverted" == name;
}

static std::optional<DInputAxis> decodeAsDInputAxis(std::string_view name)
{
	DInputAxis axis{};
	while (true)
	{
		// try to check if name is full
		if (auto offset = decodeAsDInputOffset(name))
		{
			axis.offset = *offset;
			return axis;
		}

		// try to split by last space so stuff like "X Axis Up" splits to "X Axis" and "Up"
		auto lastSpace = name.rfind(' ');
		if (-1 == lastSpace)
			return std::nullopt;

		auto typeName = name.substr(lastSpace + 1);
		if (auto type = decodeAsDInputAxisType(typeName))
			axis.type = *type;
		else if (decodeAsInverted(typeName))
			axis.inverted = true;
		else
			return std::nullopt;

		name = name.substr(0, lastSpace);
	}
}

void ConfigLoader::onValue(const char* section, const char* name, const char* value)
{
	static const std::string_view Section = "Controller";
	if (Section != section)
		return;

	std::string_view nameStr{ name };
	if (nameStr == "name")
	{
		name_ = value;
	}
	else if (auto button = decodeAsXInputButton(nameStr))
	{
		int off = atoi(value);
		if (off <= 0 || off >= sizeof(DIJOYSTATE2::rgbButtons))
			WPRINTF("Failed to decode DInput button '%s' corresponding to XInput button '%s'", name, value);
		else
			mapping_.buttons.push_back({ off - 1, *button });
	}
	else if (auto xAxis = decodeAsXInputAxis(nameStr))
	{
		if (auto dAxis = decodeAsDInputAxis(value))
			mapping_.axis.push_back({ *dAxis, *xAxis });
		else
			WPRINTF("Failed to decode DInput axis '%s' corresponding to XInput axis '%s'", name, value);
	}
	else
	{
		WPRINTF("Failed to decode name '%s'", name);
	}
}

ConfigLoader::ConfigLoader(const char* filePath)
{
	int error = ini_parse(filePath, iniHandler, this);
	if (error < 0)
	{
		WPRINTF("Failed to load config file: %s: %d", filePath, error);
	}
}
