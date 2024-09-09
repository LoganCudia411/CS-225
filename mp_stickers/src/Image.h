/**
 * @file Image.h
 * Contains your declaration of the interface for the Image class.
 */

#pragma once
//need need to show which folder 
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
using namespace std; 
using namespace cs225; 



class Image : public cs225 :: PNG{
public:
    Image();
    Image(unsigned int width, unsigned int height);
    void lighten(); 
    void lighten( double amount); 
    void darken(); 
    void darken(double amount); 
    void saturate(); 
    void saturate(double amount); 
    void desaturate(); 
    void desaturate(double amount); 
    void grayscale(); 
    void rotateColor(double degrees); 
    void illinify(); 
    void scale(double factor); //scale by a factor 
    void scale(unsigned w, unsigned h); //manually scale by height and width 
    
};