#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <cstdint>
#include <random>
#include <iostream>

enum ERR_CODE { SUCCESS = 1, NO_SUCH_NODE = 0, KEY_ALREADY_EXISTS = -1 };

/*
    使用键值对的方式表示数据，KEY类型必须是一种可比较的类型，并且可赋值（支持或重载了=运算符）
*/
template<typename KEY, typename VALUE>
class SkipList
{
private:
    struct Node
    {
        KEY key;
        VALUE value;
        uint16_t level;
        Node **forward;
    } *head_node, *last_node;
    uint16_t max_level;
    uint16_t highest_level;
    KEY max_key;
    double p = 0.5;

    ERR_CODE Find(const KEY &key, Node *&node, Node **pass) const;

    uint16_t RandomLevel(const double p) const;

public:
    SkipList(const uint16_t max, const KEY &max_key_value);

    ~SkipList();

    ERR_CODE Search(const KEY &key, VALUE &value) const;

    ERR_CODE Insert(const KEY &key, const VALUE &value);

    ERR_CODE Remove(const KEY &key);

    // For debug
    void PrintAll();

    void Clear();
};

template<typename KEY, typename VALUE>
ERR_CODE SkipList<KEY, VALUE>::Find(const KEY &key, Node *&node, Node **pass) const
{
    node = head_node;
    for (int32_t i = highest_level; i >= 0; --i)
    {
        while (node->forward[i] != nullptr && node->forward[i]->key < key)
        {
            node = node->forward[i];
        }
        if (pass)
        {
            pass[i] = node;
        }
    }
    if (node->forward[0] && node->forward[0]->key == key) // 找到了key
    {
        node = node->forward[0];
        return SUCCESS;
    }
    else // 没有找到
    {
        return NO_SUCH_NODE;
    }
}

template<typename KEY, typename VALUE>
uint16_t SkipList<KEY, VALUE>::RandomLevel(const double p) const
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<double> u(0, 1);
    uint16_t level = 0;
    while (u(e) < p && level <= max_level)
    {
        ++level;
    }
    return level;
}

template<typename KEY, typename VALUE>
SkipList<KEY, VALUE>::SkipList(const uint16_t max, const KEY &max_key_value)
{
    // 初始化相关信息
    max_level = max;
    highest_level = 0;
    max_key = max_key_value;
    // 构建根节点
    head_node = new Node;
    head_node->level = highest_level;
    head_node->forward = new Node *[max_level];
    for (uint16_t i = 0; i < max_level; ++i)
    {
        head_node->forward[i] = nullptr;
    }
    // 构建最后的节点
    last_node = head_node->forward[0] = new Node;
    last_node->key = max_key;
    last_node->level = highest_level;
}

template<typename KEY, typename VALUE>
SkipList<KEY, VALUE>::~SkipList()
{
    Clear();
}

template<typename KEY, typename VALUE>
ERR_CODE SkipList<KEY, VALUE>::Search(const KEY &key, VALUE &value) const
{
    Node *node = nullptr;
    ERR_CODE code = Find(key, node, nullptr);
    if (code == SUCCESS && node)
    {
        value = node->value;
        return SUCCESS;
    }
    else
    {
        return NO_SUCH_NODE;
    }
}

template<typename KEY, typename VALUE>
ERR_CODE SkipList<KEY, VALUE>::Insert(const KEY &key, const VALUE &value)
{
    // 找到插入位置
    Node *node = nullptr;
    Node **update = new Node *[max_level];
    for (uint16_t i = 0; i < max_level; ++i)
    {
        update[i] = head_node;
    }
    ERR_CODE code = Find(key, node, update);
    if (code == SUCCESS)
    {
        return KEY_ALREADY_EXISTS;
    }

    // 新建节点
    Node *new_node = new Node;
    new_node->key = key;
    new_node->value = value;
    new_node->level = RandomLevel(p);
    new_node->forward = new Node *[max_level];

    if (new_node->level > highest_level)
    {
        for (uint16_t i = highest_level + 1; i <= new_node->level; ++i)
        {
            head_node->forward[i] = last_node;
        }
        highest_level = new_node->level;
        head_node->level = highest_level;
    }

    // 更新旧的forward列表
    for (uint16_t i = 0; i <= new_node->level; ++i)
    {
        Node *tmp = update[i]->forward[i];
        update[i]->forward[i] = new_node;
        new_node->forward[i] = tmp;
    }

    // 删除临时空间
    delete[]update;
    return SUCCESS;
}

template<typename KEY, typename VALUE>
ERR_CODE SkipList<KEY, VALUE>::Remove(const KEY &key)
{
    Node *to_delete = nullptr;
    Node **update = new Node *[highest_level + 1];
    for (uint16_t i = 0; i < max_level; ++i)
    {
        update[i] = nullptr;
    }
    ERR_CODE code = Find(key, to_delete, update);
    if (code == NO_SUCH_NODE)
    {
        return code;
    }
    // 更新指针
    for (int32_t i = highest_level; i >= 0; --i)
    {
        update[i]->forward[i] = to_delete->forward[i];
    }
    // 检测是否为唯一的最高层节点
    if (to_delete->level == highest_level)
    {
        // 找到下一个高度的节点
        uint16_t new_highest = highest_level;
        while (new_highest >= 0 &&
               head_node->forward[new_highest] == last_node)
        {
            --new_highest;
        }
        head_node->level = highest_level = new_highest;
    }
    // 删除节点
    delete[](to_delete->forward);
    delete to_delete;
    return SUCCESS;
}

template<typename KEY, typename VALUE>
void SkipList<KEY, VALUE>::Clear()
{
    Node *node = head_node;
    while (node && node->key != max_key)
    {
        Node *tmp = node->forward[0];
        delete[](node->forward);
        delete node;
        node = tmp;
    }
    delete[](node->forward);
    delete node;
}

template<typename KEY, typename VALUE>
void SkipList<KEY, VALUE>::PrintAll()
{
    Node *node = head_node;
    while (node && node->key != max_key)
    {
        // std::cout << node->key << ": " << node->value << "\tlevel = " << node->level << std::endl;
        std::cout << "节点地址：" << node << std::endl;
        std::cout << "{" << std::endl;
        std::cout << "\tkey     = " << node->key << std::endl;
        std::cout << "\tvalue   = " << node->value << std::endl;
        std::cout << "\tlevel   = " << node->level << std::endl;
        std::cout << "\tforward = " << std::endl;
        for (int32_t level = node->level; level >= 0; --level)
        {
            std::cout << "\t\tlevel: " << level << " 指向：" << node->forward[level] << std::endl;
        }
        std::cout << "}" << std::endl << std::endl;
        node = node->forward[0];
    }
    std::cout << "末端节点地址：" << node << std::endl;
}

#endif