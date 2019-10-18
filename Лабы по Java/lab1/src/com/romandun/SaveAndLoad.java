package com.romandun;

import java.io.*;
import java.net.ServerSocket;
import java.util.ArrayList;

public class SaveAndLoad  {

    ArrayList<Bee> mas;
    ObjectOutputStream objectOutputStream;
    ObjectInputStream objectInputStream;

    SaveAndLoad(ArrayList<Bee> buf, boolean flag, File file) throws IOException, ClassNotFoundException // Save, если флаг == true
    {
        mas = buf;
        if (flag) {
            objectOutputStream = new ObjectOutputStream(new FileOutputStream(file));
            for (int i = 0; i < mas.size(); i++) {
                objectOutputStream.writeObject(mas.get(i));
            }
            objectOutputStream.flush();
            objectOutputStream.close();
        }
        if (!flag) {
            try {
                objectInputStream = new ObjectInputStream(new FileInputStream(file));
            }catch (StreamCorruptedException e){
                System.out.println("Задан некорректный файл!");
                return;
            }
            mas = new ArrayList<Bee>();
            int i=0;
            while (true) try {
                mas.add((Bee) objectInputStream.readObject());
            } catch (EOFException e) {
                objectInputStream.close();
                break;
            }
        }
    }
    public ArrayList<Bee> getMas() {
        return mas;
    }
}
