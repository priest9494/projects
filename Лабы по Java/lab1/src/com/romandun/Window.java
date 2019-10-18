package com.romandun;

import javax.swing.*;
import javax.swing.plaf.ColorUIResource;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.image.BufferStrategy;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Formatter;


public class Window extends Canvas {

    JFrame fr = new JFrame();
    ArrayList<Bee> mas;
    Image worker;
    Image trut;
    String time = "";
    boolean timeState = true;
    boolean simulationState = true;
    boolean showInfo = false;
    JButton start = new JButton("Старт");
    static JPanel panel = new JPanel();
    JButton stop = new JButton("Стоп");
    JRadioButton show = new JRadioButton("Показывать время");
    JRadioButton hide = new JRadioButton("Скрывать время");
    ButtonGroup group = new ButtonGroup();
    JRadioButton infoShow = new JRadioButton("Показывать информацию");
    JButton ok = new JButton("Ок");
    JButton cansel = new JButton("Отмена");
    JDialog dialog = new JDialog(fr, "Информация", true);
    JDialog consoleWind = new JDialog(fr, "Консоль", false);

    JPanel pan = new JPanel();
    TextArea txt = new TextArea(10,10);
    JSlider slide = new JSlider(JSlider.HORIZONTAL,0,100,0);
    JSlider slide2 = new JSlider(JSlider.HORIZONTAL, 0, 100, 0);
    JLabel infoAboutWorker = new JLabel("Вероятность рождения рабочего");
    JLabel getInfoAboutTruten = new JLabel("Допустимая доля трутней");
    JLabel slidLab = new JLabel();
    JLabel empty = new JLabel("                                         ");
    JLabel empty1 = new JLabel("                                         ");
    JLabel timeWorkerInfo = new JLabel("Время рождения рабочего");
    JLabel timeTrutenInfo = new JLabel("Время рождения трутня");
    JTextField workerCreate = new JTextField("3", 4);
    JTextField trutenCreate = new JTextField("5",4);
    JDialog excep = new JDialog(fr, "",true);
    JDialog curBeeDialog = new JDialog(fr, " ", true);
    JMenuBar menuBar = new JMenuBar();
    JMenu menu = new JMenu("Menu");
    JMenuItem mStart = new JMenuItem("Старт");
    JMenuItem mStop = new JMenuItem("Стоп");
    JToolBar bar = new JToolBar();
    JButton barBut = new JButton("bar");
    JButton menuStart = new JButton("Старт");
    JButton menuStop = new JButton("Стоп");
    JRadioButton barShowTime = new JRadioButton("Показывать время");
    JRadioButton barHideTime = new JRadioButton("Скрывать время");
    ButtonGroup gr2 = new ButtonGroup();
    JCheckBox barInfoShow = new JCheckBox("Показывать информацию");
    boolean isPainted = false;
    JLabel liveTrutLab = new JLabel("Время жизни трутней");
    JLabel liveWorkLab = new JLabel("Время жизни рабочих");
    JTextField liveTrutField = new JTextField("7",4);
    JTextField liveWorkField = new JTextField("10",4);
    JButton curBee = new JButton("          Текущие объекты           ");
    JTextArea curBeeTxt = new JTextArea(10,10);
    JComboBox priorityStream2;
    JComboBox priorityStream;
    JButton startStream1 = new JButton("   Рабочие   ");
    JButton startStream2 = new JButton("   Трутни   ");
    JButton console = new JButton("Консоль");
    JButton save = new JButton("Сохранить");
    JButton load = new JButton("Загрузить");
    JFileChooser saveOrLoad = new JFileChooser();
    JButton serv = new JButton("Сервер");
    JDialog serverWind = new JDialog(fr, "Сервер", false);
    JTextArea servTxtArea = new JTextArea(15,15);
    JTextField servSendBeeCount = new JTextField(15);
    JTextField servIdToSend = new JTextField(15);
    JButton sendBut = new JButton("Отправить");

    BufferStrategy bf;
    Graphics gr;

    Window() {
        String[] items = {
                "1",
                "2",
                "3",
                "4",
                "5",
                "6",
                "7",
                "8",
                "9",
                "10"
        };
        priorityStream = new JComboBox(items);
        priorityStream2 = new JComboBox(items);
        fr.setPreferredSize(new Dimension(1050, 550));
        fr.setLocation(150,100);
        fr.setResizable(false);
        fr.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        fr.setDefaultLookAndFeelDecorated(true);
        worker = new ImageIcon("Roman.png").getImage();
        trut = new ImageIcon("Dun.png").getImage();
        mStart.setPreferredSize(new Dimension(70,45));
        fr.add(panel, BorderLayout.EAST);
        bar.setVisible(true);
        panel.setPreferredSize(new Dimension(270,500));
        bar.addSeparator();
        gr2.add(barHideTime);
        gr2.add(barShowTime);
        bar.add(barHideTime);
        bar.add(barShowTime);
        bar.add(barInfoShow);
        bar.add(priorityStream);
        bar.add(priorityStream2);
        bar.add(serv);
        fr.setJMenuBar(menuBar);
        menuBar.add(menu);
        menuBar.add(bar);
        menu.add(mStart);
        menu.add(mStop);
        menu.add(console);
        panel.add(start);
        panel.add(stop);
        mStart.add(menuStart);
        mStop.add(menuStop);
        group.add(show);
        group.add(hide);
        setFocusable(true);
        panel.add(show);
        panel.add(hide);
        panel.add(infoShow);
        show.doClick();

        //worker chance
        panel.add(empty);
        panel.add(infoAboutWorker);
        slidLab.setOpaque(true);
        slide.setMajorTickSpacing(10);
        slide.setMinorTickSpacing(0);
        slide.setPaintLabels(true);
        slide.setPaintTicks(true);
        panel.add(slidLab);
        panel.add(slide);

        //truten chance
        panel.add(empty);
        panel.add(getInfoAboutTruten);
        slidLab.setOpaque(true);
        slide2.setMajorTickSpacing(10);
        slide2.setMinorTickSpacing(0);
        slide2.setPaintLabels(true);
        slide2.setPaintTicks(true);
        panel.add(slidLab);
        panel.add(slide2);

        //worker create
        panel.add(timeWorkerInfo);
        panel.add(workerCreate);

        //truten create
        //panel.add(empty);
        panel.add(timeTrutenInfo);
        panel.add(trutenCreate);

        // living time
        panel.add(liveTrutLab);
        panel.add(liveTrutField);

        panel.add(liveWorkLab);
        panel.add(liveWorkField);

        panel.add(curBee);
        panel.add(startStream1);
        panel.add(startStream2);

        panel.add(save);
        panel.add(load);

        fr.add(this);
        fr.pack();
        createBufferStrategy(3);
        bf = getBufferStrategy();
        fr.setVisible(true);


    }

    public  void setMas(ArrayList<Bee> mas)
    {
        this.mas = mas;
    }

    public void setTextTime(double sec)
    {
        DecimalFormat format = new DecimalFormat("#0.00");
        time = format.format(sec);
    }

    public void changeTimeState(boolean is)
    {
        timeState = is;
    }

    public void paintGr()
    {
        if (simulationState)
            paintSimulation();
        else
        paint_Statictics();
    }

    public void paint_Statictics()
    {
        if(!isPainted) {
            isPainted = true;
            dialog.setLayout(new BorderLayout());
            dialog.add(pan);
            dialog.setLocation(550, 300);
            txt.setEditable(false);
            pan.add(ok);
            pan.add(cansel);
            pan.setVisible(true);
            if (showInfo)
                txt.setText("Всего пчел: " + Bee.n + "\n" + "Трутней: " + BeeTruten.trNumber + "\n" + "Рабочих: "
                        + (Bee.n - BeeTruten.trNumber) + "\n" + "Время симуляции " + time);
            else
                txt.setText("Информация недоступна!");
            dialog.add(txt, BorderLayout.NORTH);
            dialog.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
            dialog.setSize(300, 250);
            dialog.setVisible(true);
        }
    }

    private void paintSimulation()
    {
        gr = bf.getDrawGraphics();
        super.paint(gr);
        gr.setFont(new Font("Arial",Font.ITALIC,13));
        for(int i =0; i < Bee.n ; i++)
        {

            if (mas.get(i) instanceof BeeWorker)
            {
                gr.drawImage(worker,(int)mas.get(i).getX(),(int)mas.get(i).getY(),this);

            }
            else {
                gr.drawImage(trut,(int)mas.get(i).getX(),(int)mas.get(i).getY(), this);
            }
        }
        if (timeState)
            gr.drawString("Прошло времени: " + time ,600,400);
        gr.dispose();
        bf.show();
    }

    public void setSimulationState(boolean st)
    {
        simulationState = st;
    }


}
