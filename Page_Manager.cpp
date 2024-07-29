#pragma once
#include"Page_Manager.h"
#include"Data_Manager.h"

Button::Button() {
	this->button_font = NULL;
	this->button_rect = { 0,0,0,0 };
	this->button_texture[0] = NULL;
	this->button_texture[1] = NULL;
	this->next_page = NULL;
	this->picture_surf = NULL;
	this->text_surf = NULL;
}

void Button::init_nextpage(PageBase* next_page, SDL_Window* pwin) {
	this->next_page = next_page;
}

void Button::init_rect(SDL_Rect rect) {
	this->button_rect = rect;
}

void Button::onMouseEvent(SDL_Event& event, bool is_movd, bool is_buttoned, bool is_unbuttoned, SDL_Renderer* prender) {
	SDL_Point pt = { event.button.x,event.button.y };
	if (is_buttoned) {
		if(SDL_PointInRect(&pt,&this->button_rect))
			this->draw(this->button_texture[1],prender);
	}
	if (is_unbuttoned) {
		if (SDL_PointInRect(&pt, &this->button_rect)) {
			this->draw(this->button_texture[0], prender);
			PageManager::Change_page(this->next_page, prender);
			is_change = true;
		}
	}
}


void Button::draw(SDL_Texture * texture, SDL_Renderer* prender) {
	SDL_RenderCopy(prender, texture, NULL, &this->button_rect);
	SDL_RenderPresent(prender);
}



void Button::init_button_font(int psize, std::string path) {
	if (this->button_font)
		TTF_CloseFont(this->button_font);
	this->button_font = TTF_OpenFont(path.c_str(), psize);
}

void Button::Get_button_texture_from_text(Fontset usual, Fontset pressed, SDL_Renderer* prender) {
	this->init_button_font();
	this->text_surf = TTF_RenderUTF8_Shaded(this->button_font, usual.content.c_str(), usual.fg, usual.bg);
	this->button_texture[0] = SDL_CreateTextureFromSurface(prender, this->text_surf);
	SDL_FreeSurface(this->text_surf);
	this->init_button_font();
	this->text_surf = TTF_RenderUTF8_Shaded(this->button_font, pressed.content.c_str(), pressed.fg, pressed.bg);
	this->button_texture[1] = SDL_CreateTextureFromSurface(prender, this->text_surf);
}


Button::~Button() {
	if (this->picture_surf)
		SDL_FreeSurface(this->picture_surf);
	if (this->text_surf)
		SDL_FreeSurface(this->text_surf);
	for (int i = 0; i < 3; i++) {
		if (this->button_texture[i])
			SDL_DestroyTexture(this->button_texture[i]);
	}
	if (this->button_font)
		TTF_CloseFont(this->button_font);
}


//text


Text::Text() {
	this->text_font = NULL;
	this->text_rect = { 0,0,0,0 };
	this->text_render = NULL;
	this->text_surf = NULL;
	this->text_texture = NULL;
}

void Text::init_rect(SDL_Rect rect) {
	this->text_rect = rect;
}

void Text::init_text_font(int psize, std::string path) {
	this->text_font = TTF_OpenFont(path.c_str(), psize);
}
void Text::Get_text_texture(Fontset fontset, SDL_Renderer* prender) {
	this->text_surf = TTF_RenderUTF8_Shaded(this->text_font, fontset.content.c_str(), fontset.fg, fontset.bg);
	this->text_texture = SDL_CreateTextureFromSurface(prender, this->text_surf);
}

void Text::Delete_text_texture() {
	if (this->text_surf)
		SDL_FreeSurface(this->text_surf);
	if (this->text_texture)
		SDL_DestroyTexture(this->text_texture);
}

void Text::draw(SDL_Renderer* prender) {
	SDL_RenderCopy(prender, this->text_texture, NULL, &this->text_rect);
	//SDL_RenderPresent(prender);
}
Text::~Text() {
	if (this->text_font)
		TTF_CloseFont(this->text_font);
	if (this->text_surf)
		SDL_FreeSurface(this->text_surf);
	if (this->text_texture)
		SDL_DestroyTexture(this->text_texture);
	if (this->text_render)
		SDL_DestroyRenderer(this->text_render);
}




//Page_Manager


void PageManager::Change_page(PageBase* pagebase,SDL_Renderer* prender) {
	next_page = pagebase;
	PageManager::event_loop(prender);
}

PageBase* PageManager::current_page = NULL;
PageBase* PageManager::next_page = NULL;
Keyboard_and_MouseController PageManager::key_and_mousecontroller;

PageManager::PageManager(SDL_Window* pwin, SDL_Renderer* prender) {

}
//
//PageManager::PageManager(BeginPage begin_page, PatternPage pattern_page) :begin_page(begin_page), pattern_page(pattern_page) {
//	this->current_page = NULL;
//	this->next_page = &this->begin_page;
//}

void PageManager::snakeSubscribe(std::vector<std::shared_ptr<Snake>>& snake) {
	if (snake.size()) {
		for (int j = 79; j <= 82; j++)
			PageManager::key_and_mousecontroller.KeyboardSubscribe((SDL_Scancode)j, snake[0]);
	}
	if (snake.size() > 1) {
		PageManager::key_and_mousecontroller.KeyboardSubscribe(SDL_SCANCODE_A, snake[1]);
		PageManager::key_and_mousecontroller.KeyboardSubscribe(SDL_SCANCODE_D, snake[1]);
		PageManager::key_and_mousecontroller.KeyboardSubscribe(SDL_SCANCODE_S, snake[1]);
		PageManager::key_and_mousecontroller.KeyboardSubscribe(SDL_SCANCODE_W, snake[1]);
	}
	if (snake.size())PageManager::key_and_mousecontroller.KeyboardSubscribe(SDL_SCANCODE_SPACE, snake[0]);
}

void PageManager::snakeUnsbscribe(std::vector<std::shared_ptr<Snake>>& snake) {
	if (snake.size()) {
		for (int j = 79; j <= 82; j++)
			PageManager::key_and_mousecontroller.KeyboardUnsubscribe((SDL_Scancode)j, snake[0]);
	}
	if (snake.size() > 1) {
		PageManager::key_and_mousecontroller.KeyboardUnsubscribe(SDL_SCANCODE_A, snake[1]);
		PageManager::key_and_mousecontroller.KeyboardUnsubscribe(SDL_SCANCODE_D, snake[1]);
		PageManager::key_and_mousecontroller.KeyboardUnsubscribe(SDL_SCANCODE_S, snake[1]);
		PageManager::key_and_mousecontroller.KeyboardUnsubscribe(SDL_SCANCODE_W, snake[1]);
	}
	if (snake.size())PageManager::key_and_mousecontroller.KeyboardUnsubscribe(SDL_SCANCODE_SPACE, snake[0]);
}

void PageManager::event_loop(SDL_Renderer* prender) {
	std::vector<std::shared_ptr<Button>> button;
	std::vector<std::shared_ptr<Snake>> snake;
	if (PageManager::current_page && PageManager::current_page != PageManager::next_page) {
		button = PageManager::current_page->Get_button();
		for (int i = 0; i < button.size(); i++)
			PageManager::key_and_mousecontroller.MouseUnsubscribe(button[i]);
		snake = PageManager::current_page->Get_snake();
		if(snake.size())snakeUnsbscribe(snake);
	}
	if (PageManager::next_page && PageManager::next_page != PageManager::current_page) {
		
		button = PageManager::next_page->Get_button();
		for (int i = 0; i < button.size(); i++)
			PageManager::key_and_mousecontroller.MouseSubscribe(button[i]);
		snake = PageManager::next_page->Get_snake();
		if(snake.size())snakeSubscribe(snake);
		PageManager::current_page = PageManager::next_page;
		PageManager::next_page->draw(prender);
	}
	PageManager::key_and_mousecontroller.processEvents(prender);
}





//



//PageBase

PageBase::PageBase(int num_button, int num_text,int num_snake,bool have_snake) {
	for (int i = 0; i < num_button; i++)
		this->button.push_back(std::make_shared<Button>());
	for (int i = 0; i < num_text; i++)
		this->text.push_back(std::make_shared<Text>());
	SDL_Color green = { 0,255,0,255 }, blue = { 0,0,255,255 };
	if(num_snake)this->snakes.push_back(std::make_shared<Snake>(20, green));
	if (num_snake > 1)this->snakes.push_back(std::make_shared<Snake>(40, blue));
	this->num_button = num_button;
	this->num_text = num_text;
	this->num_snake = num_snake;
	this->snake_speed = 0;
	this->have_snake = have_snake;
}

std::vector<std::shared_ptr<Button>>& PageBase::Get_button() {
	return this->button;
}
std::vector<std::shared_ptr<Text>>& PageBase::Get_text() {
	return this->text;
}
std::vector<std::shared_ptr<Snake>>& PageBase::Get_snake() {
	return this->snakes;
}


void PageBase::initPage(std::vector<std::string> title, std::vector<int> font_size,std::vector<int> wids, std::vector<int> heis, SDL_Window* pwin, SDL_Renderer* prender) {
	int i = 0;
	for (; i < this->button.size(); i++)
		this->initButton(title[i].c_str(), font_size[i], this->button[i], pwin, prender, wids[i], heis[i]);
	int j = 0;
	for (; i < this->text.size() + this->button.size() && j < this->text.size(); i++,j++)
		this->initText(title[i].c_str(),font_size[i], this->text[j], pwin, prender, wids[i], heis[i]);
}

void PageBase::reinitPage(std::vector<std::string> title, std::vector<int> font_size, std::vector<int> wids, std::vector<int> heis, SDL_Window* pwin, SDL_Renderer* prender) {
	for (int i = 0; i < this->num_button; i++)
		this->button.push_back(std::make_shared<Button>());
	for (int i = 0; i < this->num_text; i++)
		this->text.push_back(std::make_shared<Text>());
	SDL_Color green = { 0,255,0,255 }, blue = { 0,0,255,255 };
	if (this->num_snake)this->snakes.push_back(std::make_shared<Snake>(20, green));
	if (this->num_snake > 1)this->snakes.push_back(std::make_shared<Snake>(40, blue));
	this->initPage(title, font_size, wids, heis, pwin, prender);
}

void PageBase::clearPage() {
	this->button.clear();
	this->text.clear();
	this->snakes.clear();
}


void PageBase::draw(SDL_Renderer* prender) {
	SDL_SetRenderDrawColor(prender, 255, 255, 255, 255);
	SDL_RenderClear(prender);
	for (int i = 0; i < this->text.size(); i++)
		this->text[i]->draw(prender);
	for (int i = 0; i < this->button.size(); i++)
		this->button[i]->draw(this->button[i]->button_texture[0], prender);
	SDL_RenderPresent(prender);

	
}


void PageBase::initButton(std::string name,int font_size,std::shared_ptr<Button> button, SDL_Window* pwin, SDL_Renderer* prender, int wid_offset, int hei_offset) {
	button->init_nextpage(NULL, pwin);
	button->init_button_font(font_size);
	button->Get_button_texture_from_text({ name.c_str(),{0,0,0},{200,200,200,255} }, { name.c_str(),{0,0,0},{50,50,50,255} }, prender);
	button->init_rect({ (WINDOW_WIDTH + SCORE_WIDTH - button->text_surf->w + wid_offset) / 2 ,
		(WINDOW_HEIGHT - button->text_surf->h + hei_offset) / 2,button->text_surf->w,button->text_surf->h });
}
void PageBase::initText(std::string name, int font_size, std::shared_ptr<Text> text, SDL_Window* pwin, SDL_Renderer* prender, int wid_offset, int hei_offset) {
	text->init_text_font(font_size);
	text->Get_text_texture({ name.c_str(),{0,0,0},{255,255,255,255} }, prender);
	text->init_rect({ (WINDOW_WIDTH + SCORE_WIDTH - text->text_surf->w + wid_offset) / 2,
		(WINDOW_HEIGHT - text->text_surf->h + hei_offset) / 5,text->text_surf->w,text->text_surf->h });
}

void PageBase::set_snake_speed(int speed) {
	this->snake_speed = speed;
}



//Page_Base
//
//void PageBase::initButton(std::string name,std::shared_ptr<Button> button,SDL_Window* pwin,SDL_Renderer* prender,int wid_offset,int hei_offset) {
//	button->init_nextpage(NULL, pwin);
//	button->init_button_font();
//	button->Get_button_texture_from_text({ name.c_str(),{0,0,0},{200,200,200,255}}, {name.c_str(),{0,0,0},{50,50,50,255}}, prender);
//	button->init_rect({ (WINDOW_WIDTH + SCORE_WIDTH - button->text_surf->w + wid_offset) / 2 ,
//		(WINDOW_HEIGHT - button->text_surf->h + hei_offset) / 2,button->text_surf->w,button->text_surf->h });
//}
//void PageBase::initText(std::string name,std::shared_ptr<Text> text, SDL_Window* pwin, SDL_Renderer* prender, int wid_offset, int hei_offset) {
//	text->init_text_font(72);
//	text->Get_text_texture({ name.c_str(),{0,0,0},{255,255,255,255}}, prender);
//	text->init_rect({ (WINDOW_WIDTH + SCORE_WIDTH - text->text_surf->w) / 2,
//		(WINDOW_HEIGHT - text->text_surf->h) / 5,text->text_surf->w,text->text_surf->h });
//}
//
//
//
////Begin page
//
//BeginPage::BeginPage() {
//	for (int i = 0; i < 2; i++)
//		this->button.push_back(std::make_shared<Button>());
//	this->text.push_back(std::make_shared<Text>());
//}
//
//
//std::vector<std::shared_ptr<Button>>& BeginPage::Get_button() {
//	return this->button;
//}
//std::vector<std::shared_ptr<Text>>& BeginPage::Get_text() {
//	return this->text;
//}
//
//
//
//void BeginPage::initPage(SDL_Window* pwin, SDL_Renderer* prender){
//
//	this->initButton("Play game", this->button[0], pwin, prender, 0, 0);
//
//	this->initButton("Check record", this->button[1], pwin, prender, 0, 100);
//
//	this->initText("Hungry Snake", this->text[0], pwin, prender, 0, 0);
//	
//}
//void BeginPage::draw(SDL_Renderer* prender) {
//	SDL_SetRenderDrawColor(prender, 255, 255, 255, 255);
//	SDL_RenderClear(prender);
//	this->text[0]->draw(prender);
//	this->button[0]->draw(this->button[0]->button_texture[0], prender);
//	this->button[1]->draw(this->button[1]->button_texture[0], prender);
//	SDL_RenderPresent(prender);
//}
//
//
////Pattern page
//
//PatternPage::PatternPage() {
//	this->text.push_back(std::make_shared<Text>());
//	for (int i = 0; i < 3; i++)
//		this->button.push_back(std::make_shared<Button>());
//
//}
//
//
//std::vector<std::shared_ptr<Button>>& PatternPage::Get_button() {
//	return this->button;
//}
//std::vector<std::shared_ptr<Text>>& PatternPage::Get_text() {
//	return this->text;
//}
//
//void PatternPage::initPage(SDL_Window* pwin, SDL_Renderer* prender) {
//
//	this->initButton("Single player", this->button[0], pwin, prender, 0, 0);
//
//	this->initButton("Double players", this->button[1], pwin, prender, 0, 100);
//	
//	this->initButton("Exit", this->button[2], pwin, prender, 0, 200);
//
//	this->initText("Choose Pattern", this->text[0], pwin, prender, 0, 0);
//
//}
//void PatternPage::draw(SDL_Renderer* prender) {
//	SDL_SetRenderDrawColor(prender, 255, 255, 255, 255);
//	SDL_RenderClear(prender);
//	this->text[0]->draw(prender);
//	this->button[0]->draw(this->button[0]->button_texture[0], prender);
//	this->button[1]->draw(this->button[1]->button_texture[0], prender);
//	this->button[2]->draw(this->button[2]->button_texture[0], prender);
//	SDL_RenderPresent(prender);
//}