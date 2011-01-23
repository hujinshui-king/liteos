;NSIS LiteOS Setup
;The LiteOS installation Script 

!addplugindir "plugins"

;this makes sure that the branding is removed 
 BrandingText " "
	 
; MUI Settings / Header
;check out the directory for information 



; HM NIS Edit Wizard helper defines

!define PRODUCT_NAME "The LiteOS Operating System"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "University of Illinois"
!define PRODUCT_WEB_SITE "http://www.liteos.net"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

;--------------------------------
;Include Modern UI

  !include "UMUI.nsh"
  !include "EnvVarUpdate.nsh"

;--------------------------------


;--------------------------------
;Interface Settings

 !define UMUI_SKIN "blue"
 
 !define MUI_ABORTWARNING
 !define MUI_UNABORTWARNING
 
 !define UMUI_PAGEBGIMAGE
 !define UMUI_UNPAGEBGIMAGE



;; MUI Settings
;!define MUI_ABORTWARNING
;!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install-blue-full.ico"
;!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"


 Var JAVA_HOME
 Var JAVA_VER
 Var JAVA_INSTALLATION_MSG



;General


  ;Name and file
 
 	Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
  OutFile "SetupLiteOS.exe"

  ;Default installation folder
  
  InstallDir "$PROGRAMFILES\LiteOS"
  ShowInstDetails show
  ShowUnInstDetails show
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\LiteOS" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Variables

  Var StartMenuFolder
  Var PATHVAR

	
;--------------------------------
;Pages

	; Welcome page
	!insertmacro MUI_PAGE_WELCOME
  ;!insertmacro MUI_PAGE_WELCOME
	!define MUI_LICENSEPAGE_RADIOBUTTONS
 
  !insertmacro MUI_PAGE_LICENSE "License.rtf"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\LiteOS" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_PAGE_INSTFILES
  
  ; Finish page
	!insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;-------------------------------
;SETTINGS

Section -SETTINGS
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
SectionEnd


;--------------------------------
; Constants
;--------------------------------
!define GET_JAVA_URL "http://www.java.com"




;--------------------------------
;Installer Sections



SubSection "LiteOS Core Files" LiteOSPart

 ;The files on the mote and the  settings 
 
  Section "Runtime Environment" LiteOSPartRuntime
   

  SetOutPath "$INSTDIR"

  File "License.rtf"
  
  
  SetOutPath "$INSTDIR\SourceCode"
 
  File /r "OS_Dev\SourceCode\" 
  
  SetOutPath "$INSTDIR\Drivers"
 
  File /r "OS_Dev\Drivers\" 
  
  
  ;Modify 
  ;ADD YOUR OWN FILES HERE...
  
  ;Store installation folder
  WriteRegStr HKCU "Software\LiteOS" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
 
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    ;Modify
    SetOutPath $INSTDIR
    WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    
      CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
      
      
  	  ;CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Installer.lnk" "$INSTDIR\Tools\InstallerScript.sh"
  	  ;CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Shell.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  	  
     ;set the environment varaibles 
    
    
    ;ReadRegStr $PATHVAR HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" Path 
    ;StrCmp "" $PATHVAR SkipHere NotSkipHere
    ;NotSkipHere: WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "Path" "$PATHVAR;$INSTDIR;$INSTDIR\Tools"
    ;Goto DoneNow
    ;SkipHere:     WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "Path" "$INSTDIR;$INSTDIR\Tools"        
    ;Goto DoneNow 
      
    ;DoneNow:
   
   
 
   
   

    
        
  !insertmacro MUI_STARTMENU_WRITE_END

  SectionEnd
  
  Section "Operating Shell" LiteOSPartShell
  SetOutPath "$INSTDIR\Tools"
  File /r "OS_Dev\Tools\" 
  
  SectionEnd
  
  Section "Programming Environment" LiteOSPartProgramming
  SetOutPath "$INSTDIR\Apps"
  File /r "OS_Dev\Apps\" 
  SectionEnd
  
  
SubSectionEnd


SubSection "LiteOS Documentation" LiteOSDocumentation

 ; 

  
  Section "Manuals" LiteOSDocumentationManuals
   
  SetOutPath "$INSTDIR\Documentation"
  File  "OS_Dev\Documentation\releasenotes.txt"
  
  SetOutPath "$INSTDIR\Documentation\Manuals"
  File /r "OS_Dev\Documentation\Manuals\" 
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\User Manual.lnk" "$INSTDIR\Documentation\Manuals\LiteOS_User_Guide.pdf"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Programming Guide.lnk" "$INSTDIR\Documentation\Manuals\LiteOS_Programming_Guide.pdf"
  SectionEnd
  
  Section "Tutorial Labs" LiteOSDocumentationLabs
  SetOutPath "$INSTDIR\Documentation\TutorialLabs"
  File /r "OS_Dev\Documentation\TutorialLabs\" 
  SectionEnd
  
  Section "Research Papers" LiteOSDocumentationPapers
  SetOutPath "$INSTDIR\Documentation\ResearchPapers"
  File /r "OS_Dev\Documentation\ResearchPapers\" 
  SectionEnd 
    
SubSectionEnd



SubSection "LiteOS Plugins" LiteOSPlugin

	Section "Multihop Communication Stacks" LiteOSPluginComm
	
	 SetOutPath "$INSTDIR\Plugins\MultiHopProtocols"
	 File /r "OS_Dev\Plugins\MultiHopProtocols\"
	  

	 
	SectionEnd


	Section "Sensor Network Detection Performance Analyzer" LiteOSPluginAnalyzer
	
	 SetOutPath "$INSTDIR\Plugins\SensorNetworkDetectionAnalyzer\"
	 File /r "OS_Dev\Plugins\SensorNetworkDetectionAnalyzer\"
	 

	 
	SectionEnd
  
SubSectionEnd

;may consider use subsection later

Section "WinAVR Version 20070122" WinAVRPart
   SetOutPath "$INSTDIR"
		MessageBox MB_OK \
    'The WinAVR Version 20070122 is about to be installed. Please make sure that you select the default options.'
    File "PackInstaller\WinAVRSetup.exe"
		ExecWait "$INSTDIR\WinAVRSetup.exe"
		Delete "$INSTDIR\WinAVRSetup.exe"
SectionEnd

Section "Pre-compiled Binaries" BinaryPart
   SetOutPath "$INSTDIR\Binary"
	 File /r "OS_Dev\Binary\"
	 
SectionEnd


Section "Java Serial Port Operation Support" JavaPart
		MessageBox MB_OK \
    'The serial port support (javax.comm) is about to be installed. Please make sure that you have installed Java runtime environment or SDK on your computer.'
   
    Call LocateJVM
    StrCmp "" $JAVA_INSTALLATION_MSG Success OpenBrowserToGetJava
 
    Success:
        ;Call SetEnv
        ;!system "$INSTDIR\setEnv.cmd"  
 				;Delete "$INSTDIR\setEnv.cmd"  
        Goto Done
 
    OpenBrowserToGetJava:
        Exec '"explorer.exe" ${GET_JAVA_URL}'
 
    Done:

SectionEnd


var TempResult


;Install secton

Function .onInstSuccess
		
		SetOutPath "$INSTDIR\Tools"    
		File "OS_Dev\Tools\avr-gcc.exe"
		
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"	      
		${EnvVarUpdate} $0 "PATH" "P" "HKLM" "$INSTDIR;$INSTDIR\Tools"		
				
		Version::IsWindowsXP
 
   ; get result
 
     Pop $TempResult
 
     ; check result
 
     StrCmp $TempResult "1" ItIsWindowsXP IsIsNotWindowsXP
 
     ItIsWindowsXP:
        
     Delete "$INSTDIR\Tools\avr-gcc.exe"
     Goto Go2
 
      IsIsNotWindowsXP:
      
    
      
     IfFileExists "C:\WinAVR\bin\avr-gcc.exe" check1 check2
     
		     check1: 
		     
		       Rename "C:\WinAVR\bin\avr-gcc.exe" "C:\WinAVR\bin\avr-gcc-old.exe"
		       CopyFiles "$INSTDIR\Tools\avr-gcc.exe" "C:\WinAVR\bin\avr-gcc.exe"		     	
		       Delete "$INSTDIR\Tools\avr-gcc.exe"
		      ; MessageBox MB_OK "Files replaced"
		       Goto Go2
		     check2:
		       Delete "$INSTDIR\Tools\avr-gcc.exe"
		       Goto Go2
     
  
	
        
        
  Go2:  MessageBox MB_OK "Congratulations, installation complete. Please use the manual to get started. "
     
FunctionEnd


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_LiteOSPart ${LANG_ENGLISH} "The core files of LiteOS operating system."
  
  ;Language strings
  LangString DESC_LiteOSPartRuntime ${LANG_ENGLISH} "The runtime environment of LiteOS."
  
  ;Language strings
  LangString DESC_LiteOSPartShell ${LANG_ENGLISH} "The operating shell of LiteOS."

  ;Language strings
  LangString DESC_LiteOSPartProgramming ${LANG_ENGLISH} "The programming environment of LiteOS."
  
  
   ;Language strings
  LangString DESC_LiteOSDocumentation ${LANG_ENGLISH} "The documentation of LiteOS."
  
   ;Language strings
  LangString DESC_LiteOSDocumentationManuals ${LANG_ENGLISH} "The manuals for operating and programming in LiteOS."

   ;Language strings
  LangString DESC_LiteOSDocumentationLabs ${LANG_ENGLISH} "The lab materials based on LiteOS."  
  
   ;Language strings
  LangString DESC_LiteOSDocumentationPapers ${LANG_ENGLISH} "The published papers based on LiteOS."
  
   ;Language strings
  LangString DESC_LiteOSPlugin ${LANG_ENGLISH} "The plugins for LiteOS contains the additional components of LiteOS."
  
  
     ;Language strings
  LangString DESC_LiteOSPluginComm ${LANG_ENGLISH} "The communication plugin implements multi-hop communication layer protocols, including the geographic routing protocol, the flooding routing protocol, and the logical coordinate routing protocol."
 
 
     ;Language strings
  LangString DESC_LiteOSPluginAnalyzer ${LANG_ENGLISH} "The analyzer plugin implements a sensor network detection performance analyzer. The implementation is based on the results reported in the publication $\"Analysis of Target Detection Performance for Wireless Sensor Networks$\" by Qing Cao, Ting Yan, John Stankovic, and Tarek Abdelzaher."

 
 
   ;Language strings
  LangString DESC_BinaryPart ${LANG_ENGLISH} "The pre-compiled LiteOS binary images."
  

   
    
  ;Language strings
  LangString DESC_WinAVRPart ${LANG_ENGLISH} "WinAVR distribution 20070122 used by LiteOS."

  ;Language strings
  LangString DESC_JavaPart ${LANG_ENGLISH} "Adding serial port support to Java Runtime Environment(JRE)."
  
  
  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN

    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSPart} $(DESC_LiteOSPart)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSPartRuntime} $(DESC_LiteOSPartRuntime)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSPartShell} $(DESC_LiteOSPartShell)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSPartProgramming} $(DESC_LiteOSPartProgramming)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSDocumentation} $(DESC_LiteOSDocumentation)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSDocumentationManuals} $(DESC_LiteOSDocumentationManuals)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSDocumentationLabs} $(DESC_LiteOSDocumentationLabs)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSDocumentationPapers} $(DESC_LiteOSDocumentationPapers)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSPlugin} $(DESC_LiteOSPlugin)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSPluginComm} $(DESC_LiteOSPluginComm)
    !insertmacro MUI_DESCRIPTION_TEXT ${LiteOSPluginAnalyzer} $(DESC_LiteOSPluginAnalyzer)
 
    !insertmacro MUI_DESCRIPTION_TEXT ${WinAVRPart} $(DESC_WinAVRPart)   
    !insertmacro MUI_DESCRIPTION_TEXT ${JavaPart} $(DESC_JavaPart)
    !insertmacro MUI_DESCRIPTION_TEXT ${BinaryPart} $(DESC_BinaryPart) 
    !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
 


;--------------------------------
;Uninstaller Section

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONEXCLAMATION|MB_OK "LiteOS was successfully removed from your computer. Note you need to uninstall WinAVR separately. "
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components (WinAVR not included)?" IDYES +2
  Abort
FunctionEnd



Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...


  ;Delete "$INSTDIR\Uninstall.exe"
  ;Delete "$INSTDIR\${PRODUCT_NAME}.url"
  ;Delete "$INSTDIR\License.rtf"
  
  
  
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Have you backed up all your application code for future use? The LiteOS directory is going to be deleted."  IDYES +2
  Abort
  try:
  
  RMDir /r "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Website.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\User Manual.lnk"  
  Delete "$SMPROGRAMS\$StartMenuFolder\Programming Guide.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Quick Start.lnk"
  
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR"
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" "$INSTDIR\Tools"
  
  ;does not work
  ;DeleteRegValue  HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment\Path" "$INSTDIR"
  ;DeleteRegValue  HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment\Path" "$INSTDIR\Tools"
  
  
  DeleteRegKey /ifempty HKCU "Software\LiteOS"
  SetAutoClose true

SectionEnd



;--------------------------------
Function LocateJVM
    ;Check for Java version and location
    Push $0
    Push $1
    
    StrCpy $JAVA_INSTALLATION_MSG ""
    
    ReadRegStr $JAVA_VER HKLM "SOFTWARE\JavaSoft\Java Runtime Environment" CurrentVersion
    StrCmp "" "$JAVA_VER" JavaNotPresent CheckJavaVer
 
    JavaNotPresent:
        StrCpy $JAVA_INSTALLATION_MSG "Java Runtime Environment is not installed on your computer. You need version 1.4 or newer to run this program."
        Goto Done
 
    CheckJavaVer:
        ReadRegStr $0 HKLM "SOFTWARE\JavaSoft\Java Runtime Environment\$JAVA_VER" JavaHome
        GetFullPathName /SHORT $JAVA_HOME "$0"
        StrCpy $0 $JAVA_VER 1 0
        StrCpy $1 $JAVA_VER 1 2
        StrCpy $JAVA_VER "$0$1"
        IntCmp 14 $JAVA_VER FoundCorrectJavaVer FoundCorrectJavaVer JavaVerNotCorrect
 
    FoundCorrectJavaVer:
        IfFileExists "$JAVA_HOME\bin\javaw.exe" 0 JavaNotPresent
        ;MessageBox MB_OK "Found Java: $JAVA_VER at $JAVA_HOME"
        
        IfFileExists "$JAVA_HOME\lib\ext\win32com.dll" donewiththeinstall 0
        
         SetOutPath "$INSTDIR"
    			File "PackInstaller\jdom.jar" 
  				File "PackInstaller\javax.comm.properties" 
	  			File "PackInstaller\comm.jar" 
  				File "PackInstaller\win32com.dll" 
  	
        ;Now copy the files
        CopyFiles "$INSTDIR\comm.jar" "$JAVA_HOME\lib\ext"
        CopyFiles "$INSTDIR\win32com.dll" "$JAVA_HOME\lib\ext"
        CopyFiles "$INSTDIR\jdom.jar" "$JAVA_HOME\lib"
        CopyFiles "$INSTDIR\javax.comm.properties" "$JAVA_HOME\lib"
        
        Delete "$INSTDIR\comm.jar"
        Delete "$INSTDIR\win32com.dll"
        Delete "$INSTDIR\jdom.jar"
        Delete "$INSTDIR\javax.comm.properties" 
       
      
        
        ;set the environment varaibles 
        ReadRegStr $0 HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" CLASSPATH 
        StrCmp "" $0 Skip NotSkip         
        NotSkip: WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "CLASSPATH" ".;$JAVA_HOME\lib\ext;$JAVA_HOME\lib;$JAVA_HOME\lib\jdom.jar;$JAVA_HOME\lib\ext\comm.jar;$0"
        Goto Done
        Skip:     WriteRegStr HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment" "CLASSPATH" ".;$JAVA_HOME\lib\ext;$JAVA_HOME\lib;$JAVA_HOME\lib\jdom.jar;$JAVA_HOME\lib\ext\comm.jar"        
     
        
        Goto Done
        
   
 
    JavaVerNotCorrect:
        StrCpy $JAVA_INSTALLATION_MSG "The version of Java Runtime Environment installed on your computer is $JAVA_VER. Version 1.4 or newer is required to run this program."
 				
 				StrCmp "" "$JAVA_INSTALLATION_MSG" Done notdone
 				notdone: MessageBox MB_OK "$JAVA_INSTALLATION_MSG"
 				
 				donewiththeinstall:
       
        
        
    Done:
        Pop $1
        Pop $0
FunctionEnd
;--------------------------------
Function SetEnv
		System::Call 'Kernel32::SetEnvironmentVariableA(t, t) i("CLASSPATH", "value").r0'
		StrCmp $0 0 error
  	ExecWait ProgThatReadsEnv.exe
  	Goto done
		error:
  	MessageBox MB_OK "Cannot set environment variable"
		done:
		
    Push $3
    Push $4
 
    FileOpen $4 "$INSTDIR\setEnv.cmd" w
    StrCpy $3 "Set CLASSPATH=$JAVA_HOME\lib\ext;$JAVA_HOME\lib;$JAVA_HOME\lib\jdom.jar;$JAVA_HOME\lib\ext\comm.jar;%CLASSPATH%"
    FileWrite $4 "$3"
 
    FileWriteByte $4 "13"
    FileWriteByte $4 "10"
 
    ;StrCpy $3 "Set PATH=$JAVA_HOME\bin;%PATH%"
    ;FileWrite $4 "$3"
    FileClose $4
 
    Pop $4
    Pop $3
FunctionEnd
;--------------------------------


Function CopyFilesForJava


FunctionEnd
