#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <stdexcept>
#include <sstream>
#include <unordered_map>
#include <iostream>       

template <typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        
        Node(const T& data) : data(data), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* copyTree(Node* node) const;
    void clearTree(Node* node);
    Node* findNode(Node* node, const T& value) const;
    Node* extractSubtree(Node* node) const;
    bool containsSubtree(Node* tree, Node* subtree) const;
    bool compareTrees(Node* first, Node* second) const;
    
    void traverseKLP(Node* node, std::function<void(T)> action) const;
    void traverseKPL(Node* node, std::function<void(T)> action) const;
    void traverseLPK(Node* node, std::function<void(T)> action) const;
    void traverseLKP(Node* node, std::function<void(T)> action) const;
    void traversePLK(Node* node, std::function<void(T)> action) const;
    void traversePKL(Node* node, std::function<void(T)> action) const;
    
    void serializeKLP(Node* node, std::stringstream& ss) const;
    void serializeKPL(Node* node, std::stringstream& ss) const;
    void serializeLPK(Node* node, std::stringstream& ss) const;
    void serializeLKP(Node* node, std::stringstream& ss) const;
    void serializePLK(Node* node, std::stringstream& ss) const;
    void serializePKL(Node* node, std::stringstream& ss) const;
    
    Node* deserializeKLP(std::stringstream& ss);
    Node* deserializeKPL(std::stringstream& ss);
    Node* deserializeLPK(std::stringstream& ss);
    Node* deserializeLKP(std::stringstream& ss);
    Node* deserializePLK(std::stringstream& ss);
    Node* deserializePKL(std::stringstream& ss);

public:
    BinaryTree();
    BinaryTree(const BinaryTree& other);
    BinaryTree(BinaryTree&& other) noexcept;
    ~BinaryTree();

    BinaryTree& operator=(const BinaryTree& other);
    BinaryTree& operator=(BinaryTree&& other) noexcept;

    void insert(const T& value);
    bool contains(const T& value) const;
    void remove(const T& value);
    
    void traverseKLP(std::function<void(T)> action) const;
    void traverseKPL(std::function<void(T)> action) const;
    void traverseLPK(std::function<void(T)> action) const;
    void traverseLKP(std::function<void(T)> action) const;
    void traversePLK(std::function<void(T)> action) const;
    void traversePKL(std::function<void(T)> action) const;
    
    template <typename U>
    BinaryTree<U>* map(U (*func)(T)) const;
    
    template <typename U>
    BinaryTree<U>* map(const std::function<U(T)>& func) const;
    
    BinaryTree<T>* where(bool (*predicate)(T)) const;
    BinaryTree<T>* where(const std::function<bool(T)>& predicate) const;
    
    template <typename U>
    U reduce(U (*func)(U, T), U initial) const;
    
    template <typename U>
    U reduce(const std::function<U(U, T)>& func, U initial) const;

    BinaryTree<T>* merge(const BinaryTree<T>& other) const;
    BinaryTree<T>* extractSubtree(const T& value) const;
    bool containsSubtree(const BinaryTree<T>& subtree) const;

    void PrintPretty() const;
    
    std::string serialize(const std::string& traversal = "KLP") const;
    void deserialize(const std::string& str, const std::string& traversal = "KLP");
    void deserializeFromParentList(const std::string& str);
    
    
    T getByPath(const std::string& path) const;
    T getByRelativePath(const T& start, const std::string& path) const;
};


template <typename T>
BinaryTree<T>::BinaryTree() : root(nullptr) {}

template <typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& other) {
    root = copyTree(other.root);
}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree&& other) noexcept : root(other.root) {
    other.root = nullptr;
}

template <typename T>
BinaryTree<T>::~BinaryTree() {
    clearTree(root);
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::copyTree(Node* node) const {
    if (!node) return nullptr;
    Node* newNode = new Node(node->data);
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);
    return newNode;
}

template <typename T>
void BinaryTree<T>::clearTree(Node* node) {
    if (node) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

template <typename T>
void BinaryTree<T>::insert(const T& value) {
    if (!root) {
        root = new Node(value);
        return;
    }

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (!current->left) {
            current->left = new Node(value);
            return;
        } else {
            q.push(current->left);
        }

        if (!current->right) {
            current->right = new Node(value);
            return;
        } else {
            q.push(current->right);
        }
    }
}

template <typename T>
bool BinaryTree<T>::contains(const T& value) const {
    return findNode(root, value) != nullptr;
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::findNode(Node* node, const T& value) const {
    if (!node) return nullptr;
    if (node->data == value) return node;
    
    Node* found = findNode(node->left, value);
    if (found) return found;
    
    return findNode(node->right, value);
}

template <typename T>
void BinaryTree<T>::remove(const T& value) {
    if (!root) return;
    
    if (root->data == value && !root->left && !root->right) {
        delete root;
        root = nullptr;
        return;
    }

    Node* toDelete = nullptr;
    Node* deepest = nullptr;
    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        deepest = q.front();
        q.pop();

        if (deepest->data == value) {
            toDelete = deepest;
        }

        if (deepest->left) q.push(deepest->left);
        if (deepest->right) q.push(deepest->right);
    }

    if (!toDelete) return; // Узел не найден

    // Находим родителя самого глубокого узла
    Node* parent = nullptr;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->left == deepest || current->right == deepest) {
            parent = current;
            break;
        }

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    // Заменяем данные и удаляем самый глубокий узел
    toDelete->data = deepest->data;
    if (parent) {
        if (parent->left == deepest) {
            parent->left = nullptr;
        } else {
            parent->right = nullptr;
        }
    }
    delete deepest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void BinaryTree<T>::traverseKLP(std::function<void(T)> action) const {
    traverseKLP(root, action);
}

template <typename T>
void BinaryTree<T>::traverseKLP(Node* node, std::function<void(T)> action) const {
    if (!node) return;
    action(node->data);
    traverseKLP(node->left, action);
    traverseKLP(node->right, action);
}

template <typename T>
void BinaryTree<T>::traverseKPL(std::function<void(T)> action) const {
    traverseKPL(root, action);
}

template <typename T>
void BinaryTree<T>::traverseKPL(Node* node, std::function<void(T)> action) const {
    if (!node) return;
    action(node->data);
    traverseKPL(node->right, action);
    traverseKPL(node->left, action);
}

template <typename T>
void BinaryTree<T>::traverseLPK(std::function<void(T)> action) const {
    traverseLPK(root, action);
}

template <typename T>
void BinaryTree<T>::traverseLPK(Node* node, std::function<void(T)> action) const {
    if (!node) return;
    traverseLPK(node->left, action);
    traverseLPK(node->right, action);
    action(node->data);
}

template <typename T>
void BinaryTree<T>::traverseLKP(std::function<void(T)> action) const {
    traverseLKP(root, action);
}

template <typename T>
void BinaryTree<T>::traverseLKP(Node* node, std::function<void(T)> action) const {
    if (!node) return;
    traverseLKP(node->left, action);
    action(node->data);
    traverseLKP(node->right, action);
}

template <typename T>
void BinaryTree<T>::traversePLK(std::function<void(T)> action) const {
    traversePLK(root, action);
}

template <typename T>
void BinaryTree<T>::traversePLK(Node* node, std::function<void(T)> action) const {
    if (!node) return;
    traversePLK(node->right, action);
    traversePLK(node->left, action);
    action(node->data);
}

template <typename T>
void BinaryTree<T>::traversePKL(std::function<void(T)> action) const {
    traversePKL(root, action);
}

template <typename T>
void BinaryTree<T>::traversePKL(Node* node, std::function<void(T)> action) const {
    if (!node) return;
    traversePKL(node->right, action);
    action(node->data);
    traversePKL(node->left, action);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename U>
BinaryTree<U>* BinaryTree<T>::map(U (*func)(T)) const {
    return map(std::function<U(T)>(func));
}

template <typename T>
template <typename U>
BinaryTree<U>* BinaryTree<T>::map(const std::function<U(T)>& func) const {
    auto* newTree = new BinaryTree<U>();
    if (!root) return newTree;
    
    std::queue<Node*> q;
    std::queue<typename BinaryTree<U>::Node*> newQ;
    q.push(root);
    newTree->root = new typename BinaryTree<U>::Node(func(root->data));
    newQ.push(newTree->root);
    
    while (!q.empty()) {
        Node* current = q.front();
        typename BinaryTree<U>::Node* newCurrent = newQ.front();
        q.pop();
        newQ.pop();
        
        if (current->left) {
            newCurrent->left = new typename BinaryTree<U>::Node(func(current->left->data));
            q.push(current->left);
            newQ.push(newCurrent->left);
        }
        
        if (current->right) {
            newCurrent->right = new typename BinaryTree<U>::Node(func(current->right->data));
            q.push(current->right);
            newQ.push(newCurrent->right);
        }
    }
    
    return newTree;
}

template <typename T>
BinaryTree<T>* BinaryTree<T>::where(bool (*predicate)(T)) const {
    return where(std::function<bool(T)>(predicate));
}

template <typename T>
BinaryTree<T>* BinaryTree<T>::where(const std::function<bool(T)>& predicate) const {
    auto* newTree = new BinaryTree<T>();
    if (!root) return newTree;
    
    std::queue<Node*> q;
    q.push(root);
    
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        
        if (predicate(current->data)) {
            newTree->insert(current->data);
        }
        
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    
    return newTree;
}

template <typename T>
template <typename U>
U BinaryTree<T>::reduce(U (*func)(U, T), U initial) const {
    return reduce(std::function<U(U, T)>(func), initial);
}

template <typename T>
template <typename U>
U BinaryTree<T>::reduce(const std::function<U(U, T)>& func, U initial) const {
    traverseLKP([&](T value) {
        initial = func(initial, value);
    });
    return initial;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
BinaryTree<T>* BinaryTree<T>::merge(const BinaryTree<T>& other) const {
    auto* newTree = new BinaryTree<T>(*this);
    other.traverseLKP([&](T value) {
        newTree->insert(value);
    });
    return newTree;
}

template <typename T>
BinaryTree<T>* BinaryTree<T>::extractSubtree(const T& value) const {
    Node* subtreeRoot = findNode(root, value);
    if (!subtreeRoot) return nullptr;
    
    auto* subtree = new BinaryTree<T>();
    subtree->root = extractSubtree(subtreeRoot);
    return subtree;
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::extractSubtree(Node* node) const {
    if (!node) return nullptr;
    Node* newNode = new Node(node->data);
    newNode->left = extractSubtree(node->left);
    newNode->right = extractSubtree(node->right);
    return newNode;
}

template <typename T>
bool BinaryTree<T>::containsSubtree(const BinaryTree<T>& subtree) const {
    if (!subtree.root) return true;
    Node* startNode = findNode(root, subtree.root->data);
    if (!startNode) return false;
    return compareTrees(startNode, subtree.root);
}

template <typename T>
bool BinaryTree<T>::containsSubtree(Node* tree, Node* subtree) const {
    if (!subtree) return true;
    if (!tree) return false; 
    
    if (tree->data == subtree->data &&
        compareTrees(tree->left, subtree->left) &&
        compareTrees(tree->right, subtree->right)) {
        return true;
    }
    
    return containsSubtree(tree->left, subtree) || 
           containsSubtree(tree->right, subtree);
}

template <typename T>
bool BinaryTree<T>::compareTrees(Node* first, Node* second) const {
    if (!first && !second) return true;
    if (!first || !second) return false;
    return first->data == second->data &&
           compareTrees(first->left, second->left) &&
           compareTrees(first->right, second->right);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::string BinaryTree<T>::serialize(const std::string& traversal) const {
    std::stringstream ss;
    
    if (traversal == "KLP") serializeKLP(root, ss);
    else if (traversal == "KPL") serializeKPL(root, ss);
    else if (traversal == "LPK") serializeLPK(root, ss);
    else if (traversal == "LKP") serializeLKP(root, ss);
    else if (traversal == "PLK") serializePLK(root, ss);
    else if (traversal == "PKL") serializePKL(root, ss);
    else throw std::invalid_argument("Unknown traversal type");
    
    return ss.str();
}

template <typename T>
void BinaryTree<T>::serializeKLP(Node* node, std::stringstream& ss) const {
    if (!node) {
        ss << "0 ";
        return;
    }
    ss << node->data << " ";
    serializeKLP(node->left, ss);
    serializeKLP(node->right, ss);
}

template <typename T>
void BinaryTree<T>::serializeKPL(Node* node, std::stringstream& ss) const {
    if (!node) {
        ss << "0 ";
        return;
    }
    ss << node->data << " ";
    serializeKPL(node->right, ss);
    serializeKPL(node->left, ss);
}

template <typename T>
void BinaryTree<T>::serializeLPK(Node* node, std::stringstream& ss) const {
    if (!node) {
        ss << "0 ";
        return;
    }
    serializeLPK(node->left, ss);
    serializeLPK(node->right, ss);
    ss << node->data << " ";
}

template <typename T>
void BinaryTree<T>::serializeLKP(Node* node, std::stringstream& ss) const {
    if (!node) {
        ss << "0 ";
        return;
    }
    serializeLKP(node->left, ss);
    ss << node->data << " ";
    serializeLKP(node->right, ss);
}

template <typename T>
void BinaryTree<T>::serializePLK(Node* node, std::stringstream& ss) const {
    if (!node) {
        ss << "0 ";
        return;
    }
    serializePLK(node->right, ss);
    serializePLK(node->left, ss);
    ss << node->data << " ";
}

template <typename T>
void BinaryTree<T>::serializePKL(Node* node, std::stringstream& ss) const {
    if (!node) {
        ss << "0 ";
        return;
    }
    serializePKL(node->right, ss);
    ss << node->data << " ";
    serializePKL(node->left, ss);
}

template <typename T>
void BinaryTree<T>::deserialize(const std::string& str, const std::string& traversal) {
    clearTree(root);
    root = nullptr;
    
    std::stringstream ss(str);
    
    if (traversal == "KLP") root = deserializeKLP(ss);
    else if (traversal == "KPL") root = deserializeKPL(ss);
    else if (traversal == "LPK") root = deserializeLPK(ss);
    else if (traversal == "LKP") root = deserializeLKP(ss);
    else if (traversal == "PLK") root = deserializePLK(ss);
    else if (traversal == "PKL") root = deserializePKL(ss);
    else throw std::invalid_argument("Unknown traversal type");
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::deserializeKLP(std::stringstream& ss) {
    std::string token;
    if (!(ss >> token) || token == "0") return nullptr;
    
    std::stringstream converter(token);
    T value;
    converter >> value;
    
    Node* node = new Node(value);
    node->left = deserializeKLP(ss);
    node->right = deserializeKLP(ss);
    return node;
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::deserializeKPL(std::stringstream& ss) {
    std::string token;
    if (!(ss >> token) || token == "0") return nullptr;
    
    std::stringstream converter(token);
    T value;
    converter >> value;
    
    Node* node = new Node(value);
    node->right = deserializeKPL(ss);
    node->left = deserializeKPL(ss);
    return node;
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::deserializeLPK(std::stringstream& ss) {
    std::string token;
    if (!(ss >> token) || token == "0") return nullptr;
    
    // Для LPK сначала идут левое и правое поддеревья
    Node* left = deserializeLPK(ss);
    Node* right = deserializeLPK(ss);
    
    std::stringstream converter(token);
    T value;
    converter >> value;
    
    Node* node = new Node(value);
    node->left = left;
    node->right = right;
    return node;
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::deserializeLKP(std::stringstream& ss) {
    std::string token;
    if (!(ss >> token) || token == "0") return nullptr;
    
    // Для LKP сначала левое поддерево, затем значение, затем правое
    Node* left = deserializeLKP(ss);
    
    std::stringstream converter(token);
    T value;
    converter >> value;
    
    Node* node = new Node(value);
    node->left = left;
    node->right = deserializeLKP(ss);
    return node;
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::deserializePLK(std::stringstream& ss) {
    std::string token;
    if (!(ss >> token) || token == "0") return nullptr;
    
    // Для PLK сначала правое поддерево, затем левое, затем значение
    Node* right = deserializePLK(ss);
    Node* left = deserializePLK(ss);
    
    std::stringstream converter(token);
    T value;
    converter >> value;
    
    Node* node = new Node(value);
    node->left = left;
    node->right = right;
    return node;
}

template <typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::deserializePKL(std::stringstream& ss) {
    std::string token;
    if (!(ss >> token) || token == "0") return nullptr;
    
    // Для PKL сначала правое поддерево, затем значение, затем левое
    Node* right = deserializePKL(ss);
    
    std::stringstream converter(token);
    T value;
    converter >> value;
    
    Node* node = new Node(value);
    node->right = right;
    node->left = deserializePKL(ss);
    return node;
}

template <typename T>
void BinaryTree<T>::deserializeFromParentList(const std::string& str) {
    clearTree(root);
    root = nullptr;
    
    std::stringstream ss(str);
    std::unordered_map<T, Node*> nodes;
    std::unordered_map<T, char> childTypes;
    T child, parent;
    char type;
    
    while (ss >> child >> parent >> type) {
        if (!root && type == 'R') {
            root = new Node(parent);
            nodes[parent] = root;
        }
        
        Node* parentNode = nodes[parent];
        if (!parentNode) {
            parentNode = new Node(parent);
            nodes[parent] = parentNode;
            if (!root) root = parentNode;
        }
        
        Node* childNode = new Node(child);
        nodes[child] = childNode;
        
        if (type == 'L') parentNode->left = childNode;
        else parentNode->right = childNode;
    }
}

//////////////////////////////////////////////////////////

template <typename T>
T BinaryTree<T>::getByPath(const std::string& path) const {
    Node* current = root;
    if (!current) throw std::out_of_range("Tree is empty");
    
    for (char direction : path) {
        if (direction == 'L') {
            if (!current->left) throw std::out_of_range("Path not found");
            current = current->left;
        }
        else if (direction == 'P') {
            if (!current->right) throw std::out_of_range("Path not found");
            current = current->right;
        }
        else {
            throw std::invalid_argument("Invalid path character");
        }
    }
    
    return current->data;
}

template <typename T>
T BinaryTree<T>::getByRelativePath(const T& start, const std::string& path) const {
    Node* startNode = findNode(root, start);
    if (!startNode) throw std::out_of_range("Start node not found");
    
    Node* current = startNode;
    for (char direction : path) {
        if (direction == 'L') {
            if (!current->left) throw std::out_of_range("Path not found");
            current = current->left;
        }
        else if (direction == 'P') {
            if (!current->right) throw std::out_of_range("Path not found");
            current = current->right;
        }
        else {
            throw std::invalid_argument("Invalid path character");
        }
    }
    
    return current->data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& other) {
    if (this != &other) {
        clearTree(root);
        root = copyTree(other.root);
    }
    return *this;
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree&& other) noexcept {
    if (this != &other) {
        clearTree(root);
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}

template<class T> 
void BinaryTree<T>::PrintPretty() const {
        if (!root) {
            std::cout << "Tree is empty" << std::endl;
            return;
        }
        
        const std::string ch_hor = "\u2500"; // ─
        const std::string ch_ver = "\u2502"; // │
        const std::string ch_ddia = "\u250C"; // ┌
        const std::string ch_rddia = "\u2510"; // ┐
        
        auto VSCat = [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
            auto r = a;
            r.insert(r.end(), b.begin(), b.end());
            return r;
        };
        
        std::vector<std::vector<std::string>> lines;
        std::function<void(Node*, std::vector<std::string>, 
                         std::vector<std::string>, 
                         std::vector<std::string>, 
                         bool, bool)> 
        buildLines = [&](Node* node, 
                        std::vector<std::string> lpref, 
                        std::vector<std::string> cpref, 
                        std::vector<std::string> rpref, 
                        bool root, bool left) {
            if (!node) return;
            
            // Рекурсивный вызов для левого поддерева
            if (node->left) {
                buildLines(node->left, 
                         VSCat(lpref, {" ", " "}), 
                         VSCat(lpref, {ch_ddia, ch_ver}), 
                         VSCat(lpref, {ch_hor, " "}), 
                         false, true);
            }
            
            // Преобразование значения узла в строку
            std::string sval;
            std::ostringstream oss;
            oss << node->data;
            sval = oss.str();
            
            // Определение позиции символа относительно префиксов
            size_t sm = left || sval.empty() ? sval.size() / 2 : ((sval.size() + 1) / 2 - 1);
            
            // Добавление строк с символами
            for (size_t i = 0; i < sval.size(); ++i) {
                std::vector<std::string> line;
                if (i < sm) {
                    line = VSCat(lpref, {std::string(1, sval[i])});
                } else if (i == sm) {
                    line = VSCat(cpref, {std::string(1, sval[i])});
                } else {
                    line = VSCat(rpref, {std::string(1, sval[i])});
                }
                lines.push_back(line);
            }
            
            // Рекурсивный вызов для правого поддерева
            if (node->right) {
                buildLines(node->right, 
                         VSCat(rpref, {ch_hor, " "}), 
                         VSCat(rpref, {ch_rddia, ch_ver}), 
                         VSCat(rpref, {" ", " "}), 
                         false, false);
            }
        };
        
    
        buildLines(root, {}, {}, {}, true, true);
        
        // Объединяем линии для вывода
        std::vector<std::string> out;
        for (size_t l = 0; ; ++l) {
            bool last = true;
            std::string line;
            for (size_t i = 0; i < lines.size(); ++i) {
                if (l < lines[i].size()) {
                    line += lines[i][l];
                    last = false;
                } else {
                    line += " ";
                }
            }
            if (last) break;
            out.push_back(line);
        }
        
        // Выводим результат
        for (const auto& line : out) {
            std::cout << line << std::endl;
        }
    }

#endif 