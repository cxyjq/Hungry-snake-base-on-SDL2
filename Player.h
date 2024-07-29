#pragma once

#include<deque>
#include"Event_Manager.h"

class Snake;
class Food;

class Player :public KeyEventObersver, public MouseEventObersver {
public:
	void onKeyEvent(SDL_Scancode keyCode, bool is_pressed)override;
	void onMouseEvent(SDL_Event& event, bool is_moved, bool is_buttoned, bool is_unbuttoned, SDL_Renderer* prender)override;
};  

bool CheckCollision(std::deque<std::pair<int, int>>& body, int x, int y);


class Snake :public KeyEventObersver{
public:
	Snake(int y,SDL_Color color);
	Snake() {};
	void onKeyEvent(SDL_Scancode keyCode, bool is_pressed)override;
	void move();
	int getScore();
	void draw(SDL_Renderer* prender);
public:
	SDL_Color snake_color;
	int direction, len,hand_x, hand_y;
	int cell_size, num_x, num_y;
	std::deque<std::pair<int, int>> body;
	bool is_alive;
};


class Food {
public:
	Food();
	static void Generate_food(std::deque<std::pair<int, int>>& snake_body);
	static void draw(SDL_Renderer* prender);
public:
	static int food_x, food_y, cell_size, num_x, num_y;
	static bool is_exist;
};
