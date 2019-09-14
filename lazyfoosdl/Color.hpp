//
//  Color.hpp
//  lazyfoosdl
//
//  Created by Alex Hew on 11/11/2018.
//  Copyright © 2018 alphapod. All rights reserved.
//

#ifndef Color_hpp
#define Color_hpp

#include <stdio.h>
#include <stdint.h>

class Color {
public:
    float R, G, B, A;
    Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
    uint32_t ToUInt32() const;
    Color operator + (const Color&c) const;
    Color operator - (const Color&c) const;
    Color operator * (float f) const;
};

#endif /* Color_hpp */
