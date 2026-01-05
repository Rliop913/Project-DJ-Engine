#include "PDJE_MIDI.hpp"
#include <iostream>
#include <thread>
int
main()
{
    PDJE_MIDI::MIDI root = PDJE_MIDI::MIDI();
    for (const auto &i : root.GetDevices()) {
        std::cout << "device_name: " << i.device_name << std::endl;
        std::cout << "display_name: " << i.display_name << std::endl;
        std::cout << "manufacturer: " << i.manufacturer << std::endl;
        root.Config(i);
    }

    try {

        root.Run();
    } catch (const std::exception &e) {
        std::cerr << "run failed. What: " << e.what() << std::endl;
        return -1;
    }
    while (true) {
        for (const auto &i : *root.evlog.Get()) {
            std::cout << "type: " << int(i.type) << std::endl;
            std::cout << "ch: " << int(i.ch) << std::endl;
            std::cout << "pos: " << int(i.pos) << std::endl;
            std::cout << "value: " << int(i.value) << std::endl;
            std::cout << "highres_time: " << int(i.highres_time) << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }
    return 0;
}