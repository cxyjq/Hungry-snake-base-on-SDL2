#pragma once

#include<SDL.h>
#include<memory>

#include"Event_Manager.h"
#include"Player.h"
#include"Data_Manager.h"
#include"Window_Manager.h"
#include"Page_Manager.h"
#include"File.h"


#undef main

SDL_Wrapper window(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER, "test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	WINDOW_WIDTH + SCORE_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

PageBase level1(1, 2, 1, true), level2(1, 2, 1, true), level3(1, 2, 1, true), death_page(2, 2, 0, false), double_players(1, 3, 2, true);
PageBase greenWin(2, 1, 0, false), blueWin(2, 1, 0, false), tie_game(2, 1, 0, false);
PageBase begin_page(2, 1, 0, false), pattern_page(3, 1, 0, false), history_page(1, 4, 0, false), level_page(4, 1, 0, false);
PageManager page_manager(window.GetWindow(), window.GetpRender());
SDL_TimerID id = -1;
File  record("record.txt");

Uint32 Snakemovecallback(Uint32 interval, void* param);
void draw_grid(SDL_Renderer* prender);
void get_food();
void reset_score();
void after_snake_death_things();
void init();
void main_loop();

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
int main(int argc,char* argv[]) {
	
	init();
	main_loop();

	return 0;
}




void after_snake_death_things() {
	SDL_Log("Game Over");
	SDL_RemoveTimer(id);
	id = -1;
	PageBase* backup = page_manager.current_page;
	if (backup->snakes.size() == 1) {
		std::string score = "Final Score:";
		int num = backup->snakes[0]->getScore();
		score += std::to_string(num);
		death_page.text[1]->Delete_text_texture();
		death_page.text[1]->Get_text_texture({ score.c_str(),{0,0,0},{255,255,255,255} }, window.GetpRender());

		int i = 0;
		std::string title;
		if (backup->snake_speed == 80)i = 2, title = "present pattern: Easy";
		else if (backup->snake_speed == 50)i = 1, title = "present pattern: Middle";
		else i = 0, title = "present pattern: Difficult";

		if (record.level[i] < num) {
			record.level[i] = num;
			record.update_score();
			record.change_history_page(history_page, window.GetWindow(), window.GetpRender());
			history_page.button[0]->next_page = &begin_page;
		}

		page_manager.Change_page(&death_page, window.GetpRender());
		backup->clearPage();
		backup->reinitPage({ "exit",title.c_str(),"present score:10"}, {10,20,20}, {WINDOW_WIDTH,WINDOW_WIDTH ,WINDOW_WIDTH}, {200,-300,0}, window.GetWindow(), window.GetpRender());
		backup->button[0]->next_page = &begin_page;
	}
	else if (backup->snakes.size() == 2) {
		if (!backup->snakes[0]->is_alive && !backup->snakes[1]->is_alive)page_manager.Change_page(&tie_game, window.GetpRender());
		else if (backup->snakes[0]->is_alive)page_manager.Change_page(&greenWin, window.GetpRender());
		else page_manager.Change_page(&blueWin, window.GetpRender());
		backup->clearPage();
		backup->reinitPage({ "Exit","double players","Green socre:0","Blue score:0" }, { 10,20,20,20 }, { WINDOW_WIDTH,WINDOW_WIDTH ,WINDOW_WIDTH ,WINDOW_WIDTH }, { 200,-300,0,200 }, window.GetWindow(), window.GetpRender());
		backup->button[0]->next_page = &begin_page;
	}
	
}

void get_food() {
	std::deque<std::pair<int, int>> body;
	body = page_manager.current_page->snakes[0]->body;
	if (page_manager.current_page->snakes.size() > 1)
		for (int i = 0; i < page_manager.current_page->snakes[1]->body.size(); i++)
			body.push_back(page_manager.current_page->snakes[1]->body[i]);
	Food::Generate_food(body);
	Food::is_exist = true;
}

void reset_score() {
	std::string score[2] = { "Green score:","Blue score:" };
	for (int i = 0; i < page_manager.current_page->snakes.size(); i++) {
		int num = page_manager.current_page->snakes[i]->getScore();
		score[i] += std::to_string(num);
		page_manager.current_page->text[i + 1]->Delete_text_texture();
		page_manager.current_page->text[i + 1]->Get_text_texture({ score[i].c_str(),{0,0,0},{255,255,255,255} }, window.GetpRender());
	}
}

void draw_grid(SDL_Renderer* prender) {
	SDL_SetRenderDrawColor(prender, 255, 255, 255, 255);
	SDL_RenderClear(prender);
	Food::draw(window.GetpRender());
	
	
	

	SDL_SetRenderDrawColor(prender, 0, 0, 0, 255);
	for (int i = 0; i <= WINDOW_WIDTH; i += WINDOW_WIDTH)
		SDL_RenderDrawLine(prender, i, 0, i, WINDOW_HEIGHT);
	for (int i = 0; i <= WINDOW_HEIGHT; i += WINDOW_HEIGHT)
		SDL_RenderDrawLine(prender, 0, i, WINDOW_WIDTH, i);
	for (int i = 0; i < page_manager.current_page->snakes.size(); i++)
		page_manager.current_page->snakes[i]->draw(window.GetpRender());
	reset_score();
	for (int i = 0; i < page_manager.current_page->text.size(); i++)
		page_manager.current_page->text[i]->draw(window.GetpRender());
	for (int i = 0; i < page_manager.current_page->button.size(); i++)
		page_manager.current_page->button[i]->draw(page_manager.current_page->button[i]->button_texture[0], window.GetpRender());
	

	SDL_RenderPresent(window.GetpRender());
	
}

Uint32 Snakemovecallback(Uint32 interval, void* param) {
	for (int i = 0; i < page_manager.current_page->snakes.size(); i++)
		page_manager.current_page->snakes[i]->move();
	if (page_manager.current_page->snakes.size() > 1)
		for (int i = 0; i < 2; i++)
			page_manager.current_page->snakes[i]->is_alive = true;
	if (page_manager.current_page->snakes.size() > 1) {
		for (int i = 0; i < page_manager.current_page->snakes.size(); i++)
			if (CheckCollision(page_manager.current_page->snakes[(i + 1) % 2]->body, page_manager.current_page->snakes[i]->hand_x, page_manager.current_page->snakes[i]->hand_y))
				page_manager.current_page->snakes[i]->is_alive = false;
	}
	return interval;
}

void init() {
	window.initRenderer(-1, 0);


	level1.initPage({ "Exit","Present pattern: Easy","Present score:10" }, { 10,20,20 }, { WINDOW_WIDTH,WINDOW_WIDTH ,WINDOW_WIDTH }, { 200,-300,0 }, window.GetWindow(), window.GetpRender());
	level2.initPage({ "Exit","Present pattern: Middle","Present score:10" }, { 10,20,20 }, { WINDOW_WIDTH,WINDOW_WIDTH ,WINDOW_WIDTH }, { 200,-300,0 }, window.GetWindow(), window.GetpRender());
	level3.initPage({ "Exit","Present pattern: Difficult","Present score:10" }, { 10,20,20 }, { WINDOW_WIDTH,WINDOW_WIDTH ,WINDOW_WIDTH }, { 200,-300,0 }, window.GetWindow(), window.GetpRender());

	level1.set_snake_speed(80);
	level2.set_snake_speed(50);
	level3.set_snake_speed(20);

	level1.button[0]->next_page = &begin_page;
	level2.button[0]->next_page = &begin_page;
	level3.button[0]->next_page = &begin_page;

	double_players.set_snake_speed(60);
	double_players.initPage({ "Exit","double players","Green socre:10","Blue score:10" }, { 10,20,20,20 }, { WINDOW_WIDTH,WINDOW_WIDTH ,WINDOW_WIDTH ,WINDOW_WIDTH }, { 200,-300,0,200 }, window.GetWindow(), window.GetpRender());
	double_players.button[0]->next_page = &begin_page;

	death_page.initPage({ "Try again","Exit","Game over","Final score:10" }, { 36,36,72,36 }, { 0,0,0,0 }, { 0,200,-200,300 }, window.GetWindow(), window.GetpRender());
	death_page.button[0]->next_page = &level_page;
	death_page.button[1]->next_page = &begin_page;


	begin_page.initPage({ "Play game", "Check record","Hungry Snake" }, { 36,36,72 }, { 0, 0, 0 }, { 0,100,0 }, window.GetWindow(), window.GetpRender());
	begin_page.button[0]->next_page = &pattern_page;
	begin_page.button[1]->next_page = &history_page;

	pattern_page.initPage({ "Single player" ,"Double players" ,"Exit" ,"Choose Pattern" }, { 36,36,36,72 }, { 0, 0, 0, 0 }, { 0,100,200,0 }, window.GetWindow(), window.GetpRender());
	pattern_page.button[0]->next_page = &level_page;
	pattern_page.button[1]->next_page = &double_players;
	pattern_page.button[2]->next_page = &begin_page;

	record.change_history_page(history_page, window.GetWindow(), window.GetpRender());
	history_page.button[0]->next_page = &begin_page;

	level_page.initPage({ "Easy level", "Middle level","Difficult level","Exit","Choose Level" }, { 36,36,36,32,72 }, { 0,0,0,0,0 }, { 0,100,200,400,0 }, window.GetWindow(), window.GetpRender());
	level_page.button[0]->next_page = &level1;
	level_page.button[1]->next_page = &level2;
	level_page.button[2]->next_page = &level3;
	level_page.button[3]->next_page = &pattern_page;

	greenWin.initPage({ "Play again","Exit","Green Snake Win" }, { 36,36,72 }, { 0,0,0 }, { 0,100,0 }, window.GetWindow(), window.GetpRender());
	blueWin.initPage({ "Play again","Exit","Blue Snake Win" }, { 36,36,72 }, { 0,0,0 }, { 0,100,0 }, window.GetWindow(), window.GetpRender());
	tie_game.initPage({ "Play again","Exit","Tie Game" }, { 36,36,72 }, { 0,0,0 }, { 0,100,0 }, window.GetWindow(), window.GetpRender());
	greenWin.button[0]->next_page = blueWin.button[0]->next_page = tie_game.button[0]->next_page = &double_players;
	greenWin.button[1]->next_page = blueWin.button[1]->next_page =tie_game.button[1]->next_page = &begin_page;

	PageManager::next_page = &begin_page;

}

void main_loop() {
	while (true) {

		page_manager.event_loop(window.GetpRender());
		if (page_manager.current_page->have_snake) {

			if (id == -1)
				id = SDL_AddTimer(page_manager.current_page->snake_speed, Snakemovecallback, NULL);
			else {
				SDL_RemoveTimer(id);
				id = SDL_AddTimer(page_manager.current_page->snake_speed, Snakemovecallback, NULL);
			}

			while (true) {
				Uint32 start = SDL_GetTicks();
				bool snake_alive = true;
				for (int i = 0; i < page_manager.current_page->snakes.size(); i++)
					snake_alive &= page_manager.current_page->snakes[i]->is_alive;

				if (!snake_alive) {
					after_snake_death_things();
					break;
				}

				if (!Food::is_exist) {
					get_food();
				}

				page_manager.key_and_mousecontroller.processEvents(window.GetpRender());
				if (!page_manager.current_page->have_snake)
					break;

				draw_grid(window.GetpRender());

				SDL_RenderPresent(window.GetpRender());
				//ясЁы
				Uint32 duration = SDL_GetTicks() - start;
				int delay = 1000 / 64.0 - duration;
				if (delay > 0)
					SDL_Delay(delay);
			}

		}
	}
}
