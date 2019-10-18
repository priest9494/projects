package com.romandun;

import javax.swing.*;
import java.util.ArrayList;
import java.util.Iterator;

public class DialogWind extends JDialog {
    ArrayList<Bee> mas;

    DialogWind(ArrayList<Bee> buf)
    {
        mas = buf;
        String str = new String();
        for(int i=0;i<Bee.n;i++)
        {
            if(mas.get(i) instanceof BeeWorker)
                str += "Рабочий ";
            else
                str += "Трутень ";
            str += mas.get(i).uniqueKey + "\n";
        }
        JTextArea beeLab = new JTextArea(str);
        beeLab.setEditable(false);
        add(beeLab);
        setLocation(100,100);
        setSize(300,500);
        setVisible(true);

    }


}
