/* Your code here! */
#include "dsets.h"
void DisjointSets::addelements(int num){
    for (int i = 0; i < num; i++){
        elem_vec.push_back(-1); 
    }
}

int DisjointSets::find(int elem){
    if(elem_vec[elem] <0){
        return elem; 
    }else{
        //compress path here 
        return find(elem_vec[elem]);
    }
}

void DisjointSets::setunion(int a, int b){
    int rootA = find(a); 
    int rootB = find(b); 
    int sum = elem_vec[rootA] + elem_vec[rootB]; 

    if(elem_vec[rootA] <= elem_vec[rootB]){
        elem_vec[rootB] = rootA; 
        elem_vec[rootA] = sum;  
    }else if(elem_vec[rootA] > elem_vec[rootB]){
        elem_vec[rootA] = rootB; 
        elem_vec[rootB] = sum; 
    }
}

int DisjointSets::size(int elem){
    int temp = find(elem); 
    return -elem_vec[temp]; 
}