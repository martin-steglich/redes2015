/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cliente;

import Vista.Chat;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.util.List;


/**
 *
 * @author Tincho
 */
public class ThreadsPrueba implements Runnable {

    private int type; //0-sender; 1-multicast receiver; 2-private messages receiver
    private Chat chat;
    private boolean wait;
    private String receivedMessage;

    public ThreadsPrueba(int type, Chat chat) {
        this.type = type;
        this.chat = chat;
        this.wait = false;

    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public Chat getChat() {
        return chat;
    }

    public void setChat(Chat chat) {
        this.chat = chat;
    }
    
    @Override
    public void run() {
        DatagramSocket socket = null;
        while (true) {
            try {
                switch (type) {
                    case 0: {
                        if (!wait) {
                            if (chat.useSemaphore()) {
                            //System.out.println("AA");
                                if (chat.getCliente().isConnected()) {
                                    List<String> messages = chat.getCliente().getMessagesToSend();
                                    
                                    if ((messages != null) && (messages.size() > 0)) {
                                        wait = true;
                                        socket = new DatagramSocket(0);
                                        String msg = messages.get(0);
                                        if(msg.equals("LOGIN")){
                                            chat.getCliente().setPort(socket.getPort() + 1);
                                            msg += " " + socket.getInetAddress().toString() + " " + String.valueOf(socket.getPort()) + " " + chat.getCliente().getNick();
                                        }                                          
                                        
                                        String serverHost = chat.getCliente().getServerHost();
                                        Integer serverPort = chat.getCliente().getServerPort();
                                        chat.returnSemaphore();
                                        InetAddress ip = InetAddress.getByName(serverHost);
                                        
                                        byte[] data = msg.getBytes();
                                        DatagramPacket message = new DatagramPacket(data, data.length, ip, serverPort);
                                        socket.send(message);
                                        socket.close();
                                        
                                        //TODO controlar que se recibio el mensaje  
                                    }else{
                                        chat.returnSemaphore();
                                    }
                                } else {
                                    chat.returnSemaphore();
                                    
                                }
                            }

                        }
                        if (wait) {
                            if (chat.useSemaphore()) {         
                                chat.getCliente().getMessagesToSend().remove(0);
                                chat.returnSemaphore();
                                wait = false;
                                
                            }
                        }
                    }
                    break;
                    case 1:{
                        if (!wait) {
                            if (chat.useSemaphore()) {
                                //System.out.println("BB");
                                if (chat.getCliente().isConnected()) {
                                    
                                    chat.returnSemaphore();
                                    InetAddress group = InetAddress.getByName("225.5.4.29");
                                    MulticastSocket multicastSocket = new MulticastSocket(5000);
                                    multicastSocket.joinGroup(group);
                                    //InetAddress ip = InetAddress.getByAddress(port, addr);
                                    
                                    byte[] data = new byte[1024];
                                    DatagramPacket message = new DatagramPacket(data, data.length);
                                    multicastSocket.receive(message);
                                    //socket.close();
                                    receivedMessage = new String(message.getData());
                                    
                                    wait = true;
                                } else {
                                    chat.returnSemaphore();
                                   
                                }
                            }
                        }
                        if (wait) {
                            if (chat.useSemaphore()) {      
                                //System.out.println(receivedMessage);
                                chat.getCliente().addMessage(receivedMessage);
                                chat.updateMessages();
                                chat.returnSemaphore();
                                wait = false;
                                
                            }
                        }
                    }
                        break;
                    case 2: {
                        if (!wait) {
                            
                            if (chat.useSemaphore()) {
                                //System.out.println("BB");
                                if (chat.getCliente().isConnected()) {
                                    
                                    Integer port = chat.getCliente().getPort();
                                    chat.returnSemaphore();
                                    if(port != null){
                                        System.out.println("PORT: " + port);
                                        InetAddress ip = InetAddress.getLocalHost();
                                        //InetAddress ip = InetAddress.getByAddress(port, addr);
                                        socket = new DatagramSocket(port, ip);
                                        byte[] data = new byte[1024];
                                        DatagramPacket message = new DatagramPacket(data, data.length);
                                        socket.receive(message);
                                        socket.close();
                                        receivedMessage = new String(message.getData());

                                        wait = true;
                                    }
                                } else {
                                    chat.returnSemaphore();
                                   
                                }
                            }
                        }
                        if (wait) {
                            if (chat.useSemaphore()) {      
                                //System.out.println(receivedMessage);
                                chat.getCliente().addMessage(receivedMessage);
                                chat.updateMessages();
                                chat.returnSemaphore();
                                wait = false;
                                
                            }
                        }
                    }
                    break;
                }
            } catch (Exception e) {
                System.out.println(e.getMessage());
            }finally{
                if((socket != null)&&(socket.isConnected())&&(!socket.isClosed())){
                   socket.close();
                }
            }
        }
    }

}
