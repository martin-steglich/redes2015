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
    }
    
    
    
    public String getHost() {
        return host;
    }

    public void setHost(String host) {
        this.host = host;
    }

    public Integer getPort() {
        return port;
    }

    public void setPort(Integer port) {
        this.port = port;
    }

    public String getNick() {
        return nick;
    }

    public void setNick(String nick) {
        this.nick = nick;
    }

    public List<String> getMessages() {
        return messages;
    }

    public void setMessages(List<String> messages) {
        this.messages = messages;
    }

    public List<String> getMessagesToSend() {
        return messagesToSend;
    }

    public void setMessagesToSend(List<String> messagesToSend) {
        this.messagesToSend = messagesToSend;
    }

    public boolean isConnected() {
        return connected;
    }

    public void setConnected(boolean connected) {
        this.connected = connected;
    }

    public String getServerHost() {
        return serverHost;
    }

    public void setServerHost(String serverHost) {
        this.serverHost = serverHost;
    }

    public Integer getServerPort() {
        return serverPort;
    }

    public void setServerPort(Integer serverPort) {
        this.serverPort = serverPort;
    }
    
    
    
    
    
    public boolean connect(){
        connected = true;
        String loginMessage = "LOGIN";
        messagesToSend.add(loginMessage);
        return connected;
        //TODO manejar en los threads si el mensaje es de login, 
        //setear connect en true
    }
    
    public boolean disconnect(){
        connected = false;
        
        messages = new ArrayList<>();
        messagesToSend = new ArrayList<>();
        
        return !connected;
        //TODO manejar en los threads si el mensaje es de logout, 
        //setear connect en false
    }
    
    public void addMessage(String message){
        //System.out.println("M: " + message);
        messages.add(message);
    }
    
    public void sendMessage(String message){
        String msg = "MESSAGE " + message + "<CR>";
        messagesToSend.add(msg);
    }
}
