/**
 * @file bloom.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

 #include "bloom.h"

BF::BF(uint64_t size, std::vector<hashFunction> hashList){

    // Your code here 
    hv=hashList;
    bv=std::vector<bool>(size);
}

BF::BF(const BF& other){
    // Your code here 
    bv = other.bv;
    hv = other.hv;

}

BF::~BF(){
    return;
}

void BF::add(const int & key){

    // Your code here
    for(hashFunction h : hv) {
        bv[h(key) % bv.size()] = true;
    }
 
}

bool BF::contains(const int& key) const{
    // Your code here 
    for (auto h : hv) {
        if (!bv[h(key) % bv.size()]) {
            return false; 
        }
    }
    return true;
}

void BF::bit_union(const BF& other){
    // Your code here 
    //perform union using this function 
     std::transform(bv.begin(), bv.end(), other.bv.begin(), bv.begin(), std::logical_or<bool>());
}

void BF::intersect(const BF& other){
    // Your code here 
     std::transform(bv.begin(), bv.end(), other.bv.begin(), bv.begin(), std::logical_and<bool>());

}

float measureFPR(std::vector<int> inList, uint64_t size, std::vector<hashFunction> hashList, unsigned max){
    // Your code here
    float temp = 0;
    float sum = 0;
    BF bf(size, hashList);
    for (auto i : inList) {
        bf.add(i);
    }
    for (unsigned int i = 0; i < max; i++) {
        if (bf.contains(i)) {
            if (std::count(inList.begin(), inList.end(), i) < 1) {
                ++sum;
                ++temp;
            } 
        } else {
            ++sum;
        }
    }

    auto quot = temp/sum; 
    return quot;

}

bool getBitFromArray(std::vector<char> bv, int index){
    // Your code here
    size_t byte = index / 8;
    size_t bit = index % 8;

    return getBitFromByte(bv[byte], bit);

}

bool getBitFromByte(char in, int index){
    // Your code here


    return (in & (1 << (7-index))) != 0;

}

