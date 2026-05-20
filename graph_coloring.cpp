#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class FileReader {
private:
    string filename;
    
public:
    FileReader(string name) {
        filename = name;
    }
    
    bool read(int& n, vector<vector<int>>& a) {
        ifstream f(filename);
        if (!f.is_open()) {
            cout << "file not open" << endl;
            return false;
        }
        
        int m;
        f >> n >> m;
        a.resize(n);
        
        for (int i = 0; i < m; i++) {
            int x, y;
            f >> x >> y;
            a[x].push_back(y);
            a[y].push_back(x);
        }
        
        f.close();
        return true;
    }
};

class GraphColor {
private:
    int n;
    vector<vector<int>> adj;
    vector<int> color;
    
public:
    GraphColor(int N, vector<vector<int>>& a) {
        n = N;
        adj = a;
        color.assign(n, -1);
    }
    
    void greedyColoring() {
        vector<bool> used;
        
        for (int v = 0; v < n; v++) {
            used.assign(n, false);
            
            for (int to : adj[v]) {
                if (color[to] != -1) {
                    used[color[to]] = true;
                }
            }
            
            int c = 0;
            while (used[c]) c++;
            color[v] = c;
        }
    }
    
    void print() {
        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (color[i] > ans) ans = color[i];
        }
        ans++;
        
        cout << ans << endl;
        for (int i = 0; i < n; i++) {
            cout << color[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    string name;
    cout << "Enter filename: ";
    cin >> name;
    
    FileReader reader(name);
    
    int n;
    vector<vector<int>> adj;
    
    if (!reader.read(n, adj)) {
        return 1;
    }
    
    GraphColor graph(n, adj);
    graph.greedyColoring();
    graph.print();
    
    return 0;
}