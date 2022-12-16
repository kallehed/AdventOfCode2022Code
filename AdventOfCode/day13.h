#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>

struct Lis
{
    bool has_lists;
    Lis* parent;
    std::vector<Lis> lists;
    int8_t number;
};

enum ORDER {
    RIGHT_ORDER,
    WRONG_ORDER,
    NEUTRAL_ORDER
};

ORDER compares(Lis& left, Lis& right) // returns whether in right order
{
    if (left.has_lists && right.has_lists)
    {
        int i = 0;
        while (true)
        {
            if (i >= left.lists.size() && i >= right.lists.size()) { return NEUTRAL_ORDER; }
            else if (i < right.lists.size() && i >= left.lists.size()) { return RIGHT_ORDER; }
            else if (i < left.lists.size() && i >= right.lists.size()) { return WRONG_ORDER; }
            else {
                // both are in range at current i
                ORDER res = compares(left.lists[i], right.lists[i]);
                if (res != NEUTRAL_ORDER) { return res; }
            }
            ++i;
        }
    }
    else if (left.has_lists && !right.has_lists)
    {
        Lis n = { true, right.parent, {right}, -1 };
        return compares(left, n);

    }
    else if (!left.has_lists && right.has_lists)
    {
        Lis n = { true, left.parent, {left}, -1 };
        return compares(n, right);   
    }
    else if (!left.has_lists && !right.has_lists) {
        if (left.number < right.number) { return RIGHT_ORDER; }
        else if (left.number > right.number) { return WRONG_ORDER; }
        else { return NEUTRAL_ORDER; }
    }
    else {
        std::cout << "ERRORRRR123";
    }
}

void day13_1()
{


    std::ifstream file("day13.txt");
    if (file.is_open())
    {
        int nr_in_right_order = 0;
        int index = 1;

        Lis twos[2] = { {true, nullptr, {}, -1}, {true, nullptr, {}, -1 } };
        int mod = 0;
        for (std::string line; std::getline(file, line); )
        {
            if (line.length() == 0)
            {
                // compare twos
                Lis& fir = twos[0];
                Lis& sec = twos[1];

                ORDER orde = compares(fir, sec);
                if (orde == RIGHT_ORDER) { nr_in_right_order += index; std::cout << "index: " << index << " has right order\n"; }

                fir.lists.clear();
                sec.lists.clear();

                index++;
                mod = 0;
                continue;
            }

            Lis* cur = &twos[mod];
            
            bool waiting_for_new = false;

            for (int i = 1; i < line.length() - 1; ++i)
            {
                char c = line[i];
                if (!waiting_for_new && c >= '0' && c <= '9') {
                    int8_t val = std::stoi(line.substr(i));
                    Lis numb = { false, cur, {}, val };
                    cur->lists.push_back(numb);
                    waiting_for_new = true;
                }
                else if (c == ',') {
                        waiting_for_new = false;
                }
                else if (c == '[') {
                    {
                        Lis n = { true, cur, {}, 69 };
                        cur->lists.push_back(n);
                    }
                    cur = &(cur->lists[((int)cur->lists.size()) - 1]);
                }
                else if (c == ']') {
                    cur = cur->parent;
                }
            }

            ++mod;
            
        }
        
        std::cout << "Total in right order " << nr_in_right_order;

        file.close();
    }

}

void day13_2()
{
    std::ifstream file("day13.txt");
    if (file.is_open())
    {
        int nr_in_right_order = 0;
        int index = 1;

        std::vector<Lis> lists;
        for (std::string line; std::getline(file, line); )
        {
            if (line.length() == 0)
            {
                continue;
            }

            {
                Lis n = { true, nullptr, {}, -1 };
                lists.push_back(n);
            }
            Lis* cur = &lists[(int)lists.size() - 1];

            bool waiting_for_new = false;

            for (int i = 1; i < line.length() - 1; ++i)
            {
                char c = line[i];
                if (!waiting_for_new && c >= '0' && c <= '9') {
                    int8_t val = std::stoi(line.substr(i));
                    Lis numb = { false, cur, {}, val };
                    cur->lists.push_back(numb);
                    waiting_for_new = true;
                }
                else if (c == ',') {
                    waiting_for_new = false;
                }
                else if (c == '[') {
                    {
                        Lis n = { true, cur, {}, 69 };
                        cur->lists.push_back(n);
                    }
                    cur = &(cur->lists[((int)cur->lists.size()) - 1]);
                }
                else if (c == ']') {
                    cur = cur->parent;
                }
            }

        }

        {
            {
                Lis n = { true, nullptr, {{true, nullptr, {{false, nullptr, {}, 2}}}}, -1 };
                lists.push_back(n);
            }
            lists[lists.size() - 1].lists[0].parent = &lists[lists.size() - 1];
            lists[lists.size() - 1].lists[0].lists[0].parent = &lists[lists.size() - 1].lists[0];

        }
        {
            {
                Lis n = { true, nullptr, {{true, nullptr, {{false, nullptr, {}, 6}}}}, -1 };
                lists.push_back(n);
            }
            lists[lists.size() - 1].lists[0].parent = &lists[lists.size() - 1];
            lists[lists.size() - 1].lists[0].lists[0].parent = &lists[lists.size() - 1].lists[0];
        }
        // next part

        {
            // sort a new vector of pointers
            std::vector<Lis*> poins;

            for (int i = 0; i < lists.size(); ++i) {
                poins.push_back(&lists[i]);
            }

            while (true) // bubble sort
            { 
                bool sorted = true;
                for (int i = 0; i + 1 < poins.size(); ++i)
                {
                    if (compares(*poins[i], *poins[i + 1]) == WRONG_ORDER) {
                        Lis* temp = poins[i + 1];
                        poins[i + 1] = poins[i];
                        poins[i] = temp;
                        sorted = false;
                    }
                }
                if (sorted) {
                    break;
                }
            }

            int product = 1;

            for (int i = 0; i < poins.size(); ++i) {
                if (poins[i]->lists.size() == 1 && poins[i]->lists[0].has_lists && poins[i]->lists[0].lists.size() == 1 && poins[i]->lists[0].lists[0].has_lists == false) {
                    if (poins[i]->lists[0].lists[0].number == 2 || poins[i]->lists[0].lists[0].number == 6) { product *= i + 1; std::cout << "A"; }
                }
            }
            std::cout << "Product: " << product;

        }

        file.close();
    }

}
