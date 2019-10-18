#pragma once
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

string int_to_string(int x)	//�������������� ����� � ������
{
	stringstream ss;
	ss << x;
	return ss.str();
}


template <class VertexT, class EdgeT>
class Graph
{
private:
	vector<VertexT*> vertexVector;//������ �����������
	GraphForm<EdgeT> *data;       //������ �����
	bool directed;                //������������ ����
	bool dense;                   //�-����
	int edgeCounter;              //����� ����
	int getIndexFromName(string n);//��������� ������� ������� �� �����������
	int sz;

	bool vertex_has_name(string name);
public:
	Graph(); //����������� ������� L-�����
	Graph(int vertexCount, bool directed, bool dense); //����������� � �������� ������ ������, �������� ����������������� � ��������� ����
	Graph(int vertexCount, int edgeCount, bool directed, bool dense); //����������� � �������� ������ ������, ��������� ����, �������� ����������������� � ����
	Graph(const Graph<VertexT, EdgeT> &G); //����������� �����������
	~Graph(); //����������

	int V();//���������� ����� ������ � �����
	int E(); //���������� ����� ����� � �����
	bool Dense(); //���������� ����� ������������� �����
	bool Directed();//���������� ��� �����   
	double K(); //���������� ����������� ������������ �����	
	void ToListGraph();//����������� ���� � L-����� 
	void ToMatrixGraph();//����������� ���� � M-�����

	VertexT* InsertV();//��������� ������� � ����� � ���������� ����� ����������� ����� ��������� ������� 
	bool DeleteV(VertexT *v);//������� ������� �� �����, �������� ������� ����������� v 

	bool InsertE(string v1, string v2);//��������� ����� (v1, v2) � �����, ����������� �������, �������� �������� ������������ v1 � v2	
	EdgeT* InsertE(VertexT *v1, VertexT *v2);
	bool DeleteE(string v1, string v2);//������� �����, ����������� �������, �������� �������� ������������ v1 � v2
	bool DeleteE(VertexT *v1, VertexT *v2);
	EdgeT* getEdge(VertexT *v1, VertexT *v2);	//��������� �����
	bool hasEdge(VertexT *v1, VertexT *v2);		//�������� �� ��������������� �����   
	bool hasEdge(int v1, int v2);
	bool write_name_vertex(VertexT *v1, string str);//�������� ������ � �������
	bool write_data_vertex(VertexT *v1, int d);//�������� ������ � �������
	int read_data_vertex(VertexT *v1);//��������� ������ � �������
	void print_graph();//����� �� �����



	int read_weight_edge(VertexT *v1, VertexT *v2);//��������� ��� �����
	int read_data_edge(VertexT *v1, VertexT *v2);//��������� ������ � �����
	bool write_weight_edge(VertexT *v1, VertexT *v2, int w);//�������� ��� �����
	bool  write_data_edge(VertexT *v1, VertexT *v2, int data);//�������� ������ � �����


	VertexT* getVertexFromName(string n);
	VertexT* getVertex(int index);
	//����� ������, ����, ������������
	int getIndex(VertexT *v);

	//�������� ������
	class VertexIterator
	{
	private:
		Graph<VertexT, EdgeT> *graph; //�������������� ����
		bool end;	//��������� ���������
		int current;	//������� ���������
	public:
		VertexIterator(Graph<VertexT, EdgeT> &g) { graph = &g; }; //�����������

		//���������� ��������, ������������� �� ������ ������� �����
		bool begin()
		{
			if (graph->vertexVector.size() == 0)
			{
				end = true;
				return false;
			}
			current = 0;
			end = false;
			return true;
		}

		//������� � ��������� �������
		bool operator++()
		{
			try {
				if (end) {
					current = -1;
					throw "������� ������ �� ������� �����";
				}
			}
			catch (char*e) {
				cerr << endl << e << endl;
				return false;
			}
			current++;
			if (graph->vertexVector.size() - 1 == current)
				end = true;
			return true;
		}

		//���������� �������� ������ �� �������� �������
		bool toend()
		{
			end = true;
			current = graph->vertexVector.size() - 1;
			if (current < 0)
				return false;
			else
				return true;
		}

		//�������� �� �����
		bool onEnd()
		{
			return end;
		}

		//��������� �������
		VertexT* operator*()
		{
			if (current >= graph->vertexVector.size() || current == -1)
				throw "������ ���������� � �������";
			return graph->vertexVector[current];
		}

		//��������� ������� �� ������� ������� ���������
		string read_vertex()
		{
			VertexT *v = NULL;
			try
			{
				v = operator*();
			}
			catch (...) { throw; }
			if (v->isNameSet())
				return v->getName();
			else
				throw "��� ������� �� �����������";
		}

		//��������� ������ ������� �� ������� ������� ���������
		int read_data_vertex()
		{
			VertexT *v = NULL;
			try
			{
				v = operator*();
			}
			catch (...) { throw; } {
				if (v->isDataSet())
					return v->getData();
				else throw "������ ������� �� �����������";
			}
		}

		//�������� ������ � ������� �� ������� ������� ���������
		bool write_data_vertex(int d)
		{
			VertexT *v;
			try
			{
				v = operator*();
			}
			catch (char*e)
			{
				cerr << endl << e << endl;
				return false;
			}
			v->setData(d);
			return true;
		}

		//�������� ��� ������� �� ������� ������� ���������
		bool write_name_vertex(string str)
		{
			VertexT *v;
			try
			{
				v = operator*();
			}
			catch (char*e) {
				cerr << endl << e << endl;
				return false;
			}
			v->setName(str);
			return true;
		}
	};

	//�������� ����
	class EdgeIterator {
		Graph<VertexT, EdgeT> *graph; //�������������� ����
		typename GraphMatrixForm<EdgeT>::EdgeIterator *mIt;
		typename GraphListForm<EdgeT>::EdgeIterator *lIt;
		bool useM;
	public:
		int curV1, curV2;
		string str1, str2, str3, str4;
		bool end;

		EdgeIterator(Graph<VertexT, EdgeT> &g)
		{
			graph = &g;
			if (graph->Dense())
			{
				mIt = new GraphMatrixForm<EdgeT>::EdgeIterator(static_cast<GraphMatrixForm<EdgeT>*>(g.data));
				useM = true;
			}
			else {
				lIt = new GraphListForm<EdgeT>::EdgeIterator(static_cast<GraphListForm<EdgeT>*>(g.data));
				useM = false;
			}
			//begin();
		}

		//��������� � ������
		bool begin()
		{
			if (useM) return
				mIt->begin();
			else
				return lIt->begin();
		}

		//����� �� ��������� �������
		bool operator++()
		{
			if (useM)
				return mIt->next();
			else
				return lIt->next();
		}

		//�������� �� �����
		bool onEnd()
		{
			if (useM)
				return mIt->onEnd();
			else
				return lIt->onEnd();
		}

		//���������� �������� ���� �� �������� �����
		bool toend()
		{
			if (useM)
				return mIt->toend();
			else
				return lIt->toend();
		}

		//��������� �����
		EdgeT* operator*()
		{
			if (onEnd())
				throw "�������� �� ��������� �����";
			if (useM)
				return mIt->getEdge();
			else
				return lIt->getEdge();
		}

		//��������� ������� ����� � ��� �� ������� ������� ���������
		string read_edge()
		{
			EdgeT* e = operator*();
			VertexT *_v1, *_v2;
			_v1 = e->getVertex1();
			_v2 = e->getVertex2();
			if (!(_v1->isNameSet()))
				str1 = "��� ����� �� �����������";
			else
				str1 = _v1->getName();
			if (!(_v2->isNameSet()))
				str2 = "������ ����� �� �����������";
			else
				str2 = _v2->getName();
			if (e->isWeightSet() == false)
				str3 = "��� ����� �� ����������";
			else
				str3 = int_to_string(e->getWeight());
			str4 = "��������� �������: " + str1 + ". �������� �������: " + str2 + ". ��� �����: " + str3;
			return str4;
		}

		//��������� ������ ����� �� ������� ������� ���������
		int read_data_edge()
		{
			EdgeT* f = operator*();
			if (f->isDataSet())
				return f->getData();
			else
				throw "������ ����� �� �����������";
		}

		bool write_data_edge(int d)
		{
			EdgeT* e;
			try {
				e = operator*();
			}
			catch (char* e) {
				cerr << endl << e << endl;
				return false;
			}
			e->setData(d);
			return true;
		}

		bool write_weight_edge(int w)
		{
			EdgeT* e;
			try {
				e = operator*();
			}
			catch (char* e) {
				cerr << endl << e << endl;
				return false;
			}
			e->setWeight(w);
			return true;
		}
	};
	//�������� ��������� ����
	class OutputEdgeIterator {
		Graph<VertexT, EdgeT> *graph; //�������������� ����
		typename GraphMatrixForm<EdgeT>::OutputEdgeIterator *mIt;
		typename GraphListForm<EdgeT>::OutputEdgeIterator *lIt;
		bool useM;
	public:
		string str1, str2, str3, str4;
		bool end;

		OutputEdgeIterator(Graph<VertexT, EdgeT> &g, VertexT &v) {
			graph = &g;
			mIt = NULL;
			lIt = NULL;
			int curV = graph->getIndex(&v);
			if (graph->Dense()) {
				mIt = new GraphMatrixForm<EdgeT>::OutputEdgeIterator(static_cast<GraphMatrixForm<EdgeT>*>(g.data), curV);
				useM = true;
			}
			else {
				lIt = new GraphListForm<EdgeT>::OutputEdgeIterator(static_cast<GraphListForm<EdgeT>*>(g.data), curV);
				useM = false;
			}
		}

		~OutputEdgeIterator()
		{
			if (mIt != NULL)
				delete mIt;
			if (lIt != NULL)
				delete lIt;
		}

		//��������� � ������
		bool begin() {
			if (useM)
				return mIt->begin();
			else
				return lIt->begin();
		}
		//����� �� ��������� �������
		bool operator++() {
			if (useM)
				return mIt->next();
			else
				return lIt->next();
		}

		bool toend() {
			if (useM)
				return mIt->toend();
			else
				return lIt->toend();
		}

		//�������� �� �����
		bool onEnd() {
			if (useM)
				return mIt->onEnd();
			else
				return lIt->onEnd();
		}

		//��������� �����
		EdgeT* operator*() {
			if (onEnd())
				throw "�������� �� ��������� �����";
			if (useM)
				return mIt->getEdge();
			else
				return lIt->getEdge();
		}

		string read_edge() {
			EdgeT* e = operator*();
			VertexT *_v1, *_v2;
			_v1 = e->getVertex1();
			_v2 = e->getVertex2();
			if (!(_v1->isNameSet()))
				str1 = "��� ����� �� �����������";
			else
				str1 = _v1->getName();
			if (!(_v2->isNameSet()))
				str2 = "������ ����� �� �����������";
			else
				str2 = _v2->getName();
			if (e->isWeightSet() == false)
				str3 = "��� ����� �� ����������";
			else
				str3 = int_to_string(e->getWeight());
			str4 = "��������� �������: " + str1 + ". �������� �������: " + str2 + ". ��� �����: " + str3;
			return str4;
		}

		int read_data_edge() {
			EdgeT* e;
			e = operator*();
			if (e->isDataSet())
				return e->getData();
			else
				throw "������ ����� �� �����������";
		}

		bool write_data_edge(int d) {
			EdgeT* e;
			try {
				e = operator*();
			}
			catch (char* e) {
				return false;
			}
			e->setData(d);
			return true;
		}

		bool write_weight_edge(int w) {
			EdgeT* e;
			try {
				e = operator*();
			}
			catch (char*e) {
				cerr << endl << e << endl;
				return false;
			}
			e->setWeight(w);
			return true;
		}
	};
	friend class VertexIterator;  //������������� �����
	friend class EdgeIterator;
	friend class OutputEdgeIterator;
};

//��������� ������� ������� �� �����������
template <class VertexT, class EdgeT>
int Graph<VertexT, EdgeT>::getIndexFromName(string n)
{
	int index = 0;
	for (int i = 0; i < vertexVector.size(); i++) {
		if (vertexVector[index]->getName() == n)
			break;
		++index;
	}
	if (index == vertexVector.size())
		throw "������� �� �������";
	return index;
}
//������� ������ L-���� � ������� ������ ������ � ����
template <class VertexT, class EdgeT>
Graph<VertexT, EdgeT>::Graph()
{
	sz = 0;
	data = new GraphListForm<EdgeT>(false);
	directed = false;
	dense = false;
	edgeCounter = 0;
}


//������� ���� � V ���������, ��� �����, ���� D, ����� ������������� F
template <class VertexT, class EdgeT>
Graph<VertexT, EdgeT>::Graph(int vertexCount, bool directed, bool dense)
{
	sz = 0;
	if (dense)
		data = new GraphMatrixForm<EdgeT>(directed);
	else
		data = new GraphListForm<EdgeT>(directed);
	//������ ������� � �������� � ���������
	//for (int i = 0; i < vertexCount; i++)
	//data->InsertV(i);
	for (int i = 0; i < vertexCount; i++) {
		VertexT *v = new VertexT();
		v->setName(int_to_string(sz));
		sz++;
		vertexVector.push_back(v);
		data->InsertV(i);
	}
	this->directed = directed;
	this->dense = dense;
	edgeCounter = 0;
}

//������� ���� � V ���������, � E ���������� �������, ���� D , ����� F
template <class VertexT, class EdgeT>
Graph<VertexT, EdgeT>::Graph(int vertexCount, int edgeCount, bool directed, bool dense)
{
	sz = 0;
	if (dense)
		data = new GraphMatrixForm<EdgeT>(directed);
	else
		data = new GraphListForm<EdgeT>(directed);
	//������ ������� � �������� � ���������
	//for (int i = 0; i < vertexCount; i++)
	//data->InsertV(i);
	for (int i = 0; i < vertexCount; i++) {
		VertexT *v = new VertexT();
		v->setName(int_to_string(sz));
		sz++;
		vertexVector.push_back(v);
		data->InsertV(i);
	}
	this->directed = directed;
	this->dense = dense;
	edgeCounter = 0;

	//��������� ����
	if (edgeCount <= 0) return;
	//������������ ����� ����, ����� �� ��������� ���������
	if (directed) {
		if (edgeCount > vertexCount * (vertexCount - 1))
			edgeCount = vertexCount * (vertexCount - 1);
		srand((unsigned)time(NULL));
	}
	else {
		if (edgeCount > (vertexCount * (vertexCount - 1)) / 2)
			edgeCount = (vertexCount * (vertexCount - 1)) / 2;
	}
	//���������
	int v1, v2;

	while (edgeCounter < edgeCount) {
		v1 = rand() % vertexCount;
		v2 = rand() % vertexCount;
		if (v1 == v2)
			continue;
		if (data->hasEdge(v1, v2))
			continue;

		EdgeT *t = new EdgeT(vertexVector[v1], vertexVector[v2]);
		data->InsertE(v1, v2, t);
		t->setWeight((rand() % 9) + 1);
		if (directed == false)
			data->InsertE(v2, v1, t);
		edgeCounter++;
	}
}

//����������� �����������
template <class VertexT, class EdgeT>
Graph<VertexT, EdgeT>::Graph(const Graph<VertexT, EdgeT> &G)
{
	sz = 0;
	delete data;
	if (G.Dense())
		data = new GraphMatrixForm<EdgeT>(*(G.data));
	else
		data = new GraphListForm<EdgeT>(*(G.data));
	directed = G.directed;
	dense = G.dense;
}

//����������
template <class VertexT, class EdgeT>
Graph<VertexT, EdgeT>::~Graph()
{
	while (vertexVector.size() != 0)
		DeleteV(0);
	delete data;
}

//���������� ����� ������ � �����
template <class VertexT, class EdgeT>
int Graph<VertexT, EdgeT>::V()
{
	return vertexVector.size();
}

//���������� ����� ����� � �����
template <class VertexT, class EdgeT>
int Graph<VertexT, EdgeT>::E()
{
	return edgeCounter;
}

//���������� ��� �����
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::Directed()
{
	return directed;
}

//���������� ����� ������������� �����
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::Dense()
{
	return dense;
}
//���������� ����������� ������������ �����
template <class VertexT, class EdgeT>
double Graph<VertexT, EdgeT>::K()
{
	int max = vertexVector.size() * (vertexVector.size() - 1);
	if (!directed) {
		max /= 2;
	}
	if (max == 0) {
		return -1;
	}
	else {
		return (double)edgeCounter / (double)max;
	}
}
//����������� ���� � L-�����
template <class VertexT, class EdgeT>
void Graph<VertexT, EdgeT>::ToListGraph()
{
	//������ ��������� �������� L-�����
	GraphForm<EdgeT> *newData = new GraphListForm<EdgeT>(this->directed);

	//������ �������
	for (int i = 0; i < vertexVector.size(); i++)
		newData->InsertV(i);

	//��������� ����
	for (int i = 0; i < vertexVector.size(); i++)
		for (int j = 0; j < vertexVector.size(); j++)
			if (data->hasEdge(i, j))
				newData->InsertE(i, j, data->getEdge(i, j));
	delete data;
	data = newData;
	dense = false;
}
//����������� ���� � M-�����
template <class VertexT, class EdgeT>
void Graph<VertexT, EdgeT>::ToMatrixGraph()
{
	//������ ��������� �������� �-�����
	GraphForm<EdgeT> *newData = new GraphMatrixForm<EdgeT>(this->directed);

	//������ �������
	for (int i = 0; i < vertexVector.size(); i++)
		newData->InsertV(i);

	//��������� ����
	for (int i = 0; i < vertexVector.size(); i++)
		for (int j = 0; j < vertexVector.size(); j++)
			if (data->hasEdge(i, j))
				newData->InsertE(i, j, data->getEdge(i, j));
	delete data;
	data = newData;
	dense = true;
}

//��������� ������� � ����� � ���������� ����� ����������� ����� ��������� �������
template <class VertexT, class EdgeT>
VertexT* Graph<VertexT, EdgeT>::InsertV()
{
	VertexT *v = new VertexT;
	if (data->InsertV(vertexVector.size()) == false)
		throw 1;
	v->setName(int_to_string(sz));
	sz++;
	vertexVector.push_back(v);
	return v;
}

//������� ������� �� �����, �������� ������� ����������� v
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::DeleteV(VertexT *v)
{
	int index = getIndex(v);
	edgeCounter -= data->DeleteEsFromVertex(index, directed);
	if (data->DeleteV(index))
	{
		vertexVector.erase(vertexVector.begin() + index);//
		return true;
	}
	return false;
}
template <class VertexT, class EdgeT>
VertexT* Graph<VertexT, EdgeT>::getVertex(int index)
{
	if (index < 0 || index >= vertexVector.size())
		throw "������� �� �������";
	return vertexVector[index];
}
template <class VertexT, class EdgeT>
VertexT* Graph<VertexT, EdgeT>::getVertexFromName(string n)
{
	int i;
	for (i = 0; i < vertexVector.size(); i++)
		if (vertexVector[i]->getName() == n)
			return vertexVector[i];
	//if (i == vertexVector.size())
	throw "������� �� �������";
}

template <class VertexT, class EdgeT>
int Graph<VertexT, EdgeT>::getIndex(VertexT *v)
{
	int index = 0;
	for (int i = 0; i < vertexVector.size(); ++i) {
		if (vertexVector[index] == v)
			break;
		index++;
	}
	if (index == vertexVector.size())
		throw "������� �� �������";
	return index;
}

//��������� ����� (v1, v2) � �����, ����������� �������, �������� �������� ������������ v1 � v2	
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::InsertE(string v1, string v2)
{
	VertexT* _v1 = new VertexT;
	VertexT* _v2 = new VertexT;
	try {
		_v1 = getVertexFromName(v1);
	}
	catch (char*)
	{
		return false;
	}
	try
	{
		_v2 = getVertexFromName(v2);
	}
	catch (char*)
	{
		return false;
	}
	try
	{
		InsertE(_v1, _v2);
	}
	catch (char*)
	{
		return false;
	}
	return true;
}
template <class VertexT, class EdgeT>
EdgeT* Graph<VertexT, EdgeT>::InsertE(VertexT *v1, VertexT *v2)
{
	EdgeT *e = new EdgeT(v1, v2);
	if (!data->InsertE(getIndex(v1), getIndex(v2), e)) throw "Exeption#";
	if (directed == false)
		data->InsertE(getIndex(v2), getIndex(v1), e);
	edgeCounter++;
	return e;
}
//������� �����, ����������� �������, �������� �������� ������������ v1 � v2
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::DeleteE(string v1, string v2)
{
	VertexT* _v1 = new VertexT;
	VertexT* _v2 = new VertexT;
	try
	{
		_v1 = getVertexFromName(v1);
	}
	catch (char*e)
	{
		cerr << endl << e << endl;
		return false;
	}
	try
	{
		_v2 = getVertexFromName(v2);
	}
	catch (char*e)
	{
		cerr << endl << e << endl;
		return false;
	}
	if (!DeleteE(_v1, _v2)) return false;
	else return true;
}
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::DeleteE(VertexT *v1, VertexT *v2)
{
	if (data->DeleteE(getIndex(v1), getIndex(v2)))
	{
		edgeCounter--;
		if (directed == false)
			data->DeleteE(getIndex(v2), getIndex(v1));
		return true;
	}
	else
		return false;
}
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::hasEdge(VertexT *v1, VertexT *v2)
{
	int ind1, ind2;
	try {
		ind1 = getIndex(v1);
	}
	catch (int &a) {
		return false;
	}
	try {
		ind2 = getIndex(v2);
	}
	catch (int &a)
	{
		return false;
	}
	return data->hasEdge(getIndex(v1), getIndex(v2));
}
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::hasEdge(int v1, int v2)
{
	if (v1 < 0 || v1 >= vertexVector.size())
		return false;
	if (v2 < 0 || v2 >= vertexVector.size())
		return false;
	return data->hasEdge(v1, v2);
}

template <class VertexT, class EdgeT>
EdgeT* Graph<VertexT, EdgeT>::getEdge(VertexT *v1, VertexT *v2)
{
	EdgeT* e;
	try {
		e = data->getEdge(getIndex(v1), getIndex(v2));
	}
	catch (int) {
		throw "����� �� �������";
	}
	return e;
}
template <class VertexT, class EdgeT> //�������� ������ � �����
bool  Graph<VertexT, EdgeT>::write_data_edge(VertexT * v1, VertexT * v2, int data)
{
	EdgeT* e;

	try {
		e = getEdge(v1, v2);
		e->setData(data);
	}
	catch (char*e) {
		cerr << endl << e << endl;
		return false;
	}
	return true;
}
//�������� ��� �����
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::write_weight_edge(VertexT* v1, VertexT* v2, int w)
{
	EdgeT* e;

	try {
		e = getEdge(v1, v2);
		e->setWeight(w);
	}
	catch (char*e) {
		cerr << endl << e << endl;
		return false;
	}
	return true;
}
//��������� ������ � �����
template <class VertexT, class EdgeT>
int Graph<VertexT, EdgeT>::read_data_edge(VertexT* v1, VertexT* v2)
{
	EdgeT* e;
	try {
		e = getEdge(v1, v2);
	}
	catch (char*e) {
		cerr << endl << e << endl;
		return -1;
	}
	try {
		if (e->isDataSet())
			return e->getData();
		else
			throw "������ ����� �� �����������";
	}
	catch (char*e) {
		cerr << endl << e << endl;
		return -1;
	}
}

//��������� ��� �����
template <class VertexT, class EdgeT>
int Graph<VertexT, EdgeT>::read_weight_edge(VertexT* v1, VertexT* v2)
{
	EdgeT* e;
	try {
		e = getEdge(v1, v2);
	}
	catch (char*e) {
		cerr << endl << e << endl;
		return -1;
	}
	try {
		if (e->isWeightSet())
			return e->getWeight();
		else
			throw "��� ����� �� ����������";
	}
	catch (char*e) {
		cerr << endl << e << endl;
		return -1;
	}
}

//����� �� �����
template <class VertexT, class EdgeT>
void Graph<VertexT, EdgeT>::print_graph()
{
	data->print(vertexVector);

};

//��������� ������ � �������
template <class VertexT, class EdgeT>
int Graph<VertexT, EdgeT>::read_data_vertex(VertexT *v)
{
	int d = v->getData();
	try {
		if (v->isDataSet())
			return d;
		else
			throw invalid_argument("��� ������ ��� ������ �������");
	}
	catch (invalid_argument &e) {
		cerr << endl << e.what() << endl;
		return -1;
	}
}

//�������� ������ � �������
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::write_data_vertex(VertexT *v1, int d)
{
	v1->setData(d);
	return true;
}

//������������� �������
template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::write_name_vertex(VertexT *v1, string str)
{
	v1->setName(str);
	return true;
}

template <class VertexT, class EdgeT>
bool Graph<VertexT, EdgeT>::vertex_has_name(string name)
{
	for (int i = 0; i < vertexVector.size(); i++)
		if (vertexVector[i]->getName() == name)
			return true;
	return false;
}