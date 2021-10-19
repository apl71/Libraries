#include <iostream>
#include <string>
#include <climits>
#include <random>
#include <chrono>
#include <algorithm>
#include "SkipList.h"

using namespace std;

struct Result
{
    double insert;
    double query;
    double remove;
};

string RandomString(int length);
Result PerformenceTest(unsigned long long num, SkipList<unsigned long long, string> &list, unsigned long long *keys, string *values, bool rand_shuffle = true, default_random_engine e = nullptr);

int main()
{
    cout << "测试数据量：" << endl;
    unsigned long long number;
    cin >> number;
    SkipList<unsigned long long, string> skip_list(16, ULLONG_MAX);
    SkipList<unsigned long long, string> list(0, ULLONG_MAX);
    cout << "输入测试类型：" << endl;
    cout << "[0] 顺序键插入、查询、删除测试" << endl;
    cout << "[1] 随机键插入、查询、删除测试" << endl;
    cout << "[2] DEBUG:正确性测试" << endl;
    char type;
    cin >> type;
    Result skip_time, normal_time;
    unsigned long long *keys = new unsigned long long[number];
    string *values = new string[number];
    for (unsigned long long i = 0; i < number; ++i)
    {
        values[i] = RandomString(10);
    }
    if (type == '0')
    {
        skip_time = PerformenceTest(number, skip_list);
        normal_time = PerformenceTest(number, list);
        cout << "跳表耗时：\t" << endl;
        cout << "\t插入：" << skip_time.insert << endl;
        cout << "\t查询：" << skip_time.insert << endl;
        cout << "\t删除：" << skip_time.insert << endl;

        cout << "普通链表耗时：\t" << endl;
        cout << "\t插入：" << normal_time.insert << endl;
        cout << "\t查询：" << normal_time.insert << endl;
        cout << "\t删除：" << normal_time.insert << endl;
    }
    else if (type == '1')
    {
        Result skip_time = RandomTest(number, skip_list);
        Result normal_time = RandomTest(number, list);
        cout << "跳表耗时：\t" << endl;
        cout << "\t插入：" << skip_time.insert << endl;
        cout << "\t查询：" << skip_time.insert << endl;
        cout << "\t删除：" << skip_time.insert << endl;

        cout << "普通链表耗时：\t" << endl;
        cout << "\t插入：" << normal_time.insert << endl;
        cout << "\t查询：" << normal_time.insert << endl;
        cout << "\t删除：" << normal_time.insert << endl;
    }

    return 0;
}

string RandomString(int length)
{
    string str;
    static default_random_engine e{static_cast<long unsigned int>(time(0))};
    static uniform_int_distribution<int> u(33, 126);
    for (int i = 0; i < length; ++i)
    {
        str.push_back(char(u(e)));
    }
    return str;
}

Result PerformenceTest(unsigned long long num, SkipList<unsigned long long, string> &list, unsigned long long *keys, string *values, bool rand_shuffle, default_random_engine *e)
{
    Result rst;

    // 顺序键插入测试
    cout << "开始插入测试" << endl;
    auto start = chrono::high_resolution_clock::now();
    for (unsigned long long i = 0; i < num; ++i)
    {
        list.Insert(keys[i], values[i]);
    }
    auto end = chrono::high_resolution_clock::now();
    rst.insert = (end - start).count();

    // 顺序键查找测试
    if (rand_shuffle)
    {
        cout << "正在打乱数组" << endl;
        shuffle(keys, keys + num, *e);
    }
    cout << "开始查找测试" << endl;
    start = chrono::high_resolution_clock::now();
    for (unsigned long long i = 1; i < num; ++i)
    {
        string value;
        ERR_CODE rst = list.Search(keys[i], value);
    }
    end = chrono::high_resolution_clock::now();
    rst.query = (end - start).count();

    // 顺序键删除测试
    if (rand_shuffle)
    {
        cout << "正在打乱数组" << endl;
        shuffle(keys, keys + num, *e);
    }
    cout << "开始删除测试" << endl;
    start = chrono::high_resolution_clock::now();
    for (unsigned long long key = 1; key <= num; ++key)
    {
        ERR_CODE rst = list.Remove(key);
    }
    end = chrono::high_resolution_clock::now();
    rst.remove = (end - start).count();

    cout << "测试结束" << endl;

    return rst;
}