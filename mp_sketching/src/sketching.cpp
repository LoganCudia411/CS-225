#include "sketching.h"


using namespace std; 
using namespace cs225; 


std::vector<uint64_t> kminhash(std::vector<int> inList, unsigned k, hashFunction h){
    vector<uint64_t> returnVec(k, GLOBAL_MAX_INT);
    vector<uint64_t> unique;
    set<uint64_t> visited;

    // Sort the input vector
    // sort(inList.begin(), inList.end());

    //iterate through input list 
    for (unsigned i = 0 ; i< inList.size(); i++) {
        //apply hash
        uint64_t current_hash = h(inList[i]);
        
        //if we find a unique hash
        auto lookup = visited.find(current_hash);
        if (lookup == visited.end()) {
            unique.push_back(current_hash);
            visited.insert(current_hash);
        }
    }

    // Sort the unique has vector 
    sort(unique.begin(), unique.end());

    // Copy the k minimum hash values to the result vector
    for (unsigned i = 0; i < k; ++i) {
        if(i < unique.size()){
         returnVec[i]  = (unique[i]);
        }
    }

    // //fill the rest with Global max int macro 
    // while (returnVec.size() < k) {
    //     returnVec.push_back(GLOBAL_MAX_INT);
    // }
    return returnVec;
}




std::vector<uint64_t> khash_minhash(std::vector<int> inList, std::vector<hashFunction> hv){
    
    //initialized with the return vector with the same size of hv and with global max ints in each entry 
    std::vector<uint64_t> returnVect(hv.size(), GLOBAL_MAX_INT);

    // Iterate through the input list
    for (size_t val = 0; val < inList.size(); val++) {
        //iterate through list of hash functions
        for (size_t i = 0; i < hv.size(); ++i) {
            //aply each hash function to the current value to obtain current hash
            uint64_t current_hash = hv[i](inList[val]);
            //update current index in return vector if smaller hash is found
            if(current_hash < returnVect[i]){
                returnVect[i] = current_hash;
            }
        }
    }
    return returnVect;
}


std::vector<uint64_t> kpartition_minhash(std::vector<int> inList, int part_bits, hashFunction h){
    // Calculate the number of partitions (2^part_bits)
    // use unsigned long long and type cast 1, shift left "part_bits" amount of times
    uint64_t numPartitions = 1ULL << part_bits;

    // Initialize the return vector with maximum values
    std::vector<uint64_t> returnVect(numPartitions, GLOBAL_MAX_INT);

    // Iterate through the input list 
    for (size_t val = 0; val < inList.size(); val++) {
        // apply the hash function
        uint64_t current_hash = h(inList[val]);

        // the MSB of current hash is our partition index
        uint64_t partIndex = current_hash >> (64 - part_bits);

        //update current index if smaller hash is found
        if(current_hash<returnVect[partIndex]){
            returnVect[partIndex] = current_hash;
        }
    }
    return returnVect;
}



float minhash_jaccard(std::vector<uint64_t> mh1, std::vector<uint64_t> mh2){
 
    if(mh1.empty() || mh2.empty()){
        return 0; 
    }

    float intersection = 0; 

     for (unsigned i = 0; i < mh1.size(); i++) {
        for (unsigned j = 0; j < mh2.size(); j++) {
            if (mh1[i] == mh2[j] && (mh2[j] != GLOBAL_MAX_INT)) {
                intersection++;
                break;  
            }
        }
    }

    //find union by creating a new vector to hold unique elements 
    vector<uint64_t> unique = mh1; 
    
    //iterate through mh2 to see if we can find() similar values 
    for(const auto &val : mh2){
        // if we found a unique item 
        if(std::find(unique.begin(), unique.end(), val) == unique.end()){
            unique.push_back(val);
        }
    }

    

    float union_size = unique.size(); 
    if(union_size == 0){
        return 0; 
    }else{
        return intersection / union_size; 
    }
}

int minhash_cardinality(std::vector<uint64_t> mh, unsigned k){
  // E[mk] = k/N+1 
    
    //get E[mk]
    uint64_t Emk = mh[k-1]; 
    float normalized = (static_cast<float>(Emk)/static_cast<float>(GLOBAL_MAX_INT)); 
    return static_cast<int>(ceil(k/normalized)- 1);
}


float exact_jaccard(std::vector<int> raw1, std::vector<int> raw2){
    //convert each input to sets 
    set<int> rawSet1(raw1.begin(), raw1.end());
    set<int> rawSet2(raw2.begin(), raw2.end());
    
    if (rawSet1.empty() || rawSet2.empty()) {
        return 0;
    }
    //get intersection and store into new vector 
    vector<int> intersects;
    set_intersection(rawSet1.begin(), rawSet1.end(), rawSet2.begin(), rawSet2.end(), std::back_inserter(intersects));

    //get union and store into new vector 
    vector<int> unions(rawSet1.size() + rawSet2.size());
    //get iterator to the end of the union 
    auto endOfUnion = set_union(rawSet1.begin(), rawSet1.end(), rawSet2.begin(), rawSet2.end(), unions.begin());
    

    //calculate flaot value of union
    float numUnion = 0;
    for(auto it = unions.begin(); it != endOfUnion; ++it){
        numUnion++;
    }

    //calculate float value of intersection 
    float numIntersections = static_cast<float>(intersects.size());
    

        return numIntersections / numUnion;
    }





int exact_cardinality(std::vector<int> raw){
    set<int> rawSet;

    //iterate through vector and insert everything into a set
    //filters out duplicates since we are using set 
    for (const auto& value : raw) {
        rawSet.insert(value);
    }

    int cardinality = static_cast<int> (rawSet.size());
    return cardinality;
}



MM::MM(const cs225::PNG& input, unsigned numTiles, unsigned k, hashFunction h) {
    input_ = input; 
    numTiles_ = numTiles;
    k_ = k; 
    h_ = h; 
    width_length = ceil((double)input_.width() / numTiles_);
    height_length = ceil((double)input_.height() / numTiles_);


    //initialize a 3d min hash matrix 
    HashMat.resize(numTiles_);

    for (unsigned i = 0; i < HashMat[0].size(); i++) {
        HashMat[i].resize(numTiles_);
    }
    
    // Iterate through each tile and generate MinHash sketches
    for (unsigned x = 0; x < numTiles_; x++) {
        for (unsigned y = 0; y < numTiles_; y++) {
            // Calculate the coordinates in pixel space
            unsigned startX = x * width_length;
            unsigned startY = y * height_length;

             std::vector<int> lumvVector;

            for(int xcoord = 0; xcoord < width_length; xcoord++){
                for(int ycoord = 0; ycoord < height_length; ycoord++){
                    
                    //get current pixel for current tile 
                    unsigned pixel_xCoor = xcoord + startX;
                    unsigned pixel_yCoor = ycoord + startY; 
                    if(pixel_xCoor < input_.width() && pixel_yCoor < input_.height()){
                         int lum = input_.getPixel(pixel_xCoor, pixel_yCoor).l * 255;

                        lumvVector.push_back(lum);

                    }
                }  
            }
             //use kminhash, pass in illimunance of each pixel as an arguement 
        
            //try equal if doesnt work 
            HashMat[x][y]= kminhash(lumvVector, k_, h_);
        }
    }
    }




std::vector<uint64_t> MM::getMinHash(unsigned width, unsigned height) const {
    // 1 or 2 lines at most 

    
    return HashMat[width][height];
}


int MM::countMatchTiles(const MM & other, float threshold) const {
    int count = 0; 

    for(unsigned i = 0; i < numTiles_; i++){
        for(unsigned j = 0; j< numTiles_; j++){
            vector<uint64_t> currentTile =(this->getMinHash(i,j)); 
            vector<uint64_t> otherTile =other.getMinHash(i,j); 

            std::vector<int> currentTileInt(currentTile.begin(), currentTile.end());
            std::vector<int> otherTileInt(otherTile.begin(), otherTile.end());

            if( exact_jaccard(currentTileInt, otherTileInt) >= threshold){
                count++; 
            }
        }
    }
    return count; 
}


std::vector<std::tuple<int, int, int>> build_minhash_graph(std::vector<std::string> flist, unsigned numTiles, unsigned k, hashFunction h, float threshold){

    std::vector<std::tuple<int, int, int>> edgeList; 
    for(unsigned i = 0; i < flist.size(); i++){
        for(unsigned j = i; j < flist.size(); j++){
            if(i == j){
                continue; 
            }
            PNG file1; 
            file1.readFromFile(flist[i]);
            PNG file2; 
            file2.readFromFile(flist[j]); 


            MM mm1 = MM(file1,numTiles, k, h);
            MM mm2 = MM(file2, numTiles, k, h); 
            
            int commonTiles = mm1.countMatchTiles(mm2, threshold);
            std::tuple<int, int, int> edge = {(int)i, (int)j, commonTiles};
            edgeList.push_back(edge); 
        }        
    } 
    return edgeList; 
}


