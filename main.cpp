#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "json.hpp"  // Include nlohmann/json library

using json = nlohmann::json;
using namespace std;

// Decode number from given base (supports bases up to 36)
long long decodeFromBase(const string &val, int base) {
    long long result = 0;
    for (char c : val) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else throw invalid_argument("Invalid character in value");
        if (digit >= base) throw invalid_argument("Digit out of range for base");
        result = result * base + digit;
    }
    return result;
}

// Lagrange interpolation at x = 0 (to extract constant term)
double lagrangeInterpolation(vector<pair<int, long long>> &points, int k) {
    double secret = 0.0;

    for (int i = 0; i < k; i++) {
        double xi = points[i].first;
        double yi = points[i].second;

        double term = yi;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                double xj = points[j].first;
                term *= (0 - xj) / (xi - xj);
            }
        }
        secret += term;
    }
    return round(secret);
}

int main() {
    ifstream inputFile("input.json");
    if (!inputFile) {
        cerr << "Error: Could not open input.json file!" << endl;
        return 1;
    }

    json testCases;
    inputFile >> testCases;

    // Loop over all test cases
    for (size_t t = 0; t < testCases.size(); t++) {
        cout << "\n### Assignment Checkpoints (Test Case " << (t + 1) << "):\n";

        int n = testCases[t]["keys"]["n"];
        int k = testCases[t]["keys"]["k"];

        // 1. Read input
        cout << "- **1. Read the Test Case (Input) from a  separate JSON file** ✅\n";
        cout << "  n = " << n << ", k = " << k << "\n";

        vector<pair<int, long long>> points;

        for (auto &el : testCases[t].items()) {
            if (el.key() == "keys") continue;

            int x = stoi(el.key());
            int base = stoi(el.value()["base"].get<string>());
            string val = el.value()["value"].get<string>();

            long long y = decodeFromBase(val, base);
            points.push_back({x, y});
        }

        // Sort points by x
        sort(points.begin(), points.end());

        // 2. Decode Y values
        cout << "- **2. Decode the Y Values** ✅\n";
        for (auto &p : points) {
            cout << "  (" << p.first << "," << p.second << ")\n";
        }

        // Take first k points and interpolate
        vector<pair<int, long long>> subset(points.begin(), points.begin() + k);
        double secret = lagrangeInterpolation(subset, k);

        // 3. Find Secret
        cout << "- **3. Find the Secret (C)** ✅\n";
        cout << "  Secret = " << secret << "\n";
    }

    return 0;
}
