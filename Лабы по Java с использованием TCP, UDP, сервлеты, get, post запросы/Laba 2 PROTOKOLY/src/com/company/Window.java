package com.company;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;

public class Window extends JFrame{

    JPanel panel = new JPanel();
    public static Image image = new ImageIcon("100x60.png").getImage();

    Vector<Object> buf;

    Window()
    {
        setBounds(20,20,1000,600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setResizable(false);
        add(panel);
        setVisible(true);
    }

    public void paint(Graphics graphics)
    {
        super.paint(graphics);
        try {
            for (int i = 0; i < buf.size(); i++) {
                buf.get(i).paint(graphics, this);
            }
        }catch (NullPointerException ex){
        }
    }

    public void setMas(Vector<Object> mas)
    {
        this.buf = mas;
    }


}
