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
    int8_t ore_robot_ore_cost;
    int8_t clay_robot_ore_cost;
    int8_t obsidian_robot_ore_cost;
    int8_t obsidian_robot_clay_cost;
    int8_t geode_robot_ore_cost;
    int8_t geode_robot_obsidian_cost;
};

struct State
{
    int8_t ore; // FIRST ELEMENT!!!
    int8_t clay;
    int8_t obsidian;
    int8_t geodes;
    int8_t ore_robots;
    int8_t clay_robots;
    int8_t obsidian_robots;
    int8_t geode_robots;

    bool operator==(const State& other) const
    {
        return (*((int64_t*)&ore)) == (*((int64_t*)&other.ore));
    }
    struct HashFunction
    {
        size_t operator()(const State& state) const
        {
            return std::hash<int64_t>()(*((int64_t*)&state.ore));
        }
    };
};

using History = std::unordered_map<State, int8_t, State::HashFunction>;

struct Compound
{
    const Blueprint& b;
    State s;
    History history;
};

constexpr int8_t TOTAL_MINS = 32;

int answer = 0; // set by "go" at end

// build tooling
void go(Compound& comp, int8_t min)
{
    auto& s = comp.s;
    const auto& b = comp.b;
    if (min == (TOTAL_MINS-1))
    {
        int brf = s.geodes + s.geode_robots;
        if (brf > answer) {
            answer = brf;
            std::cout << answer << '\n';
        }
        return;
    }

    {
        auto it = comp.history.find(s);
        if (it != comp.history.end()) {
            if (it->second <= min) { return; } // already had same stats with same or worse time
            else { it->second = min; }
        }
        else {
            // doesn't exist:
            comp.history.insert({ s, min });
        }
    }

    // nothing
    {
        s.ore += s.ore_robots;
        s.clay += s.clay_robots;
        s.obsidian += s.obsidian_robots;
        s.geodes += s.geode_robots;

        go(comp, min + 1);

        s.ore -= s.ore_robots;
        s.clay -= s.clay_robots;
        s.obsidian -= s.obsidian_robots;
        s.geodes -= s.geode_robots;
    }

    // geode robot
    if (s.ore >= b.geode_robot_ore_cost && s.obsidian >= b.geode_robot_obsidian_cost) {

        s.ore += s.ore_robots;
        s.clay += s.clay_robots;
        s.obsidian += s.obsidian_robots;
        s.geodes += s.geode_robots;

        s.geode_robots++;
        s.ore -= b.geode_robot_ore_cost;
        s.obsidian -= b.geode_robot_obsidian_cost;
        go(comp, min + 1);
        s.geode_robots--;
        s.ore += b.geode_robot_ore_cost;
        s.obsidian += b.geode_robot_obsidian_cost;

        s.ore -= s.ore_robots;
        s.clay -= s.clay_robots;
        s.obsidian -= s.obsidian_robots;
        s.geodes -= s.geode_robots;
    }

    // obsidian robot
    if (s.ore >= b.obsidian_robot_ore_cost && s.clay >= b.obsidian_robot_clay_cost) {

        s.ore += s.ore_robots;
        s.clay += s.clay_robots;
        s.obsidian += s.obsidian_robots;
        s.geodes += s.geode_robots;

        s.obsidian_robots++;
        s.ore -= b.obsidian_robot_ore_cost;
        s.clay -= b.obsidian_robot_clay_cost;
        go(comp, min + 1);
        s.obsidian_robots--;
        s.ore += b.obsidian_robot_ore_cost;
        s.clay += b.obsidian_robot_clay_cost;

        s.ore -= s.ore_robots;
        s.clay -= s.clay_robots;
        s.obsidian -= s.obsidian_robots;
        s.geodes -= s.geode_robots;
    }

    // clay robot
    if (s.ore >= b.clay_robot_ore_cost) {

        s.ore += s.ore_robots;
        s.clay += s.clay_robots;
        s.obsidian += s.obsidian_robots;
        s.geodes += s.geode_robots;

        s.clay_robots++;
        s.ore -= b.clay_robot_ore_cost;
        go(comp, min + 1);
        s.clay_robots--;
        s.ore += b.clay_robot_ore_cost;

        s.ore -= s.ore_robots;
        s.clay -= s.clay_robots;
        s.obsidian -= s.obsidian_robots;
        s.geodes -= s.geode_robots;
    }

    // ore robot
    if (s.ore >= b.ore_robot_ore_cost) {

        s.ore += s.ore_robots;
        s.clay += s.clay_robots;
        s.obsidian += s.obsidian_robots;
        s.geodes += s.geode_robots;

        s.ore_robots++;
        s.ore -= b.ore_robot_ore_cost;
        go(comp, min + 1);
        s.ore_robots--;
        s.ore += b.ore_robot_ore_cost;

        s.ore -= s.ore_robots;
        s.clay -= s.clay_robots;
        s.obsidian -= s.obsidian_robots;
        s.geodes -= s.geode_robots;
    }
    return;
}


int highest_geode_count(const Blueprint& b)
{
    Compound comp = { b, { 0,0,0,0,1,0,0,0 }, {} };
    answer = -1;
    go(comp, 0);
    return answer;
}


void day19_2_2()
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
                for (int i = 14; i < line.size(); ++i) {
                    auto& c = line[i];
                    if (c >= '0' && c <= '9') {
                        if (find_number) {
                            int num = std::stoi(line.substr(i));
                            (&b.ore_robot_ore_cost)[number] = num;
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

    int total = 1;
    for (int i = 0; i < 3; ++i) {
        total *= highest_geode_count(blueprints[i]);
    }
    std::cout << "PRODCUT OF FIRST THREE: " << total;

}
