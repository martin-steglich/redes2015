/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package main;

import Vista.Chat;
import cliente.ThreadsPrueba;
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
        Runnable sender = new ThreadsPrueba(0, chat);
        Runnable multicastReceiver = new ThreadsPrueba(1, chat);
        //Runnable receiver = new ThreadsPrueba(2, chat);
        executor.execute(sender);
        executor.execute(multicastReceiver);
        //executor.execute(receiver);
        
    }
    
}
