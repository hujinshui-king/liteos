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

def processArguments():
   """Set the current working directory into the directory as specified
   by the arguments, and return. Assuming that the command line takes one parameter
   e.g. python binaryengine.py /opt/tinyos-1.x/apps/Blinktask. It also finds the appname"""

   global appname
   args = sys.argv[1:]
   if len(args)==1:
        os.chdir(args[0])
        print "Now change the directory to "+commands.getoutput('pwd')
        appname = re.search('/([a-zA-Z\.]+)$', args[0]).group(1)
   return

def setEnvironment():
    """Set up the environment variable"""
    os.system('export PATH=/cygdrive/e/OSPrograms/WinAVR/bin:/cygdrive/e/OSPrograms/WinAVR/utils/bin:/cygdrive/c/Program\ Files/TinyOS/jdk1.4.1_02/j2sdk1.4.1_02/bin:/cygdrive/c/Program\ Files/TinyOS/jdk1.4.1_02/j2sdk1.4.1_/jre/bin:/usr/local/bin:/usr/bin:/bin:/usr/X11R6/bin:/cygdrive/c/WINDOWS/system32:/cygdrive/c/WINDOWS:/cygdrive/c/WINDOWS/System32/Wbem:/cygdrive/c/Program\ Files/QuickTime/QTSystem/:/cygdrive/c/Program\ Files/TinyOS/ATT/Graphviz/bin:/usr/local/mspgcc/bin')
    return


def readAssembly(nameofApp):
    """Read the assembly code from the current directory"""
    filename = "TestBlink.elf"
    #filename = nameofApp+".elf"
    command = "avr-objdump -d "
    command = command + filename
    pipe = os.popen(command)
    output = pipe.read()
    pipe.close()
    scanned = output.split('\n')
    return scanned


def ParsingAssembly(scanned):
    """Parse the scanned into an array of dicts"""
    retval = {'instructions':[]}
    for line in scanned:
        if (assembly.is_assembler_line(line)):
            p = assembly.parse_assembler_line(line)
            retval['instructions'] += [p]
    return retval
    
if __name__ == "__main__":
    


    print "Now processing the arguments"
    processArguments()
    print "Now setting up the environment"
    setEnvironment()
    
    #Now make modification of the makefile and call makefile to generate
    #differnt binary images

    #first enter the default directory under the current working directory



    os.chdir("./default")
    print "Now change the directory to "+commands.getoutput('pwd')

    print "Now starting the first round of compilation with original parameters"
    
    [mem_start, mem_end, prog_start] = makefile_utilities.getParameter()
    [original_mem_start, original_mem_end, original_prog_start] = [mem_start, mem_end, prog_start]

    print "make clean now"
    os.system("pwd")
    #os.system("rm -rf TestBlink.elf dep/* TestBlink.hex TestBlink.eep TestBlink.lss TestBlink.map")
    os.system("make clean")
    os.system("make")

    print "reading assembly"
    
    scanned = readAssembly(appname)

    print "parsing and storing"

    #print scanned

    assemblycode = ParsingAssembly(scanned)

    codestore.append({'ram_start':mem_start, 'ram_end':mem_end, 'rom_start':prog_start, 'assembly': assemblycode})



    #print ret
    #Now change the memory settings

    for i in range(75):
        mem_start = mem_start+8
        mem_end  = mem_end+8
        prog_start = prog_start
        print "!!!Now parameter is " + repr(prog_start)
        makefile_utilities.setParameter(mem_start, mem_end, prog_start)

        print "make clean now twice"
        os.system("rm -rf TestBlink.elf dep/* TestBlink.hex TestBlink.eep TestBlink.lss TestBlink.map")
        os.system("make")

        print "reading assembly"

        scanned = readAssembly(appname)

        print "parsing and storing"

        #print scanned

        assemblycode = ParsingAssembly(scanned)

        codestore.append({'ram_start':mem_start, 'ram_end':mem_end, 'rom_start':prog_start, 'assembly': assemblycode})
    
    #Recompile

    #Read and parsing


    diffset = Set()
    for i in range(len(codestore)-1):
        #compare i and i+1
        #print "length is " + repr(len(codestore[i]["assembly"]["instructions"]))

        if len(codestore[i]["assembly"]["instructions"])!=len(codestore[i+1]["assembly"]["instructions"]):
           print "Serious Error!"


        for j in range( len(codestore[i]["assembly"]["instructions"])):
            if (codestore[i]["assembly"]["instructions"][j]["disassembly"] != codestore[i+1]["assembly"]["instructions"][j]["disassembly"]):
                diffset.add(j)
                
    print diffset
    testre = re.compile(r'\s+(0x[a-zA-Z0-9]+)[\s$]*')
    for i in diffset:
        print repr(i)+"  :::  ",
        for j in range(len(codestore)):
            #print codestore[j]["assembly"]["instructions"][i]["opcode"]+ " "+\
            print testre.search(codestore[j]["assembly"]["instructions"][i]["args"]).groups()[0],
        print

    makefile_utilities.setParameter(original_mem_start, original_mem_end, original_prog_start)
        
        
