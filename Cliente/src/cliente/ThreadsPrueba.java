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
import java.net.SocketTimeoutException;
import java.util.List;


/**
 *
 * @author Tincho
 */
//TODO controlar duplicados
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
                                        if(chat.getCliente().getPort() == null)
                                            socket = new DatagramSocket(0);
                                        else
                                            socket = new DatagramSocket(chat.getCliente().getPort());
                                            
                                        String msg = messages.get(0);
                                        
                                        if(msg.equals("LOGIN")){
                                            chat.getCliente().setPort(socket.getPort());
                                            msg += " " + socket.getInetAddress().toString() + " " + String.valueOf(socket.getPort()) + " " + chat.getCliente().getNick() + "<CR>";
                                        }                                          
                                        
                                        String serverHost = chat.getCliente().getServerHost();
                                        Integer serverPort = chat.getCliente().getServerPort();
                                        chat.returnSemaphore();
                                        InetAddress ip = InetAddress.getByName(serverHost);
                                        
                                        byte[] data = msg.getBytes();
                                        DatagramPacket message = new DatagramPacket(data, data.length, ip, serverPort);
                                        socket.send(message);
                                        
                                        //Espero respuesta del servidor, confirmando que le llegó el paquete
                                        socket.setSoTimeout(5000); //Seteo el tiemout
                                        byte[] ack = new byte[1024];
                                        DatagramPacket getack = new DatagramPacket(ack, ack.length);
                                        int attempts = 0;
                                        boolean received = false;
                                        while((!received)&&(attempts < 4)){ //intento reenviar hasta que se reciba la confirmación o un máximo de 3 veces.
                                            try{
                                                //Intento recibir la confirmación
                                                socket.receive(getack);
                                                received = true;
                                                
                                            }catch(SocketTimeoutException e){
                                                //Si se excede el timeout, intento de nuevo el envío.
                                                socket.send(message);
                                                attempts++;
                                            }
                                        }
                                        socket.close();
                                        
                                        //Si no se recibio la confirmacion, despliego un mensaje de error
                                        if(!received){
                                            String errorMessage = "Fallo el envío del mensaje luego de 3 intentos.\n";
                                            errorMessage += "Servidor no disponible en el host/puerto especificados";
                                            
                                            //chat.showErrorMessage(errorMessage);
                                        }else{
                                            msg = (msg.split(" "))[0];
                                            if(msg.equals("LOGIN")){
                                                chat.getCliente().setConnected(true);
                                            }else if(msg.equals("LOGOUT"))
                                                chat.getCliente().setConnected(false);
                                        }
                                        
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
                                    if(chat.getCliente().getPort() != null){
                                        int port = chat.getCliente().getPort();
                                        chat.returnSemaphore();
                                        InetAddress group = InetAddress.getByName("225.5.4.29");
                                        MulticastSocket multicastSocket = new MulticastSocket(5000);
                                        multicastSocket.joinGroup(group);
                                        //InetAddress ip = InetAddress.getByAddress(port, addr);

                                        byte[] data = new byte[1024];
                                        DatagramPacket message = new DatagramPacket(data, data.length);
                                        multicastSocket.receive(message);
                                        receivedMessage = new String(message.getData());
                                        
                                        //Envio el ACK del mensaje recibido al servidor
                                        DatagramSocket receivedSocket = new DatagramSocket(port);
                                        String ack = "ACK"; //Agregar el número de secuencia del mensaje recibido.
                                        DatagramPacket ackPacket = new DatagramPacket(ack.getBytes(), ack.getBytes().length, message.getAddress(), message.getPort());
                                        receivedSocket.send(ackPacket);
                                        receivedSocket.close();
                                        
                                        

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
                                        receivedMessage = new String(message.getData());
                                        
                                        String ack = "ACK"; //Agregar el número de secuencia del mensaje recibido.
                                        DatagramPacket ackPacket = new DatagramPacket(ack.getBytes(), ack.getBytes().length, message.getAddress(), message.getPort());
                                        socket.send(ackPacket);
                                        socket.close();
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
