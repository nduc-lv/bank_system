#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include "helpers.h"
using namespace std;
unordered_map<string, tuple<string, int, string>> users; // user: (password, amount, name)
unordered_map<string, string> admins; // admin: password
unordered_map<string, vector<vector<string>>> transmissionRecords;
unordered_map<string, vector<vector<string>>> withdrawnRecords;
unordered_map<string, vector<vector<string>>> depositRecords;
void initiate(){
    // get users
    ifstream accountFile("./data/account.txt");
    string userInfo;
    while (getline(accountFile, userInfo)){
        vector<string> info = split(userInfo, '+');
        users[info[0]] = make_tuple(info[1], stoi(info[2]), info[3]);
    }
    accountFile.close();

    // get admins
    ifstream adminFile("./data/admin.txt");
    string adminInfo;
    while (getline(adminFile, adminInfo)){
        vector<string> info = split(adminInfo, ' ');
        admins[info[0]] = info[1];
    }
    adminFile.close();

    // get transmission
    ifstream transmissionFile("./data/transmission.txt");
    string transmissionInfo;
    while (getline(transmissionFile, transmissionInfo)){
        // <from> <to> <amount> <date/time>
        vector<string> info = split(transmissionInfo, ' ');
        // send history
        if (transmissionRecords.find(info[0]) == transmissionRecords.end()){
            transmissionRecords[info[0]] = {info};
        }
        else {
            transmissionRecords[info[0]].push_back(info);
        }
        // receive history
        if (transmissionRecords.find(info[1]) == transmissionRecords.end()){
            transmissionRecords[info[1]] = {info};
        }
        else {
            transmissionRecords[info[1]].push_back(info);
        }
    }
    transmissionFile.close();

    // get withdrawn
    ifstream withdrawnFile("./data/withdrawn.txt");
    string withdrawnInfo;
    while (getline(withdrawnFile, withdrawnInfo)){
        // <from> <to> <amount> <date/time>
        updateWithdrawn(withdrawnRecords, withdrawnInfo);
    }
    withdrawnFile.close();

    // get deposit
    ifstream depositFile("./data/deposit.txt");
    string depositInfo;
    while (getline(depositFile, depositInfo)){
        // <from> <to> <amount> <date/time>
        updateDeposit(depositRecords, depositInfo);
    }
    withdrawnFile.close();
}
void save(){
    ofstream userFile("./data/account.txt");
    for (auto i : users){
        string account = i.first;
        string password = get<0>(i.second);
        string name = get<2>(i.second);
        int amount = get<1>(i.second);
        userFile << account + "+" + password + "+" + to_string(amount) + "+" + name + '\n'; 
    } 
    userFile.close();

    ofstream adminFile("./data/admin.txt");
    for (auto i : admins){
        string adminName = i.first;
        string password = i.second;
        adminFile << adminName + " " + password + '\n';
    }
}