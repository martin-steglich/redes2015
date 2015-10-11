/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*package Vista;

import cliente.Cliente;*/
import java.awt.Color;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Tincho
 */
public class Chat extends javax.swing.JFrame {
    
    private Cliente cliente;
    private List<String> chatList;
    private Integer semaphore;
    private boolean getted;
    
    public Chat() {
        initComponents();
        //TODO ver si creamos un controller o manejamos directo desde acá
        requeriedFields.setVisible(false);
        chatList = new ArrayList();
        chatArea.setOpaque(false);
        infoPanel.setVisible(true);
        privateCheckbox.setSelected(false);
        semaphore = 1;
        getted = false;
        cliente = new Cliente();
    }

    
    public void connect(){
        connectButton.setEnabled(false);
        disconnectButton.setEnabled(true);
        messageField.setEnabled(true);
        //chatArea.setEnabled(true);
        statusColor.setBackground(Color.green);
        statusLabel.setText("Conectado");
        sendButton.setEnabled(true);
        hostField.setEnabled(false);
        portField.setEnabled(false);
        nickField.setEnabled(false);
        messageField.requestFocus();
        chatArea.setOpaque(true);
        privateCheckbox.setEnabled(true);
        usersConnected.setEnabled(false);
        usersConnected.removeAllItems();
        getted = false;
        //infoPanel.setVisible(false);
        
    }
    
    public void disconnect(){
        connectButton.setEnabled(true);
        disconnectButton.setEnabled(false);
        messageField.setEnabled(false);
        statusColor.setBackground(Color.red);
        statusLabel.setText("Desconectado");
        sendButton.setEnabled(false);
        //chatArea.setEnabled(false);
        hostField.setText(cliente.getServerHost());
        portField.setText(cliente.getServerPort().toString());
        nickField.setText(cliente.getNick());
        hostField.setEnabled(true);
        portField.setEnabled(true);
        nickField.setEnabled(true);
        chatList = new ArrayList();
        chatArea.setListData(chatList.toArray());
        chatArea.setOpaque(false);
        messageField.setText("");
        privateCheckbox.setEnabled(false);
        privateCheckbox.setSelected(false);
        usersConnected.setEnabled(false);
        usersConnected.removeAllItems();
        getted = false;
        //infoPanel.setVisible(true);
    }
    
    public synchronized void showErrorMessage(String error){
        requeriedFields.setText(error);
        requeriedFields.setVisible(true);
        disconnect();
    }
    
    public synchronized void updateMessages(){
        chatArea.setListData(cliente.getMessages().toArray());
        chatArea.ensureIndexIsVisible(cliente.getMessages().size() - 1);
    }

    public synchronized Cliente getCliente() {
        return cliente;
    }

    public void setCliente(Cliente cliente) {
        this.cliente = cliente;
    }
    
    public synchronized boolean useSemaphore(){
        if(semaphore > 0){
            semaphore--;
            return true;
        }
        return false;
    }
    
    public synchronized void returnSemaphore(){
        semaphore++;
    }

    public synchronized Integer getSemaphore() {
        return semaphore;
    }

    public void setSemaphore(Integer semaphore) {
        this.semaphore = semaphore;
    }
    
    public void getConnected(List<String> users){
        usersConnected.setEnabled(true);
        usersConnected.removeAllItems();
        for(String user : users)
            usersConnected.addItem(user);
        
        getted = false;
        
    }

    
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLabel1 = new javax.swing.JLabel();
        infoPanel = new javax.swing.JPanel();
        jLabel2 = new javax.swing.JLabel();
        hostField = new javax.swing.JTextField();
        jLabel3 = new javax.swing.JLabel();
        portField = new javax.swing.JTextField();
        jLabel4 = new javax.swing.JLabel();
        nickField = new javax.swing.JTextField();
        messageField = new javax.swing.JTextField();
        sendButton = new javax.swing.JButton();
        jPanel2 = new javax.swing.JPanel();
        connectButton = new javax.swing.JButton();
        disconnectButton = new javax.swing.JButton();
        statusColor = new javax.swing.JTextField();
        statusLabel = new javax.swing.JLabel();
        requeriedFields = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        chatArea = new javax.swing.JList();
        privateCheckbox = new javax.swing.JCheckBox();
        usersConnected = new javax.swing.JComboBox();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Cliente de Chat - Redes 2015");

        jLabel1.setFont(new java.awt.Font("Tahoma", 0, 14)); // NOI18N
        jLabel1.setText("Cliente de Chat - Redes 2015");

        jLabel2.setText("Host IP:");

        jLabel3.setText("Port:");

        portField.setText("54321");

        jLabel4.setText("Apodo:");

        javax.swing.GroupLayout infoPanelLayout = new javax.swing.GroupLayout(infoPanel);
        infoPanel.setLayout(infoPanelLayout);
        infoPanelLayout.setHorizontalGroup(
            infoPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(infoPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(infoPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel2)
                    .addComponent(jLabel3)
                    .addComponent(jLabel4))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(infoPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(nickField, javax.swing.GroupLayout.PREFERRED_SIZE, 192, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(portField, javax.swing.GroupLayout.PREFERRED_SIZE, 192, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(hostField, javax.swing.GroupLayout.PREFERRED_SIZE, 192, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );
        infoPanelLayout.setVerticalGroup(
            infoPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(infoPanelLayout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addGroup(infoPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel2)
                    .addComponent(hostField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(infoPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(portField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(infoPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel4)
                    .addComponent(nickField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(38, Short.MAX_VALUE))
        );

        messageField.setEnabled(false);
        messageField.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                messageFieldKeyPressed(evt);
            }
        });

        sendButton.setText("Send");
        sendButton.setEnabled(false);
        sendButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                sendButtonActionPerformed(evt);
            }
        });

        connectButton.setText("Connect");
        connectButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                connectButtonActionPerformed(evt);
            }
        });

        disconnectButton.setText("Disconnect");
        disconnectButton.setEnabled(false);
        disconnectButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                disconnectButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addComponent(connectButton, javax.swing.GroupLayout.PREFERRED_SIZE, 103, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(disconnectButton)
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(disconnectButton)
                    .addComponent(connectButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGap(0, 24, Short.MAX_VALUE))
        );

        statusColor.setBackground(new java.awt.Color(255, 51, 51));
        statusColor.setFocusable(false);

        statusLabel.setText("Desconectado");

        requeriedFields.setForeground(new java.awt.Color(255, 0, 51));
        requeriedFields.setText("Servidor no disponible en el host/puerto especificados");

        chatArea.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        chatArea.setFocusable(false);
        jScrollPane1.setViewportView(chatArea);

        privateCheckbox.setText("Privado");
        privateCheckbox.setEnabled(false);
        privateCheckbox.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                privateCheckboxItemStateChanged(evt);
            }
        });

        usersConnected.setEnabled(false);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(158, 158, 158)
                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 201, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(statusColor, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(statusLabel)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(requeriedFields, javax.swing.GroupLayout.PREFERRED_SIZE, 398, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addGroup(layout.createSequentialGroup()
                                .addGap(0, 0, Short.MAX_VALUE)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(usersConnected, javax.swing.GroupLayout.PREFERRED_SIZE, 183, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(privateCheckbox))
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(messageField, javax.swing.GroupLayout.PREFERRED_SIZE, 187, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(sendButton))))
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(infoPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                    .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(4, 4, 4)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel1)
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(infoPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(jScrollPane1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(messageField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(sendButton))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(privateCheckbox)
                    .addComponent(usersConnected, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 22, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(statusColor, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(statusLabel)
                    .addComponent(requeriedFields)))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void messageFieldKeyPressed(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_messageFieldKeyPressed
        if(evt.getKeyCode() == KeyEvent.VK_ENTER){
            if(messageField.getText().equals("")){
                requeriedFields.setText("No puede enviar un mensaje vacío");
                requeriedFields.setVisible(true);
            }else if((privateCheckbox.isSelected()) && (usersConnected.getSelectedItem() == null)){
                requeriedFields.setText("Debe elegir un usuario para enviarle un mensaje privado");
                requeriedFields.setVisible(true);
            }else if(privateCheckbox.isSelected()){
              requeriedFields.setVisible(false);
              
              String receptor = (String) usersConnected.getSelectedItem();
              String msg = messageField.getText();
              
              cliente.sendPirvateMessage(msg, receptor);
              privateCheckbox.setSelected(false);
              usersConnected.setEnabled(false);
              usersConnected.removeAllItems();
            }else{
                requeriedFields.setVisible(false);
                
                cliente.sendRelayedMessage(messageField.getText());
            }
          
          messageField.setText("");
      }
    }//GEN-LAST:event_messageFieldKeyPressed

    private void sendButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_sendButtonActionPerformed
        if(messageField.getText().equals("")){
                requeriedFields.setText("No puede enviar un mensaje vacío");
                requeriedFields.setVisible(true);
            }else if((privateCheckbox.isSelected()) && (usersConnected.getSelectedItem() == null)){
                requeriedFields.setText("Debe elegir un usuario para enviarle un mensaje privado");
                requeriedFields.setVisible(true);
            }else if(privateCheckbox.isSelected()){
              requeriedFields.setVisible(false);
              
              String receptor = (String) usersConnected.getSelectedItem();
              String msg = messageField.getText();
              
              cliente.sendPirvateMessage(msg, receptor);
              privateCheckbox.setSelected(false);
              usersConnected.setEnabled(false);
              usersConnected.removeAllItems();
            }else{
                requeriedFields.setVisible(false);
                
                cliente.sendRelayedMessage(messageField.getText());
            }
          
          messageField.setText("");
    }//GEN-LAST:event_sendButtonActionPerformed

    private void disconnectButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_disconnectButtonActionPerformed

        cliente.disconnect();
    }//GEN-LAST:event_disconnectButtonActionPerformed

    private void connectButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_connectButtonActionPerformed
        try{
            String host = hostField.getText();
            Integer port = portField.getText().equals("") ? 0 :Integer.parseInt(portField.getText());
            String nick = nickField.getText();

            if(!host.equals("") && port != 0 && !nick.equals("")){
                cliente.setServerHost(host);
                cliente.setNick(nick);
                cliente.setServerPort(port);
                cliente.connect();
                requeriedFields.setVisible(false);
                
            }else{
                requeriedFields.setText("Todos los campos son obligatorios");
                requeriedFields.setVisible(true);
            }
        }catch(Exception e){
            requeriedFields.setText("El puerto debe ser numérico");
            requeriedFields.setVisible(true);
        }

    }//GEN-LAST:event_connectButtonActionPerformed

    private void privateCheckboxItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_privateCheckboxItemStateChanged
        if(!getted){
            getted = true;
            if(privateCheckbox.isSelected()){
                usersConnected.setEnabled(true);
                cliente.getConnected();
            }else{
                getted = false;
                usersConnected.removeAllItems();
                usersConnected.setEnabled(false);
        }
        }
    }//GEN-LAST:event_privateCheckboxItemStateChanged

    /**
     * @param args the command line arguments
     */
//    public static void main(String args[]) {
//        /* Set the Nimbus look and feel */
//        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
//        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
//         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
//         */
//        try {
//            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
//                if ("Nimbus".equals(info.getName())) {
//                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
//                    break;
//                }
//            }
//        } catch (ClassNotFoundException ex) {
//            java.util.logging.Logger.getLogger(Chat.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
//        } catch (InstantiationException ex) {
//            java.util.logging.Logger.getLogger(Chat.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
//        } catch (IllegalAccessException ex) {
//            java.util.logging.Logger.getLogger(Chat.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
//        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
//            java.util.logging.Logger.getLogger(Chat.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
//        }
//        //</editor-fold>
    /* Create and display the form */
        /*java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Chat().setVisible(true);
            }
        });
    }*/

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JList chatArea;
    private javax.swing.JButton connectButton;
    private javax.swing.JButton disconnectButton;
    private javax.swing.JTextField hostField;
    private javax.swing.JPanel infoPanel;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTextField messageField;
    private javax.swing.JTextField nickField;
    private javax.swing.JTextField portField;
    private javax.swing.JCheckBox privateCheckbox;
    private javax.swing.JLabel requeriedFields;
    private javax.swing.JButton sendButton;
    private javax.swing.JTextField statusColor;
    private javax.swing.JLabel statusLabel;
    private javax.swing.JComboBox usersConnected;
    // End of variables declaration//GEN-END:variables
}
