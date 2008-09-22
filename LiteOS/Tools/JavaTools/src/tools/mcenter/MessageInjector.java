/*                                                                      tab:4
 *
 *
 * "Copyright (c) 2000-2002 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose, without fee, and without written
 * agreement is hereby granted, provided that the above copyright
 * notice, the following two paragraphs and the author appear in all
 * copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY
 * PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
 * CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
 * UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Authors:  Philip Levis  <pal@cs.berkeley.edu>
 *
 */

/**
 * MessageInjector is a Java GUI for filling in and sending TinyOS
 * packets.
 * @author Philip Levis <pal@cs.berkeley.edu>
 */

package tools.mcenter;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import tools.message.*;
import tools.util.TinyLook;
import tools.util.LimitedStyledDocument;

public class MessageInjector extends MessageCenterInternalFrame {
	private JScrollPane pane;
	private JPanel buttonPanel;
	private JPanel nestedPanel;
	private MessageSelectionPanel selection;
	private JMenuBar menuBar;
	private SerialConnector sConnector;

	public MessageInjector() {
		super("TinyOS Message Injector");
		TinyLook.setLookAndFeel(this);
		try {
			selection = new MessageSelectionPanel();
			sConnector = SerialConnector.instance();
			initialize();
		} catch (Exception exception) {
			exception.printStackTrace();
		}
	}

	private void initialize() {
		nestedPanel = new JPanel();
		nestedPanel.setForeground(Color.black);
		nestedPanel.setLayout(new BoxLayout(nestedPanel, BoxLayout.Y_AXIS));

		selection.setForeground(Color.black);

		JScrollPane pane = new JScrollPane(selection);
		Dimension size = pane.getPreferredSize();
		if (size.getHeight() > 380) {
			size.setSize(size.getWidth(), 400);
		} else {
			size.setSize(size.getWidth(), size.getHeight() + 20);
		}
		if (size.getWidth() > 980) {
			size.setSize(1000, size.getHeight());
		} else {
			size.setSize(size.getWidth() + 20, size.getHeight());
		}
		pane.setPreferredSize(size);
		nestedPanel.add(pane);

		buttonPanel = new ButtonPanel(selection, sConnector, this);
		nestedPanel.add(buttonPanel);
		getContentPane().add(nestedPanel);

		menuBar = new MainMenuBar(this);
		menuBar.setFont(TinyLook.defaultFont());
		setJMenuBar(menuBar);
		pack();
		setVisible(true);
	}

	private class ButtonPanel extends JPanel {
		MessageSelectionPanel panel;
		SerialConnector sConnector;

		JButton quitButton;
		JLabel label;
		JTextPane text;
		JButton reconnectButton;
		JButton sendButton;

		public ButtonPanel(MessageSelectionPanel panel,
				SerialConnector sConnector, MessageInjector injector) {
			this.panel = panel;
			this.sConnector = sConnector;
			this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));

			label = new JLabel("Mote ID");
			label.setForeground(Color.black);

			text = new JTextPane(new LimitedStyledDocument(4));
			text.setText("ffff");

			sendButton = new JButton("Send");
			sendButton.addActionListener(new InjectListener(panel, sConnector,
					text));

			add(sendButton);
			add(label);
			add(text);
		}

	}

	protected class InjectListener implements ActionListener {
		private MessageSelectionPanel panel;
		private SerialConnector sConnector;
		private JTextPane text;

		public InjectListener(MessageSelectionPanel panel,
				SerialConnector sConnector, JTextPane text) {
			this.panel = panel;
			this.sConnector = sConnector;
			this.text = text;
		}

		public void actionPerformed(ActionEvent e) {
			try {
				int moteID = Integer.parseInt(text.getText(), 16);
				Message msg = panel.getMessage();
				sConnector.sendMessage(moteID, (short) msg.amType(), (short) 0,
						msg.dataGet());
				System.out.println("Injecting packet to address 0x"
						+ text.getText() + ":\n" + msg);
				System.out.print("Raw bytes: ");
				byte[] data = msg.dataGet();
				for (int i = 0; i < data.length; i++) {
					String datum = Integer.toHexString((int) (data[i] & 0xff));
					if (datum.length() < 2) {
						datum = "0" + datum;
					}
					if (datum.length() < 2) {
						datum = "0" + datum;
					}
					datum += " ";
					System.out.print(datum);
				}
				System.out.println();
			} catch (Exception exception) {
				System.err.println(exception);
			}
		}
	}

	protected class MainMenuBar extends JMenuBar implements ActionListener {

		private MessageInjector injector;
		private JMenu fileMenu;
		private JMenuItem quitMenuItem;
		private MessageSelectionPanel selection;

		public MainMenuBar(MessageInjector injector) {
			this.injector = injector;

			fileMenu = new JMenu("File");

			quitMenuItem = new JMenuItem("Quit");
			quitMenuItem.addActionListener(this);
			quitMenuItem.setFont(TinyLook.defaultFont());
			fileMenu.add(quitMenuItem);

			addMenu(fileMenu);
		}

		public void actionPerformed(ActionEvent e) {
			if (e.getActionCommand() == "Quit") {
				injector.dispose();
			}
		}

		private void addMenu(JMenu menu) {
			menu.setFont(TinyLook.labelFont());
			add(menu);
		}
	}

}
