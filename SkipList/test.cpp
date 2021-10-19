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
Result PerformenceTest(unsigned long long num, SkipList<unsigned long long, string> &list, unsigned long long *keys, string *values, bool rand_shuffle = true, default_random_engine *e = nullptr);
bool CorrectionTest(unsigned long long num, SkipList<unsigned long long, string> &list, unsigned long long *keys, string *values);
bool is_in(unsigned long long key, unsigned long long *keys, unsigned long long len)
{
    for (unsigned long long i = 0; i < len; ++i)
    {
        if (key == keys[i])
        {
            return true;
        }
    }
    return false;
}

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
    // 生成随机字符串作为数据
    string *values = new string[number];
    for (unsigned long long i = 0; i < number; ++i)
    {
        values[i] = RandomString(10);
    }
    if (type == '0')
    {
        // 生成顺序关键字
        for (unsigned long long i = 0; i < number; ++i)
        {
            keys[i] = i;
        }
        skip_time = PerformenceTest(number, skip_list, keys, values, false);
        normal_time = PerformenceTest(number, list, keys, values, false);
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
        // 生成随机关键字
        default_random_engine e{static_cast<long unsigned int>(time(0))};
        uniform_int_distribution<unsigned long long> u(0, number);
        for (unsigned long long i = 0; i < number; ++i)
        {
            bool exist = false;
            unsigned long long new_key = u(e);
            while (is_in(new_key, keys, i))
            {
                new_key = u(e);
            }
            keys[i] = new_key;
        }
        default_random_engine e2{static_cast<long unsigned int>(time(0) + 1)};
        Result skip_time = PerformenceTest(number, skip_list, keys, values, true, &e2);
        // Result normal_time = PerformenceTest(number, list, keys, values, true, &e2);
        cout << "跳表耗时：\t" << endl;
        cout << "\t插入：" << skip_time.insert << endl;
        cout << "\t查询：" << skip_time.insert << endl;
        cout << "\t删除：" << skip_time.insert << endl;

        // cout << "普通链表耗时：\t" << endl;
        // cout << "\t插入：" << normal_time.insert << endl;
        // cout << "\t查询：" << normal_time.insert << endl;
        // cout << "\t删除：" << normal_time.insert << endl;
    }
    else if (type == '2')
    {
        default_random_engine e{static_cast<long unsigned int>(time(0))};
        uniform_int_distribution<unsigned long long> u(0, number);
        cout << "正在生成随机键" << endl;
        for (unsigned long long i = 0; i < number; ++i)
        {
            bool exist = true;
            unsigned long long new_key = u(e);
            while (is_in(new_key, keys, i))
            {
                new_key = u(e);
            }
            
            keys[i] = new_key;
        }

        for (unsigned long long i = 0; i < number; ++i)
        {
            cout << keys[i] << "\t: " << values[i] << endl;
        }
        cout << "开始正确性检查" << endl;
        if (CorrectionTest(number, skip_list, keys, values))
        {
            cout << "检查完毕" << endl;
        }
    }

    delete[]keys;
    delete[]values;
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
    if (rand_shuffle && e)
    {
        cout << "正在打乱数组" << endl;
        shuffle(keys, keys + num, *e);
    }
    cout << "开始查找测试" << endl;
    start = chrono::high_resolution_clock::now();
    for (unsigned long long i = 0; i < num; ++i)
    {
        string value;
        ERR_CODE rst = list.Search(keys[i], value);
    }
    end = chrono::high_resolution_clock::now();
    rst.query = (end - start).count();

    // 顺序键删除测试
    if (rand_shuffle && e)
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

bool CorrectionTest(unsigned long long num, SkipList<unsigned long long, string> &list, unsigned long long *keys, string *values)
{
    // 先插入所有元素
    for (unsigned long long i = 0; i < num; ++i)
    {
        list.Insert(keys[i], values[i]);
    }

    // 查询所有元素，查看正确性
    default_random_engine e{static_cast<long unsigned int>(time(0))};
    // shuffle(keys, keys + num, e);
    // shuffle(values, values + num, e);
    for (unsigned long long i = 0; i < num; ++i)
    {
        string str;
        list.Search(keys[i], str);
        if (str != values[i])
        {
            cout << "1: 出错！ i = " << i << endl;
            cout << "key\t= " << keys[i] << endl;
            cout << "str\t= " << str << endl;
            cout << "values[i]\t= " << values[i] << endl;
            return false;
        }
    }

    // 删除一部分元素，重复实验
    unsigned long long to_delete = num / 3;
    unsigned long long remain = num - to_delete;
    // shuffle(keys, keys + remain, e);
    // shuffle(values, values + remain, e);
    for (unsigned long long i = remain; i < num; ++i)
    {
        list.Remove(keys[i]);
    }

    list.PrintAll();

    for (unsigned long long i = 0; i < remain; ++i)
    {
        string str;
        list.Search(keys[i], str);
        if (str != values[i])
        {
            cout << "2: 出错！ i = " << i << endl;
            cout << "key\t= " << keys[i] << endl;
            cout << "str\t= " << str << endl;
            cout << "values[i]\t= " << values[i] << endl;
            return false;
        }
    }
    return true;
}