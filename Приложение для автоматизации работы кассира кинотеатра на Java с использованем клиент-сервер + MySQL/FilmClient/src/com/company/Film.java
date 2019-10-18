package com.company;

public class Film {
    int id;
    String name;
    String hall;
    String time;
    String av_places;
    int price;
    Film(int id, String name, String hall, String time, String av_places, int price)
    {
        this.id = id;
        this.name = name;
        this.hall = hall;
        this.time = time;
        this.av_places = av_places;
        this.price = price;
    }
}
