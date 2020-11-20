#!/usr/bin/env python3

import os
import shutil
import sys
import subprocess
from NARC import *

############
# Options go here.
############

ROM_NAME = "rom.nds"  # The name of your rom
SOURCE_ROM = "test.nds"
OFFSET_TO_PUT = 0x800000
SEARCH_FREE_SPACE = False  # Set to True if you want the script to search for free space
                           # Set to False if you don't want to search for free space as you for example update the engine

#############
# Options end here.
#############

###############
# Functions start here.
###############


def MakeOffset0x100Aligned(offset: int) -> int:
    while offset % 16 != 0:
        offset += 1

    return offset


def FindOffsetToPut(rom, neededBytes: int, startOffset: int) -> int:
    offset = startOffset
    rom.seek(0, 2)
    maxPosition = rom.tell()
    numFoundBytes = 0

    while numFoundBytes < neededBytes:
        if offset + numFoundBytes >= maxPosition:
            print("End of file reached. Not enough free space.")
            return 0

        numFoundBytes += 1
        rom.seek(offset + numFoundBytes)
        if rom.read(1) != b'\xFF':
            offset = MakeOffset0x100Aligned(offset + numFoundBytes)
            numFoundBytes = 0

    return offset


def ChangeFileLine(filePath: str, lineToChange: int, replacement: str):
    with open(filePath, 'r') as file:
        copy = file.read()
        file.seek(0x0)
        lineNum = 1
        for line in file:
            if lineNum == lineToChange:
                copy = copy.replace(line, replacement)
                break
            lineNum += 1

    with open(filePath, 'w') as file:
        file.write(copy)

def RunCommand(cmd: [str]):
    """Runs the command line command."""
    try:
        subprocess.check_output(cmd)
    except subprocess.CalledProcessError as e:
        try:
            print(e.output.decode(), file=sys.stderr)
        except:
            print(e)
        sys.exit(1)

def ExterNdsRom():
    selfs = "unzip"
    subprocess.call(["ndstool.exe"] + ["-x", ROM_NAME, "-9", selfs+"/arm9.bin","-7", selfs+"/arm7.bin","-y9",
                               selfs+"/overarm9.bin","-y7", selfs+"/overarm7.bin","-d", selfs+"/root","-y",
                               selfs+"/overlay","-t", selfs+"/banner.bin","-h", selfs+"/header.bin" ])
    NARC_Unpack("unzip/root/data/weather_sys.narc", "unzip/root/data/Fill", False)

def BuildRom():
    if os.path.exists("unzip/root/data/Fill/Fill.narc"):
        os.remove("unzip/root/data/Fill/Fill.narc")
    NARC_Pack("unzip/root/data/Fill", "unzip/root/data/Fill",False)
    selfs = "unzip"
    os.remove("unzip/root/data/weather_sys.narc")
    shutil.copyfile("unzip/root/data/Fill/Fill.narc","unzip/root/data/weather_sys.narc");
    shutil.rmtree("unzip/root/data/Fill")
    subprocess.call(["ndstool.exe"] + ["-c", SOURCE_ROM, "-9", selfs+"/arm9.bin","-7", selfs+"/arm7.bin","-y9",
                               selfs+"/overarm9.bin","-y7", selfs+"/overarm7.bin","-d", selfs+"/root","-y",
                               selfs+"/overlay","-t", selfs+"/banner.bin","-h", selfs+"/header.bin" ])


def EditInsert(offset: int):
    ChangeFileLine("./scripts/insert.py", 10, "OFFSET_TO_PUT = " + hex(offset) + '\n')
    ChangeFileLine("./scripts/insert.py", 11, 'SOURCE_ROM = "' + ROM_NAME + '"\n')


def BuildCode():
    if shutil.which('python3') is not None:
        result = os.system("python3 scripts/build.py")
    else:
        result = os.system("python scripts/build.py")

    if result != 0:  # Build wasn't sucessful
        sys.exit(1)


def InsertCode():
    if shutil.which('python3') is not None:
        os.system("python3 scripts/insert.py")
    else:
        os.system("python scripts/insert.py")


def ClearFromTo(rom, from_: int, to_: int):
    rom.seek(from_)
    for i in range(0, to_ - from_):
        rom.write(b'\xFF')

##############
# Functions end here.
##############


def main():
    ExterNdsRom()
    BuildCode()
    InsertCode()
    BuildRom()



if __name__ == '__main__':
    main()
    