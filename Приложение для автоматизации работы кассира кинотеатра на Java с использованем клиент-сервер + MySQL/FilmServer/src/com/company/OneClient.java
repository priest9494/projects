package com.company;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;
import java.sql.*;

public class OneClient extends Thread {
    ResultSet resultSet;
    Statement statement;
    Connection connection;
    DataInputStream in;
    DataOutputStream out;
    boolean isWork = true;
    OneClient(Socket socket)
    {
        int price = 0;
        try {
            in = new DataInputStream(socket.getInputStream());
            out = new DataOutputStream(socket.getOutputStream());
            out.flush();
            // Работа с БД
            Class.forName("com.mysql.jdbc.Driver");
            connection = DriverManager.getConnection("jdbc:mysql://localhost:9999/movie", "root", "root");
            statement = connection.createStatement();
            resultSet = statement.executeQuery("select * from films");
            resultSet.last();
            out.writeInt(resultSet.getRow());
            resultSet.first();
            // Построчно достаем из ответа на запрос информацию о сеансах и передаем ее клиенту
            do {
                out.writeInt(resultSet.getInt("id"));
                out.writeUTF(resultSet.getString("name"));
                out.writeUTF(resultSet.getString("hall"));
                out.writeUTF(resultSet.getString("time"));
                out.writeUTF(resultSet.getString("av_places"));
                out.writeInt(resultSet.getInt("price"));
            } while(resultSet.next());

            start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run()
    {
        // 0 - Закрыть соединение; 1 - Изменить список забронированных мест
        int command = 0;
        while(isWork)
        {
            try {
                command = in.readInt();
                switch (command) {
                    case 1: {
                        int idx = in.readInt();
                        String newPlaces = in.readUTF();
                        String req = "UPDATE films SET av_places = '" + newPlaces + "' WHERE id = " + idx + ";";
                        System.out.println(req);
                        statement.executeUpdate(req);
                        break;
                    }
                    case 0: {
                        in.close();
                        out.close();
                        isWork = false;
                        break;
                    }

                }
            }catch (Exception e) {
                e.printStackTrace();
            }

        }
    }
}
