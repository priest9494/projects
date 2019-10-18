package com.romandun;

import java.io.IOException;
import java.io.PipedReader;
import java.io.PipedWriter;

public class ConsoleWindow implements Runnable{
    Runnable runnable = new Thread();
    private PipedReader pr;
    private PipedWriter pw;


    ConsoleWindow(PipedWriter pw) throws IOException
    {
        this.pr = new PipedReader(pw);

    }

    ConsoleWindow(PipedReader pr) throws IOException
    {
        this.pw = new PipedWriter(pr);
    }

    @Override
    public void run() {

    }

    PipedWriter getStream()
    {
        return pw;
    }
}
