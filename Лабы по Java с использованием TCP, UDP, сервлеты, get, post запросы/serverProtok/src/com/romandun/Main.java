package com.romandun;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {

    public static void main(String[] args) {
        Serv srv = new Serv();
        srv.start();
        BufferedReader keyboard = new BufferedReader(new InputStreamReader(System.in));
        while(true)
        {
            int com = 0;
            try {
                com = keyboard.read();
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }
            srv.req(com);
        }
    }
}
