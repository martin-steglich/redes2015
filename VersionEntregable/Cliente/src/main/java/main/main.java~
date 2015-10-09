/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package main;

import Vista.Chat;
import cliente.ThreadCliente;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 *
 * @author Tincho
 */
public class main {
    
    
    public static void main(String[] args) {
        Chat chat = new Chat();
        chat.setVisible(true);
        ExecutorService executor = Executors.newFixedThreadPool(2);
        Thread sender = new Thread(new ThreadCliente(0, chat));
        Thread multicastReceiver = new Thread(new ThreadCliente(1, chat));
        executor.execute(sender);
        executor.execute(multicastReceiver);
        executor.shutdown();
    }
    
}
