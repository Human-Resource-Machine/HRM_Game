#include <iostream>
#include "hrm_gui.h"


int main() {

    HRM_GUI my_game;
    bool result;
    do { result = my_game.run(); } while (result);
    return 0;
}
