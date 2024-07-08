#ifndef GEOHASH_H
#define GEOHASH_H

#include <string>
#include <cmath>
#include <vector>

using namespace std;

string geohash_encode(double lat, double lon, int precision = 10);
vector<std::string> geohash_neighbors(string geohash);

#endif