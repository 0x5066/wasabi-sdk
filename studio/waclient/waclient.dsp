# Microsoft Developer Studio Project File - Name="waclient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=waclient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "waclient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "waclient.mak" CFG="waclient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "waclient - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "waclient - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "waclient - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "waclient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "waclient - Win32 Release"
# Name "waclient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "attribs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\attribs\attribute.cpp
# End Source File
# Begin Source File

SOURCE=..\attribs\cfgitemi.cpp
# End Source File
# End Group
# Begin Group "services"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\studio\services\servicei.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_action.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_contextCmd.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_coreadmin.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_db.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_droptarget.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_eval.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_fileread.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_filesel.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_imggen.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_imgload.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_itemmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_mediaconverter.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_mediacore.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_metadata.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_minibrowser.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_objectdir.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_playlist.cpp
# End Source File
# Begin Source File

SOURCE=..\pledit\svc_pldir.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_scriptobj.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_skinfilter.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_stringconverter.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_textfeed.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_tooltips.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_wndcreate.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_xmlprov.cpp
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_xuiobject.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\studio\waclient.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\studio\services\svc_action.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_contextCmd.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_coreadmin.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_db.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_device.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_droptarget.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_eval.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_fileread.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_filesel.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_imggen.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_imgload.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_itemmgr.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_mediaconverter.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_mediacore.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_metadata.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_minibrowser.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_objectdir.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_playlist.h
# End Source File
# Begin Source File

SOURCE=..\pledit\svc_pldir.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_scriptobj.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_skinfilter.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_stringconverter.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_stringtypes.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_textfeed.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_tooltips.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_wndcreate.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_xmlprov.h
# End Source File
# Begin Source File

SOURCE=..\studio\services\svc_xuiobject.h
# End Source File
# End Group
# End Target
# End Project
