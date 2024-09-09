
#include "StickerSheet.h"
#include <iostream>

using namespace std;

StickerSheet::StickerSheet(const Image & picture, unsigned max){
    xCoor.push_back(0);
    yCoor.push_back(0);
    baseSheet.push_back(new Image(picture));
    max_val = max + 1;
}

StickerSheet::StickerSheet(const StickerSheet & other) {
    transfer_(other);
}

const StickerSheet & StickerSheet::operator=(const StickerSheet & other) {
    if (this != &other) {
        xCoor.clear();
        yCoor.clear();
        delete baseSheet[0];
        baseSheet[0] = nullptr;
        baseSheet.clear();
        transfer_(other);
    }
    return *this;
}

void StickerSheet::changeMaxStickers (unsigned max){
    max_val = max + 1;
    int sheet_length = baseSheet.size();
    if (baseSheet.size() > max_val){
        xCoor.erase(xCoor.begin() + max_val,xCoor.end());
        yCoor.erase(yCoor.begin() + max_val, yCoor.end());
        baseSheet.erase(baseSheet.begin() + max_val, baseSheet.end());
    }
}

int StickerSheet::addSticker(Image & sticker, int x, int y){
    unsigned int sheet_length = baseSheet.size();
    if (sheet_length >= max_val){
        changeMaxStickers(max_val);
    }
    baseSheet.push_back(&sticker);
    xCoor.push_back(x);
    yCoor.push_back(y);
    return baseSheet.size() - 2;
}

Image * StickerSheet::getSticker(unsigned index){
    unsigned int sheet_length = baseSheet.size();
    if (index + 1 < sheet_length){
        return baseSheet[index+1];
    }
    return nullptr;
}

void StickerSheet::removeSticker(unsigned index){
    unsigned int sheet_length = baseSheet.size();
    if (index < sheet_length && index >= 0){
        baseSheet.erase(baseSheet.begin() + index + 1);
        xCoor.erase(xCoor.begin() + index + 1);
        yCoor.erase(yCoor.begin() + index + 1);
    }
}

int StickerSheet::layers () const{
    return max_val-1;
}

Image StickerSheet::render () const{
    int sheet_length = baseSheet.size();
    auto Xmin = min_element(xCoor.begin(),xCoor.end());
    auto Ymin = min_element(yCoor.begin(), yCoor.end());
    int Xmax = 0;
    int Ymax = 0;

    for (int i = 0; i < sheet_length; i++){
        int currXmax =xCoor[i] + baseSheet[i]->width();
        int currYmax = yCoor[i] + baseSheet[i]->height();
        if (currXmax > Xmax){
            Xmax = currXmax;
        }
        if (currYmax > Ymax){
            Ymax = currYmax;
        }
    }

    unsigned int renderX = (Xmax - (*Xmin));
    unsigned int renderY = (Ymax - (*Ymin));
    Image result(renderX, renderY);

    int Xoffset = abs(*Xmin);
    int Yoffset = abs(*Ymin);
    for (int i = 0; i < sheet_length; i++){
        Image * curr = baseSheet[i];
        int currX =xCoor[i] + Xoffset;
        int currY = yCoor[i] + Yoffset;
        for (unsigned int x = 0; x < curr->width(); x++){
            for (unsigned int y = 0; y < curr->height(); y++){
                HSLAPixel & currentPixel = result.getPixel(currX + x, currY + y);
                HSLAPixel & basePixel = curr->getPixel(x, y);
                if (basePixel.a != 0.0){
                    currentPixel.h = basePixel.h;
                    currentPixel.s = basePixel.s;
                    currentPixel.l = basePixel.l;
                    currentPixel.a = basePixel.a;
                }
            }
        }
    }
    return result;
}

int StickerSheet::setStickerAtLayer (Image & sticker, unsigned layer, int x, int y){
    if (layer < max_val && layer < baseSheet.size()) {
        xCoor[layer+1] = x;
        yCoor[layer+1] = y;
        baseSheet[layer+1] = &sticker;
        return layer;
    }
    return -1;
}

bool StickerSheet::translate (unsigned index, int x, int y){
    unsigned int sheet_length = baseSheet.size();
    if (index + 2 <= baseSheet.size()){
       xCoor[index+1] = x;
        yCoor[index+1] = y;
        return 1;
    }
    return 0;
}

void StickerSheet::transfer_(const StickerSheet & other){
    xCoor.push_back(0);
    yCoor.push_back(0);
    baseSheet.push_back(new Image(*other.baseSheet[0]));
    max_val = other.max_val;

    int sheet_length = other.baseSheet.size();
    for (int i = 1; i < sheet_length; i++){
        xCoor.push_back(other.xCoor[i]);
        yCoor.push_back(other.yCoor[i]);
        baseSheet.push_back(other.baseSheet[i]);
    }
}

StickerSheet::~StickerSheet(){
    xCoor.clear();
    yCoor.clear();
    delete baseSheet[0];
    baseSheet.clear();
}