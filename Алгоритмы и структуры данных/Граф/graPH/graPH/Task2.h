
//14) определение центра взвешенного орграфа на основе алгоритма Флойда

#include<algorithm>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include <stack>

template <class VertexT, class EdgeT>
class CentreFloyd {
	int **mtr;
	int **matrix;
	int n, m, v;
	int INF;
	vector<int> centre;
	Graph<VertexT, EdgeT> *graph;

	void search();
	bool floyd();
public:
	typename Graph<VertexT, EdgeT>::OutputEdgeIterator *out;

	CentreFloyd() {
		INF = 1000000000;
	}
	CentreFloyd(const Graph<VertexT, EdgeT> &T) {
		graph = T.graph;
	}
	~CentreFloyd() {
		delete mtr;
	}
	bool set(Graph<VertexT, EdgeT> *g);
	bool restart();
	vector<int> result();
};



template<class VertexT, class EdgeT>
inline void CentreFloyd<VertexT, EdgeT>::search()
{
	int sz = graph->V();
	int vv1, vv2, w;
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
			stringstream stream3(out->str3);
			stream3 >> w;
			if (u == vv2) {
				vv2 = vv1;
			}
			matrix[u][vv2] = w;

			try {
				++*out;
			}
			catch (char *) {
				break;
			}
		}
	}
}

template<class VertexT, class EdgeT>
inline bool CentreFloyd<VertexT, EdgeT>::floyd()
{
	n = m = graph->V();
	matrix = new int*[n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new int[n];
		for (int j = 0; j < n; j++) {
			if (i == j)
				matrix[i][j] = 0;
			else
				matrix[i][j] = INF;
		}
	}

	search();

	for (int k = 0; k < n; ++k)
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				if (matrix[i][k] < INF && matrix[k][j] < INF)
					matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);


	vector<int> ex(n, 0);
	int minEx = INF;

	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < n; ++i) {
			if (matrix[i][j] > ex[j])
				ex[j] = matrix[i][j];
		}
		if (minEx > ex[j]) {
			minEx = ex[j];
			centre.clear();
			centre.push_back(j);
			continue;
		}
		if (minEx = ex[j]) { //check it!
			centre.push_back(j);
		}
	}
	if (!centre.empty())
		return true;
	return false;
}

template<class VertexT, class EdgeT>
inline bool CentreFloyd<VertexT, EdgeT>::set(Graph<VertexT, EdgeT>* g)
{
	graph = g;
	return floyd();
}

template<class VertexT, class EdgeT>
inline bool CentreFloyd<VertexT, EdgeT>::restart()
{
	return floyd();
}

template<class VertexT, class EdgeT>
inline vector<int> CentreFloyd<VertexT, EdgeT>::result()
{
	return centre;
}
