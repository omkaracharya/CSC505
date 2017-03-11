//============================================================================
// Name        : maxflow.cpp
// Author      : Omkar Acharya
// Problem	   : Edmands Karp (Max flow)
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

//defining infinity
#define INF 999999999

//class representing a graph with vertices and edges
class Graph{
	//number of edes and number of vertices
	int no_of_vertices, no_of_edges;
	//source and sink node
	int source, sink;
	//adjacency matrix to store the graph as vertices and edges
	vector < vector < int > > adjacency_matrix;
	//temporarily storing the path
	vector < int > path;
//	stack < int > path;
	//useful for BFS algorithm
	vector < int > color, distance, predecessor, input_a, input_b;
//	color: 0 - white, 1 - gray, 2 - black
public:
	Graph(){
		no_of_edges = 2;
		no_of_vertices = 0;
		source = 0;
		sink = 1;
	}
	void create_graph();
	void print_graph(vector < vector < int > > &);
	int breadth_first_search();
	void edmonds_karp();
};

//initiates the graph by taking number of edges and vertices as the input
void Graph::create_graph(){
	int a, b, w;
	cin >> no_of_vertices >> no_of_edges;
	//resizing the adjacency matrix according to the number of vertices
	adjacency_matrix.resize(no_of_vertices);
	color.resize(no_of_vertices);
	distance.resize(no_of_vertices);
	predecessor.resize(no_of_vertices);
//	path.resize(no_of_vertices);
	for (int i = 0; i < no_of_vertices; ++i) {
		adjacency_matrix[i].resize(no_of_vertices, INF);
	}

	//getting all the edge weights
	for (int i = 0; i < no_of_edges; ++i) {
		cin >> a >> b >> w;
		input_a.push_back(a);
		input_b.push_back(b);
		adjacency_matrix[a][b] = w;
		//add the reverse edges
		adjacency_matrix[b][a] = 0;
	}
//	print_graph(adjacency_matrix);
}

//prints the graph in form of adjacency matrix
void Graph::print_graph(vector < vector < int > > &matrix){
	for (int i = 0; i < no_of_vertices; ++i) {
		for (int j = 0; j < no_of_vertices; ++j) {
			cout << matrix[i][j] << "  ";
		}
		cout << '\n';
	}
	cout << '\n';
}

//BFS algorithm
int Graph::breadth_first_search(){
	//initially color all the vertices to white and set their predecessor and cost to unknown
	for (int i = 0; i < no_of_vertices; ++i) {
		color[i] = 0;
		predecessor[i] = -1;
		distance[i] = -1;
	}

	//color the root node gray and its distance as 0
	color[0] = 1;
	predecessor[0] = -1;
	distance[0] = 0;

	//push it to the queue to expand its all the successor nodes
	queue < int > q;
	q.push(0);

	int u, min = INF, flag = 0, current = sink;

	while(!q.empty()){
		//take out the first node out of the queue and expand its all the successor nodes
		u = q.front();
		q.pop();
		for (int i = 0; i < no_of_vertices; ++i) {
			if(adjacency_matrix[u][i] != INF && adjacency_matrix[u][i] != 0){
				//if the color is white then add the distance and its predecessor
				if(color[i] == 0){
					//color it gray
					color[i] = 1;
					//add the distance
					distance[i] = adjacency_matrix[u][i];
					//add its predecessor
					predecessor[i] = u;
					//push it to the queue
					q.push(i);
					//if its sink then indicate that this path contains the sink node
					if(i == sink){
						flag = 1;
					}
				}
			}
		}
		//after all the successors are expanded then mark it black
		color[u] = 2;
	}

	//get the BFS path for the maximum flow algorithm
	path.push_back(current);

	//if the sink is found in the path above then
	if(flag){
		while(1){
			//get the minimum edge out of the all edges in the path
			if(min > distance[current] && distance[current] != -1)
				min = distance[current];
			path.push_back(predecessor[current]);
			current = predecessor[current];
			if(current == source)
				break;
		}
	}

//	vector < int >::iterator it;
//	for (it = path.begin(); it != path.end(); ++it) {
//		cout << *it << ' ';
//	}
//	cout << '\n';
	return min;
}

//max flow algorithm
void Graph::edmonds_karp(){
	//initially the flow is zero
	int flow = 0;
	//call the BFS to find the edge with the minimum edge
	int min = breadth_first_search();
	vector < int >::iterator it;
	while (min != INF){
		//add that min to the flow
		flow += min;
		//add the min to the existing edges in the path
		//subtract the min from the reverse edges
		for (unsigned int i = 0; i < path.size() - 1; ++i) {
			adjacency_matrix[path[i + 1]][path[i]] -= min;
			adjacency_matrix[path[i]][path[i + 1]] += min;
		}
		path.clear();
//		print_graph(adjacency_matrix);
		//do another BFS
		min = breadth_first_search();
	}
	cout << flow << '\n';
	for (unsigned int i = 0; i < input_a.size(); ++i) {
		cout << input_a[i] << ' ' << input_b[i] << ' ' << adjacency_matrix[input_b[i]][input_a[i]] << '\n';
	}
//	print_graph(adjacency_matrix);
}

int main() {
	Graph g;
	g.create_graph();
	g.edmonds_karp();
	return 0;
}
