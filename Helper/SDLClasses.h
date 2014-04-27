//
//  SDLClasses.h
//  SDLandOpenGL
//
//  Created by Kristof Niederholtmeyer on 21.11.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SDLandOpenGL__SDLClasses__
#define __SDLandOpenGL__SDLClasses__

////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include "Types.h"

////////////////////////////////////////////////////////////////////////////////

struct Sound {
    Uint32 wav_length; // length of our sample
	Uint8 *wav_buffer; // buffer containing our audio file
	SDL_AudioSpec wav_spec; // the specs of our piece of music};
};

class SDLSystem {
    std::string _resource_path;
    
    bool _sound_playing;
    int _next_sound;
    std::map<int, Sound> _sounds;
    
public:
    SDLSystem(std::string const& resource_path);
    ~SDLSystem();
    
    void set_relative_mouse_mode(bool enabled);
    bool relative_mouse_mode() const;
    void set_show_cursor(bool enabled);
    bool show_cursor() const;
    bool poll_event(SDL_Event& event);
    std::string const& resource_path() const;
    std::string load_text_file(std::string const& filename) const;
    
    int load_sound(std::string const& filename);
    void play_sound(int sound);
    void update_sound();
    bool sound_playing() const;
}; // SDLSystem

////////////////////////////////////////////////////////////////////////////////

class SDLGLWindow {
    SDL_Window *_mainwindow;
    SDL_GLContext _maincontext;
    
public:
    SDLGLWindow(SDLSystem& system, Vector<int, 2> const& size, std::string const& title = "Main Window");
    ~SDLGLWindow();
    
    void set_vsync(bool enabled);
    void swap();
    void set_fullscreen(bool enabled);
    void set_size(int width, int height);
    Vector<int, 2> size() const;
    int width() const;
    int height() const;
}; // SDLGLWindow

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__SDLandOpenGL__SDLClasses__) */

////////////////////////////////////////////////////////////////////////////////
