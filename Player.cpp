#pragma once

#include<ctime>
#include<stdlib.h>
#include<ctime>
#include<stdlib.h>
#include"Player.h"
#include"Data_Manager.h"

void Player::onKeyEvent(SDL_Scancode keyCode, bool is_pressed) {
	switch (keyCode)
	{
	case SDL_SCANCODE_UP: {
		if (is_pressed)
			std::cout << "player up" << std::endl;
		break;
	}
	case SDL_SCANCODE_DOWN: {
		if (is_pressed)
			std::cout << "player down" << std::endl;
		break;
	}
	case SDL_SCANCODE_W: {
		if (is_pressed)
		{
			std::cout << "W" << std::endl;
 		}
		break;
	}
	case SDL_SCANCODE_LEFT: {
		if (is_pressed)
			std::cout << "player left" << std::endl;
		break;
	}
	case SDL_SCANCODE_RIGHT: {
		if (is_pressed)
			std::cout << "player right" << std::endl;
		break;
	}
	default:
		break;
	}

}
void Player::onMouseEvent(SDL_Event& event, bool is_moved, bool is_buttoned, bool is_unbuttoned, SDL_Renderer* prender) {
	if (is_moved) {
		std::cout << "player montion" << std::endl;
		std::cout << "player position" << " x: " << event.motion.x << " y: " << event.motion.y << std::endl;
	}
	if (is_buttoned) {
		SDL_Log("player buttoned: button:%d clicks:%d x:%d y:%d", event.button.button, event.button.clicks, event.button.x, event.button.y);
	}
	if (is_unbuttoned) {
		SDL_Log("player unbuttoned: button:%d clicks:%d x:%d y:%d", event.button.button, event.button.clicks, event.button.x, event.button.y);
	}
}


bool CheckCollision(std::deque<std::pair<int, int>>& body, int x, int y) {
	for (auto it : body) {
		if (x == it.first && y == it.second)
			return true;
	}
	return false;
}



Snake::Snake(int y,SDL_Color color) {
	this->len = 3;
	this->cell_size = 10;
	this->num_x = WINDOW_WIDTH / this->cell_size;
	this->num_y = WINDOW_HEIGHT / this->cell_size;
	this->direction = 1;
	this->snake_color = color;
	this->is_alive = true;
	srand(time(0));
	int i = y, cnt = 3;
	this->hand_x = 20, this->hand_y = i;
	while (cnt--) {
		this->body.push_back({ 20,i-- });
	}
}

void Snake::onKeyEvent(SDL_Scancode keyCode, bool is_pressed) {
	switch (keyCode)
	{
	case SDL_SCANCODE_UP: {
		if (is_pressed && this->direction != 1 && this->direction != 3) {
			SDL_Log("UP");
			this->direction = 3;
		}
		break;
	}
	case SDL_SCANCODE_W: {
		if (is_pressed && this->direction != 1 && this->direction != 3) {
			SDL_Log("WUP");
			this->direction = 3;
		}
		break;
	}
	case SDL_SCANCODE_DOWN: {
		if (is_pressed && this->direction != 1 && this->direction != 3) {
			SDL_Log("DOWN");
			this->direction = 1;
		}
		break;
	}
	case SDL_SCANCODE_S: {
		if (is_pressed && this->direction != 1 && this->direction != 3) {
			SDL_Log("SDOWN");
			this->direction = 1;
		}
		break;
	}
	case SDL_SCANCODE_LEFT: {
		if (is_pressed && this->direction != 0 && this->direction != 2) {
			SDL_Log("LEFT");
			this->direction = 2;
		}
		break;
	}
	case SDL_SCANCODE_A: {
		if (is_pressed && this->direction != 0 && this->direction != 2) {
			SDL_Log("ALEFT");
			this->direction = 2;
		}
		break;
	}
	case SDL_SCANCODE_RIGHT: {
		if (is_pressed && this->direction != 0 && this->direction != 2) {
			SDL_Log("RIGHT");
			this->direction = 0;
		}
		break;
	}
	case SDL_SCANCODE_D: {
		if (is_pressed && this->direction != 0 && this->direction != 2) {
			SDL_Log("RIGHT");
			this->direction = 0;
		}
		break;
	}
	case SDL_SCANCODE_SPACE: {
		if (is_pressed) {
			SDL_Log("SPACE");
		}
		break;
	}
	default:
		break;
	}
}
void Snake::move() {
	int x = this->hand_x + snake_dx[this->direction];
	int y = this->hand_y + snake_dy[this->direction];
	(x < 0) ? x = this->num_x - 1 : x %= num_x;
	(y < 0) ? y = this->num_y - 1 : y %= num_y;
	if (CheckCollision(this->body, x, y)) {
		this->is_alive = false;
	}
	this->body.push_front({ x,y });
	this->hand_x = x, this->hand_y = y;
	if (Food::food_x == x && Food::food_y == y) {
		this->len++;
		Food::is_exist = false;
	}
	else {
		this->body.pop_back();
	}
}
int Snake::getScore() {
	return this->len - 3;
}
void Snake::draw(SDL_Renderer* prender) {
	SDL_SetRenderDrawColor(prender, this->snake_color.r, this->snake_color.g, this->snake_color.b, 255);
	for (auto it : this->body) {
		SDL_Rect rect = { it.first * this->cell_size,it.second * this->cell_size,this->cell_size,this->cell_size };
		SDL_RenderFillRect(prender, &rect);
	}
	SDL_SetRenderDrawColor(prender, 0, 0, 0, 255);
	SDL_Rect rect = { this->body[0].first * this->cell_size,this->body[0].second * this->cell_size,this->cell_size,this->cell_size };
	SDL_RenderFillRect(prender, &rect);
}

Food::Food() {

}
void Food::Generate_food(std::deque<std::pair<int,int>>& snake_body) {
	srand(time(0));
	int x = rand() % Food::num_x;
	int y = rand() % Food::num_y;
	while (CheckCollision(snake_body, x, y)) {
		x = rand() % Food::num_x;
		y = rand() % Food::num_y;
	}
	Food::food_x = x, Food::food_y = y;
}
void Food::draw(SDL_Renderer * prender) {
	SDL_SetRenderDrawColor(prender, 255, 0, 0, 255);
	SDL_Rect rect = { Food::food_x * Food::cell_size,Food::food_y * Food::cell_size,Food::cell_size,Food::cell_size };
	SDL_RenderFillRect(prender, &rect);
}

int Food::food_x = 15;
int Food::food_y = 15;
int Food::cell_size = 10;
int Food::num_x = WINDOW_WIDTH / Food::cell_size;
int Food::num_y = WINDOW_HEIGHT / Food::cell_size;
bool Food::is_exist = false;
