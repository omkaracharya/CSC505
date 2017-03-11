#include <iostream>
#include <vector>
using namespace std;

int parent(int i){
    return (i-1)/3;
}

void percolate_up(int i,vector<int>&arr){
    int temp;
    if(i==0)
        return;
    if(arr[i] < arr[parent(i)]){
        temp = arr[i];
        arr[i] = arr[parent(i)];
        arr[parent(i)] = temp;
        percolate_up(parent(i),arr);
    }
}

void add(int n,vector<int>&arr){
    arr.push_back(n);
    if(arr.size() != 1)
        percolate_up(arr.size()-1,arr);
}

void min_heapify(int i,vector<int>&arr){
    int i1 = 3*i+1;
    int i2 = 3*i+2;
    int i3 = 3*i+3;
    int smaller,temp,minval;
    if((unsigned)i1 >= arr.size()){
        return;
    }else if((unsigned)i1 < arr.size() && (unsigned)i2 >= arr.size()){
        minval = arr[i1];
        smaller = i1;
    }else if((unsigned)i1 < arr.size() && (unsigned)i2 < arr.size() && (unsigned)i3 >= arr.size()){
        minval = min(arr[i1],arr[i2]);
        if(arr[i1] == minval)
            smaller = i1;
        else if(arr[i2] == minval)
            smaller = i2;
    }else{
        minval = min(min(arr[i1],arr[i2]),arr[i3]);
        if(arr[i1] == minval)
            smaller = i1;
        else if(arr[i2] == minval)
            smaller = i2;
        else if(arr[i3] == minval)
            smaller = i3;
    }
    if(arr[i] > minval){
        temp = arr[i];
        arr[i] = arr[smaller];
        arr[smaller] = temp;
        min_heapify(smaller,arr);
    }
}

int remove(vector<int>&arr){
    int root = arr.front();
    arr.front() = arr[arr.size()-1];
    arr.pop_back();
    min_heapify(0,arr);
    return root;
}

void print_heap(vector<int>&arr){
    for(vector<int>::iterator i=arr.begin();i!=arr.end();i++){
        cout<<*i<<" ";
    }
    cout<<'\n';
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n;
    vector<int>arr;
    while(1){
        string c;
        cin>>c;
        if(c=="add"){
            cin>>n;
            add(n,arr);
        }else if(c=="remove"){
            cout<<remove(arr);
            cout<<'\n';
        }else{
            break;
        }
        //h.print_heap();
    }
    return 0;
}
