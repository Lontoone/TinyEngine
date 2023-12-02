#pragma once
#ifndef ENV_PATH_H
#define ENV_PATH_H
#include <string>
#include <vector>

#ifndef STRINGIFY
#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)

#endif // !STRINGIFY


using namespace std;
//const std::string SRC_FOLDER = std::string( EXPAND(_PRJ_SRC_PATH)).erase(0, 1).erase(std::string(EXPAND(_PRJ_SRC_PATH)).size() - 1);
//std::string src_folder = std::string(EXPAND(_PRJ_SRC_PATH));
static std::string GET_SRC_FOLDER(){
	std::string src_folder = std::string(EXPAND(_PRJ_SRC_PATH));
	src_folder.erase(0, 1); // erase the first quote
	src_folder.erase(src_folder.size() - 1); // erase the last quote and the dot	
	return src_folder;
};


#endif // !ENV_PATH_H
