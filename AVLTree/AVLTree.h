#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <cstdint>

enum ERR_CODE { SUCCESS = 1, NO_SUCH_NODE = 0, NO_LEFT_CHILD = -1 };

template<typename KEY, typename VALUE>
class AVLTree
{
private:
    struct Node
    {
        KEY key;
        VALUE value;
        uint16_t height;
        Node *left;
        Node *right;
    } *root = nullptr;

    ERR_CODE Find(const KEY &key, Node *&node, Node **pass) const;

    int32_t GetHeight(Node *node) const;

    int32_t GetBalanceFactor(Node *node) const;

    // 右旋，node必须要有左节点
    ERR_CODE RightRotate(Node *node, Node *&new_root);

public:
    AVLTree();

    ERR_CODE Insert(const KEY &key, const VALUE &value);
};

template<typename KEY, typename VALUE>
ERR_CODE AVLTree<KEY, VALUE>::Find(const KEY &key, Node *&node, Node **pass) const
{
    node = root;
    Node *previous;
    while (node)
    {
        previous = node;
        if (node->key == key)
        {
            return SUCCESS;
        }
        else if (node->key < key)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
    node = previous;
    return NO_SUCH_NODE;
}

template<typename KEY, typename VALUE>
int32_t AVLTree<KEY, VALUE>::GetHeight(Node *node) const
{
    if (!node)
    {
        return -1;
    }
    else
    {
        return node->height;
    }
}

template<typename KEY, typename VALUE>
int32_t AVLTree<KEY, VALUE>::GetBalanceFactor(Node *node) const
{
    if (!node)
    {
        return 0;
    }
    return GetHeight(node->left) - GetHeight(node->right);
}

template<typename KEY, typename VALUE>
ERR_CODE AVLTree<KEY, VALUE>::RightRotate(Node *node, Node *&new_root)
{
    if (!node || !node->left)
    {
        new_root = nullptr;
        return NO_LEFT_CHILD;
    }

    Node *left_node = node->left;
    node->left = left_node->right;
    left_node->right = node;
    new_root = left_node;
}

template<typename KEY, typename VALUE>
AVLTree<KEY, VALUE>::AVLTree()
{
    root = nullptr;
}

template<typename KEY, typename VALUE>
ERR_CODE AVLTree<KEY, VALUE>::Insert(const KEY &key, const VALUE &value)
{
    if (!root) // 空树，直接新建节点即可
    {
        root = new Node{key, value, 0, nullptr, nullptr};
        return SUCCESS;
    }
    else
    {

    }
}

#endif