package com.romandun;

import javax.swing.*;
import java.awt.*;
import java.io.*;

public class Picture extends Object {

    Image image = new ImageIcon("100x60.png").getImage();
    public static int numOfPictures = 0;
    DataOutputStream dataOutputStream;
    FileWriter fileWriter;

    Picture()
    {
        super();
    }

    Picture(int x, int y)
    {
        super(x, y);
    }

    @Override
    public void paint(Graphics graphics, Window window)
    {
        graphics.drawImage(image, x - 40, y, window);
    }

    @Override
    public void move(Window wind) {
        if (isMoveable) {
            if (Math.random() > 0.5)
                x += (int) (Math.random() * 7);
            else
                x -= (int) (Math.random() * 7);

            if (Math.random() > 0.5)
                y += (int) (Math.random() * 12);
            else
                y -= (int) (Math.random() * 12);
            if (x < 50) x += (int) (Math.random() * 20);
            if (x > wind.getWidth() - 100) x -= (int) (Math.random() * 25);
            if (y < 40) y += (int) (Math.random() * 25);
            if (y > wind.getHeight() - 80) y -= (int) (Math.random() * 20);
        }
    }

    @Override
    public void outBin()
    {
        try {
            dataOutputStream = new DataOutputStream(new FileOutputStream("object.bin", true));
            if(SinFunc.firstWrite)
            {
                dataOutputStream.writeInt(Object.n);
                SinFunc.firstWrite = false;
            }
            dataOutputStream.writeInt(3000);
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
            if(SinFunc.firstTxtWrite)
            {
                fileWriter.write(Object.n);
                SinFunc.firstTxtWrite = false;
            }
            fileWriter.write(3000);
            fileWriter.write(x);
            fileWriter.write(y);
            fileWriter.close();
        }catch (IOException e){
            e.printStackTrace();
        }

    }


}
