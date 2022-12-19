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

int go(const Blueprint& blue, State state, int8_t minute)
{
    if (minute == 24) { // end
        state.geodes += state.geode_robots;
        return state.geodes;
    }

    int best = 0;

    // wait
    {
        int b = go(blue, { state.ore + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots, state.geode_robots }, minute + 1);
        if (b > best) { best = b; }
    }

    // geode robot
    if (state.ore >= blue.geode_robot_ore_cost && state.obsidian >= blue.geode_robot_obsidian_cost) {
        int b = go(blue, {state.ore - blue.geode_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian - blue.geode_robot_obsidian_cost + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots, state.geode_robots + 1}, minute + 1);
        if (b > best) { best = b; }
    }
    // obsidian robot
    if (state.ore >= blue.obsidian_robot_ore_cost && state.clay >= blue.obsidian_robot_clay_cost) {
        int b = go(blue, { state.ore - blue.obsidian_robot_ore_cost + state.ore_robots, state.clay - blue.obsidian_robot_clay_cost + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots, state.obsidian_robots + 1, state.geode_robots }, minute + 1);
        if (b > best) { best = b; }
    }

    // clay robots
    if (state.ore >= blue.clay_robot_ore_cost) {
        int b = go(blue, { state.ore - blue.clay_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots, state.clay_robots + 1, state.obsidian_robots, state.geode_robots }, minute + 1);
        if (b > best) { best = b; }
    }
    // ore robot
    if (state.ore >= blue.ore_robot_ore_cost) {
        int b = go(blue, { state.ore - blue.ore_robot_ore_cost + state.ore_robots, state.clay + state.clay_robots, state.obsidian + state.obsidian_robots, state.geodes + state.geode_robots, state.ore_robots + 1, state.clay_robots, state.obsidian_robots, state.geode_robots }, minute + 1);
        if (b > best) { best = b; }
    }
    return best;
}

void get_qeode_number(const Blueprint* blue, int* num, int index)
{
    int a = go(*blue, { 0,0,0,0,1,0,0,0 }, 1);
    (*num) = a;
    std::cout << "index: " << index << " Done\n";
}

void day19_1()
{
    std::vector<Blueprint> blueprints;
    {
        std::ifstream file("day19.txt"); // get names of all valves
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
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

    constexpr int LINES = 30;

    int geode_for_all[LINES];
    std::thread threads[LINES];

    _ASSERT(blueprints.size() == LINES);

    for (int i = 0; i < blueprints.size(); ++i) {
        threads[i] = std::thread(get_qeode_number, &blueprints[i], geode_for_all + i, i+1);
    }
    for (int i = 0; i < blueprints.size(); ++i) {
        threads[i].join();
    }

    int total = 0;
    for (int i = 0; i < blueprints.size(); ++i) {
        int quality = (blueprints[i].nr * geode_for_all[i]);
        total += std::max(0,quality);
        std::cout <<'i' << blueprints[i].nr << " qeodes: " << geode_for_all[i] << " quality level: " << quality << '\n';
    }
    std::cout << "TOTAL: " << total;

}
