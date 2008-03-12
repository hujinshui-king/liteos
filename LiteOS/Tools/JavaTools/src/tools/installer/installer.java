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



package tools.installer;

import java.io.*;  // needed for BufferedReader, InputStreamReader, etc.
import java.util.*;
import java.awt.event.*;

/**
 * Created by IntelliJ IDEA.
 * User: Qing Cao
 * Date: Aug 31, 2007
 * Time: 10:27:55 PM
 * To change this template use File | Settings | File Templates.
 */
public class installer {

  private static byte[] command = new byte[320];
  private static byte[] response = new byte[320];
  private static int responsecount;

  public static void main(String [] args) throws IOException {
    //first analyze the args and find the COM
    //example: java installer COM5 target.lhex

    //Decide to get all data from interactive asking. This should be better.
    //    if ((args.length !=2)&&(args.length !=3))
    //    {   colorOutput.print(colorOutput.COLOR_BRIGHT_RED, "Syntax error!\n");
    //        return;
    //    }

        if ((args.length == 1)&&(args[0].compareTo("-color")==0))
          colorOutput.setColor(true);

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,  "Starting the LiteOS installer...");
        colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,  "Please set up the following configuration parameters:");

        colorOutput.println(colorOutput.COLOR_YELLOW, "Please input the port number you intend to use: (e.g. COM1)");
        String commportA, commportB;

        commportA = commportB = null;
        try {
           commportA  = br.readLine();
         }
        catch (IOException ioe) {
         System.out.println("Unexpected IO error. The installer now exits.");
         System.exit(1);
       }

       int portNum = 0;
       try{
         if (commportA.length() > 4)
            portNum = Integer.parseInt( commportA.substring(3, commportA.length())) - 1;
           else
         portNum = Integer.parseInt( Character.toString(commportA.charAt(commportA.length()-1))) - 1;
       }
       catch (Exception io)
       {
           colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "Unexepected IO error. The installer now exits.");
           System.exit(1);
       }

       if (commportA.length() > 4)
       commportB = commportA.substring(0, commportA.length()-2) + (new Integer(portNum+2)).toString();
      else
       commportB = commportA.substring(0, commportA.length()-1) + (new Integer(portNum+2)).toString(); 


      String tempanswer = null;

      int mib510inuse = 0;

      int mib520inuse = 0;


      while (true)
      {
       colorOutput.print(colorOutput.COLOR_YELLOW, "Are you using the MIB510 or MIB520 programmer? (1 (or MIB510) and 2 (or MIB520))");

         try {
           tempanswer  = br.readLine();
         } catch (IOException ioe) {
         System.out.println("Error trying to set the programming board. The installer now terminates..");
         System.exit(1);
       }



       if ((tempanswer.compareTo("1")==0)||((tempanswer.compareTo("MIB510")==0)))
       { mib510inuse = 1;
         mib520inuse = 0;

       }
      else
       if ((tempanswer.compareTo("2")==0)||((tempanswer.compareTo("MIB520")==0)))
       {
           mib520inuse = 1;
           mib510inuse = 0;
       }
      else
       {colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "The answer to the previous question must be either 1, 2 or MIB510, MIB520. ");
      // System.exit(1);
        continue;
       }

          break;
      }


      colorOutput.println(colorOutput.COLOR_YELLOW, "Please input the node ID (0 for the base station and 1-255 for other nodes).");
      String tempid = null;

              try {
                tempid  = br.readLine();
              } catch (IOException ioe) {
              System.out.println("Error trying to read node ID. The installer is terminated now.");
              System.exit(1);
            }

      int startnodeid =0;
      try {
          startnodeid = Integer.parseInt(tempid);
      }
      catch (NumberFormatException e)
      {
           colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "Unexpected number format error. Exit now. ");
           System.exit(1);
      }

          colorOutput.println(colorOutput.COLOR_YELLOW, "Please input the channel for communication. (11-26, only used by the kernel installation)");

         try {
                tempid  = br.readLine();
              } catch (IOException ioe) {
              System.out.println("Error trying to read node ID. The installer is terminated now.");
              System.exit(1);
            }

      int nodechannel = 15;
      try {
          nodechannel = Integer.parseInt(tempid);
      }
      catch (NumberFormatException e)
      {
           colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "Unexpected number format error. Exit now. ");
           System.exit(1);
      }

      while(true)
      {

       String binaryimage = null;

          while (true)
          {
              colorOutput.println(colorOutput.COLOR_YELLOW, "Please input the binary image name (ending with hex). Leave blank or CTRL-C to exit");
              try {
                binaryimage  = br.readLine();
                if (binaryimage.compareTo("")== 0)
                {
                     colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN,  "Installation completes. Bye.");
                     System.exit(0);

                }

                File file = new File(binaryimage);

                boolean exist = file.exists();
                if (exist == false)
                     colorOutput.println(colorOutput.COLOR_BRIGHT_RED,  "The file you input does not exist in the current directory. Please copy the hex file" +
                             "to the current directory and then proceed.");
                else
                   break;
              } catch (IOException ioe) {
              System.out.println("IO error trying to read the file name.");
              System.exit(1);
             }
            catch (NullPointerException eo)
            {
              System.out.println("Unexpected pointer error. The installer now exits.");
              System.exit(1);
            }
          }


        colorOutput.println(colorOutput.COLOR_YELLOW,  "Now installing the image into the mote...");

        String commandinstall =  "uisp -dprog=mib510 -dserial=/dev/ttyS" + portNum + " --wr_fuse_h=0x19 -dpart=ATmega128 --wr_fuse_e=ff  --erase --upload if=" + binaryimage;
    //   System.out.println(commandinstall);
        try
		{
			Runtime systemShell = Runtime.getRuntime();
			Process shellOutput = systemShell.exec(commandinstall);
			InputStreamReader isr = new InputStreamReader(shellOutput.getInputStream());
			BufferedReader breader = new BufferedReader (isr);
			String line = null;
			//System.out.println("<INSTALL SCRIPT OUTPUT>");

		  //	while((line = breader.readLine()) != null )
		  	//{
		  	//	System.out.println(line);
	      	//	}

		  	//System.out.println("</INSTALL SCRIPT OUTPUT>");

		    int exitVal = shellOutput.waitFor();

		   //System.out.println("Process Exit Value : "+exitVal);

            if (exitVal != 0)
            {   colorOutput.println(colorOutput.COLOR_BRIGHT_RED,  "Installation fails. Here is the diagnosis information:");
                colorOutput.println(colorOutput.COLOR_BRIGHT_RED,  "<Diagnosis info starts>");

		     	while((line = breader.readLine()) != null )
		  	   {
		  	    colorOutput.println(colorOutput.COLOR_BRIGHT_RED,  line);
              	}

		  	    colorOutput.println(colorOutput.COLOR_BRIGHT_RED,  "<Diagnosis info ends>");

                System.exit(1);
            }


            colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "First step Installation complete.");
        }
		catch (Exception e)
		{
            colorOutput.println(colorOutput.COLOR_BRIGHT_RED,  "System failure, below is the stack trace");
            e.printStackTrace();
            System.exit(1);
        }



        String ans = null;
        colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "Do you want to proceed with kernel configuration? (y/n, this step is not needed if you are installing the base station or generic hex applications.)");

             try {
                ans  = br.readLine();
              } catch (IOException ioe) {
              System.out.println("IO error trying to read the answer.");
              System.exit(1);
            }

            if (ans.compareTo("n")==0)
            {

             colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "Installation finishes. Bye!");
             System.exit(0);
            //  System.exit(0);
            }

      //Now start asking a few questions

      //what is the subnetwork name

      String networkname = null;
      String nodename = null;
      String nodeid = null;

      colorOutput.println(colorOutput.COLOR_YELLOW, "Please input the network name that this node belongs to. (no more than 16 letters/digits.)");

      //  read the username from the command-line; need to use try/catch with the
      //  readLine() method
      try {
         networkname = br.readLine();
      } catch (IOException ioe) {
         System.out.println("IO error trying to read your input. The installer now exits. ");
         System.exit(1);
      }


      //what is the node name
      colorOutput.println(colorOutput.COLOR_YELLOW, "Input the name of this node: (1-16 letters/digits. You may use IP addresses, such as 192.168.0.1., or more intuitive names, such as MyNodeA, etc.)");

      try {
         nodename = br.readLine();
      } catch (IOException ioe) {
         System.out.println("IO error trying to read your input.");
         System.exit(1);
      }

      //what is the node id
      //send out the command

//      String commport = args[0];

          //Now set up the port
       if (mib520inuse == 1)
                  commportA = commportB;

// change the device name to /dev/ttySn in Linux environment
// haven't tested with usb port
         if (System.getProperty("os.name").equals("Linux")) 
            commportA = "/dev/ttyS" + portNum;

//                 System.out.println(""+commportA);

                 PortListener pl = new PortListener(commportA, 57600);

                 try {
                     pl.open();
                 }
                 catch (javax.comm.NoSuchPortException e) {
                 //   System.out.println("No such port. System exit now. ");

                    colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "No such port. Probably the programming board is not connected. Please check your device manager to see the current available ports.");
                    System.exit(1);
                 }
                 catch (javax.comm.PortInUseException e) {
                    //System.out.println("Port in use. System exit now. ");

                 colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "Port in use. Probably another installer is not terminated or another application is accessing this port.");
                 System.exit(1);
                 }
                 catch (javax.comm.UnsupportedCommOperationException e) {
                 //System.out.println("Comm not supported");
                 colorOutput.println(colorOutput.COLOR_BRIGHT_RED, "Port operation not supported. This is probably due to driver problems. Make sure you have updated the latest drivers for the port.");
                  System.exit(1);
                 }


      pl.setThreshold(12);

      byte[] zerobytes = new byte[100];
      for (int i=0; i<100; i++)
        zerobytes[i]  =  0;

      byte[] networknamebytes = networkname.getBytes();
      System.arraycopy(networknamebytes, 0, command, 0, networknamebytes.length);

      if (networknamebytes.length < 16)
        System.arraycopy(zerobytes, 0, command, networknamebytes.length,  16-networknamebytes.length);

      byte[] nodenamebytes = nodename.getBytes();
      System.arraycopy(nodenamebytes, 0, command, 16,  nodenamebytes.length);

      if (nodenamebytes.length < 16)
        System.arraycopy(zerobytes, 0, command, nodenamebytes.length + 16, 16-nodenamebytes.length);


      command[31] = new Integer(nodechannel).byteValue();
      command[32] = new Integer(startnodeid).byteValue();


      pl.write(command);


      //format and reboot continue
      colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "Now reboot. The mote should turn on all three LEDs during reboot, and turn them off when reboot finishes. If the mote fails to turn on all three LEDs, reboot it manually, or reinstall the mote.");


    //  System.out.println("Thanks for the  " + networkname + " "+  nodename + " " + nodeid);
      //installation complete


   //    pl.read();
   //    responsecount = pl.getCount();
   //    response = pl.getData();

      // System.out.println("Now get "+responsecount +"bytes");
     colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "Installation complete. Bye.");



      /*   colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "Continue with the next node? y/n");
         try {
           tempanswer  = br.readLine();
         } catch (IOException ioe) {
         System.out.println("IO error trying to read your name!");
         System.exit(1);
       }

      if (tempanswer.compareTo("y") == 0)
        continue;

      else
        break;

    }   */
   break;}
   //colorOutput.println(colorOutput.COLOR_BRIGHT_GREEN, "Installation finishes. ");

    System.exit(0);
  }

}
