//
//  main.m
//  LD29
//
//  Created by Kristof Niederholtmeyer on 17.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "SDLClasses.h"
#include <OpenGL/gl.h>
#include "Camera.h"
#include "DelaunayTriangulation.h"
#include <random>
#include "GameCore.h"

int main(int argc, const char * argv[])
{
    typedef std::chrono::system_clock Clock;
    typedef std::chrono::time_point<Clock> TimeStamp;
    typedef std::chrono::duration<double> Seconds;
    
    int const view_width = 1024;
    int const view_height = 768;
    
    // [[[NSBundle mainBundle] resourcePath] UTF8String]
    SDLSystem system("");
    SDLGLWindow window(system, {view_width, view_height}, "LD29");
    window.set_vsync(true);
    //window.set_fullscreen(true);
    //system.set_show_cursor(false);
    
    GameCore* game = new GameCore(view_width, view_height);
    
    TimeStamp old_time(Clock::now());
    
    bool done = false;
    SDL_Event event;
    while (!done) {
        while (system.poll_event(event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_MOUSEMOTION) {
                Vector2 cursor(event.motion.x, event.motion.y);
                cursor /= Vector2(view_width, view_height);
                cursor *= 2.0f;
                cursor -= 1.0f;
                cursor[1] *= -1.0f;
                game->mouse_moved(cursor[0], cursor[1]);
                if (event.motion.state) {
                    MouseButton mb = MBLeft;
                    if (event.button.button != SDL_BUTTON_LEFT) {
                        mb = MBRight;
                    }
                    game->mouse_dragged(mb, event.motion.xrel, event.motion.yrel);
                }
            } else if (event.type == SDL_MOUSEWHEEL) {
                game->mouse_wheel(event.wheel.y);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                MouseButton mb = MBLeft;
                if (event.button.button != SDL_BUTTON_LEFT) {
                    mb = MBRight;
                }
                game->mouse_down(mb, event.button.x, event.button.y);
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                if (game->game_over() && event.button.button == SDL_BUTTON_LEFT) {
                    delete game;
                    game = new GameCore(view_width, view_height);
                } else {
                    MouseButton mb = MBLeft;
                    if (event.button.button != SDL_BUTTON_LEFT) {
                        mb = MBRight;
                    }
                    game->mouse_up(mb, event.button.x, event.button.y);
                }
            }
        }
        
        TimeStamp new_time(Clock::now());
        Seconds elapsed = new_time - old_time;
        game->update(elapsed.count());
        old_time = new_time;
        
        window.swap();
    }
    return 0;
}
