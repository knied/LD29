//
//  main.m
//  LD29
//
//  Created by Kristof Niederholtmeyer on 17.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "SDLClasses.h"
#include "Camera.h"
#include "DelaunayTriangulation.h"
#include "GameCore.h"
#include <time.h>

int main(int argc, const char * argv[])
{
    int const view_width = 1024;
    int const view_height = 768;
    
    SDLSystem system("resources");
    SDLGLWindow window(system, Vector<int, 2>(view_width, view_height), "Dwarven Kingdoms");
    window.set_vsync(true);
    //window.set_fullscreen(true);
    //system.set_show_cursor(false);
    
    int move_sound = system.load_sound("move.wav");
    int coin_sound = system.load_sound("coin.wav");
    int kill_sound = system.load_sound("kill.wav");
    int spawn_sound = system.load_sound("spawn.wav");
    
    GameCore* game = new GameCore(view_width, view_height);
    game->set_sounds(move_sound, coin_sound, kill_sound, spawn_sound);
    
    clock_t old_time = clock();
    
    bool done = false;
    SDL_Event event;
    while (!done) {
        if (!system.sound_playing()) {
            int sound = game->next_sound();
            if (sound >= 0) {
                system.play_sound(sound);
            }
        }
        system.update_sound();
        
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
                    game->set_sounds(move_sound, coin_sound, kill_sound, spawn_sound);
                } else {
                    MouseButton mb = MBLeft;
                    if (event.button.button != SDL_BUTTON_LEFT) {
                        mb = MBRight;
                    }
                    game->mouse_up(mb, event.button.x, event.button.y);
                }
            }
        }
        
        clock_t new_time = clock();
        game->update((float)(new_time - old_time) / (float)CLOCKS_PER_SEC);
        old_time = new_time;
        
        window.swap();
    }
    return 0;
}
