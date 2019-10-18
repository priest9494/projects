package com.romandun;

import java.io.Console;
import java.util.ArrayList;

public class BasicAI extends Thread
{
    ArrayList<Bee> mas;
    boolean isWorker;
    boolean isStoped = false;
    boolean isFinished = false;
    BasicAI(ArrayList<Bee> mas, boolean isWorker)
    {
        this.mas = mas;
        this.isWorker = isWorker;
    }
    @Override
    public void run() {
        while (true)
        {
            synchronized (mas)
            {
                for (int i = 0; i < mas.size(); i++) {
                    if (isWorker) {
                        if (mas.get(i) instanceof BeeWorker)
                            mas.get(i).move();
                    }
                    else
                    {
                        if (mas.get(i) instanceof BeeTruten)
                            mas.get(i).move();
                    }
                }
            }
            if (isStoped) {
                try{
                    synchronized (this) {
                        wait();
                    }
                } catch (InterruptedException e) {
                    System.out.println(e);
                }
            }
            if (isFinished)
                break;
            try {
                Thread.sleep(20);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
