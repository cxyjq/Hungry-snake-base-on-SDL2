#pragma once

#include<SDL.h>
#include<unordered_map>
#include<vector>
#include<memory>
#include<iostream>

class KeyEventObersver {
public:
	virtual ~KeyEventObersver() {};
	virtual void onKeyEvent(SDL_Scancode keyCode, bool is_pressed) = 0;
};

//unsubscribe


class MouseEventObersver {
public:
	virtual ~MouseEventObersver() {};
	virtual void onMouseEvent(SDL_Event& event, bool is_movd,bool is_buttoned,bool is_unbuttoned, SDL_Renderer* prender) = 0;
public:
	static bool is_change;
};



class Keyboard_and_MouseController {
public:
	void KeyboardSubscribe(SDL_Scancode keyCode, std::shared_ptr<KeyEventObersver> observer);

	bool KeyboardUnsubscribe(SDL_Scancode keyCode, std::shared_ptr<KeyEventObersver> observer);

	void MouseSubscribe(std::shared_ptr<MouseEventObersver> observer);

	bool MouseUnsubscribe(std::shared_ptr<MouseEventObersver> observer);

	void processEvents(SDL_Renderer* prender);

private:

	void notifyObservers(SDL_Scancode keyCode, bool is_pressed);

	std::unordered_map<SDL_Scancode, std::vector<std::shared_ptr<KeyEventObersver>>> Observers;

	void notifyObservers(SDL_Event& event, bool is_moved, bool is_buttoned, bool is_unbuttoned, SDL_Renderer* prender);

	std::vector<std::shared_ptr<MouseEventObersver>> Obersvers;
};