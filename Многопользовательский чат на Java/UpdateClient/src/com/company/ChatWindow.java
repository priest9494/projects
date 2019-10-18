package com.company;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.DataOutputStream;
import java.io.IOException;

public class ChatWindow extends JPanel{

    DataOutputStream out;
    boolean isOwner;
    JTextField messages = new JTextField(35);
    JTextArea receiveMessages= new JTextArea(17,45);
    int group;
    JButton sendBut = new JButton("Отправить");
    JButton close = new JButton("Закрыть");

    ChatWindow(DataOutputStream os, int gr, boolean isOwn)
    {
        isOwner = isOwn;
        group = gr;
        setBounds(190,290,600,400);
        receiveMessages.setEditable(false);
        add(receiveMessages);
        add(messages, BorderLayout.EAST);
        add(sendBut, BorderLayout.WEST);
        add(close, BorderLayout.WEST);

        out = os;
        setVisible(true);

            // Считывание из формы и отрправка сообщения
        sendBut.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    synchronized (os)
                    {
                        os.writeInt(2);
                        os.writeInt(group);
                        os.writeUTF(messages.getText());
                        os.flush();
                    }
                    messages.setText("");
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }
        });
        // закрыть чат
        close.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    os.writeInt(3);
                    os.writeInt(group);
                    os.writeBoolean(isOwn);
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                User.removeChat(group);
                MainFrame.tabs.removeTabAt(MainFrame.tabs.getSelectedIndex());
            }
        });
    }
    public ChatWindow getWind()
    {
        return this;
    }
}
