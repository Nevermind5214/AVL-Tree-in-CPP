#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include <iostream>
#include <string>
#include <algorithm>

bool isNumber(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

template<typename T>
class AVL {
public:

    struct node {
        struct node* left;
        struct node* right;
        T data;
    };

    struct node* root;
    int leftRotations;
    int rightRotations;

    AVL() {
        this->root = nullptr;
        leftRotations = 0;
        rightRotations = 0;
    }

    friend std::ostream& operator<< (std::ostream& os, AVL& obj) {
        return os << obj.toString();
    }

    std::string toString() {
        return toString(this->root, 0);
    }

    void insert(T value) {
        this->root = insert(this->root, value);
    }

    void remove(T value) {
        this->root = remove(this->root, value);
    }

    std::string preOrder() {
        return preOrder(this->root);
    }

    std::string inOrder() {
        return inOrder(this->root);
    }

    std::string postOrder() {
        return postOrder(this->root);
    }

    ~AVL() = default;

private:

    std::string toString(struct node* currNode, int tabs) {
        std::string retStr;

        if (!currNode) return "";

        retStr += toString(currNode->right, tabs + 5);

        for (int k = 0; k < tabs - 5; k++)  retStr += "  ";
        if (tabs > 0) retStr += "  |--------";

        retStr += std::to_string(currNode->data) + "\n";

        retStr += toString(currNode->left, tabs + 5);

        return retStr;
    }

    int height(struct node* rootPtr) {
        int left = 0;
        int right = 0;
        if (!rootPtr) return 0;
        if (rootPtr->left) left = height(rootPtr->left);
        if (rootPtr->right) right = height(rootPtr->right);
        return std::max(left, right) + 1;
    }

    int balanceFactor(struct node* rootPtr) {
        if (rootPtr == nullptr) return 0;
        return height(rootPtr->left) - height(rootPtr->right);
    }

    struct node* leftRotate(struct node* x) {
        leftRotations++;
        struct node* z = x->right;
        x->right = z->left;
        z->left = x;
        return z;
    }

    struct node* rightRotate(struct node* z) {
        rightRotations++;
        struct node* x = z->left;
        z->left = x->right;
        x->right = z;
        return x;
    }

    struct node* insert(struct node* rootPtr, T value) {
        if (rootPtr == nullptr) {
            rootPtr = new node;
            rootPtr->left = nullptr;
            rootPtr->right = nullptr;
            rootPtr->data = value;
            return rootPtr;
        }
        else if (rootPtr->data > value)
            rootPtr->left = insert(rootPtr->left, value);
        else if (rootPtr->data < value)
            rootPtr->right = insert(rootPtr->right, value);
        else throw std::runtime_error("Value already in tree!");

        int e = balanceFactor(rootPtr);

        if (e > 1) {
            if (value > rootPtr->left->data)
                rootPtr->left = leftRotate(rootPtr->left);
            rootPtr = rightRotate(rootPtr);
        }
        else if (e < -1) {
            if (value < rootPtr->right->data)
                rootPtr->right = rightRotate(rootPtr->right);
            rootPtr = leftRotate(rootPtr);
        }

        return rootPtr;
    }

    struct node* remove(struct node* rootPtr, T value) {
        if (rootPtr == nullptr) throw std::runtime_error("No such Node!");
        else if (rootPtr->data > value) rootPtr->left = remove(rootPtr->left, value);
        else if (rootPtr->data < value) rootPtr->right = remove(rootPtr->right, value);
        else if (rootPtr->left == nullptr) rootPtr = rootPtr->right;
        else if (rootPtr->right == nullptr) rootPtr = rootPtr->left;
        else {
            struct node* leftMost = rootPtr->right;
            while (leftMost->left != nullptr) leftMost = leftMost->left;
            rootPtr->data = leftMost->data;
            rootPtr->right = remove(rootPtr->right, leftMost->data);
        }

        int e = balanceFactor(rootPtr);

        if (e > 1) {
            if (balanceFactor(rootPtr->left) < 0) rootPtr->left = leftRotate(rootPtr->left);
            rootPtr = rightRotate(rootPtr);
        }
        else if (e < -1) {
            if (balanceFactor(rootPtr->right) > 0) rootPtr->right = rightRotate(rootPtr->right);
            rootPtr = leftRotate(rootPtr);
        }

        return rootPtr;
    }

    std::string preOrder(node* currNode) {
        std::string result;
        if (!currNode) {
            return "";
        }

        result += std::to_string(currNode->data) + " ";
        result += preOrder(currNode->left);
        result += preOrder(currNode->right);

        return result;
    }

    std::string inOrder(node* currNode) {
        std::string result;
        if (!currNode) {
            return "";
        }

        result += inOrder(currNode->left);
        result += std::to_string(currNode->data) + " ";
        result += inOrder(currNode->right);

        return result;
    }

    std::string postOrder(node* currNode) {
        std::string result;
        if (!currNode) {
            return "";
        }

        result += postOrder(currNode->left);
        result += postOrder(currNode->right);
        result += std::to_string(currNode->data) + " ";

        return result;
    }
};

int main() {
    std::cout << "-- Interactive AVL tree(integer) --" << std::endl << R"(Delete node: "d", Exit: "x")" << std::endl;
    AVL<int> tree;
    std::string input;

    while (true) {
        std::cout << "New value: ";
        std::cin >> input;

        if (isNumber(input)) try {
            tree.insert(stoi(input));
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        else if (input[0] == 'd') {
            std::cout << "Value to delete from tree: ";
            std::cin >> input;
            if (isNumber(input)) try {
                tree.remove(stoi(input));
            }
            catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
            else std::cout << std::endl << "Invalid input!" << std::endl;
        }
        else if (input[0] == 'x') {
            break;
        }
        else std::cout << std::endl << "Invalid input!" << std::endl;

        std::cout << std::endl << "-------* Left rotations: " << tree.leftRotations << " Right rotations: " << tree.rightRotations << " *-------" << std::endl;
        std::cout << std::endl << tree;
        std::cout << std::endl << "Preorder traversal: " << tree.preOrder();
        std::cout << std::endl << "Inorder traversal: " << tree.inOrder();
        std::cout << std::endl << "Postorder traversal: " << tree.postOrder();
        std::cout << std::endl << std::endl;
    }
}

#pragma clang diagnostic pop
