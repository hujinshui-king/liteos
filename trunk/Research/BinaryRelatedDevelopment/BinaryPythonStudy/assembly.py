from string import *
import string
import re

def is_assembler_line(line):
    """This routine returns true if the string passed to it contains a valid line from objdump.
      This routine could add many more checks, but right now it only looks for the header."""
    if(line.find('\t') == -1):
      return False
    #explode based upon tabs
    array = line.split("\t")
    #return if we are empty
    if(not len(array)):
      return False
    temp = array[0].strip()
    #print temp
    #try:
    if (temp[-1] != ":"):
        return False
      #perform the hex conversion
        #numb = string.atol(temp[:-1],16)
    #except:
      #print "except"
     #   return False
    return True


def parse_assembler_line(line):
    """ This function returns a dictionary of the parse line:
    1. 'opcode' - This is a string containing the opcode of the instruction i.e. 'ret'
    2. 'args' - These are the arguments of the instruction (possibly empty)
    3. 'comment' - This is the comment from objdump
    4.  'disassembly' - The disassembly from objdump
    5.  'address' - The address the instruction starts on
    6.  'size' - This is the size of the function in bytes
    This routine assumes that is_assembler_line has already validated the line..."""
    
    retval = {'args':'', 'comment':'', 'opcode':''}
    #print 'Parsing Assembled "%s"'% line
    array = line.split("\t")
    retval['address'] = (array[0].strip()[:-1], 16)
    retval['disassembly'] = array[1].strip()
    retval['size'] = len(array[1].strip().split(' '))/2
    if(len(array) > 2):
        retval['opcode'] = array[2].strip()
    if(len(array) > 3):
        temp = array[3].strip()
        temp = re.sub('<[a-zA-Z_\.]+>', '', temp)
        retval['args'] = temp.strip()
        #remove the <> style line
        
    if(len(array) > 4):
        retval['comment'] = array[4].strip()
    #print 'parsed output = %s' % retval
    return retval
