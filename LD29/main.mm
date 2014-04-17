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
    
    // setup a triangle
    std::vector<Vector3> vertices {
        Vector3(-100.0f, -100.0f, 0.0f),
        Vector3(100.0f, -100.0f, 0.0f),
        Vector3(0.0f, 100.0f, 0.0f)
    };
    
    GLBuffer buffer;
    {
        GLBindBuffer bound_buffer(buffer);
        bound_buffer.set(0, 3, GL_STATIC_DRAW, vertices);
    }
    
    GLVertexArray vertex_array;
    {
        GLBindVertexArray bound_vertex_array(vertex_array);
        bound_vertex_array.set(buffer, { 0, 0, false });
    }
    
    GLProgram program;
    program.load_shaders(system.load_text_file("basic.vert"), system.load_text_file("basic.frag"), {}, {});
    GLint MVP_location = program.uniform_location("MVP");
    
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
        
        {
            GLBindProgram bound_program(program);
            bound_program.set_uniform(MVP_location, orthogonal_projection(window.width(), window.height(), 0.0f, 1.0f));
            GLBindVertexArray bound_vertex_array(vertex_array);
            bound_vertex_array.draw(GL_TRIANGLES);
            
        }
        
        window.swap();
    }
    
    return 0;
}
