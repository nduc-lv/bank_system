#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>
using namespace std;
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

bool isValidDate(string dateStr);
string getDate();
vector<string> split(string str, char del);
string getInputDate();
void updateTransmission(unordered_map<string, vector<vector<string>>>& transmissionRecords, string originInfo);
void updateWithdrawn(unordered_map<string, vector<vector<string>>>& withdrawnRecords, string originInfo);
void updateDeposit(unordered_map<string, vector<vector<string>>>& depositRecords, string originInfo);
void viewTransmissionHistory(string currUser, string fromDate, string toDate, unordered_map<string, vector<vector<string>>>transmissionRecords, unordered_map<string, tuple<string, int, string>> users);
void viewWithdrawalHistory(string currUser, string fromDate, string toDate, unordered_map<string, vector<vector<string>>>withdrawnRecords);
void viewDepositHistory(string currUser, string fromDate, string toDate, unordered_map<string, vector<vector<string>>>depositRecords);
void transferMoney(string currUser,string username, int amount, unordered_map<string, tuple<string, int, string>>& users,unordered_map<string, vector<vector<string>>>& transmissionRecords);
void withdrawnMoney(int amount, string currUser, unordered_map<string, tuple<string, int, string>>& users, unordered_map<string, vector<vector<string>>>& withdrawnRecords);
void depositMoney(string userAccount, string currUser, string amount,  unordered_map<string, tuple<string, int, string>>& users, unordered_map<string, vector<vector<string>>>& depositRecords);
void checkBalance(string userAccount, unordered_map<string, tuple<string, int, string>> users);
#endif