/*
 * analyzer.java
 *
 * Created on July 26, 2008, 8:49 PM
 */



/**
 *
 * @author  Qing Cao
 */

 import java.text.NumberFormat;
 import java.util.Locale;


    
    
public class analyzer extends javax.swing.JFrame {
 
    //this section sets all the details on the parameters
    private int settingscenario = 1; 
    private double range, width, density, beta, velocity, cycle; 

    
    
    //this section performs all the calculations 
    
    private double mFunc(double beta, double velocity, double cycle, double range)
    {
      return (1-beta)*velocity*cycle*Math.sqrt(range*range-((1-beta)*velocity*cycle/2)*((1-beta)*velocity*cycle/2))-2*range*range*Math.acos((1-beta)*velocity*cycle/(2*range));
    }
    
    
    public String analyzeResults(double range, double width, double density, double beta, double velocity, double cycle, int setting)
    {
       String labelText; 
       String targetType = ""; 
       String settingType = ""; 
       
       int typeString = 0; 
       String probString = "";
       String delayString = ""; 
       
      // String probTextString = "   <LI> The detection probability is to be reported";
      // String delayTextString = "   <LI> The average detection delay is to reported";
       
       double prob = 0;
       double delay = 0; 
       
        
       
       double threshold = 2*range/((1-beta)*cycle); 
       double a = (1-beta)*velocity*cycle/2; 
       
       
       if (velocity <= threshold) 
           targetType = "slow";
       else
           targetType = "fast"; 
            
       if (setting == 1)
           settingType = "I"; 
       else if (setting == 2)
           settingType = "II"; 
       else if (setting == 3)
           settingType = "III"; 
       else if (setting == 4)
           settingType = "IV"; 
        
       
       
       if ((setting == 1) && (velocity > threshold))
       {
         prob = 1 - Math.exp(-2*range*width*density*(beta+Math.PI*range/(2*velocity*cycle)));
         typeString = 1; 
         
       }
       else
        if ((setting == 1) && (velocity <= threshold))
       {
         prob = 1 - Math.exp(-2*range*width*density*(beta+(Math.PI*range*range + mFunc(beta, velocity, cycle, range))/(2*range*velocity*cycle)));
         typeString = 1; 
        
       }   
        
       else if ((setting == 4) && (velocity > threshold))
       {
         prob = 1 - Math.exp(-(2*range*width+Math.PI*range*range/2)*density*(beta+  Math.PI*range*width/((2*width+Math.PI*range/2)*velocity*cycle)));
         
         typeString = 1; 
         
       }
       else
        if ((setting == 4) && (velocity <= threshold))
       {
         prob = 1 - Math.exp(-(2*range*width+Math.PI*range*range/2)*density*(beta+ ( Math.PI*range*range*width+( Math.min(((width - a)*mFunc(beta, velocity, cycle, range)), 0)))/((2*width*range+Math.PI*range*range/2)*velocity*cycle)));
         typeString = 1; 
        
       }   
       
        else
        if ((setting == 2) && (velocity > threshold))
        {
        
          delay = Math.exp(-beta*Math.PI*range*range*density)/((2*range*beta*velocity + Math.PI*range*range/cycle)*density);          
          typeString = 2; 
        }
        else
        if ((setting == 2) && (velocity <= threshold))    
        {
          delay = Math.exp(-beta*Math.PI*range*range*density)/((2*range*beta*velocity + Math.PI*range*range/cycle)*density);
          delay = delay*(1 - mFunc(beta, velocity, cycle, range)*Math.exp(- (2*range*beta*velocity*cycle + Math.PI*range*range)*(1-beta)*density)/(2*range*beta*velocity*cycle + Math.PI*range*range + mFunc(beta, velocity, cycle, range))); 
          typeString = 2; 
        
        }           
        else
        if ((setting == 3) && (velocity > threshold))
        {
          delay = Math.exp(-beta*Math.PI*range*range*density/2)/((2*range*beta*velocity + Math.PI*range*range/cycle)*density);          
          typeString = 2; 
        }
        else
        if ((setting == 3) && (velocity <= threshold))    
       {
          delay = Math.exp(-beta*Math.PI*range*range*density/2)/((2*range*beta*velocity + Math.PI*range*range/cycle)*density);
          delay = delay*(1 - mFunc(beta, velocity, cycle, range)*Math.exp(- (2*range*beta*velocity*cycle + Math.PI*range*range)*(1-beta)*density/2)/(2*range*beta*velocity*cycle + Math.PI*range*range + mFunc(beta, velocity, cycle, range))); 
          typeString = 2; 
        
        } 
       if (typeString == 1)
       {
       Double percent = new Double(prob);
       NumberFormat percentFormatter;
       String percentOut;

       percentFormatter = NumberFormat.getPercentInstance(new Locale("en","US"));
       percentOut = percentFormatter.format(percent);

       probString = "the target has a probability of "+percentOut +" to be detected";
       }

       if (typeString == 2)
       {
       NumberFormat nf = NumberFormat.getInstance();
       nf.setMaximumFractionDigits(5);
       nf.setMinimumFractionDigits(5);
       String s = nf.format(delay);
       delayString = "the target has an estimated average detection delay of "+s +"s before detected";
       }
       
       labelText =
       "<HTML><BR><BR><P>";
       labelText +=    
      "<FONT SIZE= \"4 \"  FACE = \" Verdana \" >" ;
       labelText+= "Our results show that ";
       labelText+= "<FONT COLOR=RED><B>"; 
       
      //"  <LI>The target is classified as a " + targetType +" target"+
      //"  <LI>The scenario is classified as a type " + settingType +" scenario" ;
       
       
       if (typeString == 1)
       {
        //   labelText+=probTextString;
           labelText+=probString; 
       }
       
       if (typeString == 2)
       {
        //   labelText+=delayTextString;
           labelText+=delayString; 
       
       }
       
      
       
      labelText+="</B></FONT></FONT></P></HTML>";
      return labelText; 
    
    
    }   
    
    /** Creates new form analyzer */
    public analyzer() {
        initComponents();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jDialog1 = new javax.swing.JDialog();
        javax.swing.JLabel appTitleLabel = new javax.swing.JLabel();
        javax.swing.JLabel versionLabel = new javax.swing.JLabel();
        javax.swing.JLabel appVersionLabel = new javax.swing.JLabel();
        javax.swing.JLabel vendorLabel = new javax.swing.JLabel();
        javax.swing.JLabel appVendorLabel = new javax.swing.JLabel();
        javax.swing.JLabel homepageLabel = new javax.swing.JLabel();
        javax.swing.JLabel appHomepageLabel = new javax.swing.JLabel();
        javax.swing.JLabel appDescLabel = new javax.swing.JLabel();
        javax.swing.JLabel imageLabel = new javax.swing.JLabel();
        jPanel2 = new javax.swing.JPanel();
        caseButton_I = new javax.swing.JRadioButton();
        caseButton_II = new javax.swing.JRadioButton();
        caseButton_III = new javax.swing.JRadioButton();
        caseButton_IV = new javax.swing.JRadioButton();
        jLabel1 = new javax.swing.JLabel();
        jPanel3 = new javax.swing.JPanel();
        jLabel3 = new javax.swing.JLabel();
        sensingrange_field = new javax.swing.JTextField();
        jLabel4 = new javax.swing.JLabel();
        density_field = new javax.swing.JTextField();
        jLabel5 = new javax.swing.JLabel();
        dutycycle_field = new javax.swing.JTextField();
        jLabel6 = new javax.swing.JLabel();
        dutycycleratio_field = new javax.swing.JTextField();
        jLabel7 = new javax.swing.JLabel();
        speed_field = new javax.swing.JTextField();
        jLabel8 = new javax.swing.JLabel();
        width_field = new javax.swing.JTextField();
        jPanel4 = new javax.swing.JPanel();
        results_label = new javax.swing.JLabel();
        jButton1 = new javax.swing.JButton();
        jMenuBar1 = new javax.swing.JMenuBar();
        jMenu1 = new javax.swing.JMenu();
        jMenuItem1 = new javax.swing.JMenuItem();
        jScrollPane1 = new javax.swing.JScrollPane();
        jEditorPane1 = new javax.swing.JEditorPane();
        jMenuBar2 = new javax.swing.JMenuBar();
        jMenu2 = new javax.swing.JMenu();
        jMenuItem2 = new javax.swing.JMenuItem();

        jDialog1.setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        jDialog1.setTitle("About: Document Editor 1.0 ");
        jDialog1.setModal(true);
        jDialog1.setName("aboutBox"); // NOI18N
        jDialog1.setResizable(false);

        appTitleLabel.setFont(appTitleLabel.getFont().deriveFont(appTitleLabel.getFont().getStyle() | java.awt.Font.BOLD, appTitleLabel.getFont().getSize()+4));
        appTitleLabel.setText("Document Editor");

        versionLabel.setFont(versionLabel.getFont().deriveFont(versionLabel.getFont().getStyle() | java.awt.Font.BOLD));
        versionLabel.setText("Product Version:");

        appVersionLabel.setText("1.0");

        vendorLabel.setFont(vendorLabel.getFont().deriveFont(vendorLabel.getFont().getStyle() | java.awt.Font.BOLD));
        vendorLabel.setText("Vendor:");

        appVendorLabel.setText("Sun Microsystems Inc");

        homepageLabel.setFont(homepageLabel.getFont().deriveFont(homepageLabel.getFont().getStyle() | java.awt.Font.BOLD));
        homepageLabel.setText("Homepage:");

        appHomepageLabel.setText("http://appframework.dev.java.net");

        appDescLabel.setText("<html>A simple text editor based on the Swing Application Framework ");

        org.jdesktop.layout.GroupLayout jDialog1Layout = new org.jdesktop.layout.GroupLayout(jDialog1.getContentPane());
        jDialog1.getContentPane().setLayout(jDialog1Layout);
        jDialog1Layout.setHorizontalGroup(
            jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jDialog1Layout.createSequentialGroup()
                .add(imageLabel)
                .add(18, 18, 18)
                .add(jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, jDialog1Layout.createSequentialGroup()
                        .add(jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(versionLabel)
                            .add(vendorLabel)
                            .add(homepageLabel))
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(appVersionLabel)
                            .add(appVendorLabel)
                            .add(appHomepageLabel)))
                    .add(org.jdesktop.layout.GroupLayout.LEADING, appTitleLabel)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, appDescLabel, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 269, Short.MAX_VALUE))
                .addContainerGap())
        );
        jDialog1Layout.setVerticalGroup(
            jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(imageLabel, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .add(jDialog1Layout.createSequentialGroup()
                .addContainerGap()
                .add(appTitleLabel)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(appDescLabel)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(versionLabel)
                    .add(appVersionLabel))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(vendorLabel)
                    .add(appVendorLabel))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jDialog1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(homepageLabel)
                    .add(appHomepageLabel))
                .addContainerGap(53, Short.MAX_VALUE))
        );

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setResizable(false);

        jPanel2.setBorder(javax.swing.BorderFactory.createTitledBorder("Analysis Settings"));

        caseButton_I.setSelected(true);
        caseButton_I.setText("Case I, a target crosses a sensor network covered area");
        caseButton_I.setBorder(javax.swing.BorderFactory.createEmptyBorder(0, 0, 0, 0));
        caseButton_I.setMargin(new java.awt.Insets(0, 0, 0, 0));
        caseButton_I.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                caseButton_IActionPerformed(evt);
            }
        });

        caseButton_II.setText("Case II, a target moves within a sensor network area");
        caseButton_II.setBorder(javax.swing.BorderFactory.createEmptyBorder(0, 0, 0, 0));
        caseButton_II.setMargin(new java.awt.Insets(0, 0, 0, 0));
        caseButton_II.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                caseButton_IIActionPerformed(evt);
            }
        });

        caseButton_III.setText("Case III, a target enters a sensor network covered area");
        caseButton_III.setBorder(javax.swing.BorderFactory.createEmptyBorder(0, 0, 0, 0));
        caseButton_III.setMargin(new java.awt.Insets(0, 0, 0, 0));
        caseButton_III.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                caseButton_IIIActionPerformed(evt);
            }
        });

        caseButton_IV.setText("Case IV, a target leaves a sensor network covered area");
        caseButton_IV.setBorder(javax.swing.BorderFactory.createEmptyBorder(0, 0, 0, 0));
        caseButton_IV.setMargin(new java.awt.Insets(0, 0, 0, 0));
        caseButton_IV.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                caseButton_IVActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout jPanel2Layout = new org.jdesktop.layout.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .add(jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(caseButton_I)
                    .add(caseButton_II)
                    .add(caseButton_III)
                    .add(caseButton_IV))
                .addContainerGap(176, Short.MAX_VALUE))
        );

        jPanel2Layout.linkSize(new java.awt.Component[] {caseButton_I, caseButton_II, caseButton_III, caseButton_IV}, org.jdesktop.layout.GroupLayout.HORIZONTAL);

        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .add(caseButton_I, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(caseButton_II, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 15, Short.MAX_VALUE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(caseButton_III, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 15, Short.MAX_VALUE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(caseButton_IV, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 15, Short.MAX_VALUE))
        );

        jLabel1.setFont(new java.awt.Font("Tahoma", 1, 14));
        jLabel1.setText("Sensor Network Detection Performance Analyzer");

        jPanel3.setBorder(javax.swing.BorderFactory.createTitledBorder("Parameter Settings"));

        jLabel3.setText("Sensing range (m):");

        sensingrange_field.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
        sensingrange_field.setText("10");

        jLabel4.setText("Density (per sensing range):");

        density_field.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
        density_field.setText("20");

        jLabel5.setText("Duty cycle (s):");

        dutycycle_field.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
        dutycycle_field.setText("10");

        jLabel6.setText("Duty cycle ratio:");

        dutycycleratio_field.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
        dutycycleratio_field.setText("0.05");

        jLabel7.setText("Target speed (m/s):");

        speed_field.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
        speed_field.setText("10");

        jLabel8.setText("Area width (m):");

        width_field.setHorizontalAlignment(javax.swing.JTextField.RIGHT);
        width_field.setText("30");

        org.jdesktop.layout.GroupLayout jPanel3Layout = new org.jdesktop.layout.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jLabel3)
                    .add(jLabel5)
                    .add(jLabel7))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING, false)
                    .add(jPanel3Layout.createSequentialGroup()
                        .add(sensingrange_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 69, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                        .add(jLabel4)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(density_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 80, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                    .add(jPanel3Layout.createSequentialGroup()
                        .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                            .add(jPanel3Layout.createSequentialGroup()
                                .add(dutycycle_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 69, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                                .add(jLabel6))
                            .add(jPanel3Layout.createSequentialGroup()
                                .add(speed_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 69, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                                .add(jLabel8)))
                        .add(62, 62, 62)
                        .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING, false)
                            .add(width_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                            .add(dutycycleratio_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))))
                .addContainerGap())
        );

        jPanel3Layout.linkSize(new java.awt.Component[] {density_field, dutycycleratio_field, width_field}, org.jdesktop.layout.GroupLayout.HORIZONTAL);

        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jLabel3)
                    .add(jLabel4)
                    .add(sensingrange_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(density_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jLabel5)
                    .add(dutycycle_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jLabel6)
                    .add(dutycycleratio_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .add(3, 3, 3)
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jLabel7)
                    .add(speed_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jLabel8)
                    .add(width_field, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel3Layout.linkSize(new java.awt.Component[] {density_field, dutycycle_field, dutycycleratio_field, jLabel3, jLabel4, jLabel5, jLabel6, jLabel7, jLabel8, sensingrange_field, speed_field, width_field}, org.jdesktop.layout.GroupLayout.VERTICAL);

        jPanel4.setBorder(javax.swing.BorderFactory.createTitledBorder("Analysis Results"));

        results_label.setFont(new java.awt.Font("Tahoma", 0, 14)); // NOI18N
        results_label.setVerticalAlignment(javax.swing.SwingConstants.TOP);

        org.jdesktop.layout.GroupLayout jPanel4Layout = new org.jdesktop.layout.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, results_label, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 475, Short.MAX_VALUE)
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(results_label, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 151, Short.MAX_VALUE)
        );

        jButton1.setText("Analyze");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt);
            }
        });

        jMenu1.setText("Help");
        jMenu1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenu1ActionPerformed(evt);
            }
        });

        jMenuItem1.setText("About");
        jMenuItem1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem1ActionPerformed(evt);
            }
        });
        jMenu1.add(jMenuItem1);

        jMenuBar1.add(jMenu1);

        jScrollPane1.setBorder(javax.swing.BorderFactory.createEmptyBorder(1, 1, 1, 1));

        jEditorPane1.setBackground(new java.awt.Color(244, 244, 244));
        jEditorPane1.setEditable(false);
        jEditorPane1.setText("This analyzer is developed based on the results from the paper \"Analysis of Target Detection Performance for Wireless Sensor Networks\", by Q. Cao, Y. Ting, J. Stankovic, T. Abdelzaher");
        jScrollPane1.setViewportView(jEditorPane1);

        jMenu2.setText("Help");
        jMenu2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenu1ActionPerformed(evt);
            }
        });

        jMenuItem2.setText("About");
        jMenuItem2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem1ActionPerformed(evt);
            }
        });
        jMenu2.add(jMenuItem2);

        jMenuBar2.add(jMenu2);

        setJMenuBar(jMenuBar2);

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, jScrollPane1)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, jLabel1)
                    .add(jPanel2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(jPanel3, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 491, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.LEADING, jButton1)
                    .add(jPanel4, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .add(jLabel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 21, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(jPanel2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jPanel3, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jButton1)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jPanel4, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 60, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
        );

        jPanel2.getAccessibleContext().setAccessibleName("Analysis Settings ");

        pack();
    }// </editor-fold>//GEN-END:initComponents

private void caseButton_IActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_caseButton_IActionPerformed
   caseButton_II.setSelected(false);
   caseButton_III.setSelected(false);
   caseButton_IV.setSelected(false);
   settingscenario = 1; 
   width_field.setEnabled(true);
}//GEN-LAST:event_caseButton_IActionPerformed

private void caseButton_IIActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_caseButton_IIActionPerformed
// TODO add your handling code here:
   caseButton_I.setSelected(false);
   caseButton_III.setSelected(false);
   caseButton_IV.setSelected(false);
   settingscenario = 2; 
   width_field.setEnabled(false);
}//GEN-LAST:event_caseButton_IIActionPerformed

private void caseButton_IIIActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_caseButton_IIIActionPerformed
// TODO add your handling code here:
   caseButton_I.setSelected(false);
   caseButton_II.setSelected(false);
   caseButton_IV.setSelected(false);
   settingscenario = 3;
   width_field.setEnabled(false);
}//GEN-LAST:event_caseButton_IIIActionPerformed

private void caseButton_IVActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_caseButton_IVActionPerformed
// TODO add your handling code here:
   caseButton_I.setSelected(false);
   caseButton_II.setSelected(false);
   caseButton_III.setSelected(false);   
   settingscenario = 4; 
   width_field.setEnabled(true);
}//GEN-LAST:event_caseButton_IVActionPerformed

private void jMenu1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenu1ActionPerformed
// TODO add your handling code here:
  
    
}//GEN-LAST:event_jMenu1ActionPerformed

private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton1ActionPerformed
// TODO add your handling code here:
    
   range = Double.parseDouble(sensingrange_field.getText());
   width = Double.parseDouble(width_field.getText());
   density = Double.parseDouble(density_field.getText());
   
   //get the adjusted density
   density = density/(Math.PI*range*range); 
   
   beta = Double.parseDouble(dutycycleratio_field.getText());
   velocity = Double.parseDouble(speed_field.getText());
   cycle = Double.parseDouble(dutycycle_field.getText());
       
   String labelText =
      analyzeResults (range, width, density, beta, velocity, cycle, settingscenario); 
   
   results_label.setText(labelText);
}//GEN-LAST:event_jButton1ActionPerformed

private void jMenuItem1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem1ActionPerformed
// TODO add your handling code here:
    javax.swing.JOptionPane.showMessageDialog(this, "Sensor Network Detection Performance Analyzer for LiteOS.", "About", javax.swing.JOptionPane.PLAIN_MESSAGE);

}//GEN-LAST:event_jMenuItem1ActionPerformed

    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
        try {
            //javax.swing.UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel"); 
            javax.swing.UIManager.setLookAndFeel(javax.swing.UIManager.getSystemLookAndFeelClassName());    
           
            
        }
        catch (Exception e) {
            System.out.println(e);
        }    
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new analyzer().setVisible(true);
            }
        });
    }

  
    
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JRadioButton caseButton_I;
    private javax.swing.JRadioButton caseButton_II;
    private javax.swing.JRadioButton caseButton_III;
    private javax.swing.JRadioButton caseButton_IV;
    private javax.swing.JTextField density_field;
    private javax.swing.JTextField dutycycle_field;
    private javax.swing.JTextField dutycycleratio_field;
    private javax.swing.JButton jButton1;
    private javax.swing.JDialog jDialog1;
    private javax.swing.JEditorPane jEditorPane1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JMenu jMenu1;
    private javax.swing.JMenu jMenu2;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JMenuBar jMenuBar2;
    private javax.swing.JMenuItem jMenuItem1;
    private javax.swing.JMenuItem jMenuItem2;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel results_label;
    private javax.swing.JTextField sensingrange_field;
    private javax.swing.JTextField speed_field;
    private javax.swing.JTextField width_field;
    // End of variables declaration//GEN-END:variables

    
  
}
