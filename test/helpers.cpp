#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include "helpers.h"

using namespace std;
bool isValidDate(string dateStr) {
    // Define a regular expression pattern for the YYYY-MM-DD format
    std::regex datePattern("^\\d{4}-\\d{2}-\\d{2}$");

    // Check if the input string matches the pattern
    if (!std::regex_match(dateStr, datePattern)) {
        return false;
    }

    // Parse the date string to check if it's a valid date
    std::istringstream dateStream(dateStr);
    int year, month, day;
    char dash1, dash2;

    dateStream >> year >> dash1 >> month >> dash2 >> day;
    if (dateStream.fail() || dash1 != '-' || dash2 != '-') {
        // Parsing failed
        return false;
    }

    // Check if the parsed values represent a valid date
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }

    // Additional date validation logic can be added here if needed

    return true;
}

string getDate(){
     // Get the current time point
    auto now = std::chrono::system_clock::now();

    // Convert the time point to a time_t object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert the time_t object to a struct tm
    std::tm tmStruct = *std::localtime(&currentTime);

    // Create a stringstream to format the date and time
    std::stringstream ss;

    // Format the date and time
    ss << std::put_time(&tmStruct, "%Y-%m-%d/%H:%M:%S");

    // Return the formatted string
    return ss.str();
}
vector<string> split(string str, char del){
    // declaring temp string to store the curr "word" upto del
    vector<string> ans;
    string temp = "";
   
    for(int i=0; i<(int)str.size(); i++){
        // If cur char is not del, then append it to the cur "word", otherwise
          // you have completed the word, print it, and start a new word.
        if(str[i] != del){
            temp += str[i];
        }
        else{
            ans.push_back(temp);
            temp = "";
        }
    }
    ans.push_back(temp);
    return ans;
}
string getInputDate(){
    string ans;
    do{
        cout << "Enter a date in format YYYY-MM-DD ";
        cin >> ws;
        getline(cin, ans); 
    }while(!isValidDate(ans));
    return ans;
}
void updateTransmission(unordered_map<string, vector<vector<string>>>& transmissionRecords, string originInfo){
    vector<string> info = split(originInfo, ' ');
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
void updateWithdrawn(unordered_map<string, vector<vector<string>>>& withdrawnRecords, string originInfo){
    vector<string> info = split(originInfo, ' ');
    if (withdrawnRecords.find(info[0]) == withdrawnRecords.end()){
        withdrawnRecords[info[0]] = {info};
    }
    else {
        withdrawnRecords[info[0]].push_back(info);
    }
}
void updateDeposit(unordered_map<string, vector<vector<string>>>& depositRecords, string originInfo){
    vector<string> info = split(originInfo, ' ');
    if (depositRecords.find(info[0]) == depositRecords.end()){
        depositRecords[info[0]] = {info};
    }
    else {
        depositRecords[info[0]].push_back(info);
    }
}
void viewTransmissionHistory(string currUser, string fromDate, string toDate,  unordered_map<string, vector<vector<string>>>& transmissionRecords,  unordered_map<string, tuple<string, int, string>>& users) {
    vector<vector<string>> transmissionHistoryOfUser = transmissionRecords[currUser];
    int exist = 0;
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    for (auto record : transmissionHistoryOfUser){
        string fromAccount = record[0];
        string toAccount = record[1];
        string amount = record[2];
        string date = split(record[3], '/')[0];
        string time = split(record[3], '/')[1];
        if (date < fromDate){
            continue;
        }
        if (date > toDate){
            break;
        }
        
        if (!exist){
            exist = 1;
        }
        if (currUser == fromAccount){
            cout << "You transfered " + amount + " to " + get<2>(users[toAccount]) + " on " + date + " at " + time << '\n';
        }
        else{
            cout << get<2>(users[fromAccount]) + " transfered " + amount + " to you " + " on " + date + " at " + time << '\n'; 
        }
        
    }
    if (!exist){
            cout << "No records" << endl;
    }
}
void viewWithdrawalHistory(string currUser, string fromDate, string toDate, unordered_map<string, vector<vector<string>>>& withdrawnRecords) {
    vector<vector<string>> withdrawnHistoryOfUser = withdrawnRecords[currUser];
    int exist = 0;
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    for (auto record : withdrawnHistoryOfUser){
        string amount = record[1];
        string date = split(record[2], '/')[0];
        string time = split(record[2], '/')[1];
        if  (date < fromDate){
            continue;
        }
        if (date > toDate){
            break;
        }
        
        if (!exist){
            exist = 1;
        }
        cout << "You withdrawed " + amount + " to" + " on " + date + " at " + time << '\n';
            
        
    }
    if (!exist){
            cout << "No records" << endl;
    }
}
void viewDepositHistory(string currUser, string fromDate, string toDate, unordered_map<string, vector<vector<string>>>& depositRecords) {
    vector<vector<string>> depositHistoryOfUser = depositRecords[currUser];
    int exist = 0;
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    for (auto record : depositHistoryOfUser){
        string amount = record[2];
        string date = split(record[3], '/')[0];
        string time = split(record[3], '/')[1];
        if (date < fromDate){
            continue;
        }
        if (date > toDate){
            break;
        }
        if (!exist){
            exist = 1;
        }
            
        cout << "You deposited " + amount + " to" + " on " + date + " at " + time << '\n';
    }
    if (!exist){
            cout << "No records" << endl;
    }
}
void transferMoney(string currUser,string username, int amount, unordered_map<string, tuple<string, int, string>>& users,unordered_map<string, vector<vector<string>>>& transmissionRecords){
    get<1>(users[currUser]) -= amount;
    get<1>(users[username]) += amount;
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // save transmission
    string date = getDate();
    string originInfo = currUser + " " + username + " " + to_string(amount) + " " + date;
    ofstream transmissionFile;
    transmissionFile.open("./output/transferMoney.txt", ios::app);
    transmissionFile << currUser + " " + username + " " + to_string(amount) + " " + date + '\n';
    transmissionFile.close();
    // update
    updateTransmission(transmissionRecords, originInfo);
    return;
}
void withdrawnMoney(int amount, string currUser, unordered_map<string, tuple<string, int, string>>& users, unordered_map<string, vector<vector<string>>>& withdrawnRecords){
    get<1>(users[currUser]) -= amount;
    string date = getDate();
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ofstream withdrawnFile;
    string info = currUser + " " + to_string(amount) + " " + date;
    withdrawnFile.open("./output/withdrawnMoney.txt", ios::app);
    withdrawnFile << currUser + " " + to_string(amount) + " " + date + '\n';
    withdrawnFile.close();
    updateWithdrawn(withdrawnRecords, info);
    return;
}
void depositMoney(string userAccount, string currUser, string amount,  unordered_map<string, tuple<string, int, string>>& users, unordered_map<string, vector<vector<string>>>& depositRecords){
    get<1>(users[userAccount]) += stoi(amount);
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ofstream depositFile;
    string date = getDate();
    string info = userAccount + " " + currUser + " " + amount + ' ' + date;
    depositFile.open("./output/depositMoney.txt", ios::app);
    depositFile << userAccount + " " + currUser + " " + amount + ' ' + date + '\n';
    depositFile.close();
    updateDeposit(depositRecords, info);
}

void checkBalance(string userAccount, unordered_map<string, tuple<string, int, string>>& users){
    tuple<string, int, string> userInfo = users[userAccount];
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout << get<2>(userInfo) << "'s balance is: " << get<1>(userInfo) << '\n';
}