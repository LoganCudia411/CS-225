
#include "Image.h"
using namespace std; 
using namespace cs225; 
#include <cmath>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
Image::Image() : PNG() {
    //no code needed, base class handles initialization 
}

Image :: Image(unsigned int width, unsigned int height) : PNG(width,height){
   //no code needed, initializing using base class constructor
}

void Image :: lighten(){
for(unsigned int i = 0; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        if(getPixel(i,j).l <= 0.9){ //if we are able to increment by 0.1
            getPixel(i,j).l += 0.1 ; 
        }
        else{ //since it could be 0.95 for example
            getPixel(i,j).l = 1; 
        }
    }
}
}

void Image :: lighten(double amount){
for(unsigned int i = 0; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        if(getPixel(i,j).l + amount <= 1 ){ //if we are able to increment by amount and stay in bounds
            getPixel(i,j).l += amount; 
        }
        else{ //since it could be 0.95 for example
            getPixel(i,j).l = 1; 
        }
    }
}
}


void Image :: darken(){ //get the specific pixel using the getpixel() getter in Image base class 
for(unsigned int i = 0; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        if(getPixel(i,j).l >=0.1){ //if we are able to decrement by 0.1
            getPixel(i,j).l-= 0.1; 
        }
        else{ //since it could be 0.05 for example
            getPixel(i,j).l = 0; 
        }
    }
}
}

void Image :: darken(double amount){
for(unsigned int i = 0; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        if(getPixel(i,j).l >= amount){ //if we are able to decrement by amount and stay in bounds
            getPixel(i,j).l -= amount; 
        }
        else{ //since it could be 0.05 for example
            getPixel(i,j).l = 0; 
        }
    }
}
}

void Image :: saturate(){
for(unsigned int i = 0; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        if(getPixel(i,j).s <= 0.9){ //if within bounds
            getPixel(i,j).s+= 0.1; 
        }
        else{ //cap bounds at 1
            getPixel(i,j).s= 1; 
        }
    }
}
}

void Image :: saturate(double amount){
for(unsigned int i = 0; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        if(getPixel(i,j).s + amount <= 1.0){ //if within bounds
            getPixel(i,j).s+= amount; 
        }
        else{ //cap bounds at 1
            getPixel(i,j).s = 1; 
        }
    }
}
}

void Image :: desaturate(){
for(unsigned int i = 0; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        if(getPixel(i,j).s >= 0.1){ //if within bounds
            getPixel(i,j).s-= 0.1; 
        }
        else{ //cap bounds at 1
            getPixel(i,j).s= 0; 
        }
    }
}
}

void Image :: desaturate(double amount){
for(unsigned int i = 0; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        if(getPixel(i,j).s >= amount){ //if within bounds
            getPixel(i,j).s -= amount; 
        }
        else{ //cap bounds at 1
           getPixel(i,j).s = 0; 
        }
    }
}
}

void Image :: grayscale(){//set the saturation to 0 
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j = 0; j<height(); j++){
            getPixel(i,j).s = 0; 
        }
    }
}

void Image :: rotateColor(double degrees){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j = 0; j<height(); j++){
            if(getPixel(i,j).h + degrees <=360 && getPixel(i,j).h + degrees >=0){
            getPixel(i,j).h += degrees;
            }
            else if(getPixel(i,j).h + degrees > 360){ //large positive degrees
                double new_hue = getPixel(i,j).h + degrees;
                while(new_hue > 360){
                   new_hue -= 360;
                }
                getPixel(i,j).h = new_hue; 
            }
            else{ //large negative degrees  
                double new_hue = getPixel(i,j).h + degrees;
                while(new_hue < 0){
                    new_hue += 360; 
                }
                getPixel(i,j).h = new_hue;
            }
        }
    }
}

void Image :: illinify(){
//convert each pixel to illini orange (h = 11) or illini blue (h = 216)
for(unsigned int i = 0 ; i<width(); i++){
    for(unsigned int j = 0; j<height(); j++){
        double pixel_hue = (getPixel(i,j)).h;
        int blue = pixel_hue - 216; 
        int orange = pixel_hue - 11; 
        if(abs(blue) < abs(orange)){//if distance from blue is less 
            //change to blue
            pixel_hue = 216;
        }
        else{ //change to orange 
            pixel_hue = 11; 
        }
    }
}
}

void Image :: scale(double factor){
     //create a new image with new bounds
    unsigned int new_w = width() * factor; 
    unsigned int new_h = height() * factor; 
    Image *temp = new Image; //making a copy of this exact object on heap 
    temp->resize(new_w, new_h); //resize copy 

    for(unsigned int i = 0; i<new_w; i++){
        for(unsigned int j = 0; j<new_h; j++){
            //get original pixel based off factor and automatically decides based on factor 
            temp->getPixel(i,j) = getPixel((i/factor), (j/factor)); 
        }
    }
    this->resize(new_w, new_h);

    *this = *temp; //make original equal to the copy 
    delete temp; //delete copy off heap 
}

void Image :: scale(unsigned w, unsigned h){
    double w_ratio = w/(width());
    double h_ratio = h/(height()); 
    double final_ratio; 

    if(w_ratio <= h_ratio){
        final_ratio = w_ratio; 
    }else{
        final_ratio = h_ratio; 
    }
    scale(final_ratio);
}


