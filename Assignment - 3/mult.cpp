//Q.1
//Program - Matrix Multiplication using Dynamic Programming
//Author name - Omkar Pradeep Acharya
//Student ID - 200155609

#include <iostream>
#include <vector>
using namespace std;

//number of matrices to be multiplied
int n;

//the storage of k values where k is the split where the matrices to be multiplied first. (or where to put the bracket)
int kk[1000][1000];

//prints the bracket around the matrices
void print_brackets(vector<pair<int, int> > &brackets){
    //subtract one from the opening bracket count of the first matrix as the problem asks not to print the opening bracket around the entire expression
    brackets[0].first--;
    //and the same goes for the closing bracket count of the last matrix
    brackets[n].second--;

    //prints both the brackets around the matrix and renames the matrix as M_i e.g. M1, M2, etc
    for(int i = 0; i <= n; ++i){
        for(int j = 0 ; j < brackets[i].second; ++j)
            cout << " )";
        if(i != 0 && i != n)
            cout << " * " ;
        for(int j = 0 ; j < brackets[i].first; ++j)
            cout << "( "; 
        if(i != n)
            cout << "M" << i + 1 ;
    }
}

//prints the actual splits between the matrices i.e. where exactly to put the brackets first
void printk(int i, int j, vector<pair<int, int> > &brackets){
    if(i == j){
        return;
    } else{
        //increment the bracket count if you have got the k value around a particular matrix
        brackets[i].first += 1;
        brackets[j + 1].second += 1;
        //printing brackets around matrices
        printk(i, kk[i][j], brackets);
        printk(kk[i][j] + 1, j, brackets);
    }
}

//matrix multiplication function
void matrix_multiply(int p[], vector<pair<int, int> > &brackets){
    //dynamic programming table for storing the cost of multiplying matrices
    int T[n][n];

    //initially all the costs would be zero
    for(int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j) {
            T[i][j] = 0;
            kk[i][j] = i;
        }
    }

    //dynamic programming
    //this function is gonna take O(n^3) as each cell is taking O(n) to iterate over all the possible values of k
    //and there are n^2 such cells.
    //space requirement is O(n^2) as this is T[n][n] table of cells.
    for (int l = 2; l <= n; ++l) {
        for (int i = 0; i < n - l + 1; ++i) {
            int j = i + l - 1;
            //finding the minimum value of k for which the cost of matrix multiplication is least
            if(i < j) {
                int mink = i;
                int min = T[i][i] + T[i+1][j] + p[i] * p[i + 1] * p[j + 1];
                for (int k = i + 1; k < j; ++k) {
                    if(min > T[i][k] + T[k + 1][j] + p[i] * p[k + 1] * p[j + 1]){
                        min = T[i][k] + T[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                        mink = k;
                    }
                }
                //storing the minimum cost and particular value of k in the table
                T[i][j] = min;
                kk[i][j] = mink;
            }
        }
    }
    //print the output
    printk(0, n - 1, brackets);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); 
    //gets the number of matrices to be multiplied
    cin >> n;
    //creates the array for p values (heights of matrices)
    int p[n+1];
    //vector storing brackets around the matrices
    vector<pair<int, int> > brackets;
    for (int i = 0; i <= n; ++i) {
        //gets the p values from standard i/o
        cin >> p[i];
        //creates a pair of opening and closing bracket count around a particular matrix.
        //initially this would be 0,0 as there are no brackets around the matrix.
        brackets.push_back(pair<int,int>(0, 0));
    }
    //calling multiplication function
    matrix_multiply(p, brackets);
    //printing the brackets around matrices
    print_brackets(brackets);
    cout << endl;
    return 0;
}