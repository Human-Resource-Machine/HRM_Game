#include <iostream>
#include "hrm_gui.h"
#include "body.h"
#include "debug.h"

int main() {

    HRM_GUI my_game;
    while (1) {
        my_game.run();
        cin.get();
        cin.get();

        vector<int> input{};
        vector<int> output{};
        int ground;
        vector<string> instructions{};
        read(input, output, ground, instructions);


    }

    return 0;
}
