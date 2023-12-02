#include <iostream>
#include <windows.h>

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    // 获取控制台缓冲区信息和当前文本属性
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    std::cout << "Welcome to Interactive Console!" << std::endl;

    while (true) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        std::cout << "Enter a command: ";
        SetConsoleTextAttribute(hConsole, saved_attributes);

        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "exit") {
            break;
        }

        // 处理其他命令
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cout << "Command entered: " << userInput << std::endl;
        SetConsoleTextAttribute(hConsole, saved_attributes);
    }

    return 0;
}
