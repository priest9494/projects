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
	cout << "����:" << endl;
	cout << "�������� ������:" << endl;
	cout << "\t0. ������� ������ L-���� � ������� ������ ������ � �����" << endl;
	cout << "\t1. ������� ���� � V ���������, ��� �����, ���� D, ����� ������������� F" << endl;
	cout << "\t2. ������� ���� � V ���������, � E ���������� �������, ���� D, ����� F" << endl;

	cout << "�������� ��� ������:" << endl;
	cout << "\t3. ����������� ����" << endl;
	cout << "\t4. ����� ����� ������ � �����" << endl;
	cout << "\t5. ����� ����� ����� � �����" << endl;
	cout << "\t6. ����� ���� �����" << endl;
	cout << "\t7. ����� ����� ������������� �����" << endl;
	cout << "\t8. ����� ������������ ������������ �����" << endl;
	cout << "\t9. ������������� ���� � L-�����" << endl;
	cout << "\t10. ������������� ���� � M-�����" << endl;
	cout << "\t11. �������� ������� � �����" << endl;
	cout << "\t12. ������� ������� �� �����" << endl;
	cout << "\t13. �������� ����� � ����" << endl;
	cout << "\t14. ������� ����� �� �����" << endl;

	cout << "�������� ��� ���������:" << endl;
	cout << "\t15. ��������� ������ � �������" << endl;
	cout << "\t16. �������� ������ � �������" << endl;
	cout << "\t17. ������������ ��� �������" << endl;

	cout << "�������� ��� �������:" << endl;
	cout << "\t18. ��������� ��� �����" << endl;
	cout << "\t19. ��������� ������ � �����" << endl;
	cout << "\t20. �������� ������ � �����" << endl;
	cout << "\t21. �������� ��� �����" << endl;

	cout << "�������� ������:" << endl;
	cout << "\t22. ���������� �������� ������ �� ��������� �������" << endl;
	cout << "\t23. ���������� �������� ������ �� �������� �������" << endl;
	cout << "\t24. ������� � ��������� �������" << endl;
	cout << "\t25. ��������� ��� ������� �� ������� ������� ���������" << endl;
	cout << "\t26. ��������� ������ ������� �� ������� ������� ���������" << endl;
	cout << "\t27. �������� ������ ������� �� ������� ������� ���������" << endl;
	cout << "\t28. �������� ��� ������� �� ������� ������� ���������" << endl;

	cout << "�������� �����:" << endl;
	cout << "\t29. ���������� �������� ����� �� ��������� �����" << endl;
	cout << "\t30. ���������� �������� ����� �� �������� �����" << endl;
	cout << "\t31. ������� � ��������� �������" << endl;
	cout << "\t32. ��������� ����� (��., ��� �������, ���) �� ������� ������� ���������" << endl;
	cout << "\t33. ��������� ������ ����� �� ������� ������� ���������" << endl;
	cout << "\t34. �������� ������ � ����� �� ������� ������� ���������" << endl;
	cout << "\t35. �������� ��� ����� �� ������� ������� ���������" << endl;

	cout << "�������� ��������� �����" << endl;
	cout << "\t36. ������� �������� ��������� �����" << endl;
	cout << "\t37. ���������� �������� ��������� ����� �� ��������� �����" << endl;
	cout << "\t38. ���������� �������� ��������� ����� �� �������� �����" << endl;
	cout << "\t39. ������� � ��������� �������" << endl;
	cout << "\t40. ��������� ������� ����� � ��� �� ������� ������� ���������" << endl;
	cout << "\t41. ��������� ������ ����� �� ������� ������� ���������" << endl;
	cout << "\t42. �������� ������ � ����� �� ������� ������� ���������" << endl;
	cout << "\t43. �������� ��� ����� �� ������� ������� ���������" << endl;
	cout << "\t44. ������ 1 (������� ���� � ���������)" << endl;
	cout << "\t45. ������ 2 (����� ����������� ������� �� ��������� ������)" << endl;

	cout << "99. ������� ������ ������" << endl;
	cout << "100. �����" << endl;
}

int main(int argc, char* argv[])
{
	bool on = false;		//�������� ����� ���
	//����
	TGraph* graph = NULL;
	//���������
	TGraph::VertexIterator *vIt = NULL;
	TGraph::EdgeIterator *eIt = NULL;
	TGraph::OutputEdgeIterator *oIt = NULL;

	EulerCycle<TVertex, TEdge>* eCycle = NULL;
	CentreFloyd<TVertex, TEdge>* cFloyd = NULL;

	map<string, TVertex*> MAP;

	CreateCustomGraph(graph, vIt, eIt, oIt);

	bool isExit = false;	//���� ������
	setlocale(LC_ALL, "Russian");
	getmenu();
	while (!isExit)
	{
		int ans;	//��� ������� �������
		cin >> ans;

		if (graph == NULL && ans > 2 && ans < 100)
		{
			cout << "���� �� ������" << endl;
			continue;
		}

		if (oIt == NULL && ans > 36 && ans < 44)
		{
			cout << "�������� �� ������" << endl;
			continue;
		}

		try
		{
			switch (ans)
			{
			case 0://������� ������ L-���� � ������� ������ ������ � �����			
			{
				MAP.clear();

				ClearMemory(graph, vIt, eIt, oIt);
				graph = new TGraph();
				vIt = new TGraph::VertexIterator(*graph);
				eIt = new TGraph::EdgeIterator(*graph);
				cout << endl << "���� ������" << endl;
				break;
			}
			case 1:	//������� ���� � V ���������, ��� �����, ���� D, ����� ������������� F
			{
				int tmp1, tmp2, tmp3;
				cout << "������� ���������� ������: ";
				cin >> tmp1;
				cout << "��������������� �� ���� (0 - ���, 1 - ��): ";
				cin >> tmp2;
				if (tmp2 == 1)
					on = true;
				cout << "����� ����� (0 - ������, 1 - �������): ";
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
				cout << endl << "���� ������" << endl;
				break;
			}

			case 2://������� ���� � V ���������, � E ���������� �������, ���� D , ����� F
			{
					   int tmp1, tmp2, tmp3, tmp4;
					   cout << "������� ���������� ������: ";
					   cin >> tmp1;
					   cout << "������� ���������� �����: ";
					   cin >> tmp2;
					   cout << "��������������� �� ���� (0 - ���, 1 - ��): ";
					   cin >> tmp3;
					   if (tmp3 == 1)
						   on = true;
					   cout << "����� ����� (0 - ������, 1 - �������): ";
					   cin >> tmp4;
					   ClearMemory(graph, vIt, eIt, oIt);
					   graph = new TGraph(tmp1, tmp2, tmp3, tmp4);
					   vIt = new TGraph::VertexIterator(*graph);
					   eIt = new TGraph::EdgeIterator(*graph);
					   cout << endl << "���� ������" << endl;
					   break;
			}

			case 3:
				graph->print_graph();
				break;

			case 4:
				cout << "����� ������ � �����: " << graph->V() << endl;
				break;

			case 5:
				cout << "����� ����� � �����: " << graph->E() << endl;
				break;

			case 6:
				cout << "��� ����� (0 - �����������������, 1 - ���������������): " << graph->Directed() << endl;
				break;

			case 7:
				cout << "����� ������������� ����� (0 - L-����, 1 - M-����): " << graph->Dense() << endl;
				break;

			case 8:
				cout << "����������� ������������: " << graph->K() << endl;
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
				cout << "���� ������������" << endl;
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
				cout << "���� ������������" << endl;
				break;
			}
			case 11:
			{
				TVertex *v = graph->InsertV();
				MAP.insert(pair<string, TVertex*>(v->getName(), v));
				cout << "������� ��������� � ����" << endl;
				break;
			}
			case 12:
			{
				string v1;
				cout << "������� ��������� �������: ";
				cin >> v1;
				TVertex *v = MAP.at(v1);
				cout << "������� ������� (1 - ��, 0 - ���): " << graph->DeleteV(v) << endl;
				MAP.erase(v1);
				break;
			}

			case 13:
			{
				string v1, v2;
				cout << "������� �������, �� ������� ������� �����: ";
				cin >> v1;
				cout << "������� �������, � ������� ������ �����: ";
				cin >> v2;
				cout << "����� ��������� � ���� (1 - ��, 0 - ���): ";
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
				cout << "������� ������� ��� �������� 1: ";
				cin >> v1;
				cout << "������� ������� ��� �������� 2: ";
				cin >> v2;
				cout << "����� ������� (1 - ��, 0 - ���): " << graph->DeleteE(MAP.at(v1), MAP.at(v2)) << endl;
				break;
			}

				//�������� ��� ���������
			case 15://��������� ������ � �������
			{
				int tmp;
				string v1;
				cout << "������� ��� �������: ";
				cin >> v1;
				TVertex *v = MAP.at(v1);
				tmp = graph->read_data_vertex(v);
				if (tmp != -1)
					cout << endl << "������ �������: " << tmp << endl;
				break;
			}

			case 16://�������� ������ � �������
			{
				int tmp;
				string v1;
				cout << "������� ��� �������: ";
				cin >> v1;
				cout << "������� ������: ";
				cin >> tmp;
				TVertex *v = MAP.at(v1);
				cout << endl << "������ � ������� �������� (1 - ��, 0 - ���): " << graph->write_data_vertex(v, tmp) << endl;
				break;
			}
			case 17://�������� ����� ��� �������
			{
				string v1, v2;
				cout << "������� ������ ��� �������: ";
				cin >> v1;
				cout << "������� ����� ���: ";
				cin >> v2;
				TVertex *v = MAP.at(v1);
				cout << "��� ������� ������������ (1 - ��, 0 - ���): " << graph->write_name_vertex(v, v2) << endl;
				MAP.insert(pair<string, TVertex*>(v2, v));
				MAP.erase(v1);
				break;
			}
			case 18://��������� ��� �����
			{
				int tmp;
				string v1, v2;
				cout << "������� ��� ��������� �������: ";
				cin >> v1;
				cout << "������� ��� �������� �������: ";
				cin >> v2;
				tmp = graph->read_weight_edge(MAP.at(v1), MAP.at(v2));
				if (tmp != -1)
					cout << "��� �����: " << tmp << endl;
				break;
			}

			case 19://��������� ������ � �����
			{
				int tmp;
				string v1, v2;
				cout << "������� ��� ��������� �������: ";
				cin >> v1;
				cout << "������� ��� �������� �������: ";
				cin >> v2;
				tmp = graph->read_data_edge(MAP.at(v1), MAP.at(v2));
				if (tmp != -1)
					cout << "������ �����: " << tmp << endl;
				break;
			}

			case 20://�������� ������ � �����
			{
				int tmp;
				bool tmp1;
				string v1, v2;
				cout << "������� ��� ��������� �������: ";
				cin >> v1;
				cout << "������� ��� �������� �������: ";
				cin >> v2;
				cout << "������� ������: ";
				cin >> tmp;
				tmp1 = graph->write_data_edge(MAP.at(v1), MAP.at(v2), tmp);
				if (tmp1 == true)
					cout << "������ � ������� �������� (1 - ��, 0 - ���): " << tmp1 << endl;
				break;
			}

			case 21://�������� ��� �����
			{
				int tmp;
				bool tmp1;
				string v1, v2;
				cout << "������� ��� ��������� �������: ";
				cin >> v1;
				cout << "������� ��� �������� �������: ";
				cin >> v2;
				cout << "������� ���: ";
				cin >> tmp;
				tmp1 = graph->write_weight_edge(MAP.at(v1), MAP.at(v2), tmp);
				if (tmp1 == true)
					cout << "��� ����� ����������� (1 - ��, 0 - ���): " << tmp1 << endl;
				break;
			}

				//�������� ������
			case 22://���������� �������� ������ �� ��������� �������
			{
				if (!(vIt->begin()))
					cout << "�������� �� ��������� �����" << endl;
				break;
			}
			case 23://���������� �������� ������ �� �������� �������
			{
				if (!(vIt->toend()))
					cout << "�������� �� ��������� �����" << endl;
				break;
			}

			case 24://������� � ��������� �������	
			{
				bool tmp;
				tmp = ++*vIt;
				break;
			}

			case 25://��������� ��� ������� �� ������� ������� ���������
			{
				cout << vIt->read_vertex() << endl;
				break;
			}

			case 26://��������� ������ ������� �� ������� ������� ���������
			{
				cout << vIt->read_data_vertex() << endl;
				break;
			}

			case 27://�������� ������ � ������� �� ������� ������� ���������
			{
				int tmp;
				bool tmp2;
				cout << "������� ������: ";
				cin >> tmp;
				tmp2 = vIt->write_data_vertex(tmp);
				break;
			}

			case 28://�������� ��� ������� �� ������� ������� ���������
			{
				string cur;
				bool tmp;
				cout << "������� ���: ";
				cin >> cur;
				tmp = vIt->write_name_vertex(cur);
				break;
			}

				//�������� �����
			case 29://���������� �������� ����� �� ��������� �����
			{
				if (eIt->begin())
					cout << "�������� ���������� �� ��������� �����" << endl;
				break;
			}

			case 30://���������� �������� ����� �� �������� �����
			{
				if (eIt->toend())
					cout << "�������� ���������� �� �������� �����" << endl;
				break;
			}

			case 31://������� � ��������� �������
			{
				if (!(++(*eIt)))
					cout << "�������� �� ��������� �����" << endl;
				break;
			}

			case 32://��������� ������� ����� � ��� �� ������� ������� ���������
			{
				cout << eIt->read_edge() << endl;
				break;
			}

			case 33://��������� ������ ����� �� ������� ������� ���������
			{
				cout << eIt->read_data_edge() << endl;
				break;
			}

			case 34://�������� ������ � ����� �� ������� ������� ���������
			{
				int tmp;
				cout << "������� ������: ";
				cin >> tmp;
				eIt->write_data_edge(tmp);
				break;
			}

			case 35://�������� ��� ����� �� ������� ������� ���������
			{
				int tmp;
				cout << "������� ���: ";
				cin >> tmp;
				eIt->write_weight_edge(tmp);
				break;
			}

			case 36:
			{
				string v;
				cout << "������� ��� �������: ";
				cin >> v;
				oIt = new TGraph::OutputEdgeIterator(*graph, *(graph->getVertexFromName(v)));
				cout << "�������� ������" << endl;
				break;
			}

				//�������� ��������� �����
			case 37://���������� �������� ��������� ����� �� ��������� �����
			{
				if (oIt->begin())
					cout << "�������� ���������� �� ��������� �����" << endl;
				break;
			}

			case 38://���������� �������� ��������� ����� �� �������� �����	
			{
				if (!(oIt->toend()))
					cout << "�������� ���������� �� �������� �����" << endl;
				break;
			}

			case 39://������� � ��������� �������
			{
				if (!(++(*oIt)))
					cout << "�������� �� ��������� �����" << endl;
				break;
			}

			case 40://��������� ������� ����� � ��� �� ������� ������� ���������
			{
				cout << oIt->read_edge() << endl;
				break;
			}

			case 41://��������� ������ ����� �� ������� ������� ���������
			{
				int tmp;
				tmp = oIt->read_data_edge();
				cout << tmp << endl;
				break;
			}

			case 42://�������� ������ � ����� �� ������� ������� ���������
			{
				int tmp;
				cout << "������� ������: ";
				cin >> tmp;
				oIt->write_data_edge(tmp);
				break;
			}

			case 43://�������� ��� ����� �� ������� ������� ���������
			{
				int tmp;
				cout << "������� ���: ";
				cin >> tmp;
				oIt->write_weight_edge(tmp);
				break;
			}
			case 44://������ 1
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
			case 47://������ 2
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
			case 99://����� ����
			{
				getmenu();
				break;
			}

			case 100:	//�����
				isExit = true;
				break;

			case 101:	//������� ������� ����:
				//������� ������:
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
				//��������������� �� ���� (false - ���, true - ��)
				//����� ����� (false - ������, true - �������)
				vIt = new TGraph::VertexIterator(*graph);
				eIt = new TGraph::EdgeIterator(*graph);
				break;

			default:	//������ �������, �� ��������������� �� ����� �������
				cout << "����������� �������" << endl;
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

	//������� ������:
	ClearMemory(graph, vIt, eIt, oIt);

	return 0;
}

