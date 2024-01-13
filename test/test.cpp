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
    ifstream accountFile("./data/user.txt");
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
    ofstream userFile("./data/user.txt");
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
void testViewTransmissionHistory(){
    ifstream viewFile("./input/view.txt");
    string inputInfo;
    std::ofstream outputFile("./output/viewTransmission.txt");

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }

    // Redirect std::cout to the file
    std::streambuf *coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(outputFile.rdbuf());
    auto startTime = std::chrono::high_resolution_clock::now();
    while (getline(viewFile, inputInfo)){
        vector<string> info = split(inputInfo, ' ');
        string account = info[0];
        string startDate = info[1];
        string endDate = info[2];
        cout << account << "'s Transmission history" << '\n';
        viewTransmissionHistory(account, startDate, endDate, transmissionRecords, users); 
        cout << "#######################" << '\n';
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout.rdbuf(coutBuffer);
    outputFile.close();
    viewFile.close();
    std::cout << "Time taken to run viewTransmissionHistory 100,000 times: " << duration.count() << " milliseconds" << std::endl;
}
void testViewWithdrawalHistory(){
    ifstream viewFile("./input/view.txt");
    string inputInfo;
    std::ofstream outputFile("./output/viewWithdraw.txt");

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }

    // Redirect std::cout to the file
    std::streambuf *coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(outputFile.rdbuf());
    std::chrono::nanoseconds time = std::chrono::nanoseconds::zero();
    while (getline(viewFile, inputInfo)){
        vector<string> info = split(inputInfo, ' ');
        string account = info[0];
        string startDate = info[1];
        string endDate = info[2];
        auto startTime = std::chrono::high_resolution_clock::now();
        cout << account << "'s Withdrawal history" << '\n';
        viewWithdrawalHistory(account, startDate, endDate, withdrawnRecords); 
        cout << "#######################" << '\n';
        auto endTime = std::chrono::high_resolution_clock::now();
        time += (endTime - startTime);
    }
    // auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time);
    std::cout.rdbuf(coutBuffer);
    outputFile.close();
    viewFile.close();
    std::cout << "Time taken to run viewWithdrawalHistory 100,000 times: " << duration.count() << " milliseconds" << std::endl;
}
void testViewDepositHistory(){
    ifstream viewFile("./input/view.txt");
    string inputInfo;
    std::ofstream outputFile("./output/viewDeposit.txt");

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }

    // Redirect std::cout to the file
    std::streambuf *coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(outputFile.rdbuf());
    auto startTime = std::chrono::high_resolution_clock::now();

    while (getline(viewFile, inputInfo)){
        vector<string> info = split(inputInfo, ' ');
        string account = info[0];
        string startDate = info[1];
        string endDate = info[2];
        cout << account << "'s Deposit history" << '\n';
        viewDepositHistory(account, startDate, endDate, depositRecords);
        cout << "#######################" << '\n';
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout.rdbuf(coutBuffer);
    outputFile.close();
    viewFile.close();
    std::cout << "Time taken to run viewDepositHistory 100,000 times: " << duration.count() << " milliseconds" << std::endl;
}
void testTransferMoney(){
    ifstream inputFile("./input/transferMoney.txt");
    string inputInfo;
    auto startTime = std::chrono::high_resolution_clock::now();
    while (getline(inputFile, inputInfo)){
        vector<string> info = split(inputInfo, ' ');
        string fromAccount = info[0];
        string toAccount = info[1];
        string amount = info[2];
        transferMoney(fromAccount, toAccount, stoi(amount), users, transmissionRecords);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    inputFile.close();
    std::cout << "Time taken to run transferMoney 100,000 times: " << duration.count() << " milliseconds" << std::endl;
}
void testWithdrawMoney(){
    ifstream inputFile("./input/withdrawMoney.txt");
    string inputInfo;
    auto startTime = std::chrono::high_resolution_clock::now();
    while (getline(inputFile, inputInfo)){
        vector<string> info = split(inputInfo, ' ');
        string amount = info[0];
        string account = info[1];
        withdrawnMoney(stoi(amount), account, users, withdrawnRecords);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    inputFile.close();
    std::cout << "Time taken to run withdrawMoney 100,000 times: " << duration.count() << " milliseconds" << std::endl;
}
void testDepositMoney(){
    ifstream inputFile("./input/depositMoney.txt");
    string inputInfo;
    auto startTime = std::chrono::high_resolution_clock::now();
    while (getline(inputFile, inputInfo)){
        vector<string> info = split(inputInfo, ' ');
        string account = info[0];
        string admin = info[1];
        string amount = info[2];
        depositMoney(account, admin, amount, users, depositRecords);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    inputFile.close();
    std::cout << "Time taken to run depositMoney 100,000 times: " << duration.count() << " milliseconds" << std::endl;
}
void testCheckBalance(){
    ifstream inputFile("./input/checkBalance.txt");
    string inputInfo;
    std::ofstream outputFile("./output/checkBalance.txt");

     // Check if the file is opened successfully
     if (!outputFile.is_open()) {
         std::cerr << "Error opening the file!" << std::endl;
         return;
     }

     // Redirect std::cout to the file
     std::streambuf *coutBuffer = std::cout.rdbuf();
     std::cout.rdbuf(outputFile.rdbuf());
    auto startTime = std::chrono::high_resolution_clock::now();
    while (getline(inputFile, inputInfo)){
        cout << inputInfo << "'s balance" << '\n';
        checkBalance(inputInfo, users);
        cout << "#######################" << '\n';
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout.rdbuf(coutBuffer);
    outputFile.close();
    inputFile.close();
    std::cout << "Time taken to run checkBalance 100,000 times: " << duration.count() << " milliseconds" << std::endl;
}
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    initiate();
    // test View Transmission
    testViewTransmissionHistory();
    // test View WithDrawn
    testViewWithdrawalHistory();
    // test View Deposit
    testViewDepositHistory();
    // test Transfer
    testTransferMoney();
    // test Deposit
    testDepositMoney();
    // test Withdraw
    testWithdrawMoney();
    // test CheckBalance
    testCheckBalance();
}