#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <list>
#include <map>
#include "Vertex.h"
#include "Edge.h"
#include "Form.h"
#include "Graph.h"
#include "Task1.h"
#include "Task2.h"

using namespace std;

typedef Vertex <string, int> TVertex;
typedef Edge<TVertex, int, int> TEdge;
typedef Graph <TVertex, TEdge> TGraph;
//typedef Task1<TVertex, TEdge> TEx2;
void getmenu();

void CreateCustomGraph(TGraph* &graph, TGraph::VertexIterator* &vIt, TGraph::EdgeIterator* &eIt, TGraph::OutputEdgeIterator* &oIt)
{
	graph = new TGraph(NULL, true, false);	//List
	vIt = new TGraph::VertexIterator(*graph);
	eIt = new TGraph::EdgeIterator(*graph);
}

void ClearMemory(TGraph* &graph, TGraph::VertexIterator* &vIt, TGraph::EdgeIterator* &eIt, TGraph::OutputEdgeIterator* &oIt)
{
	if (vIt != NULL)
		delete vIt;
	if (eIt != NULL)
		delete eIt;
	if (oIt != NULL)
		delete oIt;
	graph = NULL;
	vIt = NULL;
	eIt = NULL;
	oIt = NULL;
}

void getmenu() {
	cout << "Меню:" << endl;
	cout << "Создание графов:" << endl;
	cout << "\t0. Создать пустой L-граф с нулевым числом вершин и ребер" << endl;
	cout << "\t1. Создать граф с V вершинами, без ребер, типа D, формы представления F" << endl;
	cout << "\t2. Создать граф с V вершинами, с E случайными ребрами, типа D, формы F" << endl;

	cout << "Операции над графом:" << endl;
	cout << "\t3. Распечатать граф" << endl;
	cout << "\t4. Опрос числа вершин в графе" << endl;
	cout << "\t5. Опрос числа ребер в графе" << endl;
	cout << "\t6. Опрос типа графа" << endl;
	cout << "\t7. Опрос формы представления графа" << endl;
	cout << "\t8. Опрос коэффициента насыщенности графа" << endl;
	cout << "\t9. Преобразовать граф к L-форме" << endl;
	cout << "\t10. Преобразовать граф к M-форме" << endl;
	cout << "\t11. Добавить вершину к графу" << endl;
	cout << "\t12. Удалить вершину из графа" << endl;
	cout << "\t13. Вставить ребро в граф" << endl;
	cout << "\t14. Удалить ребро из графа" << endl;

	cout << "Операции над вершинами:" << endl;
	cout << "\t15. Прочитать данные о вершине" << endl;
	cout << "\t16. Записать данные о вершине" << endl;
	cout << "\t17. Перезаписать имя вершины" << endl;

	cout << "Операции над ребрами:" << endl;
	cout << "\t18. Прочитать вес ребра" << endl;
	cout << "\t19. Прочитать данные о ребре" << endl;
	cout << "\t20. Записать данные о ребре" << endl;
	cout << "\t21. Записать вес ребра" << endl;

	cout << "Итератор вершин:" << endl;
	cout << "\t22. Установить итератор вершин на начальную вершину" << endl;
	cout << "\t23. Установить итератор вершин на конечную вершину" << endl;
	cout << "\t24. Переход к следующей позиции" << endl;
	cout << "\t25. Прочитать имя вершины по текущей позиции итератора" << endl;
	cout << "\t26. Прочитать данные вершины по текущей позиции итератора" << endl;
	cout << "\t27. Записать данные вершины по текущей позиции итератора" << endl;
	cout << "\t28. Записать имя вершины по текущей позиции итератора" << endl;

	cout << "Итератор ребер:" << endl;
	cout << "\t29. Установить итератор ребер на начальное ребро" << endl;
	cout << "\t30. Установить итератор ребер на конечное ребро" << endl;
	cout << "\t31. Переход к следующей позиции" << endl;
	cout << "\t32. Прочитать ребро (вх., исх вершины, вес) по текущей позиции итератора" << endl;
	cout << "\t33. Прочитать данные ребра по текущей позиции итератора" << endl;
	cout << "\t34. Записать данные о ребре по текущей позиции итератора" << endl;
	cout << "\t35. Записать вес ребра по текущей позиции итератора" << endl;

	cout << "Итератор исходящих ребер" << endl;
	cout << "\t36. Создать итератор исходящих ребер" << endl;
	cout << "\t37. Установить итератор исходящих ребер на начальное ребро" << endl;
	cout << "\t38. Установить итератор исходящих ребер на конечное ребро" << endl;
	cout << "\t39. Переход к следующей позиции" << endl;
	cout << "\t40. Прочитать вершины ребра и вес по текущей позиции итератора" << endl;
	cout << "\t41. Прочитать данные ребра по текущей позиции итератора" << endl;
	cout << "\t42. Записать данные о ребре по текущей позиции итератора" << endl;
	cout << "\t43. Записать вес ребра по текущей позиции итератора" << endl;
	cout << "\t44. Задача 1 (эйлеров цикл в неорграфе)" << endl;
	cout << "\t45. Задача 2 (центр взвешенного орграфа по алгоритму Флойда)" << endl;

	cout << "99. Вывести список команд" << endl;
	cout << "100. Выход" << endl;
}

int main(int argc, char* argv[])
{
	bool on = false;		//Итератор исход реб
	//Граф
	TGraph* graph = NULL;
	//Итераторы
	TGraph::VertexIterator *vIt = NULL;
	TGraph::EdgeIterator *eIt = NULL;
	TGraph::OutputEdgeIterator *oIt = NULL;

	EulerCycle<TVertex, TEdge>* eCycle = NULL;
	CentreFloyd<TVertex, TEdge>* cFloyd = NULL;

	map<string, TVertex*> MAP;

	CreateCustomGraph(graph, vIt, eIt, oIt);

	bool isExit = false;	//Флаг выхода
	setlocale(LC_ALL, "Russian");
	getmenu();
	while (!isExit)
	{
		int ans;	//Код нажатой клавиши
		cin >> ans;

		if (graph == NULL && ans > 2 && ans < 100)
		{
			cout << "Граф не создан" << endl;
			continue;
		}

		if (oIt == NULL && ans > 36 && ans < 44)
		{
			cout << "Итератор не создан" << endl;
			continue;
		}

		try
		{
			switch (ans)
			{
			case 0://Создать пустой L-граф с нулевым числом вершин и ребер			
			{
				MAP.clear();

				ClearMemory(graph, vIt, eIt, oIt);
				graph = new TGraph();
				vIt = new TGraph::VertexIterator(*graph);
				eIt = new TGraph::EdgeIterator(*graph);
				cout << endl << "Граф создан" << endl;
				break;
			}
			case 1:	//Создать граф с V вершинами, без ребер, типа D, формы представления F
			{
				int tmp1, tmp2, tmp3;
				cout << "Введите количество вершин: ";
				cin >> tmp1;
				cout << "Ориентированный ли граф (0 - нет, 1 - да): ";
				cin >> tmp2;
				if (tmp2 == 1)
					on = true;
				cout << "Форму графа (0 - список, 1 - матрица): ";
				cin >> tmp3;
				ClearMemory(graph, vIt, eIt, oIt);
				graph = new TGraph(0, tmp2, tmp3);
				TVertex *v;
				MAP.clear();
				for (int i = 0; i < tmp1; i++)
				{
					v = graph->InsertV();
					MAP.insert(pair<string, TVertex*>(v->getName(), v));
				}
				cout << MAP.size() << endl;
				for (int i = 0; i < tmp1; i++)
					cout << MAP.at(to_string(i))->getName() << endl;
				vIt = new TGraph::VertexIterator(*graph);
				eIt = new TGraph::EdgeIterator(*graph);
				cout << endl << "Граф создан" << endl;
				break;
			}

			case 2://Создать граф с V вершинами, с E случайными ребрами, типа D , формы F
			{
					   int tmp1, tmp2, tmp3, tmp4;
					   cout << "Введите количество вершин: ";
					   cin >> tmp1;
					   cout << "Введите количество ребер: ";
					   cin >> tmp2;
					   cout << "Ориентированный ли граф (0 - нет, 1 - да): ";
					   cin >> tmp3;
					   if (tmp3 == 1)
						   on = true;
					   cout << "Форму графа (0 - список, 1 - матрица): ";
					   cin >> tmp4;
					   ClearMemory(graph, vIt, eIt, oIt);
					   graph = new TGraph(tmp1, tmp2, tmp3, tmp4);
					   vIt = new TGraph::VertexIterator(*graph);
					   eIt = new TGraph::EdgeIterator(*graph);
					   cout << endl << "Граф создан" << endl;
					   break;
			}

			case 3:
				graph->print_graph();
				break;

			case 4:
				cout << "Число вершин в графе: " << graph->V() << endl;
				break;

			case 5:
				cout << "Число ребер в графе: " << graph->E() << endl;
				break;

			case 6:
				cout << "Тип графа (0 - неориентированный, 1 - ориентированный): " << graph->Directed() << endl;
				break;

			case 7:
				cout << "Форма предстваления графа (0 - L-граф, 1 - M-граф): " << graph->Dense() << endl;
				break;

			case 8:
				cout << "Коэффициент насыщенности: " << graph->K() << endl;
				break;

			case 9:
			{
				if (vIt != NULL)
					delete vIt;
				if (eIt != NULL)
					delete eIt;
				graph->ToListGraph();
				vIt = new TGraph::VertexIterator(*graph);
				eIt = new TGraph::EdgeIterator(*graph);
				cout << "Граф преобразован" << endl;
				break;
			}

			case 10:
			{
				if (vIt != NULL)
					delete vIt;
				if (eIt != NULL)
					delete eIt;
				graph->ToMatrixGraph();
				vIt = new TGraph::VertexIterator(*graph);
				eIt = new TGraph::EdgeIterator(*graph);
				cout << "Граф преобразован" << endl;
				break;
			}
			case 11:
			{
				TVertex *v = graph->InsertV();
				MAP.insert(pair<string, TVertex*>(v->getName(), v));
				cout << "Вершина добавлена в граф" << endl;
				break;
			}
			case 12:
			{
				string v1;
				cout << "Введите удаляемую вершину: ";
				cin >> v1;
				TVertex *v = MAP.at(v1);
				cout << "Вершина удалена (1 - да, 0 - нет): " << graph->DeleteV(v) << endl;
				MAP.erase(v1);
				break;
			}

			case 13:
			{
				string v1, v2;
				cout << "Введите вершину, из которой выходит ребро: ";
				cin >> v1;
				cout << "Введите вершину, в которую входит ребро: ";
				cin >> v2;
				cout << "Ребро добавлено в граф (1 - да, 0 - нет): ";
				try {
					graph->InsertE(MAP.at(v1), MAP.at(v2));
				}
				catch (char*) {
					cout << "0" << endl;
					break;
				}
				cout << "1" << endl;
				break;
			}

			case 14:
			{
				string v1, v2;
				cout << "Введите вершину для удаления 1: ";
				cin >> v1;
				cout << "Введите вершину для удаления 2: ";
				cin >> v2;
				cout << "Ребро удалено (1 - да, 0 - нет): " << graph->DeleteE(MAP.at(v1), MAP.at(v2)) << endl;
				break;
			}

				//Операции над вершинами
			case 15://Прочитать данные о вершине
			{
				int tmp;
				string v1;
				cout << "Введите имя вершины: ";
				cin >> v1;
				TVertex *v = MAP.at(v1);
				tmp = graph->read_data_vertex(v);
				if (tmp != -1)
					cout << endl << "Данные вершины: " << tmp << endl;
				break;
			}

			case 16://Записать данные о вершине
			{
				int tmp;
				string v1;
				cout << "Введите имя вершины: ";
				cin >> v1;
				cout << "Введите данные: ";
				cin >> tmp;
				TVertex *v = MAP.at(v1);
				cout << endl << "Данные о вершине записаны (1 - да, 0 - нет): " << graph->write_data_vertex(v, tmp) << endl;
				break;
			}
			case 17://Записать новое имя вершины
			{
				string v1, v2;
				cout << "Введите старое имя вершины: ";
				cin >> v1;
				cout << "Введите новое имя: ";
				cin >> v2;
				TVertex *v = MAP.at(v1);
				cout << "Имя вершины перезаписано (1 - да, 0 - нет): " << graph->write_name_vertex(v, v2) << endl;
				MAP.insert(pair<string, TVertex*>(v2, v));
				MAP.erase(v1);
				break;
			}
			case 18://Прочитать вес ребра
			{
				int tmp;
				string v1, v2;
				cout << "Введите имя исходящей вершины: ";
				cin >> v1;
				cout << "Введите имя входящей вершины: ";
				cin >> v2;
				tmp = graph->read_weight_edge(MAP.at(v1), MAP.at(v2));
				if (tmp != -1)
					cout << "Вес ребра: " << tmp << endl;
				break;
			}

			case 19://Прочитать данные о ребре
			{
				int tmp;
				string v1, v2;
				cout << "Введите имя исходящей вершины: ";
				cin >> v1;
				cout << "Введите имя входящей вершины: ";
				cin >> v2;
				tmp = graph->read_data_edge(MAP.at(v1), MAP.at(v2));
				if (tmp != -1)
					cout << "Данные ребра: " << tmp << endl;
				break;
			}

			case 20://Записать данные о ребре
			{
				int tmp;
				bool tmp1;
				string v1, v2;
				cout << "Введите имя исходящей вершины: ";
				cin >> v1;
				cout << "Введите имя входящей вершины: ";
				cin >> v2;
				cout << "Введите данные: ";
				cin >> tmp;
				tmp1 = graph->write_data_edge(MAP.at(v1), MAP.at(v2), tmp);
				if (tmp1 == true)
					cout << "Данные о вершине записаны (1 - да, 0 - нет): " << tmp1 << endl;
				break;
			}

			case 21://Записать вес ребра
			{
				int tmp;
				bool tmp1;
				string v1, v2;
				cout << "Введите имя исходящей вершины: ";
				cin >> v1;
				cout << "Введите имя входящей вершины: ";
				cin >> v2;
				cout << "Введите вес: ";
				cin >> tmp;
				tmp1 = graph->write_weight_edge(MAP.at(v1), MAP.at(v2), tmp);
				if (tmp1 == true)
					cout << "Вес ребра перезаписан (1 - да, 0 - нет): " << tmp1 << endl;
				break;
			}

				//Итератор вершин
			case 22://Установить итератор вершин на начальную вершину
			{
				if (!(vIt->begin()))
					cout << "Итератор за пределами графа" << endl;
				break;
			}
			case 23://Установить итератор вершин на конечную вершину
			{
				if (!(vIt->toend()))
					cout << "Итератор за пределами графа" << endl;
				break;
			}

			case 24://Переход к следующей позиции	
			{
				bool tmp;
				tmp = ++*vIt;
				break;
			}

			case 25://Прочитать имя вершины по текущей позиции итератора
			{
				cout << vIt->read_vertex() << endl;
				break;
			}

			case 26://Прочитать данные вершины по текущей позиции итератора
			{
				cout << vIt->read_data_vertex() << endl;
				break;
			}

			case 27://Записать данные о вершины по текущей позиции итератора
			{
				int tmp;
				bool tmp2;
				cout << "Введите данные: ";
				cin >> tmp;
				tmp2 = vIt->write_data_vertex(tmp);
				break;
			}

			case 28://Записать имя вершины по текущей позиции итератора
			{
				string cur;
				bool tmp;
				cout << "Введите имя: ";
				cin >> cur;
				tmp = vIt->write_name_vertex(cur);
				break;
			}

				//Итератор ребер
			case 29://Установить итератор ребер на начальное ребро
			{
				if (eIt->begin())
					cout << "Итератор установлен на начальное ребро" << endl;
				break;
			}

			case 30://Установить итератор ребер на конечное ребро
			{
				if (eIt->toend())
					cout << "Итератор установлен на конечное ребро" << endl;
				break;
			}

			case 31://Переход к следующей позиции
			{
				if (!(++(*eIt)))
					cout << "Итератор за пределами графа" << endl;
				break;
			}

			case 32://Прочитать вершины ребра и вес по текущей позиции итератора
			{
				cout << eIt->read_edge() << endl;
				break;
			}

			case 33://Прочитать данные ребра по текущей позиции итератора
			{
				cout << eIt->read_data_edge() << endl;
				break;
			}

			case 34://Записать данные о ребре по текущей позиции итератора
			{
				int tmp;
				cout << "Введите данные: ";
				cin >> tmp;
				eIt->write_data_edge(tmp);
				break;
			}

			case 35://Записать вес ребра по текущей позиции итератора
			{
				int tmp;
				cout << "Введите вес: ";
				cin >> tmp;
				eIt->write_weight_edge(tmp);
				break;
			}

			case 36:
			{
				string v;
				cout << "Введите имя вершины: ";
				cin >> v;
				oIt = new TGraph::OutputEdgeIterator(*graph, *(graph->getVertexFromName(v)));
				cout << "Итератор создан" << endl;
				break;
			}

				//Итератор исходящих ребер
			case 37://Установить итератор исходящих ребер на начальное ребро
			{
				if (oIt->begin())
					cout << "Итератор установлен на начальное ребро" << endl;
				break;
			}

			case 38://Установить итератор исходящих ребер на конечное ребро	
			{
				if (!(oIt->toend()))
					cout << "Итератор установлен на конечное ребро" << endl;
				break;
			}

			case 39://Переход к следующей позиции
			{
				if (!(++(*oIt)))
					cout << "Итератор за пределами графа" << endl;
				break;
			}

			case 40://Прочитать вершины ребра и вес по текущей позиции итератора
			{
				cout << oIt->read_edge() << endl;
				break;
			}

			case 41://Прочитать данные ребра по текущей позиции итератора
			{
				int tmp;
				tmp = oIt->read_data_edge();
				cout << tmp << endl;
				break;
			}

			case 42://Записать данные о ребре по текущей позиции итератора
			{
				int tmp;
				cout << "Введите данные: ";
				cin >> tmp;
				oIt->write_data_edge(tmp);
				break;
			}

			case 43://Записать вес ребра по текущей позиции итератора
			{
				int tmp;
				cout << "Введите вес: ";
				cin >> tmp;
				oIt->write_weight_edge(tmp);
				break;
			}
			case 44://Задача 1
			{
				eCycle = new EulerCycle<TVertex, TEdge>();
				cout << eCycle->set(graph) << endl;
				break;
			}
			case 45: {
				cout << eCycle->restart() << endl;
				break;
			}
			case 46: {
				vector<int> eulerRes = eCycle->result();
				for (int i = 0; i < eulerRes.size(); i++) {
					if (i == 0) {
						cout << eulerRes[i];
						continue;
					}
					cout << " -> " << eulerRes[i];
				}
				cout << endl;
				break;
			}
			case 47://Задача 2
			{
				cFloyd = new CentreFloyd<TVertex, TEdge>();
				cout << cFloyd->set(graph) << endl;
				break;
			}
			case 48:{
				cout << cFloyd->restart() << endl;
				break;
			}
			case 49: {
				vector<int> floydRes= cFloyd->result();
				for (int i = 0; i < floydRes.size(); i++)
					cout << floydRes[i] << "  ";
				break;
			}
			case 99://Вывод меню
			{
				getmenu();
				break;
			}

			case 100:	//Выход
				isExit = true;
				break;

			case 101:	//Создать готовый граф:
				//Очистка памяти:
				ClearMemory(graph, vIt, eIt, oIt);
				graph = new TGraph(6, true, true);	//List
				graph->InsertE("0", "1");
				graph->InsertE("0", "3");
				graph->InsertE("0", "4");
				graph->InsertE("1", "2");
				graph->InsertE("2", "3");
				graph->InsertE("3", "1");
				graph->InsertE("4", "2");
				graph->InsertE("4", "5");
				/*graph->write_weight_edge("0", "1", 3);
				graph->write_weight_edge("0", "3", 11);
				graph->write_weight_edge("1", "2", 1);
				graph->write_weight_edge("2", "3", 3);
				graph->write_weight_edge("3", "1", 8);
				graph->write_weight_edge("4", "2", 4);
				graph->write_weight_edge("4", "5", 6);*/
				//Ориентированный ли граф (false - нет, true - да)
				//Форму графа (false - список, true - матрица)
				vIt = new TGraph::VertexIterator(*graph);
				eIt = new TGraph::EdgeIterator(*graph);
				break;

			default:	//Нажата клавиша, не соответствующая ни одной команде
				cout << "Неизвестная команда" << endl;
				break;
			}
		}
		catch (char* err)
		{
			cout << err << endl;
		}
		catch (exception& exc)
		{
			cout << exc.what() << endl;
		}
	}

	//Очистка памяти:
	ClearMemory(graph, vIt, eIt, oIt);

	return 0;
}

