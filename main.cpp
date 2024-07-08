#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <cmath>
#include <limits>
#include <unordered_map>
#include "geohash.h"

using namespace std;


struct Node {
    long long osmid;
    double lat, lon;
    string geohash;
};

struct Edge {
    long long u;
    long long v;
    vector<string> names;
   
};

vector<Node> nodes;
vector<Edge> edges;
unordered_map<string, vector<Node>> geohash_map;
unordered_map<long long, Node> node_map;

vector<string> splitData( string str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);

    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        tokens.push_back(substr);
    }
    return tokens;
}

vector<string> parseMultiValueString( string str) {
    string cleanedStr = str.substr(1, str.size()-2);
    return splitData(cleanedStr, ',');
}

bool readNodes(const string&filename, vector<Node> &nodes){
    ifstream file(filename);
    if(!file<<file.is_open()){
        cerr<< "[ERROR] -> Nose puedo abrir el archivo" << filename <<endl;
        return false;
    }
    string line;
    getline(file, line);
    int lineNumber = 0;
    while(getline(file,line)){
        lineNumber++;
        stringstream ss(line);
        string item;
        Node node;

       
        try {
            getline(ss, item, ','); node.osmid = stoll(item);
            getline(ss, item, ','); node.lat = stod(item);
            getline(ss, item, ','); node.lon = stod(item);

            if (node.lat < -90.0 || node.lat > 90.0 || node.lon < -180.0 || node.lon > 180.0) {
                throw out_of_range("Coordenadas fuera de rango.");
            }
            node.geohash = geohash_encode(node.lat, node.lon);
        } catch (const exception &e) {
            cerr << "[ERROR] -> Error procesando la linea " << lineNumber << ": " << e.what() << endl;
            continue;
        }
        
        cout<< fixed << setprecision(7);
        cout << "Node ["<<lineNumber<<"] : " << node.osmid << ", " << node.lat << ", " << node.lon  << endl;
        nodes.push_back(node);
        geohash_map[node.geohash].push_back(node);
        node_map[node.osmid] = node;
    }

    cout<<"Tarea completada!"<<endl;
    file.close();
    return true;
}

bool readEdges(const string&filename, vector<Edge> &edges){
    ifstream file1(filename);
    if (!file1.is_open()) {
        cerr << "[ERROR] -> No se pudo abrir el archivo " << filename << endl;
        return false;
    }

    string line;
    getline(file1, line);
    int lineNumber = 0;

    while (getline(file1, line)) {
        lineNumber++;
        stringstream ss(line);
        string item;
        Edge edge;

        try{
            getline(ss, item, ','); edge.u = stoll(item);
            getline(ss, item, ','); edge.v = stoll(item);

        getline(ss, item);
        if (item[0] == '[') {
                vector<string> name_tokens = parseMultiValueString(item);
                for (string token : name_tokens) {
                    cout<< name_tokens.size()<<"-"<< token <<"- ";
                    edge.names.push_back(token);
                }
            } else {
                edge.names.push_back(item);
            }
        } catch (const exception &e) {
            cerr << "[ERROR] -> Error procesando la linea: " <<lineNumber <<", "<< e.what() << endl;
            continue;
        }
        cout << "Edge [" << lineNumber << "] :  (" << edge.u << " -> " << edge.v <<")" << endl;
        cout << "  -> NAMES: "<<endl<<"       |-";
        for (const auto &name : edge.names){cout << " "<< name<<" ,";}
        cout << endl;




        edges.push_back(edge);
    }
    cout<<"Tarea completada!"<<endl;
    file1.close();
    return true;
}

void readDataset(const string &nodesFile, const string &edgesFile) {
    

    bool nodesImported = readNodes(nodesFile, nodes);
    bool edgesImported = readEdges(edgesFile, edges);

    if (nodesImported) {
        cout << "[AVISO] -> Nodos importados..." << endl;
    } else {
        cout << "[ERROR] -> No se pudo importar los nodos." << endl;
    }

    if (edgesImported) {
        cout << "[AVISO] -> Aristas importadas..." << endl;
    } else {
        cout << "[ERROR] -> No se pudo importar las aristas." << endl;
    }
}

Edge findClosestEdge(double lat, double lon) {

    if (lat < -90.0 || lat > 90.0 || lon < -180.0 || lon > 180.0) {
        throw out_of_range("Coordenadas fuera de rango.");
    }
    string geohash = geohash_encode(lat, lon);
    vector<string> neighbors = geohash_neighbors(geohash);
    neighbors.push_back(geohash);

    double min_distance = numeric_limits<double>::max();
    Edge closest_edge;
    bool found = false;

    for (const string& neighbor : neighbors) {
        if (geohash_map.find(neighbor) != geohash_map.end()) {
            for (const Node& node : geohash_map[neighbor]) {
                for (const Edge& edge : edges) {
                    if (edge.u == node.osmid || edge.v == node.osmid) {
                        double distance = hypot(lat - node.lat, lon - node.lon);
                        if (distance < min_distance) {
                            min_distance = distance;
                            closest_edge = edge;
                            found = true;
                        }
                    }
                }
            }
        }
    }
    if (!found) {
        throw runtime_error("No se encontro ninguna arista cercana al punto dado.");
    }
    return closest_edge;
}
void exportCSV(const string& queryFile, const string& nodesGeohashFile, double queryLat, double queryLon, const Edge& resultEdge) {

    ofstream queryOut(queryFile);
    if (!queryOut.is_open()) {
        cerr << "[ERROR] -> No se pudo abrir el archivo " << queryFile << " para escritura." << endl;
        return;
    }
    queryOut << "query_lat,query_lon,result_u,result_v" << endl;
    queryOut << fixed << setprecision(7);
    queryOut << queryLat << "," << queryLon << "," << resultEdge.u << "," << resultEdge.v << endl;
    queryOut.close();

    ofstream nodesOut(nodesGeohashFile);
    if (!nodesOut.is_open()) {
        cerr << "[ERROR] -> No se pudo abrir el archivo " << nodesGeohashFile << " para escritura." << endl;
        return;
    }
    nodesOut << "osmid,lat,lon,geohash" << endl;
    for (const Node& node : nodes) {
        nodesOut << fixed << setprecision(7);
        nodesOut << node.osmid << "," << node.lat << "," << node.lon << "," << node.geohash << endl;
    }
    nodesOut.close();
    
    cout << "Archivos exportados correctamente: " << queryFile << ", " << nodesGeohashFile << endl;
}

void query(double lat, double lon, const string& queryFile, const string& nodesGeohashFile) {
    try {
        Edge closest_edge = findClosestEdge(lat, lon);
        cout << "La arista mas cercana al punto (" << lat << ", " << lon << ") es: " << closest_edge.u << " -> " << closest_edge.v << endl << " . Nombre: " << closest_edge.names[0] << endl;

        Node node_u = node_map.at(closest_edge.u);
        Node node_v = node_map.at(closest_edge.v);

        cout << "Coordenadas de los nodos:" << endl;
        cout << "  Nodo u (" << closest_edge.u << "): " << node_u.lat << ", " << node_u.lon << endl;
        cout << "  Nodo v (" << closest_edge.v << "): " << node_v.lat << ", " << node_v.lon << endl;

        exportCSV(queryFile, nodesGeohashFile, lat, lon, closest_edge);

    } catch (const exception &e) {
        cerr << "[ERROR] -> " << e.what() << endl;
    }
}

int main(){
    string nodesFile = "nodes.csv";
    string edgesFile = "edges.csv";

    readDataset(nodesFile, edgesFile);

    //caso de prueba Plaza de armas de Arequipa: -16.39883, -71.53697
    double lat = -16.39883;
    double lon = -71.53697;

    string queryFile = "query_exported.csv";
    string nodesGeohashFile = "nodes_geohashed.csv";
    query(lat, lon, queryFile, nodesGeohashFile);

    return 0;
}