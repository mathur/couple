#include <algorithm>
#include <iostream>

/* Couple node */
template <class T>
class CoupleNode {
public:
    T key;
    int balance;
    CoupleNode *left, *right, *parent;

    CoupleNode(T k, CoupleNode *p) : key(k), balance(0), parent(p),
                        left(NULL), right(NULL) {}

    ~CoupleNode() {
        delete left;
        delete right;
    }
};

/* Couple tree */
template <class T>
class CoupleTree {
public:
    CoupleTree(void);
    ~CoupleTree(void);
    bool insert(T key);
    void deleteKey(const T key);
    void printBalance();

private:
    CoupleNode<T> *root;

    CoupleNode<T>* rotateLeft          ( CoupleNode<T> *a );
    CoupleNode<T>* rotateRight         ( CoupleNode<T> *a );
    CoupleNode<T>* rotateLeftThenRight ( CoupleNode<T> *n );
    CoupleNode<T>* rotateRightThenLeft ( CoupleNode<T> *n );
    void rebalance                  ( CoupleNode<T> *n );
    int height                      ( CoupleNode<T> *n );
    void setBalance                 ( CoupleNode<T> *n );
    void printBalance               ( CoupleNode<T> *n );
    void clearNode                  ( CoupleNode<T> *n );
};

/* Couple class definition */
template <class T>
void CoupleTree<T>::rebalance(CoupleNode<T> *n) {
    setBalance(n);

    if (n->balance == -2) {
        if (height(n->left->left) >= height(n->left->right))
            n = rotateRight(n);
        else
            n = rotateLeftThenRight(n);
    }
    else if (n->balance == 2) {
        if (height(n->right->right) >= height(n->right->left))
            n = rotateLeft(n);
        else
            n = rotateRightThenLeft(n);
    }

    if (n->parent != NULL) {
        rebalance(n->parent);
    }
    else {
        root = n;
    }
}

template <class T>
CoupleNode<T>* CoupleTree<T>::rotateLeft(CoupleNode<T> *a) {
    CoupleNode<T> *b = a->right;
    b->parent = a->parent;
    a->right = b->left;

    if (a->right != NULL)
        a->right->parent = a;

    b->left = a;
    a->parent = b;

    if (b->parent != NULL) {
        if (b->parent->right == a) {
            b->parent->right = b;
        }
        else {
            b->parent->left = b;
        }
    }

    setBalance(a);
    setBalance(b);
    return b;
}

template <class T>
CoupleNode<T>* CoupleTree<T>::rotateRight(CoupleNode<T> *a) {
    CoupleNode<T> *b = a->left;
    b->parent = a->parent;
    a->left = b->right;

    if (a->left != NULL)
        a->left->parent = a;

    b->right = a;
    a->parent = b;

    if (b->parent != NULL) {
        if (b->parent->right == a) {
            b->parent->right = b;
        }
        else {
            b->parent->left = b;
        }
    }

    setBalance(a);
    setBalance(b);
    return b;
}

template <class T>
CoupleNode<T>* CoupleTree<T>::rotateLeftThenRight(CoupleNode<T> *n) {
    n->left = rotateLeft(n->left);
    return rotateRight(n);
}

template <class T>
CoupleNode<T>* CoupleTree<T>::rotateRightThenLeft(CoupleNode<T> *n) {
    n->right = rotateRight(n->right);
    return rotateLeft(n);
}

template <class T>
int CoupleTree<T>::height(CoupleNode<T> *n) {
    if (n == NULL)
        return -1;
    return 1 + std::max(height(n->left), height(n->right));
}

template <class T>
void CoupleTree<T>::setBalance(CoupleNode<T> *n) {
    n->balance = height(n->right) - height(n->left);
}

template <class T>
void CoupleTree<T>::printBalance(CoupleNode<T> *n) {
    if (n != NULL) {
        printBalance(n->left);
        std::cout << n->balance << " ";
        printBalance(n->right);
    }
}

template <class T>
CoupleTree<T>::CoupleTree(void) : root(NULL) {}

template <class T>
CoupleTree<T>::~CoupleTree(void) {
    delete root;
}

template <class T>
bool CoupleTree<T>::insert(T key) {
    if (root == NULL) {
        root = new CoupleNode<T>(key, NULL);
    }
    else {
        CoupleNode<T>
            *n = root,
            *parent;

        while (true) {
            if (n->key == key)
                deleteKey(n->key);

            parent = n;

            bool goLeft = n->key > key;
            n = goLeft ? n->left : n->right;

            if (n == NULL) {
                if (goLeft) {
                    parent->left = new CoupleNode<T>(key, parent);
                }
                else {
                    parent->right = new CoupleNode<T>(key, parent);
                }

                rebalance(parent);
                break;
            }
        }
    }

    return true;
}

template <class T>
void CoupleTree<T>::deleteKey(const T delKey) {
    if (root == NULL)
        return;

    CoupleNode<T>
        *n       = root,
        *parent  = root,
        *delNode = NULL,
        *child   = root;

    while (child != NULL) {
        parent = n;
        n = child;
        child = delKey >= n->key ? n->right : n->left;
        if (delKey == n->key)
            delNode = n;
    }

    if (delNode != NULL) {
        delNode->key = n->key;

        child = n->left != NULL ? n->left : n->right;

        if (root->key == delKey) {
            root = child;
        }
        else {
            if (parent->left == n) {
                parent->left = child;
            }
            else {
                parent->right = child;
            }

            rebalance(parent);
        }
    }
}

template <class T>
void CoupleTree<T>::printBalance() {
    printBalance(root);
    std::cout << std::endl;
}