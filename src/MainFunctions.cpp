#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <conio.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include "UserManager.h"
#include "RecordManager.h"

using namespace std;

#define USER_DATA_FILE "users.txt"

UserManager userManager;
RecordManager recordManager;

void change_password() {
    cout << "\n\n\t***********************************\n";
    cout << "\t  *CHANGE PASSWORD*\n";
    cout << "\t***********************************";

    string password = "";
    string new_password = "";
    char ch;

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

    cout << "\tEnter New Password: ";
    while ((ch = _getch()) != 13) {
        if (ch != 8) {
            new_password.push_back(ch);
            cout << '*';
        }
        else {
            if (!new_password.empty()) {
                new_password.pop_back();
                cout << "\b \b";
            }
        }
    }

    if (userManager.change_password(USER_DATA_FILE, password, new_password)) {
        cout << "\n\n\nChange password successfully \n";
        system("PAUSE");
        system("CLS");
    }
    else {
        cout << "\nChange password unsuccessfully...\n";
        system("CLS");
        change_password();  // Retry password change
    };
}

void init() {
    int ch = 0;
    cout << "\n\n\t***********************************\n";
    cout << "\t  *DAILY DIARY MANAGEMENT SYSTEM*\n";
    cout << "\t***********************************";
    while (ch != 1 && ch != 2)
    {
        cout << "\n\n\tLOGIN\t[1]";
        cout << "\n\tSIGN UP\t[2]";
        cout << "\n\n\tENTER YOUR CHOICE: ";
        cin >> ch;
        cin.ignore();
    }
    switch (ch)
    {
    case 1:
        system("cls");
        userManager.login(USER_DATA_FILE);
        break;
    case 2:
        system("cls");
        userManager.sign_up(USER_DATA_FILE);
        break;
    }
    system("cls");
}

void menu() {
    int ch;
    cout << "\n\n\t***********************************\n";
    cout << "\t  *DAILY DIARY MANAGEMENT SYSTEM*\n";
    cout << "\t***********************************";
    while (true)
    {
        cout << "\n\n\t\tMAIN MENU:";
        cout << "\n\n\tADD STORY\t\t[1]";
        cout << "\n\tVIEW STORY\t\t[2]";
        cout << "\n\tEDIT STORY\t\t[3]";
        cout << "\n\tDELETE STORY\t\t[4]";
        cout << "\n\tCHANGE PASSWORD\t\t[5]";
        cout << "\n\tEXIT\t\t\t[6]";
        cout << "\n\n\tENTER YOUR CHOICE: ";
        cin >> ch;
        cin.ignore();
        system("cls");

        switch (ch)
        {
        case 1:
            recordManager.add_record(userManager.get_user().username);
            break;
        case 2:
            recordManager.read_record(userManager.get_user().username);
            break;
        case 3:
            recordManager.edit_record(userManager.get_user().username);
            break;
        case 4:
            recordManager.delete_record(userManager.get_user().username);
            break;
        case 5:
            change_password();
            break; 
        case 6:
            cout << "\n\n\t\tTHANK YOU FOR USING THE SOFTWARE ";
            _getch();
            exit(0);
        default:
            cout << "\nYOU ENTERED WRONG CHOICE..";
            cout << "\nPRESS ANY KEY TO TRY AGAIN";
            _getch();
            break;
        }
        system("cls");
    }
}
