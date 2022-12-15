#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>
//#include "BigInt.h"

enum Oper {
    PLUS,
    MULT
};

struct Monkey {
    std::vector<int> items;
    bool square_old;
    Oper oper;
    int oper_num;
    int div_by;
    int true_throw;
    int false_throw;
    int inspected_numbers;
};

void day11_1() 
{
    std::ifstream file("day11.txt");
    if (file.is_open()) {

        std::vector<Monkey> monkeys;

        for (std::string line; std::getline(file, line); )
        {
            if (line.substr(0, 6) == "Monkey") {
                monkeys.push_back({ {}, false, PLUS, 0, 0, -1, -1, 0});
            }
            else if (line.substr(0, 17) == "  Starting items:") {
                int i = 17;
                while (true) {
                    if (line[i] == '\0') { break; }
                    if (line[i] == ' ') {
                        if (line[i + 1] <= '9' && line[i + 1] >= '0') {
                            int num = std::stoi(line.substr(i + 1));
                            monkeys[monkeys.size() - 1].items.push_back(num);
                        }
                        else { break; }
                    }
                    ++i;
                }
            }
            else if (line.substr(0, 23) == "  Operation: new = old ") {
                auto& m = monkeys[monkeys.size() - 1];
                if (line[23] == '+') { m.oper = PLUS; }
                else { m.oper = MULT; }
                if (line[25] == 'o') { m.square_old = true; }
                else { m.oper_num = std::stoi(line.substr(25)); }
            }
            else if (line.substr(0, 21) == "  Test: divisible by ") {
                monkeys[monkeys.size() - 1].div_by = std::stoi(line.substr(21));
            }
            else if (line.substr(0, 29) == "    If true: throw to monkey ") {
                monkeys[monkeys.size() - 1].true_throw = std::stoi(line.substr(29));
            }
            else if (line.substr(0, 30) == "    If false: throw to monkey ") {
                monkeys[monkeys.size() - 1].false_throw = std::stoi(line.substr(30));
            }
        }

        for (int _ = 0; _ < 20; ++_) {
            for (auto& m : monkeys) {
                while (m.items.size() > 0) {
                    m.inspected_numbers++;
                    int i = m.items[0];
                    if (m.square_old) {
                        if (m.oper == MULT) {
                            i = i * i;
                        }
                        else { i = i + i; }
                    }
                    else {
                        if (m.oper == MULT) {
                            i = i * m.oper_num;
                        }
                        else { i = i + m.oper_num; }
                    }
                    i /= 3;
                    if (i % m.div_by == 0) {
                        monkeys[m.true_throw].items.push_back(i);
                    }
                    else {
                        monkeys[m.false_throw].items.push_back(i);
                    }
                    m.items.erase(m.items.begin());
                }
            }
        }

        int highest = 0;
        int next_highest = 0;
        for (auto& m : monkeys) {
            if (m.inspected_numbers > highest) {
                next_highest = highest;
                highest = m.inspected_numbers;
            }
            else if (m.inspected_numbers > next_highest) {
                next_highest = m.inspected_numbers;
            }
        }
        std::cout << (highest * next_highest);

        file.close();
    }
}


struct Monkey2 {
    std::vector<long long> items;
    bool square_old;
    Oper oper;
    int oper_num;
    int div_by;
    int true_throw;
    int false_throw;
    int inspected_numbers;
};


void day11_2() 
{
    std::ifstream file("day11.txt");
    if (file.is_open()) {

        std::vector<Monkey2> monkeys;

        for (std::string line; std::getline(file, line); )
        {
            if (line.substr(0, 6) == "Monkey") {
                monkeys.push_back({ {}, false, PLUS, 0, 0, -1, -1, 0 });
            }
            else if (line.substr(0, 17) == "  Starting items:") {
                long long i = 17;
                while (true) {
                    if (line[i] == '\0') { break; }
                    if (line[i] == ' ') {
                        if (line[i + 1] <= '9' && line[i + 1] >= '0') {
                            unsigned long long num = std::stoi(line.substr(i + 1));
                            monkeys[monkeys.size() - 1].items.push_back(num);
                        }
                        else { break; }
                    }
                    ++i;
                }
            }
            else if (line.substr(0, 23) == "  Operation: new = old ") {
                auto& m = monkeys[monkeys.size() - 1];
                if (line[23] == '+') { m.oper = PLUS; }
                else { m.oper = MULT; }
                if (line[25] == 'o') { m.square_old = true; }
                else { m.oper_num = std::stoi(line.substr(25)); }
            }
            else if (line.substr(0, 21) == "  Test: divisible by ") {
                monkeys[monkeys.size() - 1].div_by = std::stoi(line.substr(21));
            }
            else if (line.substr(0, 29) == "    If true: throw to monkey ") {
                monkeys[monkeys.size() - 1].true_throw = std::stoi(line.substr(29));
            }
            else if (line.substr(0, 30) == "    If false: throw to monkey ") {
                monkeys[monkeys.size() - 1].false_throw = std::stoi(line.substr(30));
            }
        }

        long long biggest_num = 1;
        for (auto& m : monkeys) {
            biggest_num *= m.div_by;
        }

        for (long long _ = 0LL; _ < 10000; ++_) {
            for (auto& m : monkeys) {
                while (m.items.size() > 0) {
                    m.inspected_numbers++;
                    auto& i = m.items[0];
                    if (i < 0) { std::cout << "err;"; std::cin.get(); }
                    if (m.square_old) {
                        if (m.oper == MULT) {
                            i = i * i;
                        }
                        else { i = i + i; }
                    }
                    else {
                        if (m.oper == MULT) {
                            i = i * m.oper_num;
                        }
                        else { i = i + m.oper_num; }
                    }
                    //i /= 3;

                    while (i >= biggest_num) { i -= biggest_num; }

                    if (i % m.div_by == 0) {
                        monkeys[m.true_throw].items.push_back(i);
                    }
                    else {
                        monkeys[m.false_throw].items.push_back(i);
                    }
                    m.items.erase(m.items.begin());
                }
            }
        }

        long long highest = 0;
        long long next_highest = 0;
        for (auto& m : monkeys) {
            if (m.inspected_numbers > highest) {
                next_highest = highest;
                highest = m.inspected_numbers;
            }
            else if (m.inspected_numbers > next_highest) {
                next_highest = m.inspected_numbers;
            }
        }
        std::cout << (highest * next_highest);

        file.close();
    }
}
