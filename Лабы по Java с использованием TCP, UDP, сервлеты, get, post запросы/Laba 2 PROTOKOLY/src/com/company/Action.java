package com.company;

import javafx.scene.input.KeyCode;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;
import java.util.TimerTask;
import java.util.Vector;

public class Action {

    private boolean isAdd = false;
    public Vector<Object> mas;
    Graphics graphics;
    boolean change = false;
    Object obj;
    FileInputStream fileInputStream;
    FileOutputStream fileOutputStream;
    InetAddress ipAdress;
    Socket soc;
    ObjectOutputStream out;
    ObjectInputStream oin;


    public Action(Window wind) {

        class MyThread extends Thread
        {
            @Override
            public void run() {
                while (true) {
                    try {
                        if (!mas.isEmpty()) {
                            for (int i = 0; i < mas.size(); i++) {
                                try {
                                    sleep(10);
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                                mas.get(i).move(wind);
                                wind.paint(graphics);
                            }
                        }
                    }catch (ArrayIndexOutOfBoundsException ex){}
                }
            }
        }
        MyThread myThread = new MyThread();
        graphics = wind.getGraphics();
        mas = new Vector<>();
        myThread.start();



        wind.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyCode.C.getCode()) change = !change;

                // Для лабы 5 подключение к серверу
                if(e.getKeyCode() == KeyCode.NUMPAD0.getCode())
                {
                    try {
                        ipAdress = InetAddress.getByName("127.0.0.1");
                        soc = new Socket(ipAdress, 3005);
                        InputStream in = soc.getInputStream();
                        OutputStream ut = soc.getOutputStream();
                        out = new ObjectOutputStream(ut);
                        out.flush();
                        oin = new ObjectInputStream(in);
                    }catch (Exception asd) {
                        asd.printStackTrace();
                    }
                }

                //Отправка объекта серверу
                if(e.getKeyCode() == KeyCode.NUMPAD8.getCode())
                {
                    try {
                        int value = 1;
                        byte[] bytes = ByteBuffer.allocate(4).putInt(value).array();
                        DatagramSocket ds = new DatagramSocket();
                        InetAddress addr = InetAddress.getByName("127.0.0.1");
                        DatagramPacket datagramPacket = new DatagramPacket(bytes, bytes.length, addr, 9876);
                        ds.send(datagramPacket);
                        int type = 1;
                        if(mas.get(0) instanceof Picture)
                            type = 0;
                        bytes = ByteBuffer.allocate(4).putInt(type).array();
                        datagramPacket = new DatagramPacket(bytes, bytes.length, addr, 9876);
                        ds.send(datagramPacket);

                        bytes = ByteBuffer.allocate(4).putInt(mas.get(0).x).array();
                        datagramPacket = new DatagramPacket(bytes, bytes.length, addr, 9876);
                        ds.send(datagramPacket);

                        bytes = ByteBuffer.allocate(4).putInt(mas.get(0).y).array();
                        datagramPacket = new DatagramPacket(bytes, bytes.length, addr, 9876);
                        ds.send(datagramPacket);

                        ds.close();
                        mas.remove(0);
                        wind.setMas(mas);
                    }catch (Exception ee)
                    {
                        ee.printStackTrace();
                    }
                }
                // Получение размера вектора
                if(e.getKeyCode() == KeyCode.NUMPAD7.getCode())
                {
                    try {
                        DatagramSocket gss = new DatagramSocket(2700);
                        DatagramPacket getPack = new DatagramPacket(new byte[1024], 1024);
                        int value = 3;
                        byte[] bytes = ByteBuffer.allocate(4).putInt(value).array();
                        DatagramSocket dss = new DatagramSocket();
                        InetAddress addr = InetAddress.getByName("127.0.0.1");
                        DatagramPacket datagramPacket = new DatagramPacket(bytes, bytes.length, addr, 9876);
                        dss.send(datagramPacket);
                        gss.receive(getPack);
                        int num = ByteBuffer.wrap(getPack.getData()).getInt();
                        System.out.println(num);
                        gss.close();
                        dss.close();
                    }catch (Exception e1)
                    {
                        e1.printStackTrace();
                    }

                }
                //Очистить вектор на сервере
                if(e.getKeyCode() == KeyCode.NUMPAD6.getCode())
                {
                    try {
                        DatagramPacket getPack = new DatagramPacket(new byte[1024], 1024);
                        int value = 4;
                        byte[] bytes = ByteBuffer.allocate(4).putInt(value).array();
                        DatagramSocket dss = new DatagramSocket();
                        InetAddress addr = InetAddress.getByName("127.0.0.1");
                        DatagramPacket datagramPacket = new DatagramPacket(bytes, bytes.length, addr, 9876);
                        dss.send(datagramPacket);
                    }catch (Exception s)
                    {
                        s.printStackTrace();
                    }

                }
                //получение объектов с сервера
                if(e.getKeyCode() == KeyCode.NUMPAD9.getCode())
                {
                    try {

                        DatagramSocket gs = new DatagramSocket(2705);
                        DatagramPacket getPack = new DatagramPacket(new byte[1024], 1024);
                        int value = 2;
                        byte[] bytes = ByteBuffer.allocate(4).putInt(value).array();
                        DatagramSocket ds = new DatagramSocket();
                        InetAddress addr = InetAddress.getByName("127.0.0.1");
                        DatagramPacket datagramPacket = new DatagramPacket(bytes, bytes.length, addr, 9876);
                        ds.send(datagramPacket);
                        gs.receive(getPack);
                        int num = ByteBuffer.wrap(getPack.getData()).getInt();
                        Object obj;
                        while(num-->0) {
                            gs.receive(getPack);
                            int type = ByteBuffer.wrap(getPack.getData()).getInt();
                            gs.receive(getPack);
                            int x = ByteBuffer.wrap(getPack.getData()).getInt();
                            gs.receive(getPack);
                            int y = ByteBuffer.wrap(getPack.getData()).getInt();
                            if (type == 0)
                                obj = new Picture(x, y);
                            else if (type == 1)
                                obj = new SinFunc(x, y);
                            else
                                obj = new Picture();
                            mas.add(obj);
                        }
                        wind.setMas(mas);
                        gs.close();
                        ds.close();

                    }catch (Exception exx)
                    {
                        exx.printStackTrace();
                    }
                }

                if(e.getKeyCode() == KeyCode.S.getCode())
                {
                    try {
                        out.writeInt(1);
                        out.flush();
                        out.writeInt(mas.size());
                        for(int i = 0; i < mas.size(); i++)
                        {
                            if(mas.get(i) instanceof Picture)
                                out.writeInt(3000);
                            else
                                out.writeInt(2000);
                            out.writeInt(mas.get(i).x);
                            out.writeInt(mas.get(i).y);
                            out.flush();
                        }
                    } catch (Exception e1) {
                        e1.printStackTrace();
                    }
                }

                if(e.getKeyCode() == KeyCode.NUMPAD4.getCode())
                {
                    try{
                        out.writeInt(4);
                        out.writeInt(2);
                        out.flush();
                        if(oin.readInt() == 3000)
                            System.out.print("Picture ");
                        else
                            System.out.print("SinFunc ");
                        System.out.print("x = " + oin.readInt());
                        System.out.println(" y = " + oin.readInt());
                    }catch (Exception exc){
                        exc.printStackTrace();
                    }
                }

                if(e.getKeyCode() == KeyCode.NUMPAD3.getCode())
                {
                    try{
                        out.writeInt(3);
                        out.flush();
                        }catch (Exception exc){
                        exc.printStackTrace();
                    }
                }

                if(e.getKeyCode() == KeyCode.NUMPAD2.getCode())
                {
                    try{
                        out.writeInt(2);
                        out.flush();
                        System.out.println(" Кол-во объектов на сервере: " + oin.readInt());
                    }catch (Exception exc){
                        exc.printStackTrace();
                    }
                }

                if(e.getKeyCode() == KeyCode.G.getCode())
                {

                    try {
                        // Получение вектора с клиента
                        out.writeInt(5);
                        out.flush();
                        mas = new Vector<Object>();
                        int kol = oin.readInt();
                        for(int i=0; i < kol; i++) {
                            int per = oin.readInt();
                            if (per == 2000)
                                obj = new SinFunc();
                            else if (per == 3000)
                                obj = new Picture();
                            else
                                continue;
                            obj.x = oin.readInt();
                            obj.y = oin.readInt();
                            mas.add(obj);
                        }
                        System.out.println(mas.size());
                    } catch (Exception e1) {
                        e1.printStackTrace();
                    }
                    if (mas.size() > 0)
                        wind.setMas(mas);
                }

                if(e.getKeyCode() == KeyCode.B.getCode())
                {
                    for (int i=0; i<mas.size(); i++) mas.get(i).setMoveable(true);
                }

                if(e.getKeyCode() == KeyCode.E.getCode())
                {
                    for (int i=0; i<mas.size(); i++) mas.get(i).setMoveable(false);
                }

                if(e.getKeyCode() == KeyCode.W.getCode())
                {
                    File file = new File("object.bin");
                    file.delete();
                    for(int i = 0; i < mas.size(); i++)
                    {
                        mas.get(i).outBin();
                    }
                }

                if(e.getKeyCode() == KeyCode.R.getCode())
                {
                    mas = obj.inBin();
                    wind.setMas(mas);
                    wind.paint(graphics);
                }
                if(e.getKeyCode() == KeyCode.O.getCode())
                {
                    File file = new File("object.txt");
                    file.delete();
                    for(int i = 0; i < mas.size(); i++)
                    {
                        mas.get(i).outTxt();
                    }
                }
                if(e.getKeyCode() == KeyCode.I.getCode())
                {
                    mas = obj.inTxt();
                    wind.setMas(mas);
                    wind.paint(graphics);
                }
            }
        });

        wind.panel.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {

                if (e.getButton() == MouseEvent.BUTTON1)
                {
                    int x = e.getX();
                    int y = e.getY();
                    paintOrDelete(x, y, wind);
                }
                if (e.getButton() == MouseEvent.BUTTON3)
                {
                    int x = e.getX();
                    int y = e.getY();
                    moveObject(x, y);
                }
            }
        });
    }

    public void moveObject(int x, int y)
    {
        Object object1;
        for (int i = 0; i < mas.size(); i++) {
            if(mas.get(i) instanceof Picture)
                object1 = new Picture(mas.get(i).x, mas.get(i).y);
            else
                object1 = new SinFunc(mas.get(i).x, mas.get(i).y);
            if (object1.isObjClicked(x, y))
            {
                mas.get(i).changeMoveable();
            }
        }
    }


    public void paintOrDelete(int x, int y, Window wind)
    {
        isAdd = true;
        if (!mas.isEmpty()) {
            for (int i = 0; i < mas.size(); i++) {
                if(mas.get(i) instanceof Picture)
                    obj = new Picture(mas.get(i).x, mas.get(i).y);
                else
                    obj = new SinFunc(mas.get(i).x, mas.get(i).y);
                if (obj.isObjClicked(x, y)) {
                    mas.remove(i);
                    i--;
                    isAdd = false;
                    wind.paint(graphics);
                    return;
                }
            }
        }
        if (isAdd) {
            if(change)
                obj = new Picture(x, y);
            else
                obj = new SinFunc(x, y);
            mas.add(obj);
        }
        wind.setMas(mas);
        wind.paint(graphics);
    }

}
