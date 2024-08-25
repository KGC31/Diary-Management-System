#include "UserManager.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <string> 
#include <cstring> 
#include <conio.h>

using namespace std;

// INIT CURRENT USER
UserData UserManager::currentUser;

// GET USER INFO
UserData UserManager::get_user() {
    return this->currentUser;
}

// SET USER INFO
void UserManager::set_user(const string& username, const string& password) {
    this->currentUser.username = username;
    this->currentUser.password = password;
}

bool UserManager::authenticate(const string& file_path, const string& username, const string& password) {
    vector<UserData> users = read_users(file_path);

    // Binary search
    int left = 0;
    int right = users.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (users[mid].username == username) {
            // Username found, now check the password
            if (users[mid].password == password) {
                set_user(users[mid].username, users[mid].password);
                return true;
            }
            else {
                return false;  // Username found, but password is incorrect
            }
        }

        if (users[mid].username < username) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return false;
}

// LOGIN
void UserManager::login(const string& file_path) {
    cout << "\n\n\t***********************************\n";
    cout << "\t  *LOGIN PAGE*\n";
    cout << "\t***********************************";

    string username = "";
    string password = "";
    char ch;

    cout << "\n\n\n\n\n\tEnter Username: ";
    getline(cin, username);

    cout << "\tEnter Password: ";
    while ((ch = _getch()) != 13) {
        if (ch != 8) {
            password.push_back(ch);
            cout << '*';
        }
        else {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
    }

    if (authenticate(file_path, username, password)) {
        cout << "\n\n\nAccess Granted \n";
        system("PAUSE");
        system("CLS");
    }
    else {
        cout << "\nAccess Aborted...\n";
        system("CLS");
        login(file_path);
    };

}

// SIGN UP
bool UserManager::sign_up(const string& file_path) {
    cout << "\n\n\t***********************************\n";
    cout << "\t  *SIGN UP PAGE*\n";
    cout << "\t***********************************";

    string username = "";
    string password = "";
    char ch;

    cout << "\n\n\n\n\n\tEnter Username: ";
    getline(cin, username);

    cout << "\tEnter Password: ";
    while ((ch = _getch()) != 13) {
        if (ch != 8) {
            password.push_back(ch);
            cout << '*';
        }
        else {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
    }
    vector<UserData> users = read_users(file_path);

    // Check if the username already exists
    auto it = find_if(users.begin(), users.end(), [&](const UserData& user) {
        return user.username == username;
    });

    if (it != users.end()) {
        cerr << "Error: Username already exists." << endl;
        sign_up(file_path);
    }

    // Add the new user to the vector
    users.push_back(UserData{ username, password });

    // Create a directory for the user
    if (_mkdir(username.c_str()) != 0) {
        cout << "Error: Could not create directory for the user." << endl;
        system("PAUSE");
        return false;
    }

    // Open the user data file for appending or creating
    ofstream userData(file_path, ios::app);
    if (!userData.is_open()) {
        cerr << "Error: Could not open the user data file for writing." << endl;
        return false;
    }

    // Write the new user's data to the file
    userData << username << endl;
    userData << password << endl;
    userData << endl;

    cout << "\n\n\nAccount created \n";
    system("PAUSE");
    system("CLS");
}

// CHANGE PASSWORD
bool UserManager::change_password(const string& file_path, const string& old_password, const string& new_password) {
    // Get the current user
    UserData currentUser = get_user();

    // Verify the old password
    if (currentUser.password != old_password) {
        cerr << "Error: Incorrect old password." << endl;
        return false;
    }

    vector<UserData> users = read_users(file_path);

    // Find the current user in the user list
    auto it = find_if(users.begin(), users.end(), [&](const UserData& user) {
        return user.username == currentUser.username;
        });

    if (it == users.end()) {
        cerr << "Error: User not found." << endl;
        return false;
    }

    // Update the password
    it->password = new_password;
    set_user(currentUser.username, new_password);

    // Rewrite the updated user list back to the file
    ofstream userData(file_path, ios::trunc); // Truncate the file to rewrite it
    if (!userData.is_open()) {
        cerr << "Error: Could not open the user data file for writing." << endl;
        return false;
    }

    for (const auto& user : users) {
        userData << user.username << endl;
        userData << user.password << endl;
        userData << endl;
    }

    cout << "Password changed successfully." << endl;
    return true;
}


// GET LIST OF USERS
vector<UserData> UserManager::read_users(const string& filePath) {
    ifstream userData(filePath);
    vector<UserData> users;

    if (!userData.is_open()) {
        cerr << "Error: Could not open the user data file." << endl;
        return users;
    }

    string username, password;
    while (getline(userData, username) && getline(userData, password)) {
        users.push_back(UserData{ username, password });

        string emptyLine;
        getline(userData, emptyLine);
    }


    // Sort the users by the first letter of the username
    sort(users.begin(), users.end(), [](const UserData& a, const UserData& b) {
        return a.username[0] < b.username[0];
    });

    return users;
}