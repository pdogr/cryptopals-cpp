#include <bits/stdc++.h>

using namespace std;

vector<uint8_t> operator^ (const vector<uint8_t> &, const vector<uint8_t> &);

string operator^ (const string &a, const string &b);

vector<pair<vector<uint8_t>, long double> >
decodeSingleXor (string, map<uint8_t, long double>, int);

long double score (map<uint8_t, long double>, vector<uint8_t>);

long double chiSquaredStatistic (map<uint8_t, long double>, vector<uint8_t>);

int hamming (vector<uint8_t>, vector<uint8_t>);

vector<int> kasiki (vector<uint8_t> ct);