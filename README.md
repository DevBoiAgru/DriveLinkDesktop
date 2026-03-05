# DriveLink: Desktop
The desktop component for using your phone as a driving controller using DriveLink


## About
This is the desktop part of the DriveLink system where your phone sends input data to the desktop and you can use those inputs as a game controller.
This component does the following:
	- Create a user interface using ImGUI and SDL3
	- Listen for the input events from the phone using a UDP socket
	- Send the input events recieved through a vJoy virtual device

## Planned features
- Movable and resizable window

## Dependencies and installation:
You need the following components to build this app (Installed through vcpkg):
- SFML

## Building instructions

If you want to build for x86 systems, you will need to change the dll that is copied (Project Properties -> Build Events -> Post-Build Event) and the additional library path (Linker -> Input -> Additional Dependencies) for vJoy to the x86 versions for the .dll and the .lib in visual studio project properties instead.
