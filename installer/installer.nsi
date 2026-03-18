; This script creates an installer for drivelink 64bit for Windows

; BEFORE COMPILING THIS SCRIPT
; MAKE SURE:
;  - You have built the solution in Release mode, this script looks for the
;    executables in ..\x64\Release directory.
;  - You have the vJoySetup.exe in ..\x64\ directory. (This should exist -> ..\x64\vJoySetup.exe)

!include "MUI2.nsh"

Name "DriveLink"
Icon "..\DriveLinkDesktop\resources\Icon.ico"
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

ExecWait 'taskkill /IM ${APP_EXE} /F >nul 2>&1'

SetOutPath "$INSTDIR"

File /r /x *.pdb /x *.obj /x *.ilk "..\x64\Release\*"

ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\vJoy" "DisplayName"
StrCmp $0 "" install_vjoy skip_vjoy

install_vjoy:
File "/oname=$TEMP\vJoySetup.exe" "..\x64\vJoySetup.exe"
ExecWait '"$TEMP\vJoySetup.exe" /SILENT'
Delete "$TEMP\vJoySetup.exe"

skip_vjoy:

CreateDirectory "$SMPROGRAMS\DriveLink"

Delete "$DESKTOP\DriveLink.lnk"
Delete "$SMPROGRAMS\DriveLink\DriveLink.lnk"
CreateShortcut "$DESKTOP\DriveLink.lnk" "$INSTDIR\${APP_EXE}"
CreateShortcut "$SMPROGRAMS\DriveLink\DriveLink.lnk" "$INSTDIR\${APP_EXE}"

SectionEnd
