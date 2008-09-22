/*
 * Copyright (c) 2003, Vanderbilt University
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE VANDERBILT UNIVERSITY BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE VANDERBILT
 * UNIVERSITY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE VANDERBILT UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE VANDERBILT UNIVERSITY HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

package tools.mcenter;

import java.awt.event.*;
import tools.packet.*;
import tools.message.*;

/**
 * 
 * @author nadand
 */
public class SerialConnector extends javax.swing.JInternalFrame {

	private tools.packet.PacketSource serialStub;
	private MessageFactory messageFactory;
	private int action = 0x0; // 0x0: default, 0x1: local, 0x2: server, 0x3 :
								// illegal
	private long msgsent = 0;
	private long msgreceived = 0;
	private long msgadded = 0;
	private long msgposted = 0;

	private PacketReader packetReader;
	private DistributorThread distributorThread;

	private java.util.Vector packetBuffer = new java.util.Vector();

	private java.util.HashMap messageIdRegisterMap = new java.util.HashMap();
	private java.util.HashMap clientRegisterMap = new java.util.HashMap();

	static private SerialConnector _instance = null;

	private ItemRadioButton lastSelected;

	private short maxMsgLength = 36;
	private short group = 0;

	public static final int GET_ALL_MESSAGES = -1;

	private static final int PACKET_ADDRESS_FIELD = 0;
	private static final int PACKET_TYPE_FIELD = 2;
	private static final int PACKET_GROUP_FIELD = 3;
	private static final int PACKET_LENGTH_FIELD = 4;
	private static final int PACKET_DATA_BEGIN = 5;
	private static final int PACKET_CRC_SIZE = 2;

	private java.util.prefs.Preferences prefsRoot = java.util.prefs.Preferences
			.userNodeForPackage(this.getClass());
	private java.util.prefs.Preferences prefs;

	/** Creates new Singleton SerialConnector */
	protected SerialConnector() {
		initComponents();
		prefs = prefsRoot.node(prefsRoot.absolutePath() + "/SerialConnector");
		this.comPortTextField.setText(prefs.get("ComPort", "COM1"));
		this.comSpeedTextField.setText(prefs.get("ComSpeed", "57600"));
		// this.sComPortTextField.setText(prefs.get("SyncedComPort","COM1"));
		//this.sComPortSpeedTextField.setText(prefs.get("SyncedComSpeed","57600"
		// ));
		this.ipAddressTextField.setText(prefs.get("IpAddress", "127.0.0.1"));
		this.ipPortTextField.setText(prefs.get("IpPort", "9000"));

		this.tossimRadioAddressTextField.setText(prefs.get("TossimIP",
				"127.0.0.1"));
		this.tossimSerialAddressTextField.setText(prefs.get("TossimSerialIP",
				"127.0.0.1"));
		this.otherConnTextField.setText(prefs.get("OtherString", ""));

		// this.lhPortTextField.setText(prefs.get("LocalPort","9000"));
		this.groupTextField.setText(prefs.get("GroupId", "0x7D"));

		buttonGroup1.add(this.connectionRadioButtonCOM);
		// buttonGroup1.add(this.connectionRadioButtonSCOM);
		buttonGroup1.add(this.connectionRadioButtonIP);
		buttonGroup1.add(this.connectionRadioButtonTossim);
		buttonGroup1.add(this.connectionRadioButtonTossimSerial);
		buttonGroup1.add(this.connectionRadioButtonOther);
		// buttonGroup1.add(this.connectionRadioButtonLH);

		setRBstate(false);
		lastSelected = (ItemRadioButton) connectionRadioButtonCOM;
		this.buttonGroup1.setSelected(this.connectionRadioButtonCOM.getModel(),
				true);
	}

	/************************** Singleton Pattern "snstructor" **********************/
	static public SerialConnector instance() {
		if (null == _instance) {
			_instance = new SerialConnector();
		}
		return _instance;
	}

	private void setRBstate(boolean enabled) {
		this.comPortTextField.setEnabled(enabled);
		this.comSpeedTextField.setEnabled(enabled);
		this.comPortLabel.setEnabled(enabled);
		this.speedLabel.setEnabled(enabled);
		// this.sComPortTextField.setEnabled(enabled);
		// this.sComPortSpeedTextField.setEnabled(enabled);
		// this.sComPortLabel.setEnabled(enabled);
		// this.sComPortSpeedLabel.setEnabled(enabled);
		this.ipLabel.setEnabled(enabled);
		this.ipAddressTextField.setEnabled(enabled);
		this.ipPortTextField.setEnabled(enabled);
		this.portLabel.setEnabled(enabled);

		this.tossimRadioAddressTextField.setEnabled(enabled);
		this.tossimSerialAddressTextField.setEnabled(enabled);
		this.otherConnTextField.setEnabled(enabled);
		this.tossimSerialLabel.setEnabled(enabled);
		this.tossimRadioLabel.setEnabled(enabled);
		this.otherConnLabel.setEnabled(enabled);

	}

	private void setRBenable(ItemRadioButton radioButton, boolean enabled) {
		int newState = (enabled) ? ItemEvent.SELECTED : ItemEvent.DESELECTED;
		radioButton.changeEnabledState(new ItemEvent(radioButton,
				ItemEvent.ITEM_STATE_CHANGED, radioButton, newState));
	}

	public short getMsgLength() {
		return maxMsgLength;
	}

	/**
	 * This method is called from within the constructor to initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is always
	 * regenerated by the Form Editor.
	 */
	private void initComponents() {// GEN-BEGIN:initComponents
		java.awt.GridBagConstraints gridBagConstraints;

		buttonGroup1 = new javax.swing.ButtonGroup();
		mainPanel = new javax.swing.JPanel();
		connectionRadioButtonCOM = new ItemRadioButton();
		comPortLabel = new javax.swing.JLabel();
		comPortTextField = new javax.swing.JTextField();
		speedLabel = new javax.swing.JLabel();
		comSpeedTextField = new javax.swing.JTextField();
		connectionRadioButtonIP = new ItemRadioButton();
		ipLabel = new javax.swing.JLabel();
		ipAddressTextField = new javax.swing.JTextField();
		portLabel = new javax.swing.JLabel();
		ipPortTextField = new javax.swing.JTextField();
		placeholderjLabel = new javax.swing.JLabel();
		jSeparator1 = new javax.swing.JSeparator();
		packetSizeLabel = new javax.swing.JLabel();
		packetSizeTextField = new javax.swing.JTextField();
		groupLabel = new javax.swing.JLabel();
		groupTextField = new javax.swing.JTextField();
		jSeparator2 = new javax.swing.JSeparator();
		oldProtocolCheckBox = new javax.swing.JCheckBox();
		threadingCheckBox = new javax.swing.JCheckBox();
		connectionRadioButtonTossim = new ItemRadioButton();
		connectionRadioButtonTossimSerial = new ItemRadioButton();
		connectionRadioButtonOther = new ItemRadioButton();
		tossimRadioLabel = new javax.swing.JLabel();
		tossimRadioAddressTextField = new javax.swing.JTextField();
		tossimSerialLabel = new javax.swing.JLabel();
		tossimSerialAddressTextField = new javax.swing.JTextField();
		otherConnLabel = new javax.swing.JLabel();
		otherConnTextField = new javax.swing.JTextField();
		messagePanel = new javax.swing.JPanel();
		sentLabel = new javax.swing.JLabel();
		sentTextField = new javax.swing.JTextField();
		receivedLabel = new javax.swing.JLabel();
		receivedTextField = new javax.swing.JTextField();
		controlPanel = new javax.swing.JPanel();
		ssButton = new javax.swing.JButton();

		setIconifiable(true);
		setMaximizable(true);
		setResizable(true);
		setTitle("SerialConnector");
		mainPanel.setLayout(new java.awt.GridBagLayout());

		mainPanel.setBorder(new javax.swing.border.SoftBevelBorder(
				javax.swing.border.BevelBorder.RAISED));
		connectionRadioButtonCOM.setText("Serial Port");
		connectionRadioButtonCOM
				.addItemListener(new java.awt.event.ItemListener() {
					public void itemStateChanged(java.awt.event.ItemEvent evt) {
						connectionRadioButtonCOMItemStateChanged(evt);
					}
				});

		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridwidth = 2;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.anchor = java.awt.GridBagConstraints.NORTHWEST;
		mainPanel.add(connectionRadioButtonCOM, gridBagConstraints);

		comPortLabel
				.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		comPortLabel.setText("port");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.anchor = java.awt.GridBagConstraints.EAST;
		mainPanel.add(comPortLabel, gridBagConstraints);

		comPortTextField.setText("COM1");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(comPortTextField, gridBagConstraints);

		speedLabel.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		speedLabel.setText("speed");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.anchor = java.awt.GridBagConstraints.EAST;
		mainPanel.add(speedLabel, gridBagConstraints);

		comSpeedTextField.setText("57600");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(comSpeedTextField, gridBagConstraints);

		connectionRadioButtonIP.setText("Remote Server");
		connectionRadioButtonIP
				.addItemListener(new java.awt.event.ItemListener() {
					public void itemStateChanged(java.awt.event.ItemEvent evt) {
						connectionRadioButtonIPItemStateChanged(evt);
					}
				});

		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 0;
		gridBagConstraints.gridy = 2;
		gridBagConstraints.gridwidth = 2;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(connectionRadioButtonIP, gridBagConstraints);

		ipLabel.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		ipLabel.setText("address");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 2;
		gridBagConstraints.gridy = 2;
		gridBagConstraints.anchor = java.awt.GridBagConstraints.EAST;
		mainPanel.add(ipLabel, gridBagConstraints);

		ipAddressTextField.setText("127.0.0.1");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 3;
		gridBagConstraints.gridy = 2;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(ipAddressTextField, gridBagConstraints);

		portLabel.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		portLabel.setText("port");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 4;
		gridBagConstraints.gridy = 2;
		gridBagConstraints.anchor = java.awt.GridBagConstraints.EAST;
		mainPanel.add(portLabel, gridBagConstraints);

		ipPortTextField.setText("9000");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 5;
		gridBagConstraints.gridy = 2;
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(ipPortTextField, gridBagConstraints);

		placeholderjLabel
				.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 5;
		gridBagConstraints.gridy = 3;
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		mainPanel.add(placeholderjLabel, gridBagConstraints);

		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 0;
		gridBagConstraints.gridy = 7;
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(jSeparator1, gridBagConstraints);

		packetSizeLabel
				.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		packetSizeLabel.setText("packet size");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.weightx = 1.0;
		mainPanel.add(packetSizeLabel, gridBagConstraints);

		packetSizeTextField.setText("36");
		packetSizeTextField
				.addActionListener(new java.awt.event.ActionListener() {
					public void actionPerformed(java.awt.event.ActionEvent evt) {
						packetSizeTextFieldActionPerformed(evt);
					}
				});

		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridwidth = 2;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.weightx = 1.0;
		mainPanel.add(packetSizeTextField, gridBagConstraints);

		groupLabel.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		groupLabel.setText("group ID");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.weightx = 1.0;
		mainPanel.add(groupLabel, gridBagConstraints);

		groupTextField.setText("0x7D");
		groupTextField.setMinimumSize(new java.awt.Dimension(80, 20));
		groupTextField.setPreferredSize(new java.awt.Dimension(80, 20));
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.anchor = java.awt.GridBagConstraints.WEST;
		gridBagConstraints.weightx = 1.0;
		mainPanel.add(groupTextField, gridBagConstraints);

		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(jSeparator2, gridBagConstraints);

		oldProtocolCheckBox.setText("Old Serial Protocol (No Framing)");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(oldProtocolCheckBox, gridBagConstraints);

		threadingCheckBox.setText("Eanble Threading");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(threadingCheckBox, gridBagConstraints);

		connectionRadioButtonTossim.setText("Tossim Radio");
		connectionRadioButtonTossim
				.addItemListener(new java.awt.event.ItemListener() {
					public void itemStateChanged(java.awt.event.ItemEvent evt) {
						connectionRadioButtonTossimItemStateChanged(evt);
					}
				});

		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 0;
		gridBagConstraints.gridy = 5;
		gridBagConstraints.gridwidth = 2;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(connectionRadioButtonTossim, gridBagConstraints);

		connectionRadioButtonTossimSerial.setText("Tossim Serial");
		connectionRadioButtonTossimSerial
				.addItemListener(new java.awt.event.ItemListener() {
					public void itemStateChanged(java.awt.event.ItemEvent evt) {
						connectionRadioButtonTossimSerialItemStateChanged(evt);
					}
				});

		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 0;
		gridBagConstraints.gridy = 4;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(connectionRadioButtonTossimSerial, gridBagConstraints);

		connectionRadioButtonOther.setText("Other Connection");
		connectionRadioButtonOther
				.addItemListener(new java.awt.event.ItemListener() {
					public void itemStateChanged(java.awt.event.ItemEvent evt) {
						connectionRadioButtonOtherItemStateChanged(evt);
					}
				});

		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 0;
		gridBagConstraints.gridy = 6;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(connectionRadioButtonOther, gridBagConstraints);

		tossimRadioLabel
				.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		tossimRadioLabel.setText("address");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 2;
		gridBagConstraints.gridy = 5;
		gridBagConstraints.anchor = java.awt.GridBagConstraints.EAST;
		mainPanel.add(tossimRadioLabel, gridBagConstraints);

		tossimRadioAddressTextField.setText("127.0.0.1");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 3;
		gridBagConstraints.gridy = 5;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(tossimRadioAddressTextField, gridBagConstraints);

		tossimSerialLabel
				.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		tossimSerialLabel.setText("address");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 2;
		gridBagConstraints.gridy = 4;
		gridBagConstraints.anchor = java.awt.GridBagConstraints.EAST;
		mainPanel.add(tossimSerialLabel, gridBagConstraints);

		tossimSerialAddressTextField.setText("127.0.0.1");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 3;
		gridBagConstraints.gridy = 4;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(tossimSerialAddressTextField, gridBagConstraints);

		otherConnLabel
				.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		otherConnLabel.setText("string");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 2;
		gridBagConstraints.gridy = 6;
		gridBagConstraints.anchor = java.awt.GridBagConstraints.EAST;
		mainPanel.add(otherConnLabel, gridBagConstraints);

		otherConnTextField.setText("127.0.0.1");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridx = 3;
		gridBagConstraints.gridy = 6;
		gridBagConstraints.gridwidth = 3;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		mainPanel.add(otherConnTextField, gridBagConstraints);

		getContentPane().add(mainPanel, java.awt.BorderLayout.NORTH);

		messagePanel.setLayout(new java.awt.GridBagLayout());

		messagePanel.setBorder(new javax.swing.border.TitledBorder(null,
				"Message counter", javax.swing.border.TitledBorder.CENTER,
				javax.swing.border.TitledBorder.DEFAULT_POSITION));
		sentLabel.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		sentLabel.setText("sent");
		sentLabel.setToolTipText("sent");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.weightx = 1.0;
		messagePanel.add(sentLabel, gridBagConstraints);

		sentTextField.setEditable(false);
		sentTextField.setHorizontalAlignment(javax.swing.JTextField.TRAILING);
		sentTextField.setText("0");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.weightx = 1.0;
		messagePanel.add(sentTextField, gridBagConstraints);

		receivedLabel
				.setHorizontalAlignment(javax.swing.SwingConstants.TRAILING);
		receivedLabel.setText("received");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.weightx = 1.0;
		messagePanel.add(receivedLabel, gridBagConstraints);

		receivedTextField.setEditable(false);
		receivedTextField
				.setHorizontalAlignment(javax.swing.JTextField.TRAILING);
		receivedTextField.setText("0");
		gridBagConstraints = new java.awt.GridBagConstraints();
		gridBagConstraints.gridwidth = java.awt.GridBagConstraints.REMAINDER;
		gridBagConstraints.fill = java.awt.GridBagConstraints.HORIZONTAL;
		gridBagConstraints.weightx = 1.0;
		messagePanel.add(receivedTextField, gridBagConstraints);

		getContentPane().add(messagePanel, java.awt.BorderLayout.CENTER);

		controlPanel.setLayout(new java.awt.GridBagLayout());

		ssButton.setText("Start Port");
		ssButton.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(java.awt.event.ActionEvent evt) {
				ssButtonActionPerformed(evt);
			}
		});

		controlPanel.add(ssButton, new java.awt.GridBagConstraints());

		getContentPane().add(controlPanel, java.awt.BorderLayout.WEST);

		pack();
	}// GEN-END:initComponents

	private void connectionRadioButtonTossimItemStateChanged(
			java.awt.event.ItemEvent evt) {// GEN-FIRST:
											// event_connectionRadioButtonTossimItemStateChanged
		if (evt.getStateChange() == ItemEvent.SELECTED) {
			this.tossimRadioLabel.setEnabled(true);
			this.tossimRadioAddressTextField.setEnabled(true);
			lastSelected = (ItemRadioButton) connectionRadioButtonTossim;

		} else if (evt.getStateChange() == ItemEvent.DESELECTED) {
			this.tossimRadioLabel.setEnabled(false);
			this.tossimRadioAddressTextField.setEnabled(false);

		}
	}// GEN-LAST:event_connectionRadioButtonTossimItemStateChanged

	private void connectionRadioButtonOtherItemStateChanged(
			java.awt.event.ItemEvent evt) {// GEN-FIRST:
											// event_connectionRadioButtonOtherItemStateChanged
		if (evt.getStateChange() == ItemEvent.SELECTED) {
			this.otherConnLabel.setEnabled(true);
			this.otherConnTextField.setEnabled(true);
			lastSelected = (ItemRadioButton) connectionRadioButtonOther;

		} else if (evt.getStateChange() == ItemEvent.DESELECTED) {
			this.otherConnLabel.setEnabled(false);
			this.otherConnTextField.setEnabled(false);

		}
	}// GEN-LAST:event_connectionRadioButtonOtherItemStateChanged

	private void connectionRadioButtonTossimSerialItemStateChanged(
			java.awt.event.ItemEvent evt) {// GEN-FIRST:
											// event_connectionRadioButtonTossimSerialItemStateChanged
		if (evt.getStateChange() == ItemEvent.SELECTED) {
			this.tossimSerialLabel.setEnabled(true);
			this.tossimSerialAddressTextField.setEnabled(true);
			lastSelected = (ItemRadioButton) connectionRadioButtonTossimSerial;

		} else if (evt.getStateChange() == ItemEvent.DESELECTED) {
			this.tossimSerialLabel.setEnabled(false);
			this.tossimSerialAddressTextField.setEnabled(false);

		}
	}// GEN-LAST:event_connectionRadioButtonTossimSerialItemStateChanged

	private void packetSizeTextFieldActionPerformed(
			java.awt.event.ActionEvent evt) {// GEN-FIRST:
												// event_packetSizeTextFieldActionPerformed
		try {
			maxMsgLength = (short) (Integer.parseInt(packetSizeTextField
					.getText()) & 0xff);
		} catch (Exception e) {
			this.packetSizeTextField.setText(Integer.toString(maxMsgLength));
		}

	}// GEN-LAST:event_packetSizeTextFieldActionPerformed

	private void ssButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-
																			// FIRST
																			// :
																			// event_ssButtonActionPerformed

		if (this.ssButton.getText().equalsIgnoreCase("Start Port")) {

			prefs.put("ComPort", this.comPortTextField.getText());
			prefs.put("ComSpeed", this.comSpeedTextField.getText());
			// prefs.put("SyncedComPort",this.sComPortTextField.getText());
			//prefs.put("SyncedComSpeed",this.sComPortSpeedTextField.getText());
			prefs.put("IpAddress", this.ipAddressTextField.getText());
			prefs.put("IpPort", this.ipPortTextField.getText());
			prefs.put("GroupId", this.groupTextField.getText());

			prefs.put("TossimIP", this.tossimRadioAddressTextField.getText());
			prefs.put("TossimSerialIP", this.tossimSerialAddressTextField
					.getText());
			prefs.put("OtherString", this.otherConnTextField.getText());

			String value = groupTextField.getText();
			if (value.trim().toUpperCase().startsWith("0X"))
				group = Short.parseShort(value.trim().substring(2), 16);
			else
				group = Short.parseShort(value.trim());

			try {
				String connection = "";
				this.connectionRadioButtonCOM.setEnabled(false);
				// this.connectionRadioButtonSCOM.setEnabled(false);
				this.connectionRadioButtonIP.setEnabled(false);
				this.connectionRadioButtonTossim.setEnabled(false);
				this.connectionRadioButtonTossimSerial.setEnabled(false);
				this.connectionRadioButtonOther.setEnabled(false);
				this.oldProtocolCheckBox.setEnabled(false);
				this.threadingCheckBox.setEnabled(false);

				// this.connectionRadioButtonLH.setEnabled(false);

				setRBstate(false);

				// javax.swing.ButtonModel selected =
				// this.buttonGroup1.getSelection();
				if (this.connectionRadioButtonCOM == lastSelected) {
					if (oldProtocolCheckBox.isSelected())
						connection += "old-";
					connection += "serial@"
							+ this.comPortTextField.getText().trim() + ":"
							+ this.comSpeedTextField.getText().trim();
					if (oldProtocolCheckBox.isSelected())
						connection += "," + maxMsgLength;
				} else if (this.connectionRadioButtonIP == lastSelected) {
					connection += "sf@"
							+ this.ipAddressTextField.getText().trim() + ":"
							+ this.ipPortTextField.getText().trim();
				} else if (this.connectionRadioButtonTossimSerial == lastSelected) {
					connection += "tossim-serial@"
							+ this.tossimSerialAddressTextField.getText()
									.trim();
				} else if (this.connectionRadioButtonTossim == lastSelected) {
					connection += "tossim-radio@"
							+ this.tossimRadioAddressTextField.getText().trim();
				} else if (this.connectionRadioButtonOther == lastSelected) {
					connection = otherConnTextField.getText().trim();
				} else {
					throw new Exception();
				}

				System.out.println("Connecting to: " + connection);

				serialStub = BuildSource.makePacketSource(connection);

				serialStub.open(tools.util.PrintStreamMessenger.err);

				/*
				 * javax.swing.ButtonModel selected =
				 * this.buttonGroup1.getSelection();
				 * 
				 * 
				 * if(this.connectionRadioButtonCOM.getModel() == selected)
				 * serialStub = new
				 * SerialPortStub(this.comPortTextField.getText().trim(),
				 * maxMsgLength,
				 * Integer.parseInt(this.comSpeedTextField.getText()));
				 * if(this.connectionRadioButtonSCOM.getModel() == selected)
				 * serialStub = new
				 * SyncedCRCSerialPortStub(this.sComPortTextField
				 * .getText().trim(), maxMsgLength,
				 * Integer.parseInt(this.sComPortSpeedTextField.getText()));
				 * if(this.connectionRadioButtonIP.getModel() == selected)
				 * serialStub = new
				 * SerialForwarderStub(ipAddressTextField.getText(),
				 * Integer.parseInt(ipPortTextField.getText()), maxMsgLength);
				 * if(this.connectionRadioButtonLH.getModel() == selected)
				 * serialStub = new SerialForwarderStub("",
				 * Integer.parseInt(lhPortTextField.getText()), maxMsgLength);
				 * 
				 * 
				 * 
				 * serialStub.Open();
				 */
				distributorThread = new SerialConnector.DistributorThread();
				distributorThread.start();
				packetReader = new SerialConnector.PacketReader();
				packetReader.start();
				// serialStub.registerPacketListener(packetReader);

				this.ssButton.setText("Stop Port");
				System.out.println("Connected.");
				messageFactory = new MessageFactory(serialStub);
			} catch (Exception e) {
				System.err.println("Could not connect to local port: "
						+ e.toString());
				this.connectionRadioButtonCOM.setEnabled(true);
				// this.connectionRadioButtonSCOM.setEnabled(true);
				this.connectionRadioButtonIP.setEnabled(true);
				this.connectionRadioButtonTossim.setEnabled(true);
				this.connectionRadioButtonTossimSerial.setEnabled(true);
				this.connectionRadioButtonOther.setEnabled(true);
				// this.connectionRadioButtonLH.setEnabled(true);
				this.oldProtocolCheckBox.setEnabled(true);
				this.threadingCheckBox.setEnabled(true);

				this.setRBenable(lastSelected, true);
				this.ssButton.setText("Start Port");

			}

		} else if (this.ssButton.getText().equalsIgnoreCase("Stop Port")) {
			try {
				this.connectionRadioButtonCOM.setEnabled(true);
				// this.connectionRadioButtonSCOM.setEnabled(true);
				this.connectionRadioButtonIP.setEnabled(true);
				this.connectionRadioButtonTossim.setEnabled(true);
				this.connectionRadioButtonTossimSerial.setEnabled(true);
				this.connectionRadioButtonOther.setEnabled(true);

				this.oldProtocolCheckBox.setEnabled(true);
				this.threadingCheckBox.setEnabled(true);

				// this.connectionRadioButtonLH.setEnabled(true);
				this.setRBenable(lastSelected, true);

				packetReader.stopRun();
				distributorThread.end();
				serialStub.close();
				System.out.println("Connection closed.");

				this.ssButton.setText("Start Port");

			} catch (Exception e) {
				System.err
						.println("Could not terminate connection to local port: "
								+ e.toString());
			}

		}
	}// GEN-LAST:event_ssButtonActionPerformed

	private void connectionRadioButtonIPItemStateChanged(
			java.awt.event.ItemEvent evt) {// GEN-FIRST:
											// event_connectionRadioButtonIPItemStateChanged
		if (evt.getStateChange() == ItemEvent.SELECTED) {
			this.ipLabel.setEnabled(true);
			this.ipAddressTextField.setEnabled(true);
			this.ipPortTextField.setEnabled(true);
			this.portLabel.setEnabled(true);
			lastSelected = (ItemRadioButton) connectionRadioButtonIP;

		} else if (evt.getStateChange() == ItemEvent.DESELECTED) {
			this.ipLabel.setEnabled(false);
			this.ipAddressTextField.setEnabled(false);
			this.ipPortTextField.setEnabled(false);
			this.portLabel.setEnabled(false);
		}
	}// GEN-LAST:event_connectionRadioButtonIPItemStateChanged

	private void connectionRadioButtonCOMItemStateChanged(
			java.awt.event.ItemEvent evt) {// GEN-FIRST:
											// event_connectionRadioButtonCOMItemStateChanged
		if (evt.getStateChange() == ItemEvent.SELECTED) {
			this.comPortTextField.setEnabled(true);
			this.comSpeedTextField.setEnabled(true);
			this.comPortLabel.setEnabled(true);
			this.speedLabel.setEnabled(true);
			lastSelected = (ItemRadioButton) connectionRadioButtonCOM;

		} else if (evt.getStateChange() == ItemEvent.DESELECTED) {
			this.comPortTextField.setEnabled(false);
			this.comSpeedTextField.setEnabled(false);
			this.comPortLabel.setEnabled(false);
			this.speedLabel.setEnabled(false);

		}
	}// GEN-LAST:event_connectionRadioButtonCOMItemStateChanged

	// Variables declaration - do not modify//GEN-BEGIN:variables
	private javax.swing.ButtonGroup buttonGroup1;
	private javax.swing.JLabel comPortLabel;
	private javax.swing.JTextField comPortTextField;
	private javax.swing.JTextField comSpeedTextField;
	private javax.swing.JRadioButton connectionRadioButtonCOM;
	private javax.swing.JRadioButton connectionRadioButtonIP;
	private javax.swing.JRadioButton connectionRadioButtonOther;
	private javax.swing.JRadioButton connectionRadioButtonTossim;
	private javax.swing.JRadioButton connectionRadioButtonTossimSerial;
	private javax.swing.JPanel controlPanel;
	private javax.swing.JLabel groupLabel;
	private javax.swing.JTextField groupTextField;
	private javax.swing.JTextField ipAddressTextField;
	private javax.swing.JLabel ipLabel;
	private javax.swing.JTextField ipPortTextField;
	private javax.swing.JSeparator jSeparator1;
	private javax.swing.JSeparator jSeparator2;
	private javax.swing.JPanel mainPanel;
	private javax.swing.JPanel messagePanel;
	private javax.swing.JCheckBox oldProtocolCheckBox;
	private javax.swing.JLabel otherConnLabel;
	private javax.swing.JTextField otherConnTextField;
	private javax.swing.JLabel packetSizeLabel;
	private javax.swing.JTextField packetSizeTextField;
	private javax.swing.JLabel placeholderjLabel;
	private javax.swing.JLabel portLabel;
	private javax.swing.JLabel receivedLabel;
	private javax.swing.JTextField receivedTextField;
	private javax.swing.JLabel sentLabel;
	private javax.swing.JTextField sentTextField;
	private javax.swing.JLabel speedLabel;
	private javax.swing.JButton ssButton;
	private javax.swing.JCheckBox threadingCheckBox;
	private javax.swing.JTextField tossimRadioAddressTextField;
	private javax.swing.JLabel tossimRadioLabel;
	private javax.swing.JTextField tossimSerialAddressTextField;
	private javax.swing.JLabel tossimSerialLabel;

	// End of variables declaration//GEN-END:variables

	/********************************* Message sendIF ******************************/

	public void registerPacketListener(PacketListenerIF packetListener,
			int messageID) {
		java.util.HashSet listenerList;
		if (messageIdRegisterMap.containsKey(new Integer(messageID))) {
			listenerList = (java.util.HashSet) this.messageIdRegisterMap
					.get(new Integer(messageID));
		} else {
			listenerList = new java.util.HashSet();
			this.messageIdRegisterMap.put(new Integer(messageID), listenerList);
		}
		if (clientRegisterMap.containsKey(packetListener)) {

			ClientThread clientHandler = (ClientThread) clientRegisterMap
					.get(packetListener);
			clientHandler.references++;
		} else {
			ClientThread newClientHandler = new ClientThread(packetListener);
			clientRegisterMap.put(packetListener, newClientHandler);
			newClientHandler.references++;
			newClientHandler.start();
		}

		listenerList.add(packetListener);

	}

	public boolean removePacketListener(PacketListenerIF packetListener,
			int messageID) {
		java.util.HashSet listenerList;
		if (messageIdRegisterMap.containsKey(new Integer(messageID))) {
			listenerList = (java.util.HashSet) this.messageIdRegisterMap
					.get(new Integer(messageID));
		} else {
			return false;
		}
		if (clientRegisterMap.containsKey(packetListener)) {
			ClientThread clientHandler = (ClientThread) clientRegisterMap
					.get(packetListener);
			if (--clientHandler.references == 0) {
				clientHandler.end();
				clientRegisterMap.remove(packetListener);
			}
		}

		if (listenerList.remove(packetListener)) {
			if (listenerList.isEmpty())
				messageIdRegisterMap.remove(new Integer(messageID));
			return true;
		}
		return false;
	}

	public boolean removePacketListener(
			MessageCenterInternalFrame packetListener) {
		java.util.HashSet listenerList;
		java.util.Iterator mapIt = messageIdRegisterMap.keySet().iterator();
		while (mapIt.hasNext()) {
			Object key = mapIt.next();
			if (messageIdRegisterMap.containsKey(key)) {
				listenerList = (java.util.HashSet) this.messageIdRegisterMap
						.get(key);
			} else {
				continue;
			}
			if (clientRegisterMap.containsKey(packetListener)) {
				ClientThread clientHandler = (ClientThread) clientRegisterMap
						.get(packetListener);
				if (--clientHandler.references == 0) {
					clientHandler.end();
					clientRegisterMap.remove(packetListener);
				}
			}

			if (listenerList.remove(packetListener)) {
				if (listenerList.isEmpty())
					messageIdRegisterMap.remove(key);

			}

		}
		return true;
	}

	public boolean sendMessage(int address, short type, byte[] data) {
		if (data.length > Integer.parseInt(packetSizeTextField.getText()))
			return false;

		TOSMsg tosPacket = messageFactory.createTOSMsg(36);
		// address
		tosPacket.set_addr(address);
		// System.out.println("address "+messageAdress);
		// type
		tosPacket.set_type(type);
		// System.out.println("type "+pollActiveMessage);
		// Group
		tosPacket.set_group(this.group);
		// System.out.println("group "+groupId);
		// Length
		tosPacket.set_length((short) data.length);
		// System.out.println("length "+(short)data.length);
		// data
		tosPacket.dataSet(data, 0, tosPacket.offset_data(0), data.length);
		// CRC
		tosPacket.set_crc(1);

		try {
			serialStub.writePacket(tosPacket.dataGet());
			sentTextField.setText(Long.toString(++msgsent));

			return true;

		} catch (java.io.IOException ioe) {
			System.err.println("I/O Exception while sending message:"
					+ ioe.getMessage());
		} catch (java.lang.NullPointerException npe) {
			System.err.println("Cannot send message! Port not connected!");
		}
		return false;

	}

	public boolean sendMessage(int address, short type,
			short dgroup /* deprecated */, byte[] data) {
		return sendMessage(address, type, data);
	}

	/****************************** Inner Classes ***********************************/
	private class ItemRadioButton extends javax.swing.JRadioButton {
		public ItemRadioButton() {
		}

		public void changeEnabledState(java.awt.event.ItemEvent itemEvent) {
			this.fireItemStateChanged(itemEvent);
		}
	}

	protected class DistributorThread extends Thread {

		byte[] packet;

		PacketListenerIF destination;
		boolean running = true;

		public void end() {
			running = false;
		}

		public void run() {

			while (running) {
				if (!packetBuffer.isEmpty()) {
					distributeBuffer();
				} else {
					try {
						// System.out.println("Sleeping");
						Thread.sleep(100);
					} catch (InterruptedException ie) {
					}
				}
			}
		}

		private synchronized void distributeBuffer() {
			while (!packetBuffer.isEmpty()) {
				// System.out.println("Buffer Size: " + packetBuffer.size());
				packet = (byte[]) packetBuffer.get(0);
				packetBuffer.remove(0);

				java.util.HashSet toNotify = new java.util.HashSet();

				if (messageIdRegisterMap.containsKey(new Integer(
						GET_ALL_MESSAGES))) {
					toNotify.addAll((java.util.HashSet) messageIdRegisterMap
							.get(new Integer(GET_ALL_MESSAGES)));
				}
				if (messageIdRegisterMap.containsKey(new Integer(
						packet[PACKET_TYPE_FIELD]))) {
					toNotify.addAll((java.util.HashSet) messageIdRegisterMap
							.get(new Integer(packet[PACKET_TYPE_FIELD])));
				}

				java.util.Iterator notifyListIterator = toNotify.iterator();

				while (notifyListIterator.hasNext()) {

					PacketListenerIF packetIF = (PacketListenerIF) notifyListIterator
							.next();
					if (clientRegisterMap.containsKey(packetIF)) {
						ClientThread clientHandler = (ClientThread) clientRegisterMap
								.get(packetIF);
						clientHandler.forwardPacketReceived(packet, packetIF);
						msgadded++;
					}
				}
			}
		}

	}

	protected class ClientThread extends Thread {

		// byte[] packet;
		private java.util.Vector messageBuffer;
		PacketListenerIF destination;
		boolean running = true;
		public int references = 0;

		/*
		 * public ClientThread(){ }
		 */

		public ClientThread(PacketListenerIF destination) {
			this.destination = destination;
			messageBuffer = new java.util.Vector();
		}

		public synchronized void forwardPacketReceived(byte[] packet,
				PacketListenerIF destination) {
			messageBuffer.add(packet);
			// this.packet = packet;
			// this.destination = destination;

		}

		public void end() {
			running = false;
		}

		public void run() {
			while (running)
				if (!messageBuffer.isEmpty()) {
					sendBuffer();
				} else {
					try {
						Thread.sleep(100);
					} catch (InterruptedException ie) {
					}
				}
		}

		private synchronized void sendBuffer() {
			while (!this.messageBuffer.isEmpty()) {
				this.destination.packetReceived((byte[]) this.messageBuffer
						.get(0));
				this.messageBuffer.remove(0);
				msgposted++;
				// System.out.println("recvd:"+msgreceived+" added:"+msgadded+
				// "posted:"+msgposted);
			}
		}

	}

	protected class PacketReader extends Thread implements PacketListenerIF {
		boolean run = true;
		byte[] buffer = null;

		public void run() {
			try {
				while (run) {
					buffer = serialStub.readPacket();
					packetReceived(buffer);
				}
			} catch (Exception e) {
			}
		}

		public void stopRun() {
			run = false;
		}

		// the first 5 bytes are: addr(2), type(1), group(1), length(1)
		public void packetReceived(byte[] packet) {
			// packetBuffer.add(packet);
			receivedTextField.setText(Long.toString(++msgreceived));
			// System.out.println("Type: " +Integer.toHexString(0xFF &
			// packet[2]));

			java.util.HashSet toNotify = new java.util.HashSet();

			if (messageIdRegisterMap.containsKey(new Integer(GET_ALL_MESSAGES))) {
				toNotify.addAll((java.util.HashSet) messageIdRegisterMap
						.get(new Integer(GET_ALL_MESSAGES)));
			}
			if (messageIdRegisterMap.containsKey(new Integer(
					packet[PACKET_TYPE_FIELD]))) {
				toNotify.addAll((java.util.HashSet) messageIdRegisterMap
						.get(new Integer(packet[PACKET_TYPE_FIELD])));
			}
			java.util.Iterator notifyListIterator = toNotify.iterator();
			while (notifyListIterator.hasNext()) {
				((PacketListenerIF) notifyListIterator.next())
						.packetReceived(packet);
				/*
				 * PacketListenerIF packetIF
				 * =(PacketListenerIF)notifyListIterator.next();
				 * if(clientRegisterMap.containsKey(packetIF)){ ClientThread
				 * clientHandler =
				 * (ClientThread)clientRegisterMap.get(packetIF);
				 * clientHandler.forwardPacketReceived(packet,packetIF);
				 * msgadded++; }
				 */

			}

		}

	}

}
