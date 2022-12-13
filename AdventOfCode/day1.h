#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

void day1_1() 
{
    std::vector<int> calories_per = {0};

    std::ifstream file("day1.txt");
    if (file.is_open()) {
        for (std::string line; std::getline(file, line); )
        {
            //std::cout << line << '\n';
            
            if (line.length() == 0) {
                //std::cout << "empty\n";
                calories_per.push_back(0);
            }
            else {
                calories_per[calories_per.size() - 1] += std::stoi(line);
            }
        }
        file.close();
    }
    
    for (auto c : calories_per) {
        std::cout << c << '\n';
    }
    std::cout << "highest calories" << *std::max_element(calories_per.begin(), calories_per.end());
	
}

void day1_2()
{
    std::vector<int> calories_per = { 0 };

    std::ifstream file("day1.txt");
    if (file.is_open()) {
        for (std::string line; std::getline(file, line); )
        {
            //std::cout << line << '\n';

            if (line.length() == 0) {
                //std::cout << "empty\n";
                calories_per.push_back(0);
            }
            else {
                calories_per[calories_per.size() - 1] += std::stoi(line);
            }
        }
        file.close();
    }

    int biggest = 0;
    int second = 0;
    int third = 0;

    for (auto c : calories_per) {
        if (c > biggest) {
            third = second;
            second = biggest;
            biggest = c;
        }
        else if (c > second) {
            third = second;
            second = c;
        }
        else if (c > third) {
            third = c;
        }
    }
    std::cout << "highest calories: " << (biggest + second + third) << '\n' << biggest;
}