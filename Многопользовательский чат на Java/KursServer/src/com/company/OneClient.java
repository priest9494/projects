package com.company;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.Map;

public class OneClient extends Thread {

    public DataInputStream is;
    public DataOutputStream os;
    private Socket socket;
    public String userName;
    boolean isOwner;
    boolean isWork = true;
    boolean iss;

    OneClient(Socket s)
    {
        try {
            socket = s;
            os = new DataOutputStream(socket.getOutputStream());
            os.flush();
            is = new DataInputStream(socket.getInputStream());
            userName = is.readUTF();
            System.out.println("Пользователь " + userName + " подключен");
            start();
        }catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        while (isWork) {
            int i = 1;
            try {
                int sw = is.readInt();
                switch (sw) {
                    // Вступить в чат под заданным номером
                    case 0:
                        int group = is.readInt();
                        os.writeInt(5);
                        if (Server.mas[group] != null) {
                            isOwner = false;
                            Server.mas[group].addClient(this);
                            os.writeInt(1);
                            System.out.println("Подключен к чату " + group + "    " + userName + " own : " + Server.mas[group].vec.get(Server.mas[group].vec.size()-1).isOwner);
                        } else {
                            os.writeInt(-1);
                            System.out.println("Не удалось подключить к чату " + group);
                        }

                        break;

                    // Создание чата
                    case 1:
                        while (Server.mas[i] != null)
                            i++;
                        isOwner = true;
                        Server.mas[i] = new Chat(i, this);
                        Server.mas[i].addClient(this);
                        Server.mas[i].ownerName = userName;
                        // Сохранение чата в словарь для отправки

                        Server.chatMap.put(i, is.readUTF());
                        os.writeInt(1);
                        os.writeInt(i);
                        //System.out.println("Чат " + i + " создан");
                        break;

                    // Рассылка сообщений
                    case 2:
                        int groupToSend = is.readInt();
                        String message = is.readUTF();
                            for (int j = 0; j < Server.mas[groupToSend].vec.size(); j++) {
                                synchronized (os) {
                                    Server.mas[groupToSend].vec.get(j).os.writeInt(2);
                                    Server.mas[groupToSend].vec.get(j).os.writeInt(groupToSend);
                                    Server.mas[groupToSend].vec.get(j).os.writeUTF(userName);
                                    Server.mas[groupToSend].vec.get(j).os.writeUTF(message);
                                    Server.mas[groupToSend].vec.get(j).os.flush();
                                }
                            }
                            break;

                    // Отсоединение от чата
                    case 3:
                        int toDisconnect = is.readInt();
                        iss = is.readBoolean();
                        disconnectFrom(toDisconnect, iss);
                        System.out.println("Чат " + toDisconnect + " отсоединен, удаление чата: " + iss);
                        break;

                    // Закрытие сокета при выключении программы
                    case 4:
                        try {
                            int val = is.readInt();
                            int gr = 0;
                            for(int k = 0; k < val; k++)
                            {
                                gr = is.readInt();
                                iss = is.readBoolean();
                                disconnectFrom(gr, iss);
                            }
                            isWork = false;
                            is.close();
                            os.close();
                            socket.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        //System.out.println("Соединение разорвано");
                        break;

                    // Рассылка текущих чатов
                    case 5:
                        try {
                            synchronized (os) {
                                os.writeInt(6);
                                os.writeInt(Server.chatMap.size());
                                for (Map.Entry<Integer, String> entry : Server.chatMap.entrySet()) {
                                    os.writeInt(entry.getKey());
                                    os.writeUTF(entry.getValue());
                                }
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }

                        break;
                }


            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void disconnectFrom(int toDisconnect, boolean isOwner)
    {
        if (isOwner) {
            for (int j = 0; j < Server.mas[toDisconnect].vec.size(); j++) {
                synchronized (os)
                {
                    try {
                        if(Server.mas[toDisconnect].vec.get(j) != Server.mas[toDisconnect].ownerObject) {
                            Server.mas[toDisconnect].vec.get(j).os.writeInt(7);
                            Server.mas[toDisconnect].vec.get(j).os.writeInt(toDisconnect);
                        }
                        } catch (Exception x) {
                            x.printStackTrace();
                        }
                }
            }
            Server.mas[toDisconnect] = null;
            Server.chatMap.remove(toDisconnect);
        } else if(Server.mas[toDisconnect] != null){
            for (int j = 0; j < Server.mas[toDisconnect].vec.size(); j++)
                if (this.socket == Server.mas[toDisconnect].vec.get(j).socket)
                    Server.mas[toDisconnect].vec.remove(j);
        }
    }
}
