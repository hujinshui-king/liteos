/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, 2007-2008, University of Illinois , qcao2@uiuc.edu

LiteOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LiteOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LiteOS.  If not, see <http://www.gnu.org/licenses/>.
*/




package tools.terminal;

import java.util.ArrayList;

/**
 * The du command class that allows the current directory information to be displayed.
 */
public class continueCommand  {


    private byte[] reply = new byte[64];


    public int setNewCommand(String[] options, int optioncount, String [] parameters, int parametercount, fileDirectory fdir, breakpointCommand breakpointhandle) {

        fileNode currentnode = fdir.getCurrentNode();
        int currentAddress = currentnode.getNodeAddress();
        int numOfBreaks = breakpointhandle.getNumOfBreakpoint();
        int i;
        int temp = 0;
        byte[] patchinstructions = new byte[6];
        boolean patchinitilized = false;

        String numparameter;

        if (parameters[0].startsWith("0x")==true)
           numparameter = parameters[0].substring(2);
        else
           numparameter = parameters[0];


        for (i=0;i<numOfBreaks; i++)
        {
             temp = breakpointhandle.getBreakPointAddr(i);
             if (temp == Integer.parseInt(numparameter, 16))
               break;
        }

        //if (i==numOfBreaks)
        //{     System.out.println("The breakpoint is not found!");
         // return 0;
        //}


        if (i<numOfBreaks)
        {
             System.arraycopy(breakpointhandle.getInstructions(i), 0, patchinstructions, 0, 6);
             patchinitilized = true;
        }

        if (parametercount > 1)
        {
            System.out.println("Reading from file to patch instructions.. Not implemented yet");
            return 0;
        }

        if (patchinitilized == false)
        return 0;

        int addr = temp;

        addr = addr/2;

        int addrhigh = addr/256;
        int addrlow = addr%256;


        reply[0] = (byte) 12;
        reply[1] = (byte) 93;
        reply[2] = (new Integer(currentAddress)).byteValue();
        reply[3] = (new Integer(addrhigh)).byteValue();
        reply[4] =  (new Integer(addrlow)).byteValue();
        System.arraycopy(patchinstructions, 0, reply, 5, 6);

        breakpointhandle.removeBreakPointAddr(addr*2);

        //System.arraycopy(filename, 0, reply, 3, filename.length);

        //reply[3+filename.length] = (byte)0;

        return 1;  //To change body of implemented methods use File | Settings | File Templates.
    }

    //Return the  delay in milliseconds
    public int getDelay() {
        return 600;  //To change body of implemented methods use File | Settings | File Templates.
    }

    //return the command will be used
    public byte[] getNewCommand(int index) {
        return reply;
    }


    public void handleresponse(String[] options, int optioncount, String [] parameters, int parametercount, ArrayList reply, fileDirectory fdir) {


        fileNode cnode = fdir.getCurrentNode();
        String nodeName = cnode.getNodeName();
        int addr;
        byte [] response  = null;
        int start = 5;

        while (reply.size() > 0)
        {
            response = (byte [])reply.remove(0);
            if (response[start + 1]!=93)
            continue;

            if (response[ start + 3] == 0)
             System.out.println("The specified breakpoint is not found.");
        }
      }
}

