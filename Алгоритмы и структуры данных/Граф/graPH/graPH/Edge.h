#pragma once
//���������, OK

//��� "���������� ����� �����"

template <class VertexT, class WeightT, class DataT>
class Edge {
	VertexT *v1, *v2;   //�������, ������� ��������� �����
	WeightT weight;     //���
	DataT data;         //������ �����
	bool weightIsSet, dataIsSet;//�������� ��������� ���� � ������
public:
	Edge(VertexT *v1, VertexT *v2); //����� � ��������������� ����� � �������
	Edge(VertexT *v1, VertexT *v2, WeightT weight); //����� � ������������� �����
	Edge(VertexT *v1, VertexT *v2, WeightT weight, DataT data);//����� � �������������� ����� � �������
	void setWeight(WeightT weight);	//��������� ����
	void setData(DataT data);		//��������� ������
	WeightT getWeight();			//��������� ����
	DataT getData();				//��������� ������
	bool isWeightSet();			//��������������� ����
	bool isDataSet();			//��������������� ������
	VertexT* getVertex1(); //��������� ����������� ���. �������
	VertexT* getVertex2(); //��������� ����������� ��. �������
};

//������������
template <class VertexT, class WeightT, class DataT>
Edge<VertexT, WeightT, DataT>::Edge(VertexT *v1, VertexT *v2) :
v1(v1),
v2(v2),
weightIsSet(false),
weight(0),
dataIsSet(false) {}

template <class VertexT, class WeightT, class DataT>
Edge<VertexT, WeightT, DataT>::Edge(VertexT *v1, VertexT *v2, WeightT weight) :
v1(v1),
v2(v2),
weight(weight),
weightIsSet(true),
dataIsSet(false) {}

template <class VertexT, class WeightT, class DataT>
Edge<VertexT, WeightT, DataT>::Edge(VertexT *v1, VertexT *v2, WeightT weight, DataT data) :
v1(v1),
v2(v2),
weight(weight),
data(data),
weightIsSet(true),
dataIsSet(true) {}

//��������� � ��������� ���� � ������
template <class VertexT, class WeightT, class DataT>
void Edge<VertexT, WeightT, DataT>::setWeight(WeightT weight)
{
	this->weight = weight;
	weightIsSet = true;
}

template <class VertexT, class WeightT, class DataT>
void Edge<VertexT, WeightT, DataT>::setData(DataT data)
{
	this->data = data;
	dataIsSet = true;
}

template <class VertexT, class WeightT, class DataT>
WeightT Edge<VertexT, WeightT, DataT>::getWeight()
{
	return weight;
}

template <class VertexT, class WeightT, class DataT>
DataT Edge<VertexT, WeightT, DataT>::getData()
{
	return data;
}

template <class VertexT, class WeightT, class DataT>
VertexT* Edge<VertexT, WeightT, DataT>::getVertex1()
{
	return v1;
}

template <class VertexT, class WeightT, class DataT>
VertexT* Edge<VertexT, WeightT, DataT>::getVertex2()
{
	return v2;
}

template <class VertexT, class WeightT, class DataT>
bool Edge<VertexT, WeightT, DataT>::isWeightSet()
{
	return weightIsSet;
}

template <class VertexT, class WeightT, class DataT>
bool Edge<VertexT, WeightT, DataT>::isDataSet()
{
	return dataIsSet;
}

