#include<iostream>
#include<vector>
#include<sstream>
#include<string>
using namespace std;

int main() {
    vector<string> v;
    string read;

    cout << "请输入一行英文句子：";
    getline(cin, read);

    stringstream ss(read);
    string word;
    while (ss >> word) {
        v.push_back(word);
    }

    cout << "单词个数：" << v.size() << endl;
    cout << "逆序输出：";
    for(int i = v.size() - 1; i >= 0; i--){
        if(i != v.size() - 1) 
            cout << " ";
        cout << v[i];
    }
    cout << endl;

    return 0;
}