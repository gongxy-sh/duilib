# Microsoft Developer Studio Project File - Name="UiDesigner" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=UiDesigner - Win32 DebugU
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UiDesigner.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UiDesigner.mak" CFG="UiDesigner - Win32 DebugU"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UiDesigner - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "UiDesigner - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "UiDesigner - Win32 DebugU" (based on "Win32 (x86) Application")
!MESSAGE "UiDesigner - Win32 ReleaseU" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UiDesigner - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin60"
# PROP Intermediate_Dir "..\..\Output\Release60Temp\UiDesigner"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\third_party\tinyxml" /I "..\..\BCGControlBarPro" /I "..\..\include" /I "..\..\zlib" /I "..\..\zlib\contrib" /I "..\..\libpng" /I "..\..\jpeg" /I "..\..\tiff\libtiff" /I "..\..\sqlite" /I "..\..\openssl\include" /I "...\.\regex" /D "NDEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_BCGCBPRO_STATIC_" /D "UILIB_STATIC" /D "_AFXDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\..\Output\bin60/UiDesigner.exe" /libpath:"..\..\lib60" /libpath:"..\..\Output\lib60"

!ELSEIF  "$(CFG)" == "UiDesigner - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin60"
# PROP Intermediate_Dir "..\..\Output\Debug60Temp\UiDesigner"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\third_party\tinyxml" /I "..\..\BCGControlBarPro" /I "..\..\include" /I "..\..\zlib" /I "..\..\zlib\contrib" /I "..\..\libpng" /I "..\..\jpeg" /I "..\..\tiff\libtiff" /I "..\..\sqlite" /I "..\..\openssl\include" /I "...\.\regex" /D "_DEBUG" /D "_MBCS" /D "WIN32" /D "_WINDOWS" /D "_BCGCBPRO_STATIC_" /D "UILIB_STATIC" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /pdb:"..\..\Output\bin60/UiDesignerD.pdb" /debug /machine:I386 /out:"..\..\Output\bin60/UiDesignerD.exe" /pdbtype:sept /libpath:"..\..\lib60" /libpath:"..\..\Output\lib60"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "UiDesigner - Win32 DebugU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UiDesigner___Win32_DebugU"
# PROP BASE Intermediate_Dir "UiDesigner___Win32_DebugU"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin60"
# PROP Intermediate_Dir "..\Output\DebugU60Temp\UiDesigner"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\third_party\tinyxml" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\third_party\tinyxml" /I "..\..\BCGControlBarPro" /I "..\..\include" /I "..\..\zlib" /I "..\..\zlib\contrib" /I "..\..\libpng" /I "..\..\jpeg" /I "..\..\tiff\libtiff" /I "..\..\sqlite" /I "..\..\openssl\include" /I "...\.\regex" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_BCGCBPRO_STATIC_" /D "UILIB_STATIC" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\bin60\UiDesignerUD.exe" /pdbtype:sept /libpath:"..\..\lib60" /libpath:"..\..\Output\lib60"

!ELSEIF  "$(CFG)" == "UiDesigner - Win32 ReleaseU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UiDesigner___Win32_ReleaseU"
# PROP BASE Intermediate_Dir "UiDesigner___Win32_ReleaseU"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin60"
# PROP Intermediate_Dir "..\Output\ReleaseU60Temp\UiDesigner"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I ".\third_party\tinyxml" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\third_party\tinyxml" /I "..\..\BCGControlBarPro" /I "..\..\include" /I "..\..\zlib" /I "..\..\zlib\contrib" /I "..\..\libpng" /I "..\..\jpeg" /I "..\..\tiff\libtiff" /I "..\..\sqlite" /I "..\..\openssl\include" /I "...\.\regex" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_WINDOWS" /D "_BCGCBPRO_STATIC_" /D "UILIB_STATIC" /D "_AFXDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\bin60\UiDesignerU.exe" /libpath:"..\..\lib60" /libpath:"..\..\Output\lib60"

!ENDIF 

# Begin Target

# Name "UiDesigner - Win32 Release"
# Name "UiDesigner - Win32 Debug"
# Name "UiDesigner - Win32 DebugU"
# Name "UiDesigner - Win32 ReleaseU"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ClassView.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogCustomFonts.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogDefaultAttribList.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogProjectNew.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogSaveAsName.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogSkinFileNew.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogTemplateOpen.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogUIAttribEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\UiDesigner.cpp
# End Source File
# Begin Source File

SOURCE=.\UiDesigner.rc
# ADD BASE RSC /l 0x804
# ADD RSC /l 0x804 /i "..\..\BCGControlBarPro" /i "..\..\BCGControlBarPro\Styles"
# End Source File
# Begin Source File

SOURCE=.\FileView.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\HookAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ImagePreview.cpp
# End Source File
# Begin Source File

SOURCE=.\LayoutManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiUITracker.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertiesWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyTabLayoutUI.cpp
# End Source File
# Begin Source File

SOURCE=.\ResourceView.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\tinyxml\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\tinyxml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\tinyxml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\third_party\tinyxml\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBoxWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UICommandHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\UIDesignerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\UIDesignerView.cpp
# End Source File
# Begin Source File

SOURCE=.\UIImagePreview.cpp
# End Source File
# Begin Source File

SOURCE=.\UIProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\UIUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewTree.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ClassView.h
# End Source File
# Begin Source File

SOURCE=.\DialogCustomFonts.h
# End Source File
# Begin Source File

SOURCE=.\DialogDefaultAttribList.h
# End Source File
# Begin Source File

SOURCE=.\DialogProjectNew.h
# End Source File
# Begin Source File

SOURCE=.\DialogSaveAsName.h
# End Source File
# Begin Source File

SOURCE=.\DialogSkinFileNew.h
# End Source File
# Begin Source File

SOURCE=.\DialogTemplateOpen.h
# End Source File
# Begin Source File

SOURCE=.\DialogUIAttribEdit.h
# End Source File
# Begin Source File

SOURCE=.\UiDesigner.h
# End Source File
# Begin Source File

SOURCE=.\FileView.h
# End Source File
# Begin Source File

SOURCE=.\GlobalVariable.h
# End Source File
# Begin Source File

SOURCE=.\HookAPI.h
# End Source File
# Begin Source File

SOURCE=.\ImageDialog.h
# End Source File
# Begin Source File

SOURCE=.\ImagePreview.h
# End Source File
# Begin Source File

SOURCE=.\LayoutManager.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MultiUITracker.h
# End Source File
# Begin Source File

SOURCE=.\PropertiesWnd.h
# End Source File
# Begin Source File

SOURCE=.\PropertyTabLayoutUI.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ResourceView.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\targetver.h
# End Source File
# Begin Source File

SOURCE=.\ToolBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ToolBoxWnd.h
# End Source File
# Begin Source File

SOURCE=.\UICommandHistory.h
# End Source File
# Begin Source File

SOURCE=.\UIDesignerDoc.h
# End Source File
# Begin Source File

SOURCE=.\UIDesignerView.h
# End Source File
# Begin Source File

SOURCE=.\UIImagePreview.h
# End Source File
# Begin Source File

SOURCE=.\UIProperties.h
# End Source File
# Begin Source File

SOURCE=.\UIUtil.h
# End Source File
# Begin Source File

SOURCE=.\ViewTree.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ToolBox\Active.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Button.ico
# End Source File
# Begin Source File

SOURCE=.\res\class_view.ico
# End Source File
# Begin Source File

SOURCE=.\res\class_view_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Combo.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Container.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Control.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Edit.ico
# End Source File
# Begin Source File

SOURCE=.\res\file_view.ico
# End Source File
# Begin Source File

SOURCE=.\res\file_view_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\HorizontalLayout.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Label.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Option.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Pointer.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Progress.ico
# End Source File
# Begin Source File

SOURCE=.\res\properties_wnd.ico
# End Source File
# Begin Source File

SOURCE=.\res\properties_wnd_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\res_view.ico
# End Source File
# Begin Source File

SOURCE=.\res\res_view_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\ScrollBar.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Slider.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\TabLayout.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\Text.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\TileLayout.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbox_wnd_hc.ico
# End Source File
# Begin Source File

SOURCE=.\res\UIDesigner.ico
# End Source File
# Begin Source File

SOURCE=.\res\UIDesignerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToolBox\VerticalLayout.ico
# End Source File
# End Group
# End Target
# End Project
