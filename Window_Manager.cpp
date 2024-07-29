#pragma once


#include"Window_Manager.h"
#include"Data_Manager.h"


//SDLµÄ³õÊ¼»¯

void SDL_Wrapper::initRenderer(int index,Uint32 flag) {
	this->prender = SDL_CreateRenderer(this->pwin, index, flag);
	if (NULL == this->prender) {
		SDL_Log("SDL_CreateRenderer falied: %s", SDL_GetError());
		exit(0);
	}
}
SDL_Renderer* SDL_Wrapper::GetpRender() {
	return this->prender;
}



void SDL_Wrapper::initWindowSurface() {
	this->psurf = SDL_GetWindowSurface(this->pwin);
	if (NULL == this->psurf) {
		SDL_Log("SDL_GetWindowSurface falied: %s", SDL_GetError());
		exit(0);
	}
}
SDL_Surface* SDL_Wrapper::GetpSurf() {
	return this->psurf;
}



void SDL_Wrapper::initTexturefromSurface() {
	this->ptexture = SDL_CreateTextureFromSurface(this->prender, this->psurf);
	if (NULL == this->ptexture) {
		SDL_Log("SDL_CreateTextureFromSurface falied: %s", SDL_GetError());
		exit(0);
	}
}
SDL_Texture* SDL_Wrapper::GetpTexture() {
	return this->ptexture;
}


void SDL_Wrapper::initFonts(int psize, std::string path) {
	if (TTF_Init() < 0) {
		SDL_Log("TTF_Init falied:%s", TTF_GetError());
		exit(0);
	}
	this->pfont = TTF_OpenFont(path.c_str(), psize);
	if (NULL == this->pfont) {
		SDL_Log("TTF_OpenFont falied: %s", TTF_GetError());
		exit(0);
	}
}

SDL_Window* SDL_Wrapper::GetWindow() {
	return this->pwin;
}