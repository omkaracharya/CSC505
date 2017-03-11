#include <iostream>
#include <string>
#include <sys/time.h>
#include <fstream>

using namespace std;

//finds the running time
long get_milliseconds() {
    timeval tv;
    gettimeofday(&tv, NULL);
    long int ms = tv.tv_sec;
    ms = ms * 1000 + tv.tv_usec / 1000;
    return ms;
}

//Generates the string and pattern to show the better performace of KMP
void default_string_generator(string &s1, string &s2) {
//  Details are mentioned in the report pdf for the assignment
    for (int i = 0; i <= 100000; ++i)
        s1 += 'a';
    for (int i = 0; i < 80000; ++i)
        s2 += 'a';
    s1 += 'b';
    s2 += 'b';
}

//Naive string matching algorithm of O(mn)
int naive(string s1, string s2) {
    int len1 = s1.length();
    int len2 = s2.length();
    int i, j, k;
    //Shift by character by character considering each of the character of the string and pattern
    for (i = 0; i < len1; ++i) {
        k = i;
        for (j = 0; j < len2; ++j) {
            if (s1[k] != s2[j])
                break;
            k++;
        }
        if (j == len2)
            return i;
    }
    return -1;
}

//KMP Algorithm
int kmp(string s1, string s2) {
    int len1 = s1.length();
    int len2 = s2.length();

//  Computing the prefix table
    int pi[len2];
    pi[0] = 0;

    int k = 0;
    for (int i = 1; i < len2; ++i) {
        while (k > 0 && s2[k] != s2[i])
            k = pi[k - 1];
        if (s2[k] == s2[i])
            k++;
        pi[i] = k;
    }

//  KMP Algorithm
    int q = 0;
    for (int i = 0; i < len1; ++i) {
        while (q > 0 && s2[q] != s1[i]) {
            q = pi[q - 1];
        }
        if (s2[q] == s1[i])
            q++;
        if (q == len2) {
            return i - len2 + 1;
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    string s1, s2;
    if (argc > 1) {
//      If the filename is given
        ifstream fi(argv[1]);
//      Open the whatever filename is given and read the first two strings
        if (fi.is_open()) {
            getline(fi, s1);
            getline(fi, s2);
            fi.close();
        }else{
            cerr << "Please check the filename" << endl;
            return  0;
        }
    } else {
//      Default case where filename is not provided
        s1 = "";
        s2 = "";
//      Calling the string generator to generate a large string
        default_string_generator(s1, s2);
    }

//  NAIVE COMPUTATION
    int found;
    long t0 = get_milliseconds();
    found = naive(s1, s2);
    long t1 = get_milliseconds();
    if (found != -1) {
        cout << "found at: " << found << '\n';
        cout << "naive search time: " << t1 - t0 << '\n';
    } else
        cout << found << '\n';

//  BUILD IN STRING MATCHING FUNCTION
    size_t flag;
    t0 = get_milliseconds();
    flag = s1.find(s2);
    t1 = get_milliseconds();
    if (flag != string::npos) {
        cout << "found at: " << flag << '\n';
        cout << "standard search time: " << t1 - t0 << '\n';
    } else
        cout << -1 << '\n';

//  KMP COMPUTATION
    t0 = get_milliseconds();
    found = kmp(s1, s2);
    t1 = get_milliseconds();
    if (found != -1) {
        cout << "found at: " << found << '\n';
        cout << "kmp search time: " << t1 - t0 << '\n';
    } else
        cout << found << '\n';

    return 0;
}
