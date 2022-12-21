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

enum class OPER : char
{
    PLUS,
    MINUS,
    MULT,
    DIV,
    UNDEFINED,
};

struct MonkeyWithNum
{
    int64_t num;
};
struct MonkeyWithoutNum
{
    int16_t monk1;
    int16_t monk2;
    OPER op;
};

union SomeMonkey
{
    MonkeyWithNum has_num;
    MonkeyWithoutNum has_not;
};

struct Monkey
{
    bool has_num;
    SomeMonkey some;
};



int64_t calc_monk(const std::vector<Monkey>& monks, int index) // calculate the value of a specific monkey, by recursively asking it's monkeys what their answers are.
{
    auto& cur = monks[index];
    if (cur.has_num) {
        return cur.some.has_num.num;
    }
    else {
        auto& m = cur.some.has_not;
        int64_t i1 = calc_monk(monks, m.monk1);
        int64_t i2 = calc_monk(monks, m.monk2);
        
        switch (m.op) {
        case OPER::PLUS:
            return i1 + i2;
        case OPER::MINUS:
            return i1 - i2;
        case OPER::MULT:
            return i1 * i2;
        case OPER::DIV:
            return i1 / i2;
        default:
            std::cout << "ERROR";
            return -1;
        }
    }
}

void day21_1()
{
    std::unordered_map<std::string, int> name_to_index;
    {
        std::ifstream file("day21.txt");
        if (file.is_open())
        {
            int index = 0;
            for (std::string line; std::getline(file, line); )
            {
                name_to_index.insert({ line.substr(0,4),index });
                ++index;
            }

            file.close();
        }
    }
    std::vector<Monkey> monkeys;
    {
        std::ifstream file("day21.txt"); 
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
                Monkey m;
                if (line[6] >= '0' && line[6] <= '9') {
                    m.has_num = true;
                    m.some.has_num.num = std::stoi(line.substr(6));
                }
                else {
                    m.has_num = false;
                    std::string m1 = line.substr(6, 4);
                    std::string m2 = line.substr(13, 4);
                    m.some.has_not.monk1 = name_to_index.at(m1);
                    m.some.has_not.monk2 = name_to_index.at(m2);

                    char c = line[11];
                    if (c == '+') {
                        m.some.has_not.op = OPER::PLUS;
                    }
                    else if (c == '-') {
                        m.some.has_not.op = OPER::MINUS;
                    }
                    else if (c == '*') {
                        m.some.has_not.op = OPER::MULT;
                    }
                    else if (c == '/') {
                        m.some.has_not.op = OPER::DIV;
                    }
                    else {
                        std::cout << "ERROR";
                    }
                }
                monkeys.push_back(m);
            }

            file.close();
        }
    }

    std::cout << "RES: " << calc_monk(monkeys, name_to_index.at("root"));
}

void day21_2()
{
    std::unordered_map<std::string, int> name_to_index;
    {
        std::ifstream file("day21.txt"); // get names of all valves
        if (file.is_open())
        {
            int index = 0;
            for (std::string line; std::getline(file, line); )
            {
                name_to_index.insert({ line.substr(0,4),index });
                ++index;
            }

            file.close();
        }
    }
    std::vector<Monkey> monkeys;
    {
        std::ifstream file("day21.txt"); // get names of all valves
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
                Monkey m;
                if (line[6] >= '0' && line[6] <= '9') {
                    m.has_num = true;
                    m.some.has_num.num = std::stoi(line.substr(6));
                }
                else {
                    m.has_num = false;
                    std::string m1 = line.substr(6, 4);
                    std::string m2 = line.substr(13, 4);
                    m.some.has_not.monk1 = name_to_index.at(m1);
                    m.some.has_not.monk2 = name_to_index.at(m2);
                    char c = line[11];
                    if (c == '+') { 
                        m.some.has_not.op = OPER::PLUS; }
                    else if (c == '-') { 
                        m.some.has_not.op = OPER::MINUS; }
                    else if (c == '*') { 
                        m.some.has_not.op = OPER::MULT; }
                    else if (c == '/') { 
                        m.some.has_not.op = OPER::DIV; }
                    else {
                        std::cout << "ERROR";
                    }
                }
                monkeys.push_back(m);
            }

            file.close();
        }
    }

    auto& root = monkeys[name_to_index.at("root")];

    auto res2 = calc_monk(monkeys, root.some.has_not.monk2); // I saw that this was constant with my input

    auto& you = monkeys[name_to_index.at("humn")];
    you.has_num = true;
    
    int64_t num_to_yell = 0;
    while (true)
    {    
        you.some.has_num.num = num_to_yell;
        auto res1 = calc_monk(monkeys, root.some.has_not.monk1);

        if (res1 == res2) {
            std::cout << "SUCCESS: " << num_to_yell;
            break;
        }
        num_to_yell += std::abs(res1 - res2) / 54; // I saw that the highest change between changes of num_to_yell was 54, so I move forward faster with that information, also res1 moves pretty linearly down towards res2.

        ++num_to_yell;
    }

}