#pragma once

#include<algorithm>
#include"File.h"



File::File(std::string filename) {

	this->file_name = filename;
	std::vector<std::string> content = this->read_file();
	for(int i = 0;i < content.size();i++)
		for(int j = 0;j < content[i].size();j++)
			if (content[i][j] >= '0' && content[i][j] <= '9') {
				this->level[i] = content[i][j] - '0';
				break;
			}
}

std::vector<std::string>File::read_file() {

	this->in_file.open(this->file_name, std::ios::in);
	if (!this->in_file.is_open()) {
		write_file({ "Difficult level: 0 points","Middle level: 0 points","Easy level: 0 points" });
		this->in_file.open(this->file_name, std::ios::in);
	}
	
	if (!this->in_file.is_open()) {
		SDL_Log("can not read the file");
	}

	std::vector<std::string> content;
	std::string name;
	while (std::getline(this->in_file,name)) {
		content.push_back(name);
	}
	this->in_file.close();
	return content;

	
}
void File::write_file(std::vector<std::string> content) {

	this->out_file.open(this->file_name, std::ios::out);
	if (!this->out_file.is_open()) {
		SDL_Log("out_file open falied");
		exit(0);
	}

	for (int i = 0; i < content.size(); i++)
		this->out_file << content[i] << std::endl;

	this->out_file.close();
}

void File::update_score() {
	std::string d = "Difficult level: " + std::to_string(level[0]) + " points";
	std::string m = "Middle level: " + std::to_string(level[1]) + " points";
	std::string e = "Easy level: " + std::to_string(level[2]) + " points";
	this->write_file({ d,m,e });
}

void File::change_history_page(PageBase& history_page,SDL_Window* pwin,SDL_Renderer* prender) {
	std::vector<std::string> content = this->read_file();
	if (content.empty()) {
		content.push_back("Difficult level: 0 points");
		content.push_back("Middle level: 0 points");
		content.push_back("Easy level: 0 points");
	}
	content.push_back("All-time high score");
	content.push_back("exit");
	std::reverse(content.begin(), content.end());
	history_page.initPage( content, { 16,60,28,28,28 }, { 0,0,0,0,0 }, { 400,-200,400,800,1200 }, pwin, prender);
}

File::~File() {
	if(this->out_file.is_open())
		this->out_file.close();
	if(this->in_file.is_open())
		this->in_file.close();
}