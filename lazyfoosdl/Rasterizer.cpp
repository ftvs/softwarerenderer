//
//  Rasterizer.cpp
//  lazyfoosdl
//
//  Created by Alex Hew on 11/11/2018.
//  Copyright © 2018 alphapod. All rights reserved.
//

#include "Rasterizer.hpp"
#include <cstring>
#include <cmath>

void Rasterizer::setFrameBuffer(uint32_t *framebuffer, unsigned int w, unsigned int h)
{
    this->framebuffer = framebuffer;
    width = w; height = h;
}

void Rasterizer::setPixel(unsigned int x, unsigned int y, const Color &color)
{
    if (x >= width || y >= height) { return; }
    
    framebuffer[x + y * width] = color.ToUInt32();
}

void Rasterizer::setPixel(float x, float y, const Color &color)
{
    if (x >= width || y >= height) { return; }
    
    setPixel((unsigned int)x, (unsigned int)y, color);
}

void Rasterizer::clear()
{
    memset(framebuffer, 0, sizeof(uint32_t) * width * height);
}

void Rasterizer::drawLine(const Color &color1, float x1, float y1, const Color &color2, float x2, float y2)
{
    float xdiff = (x2 - x1);
    float ydiff = (y2 - y1);
    
    // line is actually a dot. no distance
    if (xdiff == 0.0f && ydiff == 0.0f)
    {
        setPixel(x1, x2, color1);
        return;
    }
    
    if (fabs(xdiff) > fabs(ydiff))
    {
        float xmin, xmax;
        
        if (x1 < x2)
        {
            xmin = x1; xmax = x2;
        }
        else
        {
            xmin = x2; xmax = x1;
        }
        
        float slope = ydiff / xdiff;
        
        for (float x = xmin; x <= xmax; x +=1.0f)
        {
            float y = (x - x1) * slope + y1;
            setPixel(x, y, color1 + ((color2 - color1) * ((x - x1) / xdiff)));
        }
    }
    else
    {
        float ymin, ymax;
        
        if (y1 < y2)
        {
            ymin = y1; ymax = y2;
        }
        else
        {
            ymin = y2; ymax = y1;
        }
        
        float slope = xdiff / ydiff;
        
        for (float y = ymin; y <= ymax; y+= 1.0f)
        {
            float x = x1 + ((y - y1) * slope);
            setPixel(x, y, color1 + ((color2 - color1) * ((y - y1) / ydiff)));
        }
    }
}
