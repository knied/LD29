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
#include <SDL2_image/SDL_image.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "Types.h"

////////////////////////////////////////////////////////////////////////////////

class SDLSystem {
    std::string _resource_path;
    
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
}; // SDLSystem

////////////////////////////////////////////////////////////////////////////////

class SDLImage {
    SDL_Surface* _surface;
    
public:
    SDLImage(SDLSystem const& system, std::string const& filename);
    ~SDLImage();
    
    int width() const;
    int height() const;
    Vector<int, 2> size() const;
    
    uint8_t const* data() const;
    
    Color4 pixel(int x, int y) const;
    Color4 pixel(Vector<int, 2> const& at) const;
}; // SDLImage

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
}; // SDLGLWindow

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__SDLandOpenGL__SDLClasses__) */

////////////////////////////////////////////////////////////////////////////////
