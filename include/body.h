//
// Created by fleo on 2023/12/3.
//

#ifndef GAME_BODY_H
#define GAME_BODY_H

#include <iostream> //读写io c++标准库
#include <fstream> //读写文件 c++标准库
#include <string> //字符串类 c++标准库
#include <sstream> //字符串流 c++标准库
#include "json.h" //jsoncpp的头文件

using namespace std;

Json::Value readJsonFile(const string &filename);

using namespace std;

void read(std::vector<int> &input,
          std::vector<int> &output,
          int &ground,
          std::vector<std::string> &instructions);

#endif //GAME_BODY_H
