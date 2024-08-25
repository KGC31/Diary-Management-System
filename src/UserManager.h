#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Structure to hold user data
struct UserData {
    string username;
    string password;
};

// Class to manage user operations
class UserManager {
public:
    UserData get_user();

    void set_user(const string& username, const string& password);

    bool authenticate(const string& file_path, const string& username, const string& password);

    // Login function
    void login(const string& file_path);

    // Sign up function
    bool sign_up(const string& file_path);

    // Change password
    bool change_password(const string& file_path, const string& old_password, const string& new_password);

    // Read user data from file
    static vector<UserData> read_users(const string& filePath);

private:
    // Variable to store the currently logged-in user
    static UserData currentUser;
};

#endif // USERMANAGER_H