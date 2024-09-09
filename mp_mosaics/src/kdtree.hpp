
/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second, int curDim)
{   
    /**
    * @todo Implement this function!
    */

     if(curDim>= Dim){
      return false; 
    }

     if(curDim < 0){
       return false; 
     }
  if(first[curDim] == second[curDim])
    return first < second;
  return first[curDim] < second[curDim];

}

template <int Dim>
bool shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest, const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
  int currDist = 0;
  int potDist = 0;

  for(int i = 0; i < Dim; i++) {
    currDist += (currentBest[i]-target[i])*(currentBest[i]-target[i]);
    potDist += (potential[i]-target[i])*(potential[i]-target[i]);
  }

  if(potDist == currDist){
    return potential < currentBest;
  }
  else{
    return potDist < currDist;
  }
    
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    if(newPoints.empty()) {
      root = NULL;
      return;
    }

    vector<Point<Dim>> points;
    for(auto it = newPoints.begin(); it != newPoints.end(); it++) { //copy to points
      points.push_back(*it);
    }

    root = buildTree(points, 0, 0, points.size()-1);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(vector<Point<Dim>>& vec, int curDim, int left, int right) {
  if(left > right){
    return NULL;
  }
 

  int median = (left+right)/2;
  auto cmp = [curDim](const Point<Dim>& first, const Point<Dim>& second){return smallerDimVal(first, second, curDim);};

  select(vec.begin() + left, vec.begin() + right + 1, vec.begin() + median, cmp);
  KDTreeNode* subRoot = new KDTreeNode(vec[median]);
  size++;
  subRoot->right = buildTree(vec, (curDim+  1) % Dim, median +1, right);
  subRoot->left = buildTree(vec, (curDim + 1) % Dim, left, median -1 );
  return subRoot;

}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
   size = other.size; 
   copyTree(root, other.root);  
   }
  
template<int Dim>
void KDTree<Dim>::_copy(KDTreeNode* Tree1, KDTreeNode* Tree2) {
  if(Tree2==NULL)
    return;

  Tree1 = new KDTreeNode(Tree2->point);

  _copy(Tree1->left, Tree2->left);
  _copy(Tree1->right, Tree2->right);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if(this != &rhs) {
    remove(root);
    root = opHelper(rhs.root);
    size = rhs.size;
  }
  return *this;
}

template<int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::opHelper(KDTreeNode* node) {
  if(node == NULL){
    return NULL;
  }
  KDTreeNode* new_node = new KDTreeNode(node->point);
  new_node->left = opHelper(node->left);
  new_node->right = opHelper(node->right);
  return new_node;
}


template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   remove(root);
}

template<int Dim>
void KDTree<Dim>::remove(KDTreeNode*& node) {
  if(node == NULL)
    return;

  if(node->left != NULL){
    remove(node->left);
  }

  if(node->right != NULL){
    remove(node->right);
  }
  
  delete(node);
  node = NULL; 
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
     
  if(root == NULL){
    return Point<Dim>();
  }
  return neightborHelper(query, root, 0);  
}

template<int Dim>
Point<Dim> KDTree<Dim>::neightborHelper(const Point<Dim>& query, KDTreeNode* curr, int dim) const {
  
  Point<Dim> nearest;
  bool recursedLeft = false;

  if(curr->left == NULL && curr->right == NULL){
    return curr->point;
  }
    
  
  if(query == curr->point){
    return curr->point;
  }

  if(smallerDimVal(query,curr->point, dim)){
    recursedLeft = true;
    if(curr->left != NULL){
      nearest = neightborHelper(query, curr->left, (dim+1)%Dim);
    }
  }

  else {
  if(curr->right != NULL)
  nearest = neightborHelper(query, curr->right, (dim+1)%Dim);
  }
  
  if(shouldReplace(query, nearest, curr->point)){
    nearest = curr->point;
  }
 
  int rad = 0;
  for(int i = 0; i < Dim; i++) {
    rad += (query[i]-nearest[i])*(query[i]-nearest[i]);
  }

  int splitDist = (curr->point[dim] - query[dim])*(curr->point[dim] - query[dim]);

  Point<Dim> tmp = nearest;
  if(rad >= splitDist) {
    if(recursedLeft) {
      if(curr->right != NULL)
      tmp = neightborHelper(query, curr->right, (dim+1)%Dim);
    }
    else {
      if(curr->left != NULL)
      tmp = neightborHelper(query, curr->left, (dim+1)%Dim);
    }
    if(shouldReplace(query, nearest, tmp)) 
      nearest = tmp;
  }

return nearest;
}

template <typename RandIter, typename Comparator>
void select(RandIter begin, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */    
  if(begin == end){
      return;
  }
  RandIter pivot = begin;
  pivot = split(begin, end, pivot, cmp);
  if(k == pivot)
    return;
  else if(k < pivot) 
    select(begin, pivot, k, cmp);
  else 
    select(pivot+1, end, k, cmp);
}

template<typename RandIter, typename Comparator>
RandIter split(RandIter begin, RandIter end, RandIter k, Comparator cmp) {
  auto pivotValue = *k;
  swap(*k, *(end-1));
  auto storeIt = begin;
  for(auto it = begin; it < end - 1; it++) {
    if(cmp(*it,pivotValue)) { //use cmp 
      swap(*it, *storeIt);
      storeIt++;
    }
  }
swap(*storeIt, *(end - 1));
return storeIt;
}
