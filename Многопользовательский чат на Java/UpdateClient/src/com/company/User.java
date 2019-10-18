package com.company;

import java.awt.event.*;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

public class User extends Thread{

    static HashMap<Integer, ChatWindow> map;
    MainFrame window;
    InetAddress ipAdress;
    Socket soc;
    int group;
    int groupConnect;
    DataInputStream in;
    DataOutputStream out;
    String userName = "User";
    boolean isWork;
    boolean own = false;
    String theme = "Theme";

    User(MainFrame wind)
    {
        window = wind;
        map = new HashMap<>();
        isWork = true;

        // Считывание username
        wind.jTextArea1.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                userName = wind.jTextArea1.getText();
            }
        });

        wind.jTextArea3.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                theme = wind.jTextArea3.getText();
            }
        });

        // Считывание номера группы для подключения
        wind.jTextArea2.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                super.focusLost(e);
                try {
                    groupConnect = Integer.parseInt(wind.jTextArea2.getText());
                }catch (Exception t){
                    wind.jTextArea2.setText("0");
                }

            }
        });
        // Вход на сервер
        wind.jButton3.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    ipAdress = InetAddress.getByName("127.0.0.1");
                    soc = new Socket(ipAdress, 6538);
                    out = new DataOutputStream(soc.getOutputStream());
                    out.flush();
                    in = new DataInputStream(soc.getInputStream());
                    start();
                    out.writeUTF(userName);
                    System.out.println(userName + " присоеднен к серверу!");
                    wind.jButton3.setEnabled(false);
                    isWork = true;
                    out.writeInt(5);
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            }
        });

        // Обработка нажатия на кнопку "Присоединиться"
        wind.jButton1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(!map.containsKey(groupConnect)) {
                    try {
                        out.writeInt(0);
                        out.writeInt(groupConnect);
                        own = false;
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
            }
        });

        // Обработка нажатия на кнопку "Создать конференцию"
        wind.jButton2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    out.writeInt(1);
                    out.writeUTF(theme);
                }catch (Exception e1)
                {
                    e1.printStackTrace();
                }
                own = true;
            }
        });

        // Отправить запрос на получение чатов доступных к подключению
        wind.groups.addFocusListener(new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent e) {
                super.focusGained(e);
                try {
                    out.writeInt(5);
                } catch (IOException e1) {
                    e1.printStackTrace();
                }

            }
        });

        // Закрытие сокета и потоков при закрытии окна
        wind.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                if(soc != null) {
                    try {
                        out.writeInt(4);
                        out.writeInt(map.size());
                        for (Map.Entry<Integer, ChatWindow> entry : map.entrySet())
                        {
                            out.writeInt(entry.getKey());
                            out.writeBoolean(map.get(entry.getKey()).isOwner);
                            System.out.println("откл " + entry.getKey());
                        }
                        out.flush();
                        isWork = false;
                        in.close();
                        out.close();
                        soc.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                    isWork = false;
                }
                super.windowClosing(e);
            }
        });


    }
    // Отключить клиента от одного чата
    public static void removeChat(int group)
    {
        map.remove(group);
        System.out.println("Выход из группы " + group);
    }

    @Override
    public void run() {
        int command;
        while (isWork) {
            try {
                command = in.readInt();
                switch (command) {
                    //Читает команду о том, что сервер создан
                    case 1:
                        group = in.readInt();
                        ChatWindow chat = new ChatWindow(out, group, own);
                        window.tabs.add(chat.getWind(),"Chat " + group);
                        map.put(group, chat);
                        System.out.println(map.get(group).group);
                        out.writeInt(5);
                        break;

                    // Читает команду (получить сообщение) - читает группу - читает username + сообщение - находит окно по групе в словаре map и пишет в поле чата сообщение
                    case 2:
                        int sendGr;
                        sendGr = in.readInt();
                        StringBuffer s = new StringBuffer(2048);
                        s.append(map.get(sendGr).receiveMessages.getText() + "\n" + in.readUTF() + ": " + in.readUTF());
                        map.get(sendGr).receiveMessages.setText(s.toString());
                        break;

                    // Если хотим подключиться к чату из списка
                    case 5:
                        if ((in.readInt()) != -1) {
                            ChatWindow chatt = new ChatWindow(out, groupConnect, false);
                            window.tabs.add(chatt.getWind(),"Chat " + groupConnect);
                            map.put(groupConnect, chatt);
                            System.out.println("Подключен к группе " + groupConnect);
                        } else
                            System.out.println("Не удалось подключиться к группе " + groupConnect);
                        out.writeInt(5);
                        break;

                    // Получение списка чатов
                    case 6:
                        StringBuffer strChats = new StringBuffer(4096);
                        int size = in.readInt();
                        for (int i = 0; i < size; i++) {
                            strChats.append(in.readInt() + ". " + in.readUTF() + "\n");
                        }
                        window.groups.setText(strChats.toString());
                        //System.out.println("Чаты получены " + strChats);
                        break;

                    // Если закрыли чат
                    case 7:
                        int dis = in.readInt();
                        if(map.get(dis).isOwner == false) {
                            map.get(dis).receiveMessages.setText("Владелец покинул беседу");
                            map.remove(dis);
                        }
                        System.out.println("Чат удален, номер " + dis);
                        out.writeInt(5);
                        break;
                }
            } catch (IOException e) {}
        }
    }
}
