#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <cstdint>
#include <random>

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
    } *head_node;
    uint16_t max_level;
    uint16_t highest_level;

    int16_t Find(const KEY &key, Node *&node, uint16_t *pass) const;

    uint16_t RandomLevel(const double p) const;

public:
    SkipList(const uint16_t max, const KEY &max_key_value);

    int16_t Search(const KEY &key, VALUE &value) const;

    int16_t Insert(const KEY &key, const VALUE &value);
};

template<typename KEY, typename VALUE>
int16_t SkipList<KEY, VALUE>::Find(const KEY &key, Node *&node, uint16_t *pass) const
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
    if (node->forward[i] && node->forward[i]->key == key) // 找到了key
    {
        node = node->forward[i];
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
    std::default_random_engine e;
    std::uniform_real_distribution<double> u(0, 1);
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
    // 构建根节点
    head_node = new Node;
    head_node->level = highest_level;
    head_node->forward = new Node *[max_level];
    for (uint16_t i = 0; i < max_level; ++i)
    {
        head_node->forward[i] = nullptr;
    }
    // 构建最后的节点
    Node *last_node = head_node->forward[0] = new Node;
    last_node->key = max_key_value;
    last_node->level = highest_level;
}

template<typename KEY, typename VALUE>
int16_t SkipList<KEY, VALUE>::Search(const KEY &key, VALUE &value) const
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
int16_t SkipList<KEY, VALUE>::Insert(const KEY &key, const VALUE &value)
{
    Node *node = nullptr;
    ERR_CODE code = Find(key, node);
    if (code == SUCCESS)
    {
        return KEY_ALREADY_EXISTS;
    }
    // 新建节点
    
}

#endif