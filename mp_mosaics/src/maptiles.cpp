/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */


    //create pointer to a new canvas based of the source images size
    MosaicCanvas * newCanvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns()); 

    //make new map and avg for corresponding canvas 
    map<Point<3> , TileImage*> newMap; 
    vector<Point<3>> avg; 

    for(unsigned int i = 0; i< theTiles.size(); i++){
        auto xyz_val = convertToXYZ(theTiles[i].getAverageColor()); 
        avg.push_back(xyz_val);

        newMap.insert(pair<Point<3>, TileImage*>(xyz_val, &(theTiles[i])));
    }

    KDTree<3> *newTree = new KDTree(avg);

    //iterate through  height and columns of source image to map the 
    //closest color to the specific pixel 
    for(int i = 0; i< theSource.getRows(); i++){
        for(int j = 0; j< theSource.getColumns(); j++){

            //collect color 
            Point<3> color = convertToXYZ(theSource.getRegionColor(i,j)); 

            //calculate closet color using Fnn function 
            Point<3> nearColor = newTree->findNearestNeighbor(color);

            //set the tile in the canvas to the closest color 
            newCanvas->setTile(i, j, newMap[nearColor]);

        }
    }
    return newCanvas; 
}

