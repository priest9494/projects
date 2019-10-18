package com.romandun;

import java.io.*;
import java.lang.Object;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Vector;

public class Serv extends Thread {
    ServerSocket soc;
    InputStream in;
    OutputStream out;
    ObjectInputStream oin;
    ObjectOutputStream oout;
    Vector<com.romandun.Object> vec;
    com.romandun.Object obj;
    Serv()
    {
        try {
            soc = new ServerSocket(3005);
            Socket ss = soc.accept();
            oout = new ObjectOutputStream(ss.getOutputStream());
            oout.flush();
            oin = new ObjectInputStream(ss.getInputStream());

            System.out.println("Connected");
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

    }

    public void req(int i)
    {
        try {
            oout.writeInt(i);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void run()
    {
        while (true)
        {
            try {
                int com = oin.readInt();
                switch (com)
                {
                    case 1:
                        System.out.println("1");
                        // Получение вектора с клиента
                        vec = new Vector<com.romandun.Object>();
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
                            vec.add(obj);
                        }
                        System.out.println(vec.size());
                        break;
                    case 2:
                        oout.writeInt(vec.size());
                        oout.flush();
                        break;
                    case 3:
                        vec = new Vector<com.romandun.Object>();
                        break;
                    case 4:
                        int num = oin.readInt();
                        if (vec.size() > num) {
                            if(vec.get(num) instanceof Picture)
                                oout.writeInt(3000);
                            else
                                oout.writeInt(2000);
                            oout.writeInt(vec.get(num).x);
                            oout.writeInt(vec.get(num).y);
                        }else
                            oout.writeObject(null);
                        oout.flush();
                        break;
                    case 5:
                        oout.writeInt(vec.size());
                        for(int i = 0; i < vec.size(); i++) {
                            if (vec.get(i) instanceof Picture)
                                oout.writeInt(3000);
                            else
                                oout.writeInt(2000);
                            oout.writeInt(vec.get(i).x);
                            oout.writeInt(vec.get(i).y);
                            oout.flush();
                        }
                        break;
                }
            }
            catch (Exception ex){
                ex.printStackTrace();
            }
        }
    }
}
