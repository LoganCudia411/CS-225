#include "cs225/PNG.h"
#include <list>
#include <iostream>

#include "colorPicker/ColorPicker.h"
#include "ImageTraversal.h"

#include "Point.h"
#include "Animation.h"
#include "FloodFilledImage.h"

using namespace cs225;
using namespace Traversals; 

/**
 * Constructs a new instance of a FloodFilledImage with a image `png`.
 * 
 * @param png The starting image of a FloodFilledImage
 */
FloodFilledImage::FloodFilledImage(const PNG & png) {
  /** @todo [Part 2] */
  curr_png = png; 
}

/**
 * Adds a FloodFill operation to the FloodFillImage.  This function must store the operation,
 * which will be used by `animate`.
 * 
 * @param traversal ImageTraversal used for this FloodFill operation.
 * @param colorPicker ColorPicker used for this FloodFill operation.
 */
void FloodFilledImage::addFloodFill(Traversals::ImageTraversal & traversal, ColorPicker & colorPicker) {
  /** @todo [Part 2] */
  curr_traversal.push_back(&traversal); 
  curr_color.push_back(&colorPicker);
}

/**
 * Creates an Animation of frames from the FloodFill operations added to this object.
 * 
 * Each FloodFill operation added by `addFloodFill` is executed based on the order
 * the operation was added.  This is done by:
 * 1. Visiting pixels within the image based on the order provided by the ImageTraversal iterator and
 * 2. Updating each pixel to a new color based on the ColorPicker
 * 
 * While applying the FloodFill to the image, an Animation is created by saving the image
 * after every `frameInterval` pixels are filled.  To ensure a smooth Animation, the first
 * frame is always the starting image and the final frame is always the finished image.
 * 
 * (For example, if `frameInterval` is `4` the frames are:
 *   - The initial frame
 *   - Then after the 4th pixel has been filled
 *   - Then after the 8th pixel has been filled
 *   - ...
 *   - The final frame, after all pixels have been filed)
 * @param frameInterval how often to save frames of the animation
 */ 
Animation FloodFilledImage::animate(unsigned frameInterval) const {
  /** @todo [Part 2] */
  Animation animation;

  PNG temp = PNG(curr_png);
  animation.addFrame(temp);


  for(unsigned int i = 0; i < curr_traversal.size(); i++){

    Traversals:: ImageTraversal* temp_traversal = curr_traversal[i]; 
    ColorPicker * temp_color = curr_color[i]; 
    unsigned int count = 0; 

    for(auto it = temp_traversal->begin(); it != temp_traversal->end(); ++it){
      temp.getPixel( (*it).x , (*it).y ) = temp_color->getColor ( (*it).x , (*it).y ); 
      count++; 
      if(count == frameInterval){
        animation.addFrame(temp);
        count = 0; 
      }
    }
  }

  animation.addFrame(temp); 
  return animation; 
}
