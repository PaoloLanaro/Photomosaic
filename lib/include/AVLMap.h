//
// Created by lanar on 10/26/2023.
//

#ifndef PHOTOMOSAIC_AVLMAP_H
#define PHOTOMOSAIC_AVLMAP_H

#include <filesystem>

namespace fs = std::filesystem;

template<typename K, typename V>
class AVLMap {

private:
    class AVLNode {
    public:
        K key;
        V value;
        AVLNode *left;
        AVLNode *right;
        int height;

        AVLNode(const K &k, const V &v);
    };

    AVLNode *root{nullptr};

    void insert(AVLNode *&node, const K &key, const V &value);

    AVLNode *search(AVLNode *current, const K &searchKey);

    void clearMap(AVLNode *current);

    int getHeight(AVLNode *node);

    int getBalanceFactor(AVLMap::AVLNode *node);

    AVLNode *rotateLeft(AVLMap::AVLNode *node);

    AVLNode *rotateRight(AVLMap::AVLNode *node);

    AVLNode *copyMap(AVLNode *current);
public:
    // constructor
    AVLMap() = default;

    // ====rule of three====
    // destructor
    ~AVLMap();

    // copy constructor
    AVLMap(const AVLMap &other);

    // copy assignment operator
    AVLMap &operator=(const AVLMap &other);

    // subscribe operator
    V &operator[](const K &searchKey);

    // public search functionality
    V &search(const K &searchKey);

    // public insert functionality
    void insert(K key, V Value);

    void loadDirectory(const std::string &inputDirectoryPath, int imageSquareSizes);
};

template<typename K, typename V>
void AVLMap<K, V>::loadDirectory(const std::string &inputDirectoryPath, int imageSquareSizes) {
    fs::path imageLibrary(inputDirectoryPath);

    AVLMap<std::string, std::string> validExtensions;
    validExtensions.insert(".jpg", ".jpg");
    validExtensions.insert(".jpeg", ".jpeg");
    validExtensions.insert(".png", ".png");
    validExtensions.insert(".bmp", ".bmp");
    validExtensions.insert(".tga", ".tga");
    for (const auto &entry: fs::recursive_directory_iterator(imageLibrary)) {
        const auto &filePath = entry.path();
        auto extension = filePath.extension().string();

        if (validExtensions.search(extension) != extension) {
            continue;
        }

        Image currentImage(filePath.relative_path());
        currentImage.readImage();
        currentImage.resizeImage(imageSquareSizes);
        Pixel pixelAvg = currentImage.getAverageImagePixel();
        insert(pixelAvg, currentImage);
    }
}

template<typename K, typename V>
AVLMap<K, V>::AVLNode::AVLNode(const K &k, const V &v): key(k), value(v), left(nullptr), right(nullptr), height(0) {}

template<typename K, typename V>
int AVLMap<K, V>::getHeight(AVLMap::AVLNode *node) {
    return (node != nullptr) ? node->height : 0;
}

template<typename K, typename V>
int AVLMap<K, V>::getBalanceFactor(AVLMap::AVLNode *node) {
    return (node != nullptr) ? getHeight(node->left) - getHeight(node->right) : 0;
}

template<typename K, typename V>
typename AVLMap<K, V>::AVLNode *AVLMap<K, V>::rotateRight(AVLMap::AVLNode *node) {
    AVLNode *x = node->left;
    AVLNode *newRoot = x;
    AVLNode *T2 = x->right;

    // perform the rotation
    newRoot->right = node;
    node->left = T2;

    // update heights
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    newRoot->height = 1 + std::max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot; // new root
}

template<typename K, typename V>
typename AVLMap<K, V>::AVLNode *AVLMap<K, V>::rotateLeft(AVLMap::AVLNode *node) {
    AVLNode *y = node->right;
    AVLNode *newRoot = y;
    AVLNode *T2 = y->left;

    // perform rotation
    newRoot->left = node;
    node->right = T2;

    // update heights
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    newRoot->height = 1 + std::max(getHeight(newRoot->left), getHeight(newRoot->right));

    return newRoot; // new root
}

// map copy helper
template<typename K, typename V>
typename AVLMap<K, V>::AVLNode *AVLMap<K, V>::copyMap(AVLMap::AVLNode *current) {
    if (current == nullptr) {
        return nullptr;
    }
    auto *newNode = new AVLNode(current->key, current->value);
    newNode->left = copyMap(current->left);
    newNode->right = copyMap(current->right);
    return newNode;

}

// map destructor / clear helper
template<typename K, typename V>
void AVLMap<K, V>::clearMap(AVLMap::AVLNode *current) {
    if (current != nullptr) {
        clearMap(current->left);
        clearMap(current->right);
        delete current;
    }
}

// private (recursive) search method
template<typename K, typename V>
typename AVLMap<K, V>::AVLNode *AVLMap<K, V>::search(AVLMap::AVLNode *current, const K &searchKey) {
    if (current == nullptr || current->key == searchKey) {
        return current;
    }

    if (searchKey < current->key) {
        if (current->left == nullptr) {
            return current;
        }
        return search(current->left, searchKey);
    } else {
        if (current->right == nullptr) {
            return current;
        }
        return search(current->right, searchKey);
    }


}

// public search method
template<typename K, typename V>
V &AVLMap<K, V>::search(const K &searchKey) {
    return search(root, searchKey)->value;
}

// public subscript operator
template<typename K, typename V>
V &AVLMap<K, V>::operator[](const K &searchKey) {
    return search(searchKey);
}

// private (recursive) insert method
template<typename K, typename V>
void AVLMap<K, V>::insert(AVLNode *&node, const K &key, const V &value) {
    if (node == nullptr) {
        node = new AVLNode(key, value);
    }

    if (key < node->key) {
        insert(node->left, key, value);
    } else if (key > node->key) {
        insert(node->right, key, value);
    } else {
        // just throws away the current value if the key is the same.
        // not optimal but i can't find an elegant solution
        node->value = value;
    }

    // update height of current node
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // check balance factor
    int balance = getBalanceFactor(node);

    // left heavy
    if (balance > 1) {
        if (key < node->left->key) {
            node = rotateRight(node); // single right rotation
        } else {
            node->left = rotateLeft(node->left); // left-right double rotation
            node = rotateRight(node);
        }
    }

    // right Heavy
    if (balance < -1) {
        if (key > node->right->key) {
            node = rotateLeft(node); // single left rotation
        } else {
            node->right = rotateRight(node->right); // right-left double rotation
            node = rotateLeft(node);
        }
    }
}

// public insert method
template<typename K, typename V>
void AVLMap<K, V>::insert(K key, V value) {
    insert(root, key, value);
}

// copy assignment operator
template<typename K, typename V>
AVLMap<K, V> &AVLMap<K, V>::operator=(const AVLMap<K, V> &other) {
    if (this != &other) {
        clearMap(root);
        root = copyMap(other.root);
    }
    return *this;
}

// copy constructor
template<typename K, typename V>
AVLMap<K, V>::AVLMap(const AVLMap<K, V> &other) {
    root = copyMap(other.root);
}

// destructor
template<typename K, typename V>
AVLMap<K, V>::~AVLMap() {
    clearMap(root);
}

#endif //PHOTOMOSAIC_AVLMAP_H
