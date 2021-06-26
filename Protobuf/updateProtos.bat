@echo off
REM  Hyperloop Competition 4 Port

REM  This file compiles the .proto descriptor file for the proto packet that is sent from the pod to the PDS.
REM  It handles moving the created files to the appropriate folders for the flight computer.

REM If proto compiler is installed 
SETLOCAL ENABLEDELAYEDEXPANSION

WHERE protoc
IF %ERRORLEVEL% NEQ 0 (
    echo To use this script you must have the protobuf compiler installed
    echo Please see https://developers.google.com/protocol-buffers/ for installation instructions
    echo Alternatively, see the README! 
) else (
    echo Proto compiler found! Updating protos...
)

REM For any `*.proto` file, will create a folder, along with nested `cpp` & `py` folders, and compile `.proto` file
for /R %%f in (*.proto) do (
    set target_dir=%%~nf
    MD "!target_dir!/cpp" "!target_dir!/py"
    protoc --cpp_out="!target_dir!/cpp" "!target_dir!.proto"
    protoc --python_out="!target_dir!/py" "!target_dir!.proto"
)

REM Move files accordingly
SET flightComputer="..\Subsurface\FlightComputer\include"
SET flightComputerSrc="..\Subsurface\FlightComputer\src"

SET PDS="..\OnLand\PDS"
SET FLASK="..\OnLand\ControlLaptop"
SET TESTTOOLS="..\Subsurface\FlightComputer\tools\net_servers"

REM Force copy the files, replacing existing ones
REM /Y - Suppress confirm prompt
REM /Y - Suppress confirm prompt

COPY /Y Paradigm\cpp\*.pb.h %flightComputer%
COPY /Y Paradigm\cpp\*.pb.cc %flightComputerSrc%

COPY /Y Paradigm\py\* %PDS%
COPY /Y Paradigm\py\* %FLASK%
COPY /Y Paradigm\py\* %TESTTOOLS%


echo Proto Update Complete
