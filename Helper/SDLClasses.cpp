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

static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

void my_audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_len ==0)
		return;
	
	len = ( len > audio_len ? audio_len : len );
	SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	//SDL_MixAudio(stream, audio_pos, len, 32);// mix from one buffer into another
	
	audio_pos += len;
	audio_len -= len;
}

SDLSystem::SDLSystem(std::string const& resource_path)
: _resource_path(resource_path + "/"), _sound_playing(false), _next_sound(0) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        throw std::runtime_error("Error: SDL_Init");
}
SDLSystem::~SDLSystem() {
    for (auto& s : _sounds) {
        SDL_FreeWAV(s.second.wav_buffer);
    }
    
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

int SDLSystem::load_sound(std::string const& filename) {
    Sound new_sound;
    if( SDL_LoadWAV((_resource_path + filename).c_str(),
                    &new_sound.wav_spec,
                    &new_sound.wav_buffer,
                    &new_sound.wav_length) == 0 ){
        return -1;
	}
    
    new_sound.wav_spec.callback = my_audio_callback;
	new_sound.wav_spec.userdata = 0;
    
    int sound_index = _next_sound++;
    _sounds.insert(std::make_pair(sound_index, new_sound));
    return sound_index;
}

void SDLSystem::play_sound(int sound) {
    auto it = _sounds.find(sound);
    if (it == _sounds.end()) {
        std::cout << "WARNING: No sound found..." << std::endl;
        return;
    }
    
    if (_sound_playing) {
        SDL_CloseAudio();
    }
    
    audio_pos = it->second.wav_buffer; // copy sound buffer
	audio_len = it->second.wav_length; // copy file length
    
    if ( SDL_OpenAudio(&it->second.wav_spec, NULL) < 0 ) {
        std::cout << "WARNING: Unable to open audio..." << std::endl;
        return;
	}
    
    SDL_PauseAudio(0);
    
    _sound_playing = true;
}

void SDLSystem::update_sound() {
    if (_sound_playing) {
        if (audio_len <= 0) {
            SDL_CloseAudio();
            _sound_playing = false;
        }
    }
}

bool SDLSystem::sound_playing() const {
    return _sound_playing;
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
