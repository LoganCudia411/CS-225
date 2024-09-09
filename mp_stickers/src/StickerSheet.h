
#pragma once
#include "Image.h"
#include <vector>
#include <algorithm>

using namespace std;
using namespace cs225;

class StickerSheet {
    public:
        StickerSheet(const Image & picture, unsigned max);
        StickerSheet(const StickerSheet & other);
        const StickerSheet & operator=(const StickerSheet & other);
        void changeMaxStickers (unsigned max);
        int addSticker(Image & sticker, int x, int y);
        int setStickerAtLayer (Image & sticker, unsigned layer, int x, int y);
        bool translate (unsigned index, int x, int y);
        Image * getSticker(unsigned index);
        void removeSticker(unsigned index);
        int layers () const;
        Image render () const;
        void transfer_(const StickerSheet & other);
        ~StickerSheet();

    private:
        vector <Image*> baseSheet;
        vector <int> xCoor;
        vector <int> yCoor;
        unsigned int max_val;
        
};
