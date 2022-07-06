#include <iostream>
#include <array>
#include <deque>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <string>
#define tile_type_number 34

class Tiles {
private:
    std::array<std::string, tile_type_number> material = {"🀇", "🀈", "🀉", "🀊", "🀋", "🀌", "🀍", "🀎", "🀏", "🀙", "🀚", "🀛", "🀜", "🀝", "🀞", "🀟", "🀠", "🀡",
                                                "🀐", "🀑", "🀒", "🀓", "🀔", "🀕", "🀖", "🀗", "🀘", "🀀", "🀁", "🀂", "🀃", "🀆", "🀅", "🀄"};
    std::array<int, tile_type_number> tile;
    std::array<int, tile_type_number * 4> mountain;
    void print_hands();
    int deal();
    bool is_win(std::array<int, tile_type_number> t, int d);
    bool is_triple(std::array<int, tile_type_number> p, int eyes);
    void discard_and_get_deal(int d);
    int tilename_to_id(std::string tilename);
    int number_of_hands;
    int mountain_position;
public:
    Tiles();
    ~Tiles();
    void initialize();
    void self_practice();
};

Tiles::Tiles(/* args */) {
    initialize();
}

Tiles::~Tiles() {
}

void Tiles::initialize() {
    // 清空手牌、重洗牌山
    number_of_hands = 0;
    tile.fill(0);
    for (int i = 0; i < tile_type_number*4; i++) mountain.at(i) = i / 4;
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(mountain.begin(), mountain.end(), std::default_random_engine(seed));
    mountain_position = 0;
}

void Tiles::self_practice() {
    initialize();
    // 發手牌
    while(number_of_hands < 13) {
        tile.at(mountain.at(mountain_position++)) += 1;
        number_of_hands += 1;
    }
    // 遊戲進行
    std::cout << "game_start" << std::endl;
    while (mountain_position < tile_type_number * 4) {
        print_hands();
        int d = deal();
        std::cout << " " << material.at(d) << std::endl; 
        if (is_win(tile, d)) {
            std::cout << "自摸" << std::endl;
            return;
        };
        discard_and_get_deal(d);;
    }
    std::cout << "流局" << std::endl;
}

void Tiles::print_hands() {
    for (int i = 0; i < tile_type_number; i++) {
        for (int n = 0; n < tile.at(i); n++) std::cout << material.at(i) << ' ';
        if (i == 8 || i == 17 || i == 26) std::cout << "  ";
    }
}

int Tiles::deal() {
    return mountain.at(mountain_position++);
}

bool Tiles::is_win(std::array<int, tile_type_number> t, int d) 
{   
    t.at(d) += 1;
    for (int i = 0; i < tile_type_number; i++) {
        // find eyes
        if (t.at(i) >= 2 && is_triple(t, i)) return true;
    }
    return false;
}

bool Tiles::is_triple(std::array<int, tile_type_number> p, int eyes)
{
    p.at(eyes) -= 2;
    for (int i = 0; i < tile_type_number; i++) {
        if (p.at(i) >= 3) p.at(i) -= 3;
        if (p.at(i) > 0) {
            if ((i >= 0 && i <= 6) || (i >= 9 && i <= 15) || (i >= 18 && i <= 24)) {
                if (p.at(i + 1) >= p.at(i) && p.at(i + 2) >= p.at(i)) {
                    p.at(i + 1) -= p.at(i);
                    p.at(i + 2) -= p.at(i);
                }
                else return false;
            }
            else return false;
        }
    }
    return true;
}

void Tiles::discard_and_get_deal(int d) {
    std::cout << "輸入要打出的牌的位置(由左至右1到14): ";
    int discard_position;
    std::cin >> discard_position;
    while(discard_position < 1 || discard_position > number_of_hands + 1) {
        std::cout << "輸入錯誤(" << discard_position << ")" << std::endl;
        std::cout << "請重新輸入: ";
        std::cin >> discard_position;
    }

    if (discard_position != number_of_hands + 1) {
        int discard_id = -1;
        while (discard_position > 0) {
            discard_id += 1;
            discard_position -= tile.at(discard_id);
        }
        tile.at(discard_id) -= 1;
        tile.at(d) += 1;
    }
    else {/* discard what player gets, do nothing*/}
}

int Tiles::tilename_to_id(std::string tilename) {
    if (tilename.at(0) < '1' || tilename.at(0) > '9') return -1;
    if (tilename.at(1) != 'w' || tilename.at(1) != 'p' || tilename.at(1) != 's' || tilename.at(1) != 'z') return -1;

    int id = tilename.at(0) - '0';
    switch (tilename.at(1))
    {
    case 'm':
        id -= 1;
        break;
    case 'p':
        id += 8;
        break;
    case 's':
        id += 17;
        break;
    case 'z':
        if (tilename.at(0) <= '7') id += 26;
        else id = -1;
        break;
    default:
        id = -1;
        break;
    }
    return id;
}

int main() {
    Tiles p;
    bool ongoing = true;
    std::string command;

    while (ongoing) {
        std::cout << "初始化中..." << std::endl;
        p.initialize();
        // TODO: add more mode
        // std::cout << "請選擇模式";
        // std::cin >> command;
        p.self_practice();
        std::cout << "輸入r開啟新局，任意鍵離開: ";
        std::cin >> command;
        ongoing = (command == "r");
    }

    return 0;
}