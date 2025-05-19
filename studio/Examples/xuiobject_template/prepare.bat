@echo off

REM ------------- CUSTOMIZE HERE

set WAC_FILENAME=xuiwac
set WAC_NAME=XuiObject Example

set XUI_FILENAME=myobj
set XUI_NAMESPACE=MyNamespace
set XUI_TAG=MyObjectTag
set XUI_CLASSNAME=MyClass

REM -------------- END CUSTOMIZE

echo This batch file will prepare the project with the following files :
echo.
echo WAC        : %WAC_FILENAME%.cpp, %WAC_FILENAME%.h [Name:%WAC_NAME%]
echo XUI Object : %XUI_FILENAME%.cpp, %XUI_FILENAME%.h [Tag:%XUI_NAMESPACE%:%XUI_TAG%, Classname:%XUI_CLASSNAME%]
echo.
pause

gsar -smyobj -r%XUI_FILENAME% myobj.cpp -o
gsar -sMyObjectTag -r%XUI_TAG% myobj.cpp -o
gsar -sMyNamespace -r%XUI_NAMESPACE% myobj.cpp -o
gsar -sMyClass -r%XUI_CLASSNAME% myobj.cpp -o

gsar -smyobj -r%XUI_FILENAME% myobj.h -o
gsar -sMyObjectTag -r%XUI_TAG% myobj.h -o
gsar -sMyNamespace -r%XUI_NAMESPACE% myobj.h -o
gsar -sMyClass -r%XUI_CLASSNAME% myobj.h -o

gsar -smyobj -r%XUI_FILENAME% xuiwac.cpp -o
gsar -sMyObjectTag -r%XUI_TAG% xuiwac.cpp -o
gsar -sMyNamespace -r%XUI_NAMESPACE% xuiwac.cpp -o
gsar -sMyClass -r%XUI_CLASSNAME% xuiwac.cpp -o

gsar -smyobj -r%XUI_FILENAME% xuiwac.h -o
gsar -sMyObjectTag -r%XUI_TAG% xuiwac.h -o
gsar -sMyNamespace -r%XUI_NAMESPACE% xuiwac.h -o
gsar -sMyClass -r%XUI_CLASSNAME% xuiwac.h -o

gsar "-sXuiObject Example" "-r%WAC_NAME%" xuiwac.h -o
gsar -smyobj -r%XUI_FILENAME% xuiwac.dsp -o
gsar -sxuiwac -r%WAC_FILENAME% xuiwac.dsp -o
gsar -sxuiwac -r%WAC_FILENAME% xuiwac.cpp -o

gsar -sMyObjectTag -r%XUI_TAG% usage.xml -o
gsar -sMyNamespace -r%XUI_NAMESPACE% usage.xml -o

ren myobj.cpp %XUI_FILENAME%.cpp
ren myobj.h %XUI_FILENAME%.h
ren xuiwac.cpp %WAC_FILENAME%.cpp
ren xuiwac.h %WAC_FILENAME%.h
ren xuiwac.rc %WAC_FILENAME%.rc

