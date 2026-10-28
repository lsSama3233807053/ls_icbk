#include <iostream>
#include <string>
using namespace std;

class Student
{
private:
    string name;
    string id;
    int* scores;
    int numscores;
public:
    // 普通构造函数
    Student(string n, string i, int* s, int count)
    {
        name = n;
        id = i;
        numscores = count;
        scores = new int[numscores];
        for (int i = 0; i < numscores; i++)
        {
            scores[i] = s[i];
        }
    }

    // 深拷贝构造函数
    Student(const Student& other)
    {
        cout << "深拷贝构造函数调用" << endl;
        name = other.name;
        id = other.id;
        numscores = other.numscores;
        scores = new int[numscores];
        for (int j = 0; j < numscores; j++)
        {
            scores[j] = other.scores[j];
        }
    }

    // 析构函数
    ~Student()
    {
        cout << "析构函数的调用" << endl;
        delete[] scores;
    }

    // 打印信息
    void print()
    {
        cout << "姓名:" << name << " 学号:" << id << endl;
        cout << "成绩:";
        for (int i = 0; i < numscores; i++)
        {
            cout << scores[i] << " ";
        }
        cout << endl;
    }
};

int main()
{
    int arr[] = { 85,90,78 };
    Student stu1("张三", "2025001", arr, 3);
    stu1.print();
    Student s2 = stu1;
    s2.print();
    return 0;
}