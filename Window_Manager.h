#pragma once

#include<SDL.h>
#include<SDL_ttf.h>
#include<string>


class  SDL_Wrapper {
public:
	SDL_Wrapper(Uint32 initFlags, std::string WindowTitle, int x, int y, int w, int h, Uint32 WindowFlags) {
		if (SDL_Init(initFlags) < 0) {
			SDL_Log("SDL_Init falied: %s", SDL_GetError());
			exit(0);
		}

		if (TTF_Init() < 0) {
			SDL_Log("TTF_Init falied: %s", TTF_GetError());
			exit(0);
		}

		this->pwin = SDL_CreateWindow(WindowTitle.c_str(), x, y, w, h, WindowFlags);
		if (NULL == this->pwin) {
			SDL_Log("SDL_CreateWindow falied: %s", SDL_GetError());
			exit(0);
		}

		this->prender = NULL;
		this->psurf = NULL;
		this->ptexture = NULL;
		this->pfont = NULL;
	}

	//Renderer
	void initRenderer(int index, Uint32 flag);
	SDL_Renderer* GetpRender();

	//WindowSurface
	void initWindowSurface();
	SDL_Surface* GetpSurf();

	//Texture
	void initTexturefromSurface();
	SDL_Texture* GetpTexture();

	SDL_Window* GetWindow();

	void initFonts(int psize = 36, std::string path = "C:\\Windows\\Fonts\\simfang.ttf");
	~SDL_Wrapper() {
		if (this->pfont) {
			TTF_CloseFont(this->pfont);
		}
		if (this->ptexture) {
			SDL_DestroyTexture(this->ptexture);
		}
		if (this->psurf) {
			SDL_FreeSurface(this->psurf);
		}
		if (this->prender) {
			SDL_DestroyRenderer(this->prender);
		}
		SDL_DestroyWindow(this->pwin);
		SDL_Quit();
	}
private:
	SDL_Window* pwin;
	SDL_Renderer* prender;
	SDL_Surface* psurf;
	SDL_Texture* ptexture;
	TTF_Font* pfont;
};