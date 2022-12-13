#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>

void day6_1()
{


    std::ifstream file("day6.txt");
    if (file.is_open()) {
        int line_nbr = 0;
        for (std::string line; std::getline(file, line); )
        {
            std::array<char, 4> chars;
            chars[0] = line[0];
            chars[1] = line[1];
            chars[2] = line[2];
            chars[3] = line[3];

            for (int i = 4; i < line.size(); ++i) {
                if (chars[0] != chars[1] && chars[0] != chars[2] && chars[0] != chars[3] && chars[1] != chars[2] && chars[1] != chars[3] && chars[2] != chars[3]) {
                    std::cout << "recived: " << i << '\n';
                    goto end;
                }
                else {
                    chars[0] = chars[1];
                    chars[1] = chars[2];
                    chars[2] = chars[3];
                    chars[3] = line[i];
                }

            }
        }
        end:
        file.close();
    }

}

void day6_2()
{


    std::ifstream file("day6.txt");
    if (file.is_open()) {
        int line_nbr = 0;
        for (std::string line; std::getline(file, line); )
        {
            std::array<char, 14> chars;
            chars[0] = line[0];
            chars[1] = line[1];
            chars[2] = line[2];
            chars[3] = line[3];
            for (int i = 0; i < 14; i++) {
                chars[i] = line[i];
            }

            for (int i = 14; i < line.size(); ++i) {

                bool are_unique = true;
                for (int j = 0; j < 13; ++j) {
                    for (int k = j+1; k < 14; ++k) {
                        if (chars[j] == chars[k]) {
                            are_unique = false;
                            goto after;
                        }
                    }
                }
                after:

                if (are_unique) {
                    std::cout << "recived: " << i << '\n';
                    goto end;
                }
                else {
                    for (int j = 0; j < 13; ++j) {
                        chars[j] = chars[j + 1];
                    }
                    chars[13] = line[i];
                }

            }
        }
    end:
        file.close();
    }

}
