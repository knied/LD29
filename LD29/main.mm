//
//  main.m
//  LD29
//
//  Created by Kristof Niederholtmeyer on 17.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include <Cocoa/Cocoa.h>
#include "SDLClasses.h"
#include "GLClasses.h"

int main(int argc, const char * argv[])
{
    SDLSystem system([[[NSBundle mainBundle] resourcePath] UTF8String]);
    SDLGLWindow window(system, {1024, 768}, "LD29");
    window.set_vsync(true);
    
    bool done = false;
    SDL_Event event;
    while (!done) {
        while (system.poll_event(event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
        
        glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        window.swap();
    }
    
    return 0;
}
