#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

void day2_1()
{
    int score = 0;

    std::ifstream file("day2.txt");
    if (file.is_open()) {
        for (std::string line; std::getline(file, line); )
        {
            char opp = line[0];
            char you = line[2];
            if (you == 'X') {
                score += 1;
                if (opp == 'A') { score += 3; }
                if (opp == 'B') { score += 0; }
                if (opp == 'C') { score += 6; }
             }
            if (you == 'Y') { 
                score += 2; 
                if (opp == 'B') { score += 3; }
                if (opp == 'C') { score += 0; }
                if (opp == 'A') { score += 6; }
            }
            if (you == 'Z') {
                score += 3;
                if (opp == 'C') { score += 3; }
                if (opp == 'A') { score += 0; }
                if (opp == 'B') { score += 6; }
            }
        }
        file.close();
    }
    std::cout << score;


}

void day2_2()
{
    int score = 0;

    std::ifstream file("day2.txt");
    if (file.is_open()) {
        for (std::string line; std::getline(file, line); )
        {
            char opp = line[0];
            char loseDrawWin = line[2];

            if (loseDrawWin == 'X') { // lose
                if (opp == 'A') { score += 3; }
                if (opp == 'B') { score += 1; }
                if (opp == 'C') { score += 2; }
            }
            if (loseDrawWin == 'Y') { // draw
                if (opp == 'A') { score += 1; }
                if (opp == 'B') { score += 2; }
                if (opp == 'C') { score += 3; }
                score += 3;
            }
            if (loseDrawWin == 'Z') { // win
                if (opp == 'A') { score += 2; }
                if (opp == 'B') { score += 3; }
                if (opp == 'C') { score += 1; }
                score += 6;
            }
            //std::cout << score << '\n';
        }
        file.close();
    }
    std::cout << score;


}
