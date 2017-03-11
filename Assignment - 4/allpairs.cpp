//============================================================================
// Name        : allpairs.cpp
// Author      : Omkar Acharya
// Problem     : All Pairs Shortest Path
//============================================================================

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <climits>
#include <iomanip>
#include <math.h>
using namespace std;

#define INF 999999999

//data structure representing a word as a vertex
struct node{
	//string as in a word
	string word;
	//unique ID for a vertex
	int index;
};

//class representing a graph with vertices and edges
class Graph{
	//number of vertices of the graph
	int no_of_vertices;
	//container for storing all the vertices in the graph
	vector < node > node_vector;
	//adjacency matrices representing the graph
	//k_matrix stores the k values that achieves the shortest path between each pair of vertices
	vector < vector < int > > adjacencyMatrix, original_adjacency_matrix, k_matrix;
public:
	//initialization of a graph with all the vertices and edges (including weights)
	void make_graph();
	//creates the adjacency matrix using edges, weights and nodes
	void create_adjacency_matrix();
	//returns an edge weight of the edge betweek two words represented as vertices in the graph
	//given that only one alphabet from the same position is different e.g. raM and raN
	int calculate_edge_weight(string, string);
	//prints the adjacency matrix. This is just used for representing the graph as an adjacency matrix
	void print_matrix(vector <vector < int > >&);
	//returns the number of reachable vertices from a particular vertex in the graph
	int calculate_reachable_nodes(int);
	//all pairs shortes path algorithm
	void floyd_warshall_algorithm();
	//answers all the shortest path questions
	void query();
	//returns the unique ID of a vertex representing a particular word
	int get_index(string);
	//gets the path between two vertices
	void get_path(int, int);
};

//creates the adjacency matrix according the vertices and edges
void Graph::make_graph(){
	//gets the number of vertices as an input
	cin >> no_of_vertices;
	//resize the adjacency matrix according to the number of edges
	adjacencyMatrix.resize(no_of_vertices);
	k_matrix.resize(no_of_vertices);
	original_adjacency_matrix.resize(no_of_vertices);
	for (int i = 0; i < no_of_vertices; ++i) {
		adjacencyMatrix[i].resize(no_of_vertices);
		k_matrix[i].resize(no_of_vertices);
		original_adjacency_matrix[i].resize(no_of_vertices);
	}

	//inserting vertices into the node container
	string s;
	for (int i = 0; i < no_of_vertices; ++i) {
		cin >> s;
		node *w = new node;
		w->index = i;
		w->word = s;
		node_vector.push_back(*w);
	}

//	for (int i = 0; i < no_of_vertices; ++i) {
//		cout << node_vector[i].index << ' ' << node_vector[i].word << '\n';
//	}
}

// creates the adjacency matrix with vertices and edge weights
void Graph::create_adjacency_matrix(){
	for (int i = 0; i < no_of_vertices; ++i) {
		for (int j = 0; j < no_of_vertices; ++j) {
			if(i == j){
				adjacencyMatrix[i][j] = 0;
			}
			else{
				adjacencyMatrix[i][j] = calculate_edge_weight(node_vector[i].word, node_vector[j].word);
			}
			original_adjacency_matrix[i][j] = adjacencyMatrix[i][j];
			k_matrix[i][j] = INF;
		}
	}
}

//returns edge weight from a word to another
//given that only one alphabet from the same position is different e.g. raM and raN
int Graph::calculate_edge_weight(string s1, string s2){
	int count, distance;
	//counts the number of different characters between two words at the same position.
	count = 0;
	//initially lets say its infinite
	distance = INF;
	//if both the words have different length then they don't have any edge between them
	if(s1.length() != s2.length())
		return INF;
	int length = s1.length();
	for (int i = 0; i < length; ++i) {
		if (s1[i] != s2[i] && !count) {
			//distance is calculated as the distance between two different alphabets
			distance = abs((int)s1[i] - (int)s2[i]);
			//increment count if they have different alphabets at the same index
			count++;
		} else if (s1[i] != s2[i] && count) {
			//if the count is more then it means that there are more than 1 alphabets which are different
			//returning infinity
			return INF;
		}
	}
//	cout << s1 << " -> " << s2 << " : " << distance << '\n';
	return distance;
}

//prints out the matrix just for the sake of convinience
void Graph::print_matrix(vector < vector < int > > &matrix){
	for (unsigned int i = 0; i < matrix.size(); ++i) {
		for (unsigned int j = 0; j < matrix[i].size(); ++j) {
			if(matrix[i][j] == INF)
				cout << "INF" << "  ";
			else
				cout << matrix[i][j] << "  ";
		}
		cout << '\n';
	}
	cout << "\n\n";
}

//calculates the number of reachable vertices from the given vertex
int Graph::calculate_reachable_nodes(int n){
	int count = 0;
	for (int i = 0; i < no_of_vertices; ++i) {
		//if the edge weight is not infinite then the vertex is reachable
		if (adjacencyMatrix[n][i] != INF)
			count++;
	}
	return count;
}

//all pair shortest path algorithm
void Graph::floyd_warshall_algorithm(){
	for (int k = 0; k < no_of_vertices; ++k) {
		for (int i = 0; i < no_of_vertices; ++i) {
			for (int j = 0; j < no_of_vertices; ++j) {
				if (adjacencyMatrix[i][k] + adjacencyMatrix[k][j] < adjacencyMatrix[i][j]){
					adjacencyMatrix[i][j] = adjacencyMatrix[i][k] + adjacencyMatrix[k][j];
//					cout << "HERE" << '\n';
					k_matrix[i][j] = k;
				}
			}
		}
//		print_matrix(adjacencyMatrix);
	}

//	cout << "Adjacency Matrix:\n";
//	print_matrix(original_adjacency_matrix);
//	cout << "All Pair Shortest Path Matrix:\n";
//	print_matrix(adjacencyMatrix);
//	cout << "K Matrix:\n";
//	print_matrix(k_matrix);
}

//get the shortest path between the two vertices
void Graph::get_path(int a, int b){
//	cout << "\na: " << node_vector[a].word << " b: " << node_vector[b].word << " ";
	int distance = original_adjacency_matrix[a][b];
	//if there is a direct edge then return get that edge
	if (distance && distance != INF){
//		cout << k;
		cout << " " + node_vector[b].word;
		return;
	}
	int k = k_matrix[a][b];
	//it involves two recursive calls
	//1st call is from a to k where k is the shortest you can go to from a, provided that k to b is also the shortest
	//2nd call is from k to b
	if (k != INF) {
		get_path(a, k);
		get_path(k, b);
	}
//	return "";
}

//answers all the shortes path distances between all the pairs which are taken from input
void Graph::query(){
	int count = 0;
	//calculating the total count of reachable vertices for all the nodes
	for (int i = 0; i < no_of_vertices; ++i) {
		count += calculate_reachable_nodes(i);
	}

	//average reachable vertices = total number of reachable vertices / total vertices in the graph
	double x = (double)count / no_of_vertices;
	//rounding upto 2 places after the decimal point
	cout << ceil((x * pow(10, 2)) - 0.49) / pow(10 ,2) << '\n';

	int n;
	cin >> n;
	string a, b;
	int a_index, b_index, distance;
	for (int i = 0; i < n; ++i) {
		//taking two vertices which are source and destination for the shortest path calculation
		cin >> a >> b;
		// a and b would be the words (string). getting their unique indices and then finding the shortest path
		a_index = get_index(a);
		b_index = get_index(b);
		distance = adjacencyMatrix[a_index][b_index];
		// if the distance is infinite then there is no path
		if (distance == INF)
			cout << a << ' ' << b << " not reachable";
		else{
			//if they are reachable then get the path from a to b
			cout << distance << ' ' << a;
			get_path(a_index, b_index);
//			cout << b;
		}
		cout << '\n';
	}
}

//getting unique id for the vertex
int Graph::get_index(string s){
	vector < node >::iterator i;
	for (i = node_vector.begin(); i != node_vector.end(); ++i) {
		if((*i).word == s)
			return (*i).index;
	}
	return -1;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	//instantiation of the graph class
	Graph g;
	g.make_graph();
	g.create_adjacency_matrix();
	g.floyd_warshall_algorithm();
	g.query();
	return 0;
}
