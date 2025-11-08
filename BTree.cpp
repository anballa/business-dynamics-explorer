#include "BTree.h"
#include <iostream>
#include <algorithm>
using namespace std;

BTreeNode::BTreeNode(int _t, bool _leaf) {
    t = _t;
    leaf = _leaf;
}

int BTreeNode::findKey(const string &key) {
    int idx = 0;
    while (idx < (int)keys.size() && keys[idx] < key)
        ++idx;
    return idx;
}

void BTreeNode::remove(const string &key) {
    int idx = findKey(key);

    if (idx < (int)keys.size() && keys[idx] == key) {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    } else {
        if (leaf) {
            cout << "The key " << key << " is not present in the tree.\n";
            return;
        }

        bool flag = ((idx == (int)keys.size()) ? true : false);
        if ((int)children[idx]->keys.size() < t)
            fill(idx);

        if (flag && idx > (int)keys.size())
            children[idx - 1]->remove(key);
        else
            children[idx]->remove(key);
    }
}

void BTreeNode::removeFromLeaf(int idx) {
    keys.erase(keys.begin() + idx);
    values.erase(values.begin() + idx);
}

void BTreeNode::removeFromNonLeaf(int idx) {
    string k = keys[idx];

    if ((int)children[idx]->keys.size() >= t) {
        string predKey = getPredecessor(idx);
        Record predVal = children[idx]->values.back();
        keys[idx] = predKey;
        values[idx] = predVal;
        children[idx]->remove(predKey);
    } else if ((int)children[idx + 1]->keys.size() >= t) {
        string succKey = getSuccessor(idx);
        Record succVal = children[idx + 1]->values.front();
        keys[idx] = succKey;
        values[idx] = succVal;
        children[idx + 1]->remove(succKey);
    } else {
        merge(idx);
        children[idx]->remove(k);
    }
}

string BTreeNode::getPredecessor(int idx) {
    BTreeNode* cur = children[idx];
    while (!cur->leaf)
        cur = cur->children.back();
    return cur->keys.back();
}

string BTreeNode::getSuccessor(int idx) {
    BTreeNode* cur = children[idx + 1];
    while (!cur->leaf)
        cur = cur->children.front();
    return cur->keys.front();
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && (int)children[idx - 1]->keys.size() >= t)
        borrowFromPrev(idx);
    else if (idx != (int)keys.size() && (int)children[idx + 1]->keys.size() >= t)
        borrowFromNext(idx);
    else {
        if (idx != (int)keys.size())
            merge(idx);
        else
            merge(idx - 1);
    }
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx - 1];

    child->keys.insert(child->keys.begin(), keys[idx - 1]);
    child->values.insert(child->values.begin(), values[idx - 1]);

    if (!child->leaf)
        child->children.insert(child->children.begin(), sibling->children.back());

    keys[idx - 1] = sibling->keys.back();
    values[idx - 1] = sibling->values.back();

    sibling->keys.pop_back();
    sibling->values.pop_back();
    if (!sibling->leaf)
        sibling->children.pop_back();
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];

    child->keys.push_back(keys[idx]);
    child->values.push_back(values[idx]);

    if (!child->leaf)
        child->children.push_back(sibling->children.front());

    keys[idx] = sibling->keys.front();
    values[idx] = sibling->values.front();

    sibling->keys.erase(sibling->keys.begin());
    sibling->values.erase(sibling->values.begin());
    if (!sibling->leaf)
        sibling->children.erase(sibling->children.begin());
}

void BTreeNode::merge(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];

    child->keys.push_back(keys[idx]);
    child->values.push_back(values[idx]);

    for (int i = 0; i < (int)sibling->keys.size(); ++i) {
        child->keys.push_back(sibling->keys[i]);
        child->values.push_back(sibling->values[i]);
    }
    if (!child->leaf) {
        for (auto c : sibling->children)
            child->children.push_back(c);
    }

    keys.erase(keys.begin() + idx);
    values.erase(values.begin() + idx);
    children.erase(children.begin() + idx + 1);

    delete sibling;
}

BTree::BTree(int _t) {
    root = nullptr;
    t = _t;
}

void BTree::remove(const string &key) {
    if (!root) return;
    root->remove(key);
    if (root->keys.empty()) {
        BTreeNode* tmp = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->children[0];
        delete tmp;
    }
}

void BTreeNode::insertNonFull(const string& key, const Record& value) {
    int i = (int)keys.size() - 1;

    if (leaf) {
        while (i >= 0 && keys[i] > key)
            i--;
        keys.insert(keys.begin() + i + 1, key);
        values.insert(values.begin() + i + 1, value);
    } else {
        while (i >= 0 && keys[i] > key)
            i--;
        i++;
        if ((int)children[i]->keys.size() == 2 * t - 1) {
            splitChild(i, children[i]);
            if (keys[i] < key)
                i++;
        }
        children[i]->insertNonFull(key, value);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->t, y->leaf);

    for (int j = 0; j < t - 1; j++) {
        z->keys.push_back(y->keys[j + t]);
        z->values.push_back(y->values[j + t]);
    }

    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->children.push_back(y->children[j + t]);
    }

    y->keys.resize(t - 1);
    y->values.resize(t - 1);
    if (!y->leaf)
        y->children.resize(t);

    children.insert(children.begin() + i + 1, z);
    keys.insert(keys.begin() + i, y->keys[t - 1]);
    values.insert(values.begin() + i, y->values[t - 1]);
}

void BTree::insert(const string& key, const Record& value) {
    if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->keys.push_back(key);
        root->values.push_back(value);
    } else {
        if ((int)root->keys.size() == 2 * t - 1) {
            BTreeNode* s = new BTreeNode(t, false);
            s->children.push_back(root);
            s->splitChild(0, root);
            int i = 0;
            if (s->keys[0] < key)
                i++;
            s->children[i]->insertNonFull(key, value);
            root = s;
        } else {
            root->insertNonFull(key, value);
        }
    }
}

BTreeNode* BTreeNode::search(const string& key) {
    int i = 0;
    while (i < (int)keys.size() && key > keys[i])
        i++;
    if (i < (int)keys.size() && keys[i] == key)
        return this;
    if (leaf)
        return nullptr;
    return children[i]->search(key);
}

Record* BTree::search(const string& key) {
    if (root == nullptr) return nullptr;
    BTreeNode* node = root->search(key);
    if (node == nullptr) return nullptr;
    for (size_t i = 0; i < node->keys.size(); i++) {
        if (node->keys[i] == key)
            return &node->values[i];
    }
    return nullptr;
}

void BTreeNode::collectPrefix(const string& prefix, vector<pair<string, Record>>& results) {
    for (size_t i = 0; i < keys.size(); i++) {
        if (keys[i].substr(0, prefix.length()) == prefix) {
            results.push_back({keys[i], values[i]});
        }
    }
    

    if (!leaf) {
        for (size_t i = 0; i < children.size(); i++) {
            children[i]->collectPrefix(prefix, results);
        }
    }
}

vector<pair<string, Record>> BTree::searchPrefix(const string& prefix) {
    vector<pair<string, Record>> results;
    if (root == nullptr) return results;
    root->collectPrefix(prefix, results);
    return results;
}

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < (int)keys.size(); i++) {
        if (!leaf) {
            children[i]->traverse();
        }
        cout << " " << keys[i];
    }
    if (!leaf) {
        children[i]->traverse();
    }
}

void BTree::traverse() {
    if (root != nullptr) {
        root->traverse();
    }
}
