#pragma once

#include<SDL.h>
#include<SDL_ttf.h>
#include<string>
#include<memory>
#include<vector>
#include<functional>
#include"Event_Manager.h"
#include"Player.h"

//class PageBase;
class Button;
class Text;
class PageManager; 
//class PatternPage;
//class BeginPage;
class PageBase;
class GamePage;

struct Fontset {
	std::string content;
	SDL_Color fg, bg;
};


class Button :public MouseEventObersver{
public:
	Button();
	void init_nextpage(PageBase* next_page, SDL_Window* pwin);
	void init_rect(SDL_Rect rect);
	void onMouseEvent(SDL_Event& event, bool is_movd, bool is_buttoned, bool is_unbuttoned, SDL_Renderer* prender);
	void draw(SDL_Texture* texture, SDL_Renderer* prender);
	void init_button_font(int psize = 36,std::string path = "C:\\Windows\\Fonts\\simfang.ttf");
	void Get_button_texture_from_text(Fontset usual,Fontset pressed, SDL_Renderer* prender);
	~Button();
public:
	SDL_Rect button_rect;

	SDL_Surface * picture_surf, * text_surf;
	SDL_Texture * button_texture[2];
	TTF_Font* button_font;
	PageBase* next_page;
};



class Text {
public:
	Text();
	void init_rect(SDL_Rect rect);
	void init_text_font(int psize = 36, std::string path = "C:\\Windows\\Fonts\\simfang.ttf");
	void Get_text_texture(Fontset fontset, SDL_Renderer* prender);
	void Delete_text_texture();
	void draw(SDL_Renderer* prender);
	~Text();
public:
	SDL_Rect text_rect;
	SDL_Surface* text_surf;
	SDL_Texture* text_texture;
	TTF_Font* text_font;
	SDL_Renderer* text_render;
};


class PageManager {
public:
	PageManager(SDL_Window * pwin,SDL_Renderer* prender);
	/*PageManager(BeginPage begin_page, PatternPage pattern_page);*/
	static void snakeSubscribe(std::vector<std::shared_ptr<Snake>>& snake);
	static void snakeUnsbscribe(std::vector<std::shared_ptr<Snake>>& snake);
	static void event_loop(SDL_Renderer* prender);
	static void Change_page(PageBase* pagebase, SDL_Renderer* prender);
public:
	static PageBase* current_page, *next_page;

	static  Keyboard_and_MouseController key_and_mousecontroller;
};



//


//Record page
class PageBase{
public:
	PageBase(int num_button, int num_text,int num_snake,bool have_snake);
	std::vector<std::shared_ptr<Button>>& Get_button();
	std::vector<std::shared_ptr<Text>>& Get_text();
	std::vector<std::shared_ptr<Snake>>& Get_snake();
	void initButton(std::string name, int font_size, std::shared_ptr<Button> button, SDL_Window* pwin, SDL_Renderer* prender, int wid_offset, int hei_offset);
	void initText(std::string name, int font_size, std::shared_ptr<Text> text, SDL_Window* pwin, SDL_Renderer* prender, int wid_offset, int hei_offset);
	void initPage(std::vector<std::string> title, std::vector<int> font_size,std::vector<int> wids,std::vector<int> heis,SDL_Window* pwin, SDL_Renderer* prender);
	void reinitPage(std::vector<std::string> title, std::vector<int> font_size, std::vector<int> wids, std::vector<int> heis, SDL_Window* pwin, SDL_Renderer* prender);
	void clearPage();
	void draw(SDL_Renderer* prender);
	void set_snake_speed(int speed);
public:
	std::vector<std::shared_ptr<Button>> button;
	std::vector<std::shared_ptr<Text>> text;
	std::vector<std::shared_ptr<Snake>> snakes;
	int num_button, num_text, num_snake;
	int snake_speed;
	bool have_snake;
};

      




//PageBase
//
//class PageBase {
//public:
//	PageBase() {};
//	virtual void draw(SDL_Renderer* prender) = 0;
//	virtual std::vector<std::shared_ptr<Button>>& Get_button() = 0;
//	virtual std::vector<std::shared_ptr<Text>>& Get_text() = 0;
//	void initButton(std::string name,std::shared_ptr<Button> button, SDL_Window* pwin, SDL_Renderer* prender, int wid_offset, int hei_offset);
//	void initText(std::string name,std::shared_ptr<Text> text, SDL_Window* pwin, SDL_Renderer* prender, int wid_offset, int hei_offset);
//	virtual ~PageBase() {};
//};
//
//
////Begin page
//
//class BeginPage :public PageBase {
//public:
//	BeginPage();
//	std::vector<std::shared_ptr<Button>>& Get_button();
//	std::vector<std::shared_ptr<Text>>& Get_text();
//	void initPage(SDL_Window * pwin, SDL_Renderer* prender);
//	void draw(SDL_Renderer* prender)override;
//public:
//	std::vector<std::shared_ptr<Button>> button;
//	std::vector<std::shared_ptr<Text>> text;
//};
//
//
//// Pattern page
//
//class PatternPage :public PageBase {
//public:
//	PatternPage();
//	std::vector<std::shared_ptr<Button>>& Get_button();
//	std::vector<std::shared_ptr<Text>>& Get_text();
//	void initPage(SDL_Window* pwin, SDL_Renderer* prender);
//	void draw(SDL_Renderer* prender)override;
//	
//public:
//	std::vector<std::shared_ptr<Button>> button;
//	std::vector<std::shared_ptr<Text>> text;
//};
