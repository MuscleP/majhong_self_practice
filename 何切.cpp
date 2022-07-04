#include <iostream>
#include <array>
#include <deque>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <string>
#define pile_type_number 34

class Piles
{
private:
    std::array<int, pile_type_number> pile;
    std::array<int, pile_type_number * 4> mountain;
    void output_pile();
    bool is_win();
    bool is_triple(std::array<int, pile_type_number> p, int eyes);
    void discard();
    std::string id_to_pilename(int id);
    int pilename_to_id(std::string pilename);
public:
    Piles();
    ~Piles();
    void initialize();
    void game_play();
};

Piles::Piles(/* args */)
{
}

Piles::~Piles()
{
    initialize();
}

void Piles::initialize()
{
    pile.fill(0);
    for (int i = 0; i < pile_type_number*4; i++) mountain.at(i) = i / 4;
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(mountain.begin(), mountain.end(), std::default_random_engine(seed));
}

void Piles::game_play()
{
    int position = 0;
    while(position < 13) pile.at(mountain.at(position++)) += 1;
    std::cout << "game_start" << std::endl;
    while (!is_win() && position < pile_type_number * 4) {
        output_pile();
        std::cout << " " << id_to_pilename(mountain.at(position)) << std::endl;
        pile.at(mountain.at(position)) += 1;
        if (is_win()) break;
        discard();
        position++;
    }
    if (position < pile_type_number * 4) std::cout << "自摸" << std::endl;
    else std::cout << "流局" << std::endl;
}

void Piles::output_pile()
{
    int i = 0;
    // 輸出萬子
    while (i < 9) {
        for (int n = 0; n < pile.at(i); n++) std::cout << i + 1;
        i++;
    }
    std::cout << "m";
    // 輸出筒子
    while (i < 18) {
        for (int n = 0; n < pile.at(i); n++) std::cout << i - 8;
        i++;
    }
    std::cout << "p";
    // 輸出索子
    while (i < 27) {
        for (int n = 0; n < pile.at(i); n++) std::cout << i - 17;
        i++;
    }
    std::cout << "s";
     // 輸出字牌
    while (i < pile_type_number) {
        for (int n = 0; n < pile.at(i); n++) std::cout << i - 26;
        i++;
    }
    std::cout << "z";
}

bool Piles::is_win()
{   
    for (int i = 0; i < pile_type_number; i++) {
        // find eyes
        if (pile.at(i) >= 2 && is_triple(pile, i)) return true;
    }
    return false;
}

bool Piles::is_triple(std::array<int, pile_type_number> p, int eyes)
{
    p.at(eyes) -= 2;
    for (int i = 0; i < pile_type_number; i++) {
        if (p.at(i) >= 3) p.at(i) -= 3;
        if (p.at(i) > 0) {
            if ((i >= 0 && i <= 6) || (i >= 9 && i <= 15) || (i >= 18 < i <= 24)) {
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

void Piles::discard()
{
    std::cout << "enter which pile to discard: ";
    std::string discard_pilename;
    std::cin >> discard_pilename;
    int id = pilename_to_id(discard_pilename);
    while (id == -1 || pile.at(id) <= 0) {
        std::cout << "invalid discard: " << discard_pilename << std::endl;
        std::cout << "please enter again: ";
        std::cin >> discard_pilename;
        id = pilename_to_id(discard_pilename);
    }
    pile.at(id) -= 1;
}

std::string Piles::id_to_pilename(int id) 
{
    std::string s;
    switch (id)
    {
    case 0 ... 8:
        s = std::to_string(id + 1) + "m";
        break;
    case 9 ... 17:
        s = std::to_string(id - 8) + "p";
        break;
    case 18 ... 26:
        s = std::to_string(id - 17) + "s";
        break;
    case 27 ... pile_type_number:
        s = std::to_string(id - 26) + "z";
        break;
    default:
        s = "invalid id at id_to_pilename";
        break;
    }
    return s;
}

int Piles::pilename_to_id(std::string pilename) {
    if (pilename.at(0) < '1' || pilename.at(0) > '9') return -1;

    int id = pilename.at(0) - '0';
    switch (pilename.at(1))
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
        if (pilename.at(0) > '7') id = -1;
        else id += 26;
        break;
    default:
        id = -1;
        break;
    }
    return id;
}

int main() 
{
    Piles p;
    bool ongoing = true;

    while (ongoing) {
        std::cout << "waiting for initialization" << std::endl;
        p.initialize();
        p.game_play();
        //ongoing = false;
    }

    return 0;
}