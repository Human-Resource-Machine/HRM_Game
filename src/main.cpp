#include <iostream>
#include "hrm_gui.h"

#include "debug.h"

int main() {
    
    HRM_GUI my_game;
    while(1)
    {
        my_game.run();
        cin.get();
        cin.get();


    }

    return 0;
}
