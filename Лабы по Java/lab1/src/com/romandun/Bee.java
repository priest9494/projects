package com.romandun;

import java.io.Serializable;

abstract public class Bee implements IBehaviour, Serializable{
    double x;
    double y;
    int speed = 2;
    int uniqueKey;
    double liveTime;
    double bornTime;
    public  static int n=0;

    public Bee()
    {
        x=0;
        y=0;
        n++;
    }

    public Bee(double x,double y,int uniqueKey, double bornTime, double liveTime)
    {
        this.x = x;
        this.y = y;
        this.liveTime = liveTime;
        this.bornTime = bornTime;
        this.uniqueKey = uniqueKey;
        n++;
    }

    @Override
    public void move() {

    }

    boolean isAlive(double dt)
    {
        if ((liveTime -= dt) < 0)
            return false;
        else
            return true;
    }

    @Override
    public void setX(int n) {
        x=n;
    }

    @Override
    public void setY(int n) {
        y=n;
    }

    @Override
    public double getX() {
        return x;
    }

    @Override
    public double getY() {
        return y;
    }
}
