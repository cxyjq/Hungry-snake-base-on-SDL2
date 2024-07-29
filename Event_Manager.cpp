#pragma once

#include<algorithm>
#include"Event_Manager.h"


bool MouseEventObersver::is_change = false;


//key and mouse

void Keyboard_and_MouseController::KeyboardSubscribe(SDL_Scancode keyCode, std::shared_ptr<KeyEventObersver> observer) {
	this->Observers[keyCode].push_back(observer);
}

bool Keyboard_and_MouseController::KeyboardUnsubscribe(SDL_Scancode keyCode, std::shared_ptr<KeyEventObersver> observer) {
	auto it = find(this->Observers[keyCode].begin(), this->Observers[keyCode].end(), observer);
	if (it != this->Observers[keyCode].end()) {
		this->Observers[keyCode].erase(it);
		return true;
	}
	else {
		SDL_Log("unsubscribe KeyboardController falied as not found the particular element");
		return false;
	}
}

void Keyboard_and_MouseController::MouseSubscribe(std::shared_ptr<MouseEventObersver> observer) {
	this->Obersvers.push_back(observer);
}

bool Keyboard_and_MouseController::MouseUnsubscribe(std::shared_ptr<MouseEventObersver> observer) {
	auto it = find(this->Obersvers.begin(), this->Obersvers.end(), observer);
	if (it != this->Obersvers.end()) {
		this->Obersvers.erase(it);
		return true;
	}
	else {
		SDL_Log("unsubscribe MouseController falied as not found the particular element");
		return false;
	}
}

void Keyboard_and_MouseController::processEvents(SDL_Renderer* prender) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			notifyObservers(event.key.keysym.scancode, event.type == SDL_KEYDOWN);
		}
		bool is_moved = false, is_buttoned = false, is_unbuttoned = false;
		if (event.type == SDL_MOUSEMOTION)
			is_moved = true;
		if (event.type == SDL_MOUSEBUTTONDOWN)
			is_buttoned = true;
		if (event.type == SDL_MOUSEBUTTONUP)
			is_unbuttoned = true;
		if (event.type == SDL_QUIT)
			exit(0);
		this->notifyObservers(event, is_moved, is_buttoned, is_unbuttoned, prender);
	}
}



void Keyboard_and_MouseController::notifyObservers(SDL_Scancode keyCode, bool is_pressed) {
	auto it = this->Observers.find(keyCode);
	if (it != this->Observers.end()) {
		for (const auto& Observer : it->second) {
			Observer->onKeyEvent(keyCode, is_pressed);
		}
	}
}


void Keyboard_and_MouseController::notifyObservers(SDL_Event& event, bool is_moved, bool is_buttoned, bool is_unbuttoned, SDL_Renderer* prender) {
	if (this->Obersvers.size()) {
		for (const auto& Obersver : this->Obersvers) {
			Obersver->onMouseEvent(event, is_moved, is_buttoned, is_unbuttoned, prender);
			if (MouseEventObersver::is_change) {
				MouseEventObersver::is_change = false;
				break;
			}
		}
	}
}