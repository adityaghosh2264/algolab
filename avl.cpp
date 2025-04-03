#include <iostream>
using namespace std;

class Node {
    public:
        int key;
        Node* left;
        Node* right;
        int height;
};

int max(int a, int b) {
    return (a > b) ? a : b;
}
class avl{
    public:
        Node *root;
        avl() : root(nullptr) {}
        void push(int val){ root = insert(root,val);}
        void inorder(){ inorder(root);}
        void remove(int val){ root = deleteNode(root,val);}
    private:
        int height(Node* node) {
            if (node == nullptr)
                return 0;
            return node->height;
        }

        int getBalanceFactor(Node* node) {
            if (node == nullptr)
                return 0;
            return height(node->left) - height(node->right);
        }

        Node* newNode(int key) {
            Node* node = new Node();
            node->key = key;
            node->left = nullptr;
            node->right = nullptr;
            node->height = 1;
            return node;
        }

        Node* rightRotate(Node* y) {
            Node* x = y->left;
            Node* T2 = x->right;

            x->right = y;
            y->left = T2;

            y->height = max(height(y->left), height(y->right)) + 1;
            x->height = max(height(x->left), height(x->right)) + 1;

            return x;
        }

        Node* leftRotate(Node* x) {
            Node* y = x->right;
            Node* T2 = y->left;

            y->left = x;
            x->right = T2;

            x->height = max(height(x->left), height(x->right)) + 1;
            y->height = max(height(y->left), height(y->right)) + 1;

            return y;
        }

        Node* insert(Node* node, int key) {
            if (node == nullptr)
                return newNode(key);

            if (key < node->key)
                node->left = insert(node->left, key);
            else if (key > node->key)
                node->right = insert(node->right, key);
            else
                return node;

            node->height = 1 + max(height(node->left), height(node->right));

            int balance = getBalanceFactor(node);

            if (balance > 1 && key < node->left->key)
                return rightRotate(node);

            if (balance < -1 && key > node->right->key)
                return leftRotate(node);

            if (balance > 1 && key > node->left->key) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }

            if (balance < -1 && key < node->right->key) {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }

            return node;
        }

        Node* minValueNode(Node* node) {
            Node* current = node;
            while (current->left != nullptr)
                current = current->left;
            return current;
        }
        
        Node* deleteNode(Node* node, int key) {
            if (node == nullptr)
                return node;

            if (key < node->key)
                node->left = deleteNode(node->left, key);
            else if (key > node->key)
                node->right = deleteNode(node->right, key);
            else {
                if ((node->left == nullptr) || (node->right == nullptr)) {
                    Node* temp = node->left ? node->left : node->right;
                    if (temp == nullptr) {
                        temp = node;
                        node = nullptr;
                    } else
                        *node = *temp;
                    delete temp;
                } else {
                    Node* temp = minValueNode(node->right);
                    node->key = temp->key;
                    node->right = deleteNode(node->right, temp->key);
                }
            }

            if (node == nullptr)
                return node;

            node->height = 1 + max(height(node->left), height(node->right));
            int balance = getBalanceFactor(node);

            if (balance > 1 && getBalanceFactor(node->left) >= 0)
                return rightRotate(node);

            if (balance > 1 && getBalanceFactor(node->left) < 0) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }

            if (balance < -1 && getBalanceFactor(node->right) <= 0)
                return leftRotate(node);

            if (balance < -1 && getBalanceFactor(node->right) > 0) {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }

            return node;
        }

        void inorder(Node* node) {
            if (node != nullptr) {
                inorder(node->left);
                cout << node->key << " ";
                inorder(node->right);
            }
        }
};

int main() {
    avl tree;

    tree.push(10);
    tree.push(20);
    tree.push(30);
    tree.push(40);
    tree.push(50);
    tree.push(25);

    cout << "In-order traversal of the AVL tree: ";
    tree.inorder();
    cout << endl;

    tree.remove(30); 
    tree.remove(40);
    
    cout << "In-order traversal of the AVL tree: ";
    tree.inorder();
    cout << endl;

    return 0;
}