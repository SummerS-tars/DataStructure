#include <iostream>
#include "../include/MapGenerator.h"

int main(int argc, char** argv) {
    GenConfig cfg;
    cfg.room_num = 20;
    if (argc > 1) {
        try { cfg.room_num = std::max(20, std::stoi(argv[1])); } catch (...) {}
    }
    auto map = MapGenerator::generate(cfg);
    std::cout << map.to_string() << std::endl;
    std::cout << (map.is_connected() ? "Connected" : "NotConnected") << std::endl;
    return 0;
}
