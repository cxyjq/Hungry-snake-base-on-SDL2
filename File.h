#pragma once

#include<SDL.h>
#include<SDL_ttf.h>
#include<vector>
#include<string>
#include<fstream>
#include"Page_Manager.h"

class File {
public:
	File(std::string filename);
	std::vector<std::string> read_file();
	void write_file(std::vector<std::string> record);
	void change_history_page(PageBase& history_page, SDL_Window* pwin, SDL_Renderer* prender);
	void update_score();
	~File();
public:
	int level[3];
	std::string file_name;
	std::ofstream out_file;
	std::ifstream in_file;
};			 