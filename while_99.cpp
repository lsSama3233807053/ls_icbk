#include<iostream>
#include<cstdlib>
using namespace std;
int main(){
	int i=0;
	while (i<9) {
		i++;
		int j = 0;
		while (j<i){
			j++;
			cout<<j<<"*"<<i<<"="<<j*i<<"  ";
		}
		cout<<endl;
		}
	system("pause");
	return 0;
}
