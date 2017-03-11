/*
Omkar Pradeep Acharya
Student ID: 200155609
Problem Statement: Q6 To develop a program that counts the number of different strings that are substrings of both of two input strings.
*/

#include <iostream>
#include <cstring>
#include <set>

using namespace std;

//Find out all the unique substrings
void find_substrings(string s,set<string>&sub){
    int len = s.length();
    int count = 0;
    for(int i=1;i<len+1;i++){
        for(int j=0;j<len+1-i;j++){
            //Check if the substring is already present in the set or not
            if(!sub.count(s.substr(j,i))){
                count++;
                sub.insert(s.substr(j,i));
            }
        }
    }
}

//Find the common substrings for s1 and s2
int common_count(set<string>&sub1,set<string>&sub2){
    int common_count = 0;
    set<string>::iterator i;
    for(i = sub1.begin();i!=sub1.end();i++){
        if(sub2.count(*i)){
            common_count++;
        }
    }
    return common_count;
}

int main(){
    string s1,s2;
    set<string>sub1,sub2;
    cin>>s1>>s2;

    find_substrings(s1,sub1);
    find_substrings(s2,sub2);

    cout<<common_count(sub1,sub2)<<'\n';
    return 0;
}