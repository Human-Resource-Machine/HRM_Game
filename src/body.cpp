#include <iostream> //读写io c++标准库
#include <fstream> //读写文件 c++标准库
#include <string> //字符串类 c++标准库
#include <sstream> //字符串流 c++标准库
#include "json.h" //jsoncpp的头文件

using namespace std;

Json::Value readJsonFile(const string &filename) {
    //1.打开文件
    ifstream ifile;
    ifile.open(filename);

    //2.创建json读工厂对象
    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会编程\uxxx

    //3.创建json对象，等会要返回这个对象
    Json::Value root;

    //4.把文件转变为json对象，要用到上面的三个变量,数据写入root
    string strerr;
    bool ok = Json::parseFromStream(ReaderBuilder, ifile, &root, &strerr);
    if (!ok) {
        cerr << "json解析错误";
    }
    //5.返回存有数据的json，这个json对象已经能用了
    return root;
}

void read(std::vector<int> &input,
          std::vector<int> &output,
          int &ground,
          std::vector<std::string> &instructions) {
    using std::vector;
    using std::string;
    input.clear();
    output.clear();
    instructions.clear();
    auto root = readJsonFile("level/level1.json");
    ground = root["ground"].asInt();
    for (const auto &i: root["instructions"]) {
        auto s = i.asString();
        instructions.push_back(s);
    }
    for (const auto &i: root["input"]) {
        auto s = i.asInt();
        input.push_back(s);
    }
    for (const auto &i: root["output"]) {
        auto s = i.asInt();
        output.push_back(s);
    }
}

void level(){

}