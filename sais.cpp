#include <iostream>

using namespace std;


bool equalSubarrays(const vector<int> &T, int start1, int end1, int start2, int end2) {
    if (end1 - start1 != end2 - start2) {
        return false;
    }
    for (int i = start1, j = start2; i < end1 && j < end2; i++, j++) {
        if (T[i] != T[j]) {
            return false;
        }
    }
    return true;
}

map<int, pair<int, int>> getBuckets(const vector<int> &T) {
    map<int, int> count;
    map<int, pair<int, int>> buckets;
    for (int c : T) {
        count[c]++;
    }
    int start = 0;
    vector<int> keys;
    for (auto countPair : count) {
        keys.push_back(countPair.first);
    }
    sort(keys.begin(), keys.end());
    for (auto c : keys) {
        buckets.insert({c, {start, start + count[c]}});
        start += count[c];
    }
    return buckets;
}

vector<int> sais(const vector<int> &T) {
    vector<string> t(T.size(), "_");
    t[T.size() - 1] = "S";
    for (int i = T.size() - 1; i > 0; i--) {
        if (T[i - 1] == T[i]) {
            t[i - 1] = t[i];
        } else {
            t[i - 1] = T[i - 1] < T[i] ? "S" : "L";
        }
    }

    map<int, pair<int, int>> buckets = getBuckets(T);

    map<int, int> count;
    vector<int> SA(T.size(), -1);
    map<int, int> LMS;
    int end = -1;
    for (int i = T.size() - 1; i > 0; i--) {
        if ((t[i] == "S") && (t[i - 1] == "L")) {
            count[T[i]]++;
            int revoffset = count[T[i]];
            SA[buckets[T[i]].second - revoffset] = i;
            if (end != -1) {
                LMS[i] = end;
            }
            end = i;
        }
    }
    LMS[T.size() - 1] = T.size() - 1;
    count.clear();
    for (int i = 0; i < T.size(); i++) {
        if (SA[i] > 0) {
            if (t[SA[i] - 1] == "L") {
                int symbol = T[SA[i] - 1];
                int offset = count[symbol];
                count[symbol] = count[symbol] + 1;
                SA[buckets[symbol].first + offset] = SA[i] - 1;
            }
        } else if (SA[i] == 0) {
            if (t[t.size() - 1] == "L") {
                int symbol = T[T.size() - 1];
                int offset = count[symbol];
                count[symbol] = count[symbol] + 1;
                SA[buckets[symbol].first + offset] = SA[i] - 1;
            }
        }
    }

    count.clear();

    for (int i = T.size() - 1; i > 0; i--) {
        if (SA[i] > 0) {
            if (t[SA[i] - 1] == "S") {
                int symbol = T[SA[i] - 1];
                count[symbol] = count[symbol] + 1;
                int revoffset = count[symbol];
                SA[buckets[symbol].second - revoffset] = SA[i] - 1;
            }
        }
    }
    vector<int> namesp(T.size(), -1);
    int name = 0;
    int prev = -1;

    for (int i = 0; i < SA.size(); i++) {
        string a = t[SA[i]];
        string b;
        if (SA[i] > 0) {
            b = t[SA[i] - 1];
        } else if (SA[i] == 0) {
            b = t[t.size() - 1];
        }

        if (a == "S" && b == "L") {
            if (prev != -1) {
                if (!equalSubarrays(T, SA[prev], LMS[SA[prev]], SA[i], LMS[SA[i]])) {
                    name += 1;
                }
            }
            prev = i;
            namesp[SA[i]] = name;
        }
    }
    vector<int> names;
    vector<int> SApIdx;

    for (int i = 0; i < T.size(); i++) {
        if (namesp[i] != -1) {
            names.push_back(namesp[i]);
            SApIdx.push_back(i);
        }
    }

    if (name < names.size() - 1) {
        names = sais(names);
    }

    reverse(names.begin(), names.end());

    SA.assign(T.size(), -1);
    count.clear();
    for (int i = 0; i < names.size(); i++) {
        int pos = SApIdx[names[i]];
        count[T[pos]] = count[T[pos]] + 1;
        int revoffset = count[T[pos]];
        SA[buckets[T[pos]].second - revoffset] = pos;
    }

    count.clear();

    for (int i = 0; i < T.size(); i++) {
        if (SA[i] > 0) {
            if (t[SA[i] - 1] == "L") {
                int symbol = T[SA[i] - 1];
                int offset = count[symbol];
                SA[buckets[symbol].first + offset] = SA[i] - 1;
                count[symbol] = offset + 1;
            }
        }
    }
    count.clear();
    for (int i = T.size() - 1; i > 0; i--) {
        if (SA[i] > 0) {
            if (t[SA[i] - 1] == "S") {
                int symbol = T[SA[i] - 1];
                count[symbol] = count[symbol] + 1;
                int revoffset = count[symbol];
                SA[buckets[symbol].second - revoffset] = SA[i] - 1;
            }
        }
    }
    return SA;
}


int main() {




    return 0;
}