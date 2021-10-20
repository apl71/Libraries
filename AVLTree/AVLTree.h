#ifndef AVLTREE_H_
#define AVLTREE_H_

enum ERR_CODE { SUCCESS = 1 };

template<typename KEY, typename VALUE>
class AVLTree
{
private:
    struct Node
    {
        KEY key;
        VALUE value;
        Node *left;
        Node *right;
    } *root;

public:
    AVLTree();


};

template<typename KEY, typename VALUE>
AVLTree<KEY, VALUE>::AVLTree()
{
    root = new Node;
}

#endif