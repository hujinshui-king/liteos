"""This file is for test only and see how things go on in the system"""

import binaryengine
import sys
import commands
import re
import os
import makefile_utilities
import string
import assembly
from sets import *

appname = ''
codestore = []

def readAssembly2(nameofApp):
    """Read the assembly code from the current directory"""
    #filename = "TestBlink.elf"
    filename = nameofApp+".elf"
    command = "avr-objdump -d "
    command = command + filename
    pipe = os.popen(command)
    output = pipe.read()
    pipe.close()
    scanned = output.split('\n')
    return scanned


if __name__ == "__main__":



    binaryengine.processArguments()
    binaryengine.setEnvironment()

    #Now make modification of the makefile and call makefile to generate
    #differnt binary images

    #first enter the default directory under the current working directory

    os.chdir("./default")
    print "Now change the directory to "+commands.getoutput('pwd')

    print "Now starting the first round of compilation with original parameters"


    scanned = readAssembly2("TestBlink")

    print "parsing and storing"

    #print scanned

    assemblycode = binaryengine.ParsingAssembly(scanned)

    mem_start = mem_end = prog_start = 0
        

    codestore.append({'ram_start':mem_start, 'ram_end':mem_end, 'rom_start':prog_start, 'assembly': assemblycode})



    print "reading assembly"

    scanned = readAssembly2("TestBlink1")

    print "parsing and storing"

    #print scanned

    assemblycode = binaryengine.ParsingAssembly(scanned)


    codestore.append({'ram_start':mem_start, 'ram_end':mem_end, 'rom_start':prog_start, 'assembly': assemblycode})

    #Recompile

    #Read and parsing

    diffset = Set()
    for i in range(len(codestore)-1):
        #compare i and i+1
        print "length is " + repr(len(codestore[i]["assembly"]["instructions"]))

        if len(codestore[i]["assembly"]["instructions"])!=len(codestore[i]["assembly"]["instructions"]):
           print "Serious Error!"

        for j in range(35, len(codestore[i]["assembly"]["instructions"])):
            if (codestore[i]["assembly"]["instructions"][j]["disassembly"] != codestore[i+1]["assembly"]["instructions"][j]["disassembly"]):
                diffset.add(j)
    print diffset
    for i in diffset:
        print repr(i)+": "+codestore[0]["assembly"]["instructions"][i]["opcode"]\
        +" "+codestore[0]["assembly"]["instructions"][i]["args"]+ "  vs  "\
        +codestore[1]["assembly"]["instructions"][i]["opcode"]+" "+codestore[1]["assembly"]["instructions"][i]["args"]
        

