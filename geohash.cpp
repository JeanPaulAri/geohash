#include "geohash.h"
#include <bitset>

#include "geohash.h"
#include <bitset>

using namespace std;

const string BASE32 = "0123456789bcdefghjkmnpqrstuvwxyz";
const int BITS[5] = {16, 8, 4, 2, 1};

string geohash_encode(double lat, double lon, int precision) {
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

vector<std::string> geohash_neighbors(string geohash) {
    // Implementación básica de los vecinos de un geohash.
    // Para simplicidad, aquí se devuelve solo el propio geohash.
    // Para una implementación completa, debes agregar lógica para calcular vecinos.
    return {geohash};
}