package com.romandun;

import javax.swing.*;
import javax.swing.text.BadLocationException;
import java.io.PipedWriter;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.*;

public class ConsoleThread extends Thread {
    JTextArea area;
    int n;
    int num;
    private int lines = 2;
    String lastStr;
    boolean isInt = false;

    PipedWriter wr = new PipedWriter();
    ConsoleThread(JTextArea area){
        this.area = area;
    }

    public void run()
    {
        while (true)
        {
            n = area.getLineCount();
            if (n > lines) {
                lines = n;
                try {
                    lastStr = area.getText();
                    lastStr = lastStr.substring(area.getLineEndOffset(n-3));
                    Pattern re = Pattern.compile("^ *?returnNum ([a-zA-Z]{4})$",Pattern.MULTILINE);
                    Matcher match = re.matcher(lastStr);
                    if ((match.find()) && ((match.group(1).equals("work"))|| (match.group(1).equals("trut"))))
                    {
                        if (match.group(1).equals("work"))
                            wr.write("w");
                        if (match.group(1).equals("trut"))
                            wr.write("t");
                        synchronized (this)
                        {
                            isInt = true;
                            wait();
                        }
                        isInt = false;
                        area.setText(area.getText() + num + "\n");
                        lines += 1;
                        area.setCaretPosition(area.getLineEndOffset(lines-3)+2);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            try {
                Thread.sleep(50);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
