#include "RecordManager.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// HELPER FUNCTION
void write_records(const vector<Record>& records, const string& file_path) {
    ofstream outfile(file_path);
    if (!outfile) {
        cerr << "SYSTEM ERROR: Could not open file for writing." << endl;
        return;
    }
    for (const auto& record : records) {
        outfile << record.time << '\n'
            << record.content << '\n'
            << '\n'; // Separate records with a blank line
    }
    outfile.close();
}

// ADD RECORD
void RecordManager::add_record(const string& folder_path) {
    Record record;
    char another = 'Y';
    string filename, file_path;

    cout << "\n\n\tENTER DATE OF YOUR RECORD [yyyy-mm-dd]: ";
    getline(cin, filename);
    file_path = folder_path + "/" + filename + ".txt";

    while (another == 'Y' || another == 'y') {
        cout << "\n\tENTER TIME [hh:mm]: ";
        getline(cin, record.time);

        cout << "\tENTER CONTENT: ";
        getline(cin, record.content);

        // Append record to file
        ofstream outfile(file_path, ios::app);
        if (!outfile) {
            cerr << "\nSYSTEM ERROR: Could not open file." << endl;
            return;
        }
        outfile << record.time << '\n'
            << record.content << '\n'
            << '\n'; // Separate records with a blank line
        outfile.close();

        cout << "\nYOUR RECORD IS ADDED...\n";
        cout << "\n\tADD ANOTHER RECORD...(Y/N): ";
        cin >> another;
        cin.ignore(); // To ignore the newline character left in the buffer
    }
    cout << "\n\n\tPRESS ENTER TO EXIT...";
    cin.get();
}

// READ RECORD
void RecordManager::read_record(const string& folder_path) {
    string filename;
    cout << "\n\n\tENTER DATE OF THE RECORD [yyyy-mm-dd]: ";
    getline(cin, filename);
    string file_path = folder_path + "/" + filename + ".txt";

    ifstream infile(file_path);
    if (!infile) {
        cerr << "SYSTEM ERROR: Could not open file." << endl;
        return;
    }

    vector<Record> records;
    Record record;
    string line;

    // Read all records from the file into the vector
    while (getline(infile, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }
        record.time = line;
        getline(infile, record.content);
        getline(infile, line); // Skip the blank line

        records.push_back(record);
    }
    infile.close();

    // Custom sort function to compare times without sscanf
    auto compareTime = [](const Record& a, const Record& b) {
        // Extract hour and minute from time string "hh:mm"
        int hourA = stoi(a.time.substr(0, 2));
        int minuteA = stoi(a.time.substr(3, 2));
        int hourB = stoi(b.time.substr(0, 2));
        int minuteB = stoi(b.time.substr(3, 2));

        // First compare hours, if equal then compare minutes
        if (hourA == hourB) {
            return minuteA < minuteB;
        }
        return hourA < hourB;
        };

    // Implementing selection sort with custom time comparison
    for (int i = 0; i < records.size() - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < records.size(); ++j) {
            if (compareTime(records[j], records[minIndex])) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(records[i], records[minIndex]);
        }
    }

    // Display the sorted records
    cout << "\n\n\t\tRECORDS\n";
    for (const auto& rec : records) {
        cout << "\nTIME: " << rec.time;
        cout << "\nCONTENT: " << rec.content;
        cout << "\n-------------------------\n";
    }

    cout << "\n\n\tPRESS ENTER TO EXIT...";
    cin.get();
}


// EDIT RECORD
void RecordManager::edit_record(const string& folder_path) {
    string filename;
    cout << "\n\n\tENTER DATE OF THE RECORD [yyyy-mm-dd]: ";
    getline(cin, filename);
    string file_path = folder_path + "/" + filename + ".txt";

    ifstream infile(file_path);
    if (!infile) {
        cerr << "SYSTEM ERROR: Could not open file." << endl;
        return;
    }

    vector<Record> records;
    Record record;
    string timeToEdit;
    cout << "\n\n\tENTER TIME OF THE RECORD TO EDIT [hh:mm]: ";
    getline(cin, timeToEdit);

    bool recordFound = false;

    while (getline(infile, record.time)) {
        getline(infile, record.content);  // Read the content line
        infile.ignore();  // Ignore the empty line after content

        if (record.time == timeToEdit) {
            cout << "\tENTER NEW CONTENT: ";
            getline(cin, record.content);
            recordFound = true;  // Mark that the record was found and edited
        }

        records.push_back(record);
    }
    infile.close();

    // If the record was not found, add it to the end
    if (!recordFound) {
        cout << "\n\tNO RECORD FOUND AT " << timeToEdit << ". ADDING NEW RECORD...\n";
        Record newRecord;
        newRecord.time = timeToEdit;
        cout << "\tENTER CONTENT: ";
        getline(cin, newRecord.content);
        records.push_back(newRecord);
    }

    write_records(records, file_path);
    cout << "\nRECORD UPDATED...\n";
    cout << "\n\n\tPRESS ENTER TO EXIT...";
    cin.get();
}


// DELETE RECORD
void RecordManager::delete_record(const string& folder_path) {
    string filename;
    cout << "\n\n\tENTER DATE OF THE RECORD [yyyy-mm-dd]: ";
    getline(cin, filename);
    string file_path = folder_path + "/" + filename + ".txt";

    ifstream infile(file_path);
    if (!infile) {
        cerr << "SYSTEM ERROR: Could not open file." << endl;
        return;
    }

    vector<Record> records;
    Record record;
    string timeToDelete;
    cout << "\n\n\tENTER TIME OF THE RECORD TO DELETE: ";
    getline(cin, timeToDelete);

    // Read records and store them if they don't match the time to delete
    while (getline(infile, record.time)) {
        getline(infile, record.content);  // Read content
        infile.ignore(); // Ignore the empty line after content

        if (record.time != timeToDelete) {
            records.push_back(record);
        }
    }
    infile.close();

    // Write back the records that are not deleted
    ofstream outfile(file_path);
    if (!outfile) {
        cerr << "SYSTEM ERROR: Could not open file for writing." << endl;
        return;
    }
    for (const auto& rec : records) {
        outfile << rec.time << '\n'
            << rec.content << '\n'
            << '\n'; // Separate records with a blank line
    }
    outfile.close();

    cout << "\nRECORD DELETED...\n";
    cout << "\n\n\tPRESS ENTER TO EXIT...";
    cin.get();
}