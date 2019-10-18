package com.company;

import java.awt.*;
import java.io.*;
import java.util.Vector;

public class SinFunc extends Object {

    int dx;
    int dy;
    boolean isPlusX;
    boolean isPlusY;
    ObjectInputStream objectInputStream;
    ObjectOutputStream objectOutputStream;
    FileWriter fileWriter;
    DataOutputStream dataOutputStream;
    Vector<Object> mas;
    public static boolean firstTxtWrite = true;
    public static boolean firstWrite = true;

    SinFunc()
    {
        super();
        reroll();
    }

    SinFunc(int x, int y)
    {
        super(x ,y);
        reroll();
    }

    @Override
    public void paint(Graphics graphics, Window window)
    {
        graphics.setColor(Color.MAGENTA);
        for ( int xx = x -55 ;xx < x + 65;xx++ )
        {
            int yy = (int)(50*Math.sin( Math.toRadians(xx)*6) + y + 22);
            graphics.fillOval( xx,yy,3,3 );
        }
    }

    @Override
    public void move(Window wind)
    {
        if (isMoveable) {

            if(x < 100)
            {
                reroll();
                isPlusX = true;
            }
            if(x > wind.getWidth() - 100)
            {
                isPlusX = false;
                reroll();
            }
            if(y < 100)
            {
                reroll();
                isPlusY = true;
            }

            if(y > wind.getHeight() - 100)
            {
                isPlusY = false;
                reroll();
            }

            if(isPlusX) {
                if (isPlusY) {
                    x += dx;
                    y += dy;
                } else {
                    x += dx;
                    y -= dy;
                }
            }
            else {
                if (isPlusY) {
                    x -= dx;
                    y += dy;
                } else {
                    x -= dx;
                    y -= dy;
                }
            }
        }
    }

    public void reroll()
    {
        dx = (int)(Math.random()*15);
        dy = (int)(Math.random()*5);
    }


    @Override
    public void outBin()
    {
        try {
            dataOutputStream = new DataOutputStream(new FileOutputStream("object.bin", true));
            if(firstWrite)
            {
                dataOutputStream.writeInt(Object.n);
                firstWrite = false;
            }
            dataOutputStream.writeInt(2000);
            dataOutputStream.writeInt(this.x);
            dataOutputStream.writeInt(this.y);
            dataOutputStream.close();
        }catch (FileNotFoundException e){
            e.printStackTrace();
        }catch (IOException ex){
            ex.printStackTrace();
        }
        }

    @Override
    public void outTxt()
    {
        try {
            fileWriter = new FileWriter("object.txt", true);
            if(firstTxtWrite)
            {
                fileWriter.write(Object.n);
                firstTxtWrite = false;
            }
            fileWriter.write(2000);
            fileWriter.write(x);
            fileWriter.write(y);
            fileWriter.close();
        }catch (IOException e){
            e.printStackTrace();
        }
    }


}
