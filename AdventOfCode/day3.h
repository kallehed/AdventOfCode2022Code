#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int priority_item_type(char c) {
    if (c < 'a') {
        return (c - 'A') + 27;
    }
    else { return (c - 'a') + 1; }
}

void day3_1()
{
    int sum = 0;

    std::ifstream file("day3.txt");
    if (file.is_open()) {
        for (std::string line; std::getline(file, line); )
        {
            int start_for_second = line.size() / 2;
            for (int i = 0; i < start_for_second; ++i) {
                for (int j = start_for_second; j < line.size(); ++j) {
                    if (line[i] == line[j]) {
                        sum += priority_item_type(line[i]);
                        goto end;
                    }
                }
                continue;
            end:
                break;
            }

          
        }
        file.close();
    }
    std::cout << sum;
}

void day3_2()
{
    int sum = 0;

    std::ifstream file("day3.txt");
    if (file.is_open()) {
        std::string combined[3] = { "","",""};
        int mod = 0;
        for (std::string line; std::getline(file, line); )
        {
            combined[mod] = line;

            if (mod == 2) {
                for (char c1 : combined[0]) {
                    for (char c2 : combined[1]) {
                        if (c1 == c2) {
                            for (char c3 : combined[2]) {
                                if (c3 == c1) {
                                    sum += priority_item_type(c1);
                                    goto ending;
                                }
                            }
                        }
                        
                    }
                }
            ending:
                combined[0] = "";
                combined[1] = "";
                combined[2] = "";
            }
            
            mod = (mod + 1) % 3;


        }
        file.close();
    }
    std::cout << sum;
}

