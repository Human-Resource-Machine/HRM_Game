#include <iostream>
#include "hrm_gui.h"


int main() {
    HRM_GUI my_game;
    bool result;
    my_game.clear_screen();
    // 欢迎界面
    int continue_last = my_game.welcome();
    my_game.clear_screen();
    // 角色创建
    if (!continue_last) {
        // 删除旧档
        my_game.record->card[my_game.record_id].valid = false;
        my_game.record->save();
        my_game.new_record();
    }
    std::cin.get();
    std::cin.sync();//清理缓冲区内容
    do {
        result = my_game.run();
    } while (result);
    return 0;
}
