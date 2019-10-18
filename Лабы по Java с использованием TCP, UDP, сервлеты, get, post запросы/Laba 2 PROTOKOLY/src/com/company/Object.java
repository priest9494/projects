package com.company;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.Vector;

abstract public class Object implements Serializable {
    int x;
    int y;
    int pVis = 30, pShir = 60;
    int sinVis = 50, sinShir = 60;
    boolean yeah;
    boolean isMoveable = true;
    DataInputStream dataInputStream;
    public static int n = 0;
    FileReader fileReader;
    ObjectInputStream objectInputStream;
    ObjectOutputStream objectOutputStream;
    private static final long serialVersionUID = 1L;

    Object()
    {
        x=0;
        y=0;
        isMoveable = true;
        n++;
    }

    Object(int x, int y)
    {
        this.x=x;
        this.y=y;
        isMoveable = true;
        n++;
    }

    public void paint(Graphics graphics, Window window){}

    public void setMoveable(boolean is)
    {
        isMoveable = is;
    }

    public void changeMoveable()
    {
        isMoveable = !isMoveable;
    }

    public boolean isObjClicked(int x, int y)
    {

        if(this instanceof Picture)
            if (x > (this.x - pShir) && x < (this.x + pShir) && y > (this.y - pVis) && y < (this.y + pVis))
                yeah = true;
        else yeah = false;

        if(this instanceof SinFunc)
            if (x > (this.x - sinShir ) && x < (this.x + sinShir) && y > (this.y - sinVis-10) && y < (this.y + sinVis-5))
                yeah = true;
        else yeah = false;

        return yeah;
    }

    public void move(Window wind) {}

    public Vector inBin()
    {
        Vector<Object> mas = new Vector<Object>();
        Object obj;
        int kol = 0;
        n = 0;
        try
        {
            dataInputStream = new DataInputStream(new FileInputStream("object.bin"));
            kol = dataInputStream.readInt();
            for(int i=0; i < kol; i++)
            {
                int per = dataInputStream.readInt();
                if( per == 2000)
                    obj = new SinFunc();
                else  if (per == 3000)
                    obj = new Picture();
                else
                    continue;
                obj.x = dataInputStream.readInt();
                obj.y = dataInputStream.readInt();
                mas.add(obj);
            }
        }catch (FileNotFoundException e){
           // e.printStackTrace();
        }catch (IOException e){
            //e.printStackTrace();
        }
        return mas;
    }

    public Vector inTxt()
    {
        Vector<Object> mas = new Vector<Object>();
        Object obj;
        n = 0;
        try {
            fileReader = new FileReader("object.txt");
            int kol = 0;
            kol = fileReader.read();
            for(int i = 0; i < kol; i++)
            {
                int lul = fileReader.read();

                if(lul == 2000)
                    obj = new SinFunc();
                else
                    if(lul == 3000)
                        obj = new Picture();
                else continue;
                obj.x = fileReader.read();
                obj.y = fileReader.read();
                System.out.println(obj.getClass() + " x = " + obj.x + " y = " + obj.y);
                mas.add(obj);
            }
        }catch (FileNotFoundException e){
            e.printStackTrace();
        }catch (IOException e){
            e.printStackTrace();
        }
        for (int i = 0; i < mas.size(); i++)
        {
            System.out.println(i);
            System.out.println(mas.get(i).x);
            System.out.println(mas.getClass());
        }
        return mas;
    }

    public void outTxt()
    {

    }

    public void outBin()
    {

    }

}
