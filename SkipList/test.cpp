#include <iostream>
#include <string>
#include <climits>
#include <random>
#include "SkipList.h"

using namespace std;

string RandomString(int length)
{
    string str;
    static default_random_engine e;
    static uniform_int_distribution<int> u(33, 126);
    for (int i = 0; i < length; ++i)
    {
        str.push_back(char(u(e)));
    }
    return str;
}

int main()
{
    SkipList<unsigned long long, string> skip_list(16, ULLONG_MAX);

    // 顺序键插入测试
    // skip_list.PrintAll();
    // cout << endl;
    // cout << "========================================" << endl;
    // cout << endl;
    for (unsigned long long key = 1; key <= 5; ++key)
    {
        string value = RandomString(10);
        skip_list.Insert(key, value);
        cout << "插入元素：[" << "key = " << key << ", value = " << value << "]" << endl;
        // skip_list.PrintAll();
        // cout << endl;
        // cout << "========================================" << endl;
        // cout << endl;
    }

    // 顺序键查找测试
    for (unsigned long long key = 1; key <= 5; ++key)
    {
        string value;
        ERR_CODE rst = skip_list.Search(key, value);
        if (rst == SUCCESS)
        {
            cout << "成功查找元素：key = " << key << ", value = " << value << endl;
        }
        else
        {
            cout << "查找元素失败：key = " << key << endl;
        }
    }

    // 顺序键删除测试
    skip_list.PrintAll();
    cout << endl;
    cout << "========================================" << endl;
    cout << endl;
    for (unsigned long long key = 1; key <= 5; ++key)
    {
        ERR_CODE rst = skip_list.Remove(key);
        if (rst == SUCCESS)
        {
            cout << "成功删除元素，key = " << key << endl;
        }
        else
        {
            cout << "删除元素出错，key = " << key << endl;
        }
        skip_list.PrintAll();
        cout << endl;
        cout << "========================================" << endl;
        cout << endl;
    }

    return 0;
}