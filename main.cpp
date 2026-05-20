#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

int var(int v, int c, int k) {
    return v * k + c + 1;
}

void writeCNF(int n, vector<pair<int,int>>& edges, int k, string filename) {
    ofstream f(filename);
    int vars = n * k;
    int clauses = n + edges.size() * k;
    
    f << "p cnf " << vars << " " << clauses << "\n";
    
    for (int v = 0; v < n; v++) {
        for (int c = 0; c < k; c++) {
            f << var(v, c, k) << " ";
        }
        f << "0\n";
    }
    
    for (auto& e : edges) {
        for (int c = 0; c < k; c++) {
            f << -var(e.first, c, k) << " " << -var(e.second, c, k) << " 0\n";
        }
    }
    f.close();
}

int main(int argc, char* argv[]) {
    string input_file;
    if (argc > 1) {
        input_file = argv[1];
    } else {
        cin >> input_file;
    }
    
    ifstream f(input_file);
    int n, m;
    f >> n >> m;
    
    vector<pair<int,int>> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        f >> u >> v;
        edges.push_back({u, v});
    }
    f.close();
    
    for (int k = 2; k <= n; k++) {
        string cnf_file = "temp.cnf";
        writeCNF(n, edges, k, cnf_file);
        
        string cmd = "minisat " + cnf_file + " temp.out > /dev/null 2>&1";
        system(cmd.c_str());
        
        ifstream out("temp.out");
        string line;
        getline(out, line);
        
        if (line == "SAT") {
            cout << k << endl;
            
            while (getline(out, line)) {
                if (line[0] == 'v') {
                    stringstream ss(line.substr(2));
                    int val;
                    vector<int> colors(n, -1);
                    while (ss >> val) {
                        if (val > 0) {
                            int idx = val - 1;
                            int c = idx % k;
                            int v = idx / k;
                            if (colors[v] == -1) colors[v] = c;
                        }
                    }
                    for (int i = 0; i < n; i++) {
                        cout << colors[i] << " ";
                    }
                    cout << endl;
                    break;
                }
            }
            break;
        }
        out.close();
    }
    
    return 0;
}
