; This script creates an installer for drivelink 64bit for Windows

; BEFORE COMPILING THIS SCRIPT
; MAKE SURE:
;  - You have built the project, this script looks for the
;    executables in ..\build\bin directory.
;  - You have the vJoySetup.exe in ..\thirdparty\ directory. (This should exist -> ..\thirdparty\vJoySetup.exe)

!include "MUI2.nsh"

Name "DriveLink"
Icon "..\assets\Icon.ico"
OutFile "DriveLinkSetup.exe"
InstallDir "$PROGRAMFILES64\DriveLink"
!define APP_EXE "DriveLinkDesktop.exe"


!define MUI_FINISHPAGE_RUN "$INSTDIR\${APP_EXE}"
!define MUI_FINISHPAGE_RUN_TEXT "Launch DriveLink"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

RequestExecutionLevel admin

Section

ExecWait '"$SYSDIR\taskkill.exe" /IM ${APP_EXE} /F'

SetOutPath "$INSTDIR"

File /r /x *.pdb /x *.obj /x *.ilk /x *.txt "..\build\bin\*"

ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\vJoy" "DisplayName"
StrCmp $0 "" install_vjoy skip_vjoy

install_vjoy:
File "/oname=$TEMP\vJoySetup.exe" "..\thirdparty\vJoySetup.exe"
ExecWait '"$TEMP\vJoySetup.exe" /SILENT'
Delete "$TEMP\vJoySetup.exe"

skip_vjoy:

CreateDirectory "$SMPROGRAMS\DriveLink"

Delete "$DESKTOP\DriveLink.lnk"
Delete "$SMPROGRAMS\DriveLink\DriveLink.lnk"
CreateShortcut "$DESKTOP\DriveLink.lnk" "$INSTDIR\${APP_EXE}"
CreateShortcut "$SMPROGRAMS\DriveLink\DriveLink.lnk" "$INSTDIR\${APP_EXE}"

SectionEnd
