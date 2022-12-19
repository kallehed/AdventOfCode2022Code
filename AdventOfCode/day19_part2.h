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
#include <thread>

struct Blueprint
{
    int nr;
    int ore_robot_ore_cost;
    int clay_robot_ore_cost;
    int obsidian_robot_ore_cost;
    int obsidian_robot_clay_cost;
    int geode_robot_ore_cost;
    int geode_robot_obsidian_cost;
};

struct State
{
    int ore;
    int clay;
    int obsidian;
    int geodes;
    int ore_robots;
    int clay_robots;
    int obsidian_robots;
    int geode_robots;
};

int the_value = 0;

int best_at[33];

void go(int* return_val, const Blueprint& blue, State state, int8_t minute) // thread only on minutes 4 to 8
{
    if (minute == 32) { // end
        state.geodes += state.geode_robots;
        (*return_val) = state.geodes;
        if (*return_val > the_value) {
            the_value = *return_val;
            std::cout << "VAL: " << the_value << '\n';
        }
        return;
    }

    if (state.geodes > best_at[minute]) {
        best_at[minute] = state.geodes;
    }
    else if (state.geodes < best_at[minute]) {
        *return_val = 0;
        return;
    }
    if (minute < 4 || minute > 8) {
        int best = 0;

        // wait
        {
            int b;
            go(&b, blue, { state.ore + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots, state.geode_robots }, minute + 1);
            if (b > best) { best = b; }
        }

        // geode robot
        if (state.ore >= blue.geode_robot_ore_cost && state.obsidian >= blue.geode_robot_obsidian_cost) {
            int b;
            go(&b, blue, { state.ore - blue.geode_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian - blue.geode_robot_obsidian_cost + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots, state.geode_robots + 1 }, minute + 1);
            if (b > best) { best = b; }
        }
        // obsidian robot
        if (state.ore >= blue.obsidian_robot_ore_cost && state.clay >= blue.obsidian_robot_clay_cost) {
            int b;
            go(&b, blue, { state.ore - blue.obsidian_robot_ore_cost + state.ore_robots, state.clay - blue.obsidian_robot_clay_cost + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots + 1, state.geode_robots }, minute + 1);
            if (b > best) { best = b; }
        }

        // clay robots
        if (state.ore >= blue.clay_robot_ore_cost) {
            int b;
            go(&b, blue, { state.ore - blue.clay_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots + 1, state.obsidian_robots, state.geode_robots }, minute + 1);
            if (b > best) { best = b; }
        }
        // ore robot
        if (state.ore >= blue.ore_robot_ore_cost) {
            int b;
            go(&b, blue, { state.ore - blue.ore_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots + 1, state.clay_robots, state.obsidian_robots, state.geode_robots }, minute + 1);
            if (b > best) { best = b; }
        }
        (*return_val) = best;
        return;
    }
    else { // threading
        std::thread ts[5];
        int nums[5] = {0,0,0,0,0};

        // wait
        {
            ts[0] = std::thread(go, nums+0, blue, State{ state.ore + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots, state.geode_robots }, minute + 1);
        }

        // geode robot
        if (state.ore >= blue.geode_robot_ore_cost && state.obsidian >= blue.geode_robot_obsidian_cost) {
            ts[1] = std::thread(go, nums+1, blue, State{ state.ore - blue.geode_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian - blue.geode_robot_obsidian_cost + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots, state.geode_robots + 1 }, minute + 1);
        }
        // obsidian robot
        if (state.ore >= blue.obsidian_robot_ore_cost && state.clay >= blue.obsidian_robot_clay_cost) {

            ts[2] = std::thread(go, nums+2, blue, State{ state.ore - blue.obsidian_robot_ore_cost + state.ore_robots, state.clay - blue.obsidian_robot_clay_cost + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots + 1, state.geode_robots }, minute + 1);
        }

        // clay robots
        if (state.ore >= blue.clay_robot_ore_cost) {

            ts[3] = std::thread(go, nums+3, blue,State { state.ore - blue.clay_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots + 1, state.obsidian_robots, state.geode_robots }, minute + 1);

        }
        // ore robot
        if (state.ore >= blue.ore_robot_ore_cost) {

            ts[4] = std::thread(go, nums+4, blue, State{ state.ore - blue.ore_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots + 1, state.clay_robots, state.obsidian_robots, state.geode_robots }, minute + 1);
        }

        for (int i = 0; i < 5; ++i) {
            if (ts[i].joinable()) { ts[i].join(); }
        }

        int biggest = 0;
        for (int i = 0; i < 5; ++i) {
            if (nums[i] > biggest) {
                biggest = nums[i];
            }
        }
        (*return_val) = biggest;
        return;

    }
}

    

void get_qeode_number(const Blueprint* blue, int* num, int index)
{
    go(num,*blue, { 0,0,0,0,1,0,0,0 }, 1);
    std::cout << "index: " << index << " Done\n";
}

void day19_2()
{
    std::vector<Blueprint> blueprints;
    {
        std::ifstream file("day19.txt"); // get names of all valves
        if (file.is_open())
        {
            int i = 0;
            for (std::string line; std::getline(file, line); )
            {
                ++i;
                if (i == 4) { break; }
                Blueprint b;
                bool find_number = true;
                int number = 0;
                for (int i = 0; i < line.size(); ++i) {
                    auto& c = line[i];
                    if (c >= '0' && c <= '9') {
                        if (find_number) {
                            int num = std::stoi(line.substr(i));
                            ((int*)&b.nr)[number] = num;
                            ++number;
                            find_number = false;
                        }
                    }
                    else {
                        find_number = true;
                    }
                }
                blueprints.push_back(b);
            }

            file.close();
        }
    }

    constexpr int LINES = 3;

    int geode_for_all[LINES];

    _ASSERT(blueprints.size() == LINES);

    for (int i = 0; i < LINES; ++i) {
        for (int j = 0; j < 33; ++j) { best_at[j] = 0; }
        get_qeode_number(& blueprints[i], geode_for_all + i, i + 1);
        std::cout << "num: " << geode_for_all[i];
        the_value = 0;
    }

    int total = 1;
    for (int i = 0; i < blueprints.size(); ++i) {
        total *= geode_for_all[i];
        std::cout << 'i' << blueprints[i].nr << " qeodes: " << geode_for_all[i] << '\n';
    }
    std::cout << "TOTAL: " << total;

}
