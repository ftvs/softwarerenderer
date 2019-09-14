//
//  Rasterizer.hpp
//  lazyfoosdl
//
//  Created by Alex Hew on 11/11/2018.
//  Copyright © 2018 alphapod. All rights reserved.
//

#ifndef Rasterizer_hpp
#define Rasterizer_hpp

#include <stdio.h>
#include <stdint.h>
#include "Color.hpp"

class Rasterizer {
protected:
    uint32_t *framebuffer;
    unsigned int width, height;
    
public:
    void setFrameBuffer(uint32_t *framebuffer, unsigned int w, unsigned int h);
    void setPixel(unsigned int x, unsigned int y, const Color &color = Color());
    void setPixel(float x, float y, const Color &color = Color());
    void clear();
    void drawLine(const Color &color1, float x1, float y1, const Color &color2, float x2, float y2);
};

#endif /* Rasterizer_hpp */
