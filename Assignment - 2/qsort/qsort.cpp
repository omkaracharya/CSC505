#include <iostream>
#include <vector>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

int partition(vector<int>&v, int p, int r){
    int i = p - 1;
    for(int j = p; j < r; j++){
        if(v[j] <= v[r]){
            i++;
            int temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
    int temp = v[i+1];
    v[i+1] = v[r];
    v[r] = temp;
    return i+1;
}

void quick_sort(vector<int>&v, int p, int r, int k){
    if(p < r && r - p + 1 <= k){
        for( int i = 0 ; i < r - p ; i++ ){
            for( int j = 0 ; j < r - p - i ; j++ ){
                if( v[p + j] > v[p + j + 1] ){
                    int temp = v[p + j];
                    v[p + j] = v[p + j + 1];
                    v[p + j + 1] = temp;                    
                }
            }            
        }
    }else if(p < r){
        int q = partition(v,p,r);
        quick_sort(v,p,q-1,k);
        quick_sort(v,q+1,r,k);
    }
}

long get_milliseconds(){
    timeval tv;
    gettimeofday(&tv,NULL);
    long int ms = tv.tv_sec;
    ms = ms * 1000 + tv.tv_usec / 1000;
    return ms;
}

int main(int argc, char *argv[]){
    if(argc < 2)
        return 0;
    vector<int>v;
    int n;
    while(cin>>n){
        v.push_back(n);
    }
    long t0 = get_milliseconds();
    quick_sort(v,0,v.size()-1,atoi(argv[1]));
    long t1 = get_milliseconds();
    cerr<<t1-t0<<'\n';
    for(vector<int>::iterator i=v.begin();i!=v.end();i++)
        cout<<*i<<'\n';
    return 0;
}
