#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


// Structure to hold user data
struct Record {
    string time;
    string content;
};

// Class to manage user operations
class RecordManager {
public:
    void add_record(const string& folder_path);

    void read_record(const string& folder_path);

    void edit_record(const string& folder_path);

    void delete_record(const string& folder_path);
};

#endif // RECORDMANAGER_H