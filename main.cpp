#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <iomanip>

using namespace std;


struct Node {
    long long osmid;
    double lat, lon;
};

struct Edge {
    long long u;
    long long v;
    vector<string> names;
   
};

vector<Node> nodes;
vector<Edge> edges;


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
        } catch (const exception &e) {
            cerr << "[ERROR] -> Error procesando la línea " << lineNumber << ": " << e.what() << endl;
            continue;
        }
        
        cout<< fixed << setprecision(10);
        cout << "Node ["<<lineNumber<<"] : " << node.osmid << ", " << node.lat << ", " << node.lon  << endl;
        nodes.push_back(node);
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
            cerr << "[ERROR] -> Error procesando la línea: " <<lineNumber <<", "<< e.what() << endl;
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

int main(){
    string nodesFile = "nodes.csv";
    string edgesFile = "edges.csv";

    readDataset(nodesFile, edgesFile);

    return 0;
}