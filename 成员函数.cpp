#include <iostream>
#include<cstdlib>
using namespace std;

class MyClass
{
public:
    void fun(int arr[], int len)
    {
        //查找最小值下标
        int minIndex = 0;
        for (int i = 1; i < len; i++)
        {
            if (arr[i] < arr[minIndex])
            {
                minIndex = i;
            }
        }
        //最小值 和数组最后一个元素交换
        int temp = arr[minIndex];
        arr[minIndex] = arr[len - 1];
        arr[len - 1] = temp;
    }
};

int main()
{
    system("chcp 65001");
    //数组一共6个元素
    int n = 6;
    int arr[100];

    //读取6个数字：5 34 1 2 56 4
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    cout << "交换前：";
    for (int i = 0; i < n; i++)
    {
        if (i > 0) cout << ",";
        cout << arr[i];
    }
    cout << endl;

    MyClass obj;
    obj.fun(arr, n);
    cout << "交换后：";
    for (int i = 0; i < n; i++)
    {
        if (i > 0) cout << ",";
        cout << arr[i];
    }
    cout << endl;

    return 0;
}