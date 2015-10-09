/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*package cliente;

import Vista.Chat;*/
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.List;


/**
 *
 * @author Tincho
 */
//TODO controlar duplicados
public class ThreadCliente implements Runnable {

    private int type; //0-sender; 1-multicast receiver; 2-private messages receiver
    private Chat chat;
    private String receivedMessage;
    private Integer sequenceNumber;
    

    public ThreadCliente(int type, Chat chat) {
        this.type = type;
        this.chat = chat;
        this.sequenceNumber = 0;

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
    
    public String armarPaquete(String sourceHost, Integer sourcePort, String destHost, Integer destPort, String msg, Integer seq, Integer isAck){
        String message = "<head>"+ sourceHost + "|" + sourcePort + "|" + destHost + "|" + destPort + "|";
        message += seq + "|" + isAck + "|0</head>";
        message += "<data>" + msg + "</data>";
        
        return message;
    }
    
    public int getSequenceNumber(String message){
        
        String[] split = message.split("</head>")[0].split("\\|");       
        int seqNum = Integer.parseInt(split[4]);
        
        return seqNum; 
    }
    
    public int getServerSequence(String message){
        
        String[] split = message.split("</head>")[0].split("\\|");       
        int seqNum = Integer.parseInt(split[6]);
              
        return seqNum; 
    }
    
    public boolean isForMe(String myHost, Integer myPort, String receivedMessage){
        String[] split = receivedMessage.split("</head>")[0].split("\\|");
        
        String destHost = split[2];
        Integer destPort = Integer.parseInt(split[3]);

        
        return ((destHost.equals(myHost)) && (destPort.equals(myPort))) || isMulticast(receivedMessage);
        
    }
    
    public boolean isMulticast(String receivedMessage){
        String[] split = receivedMessage.split("</head>")[0].split("\\|");
        
        String destHost = split[2];
        Integer destPort = Integer.parseInt(split[3]);
        
        return ((destHost.equals("0")) && (destPort.equals(0)));
    }
    
    public boolean isACK(String receivedMessage){
        String[] split = receivedMessage.split("</head>")[0].split("\\|");
        int isACK = Integer.parseInt(split[5]);
        return (isACK == 1);
    }
    
    public String getMessage(String message){
        String[] split = message.split("<data>")[1].split("</data>")[0].split("<CR>");
        String data = split[0];
        
        String[] splittedData = data.split(" ");
        String m = splittedData[2];
        for(int i = 3; i < splittedData.length; i++){
            m += " " + splittedData[i];
            
        }
        
        String msg = splittedData[1] + ": " + m;
        
        return msg;
        
        
    }
    
    public List<String> getConnected(String message, String nick){
        String[] split = message.split("<data>")[1].split("</data>")[0].split("<CR>");
        String data = split[0];
        
        String[] splittedData = data.split(" ");
        
        List<String> connected = new ArrayList();
        String users = "";
        for(int i=1; i < splittedData.length; i++){
            users += splittedData[i];
        }
        splittedData = users.split("\\|");
        for(int i=0; i < splittedData.length; i++){
            if(!splittedData[i].equals(nick))
                connected.add(splittedData[i]);
        }
        
        return connected;
    }
    
    public String getTypeMessage(String message){
        String[] split = message.split("<data>")[1].split("</data>");
        String data = split[0];
        if(data.startsWith("RELAYED_MESSAGE")){
            return "RELAYED_MESSAGE";
        }else if(data.startsWith("PRIVATE_MESSAGE")){
            return "PRIVATE_MESSAGE";
        }else if(data.startsWith("CONNECTED")){
            return "CONNECTED";
        }else if(data.startsWith("GOODBYE")){
            return "GOODBYE";
        }
        
        return "";
    }
    
    @Override
    public void run() {
        DatagramSocket socket = null;
        while (true) {
            try {
                switch (type) {
                    case 0: {

                        List<String> messages = chat.getCliente().getMessagesToSend();
                        
                        if((messages != null) && (messages.size() > 0)){
                            if(chat.getCliente().getPort() == null){ //Es el Login
                                socket = new DatagramSocket();
                                chat.getCliente().setPort(socket.getLocalPort());
                                chat.getCliente().setHost(InetAddress.getLocalHost().getHostAddress());
                                sequenceNumber = 0;
                            }else{
                                socket = new DatagramSocket(chat.getCliente().getPort());
                            }

                            String msg = messages.get(0); 
                            
                            
                            //Obtengo los datos del servidor
                            String serverHost = chat.getCliente().getServerHost();
                            Integer serverPort = chat.getCliente().getServerPort();
                            InetAddress ip;
                            try{
                                ip = InetAddress.getByName(serverHost);
                            }catch(Exception e){
                                String errorMessage = "El host ingresado es incorrecto";
                               chat.showErrorMessage(errorMessage);
                               chat.getCliente().setConnected(false);
                               continue;
                            }
                                
                            String host = chat.getCliente().getHost();
                            Integer port = chat.getCliente().getPort();
                            //Le agrego el encabezado al mensaje a enviar
                            String message = armarPaquete(host,port, serverHost, serverPort, msg, sequenceNumber, 0);
                            System.out.println("Mensaje Enviado: " + message);
                            byte[] data = message.getBytes();
                            DatagramPacket datagramPacket = new DatagramPacket(data, data.length, ip, serverPort);
                           
                            //Envio el paquete
                            socket.send(datagramPacket);



                            //Variables para almacenar el ACK
                            byte[] ack = new byte[1024];
                            DatagramPacket getAck = new DatagramPacket(ack, ack.length);

                            int attempts = 0; //Cantidad de reenvios
                            boolean received = false; //variable de control sobre si fue recibido el paquete
                            
                           
                            //Intento reenviar hasta que se reciba el ACK o un maximo de 3 veces
                            while((!received) && (attempts < 3)){
                               try{
                                    //Seteo el timeoute para recibir el ACK
                                    socket.setSoTimeout(50);
                                    socket.receive(getAck);
                                    
                                    String m = new String(getAck.getData());
                                    int seqNum = getSequenceNumber(m);
                                    chat.getCliente().setServerSequence(getServerSequence(m));
                                    if((isACK(m))){
                                        if(sequenceNumber == seqNum){
                                            System.out.println("ACK recibido: " + m);
                                            //Si se recibio un ACK, y coincide el numero de secuencia
                                            received = true;
                                            sequenceNumber = (sequenceNumber == 0) ? 1 : 0;
                                            socket.setSoTimeout(0);
                                        }
                                    }else{
                                        //Si se recibio un ACK, pero no coincide el número de secuencia, reenvio
                                        socket.send(datagramPacket);
                                        attempts++;
                                    }
                               }catch(SocketTimeoutException e){
                                   //Si se excede el timeout, reenvio
                                   socket.send(datagramPacket);
                                   attempts++;
                               }
                           }
                           socket.close();
                           chat.getCliente().getMessagesToSend().remove(0);
                           if(!received){
                               String errorMessage = "Servidor no disponible en el host/puerto especificados";

                               chat.showErrorMessage(errorMessage);
                               chat.getCliente().setConnected(false);
                               sequenceNumber = 0;
                           }else{
                               if(msg.startsWith("LOGIN")){
                                   chat.getCliente().setConnected(true);
                                   chat.connect();
                               }
                           }

                        }
                        
                        
                    }break;
                    case 1:{
                        if(chat.getCliente().isConnected()){
                            //Datos del grupo mutlicast
                            InetAddress group = InetAddress.getByName("225.5.4.29");
                            MulticastSocket multicastSocket = new MulticastSocket(5000);
                            multicastSocket.joinGroup(group);
                            
                            //Recibo los mensajes a traves del multicast
                            byte[] data = new byte[1024];
                            DatagramPacket message = new DatagramPacket(data, data.length);
                            try{
                                multicastSocket.setSoTimeout(50);
                                multicastSocket.receive(message);

                                receivedMessage = new String(message.getData());
                                System.out.println("Mensaje Recibido: " + receivedMessage);

                                //Envio el ACK para el paquete recibido
                                int seqNum = getSequenceNumber(receivedMessage);
                                String ack = armarPaquete(chat.getCliente().getHost(), chat.getCliente().getPort(), 
                                        chat.getCliente().getServerHost(), chat.getCliente().getServerPort(), "", seqNum, 1);
                                System.out.println("ACK Enviado: " + ack);
                                InetAddress ip = InetAddress.getByName(chat.getCliente().getServerHost());
                                DatagramSocket ackSocket = new DatagramSocket();
                                DatagramPacket ackPacket = new DatagramPacket(ack.getBytes(), ack.getBytes().length, ip, 51234);
                                ackSocket.send(ackPacket);
                                ackSocket.close();
                                sequenceNumber = chat.getCliente().getServerSequence();
                                //Proceso el mensaje recibido
                                System.out.println(" sequencenumber " + sequenceNumber);
                                System.out.println("seqnum " + seqNum);
                                if( sequenceNumber == seqNum){
                                    sequenceNumber = (sequenceNumber == 0) ? 1 : 0;
                                    chat.getCliente().setServerSequence(sequenceNumber);
                                    //Si es el numero de secuencia que esperaba
                                    if(isForMe(chat.getCliente().getHost(), chat.getCliente().getPort(), receivedMessage)){
                                        //Si el paquete es para mi (privado) o multicast
                                        String typeMessage = getTypeMessage(receivedMessage);
                                        if(typeMessage.equals("GOODBYE")){
                                            chat.getCliente().setConnected(false);
                                            chat.disconnect();

                                        }else if(typeMessage.equals("CONNECTED")){
                                            List<String> connected = getConnected(receivedMessage, chat.getCliente().getNick());
                                            chat.getConnected(connected);
                                        }else if(typeMessage.equals("RELAYED_MESSAGE")){
                                            String msg = getMessage(receivedMessage);
                                            msg = "Mensaje de " + msg;
                                            chat.getCliente().addMessage(msg);
                                            chat.updateMessages();
                                        }else if(typeMessage.equals("PRIVATE_MESSAGE")){
                                            String msg = getMessage(receivedMessage);
                                            msg = "Mensaje Privado de " + msg;
                                            chat.getCliente().addMessage(msg);
                                            chat.updateMessages();
                                        }
                                    }
                                }
                            }catch(SocketTimeoutException e){
                                
                            }
                            multicastSocket.close();
                        
                        }
                    }break;
                }
            } catch (Exception e) {
                if((socket != null)&&(socket.isConnected())&&(!socket.isClosed())){
                   socket.close();
                }
            }finally{
                if((socket != null)&&(socket.isConnected())&&(!socket.isClosed())){
                   socket.close();
                }
            }
        }
    }

}
