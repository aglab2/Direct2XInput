# DirectInput to XInput converter

This project is a DLL that exposes XInput API converting DirectInput calls to XInput, allowing older DirectInput controllers to work seamlessly with modern XInput-only tools.

## Configuration  
Mappings are defined in a simple INI file format in d2x.log file: 

```ini
[Controller]
name = MAYFLASH GameCube Controller Adapter
A              = 2
B              = 3
X              = 1
Y              = 4
DPad Up        = 13
DPad Down      = 14
DPad Left      = 15
DPad Right     = 16
Start          = 5
Back           = 6
Left Thumb     = 11
Right Thumb    = 12
Left Shoulder  = 7
Right Shoulder = 8
Left Trigger   = 1
Right Trigger  = 2
Left Thumb X   = X Axis
Left Thumb Y   = Y Axis Inverted
Right Thumb X  = X Rotation
Right Thumb Y  = Y Rotation Inverted
```

## Log

Log files are written near DLL in file d2x.log. Log file has all connected DInput devices attempted to be pulled and parsing errors.

```
Failed to find a matching device matching 'PSX to USB v1.0'
Found device 'POKKEN CONTROLLER'
Failed to find a matching device matching 'PSX to USB v1.0'
Found device 'POKKEN CONTROLLER'
Device matched, opening
Found device 'POKKEN CONTROLLER'
Device matched, opening
Found device 'MAYFLASH GameCube Controller Adapter'
Device matched, opening
Found device 'MAYFLASH GameCube Controller Adapter'
Failed to decode DInput axis 'Left Trigger' corresponding to XInput axis 'test error'
```
