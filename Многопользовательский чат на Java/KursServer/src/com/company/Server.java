package com.company;

import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;


public class Server {
    static Chat[] mas = new Chat[100];
    static HashMap<Integer, String> chatMap; // Для хранения названий чатов

    Server() throws Exception
    {
        chatMap = new HashMap<>();
        ServerSocket serverSocket = new ServerSocket(6538);
        System.out.println("Start...");
        while (true)
        {
            Socket socket = serverSocket.accept();
            new OneClient(socket);
            System.out.println("Клиент подключен ");
        }
    }

}
