/*
The following is the license of LiteOS.

This file is part of LiteOS.
Copyright Qing Cao, Hossein Ahmadi 2007-2009, University of Illinois , qcao2@uiuc.edu

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
package tools.tools;

import javax.comm.*;
import java.io.InputStream;

/**
 * @author Hossein Ahmadi
 * 			SerialListener class provides a recording mechanism to record serial 
 * 			port logs comming from LiteOS Kernel and Application. Given an specific
 * 			port, it displays the mote serial output and add timestamp to each line  
 */

public class SerialListener implements SerialPortEventListener  {
	private InputStream stream;
	private boolean newline;
	private String port_name;
	
	public SerialListener(SerialPort port, String port_name) throws java.util.TooManyListenersException, java.io.IOException {
		port.addEventListener(this);
		port.notifyOnDataAvailable(true);
		stream = port.getInputStream();
		newline = true;
		this.port_name = port_name;
	}
	
	public synchronized void serialEvent(SerialPortEvent ev) throws java.io.IOException {			
		while (stream.available() > 0) {
			char c = (char)stream.read();
			if (newline) {
				System.out.print(System.currentTimeMillis() + " " + port_name + " ");
				newline = false;
			}
			if (c == '\n')
				newline = true;
			System.out.print(c);
		}
	}
	
	public static void main(String args[]) throws Exception {
		if (args.length < 1) {
			System.out.println("Usage: SerialListener [Port Name e.g. COM4]\n");
			return;
		}
		
		CommPortIdentifier portId = CommPortIdentifier.getPortIdentifier(args[0]);
		SerialPort port = (SerialPort) portId.open("SerialListener", 0);
		port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
		port.disableReceiveFraming();
		port.setSerialPortParams(57600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

		SerialListener listener = new SerialListener(port, args[0]);
		while (true) 
			Thread.sleep(100000);		
	}
}