#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <utility>

constexpr int VALVES = 62;
//constexpr int VALVES = 10;
constexpr int MAX_LEADS = 5;
constexpr int TOTAL_MINUTES = 30;

struct Valve
{
    int flow_rate;
    std::array<int8_t, MAX_LEADS> goes_to; // will be -1 if doesn't go anywhere, and rest don't too
};

// returns highest possible release possible from this point on + previous
int go(const std::array<Valve, VALVES>& valves, std::array<bool, VALVES>& turned_on, int8_t minutes_left, int8_t cur, int released, std::array<int, TOTAL_MINUTES>& current_best)
{
    if (minutes_left == 0) { 
        if (released >= current_best[0]) std::cout << "rel: " << released << '\n';
        return released;  
    }

    for (int i = 0; i < VALVES; ++i) { released += valves[i].flow_rate * turned_on[i]; }


    // max possible
    {
        int max = released;
        for (int i = 0; i < VALVES; ++i) {
            if (!turned_on[i]) {
                max += valves[i].flow_rate * (minutes_left - 1);
            }
        }
        
        if (max <= current_best[minutes_left - 1]) {
            return 0;
        }
    }

    if (released > current_best[minutes_left - 1]) { current_best[minutes_left - 1] = released; }


    int most_released = 0;

    // options

    if (valves[cur].flow_rate != 0 && !turned_on[cur]) {
        turned_on[cur] = true;
        int res = go(valves, turned_on, minutes_left - 1, cur, released, current_best);
        if (res > most_released) { most_released = res; }
        turned_on[cur] = false; // reset
    }
    for (auto way : valves[cur].goes_to) {
        if (way == -1) { break; }
        int res = go(valves, turned_on, minutes_left - 1, way, released, current_best);
        if (res > most_released) { most_released = res; }
    }
    return most_released;
}

void day16_1()
{

    std::unordered_map<std::string, int8_t> names_to_num;
    {
        std::ifstream file("day16.txt"); // get names of all valves
        if (file.is_open())
        {
            int8_t index = 0;
            for (std::string line; std::getline(file, line); )
            {
                std::string name = line.substr(6, 2);

                names_to_num.insert({ name, index });

                ++index;
            }

            file.close();
        }
    }

    std::array<Valve, VALVES> valves;
    for (auto& v : valves) {
        for (auto& g : v.goes_to) {
            g = -1;
        }
        v.flow_rate = -1;
    }

    {
        std::ifstream file("day16.txt"); // get details about how the valves are connected and their flow rate for pressure
        if (file.is_open())
        {
            int8_t index = 0;
            for (std::string line; std::getline(file, line); )
            {
                auto& cur = valves[index];

                {
                    size_t eq_sign = line.find('=');
                    int flow_rate = std::stoi(line.substr(eq_sign + 1));
                    cur.flow_rate = flow_rate;
                }

                size_t before_space = line.find("valve");
                size_t before_goes_to = line.substr(before_space).find(' ') + before_space;

                int i = (int)before_goes_to + 1;
                int go_to_index = 0;
                while (true) {
                    if (i + 1 >= line.size()) { break; }
                    std::string name = line.substr(i, 2);

                    cur.goes_to[go_to_index] = names_to_num[name];

                    i += 4;
                    ++go_to_index;
                }

                ++index;
            }

            file.close();
        }
    }

    std::array<bool, VALVES> turned_on;
    for (auto& e : turned_on) { e = false; }

    int8_t cur = names_to_num.at("AA");

    std::array<int, TOTAL_MINUTES> current_best;
    for (auto& i : current_best) { i = 0; }

    std::cout << "Result: " << go(valves, turned_on, TOTAL_MINUTES, cur, 0, current_best);
}


constexpr int TOTAL_MINUTES2 = 26;

int go2(const std::array<Valve, VALVES>& valves, std::array<bool, VALVES>& turned_on, int8_t minutes_left, int8_t cur, int8_t elph, int released, std::array<int, TOTAL_MINUTES2>& current_best)
{
    if (minutes_left == 0) {
        if (released >= current_best[0]) std::cout << "rel: " << released << '\n';
        return released;
    }
    

    for (int i = 0; i < VALVES; ++i) { released += valves[i].flow_rate * turned_on[i]; }

    //if (minutes_left < 23 && released < 11) { return 0; }
    if (minutes_left < 20 && released < 100) { return 0; }
    if (minutes_left < 15 && released < 526) { return 0; }
    if (minutes_left < 10 && released < 1110) { return 0; }
    //if (minutes_left < 5 && released < 400) { return 0; }
    // max possible
    {
        int max = released;
        for (int i = 0; i < VALVES; ++i) {
            if (!turned_on[i]) {
                max += valves[i].flow_rate * (minutes_left - 1);
            }
        }
        if (max <= current_best[minutes_left - 1]) {
            return 0;
        }
    }

    if (released > current_best[minutes_left - 1]) { current_best[minutes_left - 1] = released; }

    {
        bool finished = true;
        for (int i = 0; i < VALVES; ++i) {
            if (valves[i].flow_rate != 0 && !turned_on[i]) {
                finished = false;
                break;
            }
        }
        if (finished) { return released; }
    }

    int most_released = 0;

    // options

    if (valves[cur].flow_rate != 0 && !turned_on[cur]) {
        turned_on[cur] = true;

        // elephant code
        {
            if (valves[elph].flow_rate != 0 && !turned_on[elph]) {
                turned_on[elph] = true;

                int res = go2(valves, turned_on, minutes_left - 1, cur, elph, released, current_best);
                if (res > most_released) { most_released = res; }

                turned_on[elph] = false;
            }
            else {
                for (auto elph_way : valves[elph].goes_to) {
                    if (elph_way == -1) { break; }
                    int res = go2(valves, turned_on, minutes_left - 1, cur, elph_way, released, current_best);
                    if (res > most_released) { most_released = res; }
                }
            }
        }
        turned_on[cur] = false; // reset
    }
    else {
        for (auto way : valves[cur].goes_to) {
            if (way == -1) { break; }
            //int res = go2(valves, turned_on, minutes_left - 1, way, elph, released, current_best);
            //if (res > most_released) { most_released = res; }
            // elephant code
            {
                if (valves[elph].flow_rate != 0 && !turned_on[elph]) {
                    turned_on[elph] = true;

                    int res = go2(valves, turned_on, minutes_left - 1, way, elph, released, current_best);
                    if (res > most_released) { most_released = res; }

                    turned_on[elph] = false;
                }
                else {
                    for (auto elph_way : valves[elph].goes_to) {
                        if (elph_way == -1) { break; }
                        int res = go2(valves, turned_on, minutes_left - 1, way, elph_way, released, current_best);
                        if (res > most_released) { most_released = res; }
                    }
                }
            }
        }
    }
    return most_released;
}

void day16_2()
{

    std::unordered_map<std::string, int8_t> names_to_num;
    {
        std::ifstream file("day16.txt"); // get names of all valves
        if (file.is_open())
        {
            int8_t index = 0;
            for (std::string line; std::getline(file, line); )
            {
                std::string name = line.substr(6, 2);

                names_to_num.insert({ name, index });

                ++index;
            }

            file.close();
        }
    }

    std::array<Valve, VALVES> valves;
    for (auto& v : valves) {
        for (auto& g : v.goes_to) {
            g = -1;
        }
        v.flow_rate = -1;
    }

    {
        std::ifstream file("day16.txt"); // get details about how the valves are connected and their flow rate for pressure
        if (file.is_open())
        {
            int8_t index = 0;
            for (std::string line; std::getline(file, line); )
            {
                auto& cur = valves[index];

                {
                    size_t eq_sign = line.find('=');
                    int flow_rate = std::stoi(line.substr(eq_sign + 1));
                    cur.flow_rate = flow_rate;
                }

                size_t before_space = line.find("valve");
                size_t before_goes_to = line.substr(before_space).find(' ') + before_space;

                int i = (int)before_goes_to + 1;
                int go_to_index = 0;
                while (true) {
                    if (i + 1 >= line.size()) { break; }
                    std::string name = line.substr(i, 2);

                    cur.goes_to[go_to_index] = names_to_num[name];

                    i += 4;
                    ++go_to_index;
                }

                ++index;
            }

            file.close();
        }
    }

    std::array<bool, VALVES> turned_on;
    for (auto& e : turned_on) { e = false; }

    int8_t cur = names_to_num.at("AA");
    int8_t elph = cur;

    std::array<int, TOTAL_MINUTES2> current_best;
    for (auto& i : current_best) { i = 0; }

    std::cout << "Result: " << go2(valves, turned_on, TOTAL_MINUTES2, cur, elph, 0, current_best);
}
