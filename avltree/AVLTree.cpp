#include <functional>
#include "AVLTree.h"

using namespace ::std;
/// \param k key of the node
AVLTree::Node::Node(const int k) : key(k) {}
/// \param k key of the node
/// \param p parent of the node
AVLTree::Node::Node(const int k, Node *p) : key(k),parent(p) {}
/// \param k key of the node
/// \param p parent of the node
/// \param l left child of the node
/// \param r right child of the node
AVLTree::Node::Node(const int k, Node *l, Node *r, Node *p)
        : key(k), left(l), right(r), parent(p){}

AVLTree::Node::~Node() {
    delete left;
    delete right;
}

AVLTree::~AVLTree() {
    delete root;
}

/********************************************************************
 * Search
 *******************************************************************/
/// \param value value which schould be searched
/// \returns whether given value is present in the tree
bool AVLTree::search(const int value) const {
    if (root == nullptr)
        return false;
    return root->search(value);
}
/// \param value value which is searched
/// \returns whether given value is present in the tree
bool AVLTree::Node::search(const int value) const {
    if (value == key) return true;
    if (value < key && left != nullptr) return left->search(value);
    if (value > key && right != nullptr) return right->search(value);
    return false;
}

/********************************************************************
 * Insert
 *******************************************************************/

/// \param value value to be inserted
void AVLTree::insert(int value) {
    if (root == nullptr)
        root = new Node(value);
    else
        root->insert(value,this);
}
/// \param value value to be inserted
/// \param tree passes on the tree in case the root needs to be changed
void AVLTree::Node::insert(int value, AVLTree *tree) {
    if (value == key)
        return;

    if (value < key) {
         if(left == nullptr){
             left = new Node(value,this);
             if(balance == 0) {
                 balance = -1;
                 upin(tree);
             } else {
                 balance = 0;
             }

             } else left->insert(value,tree);
         }


    if (value > key) {
        if (right == nullptr) {
            right = new Node(value,this);

            if (balance == 0) {
                balance = 1;
                upin(tree);
            } else {
                balance = 0;
            }


        }
        else right->insert(value,tree);
    }
}
/// \param tree passes on the tree in case the root needs to be changed
void AVLTree::Node::upin(AVLTree* tree) {
    if(parent == nullptr){
        return;
    }
    if(this == parent->left){
        if(parent->balance == 1)
            parent->balance =0;
        else if (parent->balance == 0){
            parent->balance = -1;
            parent->upin(tree);
        }
        else if(parent->balance == -1) {
            if (balance == -1) {
                parent->rotateRight(tree);
            } else if (balance == 1) {
                int t;
                if(right != nullptr){
                    t = right->balance;
                }

                auto p = parent;
                rotateLeft(tree);
                p->rotateRight(tree);
                if (t == 1) {
                    balance = -1;
                    parent->right->balance = 0;
                } else if (t == -1){
                    balance = 1;
                    parent->right->balance = 0;
                }else{
                    balance = 0;
                }
            }
        }
    } else {
        if(parent->balance == -1) {
            parent->balance = 0;
            return;
        }
        if (parent->balance == 0){
            parent->balance =1;
            parent->upin(tree);
            return;
        }

        if(parent->balance == 1){
                if(balance == 1){
                    parent->rotateLeft(tree);
                } else if(balance == -1) {
                    int t;
                    if(left != nullptr){
                        t = left->balance;
                    }
                    auto p = parent;
                    rotateRight(tree);
                    p->rotateLeft(tree);
                    if (t == 1) {
                        balance = 0;
                        parent->left->balance = -1;
                    } else if (t == -1){
                        balance = 1;
                        parent->left->balance = 0;
                    } else{
                        balance = 0;
                    }
                }
        }

    }

}
/// \param tree passes on the tree in case the root needs to be changed
void AVLTree::Node::rotateRight(AVLTree *tree){
    auto x = left;
    if(this == tree->root ){
        tree->root = x;
    } else if (parent->left == this){
        parent->left = x;
    }
    else {
        parent->right = x;
    }
    x->balance = 0;
    balance = 0;
    left = left->right;
    if(left != nullptr)
        left->parent = this;
    x->right = this;
    x->parent = parent;
    parent = x;

}
/// \param tree passes on the tree in case the root needs to be changed
void AVLTree::Node::rotateLeft(AVLTree *tree){

    auto x = right;
    if(this == tree->root){
        tree->root = x;
    }
    else if (parent->left == this){
        parent->left =x;
    }
    else {
        parent->right = x;
    }
    x->balance  = 0;
    balance = 0;

    right = right->left;
    x->left = this;
    x->parent = parent;
    parent = x;

}


/********************************************************************
 * Remove
 *******************************************************************/
/// \param node node of which the successor should be looked for
AVLTree::Node *findSymSucc(AVLTree::Node *node) {
    if (node->right == nullptr)
        return nullptr;
    auto result = node->right;
    while (result->left != nullptr) {
        result = result->left;
    }
    return result;
}
/// \param value value of the node to remove
void AVLTree::remove(const int value) {
    if (root != nullptr) {
        if (root->key == value) {
            if (root->left == nullptr && root->right == nullptr) {
                root = nullptr;
            } else if (root->left == nullptr) {
                root = root->right;
            } else if (root->right == nullptr)
                root = root->left;
            else {
                auto symSucc = findSymSucc(root);
                if (symSucc == root->right) {
                    root = new Node(symSucc->key, root->left, nullptr, nullptr);
                    root->balance = -1;
                    root->left->upout(this);
                } else {
                    root->right->remove(symSucc->key, this);
                    int bal = root->balance;
                    root = new Node(symSucc->key, root->left, root->right, nullptr);
                    root->balance = bal;
                    root->right->upout(this);
                }
            }
        } else
            root->remove(value, this);
    }
}
/// \param value value of the node to remove
/// \param tree passes on the tree in case the root needs to be changed
void AVLTree::Node::remove(const int value, AVLTree *tree) {

    if (value < key) {
        if (left != nullptr) {
            left->remove(value, tree);
        }
    }

    if (value > key) {
        if (right != nullptr) {
            right->remove(value, tree);
        }
    }


    if (key == value) {
        if (left == nullptr && right == nullptr) {
            // Both successors are leafs

            if (parent->left->key == key) {
                // Node is left successor of parent

                if (parent->right == nullptr) {
                    // Right brother has height 0
                    parent->balance = 0;
                    parent->left = nullptr;
                    parent->upout(tree);
                    return;
                } else if (parent->right->right == nullptr && parent->right->left == nullptr) {
                    // Right brother has height 1
                    parent->balance = 1;
                    parent->left = nullptr;
                    return;
                } else {
                    // Right brother has height 2
                    parent->left = nullptr;
                    parent->balance = 1;
                    if (parent->right->right != nullptr && parent->right->left != nullptr) {
                        auto p = parent;
                        auto r = parent->right;
                        parent->rotateLeft(tree);
                        p->balance = 1;
                        r->balance = -1;
                    } else if (parent->right->left == nullptr) {
                        auto p = parent;
                        auto r = parent->right;
                        parent->rotateLeft(tree);
                        p->balance = 0;
                        r->balance = 0;
                        r->upout(tree);
                    } else {
                        auto rl = parent->right->left;
                        parent->right->rotateRight(tree);
                        parent->rotateLeft(tree);
                        rl->balance = 0;
                        rl->right->balance = 0;
                        rl->left->balance = 0;
                        rl->upout(tree);
                    }
                }
            } else {
                // Node is right successor of parent

                if (parent->left == nullptr) {
                    // Left brother has height 0
                    parent->balance = 0;
                    parent->right = nullptr;
                    parent->upout(tree);
                    return;
                } else if (parent->left->left == nullptr && parent->left->right == nullptr) {
                    // Left brother has height 1
                    parent->balance = 1;
                    parent->right = nullptr;
                } else {
                    // Left brother has height 2
                    parent->right = nullptr;
                    parent->balance = -1;
                    if (parent->left->left != nullptr && parent->left->right != nullptr) {
                        // 2 childs
                        auto p = parent;
                        auto l = parent->left;
                        parent->rotateRight(tree);
                        p->balance = -1;
                        l->balance = 1;
                    } else if (parent->left->right == nullptr) {
                        // Only left child
                        auto p = parent;
                        auto l = parent->left;
                        parent->rotateRight(tree);
                        l->balance = 0;
                        p->balance = 0;
                        l->upout(tree);
                    } else {
                        // Only right child
                        auto lr = parent->left->right;
                        parent->left->rotateLeft(tree);
                        parent->rotateRight(tree);
                        lr->balance = 0;
                        lr->right->balance = 0;
                        lr->left->balance = 0;
                        lr->upout(tree);
                    }
                }
            }
        } else if (left == nullptr) {
            // Left successor is a leaf

            if (parent->left->key == key) {
                parent->left = right;
                parent->left->parent = parent;
                parent->left->upout(tree);
                return;
            } else {
                parent->right = right;
                parent->right->parent = parent;
                parent->right->upout(tree);
                return;
            }
        } else if (right == nullptr) {
            // Right successor is a leaf

            if (parent->left->key == key) {
                parent->left = left;
                parent->left->parent = parent;
                parent->left->upout(tree);
                return;
            } else {
                parent->right = left;
                parent->right->parent = parent;
                parent->right->upout(tree);
                return;
            }
        } else {
            // Both successors are nodes
            auto symSucc = findSymSucc(this);
            if (symSucc == right) {
                symSucc->balance = -1;
                right = nullptr;
            } else {
                symSucc->balance = balance;
            }

            if (parent->left->key == key) {
                parent->left = new Node(symSucc->key, left, right, parent);
                parent->left->balance = symSucc->balance;
                if(parent->left->right != nullptr) {
                    parent->left->right->upout(tree);
                }
            } else {
                parent->right = new Node(symSucc->key, left, right, parent);
                parent->right->balance = symSucc->balance;
                if (parent->right->right != nullptr) {
                    parent->right->right->upout(tree);
                }
            }
        }
    }
}

void AVLTree::Node::upout(AVLTree* tree) {
    if (parent == nullptr) {
        return;
    }

    if (parent->left->key == key) {
        // Node is left successor of parent

        if(parent->balance == -1) {
            parent->balance = 0;
            parent->upout(tree);
        } else if (parent->balance == 0) {
            parent->balance = 1;
        } else {
            // Parent's balance must be 1
            if (parent->right->balance == 0) {
                auto p = parent;
                auto r = parent->right;
                parent->rotateLeft(tree);
                p->balance = 1;
                r->balance = -1;
            } else if (parent->right->balance == 1) {
                auto r = parent->right;
                rotateLeft(tree);
                r->upout(tree);
            } else {
                // Right brothers balance must be -1
                // Double Rotate right-left
                auto rl = parent->right->left;
                parent->right->rotateRight(tree);
                rl->rotateLeft(tree);
                rl->upout(tree);
            }
        }
    } else {
        // Node is right successor of parent

        if(parent->balance == 1) {
            parent->balance = 0;
            parent->upout(tree);
        } else if (parent->balance == 0) {
            parent->balance = 1;
        } else {
            // Parent's balance must be -1
            if(parent->left->balance == 0) {
                auto p = parent;
                auto l = parent->left;
                parent->rotateRight(tree);
                p->balance = -1;
                l->balance = 1;
            } else if (parent->left->balance == -1) {
                auto l = parent->left;
                rotateLeft(tree);
                l->upout(tree);
            } else {
                // Left brother's balance must be 1
                // Double Rotate left-right
                auto lr = parent->left->right;
                parent->left->rotateLeft(tree);
                lr->rotateRight(tree);
                lr->upout(tree);
            }
        }

    }

}

/********************************************************************
 * Traversal
 *******************************************************************/

vector<int> *AVLTree::preorder() const {
    if (root == nullptr)
        return nullptr;
    return root->preorder();
}

vector<int> *AVLTree::Node::preorder() const {
    auto vec = new vector<int>();
    // Wurzel in vec
    vec->push_back(key);
    // linken Nachfolger in vec
    if (left != nullptr) {
        auto left_vec = left->preorder();
        vec->insert(vec->end(), left_vec->begin(), left_vec->end());
    }
    // rechten Nachfolger in vec
    if (right != nullptr) {
        auto right_vec = right->preorder();
        vec->insert(vec->end(), right_vec->begin(), right_vec->end());
    }
    return vec;
}

vector<int> *AVLTree::inorder() const {
    if (root == nullptr)
        return nullptr;
    return root->inorder();
}

vector<int> *AVLTree::Node::inorder() const {
    auto vec = new vector<int>();
    // linken Nachfolger in vec
    if (left != nullptr) {
        auto left_vec = left->inorder();
        vec->insert(vec->end(), left_vec->begin(), left_vec->end());
    }
    // Wurzel in vec
    vec->push_back(key);
    // rechten Nachfolger in vec
    if (right != nullptr) {
        auto right_vec = right->inorder();
        vec->insert(vec->end(), right_vec->begin(), right_vec->end());
    }
    return vec;
}

vector<int> *AVLTree::postorder() const {
    if (root == nullptr)
        return nullptr;
    return root->postorder();
}

vector<int> *AVLTree::Node::postorder() const {
    auto vec = new vector<int>();
    // linken Nachfolger in vec
    if (left != nullptr) {
        auto left_vec = left->postorder();
        vec->insert(vec->end(), left_vec->begin(), left_vec->end());
    }
    // rechten Nachfolger in vec
    if (right != nullptr) {
        auto right_vec = right->postorder();
        vec->insert(vec->end(), right_vec->begin(), right_vec->end());
    }
    // Wurzel in vec
    vec->push_back(key);
    return vec;
}

vector<int> *AVLTree::inorderBalance() const {
    if (root == nullptr)
        return nullptr;
    return root->inorderBalance();
}

vector<int> *AVLTree::Node::inorderBalance() const {
    auto vec = new vector<int>();
    // linken Nachfolger in vec
    if (left != nullptr) {
        auto left_vec = left->inorderBalance();
        vec->insert(vec->end(), left_vec->begin(), left_vec->end());
    }
    // Wurzel in vec
    vec->push_back(balance);
    // rechten Nachfolger in vec
    if (right != nullptr) {
        auto right_vec = right->inorderBalance();
        vec->insert(vec->end(), right_vec->begin(), right_vec->end());
    }
    return vec;
}
/********************************************************************
 * operator<<
 *******************************************************************/

std::ostream &operator<<(std::ostream &os, const AVLTree &tree) {
    function<void(std::ostream &, const int, const AVLTree::Node *, const string)> printToOs
            = [&](std::ostream &os, const int value, const AVLTree::Node *node, const string l) {

                static int nullcount = 0;

                if (node == nullptr) {
                    os << "    null" << nullcount << "[shape=point];" << endl;
                    os << "    " << value << " -> null" << nullcount
                       << " [label=\"" << l << "\"];" << endl;
                    nullcount++;
                } else {
                    os << "    " << value << " -> " << node->key
                       << " [label=\"" << l << "\"];" << endl;

                    printToOs(os, node->key, node->left, "l");
                    printToOs(os, node->key, node->right, "r");
                }
            };
    os << "digraph tree {" << endl;
    if (tree.root == nullptr) {
        os << "    null " << "[shape=point];" << endl;
    } else {
        printToOs(os, tree.root->key, tree.root->left, "l");
        printToOs(os, tree.root->key, tree.root->right, "r");
    }
    os << "}" << endl;
    return os;
}