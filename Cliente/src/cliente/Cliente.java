/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cliente;

import Vista.Chat;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Tincho
 */
public class Cliente {

    private String host;
    private Integer port;
    private String serverHost;
    private Integer serverPort;
    private String nick;
    private List<String> messages;
    private List<String> messagesToSend;
    private boolean connected;
    

    public Cliente() {
        messages = new ArrayList<>();
        messagesToSend = new ArrayList<>();
        connected = false;
        port = null;
    }
    
    
    
    public synchronized String getHost() {
        return host;
    }

    public synchronized void setHost(String host) {
        this.host = host;
    }

    public synchronized Integer getPort() {
        return port;
    }

    public synchronized void setPort(Integer port) {
        this.port = port;
    }

    public synchronized String getNick() {
        return nick;
    }

    public synchronized void setNick(String nick) {
        this.nick = nick;
    }

    public synchronized List<String> getMessages() {
        return messages;
    }

    public synchronized void setMessages(List<String> messages) {
        this.messages = messages;
    }

    public synchronized List<String> getMessagesToSend() {
        return messagesToSend;
    }

    public synchronized void setMessagesToSend(List<String> messagesToSend) {
        this.messagesToSend = messagesToSend;
    }

    public synchronized boolean isConnected() {
        return connected;
    }

    public synchronized void setConnected(boolean connected) {
        this.connected = connected;
        messages = new ArrayList<>();
        messagesToSend = new ArrayList<>();
        
        if(!connected){
            port =null;
            host = null;
            
        }
    }

    public synchronized String getServerHost() {
        return serverHost;
    }

    public synchronized void setServerHost(String serverHost) {
        this.serverHost = serverHost;
    }

    public synchronized Integer getServerPort() {
        return serverPort;
    }

    public synchronized void setServerPort(Integer serverPort) {
        this.serverPort = serverPort;
    }
    
    
    
    
    
    public synchronized void connect(){
        String loginMessage = "LOGIN " + nick + "<CR>";
        messagesToSend.add(loginMessage);
        //TODO manejar en los threads si el mensaje es de login, 
        //setear connect en true
    }
    
    public synchronized void disconnect(){
        
        String logoutMessage = "LOGOUT<CR>";
        messagesToSend.add(logoutMessage);
        
        /*messages = new ArrayList<>();
        messagesToSend = new ArrayList<>();*/
        
        //TODO manejar en los threads si el mensaje es de logout, 
        //setear connect en false
    }
    
    public synchronized void addMessage(String message){
        //System.out.println("M: " + message);
        messages.add(message);
    }
    
    public synchronized void sendRelayedMessage(String message){
        String msg = "MESSAGE " + message + "<CR>";
        messagesToSend.add(msg);
    }
    
    public synchronized void getConnected(){
        String msg = "GET_CONNECTED<CR>";
        messagesToSend.add(msg);
    }
    
    public synchronized void sendPirvateMessage(String message, String receptor){
        String msg = "PRIVATE_MESSAGE " + receptor + " " + message + "<CR>";
        messagesToSend.add(msg);
    }
}
