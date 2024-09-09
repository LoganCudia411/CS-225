/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    auto result = find(root,key);

    if (result == NULL){
        return V(); 
    }else{
        return result->value; 
    }
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    if(subtree == NULL){
        return subtree;
    }

    if(subtree->key == key){
        return subtree; 
    }
    
    if(key < subtree->key){
        return find(subtree->left, key);
    }else{
        return find(subtree->right, key);
    }
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    auto nodeExist = find(key);

    if(nodeExist != V()){
        return; 
    }
    insert(root, key, value); 

}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    if(subtree == NULL){
        subtree = new Node(key, value); 
        return; 
    }
    if(key< subtree->key){
        insert(subtree->left, key, value);
    }
    if(key > subtree->key){
        insert(subtree->right, key, value);
    }
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    K key_ = first->key; 
    V value_ = first->value; 
    first->key = second->key; 
    first->value = second->value;
    second->key = key_; 
    second->value = value_; 
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    remove(root,key); 
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    Node *& cur = find(subtree,key); 
    if(cur == NULL){
        return; 
    }else if(cur->right == NULL && cur->right == NULL){
        delete cur; 
        cur = NULL; 
    }else if(cur->left == NULL){
        Node * tempNode = cur; 
        cur = cur->right;
        delete tempNode; 
    }else if(cur->right == NULL){
        Node * tempNode = cur; 
        cur = cur->left;
        delete tempNode; 
    }else{
        Node * prev = cur->right; 
        while(prev->left != NULL){
            prev = prev ->left;
        }
        swap(cur, prev); 
        remove(cur->right,key);
    }
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    BST<K,V> newBST; 
    for(const auto &pair : inList){
        newBST.insert(pair.first, pair.second); 
    }
    return newBST; 
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    int listSize = inList.size(); 
    std::vector<int> height(listSize,0); 

    std::vector<std::pair<K,V>> tmpList = inList; 
    std::sort(tmpList.begin(), tmpList.end()); 

    do{
        BST<K, V> newBST = listBuild(tmpList);
        int h = newBST.height();
        height[h]++;
    } while (std::next_permutation(tmpList.begin(), tmpList.end()));
    return height; 
}