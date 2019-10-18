package com.romandun;

public class BeeTruten extends Bee {

    public static int trNumber = 0;
    int targetX;
    int targetY;
    boolean isMove = false;

    public BeeTruten(double x, double y,int uniqueKey, double liveTime, double bornTime) {
        super(x, y,uniqueKey,liveTime,bornTime);
        trNumber++;
    }

    @Override
    public void move() {
        if (!isMove)
        {
            isMove = true;
            targetX = (int)(Math.random() * 680) + 40;
            targetY = (int)(Math.random() * 320) + 55;
        }

        double dx = targetX - x;
        double dy = targetY - y;

        double len = Math.sqrt( dx * dx + dy * dy);
        if (len <= speed)
        {
            x = targetX;
            y = targetY;
            isMove = !isMove;
        }
        else
        {
            double time = len / speed;
            x += dx / time;
            y += dy / time;
        }

    }
}

