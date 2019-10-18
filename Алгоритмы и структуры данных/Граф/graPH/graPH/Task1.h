
// 15) определение эйлерова цикла в неориентированном графе

#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include <stack>


template <class VertexT, class EdgeT>
class EulerCycle {
	int INF;
	int **matrix;
	vector<int> cycle;
	Graph<VertexT, EdgeT> *graph;
	void search_matrix();
	void search_euler(int v);
	bool euler_run();

public:
	typename Graph<VertexT, EdgeT>::OutputEdgeIterator *out;
	EulerCycle() {}
	EulerCycle(const Graph<VertexT, EdgeT> &T) {
		graph = T.graph;
	}
	~EulerCycle() {}

	bool set(Graph<VertexT, EdgeT> *gg);
	bool restart();
	vector<int> result();
};

template<class VertexT, class EdgeT>
inline bool EulerCycle<VertexT, EdgeT>::set(Graph<VertexT, EdgeT>* gg)
{
	graph = gg;
	if (graph->Directed()) {
		return false;
	}

	if (euler_run()) 
		return true;
	else 
		return false;
}

template<class VertexT, class EdgeT>
inline bool EulerCycle<VertexT, EdgeT>::restart()
{
	if (graph == NULL)
		return false; 

	if (graph->Directed()) {
		return false;
	}

	if (euler_run())
		return true;
	else
		return false;
}

template<class VertexT, class EdgeT>
inline vector<int> EulerCycle<VertexT, EdgeT>::result()
{
	if (!cycle.empty())
		return cycle;
	else {
		cout << "Задача не решена";
		return cycle;
	}
}

//поиск смежных вершин
template<class VertexT, class EdgeT>
inline void EulerCycle<VertexT, EdgeT>::search_matrix()
{
	int sz = graph->V();
	int vv1, vv2;

	for (int u = 0; u < sz; u++) {
		try {
			out = new Graph<VertexT, EdgeT>::OutputEdgeIterator(*graph, *graph->getVertex(u));
		}
		catch (char *) {
			continue;
		}
		if (!out->begin()) {
			continue;
		}

		for (int j = 0; j < sz; j++) {
			try {
				out->read_edge();
			}
			catch (char *) {
				break;
			}
			stringstream stream1(out->str1);
			stream1 >> vv1;
			stringstream stream2(out->str2);
			stream2 >> vv2;
			if (u == vv2) {
				vv2 = vv1;
			}
			//u - нулевая (исходящая), vv2 - первая (входящая), 1 - ребро существует
			matrix[u][vv2] = 1;

			try {
				++*out;
			}
			catch (char *) {
				break;
			}
		}
	}
}

//находим эйлеров (цикл, проходящий по всем рёбрам только один раз)
template<class VertexT, class EdgeT>
inline void EulerCycle<VertexT, EdgeT>::search_euler(int v)
{
	int ind;
	int n = graph->V();
	for (int i = 0; i < n; ++i) {
		if (matrix[v][i] != 0) {		// если ребро есть проходим по нему					
			matrix[v][i] = 0;
			matrix[i][v] = 0;
			search_euler(i);		//вызывается рекурсия для второй вершины
		}
	}
	cycle.push_back(v);
}


//цикл Эйлера
template<class VertexT, class EdgeT>
inline bool EulerCycle<VertexT, EdgeT>::euler_run()
{
	int n = graph->V(); //число вершин в графе
	//изначально ребра равны 0 (не существуют) (как бы нули)
	matrix = new int*[n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new int[n];
		for (int j = 0; j < n; j++) {
			matrix[i][j] = 0;
		}
	}
	cycle.clear();
	search_matrix();

	vector<int> degree(n, 0); //n нулевых элементов

	//вычисляем степени вершин
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (matrix[i][j] != 0) {
				++degree[j];
			}
		}
	}
	//проверяем существует ли в данном графе эйлеров цикл
	int count = 0; // количество степеней не кратных двум			 
	int j = 0; // начальная вершина
	for (int i = 0; i < n; ++i) {
		if (degree[i] % 2 != 0)
			return false;
	}
	//находим цикл, проходящий по всем рёбрам только один раз
	search_euler(j);
}

