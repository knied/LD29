//
//  SDLClasses.cpp
//  SDLandOpenGL
//
//  Created by Kristof Niederholtmeyer on 21.11.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "SDLClasses.h"
#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

SDLSystem::SDLSystem(std::string const& resource_path)
: _resource_path(resource_path + "/") {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) throw std::runtime_error("Error: SDL_Init");
}
SDLSystem::~SDLSystem() {
    SDL_Quit();
}

void SDLSystem::set_relative_mouse_mode(bool enabled) {
    SDL_SetRelativeMouseMode( enabled ? SDL_TRUE : SDL_FALSE );
}
bool SDLSystem::relative_mouse_mode() const {
    return SDL_GetRelativeMouseMode() == SDL_TRUE ? true : false;
}
void SDLSystem::set_show_cursor(bool enabled) {
    SDL_ShowCursor( enabled ? 1 : 0 );
}
bool SDLSystem::show_cursor() const {
    return SDL_ShowCursor(-1) == 1 ? true : false;
}

bool SDLSystem::poll_event(SDL_Event& event) {
    return SDL_PollEvent(&event);
}

std::string const& SDLSystem::resource_path() const {
    return _resource_path;
}

std::string SDLSystem::load_text_file(std::string const& filename) const {
    std::ifstream file(_resource_path + filename, std::ios::in | std::ios::binary);
    if (file)
    {
        std::string content;
        file.seekg(0, std::ios::end);
        content.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&content[0], content.size());
        file.close();
        return content;
    }
    throw std::runtime_error("Error: Unable to load file: " + _resource_path + filename);
    return "";
}

////////////////////////////////////////////////////////////////////////////////

SDLGLWindow::SDLGLWindow(SDLSystem& system, Vector<int, 2> const& size, std::string const& title) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,16);
    
    _mainwindow = SDL_CreateWindow(title.c_str(),
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   size[0], size[1],
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!_mainwindow) throw std::runtime_error("GLWindow: SDL_CreateWindow");
    
    _maincontext = SDL_GL_CreateContext(_mainwindow);
    if (!_maincontext) throw std::runtime_error("GLWindow: SDL_GL_CreateContext");
}

SDLGLWindow::~SDLGLWindow() {
    SDL_GL_DeleteContext(_maincontext);
    SDL_DestroyWindow(_mainwindow);
}

void SDLGLWindow::set_vsync(bool enabled) {
    SDL_GL_SetSwapInterval( enabled ? 1 : 0 );
}

void SDLGLWindow::swap() {
    SDL_GL_SwapWindow(_mainwindow);
}

void SDLGLWindow::set_fullscreen(bool enabled) {
    SDL_SetWindowFullscreen(_mainwindow, enabled ? 1 : 0);
}

void SDLGLWindow::set_size(int width, int height) {
    SDL_SetWindowSize(_mainwindow, width, height);
}

Vector<int, 2> SDLGLWindow::size() const {
    Vector<int, 2> size;
    SDL_GetWindowSize(_mainwindow, &size[0], &size[1]);
    return size;
}

int SDLGLWindow::width() const {
    return size()[0];
}

int SDLGLWindow::height() const {
    return size()[1];
}

////////////////////////////////////////////////////////////////////////////////
