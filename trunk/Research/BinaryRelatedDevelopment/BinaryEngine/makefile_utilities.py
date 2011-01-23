import fileinput
import re
import string

def getParameter():
    """This routine gets the memory settings from the makefile and return it"""
    dataret = textret = stackret = -1
    for line in fileinput.input("makefile"):
        data = re.search(r'\.data=([a-zA-Z0-9]*)', line)
        if data!=None:
            #print "!!!"+ repr(data.group(1))
            dataret = long(data.group(1), 16)
            dataret = dataret - 0x800000
            print repr(dataret)
            data = None
        text = re.search(r'\.text=([a-zA-Z0-9]*)', line)
        if text!=None:
            textret = long(text.group(1), 16)
            print repr(textret)
            text = None
        stack= re.search(r'stack=([a-zA-Z0-9]*)', line)
        if stack!=None:
            stackret = long(stack.group(1), 16)
            print repr(stackret)
            stack = None
    return [dataret, stackret, textret]

def setParameter(ram_start, ram_end, rom_start):
    """This routine sets the makefile memory settings based on user input"""
    ram_start_string = hex(long(ram_start+0x800000))
    ram_start_string = ram_start_string.rstrip('L')
    ram_end_string = hex(int(ram_end))
    rom_start_string = hex(int(rom_start))
    print "all "+ ram_start_string + " "+ ram_end_string+ " "+ rom_start_string
    for line in fileinput.input("makefile", inplace = 1):
        line = line.rstrip('\n')
        data = re.search(r'\.data=([a-zA-Z0-9]*)', line)
        if data!=None:
            line = re.sub('0x[0-9a-zA-Z]+', ram_start_string, line)
            data = None
        text = re.search(r'\.text=([a-zA-Z0-9]*)', line)
        if text!=None:
            line = re.sub('0x[0-9a-zA-Z]+', rom_start_string, line)
            text = None
        stack= re.search(r'stack=([a-zA-Z0-9]*)', line)
        if stack!=None:
            line = re.sub('0x[0-9a-zA-Z]+', ram_end_string, line)
            stack = None
        print line
    return 
    
