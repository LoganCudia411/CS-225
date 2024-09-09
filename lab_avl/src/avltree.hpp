/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */


using namespace std; 
template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    if(t == NULL){
        return; 
    }
    Node* node1 = t->right; 
    if(node1 == NULL){
        return; 
    }
    t->right = node1->left; 
    node1->left = t;

    t->height=max(heightOrNeg1(t->right),heightOrNeg1(t->left))+1;
    
    t=node1;
    
    node1->height=max(heightOrNeg1(node1->right),heightOrNeg1(node1->left))+1;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    
    if(t==NULL) {
        return;
    }
    Node* node1 = t->left;
    if(node1==NULL) {
    return;
    }
    t->left = node1->right;
    node1->right = t;
    t->height=max(heightOrNeg1(t->right),heightOrNeg1(t->left))+1;

    t=node1;

    node1->height=max(heightOrNeg1(node1->right),heightOrNeg1(node1->left))+1;
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)

    rotateRight(t->right); 
    rotateLeft(t); 
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{

    int bfactor = 0; 

    if(subtree==NULL){
        return; 
    }

    bfactor = (heightOrNeg1(subtree -> right))-(heightOrNeg1(subtree -> left));
    if(bfactor == - 2){
        if(heightOrNeg1(subtree->left->right)-heightOrNeg1(subtree->left->left)==-1){
            rotateRight(subtree);
        }
        else{
            rotateLeftRight(subtree);
        }
    }
    else if(bfactor == 2){
        if((heightOrNeg1(subtree->right->right)-heightOrNeg1(subtree->right->left))==1){
            rotateLeft(subtree);
        }
        else rotateRightLeft(subtree);
    }

    subtree -> height = 1 + max(heightOrNeg1(subtree -> left), heightOrNeg1(subtree -> right));

}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
     if(subtree==NULL){
        subtree=new Node(key,value);
    }

    else if(key>subtree->key){
        insert(subtree->right,key,value);
    }

    else if(key<subtree->key){
        insert(subtree->left,key,value);
    }
    rebalance(subtree);
    return;
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL){
        return;
    }
    if (key < subtree->key) {
        remove(subtree->left,key);
    } 
    else if (key > subtree->key) {
        remove(subtree->right,key);
    } 
    else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            delete subtree; 
            subtree = NULL; 
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            Node* temp = subtree ->left; 
            while(temp ->right != NULL){
                temp = temp->right; 
            }
            subtree ->value = temp ->value; 
            subtree->key = temp ->key; 
            remove(subtree->left, temp->key );
        } else {
            /* one-child remove */
            Node* l = subtree -> left; 
            Node* r = subtree ->right; 
            delete subtree; 
            if(l != NULL){
                subtree = l; 
            }else{
                subtree = r; 
            }
        }
    }
    rebalance(subtree);
}
