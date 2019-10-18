package com.romandun;

public class BeeWorker extends Bee {

    double startX;
    double startY;
    boolean isToStart = true;

    public BeeWorker(double x,double y,int uniqueKey, double liveTime, double bornTime)
    {
        super(x,y,uniqueKey,liveTime,bornTime);
        startX = x;
        startY = y;
    }

    @Override
    public void move()
    {
        double dx;
        double dy;
        double targetX;
        double targetY;
        if (isToStart)
        {
            targetX = 0;
            targetY = 0;
        }
        else
        {
            targetX = startX;
            targetY = startY;
        }
        dx = targetX - x;
        dy = targetY - y;

        double len = Math.sqrt( dx * dx + dy * dy);
        if (len <= speed)
        {
            x = targetX;
            y = targetY;
            isToStart = !isToStart;
        }
        else
        {
            double time = len / speed;
            x += dx / time;
            y += dy / time;
        }
    }

}
