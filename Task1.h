#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <set>
#include "Graph.h"

using namespace std;

template<class Vertex, class EdgeT>
class Task2 {
public:
	~Task2() {};

	// template<class Vertex, class EdgeT>
	Task2(Graph<Vertex, EdgeT>* g) {
		Set(g);
	}

	Task2(const Graph<Vertex, EdgeT>& G)
	{
		if (G.CheckDense())
			this->data = new GraphMatrixForm<EdgeT>(*(G.data));
		else
			this->data = new GraphListForm<EdgeT>(*(G.data));
		this->directed = G.directed;
		this->dense = G.dense;
	}

	//template<class Vertex, class EdgeT>
	void Set(Graph<Vertex, EdgeT>* g) {
		 // if (!g->Directed()) cout << " Wrong Graph " << endl;
		this->g1 = g;
	}

	void dfs(int node) {
		cur_stack.push_back(node);
		was[node] = true;
		times[node] = up[node] = cur_time++;
		for (unsigned i = 0; i < g1->VertexSize(); ++i) {
			if ((i != node) && (g1->hasEdge(node, i))) {
				if (!was[i]) {
					dfs(i);
					up[node] = min(up[node], up[i]);
					continue;
				}
				for (unsigned j = 0; j < cur_stack.size(); ++j) {
					if (cur_stack[j] == i) {
						up[node] = min(up[node], up[i]);
						break;
					}
				}
			}
		}
		if (up[node] == times[node]) {
			while (cur_stack[cur_stack.size() - 1] != node) {
				comps[cur_stack[cur_stack.size() - 1]] = comp_num;
				cur_stack.pop_back();
			}
			comps[cur_stack[cur_stack.size() - 1]] = comp_num;
			cur_stack.pop_back();
			comp_num++;
		}
	}

	void dfs2(int node) {
		was[node] = true;
		for (unsigned i = 0; i < g1->VertexSize(); ++i)
		{
			if ((i != node) && (g1->hasEdge(node, i)))
			{
			if (comps[node] != comps[i])
			{
				ribs_set.insert(make_pair(comps[node], comps[i]));	
			}
			if (!was[i])
			{
				dfs2(i);
			}
			}
		}
	}

	void Restart(){
		Result();
	}

	template<class TVertex, class TEdge>
	Graph<Vertex,EdgeT>* Result()
	{
		int size = g1->VertexSize();
		was = vector<bool>(size);
		times = vector<int>(size);
		comps = vector<int>(size);
		up = vector<int>(size);

		for (unsigned i = 0; i < size; ++i) {
			if (!was[i]) {
				dfs(i);
				if (!cur_stack.empty()) {
					while (cur_stack.size() != 0) {
						comps[cur_stack[cur_stack.size() - 1]] = comp_num;
						cur_stack.pop_back();
					}
					comp_num++;
				}
			}
		}

		fill(was.begin(), was.end(), false);
		for (unsigned i = 0; i < size; ++i) {
			if (!was[i]) {
				dfs2(i);
			}
		}
		// comp_num-1 компонент сильной связности - количество вершин 
		// ribs_set - пары рёбер. 
		Graph<Vertex, EdgeT>* graphy = new Graph<Vertex,EdgeT>(comp_num - 1, true, true);
		for (set <pair<int, int>> ::iterator go = ribs_set.begin(); go != ribs_set.end(); ++go) {
			graphy -> InsertEdge(to_string(go->first - 1), to_string(go->second - 1));
		}
		return graphy;
	}
private:
	vector<bool> was;
	vector<int> times;
	vector<int> cur_stack;
	vector<int> comps;
	vector<int> up;
	set<pair<int, int>> ribs_set;
	Graph<Vertex, EdgeT>* g1;
	int num_ribs = 0;
	int cur_time = 0;
	int comp_num = 1;
};