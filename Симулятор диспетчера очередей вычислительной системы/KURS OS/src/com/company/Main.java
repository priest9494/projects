package com.company;

import java.util.Vector;

public class Main {
    static int time = 0;
    private static int KV = 20; // квант времени, выделяемый задаче
    private static int TSV = 10; // время работы супервизора
    static Vector<Task> vector = new Vector<>();
    static int maxPrtIndex;
    static boolean sayAboutWait = false;

    public static class Task {
        int tPrih;
        int trudoemk;
        boolean isCome;
        int taskNum;
        int priority;
        boolean doEarly;
        int waitTimes;

        Task(int tPrih, int trudoemk, int num, int priority) {
            this.tPrih = tPrih;
            this.trudoemk = trudoemk;
            isCome = false;
            taskNum = num;
            this.priority = priority;
            doEarly = false;
            waitTimes =0;
        }
    }
    // Оповещение о поступлении задачи
    public static void checkTask()
    {
        for (int i = 0; i < vector.size(); i++) {
            if (time > vector.get(i).tPrih && vector.get(i).isCome == false) {
                System.out.println(vector.get(i).tPrih + "   Постуила задача " + vector.get(i).taskNum);
                vector.get(i).isCome = true;
            }
        }
    }

    //Подбор задачи к выполнению( для динамического приоритета)
    public static void getMaxPriority()
    {
        // Поиск задачи с наибольшим приоритетом
        for (int i = 0; i < vector.size(); i++) {
            if (vector.get(i).priority > vector.get(maxPrtIndex).priority && vector.get(i).isCome == true)
                maxPrtIndex = i;
        }
        // Если есть задачи с одинаковым приоритетом, выполняется та, что уже выполнялась
        for (int i = 0; i < vector.size(); i++)
        {
            if(vector.get(i).priority == vector.get(maxPrtIndex).priority && vector.get(maxPrtIndex).doEarly == false && vector.get(i).doEarly == true) {
                maxPrtIndex = i;
                System.out.println("Задача " + vector.get(maxPrtIndex).taskNum + " уже выполнялась, поэтому выполняем ее ранее");
            }
        }
        // Из двух задач с одинаковым приоритетом вперед будет выполнена та, что дольше ожидала выполнения
        for (int i = 0; i < vector.size(); i++)
        {
            if(vector.get(i).priority == vector.get(maxPrtIndex).priority && vector.get(i).waitTimes > vector.get(maxPrtIndex).waitTimes && vector.get(i).doEarly == true)
            {
                sayAboutWait = true;
                maxPrtIndex = i;
            }
        }
        for (int i = 0; i < vector.size(); i++)
            if(vector.get(i).isCome == true) {
                vector.get(i).waitTimes++;
                vector.get(i).doEarly = false;
            }
    }

    public static void main(String[] args) {
        // Задачи сформированы согласно таблице 1.
        Task task1 = new Task(12, 30, 1, 3);
        Task task2 = new Task(21, 60, 2, 5);
        Task task3 = new Task(32, 90, 3, 8);
        Task task4 = new Task(94, 20, 4, 3);
        Task task5 = new Task(350, 30, 5, 3);
        Task task6 = new Task(89, 60, 6, 5);
        Task task7 = new Task(194, 90, 7, 8);
        Task task8 = new Task(441, 20, 8, 3);
        Task task9 = new Task(441, 30, 9, 3);
        Task task10 = new Task(226, 60, 10, 5);

        vector.add(task1);
        vector.add(task2);
        vector.add(task3);
        vector.add(task4);
        vector.add(task5);
        vector.add(task6);
        vector.add(task7);
        vector.add(task8);
        vector.add(task9);
        vector.add(task10);

        if (false) { // ДО FIFO
            while (!vector.isEmpty()) {
                checkTask();
                int next = 0;
                for(int i = 0; i < vector.size(); i++)
                {
                    if(vector.get(i).isCome == true && vector.get(i).tPrih < vector.get(next).tPrih)
                        next = i;

                }


                if (vector.get(next).isCome == true)
                {
                    System.out.println((time) + "   Задача " + vector.get(next).taskNum + " запущена");
                    if (vector.get(next).trudoemk > KV) {
                        vector.get(next).trudoemk -= KV;
                        time += KV;
                        checkTask();
                        System.out.println(time + "   Истек квант времени задачи " + vector.get(next).taskNum);
                    } else
                        {
                            time += vector.get(next).trudoemk;
                            System.out.println(time + "   задача " + vector.get(next).taskNum + " завершена");
                            vector.remove(next);
                        }
                        System.out.println(time + "   Супервизор запущен");
                }
                time += TSV;
            }
        }
        if(true)
        {
            while(!vector.isEmpty()) // ДП от времени обслуживания
            {
                maxPrtIndex = 0;
                checkTask();
                getMaxPriority();
                if (vector.get(maxPrtIndex).isCome == true)
                {
                    if(sayAboutWait)
                    {
                        System.out.println(time + "   Задача " + vector.get(maxPrtIndex).taskNum + " будет запущена, т.к. имеет большой приоритет и ее время ожидания наибольшее ");
                        sayAboutWait =false;
                    }

                    System.out.println((time) + "   Задача " + vector.get(maxPrtIndex).taskNum + " запущена (приоритет = " + vector.get(maxPrtIndex).priority + ")");
                    if (vector.get(maxPrtIndex).trudoemk > KV)
                    {
                        vector.get(maxPrtIndex).doEarly = true;
                        vector.get(maxPrtIndex).trudoemk -= KV;
                        vector.get(maxPrtIndex).waitTimes = 0;
                        time += KV;
                        checkTask();
                        System.out.println(time + "   Истек квант времени задачи " + vector.get(maxPrtIndex).taskNum);
                        vector.get(maxPrtIndex).priority--;
                        System.out.println(time + "   Приоритет " + vector.get(maxPrtIndex).taskNum + " задачи изменен с " + (vector.get(maxPrtIndex).priority + 1) + " на " + vector.get(maxPrtIndex).priority);
                        checkTask();
                    } else
                    {
                        time += vector.get(maxPrtIndex).trudoemk;
                        System.out.println(time + "   задача " + vector.get(maxPrtIndex).taskNum + " завершена");
                        vector.remove(maxPrtIndex);
                    }
                    System.out.println(time + "   Супервизор запущен");
                }
                time += TSV;
            }
        }
    }
}
