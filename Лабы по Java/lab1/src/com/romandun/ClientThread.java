package com.romandun;

import javax.swing.*;
import java.io.DataInputStream;
import java.io.IOException;
import java.util.ArrayList;

public class ClientThread extends Thread {
    DataInputStream in;
    Window w;
    boolean isEnd = false;
    int size;
    ArrayList<Bee> mas;

    ClientThread(DataInputStream inputStream, Window wind, ArrayList<Bee> maas)
    {
        mas = maas;
        w = wind;
        in = inputStream;
        start();
    }


    @Override
    public void run()
    {
        while(true)
        {
            try {
                if(!isEnd) {
                    int k = in.readInt();
                    switch (k) {
                        case 1:
                            size = in.readInt();
                            String str = "";
                            for (int i = 0; i < size; i++) {
                                str += String.valueOf(in.readInt());
                                str += "\n";
                            }
                            w.servTxtArea.setText("Подключенные пользователи: \n" + str);
                            break;

                        case 2:
                            size = in.readInt();
                            System.out.println(size);
                            for (int i = 0; i < size; ++i)
                            {

                                Bee bb;
                                int type = in.readInt();
                                double x = in.readDouble();
                                double y = in.readDouble();
                                int key = in.readInt();
                                double liveTime = in.readDouble();
                                double bornTime = in.readDouble();

                                if (type == 1) {
                                    double startX = in.readDouble();
                                    double startY = in.readDouble();
                                    bb = new BeeWorker(x, y, key, liveTime, bornTime);
                                    BeeWorker bw = (BeeWorker)bb;
                                    bw.startX = startX;
                                    bw.startY = startY;
                                }
                                else
                                    bb = new BeeTruten(x, y, key, liveTime, bornTime);
                                mas.add(bb);

                            }
                            break;
                    }
                }
                else
                    return;
            }catch (IOException e){

            }
        }
    }
}
