import re
import sys
import os
import fileinput
import string

def ishexdigit(char):
    return char in '0123456789abcdefABCDEF'


def ishexstring(strng):
    return filter(ishexdigit, strng) == strng

   
fileconfig = ""
cachedstrings = []
inputfile = ""

def processArguments():
    global inputfile
    global fileconfig
    """Read the user argument for the target type"""

    args = sys.argv[1:]

    if len(args) != 2:
        print "The number of parameters is incorrect."
        sys.exit(1)

    fileconfig = args[0]
    inputfile = args[1]



def isAddressString( str ):
    if len(str)<5:
        return False
    
    if str[len(str)-1] != ':':
        return False

    for i in range(len(str)-1):
        if ishexdigit(str[i]):
            continue;
        else:
            return False;
    return True;


def getAddress(str):
        temp2 = len(str)-1
        temp = str.strip()[:temp2]
        return temp

#MUST BE two digit to avoid add
def HexBinaryTwoDigit(str):
    if len(str)>2:
        return False;
    for i in range(len(str)):
        if ishexdigit(str[i]):
            continue;
        else:
            return False;
    return True;

#return a list contianing all the relevant parts
#the whole instruction should be one part

def processIndividualLine(strline):
    #remove the prefix and last parts first
    retarray = []
    strline = strline.strip()
    limiter = strline.find(':')
    strline = strline[limiter + 1:]
    limiter = strline.find(';')
    if limiter!=-1:
        strline = strline[:limiter]
    strline = strline.strip()
    arraytemp = strline.split()
    tempconcat = ""
    for i in range(len(arraytemp)):
        if HexBinaryTwoDigit(arraytemp[i]) == True:
            retarray.append(arraytemp[i])
        else:
            for j in range(i, len(arraytemp)):
                tempconcat += arraytemp[j]
                tempconcat += ' '
            retarray.append(tempconcat.strip())
            return retarray
    

#this function handles the file and then use lineno to extract info into global data

def handle_file_with_lineno( filename, lineid ):
    locatedstring = False
    cachedstrings = []
    lineid = lineid.strip()
    f = open(filename, 'r')
    while True:
        line1 = f.readline()
        line1 = line1.strip()
        dataarray = line1.split()
        if len(dataarray) <3:
            continue
        firstelement = dataarray[0].strip()
        if isAddressString(firstelement.strip()) == False:
            continue;
        if getAddress(firstelement) == lineid:
            cachedstrings.append(line1)
            locatedstring = True
            totalcount = 0
            #here start looking for the following strings
            while True:
                line2 = f.readline()
                if len(line2.split()) <2:
                	continue;
                firsttemp = line2.split()[0].strip()
                if isAddressString(firsttemp) == False:
                    continue
                else:
                    cachedstrings.append(line2)
                    totalcount = totalcount+1
                    if totalcount <6:
                        continue
                    else:
                        break
            break
        else:
            continue;
        


    if locatedstring == False:
        print "The file specification is error!"
        sys.exit(1)
    f.close()

        #now the strings have been located and placed into cachedstrings

        #Next place the strings into two lists, one for the instructions, the other for the instructions

    instructionset = []
    binaryset = []

    for i in range(len(cachedstrings)):
        arrayreturn = processIndividualLine(cachedstrings[i])
        for j in range(len(arrayreturn)-1):
            binaryset.append(arrayreturn[j])
        instructionset.append(arrayreturn[len(arrayreturn)-1])
        if (len(binaryset) == 14) or (len(binaryset)  == 16):
              break;
    f.close()
    return [lineid, instructionset, binaryset]

globalset = []


def generateOutputData(i, instructionset):
    if i == 0:
        print('if (regsource == APPTRACEPOINT'+str(i)+'COMPARE)\n');
    else:
        print('else if (regsource == APPTRACEPOINT'+str(i)+'COMPARE)\n');

    print('{\n')
    print('__asm__ __volatile__("pop r29" ::);\n__asm__ __volatile__("pop r28" ::);\n__asm__ __volatile__("pop r25" ::);\n__asm__ __volatile__("pop r24" ::);\n');
    print('__asm__ __volatile__("pop r31" ::);\n__asm__ __volatile__("pop r30" ::);\n__asm__ __volatile__("pop r31" ::);\n__asm__ __volatile__("pop r30" ::);\n');
    print('__asm__ __volatile__("pop r31" ::);\n__asm__ __volatile__("pop r30" ::);\n');

    for j in range(len(instructionset)):
        print('__asm__ __volatile__("'+instructionset[j]+'"::);\n');
    print('}\n')
        
if __name__ == "__main__":
    processArguments()
    #outputfile = open('temptest', 'w')

    for line in fileinput.input(fileconfig):
        configarray = line.split(' ')
        filename = configarray[0]
        lineid = configarray[1]
        [testid, iset, bset] = handle_file_with_lineno(filename, lineid)
        globalset.append([testid, iset, bset])


    #Open the trace file for the output of data
    state = 'normal'
    
    for myline in fileinput.input("./"+inputfile, inplace = 1):
        myline = myline.rstrip('\n')

        if state == 'normal':
            data = re.search('PATCHDEFSTART', myline)
            if data!=None:
                print myline
                for j in range(len(globalset)):
                    print "volatile uint8_t tracepatch"+str(j)+"[16];"
                state = 'patchdef'
                continue
            
            data = re.search('APPTRACESTART', myline)
            if data!=None:
                print myline
                for i in range(len(globalset)):
                    numvalue = globalset[i][0]
                    valuehex1 = int("0x"+numvalue, 16)/2
                    valuehex2 = valuehex1+5
                    temp1 = valuehex2/256
                    temp2 = valuehex2%256
                    valuehex2 = temp1+temp2*256

                    print('#define APPTRACEPOINT'+str(i)+'  '+ hex(valuehex1) + '\n')
                    print('#define APPTRACEPOINT'+str(i)+'COMPARE'+' '+ hex(valuehex2)+'\n\n\n')
                state = 'apptrace'
                continue

            data = re.search('APPJUMPSTART', myline)
            if data!=None:
                print myline
                state = 'appjump'
                for i in range(len(globalset)):
                    generateOutputData(i, globalset[i][1])
                continue

            data = re.search('CLEANTRACE', myline)
            if data!=None:
                print myline
                state = 'cleantrace'
                for i in range(len(globalset)):
                    if len(globalset[i][2])== 14:
                        appendix = ''
                    else:
                        appendix='_longinstruction'
                    print('\n')
                    print('{')
                    print('uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT'+str(i)+'*2);\n')
                    print('pagenum = tracepointlocation/2/128;\n')
                    print('pageoffset = tracepointlocation%256;\n')
                    print('removeUserTracePoint'+appendix+'( pagenum,  pageoffset , tracepatch'+str(i)+');\n')
                    print('}')
                    print('\n')
                continue
                    
            data = re.search('CLEANDEF', myline)
            if data!=None:
                print myline
                state = 'cleandef'
                for i in range(len(globalset)):
                    tempbset = globalset[i][2]
                    length = len(tempbset)
                    for j in range(length):
                        print('tracepatch'+str(i)+'['+str(j)+'] = 0x'+ tempbset[j]+';\n')
                    if length < 16:
                        print('tracepatch'+str(i)+'[ 14 ] = 0xff'+';\n')
                        print('tracepatch'+str(i)+'[ 15 ] = 0xff'+';\n')
                continue
                    
            data = re.search('ADDTRACE', myline)
            if data!=None:
                print myline
                state = 'addtrace'

                for i in range(len(globalset)):
                    if len(globalset[i][2])== 14:
                        appendix = ''
                    else:
                        appendix='_longinstruction'
                        
                    print('\n')
                    print('{')
                    print('uint32_t tracepointlocation = ((uint32_t)APPTRACEPOINT'+str(i)+'*2);\n')
                    print('pagenum = tracepointlocation/2/128;\n')
                    print('pageoffset = tracepointlocation%256;\n')
                    print('addUserTracePoint'+appendix+'( pagenum,  pageoffset);\n')
                    print('}')
                    print('\n')
                continue

            data = re.search('USERTRACEACTION', myline)
            if data!=None:
                print myline
                state = 'addtraceaction'

                for i in range(len(globalset)):
                    if i >0:
                        appendixif = 'else '
                    else:
                        appendixif = ''
                    print('\n')
                    print(appendixif + 'if ( regsource == APPTRACEPOINT'+str(i)+'COMPARE )\n')
                    print('{\n')
                    print(' value = MEMORY_UINT16(2000);\n')
                    #print('trace[currentindex++] = 2;\n')
                    #print('trace[currentindex++] = 12;\n')
                    #print('trace[currentindex++] = 0;\n')
                    #print('trace[currentindex++] = APPTRACEPOINT'+str(i)+'COMPARE%256;\n')
                    #print('trace[currentindex++] = APPTRACEPOINT'+str(i)+'COMPARE/256;\n')

                    print('trace[currentindex++] = value%256;\n')
                    print('trace[currentindex++] = value/256;\n')
                    
                    print('}\n')
                    
                continue
            
            print myline
            
                
        elif state == 'patchdef':
            data = re.search('PATCHDEFEND', myline)
            if data!=None:
                print myline
                state = 'normal'
                continue
            else:
                continue

        elif state == 'apptrace':
            data = re.search('APPTRACEEND', myline)
            if data!=None:
                print myline
                state = 'normal'
                continue
            else:
                continue

        elif state == 'appjump':
            data = re.search('APPJUMPEND', myline)
            if data!=None:
                print myline
                state = 'normal'
                continue
            else:
                continue
        elif state =='cleantrace':
            data = re.search('CLEANTRACEEND', myline)
            if data != None:
                print myline
                state = 'normal'
                continue
            else:
                continue
        elif state =='cleandef':
            data = re.search('CLEANDEFEND', myline)
            if data != None:
                print myline
                state = 'normal'
                continue
            else:
                continue
        elif state =='addtrace':
            data = re.search('ADDTRACEEND', myline)
            if data != None:
                print myline
                state = 'normal'
                continue
            else:
                continue
        elif state == 'addtraceaction':
            data = re.search('USERTRACEACTIONEND', myline)
            if data!=None:
                print myline
                state = 'normal'
                continue
            else:
                continue


    #for i in range(len(globalset)):
     #   generateOutputData(outputfile, i, globalset[i][1])


        
        
