//Q.5
//Program - Huffman code generation
//Author name - Omkar Pradeep Acharya
//Student ID - 200155609

#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

std::vector<int> st;
//storing huffman codes of all the characters
std::vector<string> code(257);
int first_time = 0;

//a character is represented as an individual node in the min_heap
struct node{
	//frequency of a character
	int frequency;
	//which character is it
	char c;
	//unique ID
	int num;
	//left and right links
	node *left;
	node *right;
public:
	node(){
		left = right = NULL;
	}
};

//finds the parent nodes index
int parent(int i){
    return (i-1)/2;
}

//percolates up the newly added value
void percolate_up(int i, vector <node> &heap_vec){
    node temp;
    if(i==0)
        return;
    //check whether a parent has min value than child. if not then swap.
    if(heap_vec[i].frequency < heap_vec[parent(i)].frequency){
        temp = heap_vec[i];
        heap_vec[i] = heap_vec[parent(i)];
        heap_vec[parent(i)] = temp;
        percolate_up(parent(i), heap_vec);
    }
}

//adds a new node to the heap
void add(node &temp, vector <node> &heap_vec){
	//add node to heap
    heap_vec.push_back(temp);
    //check the constraint of heap structure
    if(heap_vec.size() != 1)
        percolate_up(heap_vec.size()-1, heap_vec);
}

//restores the heap property which says the parent's value must be minimum than its children
void min_heapify(int i, vector <node> &heap_vec){
	//get the child nodes of a particular node
    int i1 = 2*i+1;
    int i2 = 2*i+2;
    int smaller, minval;
	node temp;
	//check which is child has the least value for checking whether the constraint is violated
    if((unsigned)i1 >= heap_vec.size()){
        return;
    }else if((unsigned)i1 < heap_vec.size() && (unsigned)i2 >= heap_vec.size()){
        minval = heap_vec[i1].frequency;
        smaller = i1;
    }else{
        minval = min(heap_vec[i1].frequency, heap_vec[i2].frequency);
        if(heap_vec[i1].frequency == minval)
            smaller = i1;
        else if(heap_vec[i2].frequency == minval)
            smaller = i2;
    }
    //swap the parent node if its value is maximum than that of its child
    if(heap_vec[i].frequency > minval){
        temp = heap_vec[i];
        heap_vec[i] = heap_vec[smaller];
        heap_vec[smaller] = temp;
        min_heapify(smaller, heap_vec);
    }
}

//removes a node from heap
node* remove(vector<node>&heap_vec){
	node *root = new node;
	//take the front node off the heap
    *root = heap_vec.front();
    //make the last node the root node of the heap
    heap_vec.front() = heap_vec[heap_vec.size()-1];
    //pop it from the back
    heap_vec.pop_back();
    //min heapify for the structure constraints
    min_heapify(0, heap_vec);
    //return the extracted root node
    return root;
}

//creates a heap of characters as nodes
void make_heap(vector<node>&arr, vector<node>&heap_vec, vector<int>&freq){
	char ch = 0;
	int counter = 0;
	//adding the node in the heap along with the frequency
	for(vector <node>::iterator it = arr.begin(); it != arr.end(); it++){
		if(counter != 256){
			(*it).c = ch;
			ch++;			
		}
		(*it).num = counter;
		//assign the frequency to the node
		(*it).frequency = freq[counter++];
		//add node to the heap
		add((*it), heap_vec); 
	}
	//removing 2 minimum frequency nodes and merging them together, and then adding back the merged node to the heap
	while(heap_vec.size() > 1){
		//remove 2 mim freq nodes
		node *x = remove(heap_vec);
	 	node *y = remove(heap_vec);
	 	//make a new parent node
	 	node *z = new node;
	 	//assign it as the parent of other 2 nodes
		z->left = x;
		z->right = y;
		//add the frequency of children
		z->frequency = x->frequency + y->frequency;
		//add it to the heap
		add(*z, heap_vec);
	}
}

//prints the heap
void print_heap(vector<node>&heap_vec){
    for(vector<node>::iterator i = heap_vec.begin(); i != heap_vec.end(); i++){
        cout << (*i).frequency <<"\n";
    }
    cout<<'\n';
}

//encodes the huffman code for each of the character by traversing through the heap.
void traverse_tree(node *root, char side){
	if(root == NULL){
		st.pop_back();
		return;
	}

	if(root->left == NULL && root->right == NULL){
		string s = "";
		//append 0's and 1's to create the huffman code of a particular node
		for(vector<int>::iterator it = st.begin(); it != st.end(); it++)
			 s += (*it) + '0';
		if(root->num != 256){
			//add the code to the vector of codes
			if(root->c - 0 < 0)
				code[root->c +256] = s;
			else
				code[root->c - 0] = s;
		}else{
			//code for EOF
			code[256] = s;
		}
	}
	//0 for the left child branch
	st.push_back(0);
	traverse_tree(root->left, 'l');
	// 1 for the right child branch
	st.push_back(1);
	traverse_tree(root->right, 'r');
	st.pop_back();
}

//prints the huffman codes
void print_codes(vector <string> &code){
	char c = 0;
	int ii = 0;
	for (std::vector<string>::iterator it = code.begin(); it != code.end(); ++it){
		if(ii != 256){
			if(ii < 33 || ii > 126)
				cout << " " << uppercase << setfill('0') << hex << setw(2) << (int)(c & 0xff) << dec;
			else
				cout << "  " << c;
			c++;
			ii++;
			cout << " " << (*it) << '\n';
		}else{
			cout << "EOF " << (*it) << '\n';
		}
	}

}

//don't need this function, but it calculates the height of the tree. this will tell the maximum length of huffman code
int height(node* nod) {
    if (!nod) return -1;
    return 1 + max(height(nod->left), height(nod->right));
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	char c;
	//storing character frequency
	std::vector<int> freq(257,1);
	while(cin >> noskipws >> c){
		if(c - 0 < 0)
			freq[c + 256]++;
		else
			freq[c - 0]++; 
	}
	//EOF at the end
	freq[256] = 1;
	//storage for all the nodes (characters) in the heap
	vector <node> arr(257);
	//heap data structure
	vector <node> heap_vec;
	//creating heap for all the characters as nodes
	make_heap(arr, heap_vec, freq);

	//creating huffman codes
	node *root = new node;
	*root = heap_vec.front();
	first_time = 1;
	traverse_tree(root, 'l');

	//printing huffman codes
	print_codes(code);
	return 0;
}