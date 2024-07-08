#ifndef GEOHASH_H
#define GEOHASH_H

#include <string>
#include <cmath>
#include <vector>
#include <bitset>
#include <map>


using namespace std;

const string BASE32 = "0123456789bcdefghjkmnpqrstuvwxyz";
const int BITS[5] = {16, 8, 4, 2, 1};

const map<char, string> neighbors = {
    {'n', "p0r21436x8zb9dcf5h7kjnmqesgutwvy"},
    {'e', "bc01fg45238967deuvhjyznpkmstqrwx"},
    {'s', "14365h7k9dcfesgujnmqp0r2twvyx8zb"},
    {'w', "238967debc01fg45kmstqrwxuvhjyznp"}
};

const map<char, string> borders = {
    {'n', "prxz"},
    {'e', "bcfguvyz"},
    {'s', "028b"},
    {'w', "0145hjnp"}
};

string geohash_encode(double lat, double lon, int precision=9) {
    double lat_min = -90.0, lat_max = 90.0;
    double lon_min = -180.0, lon_max = 180.0;

    string geohash;
    bool is_even = true;
    int bit = 0, ch = 0;

    while (geohash.length() < precision) {
        double mid;
        if (is_even) {
            mid = (lon_min + lon_max) / 2;
            if (lon > mid) {
                ch |= BITS[bit];
                lon_min = mid;
            } else {
                lon_max = mid;
            }
        } else {
            mid = (lat_min + lat_max) / 2;
            if (lat > mid) {
                ch |= BITS[bit];
                lat_min = mid;
            } else {
                lat_max = mid;
            }
        }

        is_even = !is_even;
        if (bit < 4) {
            bit++;
        } else {
            geohash += BASE32[ch];
            bit = 0;
            ch = 0;
        }
    }

    return geohash;
}

string calculate_adjacent(const string& hash, char direction) {
    if (hash.empty()) return "";
    string base = hash.substr(0, hash.size() - 1);
    char last_char = hash[hash.size() - 1];
    int neighbor_index = neighbors.at(direction).find(last_char);

    if (borders.at(direction).find(last_char) != string::npos) {
        base = calculate_adjacent(base, direction);
    }

    return base + BASE32[neighbor_index];
}


vector<string> geohash_neighbors(string geohash) {
    vector<string> neighbor_hashes;

    string north = calculate_adjacent(geohash, 'n');
    string south = calculate_adjacent(geohash, 's');
    string east = calculate_adjacent(geohash, 'e');
    string west = calculate_adjacent(geohash, 'w');

    neighbor_hashes.push_back(north);
    neighbor_hashes.push_back(south);
    neighbor_hashes.push_back(east);
    neighbor_hashes.push_back(west);
    neighbor_hashes.push_back(calculate_adjacent(north, 'e')); // Noreste
    neighbor_hashes.push_back(calculate_adjacent(north, 'w')); // Noroeste
    neighbor_hashes.push_back(calculate_adjacent(south, 'e')); // Sureste
    neighbor_hashes.push_back(calculate_adjacent(south, 'w')); // Suroeste

    return neighbor_hashes;
    return {geohash};
}

#endif