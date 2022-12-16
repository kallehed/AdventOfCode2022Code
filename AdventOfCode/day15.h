#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <utility>

using Int = int64_t;

struct Pos {
    Int x;
    Int y;
    Int manhattan_distane(const Pos& o) {
        return std::abs(x - o.x) + std::abs(y - o.y);
    }
    bool operator==(const Pos& o) const
    {
        return x == o.x && y == o.y;
    }
};

struct Sensor {
    Pos pos;
    Int manhattan_dist;
};

void day15_1()
{
    std::vector<Sensor> sensors;
    std::vector<Pos> beacons;

    std::ifstream file("day15.txt");
    if (file.is_open())
    {
        for (std::string line; std::getline(file, line); )
        {
            Int sensor_x, sensor_y, beacon_x, beacon_y;
            {
                sensor_x = std::stoll(line.substr(12));
                size_t first_y = line.find('y');
                sensor_y = std::stoull(line.substr(first_y + 2ULL));
                size_t second_x = line.substr(first_y + 2).find('x') + first_y + 2;
                std::string at_second_x = line.substr(second_x + 2);
                beacon_x = std::stoll(at_second_x);
                size_t second_y = line.substr(second_x + 2).find('y') + second_x + 2;
                beacon_y = std::stoll(line.substr(second_y + 2));
            }

            {
                Sensor s;
                s.pos = { sensor_x, sensor_y };
                Int dist = std::abs(sensor_x - beacon_x) + std::abs(sensor_y - beacon_y);
                s.manhattan_dist = dist;
                sensors.push_back(s);
            }
            Pos b = { beacon_x, beacon_y };
            if (std::find(beacons.begin(), beacons.end(), b) == beacons.end()) {
                beacons.push_back({ beacon_x, beacon_y });
            }

        }

        file.close();
    }


    Int cannot_contain_beacon = 0LL;
    for (Int x = -10000000; x < 10000000; ++x) {
        Int y = 2000000LL;
        Pos p = { x, y };
        bool at_beacon = false;
        for (auto& b : beacons) {
            if (b == p) {
                at_beacon = true;
                break;
            }
        }
        if (at_beacon) {
            continue;
        }

        for (auto& s : sensors) {
            if (p.manhattan_distane(s.pos) <= s.manhattan_dist) {
                ++cannot_contain_beacon;
                break;
            }
        }
    }
    std::cout << "total positions that cant contain beacons: " << cannot_contain_beacon;

}

void day15_2()
{
    std::vector<Sensor> sensors;
    std::vector<Pos> beacons;

    std::ifstream file("day15.txt");
    if (file.is_open())
    {
        for (std::string line; std::getline(file, line); )
        {
            Int sensor_x, sensor_y, beacon_x, beacon_y;
            {
                sensor_x = std::stoll(line.substr(12));
                size_t first_y = line.find('y');
                sensor_y = std::stoull(line.substr(first_y + 2ULL));
                size_t second_x = line.substr(first_y + 2).find('x') + first_y + 2;
                std::string at_second_x = line.substr(second_x + 2);
                beacon_x = std::stoll(at_second_x);
                size_t second_y = line.substr(second_x + 2).find('y') + second_x + 2;
                beacon_y = std::stoll(line.substr(second_y + 2));
            }

            {
                Sensor s;
                s.pos = { sensor_x, sensor_y };
                Int dist = std::abs(sensor_x - beacon_x) + std::abs(sensor_y - beacon_y);
                s.manhattan_dist = dist;
                sensors.push_back(s);
            }
            Pos b = { beacon_x, beacon_y };
            if (std::find(beacons.begin(), beacons.end(), b) == beacons.end()) {
                beacons.push_back({ beacon_x, beacon_y });
            }

        }

        file.close();
    }

    for (Int y = 0; y < 4000000LL; ++y) {
        for (Int x = 0; x < 4000000LL; ++x) {
            Pos p = { x, y };
            bool bad_pos = false;

            for (auto& s : sensors) {
                Int dist = std::abs(s.pos.x - x) + std::abs(s.pos.y - y);
                if (dist <= s.manhattan_dist) {
                    Int dif = s.manhattan_dist - dist;
                    x += dif;
                    bad_pos = true;
                    break;
                }
            }
            if (bad_pos) { continue; }
            // it is a position where a beacon can be
            std::cout << "possible position: x:" << x << " y:" << y << " With tuning frequency: " << (y + 4000000 * x) << '\n';
        }
    }

}