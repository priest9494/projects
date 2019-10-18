package com.company;

import java.util.Vector;

public class Chat {

    static int group;
    Vector<OneClient> vec;
    String ownerName;
    OneClient ownerObject;

    Chat(int gr, OneClient o)
    {
        vec = new Vector<>();
        group = gr;
        ownerObject = o;
    }

    public void addClient(OneClient client)
    {
        vec.add(client);
    }


}
