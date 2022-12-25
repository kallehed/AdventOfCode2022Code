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

enum class Sym : char
{
    DUB_MINUS,
    MINUS,
    ZERO,
    ONE,
    TWO,

    TOTAL
};

constexpr size_t DIGITS = 20; // Max digits in the final SNAFU number, might need to be increased, but code will tell you if so.

// in the wrong order
int64_t SNAFU_to_DEC(const std::array<Sym, DIGITS>& SNAFU)
{
    int64_t mult = 1;
    int64_t num = 0;
    for (size_t i = 0; i < SNAFU.size(); ++i)
    {
        switch (SNAFU[i]) {
        case Sym::ZERO:
            break;
        case Sym::ONE:
            num += mult;
            break;
        case Sym::TWO:
            num += mult * 2LL;
            break;
        case Sym::MINUS:
            num -= mult;
            break;
        case Sym::DUB_MINUS:
            num -= 2LL * mult;
            break;
        }
        mult *= 5;
    }
    return num;
}

// Explanation:
// Read the input, get the sum in decimal. 
// Get some SNAFU number (in the wrong order)
// Randomly manipulate it many times, converting it back to decimal and checking if it's closer or not.
// Repeat, until victory.
// First I tried with some sort of algorithm that tried it's best to change the most important digits
// The question only was, what should be changed? And I could not find any simple answer.
// Thus, the answer is chosen randomly for me. 
// Very good solution though, it's runs instantly, even in DEBUG mode. 

void day25_1()
{
    int64_t total = 0;
    {
        std::vector<int64_t> nums;
        std::ifstream file("day25.txt");
        if (file.is_open())
        {
            for (std::string line; std::getline(file, line); )
            {
                int64_t num = 0;
                int64_t mult = 1;
                for (int i = line.size() - 1; i >= 0; --i)
                {
                    char repr = line[i];
                    int64_t num_repr;
                    if (repr == '1') {
                        num_repr = 1;
                    }
                    else if (repr == '2') {
                        num_repr = 2;
                    }
                    else if (repr == '0') {
                        num_repr = 0;
                    }
                    else if (repr == '-') {
                        num_repr = -1;
                    }
                    else if (repr == '=') {
                        num_repr = -2;
                    }
                    else {
                        std::cout << "OUT OF ERROR";
                    }
                    int64_t final_add = num_repr * mult;
                    num += final_add;

                    mult *= 5;
                }
                nums.push_back(num);
            }
            file.close();
            for (auto& a : nums) {
                total += a;
            }
        }
    }
    // get SNAFU version of this number
    std::cout << "TOTAL in DEC: " << total << '\n';

    {
        std::array<Sym, DIGITS> num; // num which will be gradually changed to become the right one
        int64_t smallest_dif;
        {
            for (auto& a : num) { a = Sym::TWO; }
            int64_t cur = SNAFU_to_DEC(num);
            if (cur < total)
            {
                std::cout << "ERROR TOO FEW DIGITS!!!;";
                return;
            }
            smallest_dif = std::abs(cur - total);
        }

        while (true) // ALGORITHM: create lots of snafu numbers equal to the current best, randomly change them(YES!) and change 'num' to the best one. Repeat ad nauseam.
        {
            std::array<Sym, DIGITS> snafu; 
            for (size_t i = 0; i < snafu.size(); ++i) {
                snafu[i] = num[i];
            }

            { // randomly change 'snafu' 1 to 3 times
                int rand_times = 1 + (rand() % 3);
                for (int _ = 0; _ < rand_times; ++_)
                {
                    snafu[(rand() % DIGITS)] = Sym(rand() % (int)Sym::TOTAL);
                }
                
                int64_t dif = std::abs(SNAFU_to_DEC(snafu) - total);
                if (dif < smallest_dif) {
                    smallest_dif = dif;
                    for (size_t i = 0; i < DIGITS; ++i) { // set num to new best num
                        num[i] = snafu[i];
                    }
                }
            }
            if (smallest_dif == 0) // you win!
            {
                std::cout << "FOUND WINNER!\n";

                for (int64_t i = snafu.size() - 1; i >= 0; --i) { // print the solution
                    char c = 'E';
                    switch (snafu[i]) {
                    case Sym::DUB_MINUS:
                        c = '=';
                        break;
                    case Sym::MINUS:
                        c = '-';
                        break;
                    case Sym::ZERO:
                        c = '0';
                        break;
                    case Sym::ONE:
                        c = '1';
                        break;
                    case Sym::TWO:
                        c = '2';
                        break;
                    }
                    std::cout << c;
                }

                return;
            }
        }
    }
}