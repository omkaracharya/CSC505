//Q.4
//Program - Strongly connected components
//Author name - Omkar Pradeep Acharya
//Student ID - 200155609

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>
using namespace std;

//structure representing the vertex of the graph
struct node{
	//assigning a unique ID to each vertex
	int number;
	//name from the input for the particular vertex
	string name;
	//the order in which the vertex is coming as an input as we need to display the output as per the input
	int position;
	//0 - white, 1 - gray, 2 - black
	int color;
	//constructor
	node(){
		name = " ";
		color = 0;
	}
};

//class representing the entire graph
class Graph{
	int no_of_vertices;
	int no_of_edges;
	//vector of vertices in the graph
	vector <node> graph;
	//vector of strongly connected components in the graph
	vector <int> components;
	//stores the output vector of connected components
	vector < vector <int> > output_vector;
	//vector for storing the names of strongly connected components
	vector <string> names;
	//stores the vertices after doing topological sorting
	stack <int> topological_order;
	vector <int> topo;
	int dependency_number;
	int counter;
	map <string, int> graph_map;
	map <string, int> sorted_graph_map;
	map <string, int>::iterator it1;
	map <string, int>::iterator it2;
	map <string, int>::iterator it3;
	map <string, int>::iterator it4;
	vector <node>::iterator it;
	//actual adjacency matrices representing graph
	bool **adjMatrix, **adjMatrix2, **sortedMatrix;
public:
	Graph(){
		dependency_number = 0;
		counter = 0;
		//all the iterators required for iterating over the vectors declared above
		it = graph.begin();
		it1 = graph_map.begin();
		it2 = graph_map.begin();
		it3 = sorted_graph_map.begin();
		it4 = sorted_graph_map.begin();
	}
	void createGraph();
	void topologicalSort(int);
	void print();
	void setWhite();
	void recursiveTopological(node&, int);
	node& getNode(int);
	node& getNodeByName(string);
	node& getNodeByPosition(int);
	void reverseEdges();
};

//creating graph by taking number of vertices and all the edges
void Graph::createGraph(){
	cin >> no_of_vertices;
	string s;
	for(int i = 0; i < no_of_vertices; ++i){
		cin >> s;
		//creating a new vertex as per the input
		node *p = new node();
		p->number = i;
		p->name = s;
		//inserting into graph the vertex and its number and name
		graph_map.insert(make_pair(s, i));
		graph.push_back(*p);
		names.push_back(s);
	}

	//sorting the names alphabetically
	sort(names.begin(), names.end());

	//i will be using the vertices in alphabetical order henceforth, that's why i am taking all the care of sorting the vertices and storing them
	vector <string>::iterator no = names.begin();
	for(int i = 0; i < no_of_vertices; ++i){
		node &temp = getNodeByName(*no);
		temp.position = i;
		sorted_graph_map.insert(make_pair(*no, i));
		no++;
	}

	//initializing the adjacency matrix
	adjMatrix = new bool*[no_of_vertices];
	for(int i = 0; i < no_of_vertices; ++i)
		adjMatrix[i] = new bool[no_of_vertices];

	adjMatrix2 = new bool*[no_of_vertices];
	for(int i = 0; i < no_of_vertices; ++i)
		adjMatrix2[i] = new bool[no_of_vertices];

	//initializing the matrix with alphabetically sorted vertices
	sortedMatrix = new bool*[no_of_vertices];
	for(int i = 0; i < no_of_vertices; ++i)
		sortedMatrix[i] = new bool[no_of_vertices];

	//creating adjacency matrix using vertices and edges
	cin >> no_of_edges;
	for (int i = 0; i < no_of_vertices; ++i){
		for (int j = 0; j < no_of_vertices; ++j){
			adjMatrix[i][j] = false;
		}
	}

	//inserting edges into adjacency matrix and the other similar matrix which is sorted for alphabetical ordered vertices
	string s1, s2;
	for(int i = 0; i < no_of_edges; ++i){
		cin >> s1 >> s2;
		it1 = graph_map.find(s1);
		it2 = graph_map.find(s2);
		it3 = sorted_graph_map.find(s1);
		it4 = sorted_graph_map.find(s2);
		adjMatrix[it1->second][it2->second] = true;
		sortedMatrix[it3->second][it4->second] = true;
	}
}

//prints the adjacency matrix
void Graph::print(){
	for (int i = 0; i < no_of_vertices; ++i){
		for (int j = 0; j < no_of_vertices; ++j){
			cout << sortedMatrix[i][j] << "  ";
		}
		cout << '\n';
	}
}

//sorting the vertices by name
bool sortByName(const node &a, const node &b){
	return a.name > b.name;
}

//topological sort function
void Graph::topologicalSort(int t){
	//setting all the vertices to white i.e. unvisited so far
	setWhite();

	//if you're calling the topological sort function first time(before reversing the edges)
	if(t == 1){
		it = graph.begin();
		for(int i = 0; i < (int)graph.size(); ++i){
			//if the vertex is not visited yet (i.e. white)
			if((*it).color == 0){
				//calling topological sorting recursively
				recursiveTopological((*it), t);
			}
			it = graph.begin();
			it += i + 1;
		}		

		//topological_order is just a stack of numbers representing uniqueID for each vertices
		//we need actual names of the components so,
		//topo is a vector containing the vertices with names in topological order

		while(!topological_order.empty()){
			topo.push_back(topological_order.top());
			topological_order.pop();
		}
	}

	//or if you're calling this second time(after reversing the edges)
	else if(t == 2){
		vector <int>::iterator to = topo.begin();
		for(int i = 0; i < (int)topo.size(); ++i){
			node &temp = getNode(*to);
			if(temp.color == 0){
				//topological sort again (recursive call)
				recursiveTopological(temp, t);
				//we don't need a single component, so getting rid of them
				//dependency_number is number of dependent components
				if(dependency_number > 1){
					//we need them to be in the order in which they came, so sorting according the the order in which they came
					sort(components.begin(), components.end());
					//storing in the final output vector
					output_vector.push_back(components);
					//clearing vector for the next components
					components.clear();
				}
			}
			to = topo.begin();
			to += i + 1;
			dependency_number = 0;
		}
		//finally sorting the output vector to rearrage the strongly connected components just to show the first come first show basis
		sort(output_vector.begin(), output_vector.end());
		//printing the strongly connected components
		vector < vector <int> >::iterator i1;
		vector <int>::iterator i2;
		for(i1 = output_vector.begin(); i1 != output_vector.end(); ++i1){
			for(i2 = i1->begin(); i2 != i1->end(); ++i2){
				if(i2 != i1->begin())
					cout << " ";
				cout << getNode(*i2).name;
			}
			cout << '\n';
		}
	}

}

//recursive topological sort function
void Graph::recursiveTopological(node &current, int t){
	//color the vertex gray
	current.color = 1;
	int i = current.position;
	//do recursive topological sorting before reversing the edges
	if(t == 1){
		for(int j = 0; j < no_of_vertices; ++j){
			//for all of its neighboring vertices (i.e. for all those vertices having a direct edge i -> j), do topological sort
			if(sortedMatrix[i][j] == 1){
				node &temp = getNodeByPosition(j);
				//if it is white
				if(temp.color == 0){
					recursiveTopological(temp, t);
				}
			}
		}
		//make it black
		current.color = 2;
		//store it into the stack
		topological_order.push(current.number);
	}else if(t == 2){
		//increment the dependency component number as they are found
		dependency_number++;
		for(int j = 0; j < no_of_vertices; ++j){
			//for all of its neighboring vertices (i.e. for all those vertices having a direct edge i -> j), do topological sort
			if(adjMatrix2[i][j] == 1){
				node &temp = getNodeByPosition(j);
				//if it is white
				if(temp.color == 0){
					recursiveTopological(temp, t);
				}
			}
		}
		//make it black
		current.color = 2;
		//store it into the vector of components
		if(dependency_number > 1)
			components.push_back(current.number);
	}
}

//set all the vertices to white
void Graph::setWhite(){
	for(it = graph.begin(); it != graph.end(); ++it)
		(*it).color = 0;
}

//returns a node given its ID
node& Graph::getNode(int n){
	for(it = graph.begin(); it != graph.end(); ++it){
		if((*it).number == n)
			return (*it);
	}	
	node *temp = new node();
	return *temp;
}

//returns a node given its name
node& Graph::getNodeByName(string s){
	for(it = graph.begin(); it != graph.end(); ++it){
		if((*it).name == s)
			return (*it);
	}	
	node *temp = new node();
	return *temp;
}

//returns a node given its position in the sequence of input vertices
node& Graph::getNodeByPosition(int n){
	for(it = graph.begin(); it != graph.end(); ++it){
		if((*it).position == n)
			return (*it);
	}	
	node *temp = new node();
	return *temp;
}

//reverses all the edges
void Graph::reverseEdges(){
	for(int i = 0; i < no_of_vertices; ++i){
		for (int j = 0; j < no_of_vertices; ++j){
			if(sortedMatrix[i][j] == 1){
				adjMatrix2[j][i] = 1;
			}
		}
	}
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	//object of class Graph
	Graph g;

	//create a graph
	g.createGraph();

	//first topological sort
	g.topologicalSort(1);

	//reverse all the edges
	g.reverseEdges();

	//second topological sort
	g.topologicalSort(2);

	return 0;
}