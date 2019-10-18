package com.company;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.sql.*;

public class Main {
    /* КОМАНДЫ ДЛЯ БД (пример)
    create database movie;
    use movie;
    create table films(id serial primary key, name varchar(40), hall varchar(2), time varchar(40), av_places varchar(40), price int);
    insert into films(name, hall, time, av_places, price) values ('Титаник','A', '21:00', '1,2,3,4,5,6,7', 400);
    insert into films(name, hall, time, av_places, price) values ('Шрек','B', '21:30', '1,2,3,4,5', 200);
    insert into films(name, hall, time, av_places, price) values ('Шрек 2','A', '23:00', '1,2,3', 250);
     */

    public static void main(String[] args) {

        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(8814);
        } catch (IOException e) {
            e.printStackTrace();
        }
        while (true)
        {
            Socket socket = null;
            try {
                // Ждем присоединения клиентов
                socket = serverSocket.accept();
            } catch (IOException e) {
                e.printStackTrace();
            }
            // Для каждого клиента создаем экземпляр класса для последующей работы
            new OneClient(socket);
            System.out.println("Клиент подключен ");
        }
    }
}
