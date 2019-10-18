package com.romandun;

import javafx.scene.input.KeyCode;

import javax.accessibility.Accessible;
import javax.accessibility.AccessibleContext;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.util.*;
import java.util.Timer;
import javax.swing.JButton;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class Habitat  {

    private int timeSpawnWorker = 3;
    private int timeSpawnTruten = 5;
    private float secForWorker=0;
    private float secForTrut=0;
    private ArrayList<Bee> mas = new ArrayList<Bee>();
    private HashSet<Integer> keys = new HashSet<>();
    private TreeMap<Integer, Double> timeMap = new TreeMap<>();
    private double elapsed;
    private float workerCreateChance = 0f;
    private float trutenPersent = 0f;
    private int timeTrutLiving = 7;
    private int timeWorkLiving = 10;
    DataOutputStream dataOutputStream;
    DataInputStream dataInputStream;
    Window wind;
    Graphics gr;
    Habitat habit;

    Socket soc;
    public DataOutputStream out;
    public DataInputStream in;
    BasicAI workerAI ;
    BasicAI trutAI;

    InetAddress ipAdress;
    private Timer tim = new Timer() ;
    private boolean firstRun = true;
    private long startTime = 0;
    private long lastTime = 0;
    private double time = 0 ;
    private boolean isStarted = false;
    private boolean showTime=true;
    private boolean showInfo = false;
    private boolean isPaused = false;
    File file;
    ConsoleThread consThread = null;
    PipedReader red = null;
    ClientThread clFr;
    int idToSend;
    int numBee;



    public Habitat(Window myWind) {

        in(dataInputStream);

        myWind.workerCreate.setText(Integer.toString(timeSpawnWorker));
        myWind.trutenCreate.setText(Integer.toString(timeSpawnTruten));
        myWind.liveTrutField.setText(Integer.toString(timeTrutLiving));
        myWind.liveWorkField.setText(Integer.toString(timeWorkLiving));
        myWind.slide.setValue((int)(workerCreateChance*100));
        myWind.slide2.setValue((int)(trutenPersent*100));


        habit = this;
        wind = myWind;
        wind.setTextTime(0);
        JButton but1 = new JButton("ОК");
        myWind.show.setEnabled(true);
        tim.schedule(new Updater(habit), 1000, 20);
        gr = wind.getGraphics();

        wind.sendBut.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    out.writeInt(1);
                    out.writeInt(idToSend);
                    out.writeInt(numBee);
                    for (int i = 0; i < numBee; ++i)
                    {
                        Bee bb = mas.get(i);
                        if (bb instanceof BeeWorker)
                            out.writeInt(1);
                        else
                            out.writeInt(2);
                        out.writeDouble(bb.x);
                        out.writeDouble(bb.y);
                        out.writeInt(bb.uniqueKey);
                        out.writeDouble(bb.liveTime);
                        out.writeDouble(bb.bornTime);
                        if (bb instanceof BeeWorker) {
                            BeeWorker bw = (BeeWorker)bb;
                            out.writeDouble(bw.startX);
                            out.writeDouble(bw.startY);
                        }
                    }
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            }
        });

        wind.servIdToSend.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                idToSend = Integer.parseInt(wind.servIdToSend.getText());
            }
        });

        wind.servSendBeeCount.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                numBee =  Integer.parseInt(wind.servSendBeeCount.getText());
            }
        });

        wind.fr.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                out(dataOutputStream);
                try {
                    out.writeInt(0);
                    out.flush();
                    synchronized (clFr) {
                        clFr.isEnd = true;
                    }
                    in.close();
                    out.close();
                    soc.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }

                super.windowClosing(e);
            }
        });

        wind.consoleWind.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                try {
                    out.writeInt(0);
                    out.flush();
                    synchronized (clFr) {
                        clFr.isEnd = true;
                    }
                    in.close();
                    out.close();
                    soc.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                super.windowClosing(e);
            }
        });

        myWind.serv.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {

                myWind.serverWind.setSize(500,300);
                myWind.serverWind.setLocation(300,300);
                myWind.serverWind.add(myWind.servTxtArea, BorderLayout.NORTH);
                myWind.serverWind.add(myWind.servSendBeeCount, BorderLayout.EAST);
                myWind.serverWind.add(myWind.servIdToSend, BorderLayout.WEST);
                myWind.serverWind.add(myWind.sendBut);
                myWind.serverWind.setVisible(true);
                try {
                    ipAdress = InetAddress.getByName("127.0.0.1");
                    soc = new Socket(ipAdress, 6996);
                    out = new DataOutputStream(soc.getOutputStream());
                    out.flush();
                    in = new DataInputStream(soc.getInputStream());
                    System.out.println(in.readInt());
                    clFr = new ClientThread(in, wind, mas);
                } catch (Exception ex) {
                    ex.printStackTrace();
                }

            }
        });

        myWind.save.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                myWind.saveOrLoad.showOpenDialog(wind);
                file = myWind.saveOrLoad.getSelectedFile();
                try {
                    save(file);
                } catch (IOException e1) {
                    e1.printStackTrace();
                } catch (ClassNotFoundException e1) {
                    e1.printStackTrace();
                }
            }
        });

        myWind.load.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                myWind.saveOrLoad.showOpenDialog(wind);
                file = myWind.saveOrLoad.getSelectedFile();
                synchronized (mas) {
                    try {
                        mas = load(file);
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    } catch (ClassNotFoundException e1) {
                        e1.printStackTrace();
                    }
                    BeeTruten.trNumber = 0;
                    for (int i = 0; i < mas.size(); i++) {
                        if (mas.get(i) instanceof BeeTruten) BeeTruten.trNumber++;
                    }
                    keys = new HashSet<>();
                    timeMap = new TreeMap<>();
                    wind.setSimulationState(true);
                    time = 0;
                    secForTrut = 0;
                    secForWorker = 0;
                    Bee.n = mas.size();
                    startSimulation(wind);
                }
            }
        });

        myWind.console.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (consThread == null)
                {
                    JTextArea consoleArea = new JTextArea(5,40);

                    myWind.consoleWind.getContentPane().add(new JScrollPane(consoleArea));
                    myWind.consoleWind.pack();
                    myWind.consoleWind.setLocation(600,100);
                    myWind.consoleWind.setSize(500,500);
                    consoleArea.setText("returnNum(work/trut) - вернуть кол-во живых рабочих/трутней" +"\n");
                    consThread = new ConsoleThread(consoleArea);
                    consThread.start();
                    try {
                        red = new PipedReader(consThread.wr);
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
                myWind.consoleWind.setVisible(true);

            }
        });

        myWind.priorityStream2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int priority = myWind.priorityStream.getSelectedIndex() + 1;
                trutAI.setPriority(priority);
            }
        });

        myWind.priorityStream.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int priority = myWind.priorityStream.getSelectedIndex() + 1;
                workerAI.setPriority(priority);
            }
        });

        myWind.curBee.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                DialogWind d = new DialogWind(mas);
            }
        });

        myWind.barInfoShow.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                    showInfo = !showInfo;
                    focusDisable(wind);
                    wind.showInfo = showInfo;
                    myWind.infoShow.setSelected(showInfo);
            }
        });

        myWind.barShowTime.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (!showTime) wind.changeTimeState(true);
                showTime=true;
                focusDisable(wind);
                myWind.show.setSelected(true);
            }
        });

        myWind.barHideTime.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(showTime) wind.changeTimeState(false);
                showTime=false;
                focusDisable(wind);
                myWind.hide.setSelected(true);
            }
        });

        myWind.menuStart.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(!isStarted) {
                    startSimulation(wind);
                }
            }
        });

        myWind.menuStop.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (isStarted)
                    stopSimulation(wind);
            }
        });

        myWind.startStream1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (workerAI.isStoped)
                {
                    workerAI.isStoped = false;
                    synchronized (workerAI) {
                        workerAI.notify();
                    }
                }
                else
                    workerAI.isStoped = true;
            }
        });

        myWind.startStream2.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (trutAI.isStoped)
                {
                    trutAI.isStoped = false;
                    synchronized (trutAI) {
                        trutAI.notify();
                    }
                }
                else
                    trutAI.isStoped = true;
            }
        });

        myWind.trutenCreate.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                    try {
                        timeSpawnTruten = Integer.parseInt(myWind.trutenCreate.getText());
                    } catch (Exception ex) {
                        timeSpawnTruten = 5;
                        myWind.excep.setLayout(new BorderLayout());
                        JPanel pan1 = new JPanel();
                        myWind.excep.setSize(230, 90);
                        JLabel lab1 = new JLabel("Введены некорректне данные!");
                        myWind.excep.setLocation(550, 300);
                        myWind.excep.add(pan1);
                        pan1.add(lab1, BorderLayout.NORTH);
                        pan1.add(but1, BorderLayout.SOUTH);
                        myWind.excep.setVisible(true);
                        pan1.setVisible(true);
                        but1.setVisible(true);
                    }
                }
        });

        myWind.liveTrutField.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                try {
                    timeTrutLiving = Integer.parseInt(myWind.liveTrutField.getText());
                } catch (Exception ex) {
                    timeTrutLiving = 7;
                    myWind.excep.setLayout(new BorderLayout());
                    JPanel pan1 = new JPanel();
                    myWind.excep.setSize(230, 90);
                    JLabel lab1 = new JLabel("Введены некорректне данные!");
                    myWind.excep.setLocation(550, 300);
                    myWind.excep.add(pan1);
                    pan1.add(lab1, BorderLayout.NORTH);
                    pan1.add(but1, BorderLayout.SOUTH);
                    myWind.excep.setVisible(true);
                    pan1.setVisible(true);
                    but1.setVisible(true);
                }
            }
        });

        myWind.liveWorkField.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                try {
                    timeWorkLiving = Integer.parseInt(myWind.liveWorkField.getText());
                } catch (Exception ex) {
                    timeWorkLiving = 7;
                    myWind.excep.setLayout(new BorderLayout());
                    JPanel pan1 = new JPanel();
                    myWind.excep.setSize(230, 90);
                    JLabel lab1 = new JLabel("Введены некорректне данные!");
                    myWind.excep.setLocation(550, 300);
                    myWind.excep.add(pan1);
                    pan1.add(lab1, BorderLayout.NORTH);
                    pan1.add(but1, BorderLayout.SOUTH);
                    myWind.excep.setVisible(true);
                    pan1.setVisible(true);
                    but1.setVisible(true);
                }
            }
        });

        myWind.workerCreate.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                try {
                    timeSpawnWorker = Integer.parseInt(myWind.workerCreate.getText());
                }catch (Exception ex){
                    timeSpawnWorker = 3;
                    myWind.excep.setLayout(new BorderLayout());
                    JPanel pan1 = new JPanel();
                    myWind.excep.setSize(230,90);
                    myWind.excep.setLocation(550,300);
                    myWind.excep.add(pan1);
                    JLabel lab1 = new JLabel("    Введены некорректные данные!");
                    pan1.add(lab1, BorderLayout.NORTH);
                    pan1.add(but1, BorderLayout.SOUTH);
                    but1.setVisible(true);
                    myWind.excep.setVisible(true);
                }
            }
        });


        but1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                myWind.excep.dispose();
            }
        });

        myWind.slide2.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                JSlider source = (JSlider) e.getSource();
                trutenPersent = (float)source.getValue()/100;
            }
        });

        myWind.slide.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                JSlider source = (JSlider) e.getSource();
                workerCreateChance = (float)source.getValue()/100;
            }
        });

        myWind.ok.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                wind.setSimulationState(true);
                myWind.dialog.dispose();
                time = 0;
                secForTrut =0;
                secForWorker=0;
                mas = new ArrayList<Bee>();
                wind.setMas(mas);
                Bee.n=0;
                BeeTruten.trNumber=0;
                myWind.start.setEnabled(true);
                myWind.stop.setEnabled(false);
            }
        });

        myWind.cansel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                wind.setSimulationState(true);
                myWind.dialog.dispose();
                focusDisable(wind);
                startSimulation(wind);
            }
        });

        myWind.infoShow.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                showInfo = !showInfo;
                focusDisable(wind);
                wind.showInfo = showInfo;
                myWind.barInfoShow.setSelected(showInfo);
            }
        });

        myWind.show.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
            if (!showTime) wind.changeTimeState(true);
            showTime=true;
            focusDisable(wind);
            myWind.barShowTime.setSelected(true);
            }
        });

        myWind.hide.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(showTime) wind.changeTimeState(false);
                showTime=false;
                focusDisable(wind);
                myWind.barHideTime.setSelected(true);
            }
        });

        myWind.start.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(!isStarted) {
                    startSimulation(wind);
                }
            }
        });

        myWind.stop.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (isStarted)
                    stopSimulation(wind);

            }
        });

        wind.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {

               if (e.getKeyCode() == KeyCode.B.getCode())
               {
                   if (!isStarted) {
                       startSimulation(wind);
                   }
               }
                if(e.getKeyCode() == KeyCode.ENTER.getCode())
                {

                }

                if (e.getKeyCode() == KeyCode.E.getCode())
               {
                   if (isStarted) {
                      stopSimulation(wind);
                   }
               }
               if (e.getKeyCode() == KeyCode.T.getCode())
               {
                   showTime=!showTime;
                   wind.changeTimeState(showTime);
               }
            }
        });


    }

    public void in(DataInputStream dataInputStream){
        try{
            dataInputStream = new DataInputStream(new FileInputStream("config.txt"));
            timeSpawnTruten = dataInputStream.readInt();
            timeSpawnWorker = dataInputStream.readInt();
            timeTrutLiving = dataInputStream.readInt();
            timeWorkLiving = dataInputStream.readInt();
            trutenPersent = dataInputStream.readFloat();
            workerCreateChance = dataInputStream.readFloat();
            dataInputStream.close();
        }catch (IOException e){
            e.printStackTrace();;
        }

    }


    public void out(DataOutputStream dataOutputStream)
    {       try {
        dataOutputStream = new DataOutputStream(new FileOutputStream("config.txt"));
    }catch (IOException e){
        e.printStackTrace();
    }
        try{
            dataOutputStream.writeInt(timeSpawnTruten);
            dataOutputStream.writeInt(timeSpawnWorker);
            dataOutputStream.writeInt(timeTrutLiving);
            dataOutputStream.writeInt(timeWorkLiving);
            dataOutputStream.writeFloat(trutenPersent);
            dataOutputStream.writeFloat(workerCreateChance);
            dataOutputStream.close();
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    public void save(File file) throws IOException, ClassNotFoundException
    {
        synchronized (mas) {
            ObjectOutputStream objectOutputStream = new ObjectOutputStream(new FileOutputStream(file));
            objectOutputStream.writeObject(mas);
            objectOutputStream.flush();
            objectOutputStream.close();
        }
    }

    public ArrayList<Bee> load(File file) throws IOException, ClassNotFoundException
    {
        ObjectInputStream objectInputStream;
        try {
            objectInputStream = new ObjectInputStream(new FileInputStream(file));
        }catch (StreamCorruptedException e){
            System.out.println("Задан некорректный файл!");
            return null;
        }
            mas =(ArrayList<Bee>) objectInputStream.readObject();
            objectInputStream.close();
            return mas;
    }

    public void startSimulation(Window myWind)
    {
        wind.setMas(mas);
        workerAI = new BasicAI(mas,true);
        workerAI.start();
        trutAI = new BasicAI(mas,false);
        trutAI.start();
        isStarted = true;
        wind.setSimulationState(isStarted);
        myWind.start.setEnabled(false);
        myWind.stop.setEnabled(true);
        focusDisable(wind);
        isPaused = false;
        wind.isPainted = false;
    }

    public void stopSimulation(Window myWind)
    {
        workerAI.isFinished = true;
        trutAI.isFinished = true;

        isStarted = false;
        wind.setSimulationState(isStarted);
        wind.paintGr();
        focusDisable(wind);
        isPaused = true;
    }

    public void focusDisable(Window myWind)
    {
        myWind.start.setFocusable(false);
        myWind.stop.setFocusable(false);
        myWind.hide.setFocusable(false);
        myWind.show.setFocusable(false);
        myWind.infoShow.setFocusable(false);
        myWind.ok.setFocusable(false);
        myWind.cansel.setFocusable(false);

    }

    private class Updater extends TimerTask
    {
        Habitat obj;
        public Updater(Habitat hab)
        {
            obj = hab;
        }

        @Override
        public void run() {
            if (firstRun)
            {
                startTime = System.currentTimeMillis();
                lastTime = startTime;
                firstRun = false;
            }
            long curTime = System.currentTimeMillis();
            if(!isPaused) elapsed = (curTime - lastTime) / 1000.0;
            obj.update(elapsed);
            lastTime = System.currentTimeMillis();
        }
    }

    public void update(double elapsed)
    {
        wind.panel.setFocusable(false);
        if (isStarted)
        {
            secForTrut += elapsed;
            secForWorker += elapsed;
            time += elapsed;
            if (secForTrut > timeSpawnTruten) {
                if ((float) BeeTruten.trNumber / Bee.n < trutenPersent) {
                    synchronized (mas)
                    {
                        int key = (int) (Math.random() * 100000);
                        while (keys.contains(key))
                            key = (int) (Math.random() * 100000);
                        Bee tmpBee = new BeeTruten((int) (Math.random() * 720), 55 + (int) (Math.random() * 380), key, time, timeTrutLiving);
                        mas.add(tmpBee);
                        keys.add(key);
                        timeMap.put(tmpBee.uniqueKey, tmpBee.liveTime);
                    }
                }
                secForTrut -= timeSpawnTruten;
            }
            if (secForWorker > timeSpawnWorker) {
                if (Math.random() >= (1 - workerCreateChance)) {
                    synchronized (mas)
                    {
                        int key = (int) (Math.random() * 100000);
                        while (keys.contains(key))
                            key = (int) (Math.random() * 100000);
                        Bee tmpBee = new BeeWorker((int) (Math.random() * 720), 55 + (int) (Math.random() * 380), key, time, timeWorkLiving);
                        mas.add(tmpBee);
                        keys.add(key);
                        timeMap.put(tmpBee.uniqueKey, tmpBee.liveTime);
                    }
                }
                secForWorker -= timeSpawnWorker;
            }
            wind.setTextTime(time);
            synchronized (mas)
            {
                for (int i = 0; i < Bee.n; ++i) {
                    if (!mas.get(i).isAlive(elapsed)) {
                        if (mas.get(i) instanceof BeeTruten)
                            BeeTruten.trNumber -= 1;
                        keys.remove(mas.get(i).uniqueKey);
                        timeMap.remove(mas.get(i).uniqueKey);
                        mas.remove(i);
                        Bee.n -= 1;
                        i -= 1;
                    }
                }
            }
        }
        wind.paintGr();

        if (consThread != null)
        {
            int item;
            if (consThread.isInt) {
                try {
                    item = red.read();
                    if ((char) item == 'w')
                        consThread.num = Bee.n - BeeTruten.trNumber;
                    if ((char) item == 't')
                        consThread.num = BeeTruten.trNumber;
                    synchronized (consThread) {
                        consThread.notify();
                    }

                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }
}
