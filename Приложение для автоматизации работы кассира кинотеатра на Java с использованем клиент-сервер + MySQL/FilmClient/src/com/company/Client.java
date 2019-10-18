package com.company;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

public class Client {
    Auth auth;  // Окно авторизации
    ClientWindow clientWindow; // Основное окно
    InetAddress ipAdress;
    DataInputStream in;
    DataOutputStream out;
    Socket soc;
    Hall_A hall_a; // Окно-схема зала А
    Hall_B hall_b; // Окно-схема зала В
    ArrayList<Film> filmArrayList = new ArrayList<>();
    ArrayList<JButton> places_A; // Массив для кнопок(сидячих мест) в зале А
    ArrayList<JButton> places_B; // Массив для кнопок(сидячих мест) в зале В
    int placesChoosed = 0; // Количество выбранных мест (чтобы вывести пользователю)
    int price = 0; // Цена билетов
    int filmChoosedPrice = 0; // Цена выбранного фильма (чтобы при отмене заказа вычетать цену билета из общей цены)
    Client()
    {
        // Заходя, загружаю все сеансы и всю информацию.
        // Храню массив свободных мест для каждого сеанса
        // Могу заходить в разные сеансы и бронировать разные места, количество мест неограничено
        // Бронь нельзя отменить
        // Порт 9999
        auth = new Auth();
        clientWindow = new ClientWindow();
        clientWindow.setVisible(false);

        // Слушатель кнопки "Войти" окна Авторизации
        auth.jButton1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(auth.login.getText().equals("admin") & auth.pass.getText().equals("admin"))
                {
                    auth.dispose();
                    clientWindow.setVisible(true);
                    try {
                        // Подключение к серверу
                        ipAdress = InetAddress.getByName("127.0.0.1");
                        soc = new Socket(ipAdress, 8814);
                        out = new DataOutputStream(soc.getOutputStream());
                        out.flush();
                        in = new DataInputStream(soc.getInputStream());
                        int numRows = in.readInt();
                        // Считывание с сервера данных о сеансах
                        while(numRows-- > 0)
                        {
                            filmArrayList.add(new Film(in.readInt(), in.readUTF(), in.readUTF(), in.readUTF(), in.readUTF(),in.readInt()));
                        }
                        // Добавление сеансов в выпадающий спсок
                        for (int i = 0; i < filmArrayList.size(); i++)
                        {
                            clientWindow.films.addItem(filmArrayList.get(i).name + " (" + filmArrayList.get(i).time + ") " + filmArrayList.get(i).price + " руб.");
                        }
                    } catch (Exception ex) {
                        ex.printStackTrace();
                    }
                }
            }
        });

        // Слушатель кнопки "Готово окна клиента"
        clientWindow.jButton1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("*Вылезает чек на сумму " + price + " рублей.*");
                price = 0;
                placesChoosed = 0;
                refreshPrice();
                refreshPlaces();
            }
        });

        // Слушатель кнопки "Выбрать" окна с сеансами
        clientWindow.choose.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                clientWindow.choose.setEnabled(false);
                int idx = clientWindow.films.getSelectedIndex();
                String av_places = filmArrayList.get(idx).av_places;
                filmChoosedPrice = filmArrayList.get(idx).price;

                // Выбран зал А
                if(filmArrayList.get(idx).hall.equals("A"))
                {
                    hall_a = new Hall_A(filmArrayList.get(idx).id);
                    // Обработка нажатия на кнопку "Забронировать"
                    hall_a.get.addActionListener(new ActionListener() {
                        @Override
                        public void actionPerformed(ActionEvent e) {
                            hall_a.dispose();
                            clientWindow.choose.setEnabled(true);
                            String newPlaces = "";
                            // В классе Hall_a или Hall_b храним список мест, которые пользователь забронировал,
                            // чтобы отправить запрос к БД на обновление свободных мест
                            for(int i = 0; i < hall_a.places.size(); i++)
                            {
                                if(i != 0)
                                    newPlaces += ",";
                                newPlaces += hall_a.places.get(i);
                            }
                            // Получаем обновленный список свободных мест( за вычетом мест, занятых пользователем)
                            String req = xor(av_places, newPlaces);
                            // Обновляем список свободных мест в нашем массиве с сеансами на стророне клиента
                            filmArrayList.get(hall_a.filmIdx - 1).av_places = req;
                            try {
                                // Передаем на сервер запрос для обновления свободных мест для соответсрвующего сеанса
                                out.writeInt(1);
                                out.writeInt(hall_a.filmIdx);
                                out.writeUTF(req);
                            } catch (IOException e1) {
                                e1.printStackTrace();
                            }

                        }
                    });
                    // Добавляем кнопки в массив кнопок для автоматизации процесса покраски мест и обработки событий
                    // КРАСНЫЙ - занято    ЗЕЛЕНЫЙ - выбрано    СЕРЫЙ - свободно
                    places_A = new ArrayList<>();
                    places_A.add(hall_a.b1);
                    places_A.add(hall_a.b2);
                    places_A.add(hall_a.b3);
                    places_A.add(hall_a.b4);
                    places_A.add(hall_a.b5);
                    places_A.add(hall_a.b6);
                    places_A.add(hall_a.b7);
                    places_A.add(hall_a.b8);
                    // Окно сеанса нельзя закрыть (только на кнопку "забронировать")
                    hall_a.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
                    for(int i = 0; i < places_A.size(); i++)
                    {
                        // Добалвение слушателя на каждую из кнопок
                        places_A.get(i).addActionListener(new ActionListener() {
                            @Override
                            public void actionPerformed(ActionEvent e) {
                                JButton change = (JButton) e.getSource();
                                // Если место было выбрано пользователем, повторный клик отменит выбор места
                                if(change.getBackground() == Color.GREEN)
                                {
                                    change.setBackground(Color.lightGray);
                                    placesChoosed--;
                                    price-= filmChoosedPrice;
                                    // Удаляем место из списка занятятых мест
                                    hall_a.places.remove(change.getText());
                                }
                                else // Если место свободно, пользователь может кликнуть по нему, чтобы выбрать для брони
                                {
                                    change.setBackground(Color.GREEN);
                                    placesChoosed++;
                                    price+= filmChoosedPrice;
                                    // Добавляем место в список занятых мест
                                    hall_a.places.add(change.getText());
                                }
                                // Обновляем отобраение информации о занятых местах и цене в окне пользователя
                                refreshPlaces();
                                refreshPrice();
                            }
                        });
                        // Красим места. Кнопки содержат текст(номер места). Если текст кнопки содержится
                        // в строке доступных мест, кнопка красится в серый
                        if(av_places.indexOf(places_A.get(i).getText()) != -1)
                        {
                            places_A.get(i).setBackground(Color.LIGHT_GRAY);
                        }
                        // Иначе в красный, т.к. место занято
                        else
                        {
                            places_A.get(i).setBackground(Color.RED);
                            places_A.get(i).setEnabled(false);
                        }
                    }
                }
                // Выбран зал В
                else
                {
                    // Тот же самый код, что и для зала А
                    hall_b = new Hall_B(filmArrayList.get(idx).id);
                    hall_b.get.addActionListener(new ActionListener() {
                        @Override
                        public void actionPerformed(ActionEvent e) {
                            hall_b.dispose();
                            clientWindow.choose.setEnabled(true);
                            String newPlaces = "";
                            for(int i = 0; i < hall_b.places.size(); i++)
                            {
                                if(i != 0)
                                    newPlaces += ",";
                                newPlaces += hall_b.places.get(i);
                            }
                            String req = xor(av_places, newPlaces);
                            filmArrayList.get(hall_b.filmIdx - 1).av_places = req;
                            try {
                                out.writeInt(1);
                                out.writeInt(hall_b.filmIdx);
                                out.writeUTF(req);
                            } catch (IOException e1) {
                                e1.printStackTrace();
                            }

                        }
                    });
                    places_B = new ArrayList<>();
                    places_B.add(hall_b.b1);
                    places_B.add(hall_b.b2);
                    places_B.add(hall_b.b3);
                    places_B.add(hall_b.b4);
                    places_B.add(hall_b.b5);
                    places_B.add(hall_b.b6);
                    places_B.add(hall_b.b7);
                    places_B.add(hall_b.b8);
                    places_B.add(hall_b.b9);
                    hall_b.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
                    for(int i = 0; i < places_B.size(); i++)
                    {
                        places_B.get(i).addActionListener(new ActionListener() {
                            @Override
                            public void actionPerformed(ActionEvent e) {
                                JButton change = (JButton) e.getSource();
                                if(change.getBackground() == Color.GREEN)
                                {
                                    change.setBackground(Color.lightGray);
                                    placesChoosed--;
                                    price-= filmChoosedPrice;
                                    hall_b.places.remove(change.getText());
                                }
                                else
                                {
                                    change.setBackground(Color.GREEN);
                                    placesChoosed++;
                                    price+= filmChoosedPrice;
                                    hall_b.places.add(change.getText());
                                }
                                refreshPlaces();
                                refreshPrice();
                            }
                        });
                        if(av_places.indexOf(places_B.get(i).getText()) != -1)
                        {
                            places_B.get(i).setBackground(Color.LIGHT_GRAY);
                        }
                        else
                        {
                            places_B.get(i).setBackground(Color.RED);
                            places_B.get(i).setEnabled(false);
                        }
                    }
                }
            }
        });
        // Закрытие потоков при закрытии окна пользователем и оповещение сервера об окончании сессии
        clientWindow.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
                try {
                    out.writeInt(0);
                    soc.close();
                    in.close();
                    out.close();
                } catch (Exception e1) {
                    e1.printStackTrace();
                }
            }
        });
    }
    public void refreshPlaces()
    {
        clientWindow.jLabel2.setText("Забронировано мест: " + placesChoosed);
    }
    public void refreshPrice()
    {
        clientWindow.jLabel3.setText("Цена: " + price + " руб.");
    }

    // Удаление повторяющихся символов строк (используется для обновления строки свободных мест)
    public String xor(String str1, String str2)
    {
        Set<Character> res = new HashSet<>();
        for(int i = 0; i < str1.length(); i++)
        {
            if(str2.indexOf(str1.charAt(i)) == -1 & str1.charAt(i) != ',')
                res.add(str1.charAt(i));
        }
        String result = "";
        Iterator iter = res.iterator();
        if(iter.hasNext())
            result += iter.next();
        while(iter.hasNext())
        {
            result += ",";
            result+= iter.next();
        }
        return result;
    }
}
