#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <vector>
#include <map>
#include <sys/resource.h>
#include <iomanip>

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

int search(const string &text, const string &pattern, const vector<int> &SA) {
    int left = 0;
    int right = text.length() - 1;
    int first_occurrence = -1;

    while (left <= right) {
        int mid = (left + right) / 2;
        string suffix = text.substr(SA[mid]);
        if (suffix.compare(0, pattern.length(), pattern) == 0) {
            first_occurrence = mid;
            right = mid - 1; 
        }
        else if (pattern < suffix) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }

    return first_occurrence;
}

vector<int> find_all_occurrences(const string &text, const string &pattern, const vector<int> &SA) {
    int firstOccurrence = search(text, pattern, SA);
    if (firstOccurrence == -1) {
        return vector<int>(1, -1);
    }
    int i = firstOccurrence;
    vector<int> occurrences;
    while (text.substr(SA[i]).compare(0, pattern.length(), pattern) == 0) {
        occurrences.push_back(SA[i]);
        i++;
    }
    sort(occurrences.begin(), occurrences.end());
    return occurrences;
}

string read_file_content(const string &filename) {
    ifstream inputFile(filename);
    string text;
    char c;
    while (inputFile.get(c)) {
        if (isalpha(c))
            text += toupper(c);
    }
    inputFile.close();
    return text;
}

void print_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    long memory_usage = usage.ru_maxrss;
    double memory_usage_mib = static_cast<double>(memory_usage) / 1024;
    cout << "Memoria usada: " << fixed << setprecision(3) << memory_usage_mib << " [MiB]" << endl;
}

void write_suffix_array(const vector<int> &SA, const string &outputSA_filename) {
    ofstream outputFile(outputSA_filename);
    outputFile << "[" << SA[0];
    for (int i = 1; i < SA.size(); i++)
        outputFile << ", " << SA[i];
    outputFile << "]";
    outputFile.close();
    cout << "Arreglo de sufijos creado satisfactoriamente"  << endl;
}

void write_total_occurrences(const vector<int> &occurrences, const string &outputOcurrences_filename) {
    ofstream outputFile(outputOcurrences_filename);
    outputFile << "[" << occurrences[0];
    for (int i = 1; i < occurrences.size(); i++)
        outputFile << ", " << occurrences[i];
    outputFile << "]";
    outputFile.close();
    cout << "Arreglo de las posiciones de la cadena y sus ocurrencias creado satisfactoriamente" << endl;
}

int main() {
    auto startTime = chrono::high_resolution_clock::now();
    string inputBook = "Libros/one.txt";
    string outputSA = "Output_CPlusPlus/outputSA_Cpp.txt";
    string stringPatternInput = "searchString.txt";
    string stringS2S = read_file_content(stringPatternInput);
    string textSA = read_file_content(inputBook);
    textSA += '$';

    vector<int> T(textSA.begin(), textSA.end());

    vector<int> SA = sais(T);

    write_suffix_array(SA, outputSA);

    string inputS2S_filename = "searchString.txt";
    string outputOcurrences = "Output_CPlusPlus/outputS2S_Cpp.txt";
    string textS2S = read_file_content(inputS2S_filename);

    vector<int> ocurrences = find_all_occurrences(textSA, textS2S, SA);

    write_total_occurrences(ocurrences, outputOcurrences);
    cout << "Total de Ocurrencias: " << ocurrences.size() << endl;

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = endTime - startTime;
    cout << "Duración del programa en: " << elapsed_time.count() << " segundos" << endl;

    print_memory_usage();

    return 0;
}

