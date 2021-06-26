#!/usr/bin/env bash

#  Hyperloop Competition 4 Port
#
#  This file compiles the .proto descriptor file for the proto packet that is sent from the pod to the PDS.
#  It handles moving the created files to the appropriate folders for the flight computer.

# If proto compiler is installed 
if ! type "protoc" > /dev/null; then
  echo "To use this script you must have the protobuf compiler installed"
  echo "Please see https://developers.google.com/protocol-buffers/ for installation instructions"
  exit
fi

# For any `*.proto` file, will create a folder, along with nested `cpp` & `py` folders, and compile `.proto` file
for f in *.proto
do
    dir=${f%.*}
    mkdir -p $dir/cpp $dir/py
	protoc --cpp_out=$dir/cpp $f
	protoc --python_out=$dir/py $f
done

# Move files accordingly
flightComputer="../Subsurface/FlightComputer/include"
flightComputerSrc="../Subsurface/FlightComputer/src"

PDS="../OnLand/PDS"
FLASK="../OnLand/ControlLaptop"
TESTTOOLS="..\Subsurface\FlightComputer\tools\net_servers"

# Force copy the files, replacing existing ones
cp -f Paradigm/cpp/*.pb.h $flightComputer
cp -f Paradigm/cpp/*.pb.cc $flightComputerSrc

cp -f Paradigm/py/* $PDS
cp -f Paradigm/py/* $FLASK
cp -f Paradigm/py/* $TESTTOOLS

echo "Proto Update Complete"
