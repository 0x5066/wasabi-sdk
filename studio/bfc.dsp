# Microsoft Developer Studio Project File - Name="bfc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=bfc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bfc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bfc.mak" CFG="bfc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bfc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "bfc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "$/Studio/bfc"
# PROP Scc_LocalPath ".\bfc"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "bfc - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "bfc - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
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

# Name "bfc - Win32 Release"
# Name "bfc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bfc\util\cmdline.h
# End Source File
# Begin Source File

SOURCE=.\bfc\util\pathparse.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\util\pathparse.h
# End Source File
# Begin Source File

SOURCE=.\bfc\util\recursedir.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\util\recursedir.h
# End Source File
# Begin Source File

SOURCE=.\bfc\util\showpref.h
# End Source File
# Begin Source File

SOURCE=.\bfc\util\timefmt.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\util\timefmt.h
# End Source File
# End Group
# Begin Group "wnds"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bfc\wnds\abstractwndhold.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\abstractwndhold.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\autofilterlist.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\autofilterlist.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\autoquerylist.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\autoquerylist.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\backbufferwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\backbufferwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\blankwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\blankwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\buttbar.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\buttbar.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\buttwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\buttwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\checkbox.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\checkbox.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\combobox.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\combobox.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\customobject.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\customobject.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\dropdownlist.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\dropdownlist.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\embeddedxui.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\embeddedxui.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\gradientwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\gradientwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\groupclickwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\groupclickwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\grouptgbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\grouptgbutton.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\guiradiogroup.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\guiradiogroup.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\historyeditbox.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\historyeditbox.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\itemlistwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\itemlistwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\listwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\listwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\mbtextcontent.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\mbtextcontent.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\objdirwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\objdirwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\pathpicker.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\pathpicker.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\qpaintwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\qpaintwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\queryline.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\queryline.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\rootwndholder.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\rootwndholder.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\scbkgwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\scbkgwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\scrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\scrollbar.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\skinwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\skinwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\status.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\status.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\svcwndhold.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\svcwndhold.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\tabsheet.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\tabsheet.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\tabsheetbar.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\tabsheetbar.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\titlebox.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\titlebox.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\treewnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\treewnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\typesheet.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\wnds\typesheet.h
# End Source File
# End Group
# Begin Group "db"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bfc\db\multiqueryclient.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\db\multiqueryclient.h
# End Source File
# Begin Source File

SOURCE=.\bfc\db\multiqueryserver.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\db\multiqueryserver.h
# End Source File
# Begin Source File

SOURCE=.\bfc\db\scanner.h
# End Source File
# Begin Source File

SOURCE=.\bfc\db\scannerserver.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\db\scannerserver.h
# End Source File
# Begin Source File

SOURCE=.\bfc\db\sharedscanner.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\db\sharedscanner.h
# End Source File
# Begin Source File

SOURCE=.\bfc\db\subqueryserver.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\db\subqueryserver.h
# End Source File
# End Group
# Begin Group "platform"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bfc\platform\guid.h
# End Source File
# Begin Source File

SOURCE=.\bfc\platform\platform.h
# End Source File
# Begin Source File

SOURCE=.\bfc\platform\win32.h
# End Source File
# End Group
# Begin Group "c_script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common\script\c_script\c_browser.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_browser.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_button.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_button.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_container.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_container.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_dropdownlist.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_dropdownlist.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_edit.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_edit.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_group.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_group.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_grouplist.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_grouplist.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_guiobject.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_guiobject.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_layout.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_layout.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_rootobj.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_rootobj.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_slider.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_slider.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_text.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_text.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_togglebutton.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\c_togglebutton.h
# End Source File
# Begin Source File

SOURCE=.\common\script\guiobject.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_browser.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_browser.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_button.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_button.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_container.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_container.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_dropdownlist.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_dropdownlist.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_edit.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_edit.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_group.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_group.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_guiobject.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_guiobject.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_layout.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_layout.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_rootobj.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_rootobj.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_slider.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_slider.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_text.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_text.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_togglebutton.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\h_togglebutton.h
# End Source File
# Begin Source File

SOURCE=.\common\script\objcontroller.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\objcontroller.h
# End Source File
# Begin Source File

SOURCE=.\common\script\rootobj.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\rootobj.h
# End Source File
# Begin Source File

SOURCE=.\common\script\rootobjcb.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\rootobjcb.h
# End Source File
# Begin Source File

SOURCE=.\common\script\scriptguid.h
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\scripthook.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\c_script\scripthook.h
# End Source File
# Begin Source File

SOURCE=.\common\script\scriptobj.cpp
# End Source File
# Begin Source File

SOURCE=.\common\script\scriptobj.h
# End Source File
# Begin Source File

SOURCE=.\common\script\scriptvar.h
# End Source File
# Begin Source File

SOURCE=.\common\script\vcputypes.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\bfc\appcmds.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\assert.cpp
# End Source File
# Begin Source File

SOURCE=.\attribs\attribute.cpp
# End Source File
# Begin Source File

SOURCE=.\attribs\attrstr.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\autobitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\basewnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\blending.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\canvas.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\cdda\cdamci.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\cdda\cddaenum.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\cdda\cddaplaylist.cpp
# End Source File
# Begin Source File

SOURCE=.\common\cells.cpp
# End Source File
# Begin Source File

SOURCE=.\attribs\cfgitemi.cpp
# End Source File
# Begin Source File

SOURCE=.\common\checkwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\clickwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\common.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\consolecb.cpp
# End Source File
# Begin Source File

SOURCE=.\common\contextcmd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\contextmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\common\contwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\corecb.cpp
# End Source File
# Begin Source File

SOURCE=.\common\corehandle.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\depend.cpp
# End Source File
# Begin Source File

SOURCE=.\common\dragitemi.cpp
# End Source File
# Begin Source File

SOURCE=.\common\droplist.cpp
# End Source File
# Begin Source File

SOURCE=.\common\editwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\editwndstring.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\encodedstr.cpp
# End Source File
# Begin Source File

SOURCE=.\common\filename.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\filteredcolor.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\findobjectcb.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\foreach.cpp
# End Source File
# Begin Source File

SOURCE=.\common\formwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\framewnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\freelist.cpp
# End Source File
# Begin Source File

SOURCE=.\common\guiobjwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\guistatuscb.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\xml\hashtable.c
# End Source File
# Begin Source File

SOURCE=.\common\intarray.cpp
# End Source File
# Begin Source File

SOURCE=.\common\labelwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\loadlib.cpp
# End Source File
# Begin Source File

SOURCE=.\common\mainminibrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\memblock.cpp
# End Source File
# Begin Source File

SOURCE=.\common\metadatasvc.cpp
# End Source File
# Begin Source File

SOURCE=.\common\minibrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\common\minibrowserwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\msgbox.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\msgpump.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\nakedobject.cpp
# End Source File
# Begin Source File

SOURCE=.\common\node.cpp
# End Source File
# Begin Source File

SOURCE=.\common\nsGUID.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\objectactuator.cpp
# End Source File
# Begin Source File

SOURCE=.\common\objectdir.cpp
# End Source File
# Begin Source File

SOURCE=.\common\oswnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\paintcb.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\paramparser.cpp
# End Source File
# Begin Source File

SOURCE=.\common\playstring.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\popexitcb.cpp
# End Source File
# Begin Source File

SOURCE=.\common\popup.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\profiler.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\ptrlist.cpp
# End Source File
# Begin Source File

SOURCE=.\common\radiogroup.cpp
# End Source File
# Begin Source File

SOURCE=.\common\radiownd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\readdir.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\region.cpp
# End Source File
# Begin Source File

SOURCE=.\common\rootcomp.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\rootwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\selectfile.cpp
# End Source File
# Begin Source File

SOURCE=.\common\sepwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\sequence.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\services\servicei.cpp
# End Source File
# Begin Source File

SOURCE=.\common\servicewnd.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\xml\simap.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\skincb.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\skinclr.cpp
# End Source File
# Begin Source File

SOURCE=.\common\slider.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\stack.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\std.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\string.cpp
# End Source File
# Begin Source File

SOURCE=.\common\stringpool.cpp
# End Source File
# Begin Source File

SOURCE=.\common\styleposwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\svc_enum.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\services\svc_imgload.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\services\svc_wndcreate.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\syscb.cpp
# End Source File
# Begin Source File

SOURCE=.\common\textbar.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\textfeedcb.cpp
# End Source File
# Begin Source File

SOURCE=.\common\thread.cpp
# End Source File
# Begin Source File

SOURCE=.\common\timer.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\timerclient.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\timermul.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\timeslicer.cpp
# End Source File
# Begin Source File

SOURCE=.\common\tipwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\tooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\u8string.cpp
# End Source File
# Begin Source File

SOURCE=.\common\urlgrab.cpp
# End Source File
# Begin Source File

SOURCE=.\common\utf8.cpp
# End Source File
# Begin Source File

SOURCE=.\bfc\virtualwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\common\wildcharsenum.cpp
# End Source File
# Begin Source File

SOURCE=.\common\wndcb.cpp
# End Source File
# Begin Source File

SOURCE=.\common\xml.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\xmlmanagedobj.cpp
# End Source File
# Begin Source File

SOURCE=.\common\xmlobject.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\xmlparse.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\xml\xmlparser.c
# End Source File
# Begin Source File

SOURCE=.\studio\xmlreader.cpp
# End Source File
# Begin Source File

SOURCE=.\studio\xml\xmlrole.c
# End Source File
# Begin Source File

SOURCE=.\studio\xml\xmltok.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bfc\appcmds.h
# End Source File
# Begin Source File

SOURCE=.\bfc\autobitmap.h
# End Source File
# Begin Source File

SOURCE=.\bfc\basewnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\bitlist.h
# End Source File
# Begin Source File

SOURCE=.\bfc\bitmap.h
# End Source File
# Begin Source File

SOURCE=.\bfc\blending.h
# End Source File
# Begin Source File

SOURCE=.\common\bucketitem.h
# End Source File
# Begin Source File

SOURCE=.\bfc\canvas.h
# End Source File
# Begin Source File

SOURCE=.\bfc\cdda\cdamci.h
# End Source File
# Begin Source File

SOURCE=.\bfc\cdda\cddaenum.h
# End Source File
# Begin Source File

SOURCE=.\bfc\cdda\cddaplaylist.h
# End Source File
# Begin Source File

SOURCE=.\common\cells.h
# End Source File
# Begin Source File

SOURCE=.\common\checkwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\clickwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\common.h
# End Source File
# Begin Source File

SOURCE=.\common\compdb.h
# End Source File
# Begin Source File

SOURCE=.\bfc\console.h
# End Source File
# Begin Source File

SOURCE=.\common\contextcmd.h
# End Source File
# Begin Source File

SOURCE=.\common\contextmenu.h
# End Source File
# Begin Source File

SOURCE=.\common\contwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\corehandle.h
# End Source File
# Begin Source File

SOURCE=.\common\cwndtrack.h
# End Source File
# Begin Source File

SOURCE=.\bfc\depend.h
# End Source File
# Begin Source File

SOURCE=.\bfc\depview.h
# End Source File
# Begin Source File

SOURCE=.\bfc\dispatch.h
# End Source File
# Begin Source File

SOURCE=.\bfc\drag.h
# End Source File
# Begin Source File

SOURCE=.\common\dragitem.h
# End Source File
# Begin Source File

SOURCE=.\common\dragitemi.h
# End Source File
# Begin Source File

SOURCE=.\common\droplist.h
# End Source File
# Begin Source File

SOURCE=.\common\editwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\editwndstring.h
# End Source File
# Begin Source File

SOURCE=.\bfc\encodedstr.h
# End Source File
# Begin Source File

SOURCE=.\common\fakedrag.h
# End Source File
# Begin Source File

SOURCE=.\common\filename.h
# End Source File
# Begin Source File

SOURCE=.\bfc\filteredcolor.h
# End Source File
# Begin Source File

SOURCE=.\bfc\findobjectcb.h
# End Source File
# Begin Source File

SOURCE=.\bfc\fontdef.h
# End Source File
# Begin Source File

SOURCE=.\bfc\foreach.h
# End Source File
# Begin Source File

SOURCE=.\common\formwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\framewnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\freelist.h
# End Source File
# Begin Source File

SOURCE=.\common\guiobjwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\guistatuscb.h
# End Source File
# Begin Source File

SOURCE=.\common\intarray.h
# End Source File
# Begin Source File

SOURCE=.\common\labelwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\loadlib.h
# End Source File
# Begin Source File

SOURCE=.\common\locales.h
# End Source File
# Begin Source File

SOURCE=.\common\mainminibrowser.h
# End Source File
# Begin Source File

SOURCE=.\bfc\map.h
# End Source File
# Begin Source File

SOURCE=.\bfc\memblock.h
# End Source File
# Begin Source File

SOURCE=.\common\metadatasvc.h
# End Source File
# Begin Source File

SOURCE=.\common\metatags.h
# End Source File
# Begin Source File

SOURCE=.\common\minibrowser.h
# End Source File
# Begin Source File

SOURCE=.\common\minibrowserwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\msgbox.h
# End Source File
# Begin Source File

SOURCE=.\bfc\msgpump.h
# End Source File
# Begin Source File

SOURCE=.\common\multimap.h
# End Source File
# Begin Source File

SOURCE=.\bfc\nakedobject.h
# End Source File
# Begin Source File

SOURCE=.\bfc\named.h
# End Source File
# Begin Source File

SOURCE=.\common\node.h
# End Source File
# Begin Source File

SOURCE=.\bfc\notifmsg.h
# End Source File
# Begin Source File

SOURCE=.\common\nsGUID.h
# End Source File
# Begin Source File

SOURCE=.\bfc\objectactuator.h
# End Source File
# Begin Source File

SOURCE=.\common\objectdir.h
# End Source File
# Begin Source File

SOURCE=.\common\oswnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\paintcb.h
# End Source File
# Begin Source File

SOURCE=.\bfc\pair.h
# End Source File
# Begin Source File

SOURCE=.\bfc\paramparser.h
# End Source File
# Begin Source File

SOURCE=.\common\playstring.h
# End Source File
# Begin Source File

SOURCE=.\common\plstring.h
# End Source File
# Begin Source File

SOURCE=.\bfc\popexitcb.h
# End Source File
# Begin Source File

SOURCE=.\common\popup.h
# End Source File
# Begin Source File

SOURCE=.\common\profiler.h
# End Source File
# Begin Source File

SOURCE=.\bfc\ptrlist.h
# End Source File
# Begin Source File

SOURCE=.\common\radiogroup.h
# End Source File
# Begin Source File

SOURCE=.\common\radiownd.h
# End Source File
# Begin Source File

SOURCE=.\common\readdir.h
# End Source File
# Begin Source File

SOURCE=.\bfc\reentryfilter.h
# End Source File
# Begin Source File

SOURCE=.\bfc\region.h
# End Source File
# Begin Source File

SOURCE=.\common\rootcomp.h
# End Source File
# Begin Source File

SOURCE=.\bfc\rootwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\selectfile.h
# End Source File
# Begin Source File

SOURCE=.\common\sepwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\servicewnd.h
# End Source File
# Begin Source File

SOURCE=.\common\SimpleWndCreate.h
# End Source File
# Begin Source File

SOURCE=.\bfc\skinclr.h
# End Source File
# Begin Source File

SOURCE=.\common\slider.h
# End Source File
# Begin Source File

SOURCE=.\bfc\stack.h
# End Source File
# Begin Source File

SOURCE=.\bfc\std.h
# End Source File
# Begin Source File

SOURCE=.\bfc\string.h
# End Source File
# Begin Source File

SOURCE=.\common\stringpool.h
# End Source File
# Begin Source File

SOURCE=.\common\styleposwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\svc_enum.h
# End Source File
# Begin Source File

SOURCE=.\common\textbar.h
# End Source File
# Begin Source File

SOURCE=.\bfc\textfeedcb.h
# End Source File
# Begin Source File

SOURCE=.\common\thread.h
# End Source File
# Begin Source File

SOURCE=.\common\timer.h
# End Source File
# Begin Source File

SOURCE=.\bfc\timerclient.h
# End Source File
# Begin Source File

SOURCE=.\bfc\timermul.h
# End Source File
# Begin Source File

SOURCE=.\bfc\timeslicer.h
# End Source File
# Begin Source File

SOURCE=.\common\tipwnd.h
# End Source File
# Begin Source File

SOURCE=.\bfc\tlist.h
# End Source File
# Begin Source File

SOURCE=.\bfc\tooltip.h
# End Source File
# Begin Source File

SOURCE=.\bfc\u8string.h
# End Source File
# Begin Source File

SOURCE=.\common\urlgrab.h
# End Source File
# Begin Source File

SOURCE=.\bfc\usermsg.h
# End Source File
# Begin Source File

SOURCE=.\common\usermsg.h
# End Source File
# Begin Source File

SOURCE=.\common\utf8.h
# End Source File
# Begin Source File

SOURCE=.\bfc\virtualwnd.h
# End Source File
# Begin Source File

SOURCE=.\common\wildcharsenum.h
# End Source File
# Begin Source File

SOURCE=.\common\wndcb.h
# End Source File
# Begin Source File

SOURCE=.\bfc\wndcreator.h
# End Source File
# Begin Source File

SOURCE=.\common\xlatstr.h
# End Source File
# Begin Source File

SOURCE=.\common\xml.h
# End Source File
# Begin Source File

SOURCE=.\common\xmlobject.h
# End Source File
# End Group
# End Target
# End Project
