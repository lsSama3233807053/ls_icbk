#include <iostream>
#include<windows.h>
#include<algorithm>
#include<vector>
#include<set>
using namespace std;
int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	vector<int>v;
	int n, num;
	cin >> n;
	for (int i = 0;i < n;i++) {
		cin >> num;
		v.push_back(num);
	}
	//排序
	//sort可以将一段范围的数据重新排好，升序排列
	sort(v.begin(), v.begin());
	//输出排序结果
	cout << "排序结果为:" << endl;
	//auto自动识别数据类型，改为int也行
	for (auto x : v) {
		cout << x;
	}
	cout << endl;
	set<int>s(v.begin(), v.end());
	cout << "去重结果为:" << endl;
	for (auto x : s) {
		cout << x;
	}
	return 0;
}