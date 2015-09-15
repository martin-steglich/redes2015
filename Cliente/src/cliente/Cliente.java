/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cliente;

import Vista.Chat;

/**
 *
 * @author Tincho
 */
public class Cliente {

    String host;
    Integer port;
    String nick;

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
    
    public boolean connect(){
        return true;
        //TODO manejar la conexión con el servidor
    }
    
    public boolean disconnect(){
        return true;
        //TODO manejar la desconexión con el servidor
    }
    
    
    
}
