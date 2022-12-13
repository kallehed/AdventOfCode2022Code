#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>


struct Dir {
    long long size;
    std::vector<Dir> dirs;
    Dir* parent;
    std::string name;
};

long long go_through(Dir* cur, long long* total) {
    long long size = cur->size;
    
    for (auto& d : cur->dirs) {
        size += go_through(&d, total);
    }

    if (size <= 100000) { *total += size; }
    return size;
}

void day7_1()
{
    Dir s{ 0, {}, nullptr, "/"};

    Dir* cur = &s;

    std::ifstream file("day7.txt");
    if (file.is_open()) {
        int line_nbr = 0;
        for (std::string line; std::getline(file, line); )
        {
            if (line.substr(0, 4) == "$ cd") {
                if (line[5] == '/') {
                    cur = &s;
                }
                else if (line.substr(5, 2) == "..") {
                    cur = cur->parent;
                }
                else {
                    std::string goto_dir = line.substr(5);
                    bool found_dir = false;
                    for (int i = 0; i < cur->dirs.size(); ++i) {
                        if (cur->dirs[i].name == goto_dir) {
                            found_dir = true;
                            cur = &cur->dirs[i];
                            break;
                        }
                    }
                    if (!found_dir) {
                        cur->dirs.push_back({ 0, {}, cur, goto_dir });
                        cur = &cur->dirs[cur->dirs.size() - 1];
                    }
                }
            }
            else if (line[0] != '$')
            {
                if (line.substr(0, 3) == "dir") {

                }
                else {
                    long long tot = std::stol(line);
                    cur->size += tot;
                }
            }
        }
        file.close();
    }

    long long total = 0;
    go_through(&s, &total);
    std::cout << total;
}

long long go_through_2(Dir* cur, long long needed, long long *result) {
    long long size = cur->size;

    for (auto& d : cur->dirs) {
        size += go_through_2(&d, needed, result);
    }

    if (size >= needed && size <= *result) { *result = size; }
    return size;
}

void day7_2()
{
    Dir s{ 0, {}, nullptr, "/" };

    Dir* cur = &s;

    std::ifstream file("day7.txt");
    if (file.is_open()) {
        int line_nbr = 0;
        for (std::string line; std::getline(file, line); )
        {
            if (line.substr(0, 4) == "$ cd") {
                if (line[5] == '/') {
                    cur = &s;
                }
                else if (line.substr(5, 2) == "..") {
                    cur = cur->parent;
                }
                else {
                    std::string goto_dir = line.substr(5);
                    bool found_dir = false;
                    for (int i = 0; i < cur->dirs.size(); ++i) {
                        if (cur->dirs[i].name == goto_dir) {
                            found_dir = true;
                            cur = &cur->dirs[i];
                            break;
                        }
                    }
                    if (!found_dir) {
                        cur->dirs.push_back({ 0, {}, cur, goto_dir });
                        cur = &cur->dirs[cur->dirs.size() - 1];
                    }
                }
            }
            else if (line[0] != '$')
            {
                if (line.substr(0, 3) == "dir") {

                }
                else {
                    long long tot = std::stol(line);
                    cur->size += tot;
                }
            }
        }
        file.close();
    }

    long long total = 0;
    long long total_size = go_through(&s, &total);
    long long space_needed = 30000000LL - (70000000LL - total_size);
    std::cout << "total size: " << total_size << " space needed: " << space_needed << '\n';

    long long res = 459234892734LL;

    go_through_2(&s, space_needed, &res);
    
    std::cout << res;
}