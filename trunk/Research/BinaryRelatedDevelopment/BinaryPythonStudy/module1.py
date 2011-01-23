import os
import commands

def printall():
    print commands.getoutput('pwd')
    os.chdir("/opt/tinyos-1.x/apps/Blink.old")
    #print commands.getoutput('cd ..')
    print commands.getoutput('pwd')
    print commands.getoutput('make micaz')
    return


if __name__ == "__main__":
    printall()