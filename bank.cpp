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
string currUser;
void initiate(){
    // get users
    ifstream accountFile("./user/account.txt");
    string userInfo;
    while (getline(accountFile, userInfo)){
        vector<string> info = split(userInfo, '+');
        users[info[0]] = make_tuple(info[1], stoi(info[2]), info[3]);
    }
    accountFile.close();

    // get admins
    ifstream adminFile("./user/admin.txt");
    string adminInfo;
    while (getline(adminFile, adminInfo)){
        vector<string> info = split(adminInfo, ' ');
        admins[info[0]] = info[1];
    }
    adminFile.close();

    // get transmission
    ifstream transmissionFile("./user/transmission.txt");
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
    ifstream withdrawnFile("./user/withdrawn.txt");
    string withdrawnInfo;
    while (getline(withdrawnFile, withdrawnInfo)){
        // <from> <to> <amount> <date/time>
        updateWithdrawn(withdrawnRecords, withdrawnInfo);
    }
    withdrawnFile.close();

    // get deposit
    ifstream depositFile("./user/deposit.txt");
    string depositInfo;
    while (getline(depositFile, depositInfo)){
        // <from> <to> <amount> <date/time>
        updateDeposit(depositRecords, depositInfo);
    }
    withdrawnFile.close();
}
void save(){
    ofstream userFile("./user/account.txt");
    for (auto i : users){
        string account = i.first;
        string password = get<0>(i.second);
        string name = get<2>(i.second);
        int amount = get<1>(i.second);
        userFile << account + "+" + password + "+" + to_string(amount) + "+" + name + '\n'; 
    } 
    userFile.close();

    ofstream adminFile("./user/admin.txt");
    for (auto i : admins){
        string adminName = i.first;
        string password = i.second;
        adminFile << adminName + " " + password + '\n';
    }
}
int isNumber(string number){
    return number.find_first_not_of("0123456789") == std::string::npos;
}
bool containsPlusOrWhitespace(const std::string& input) {
    for (char ch : input) {
        if (ch == '+' || std::isspace(ch)) {
            return true;
        }
    }
    return false;
}

void createUserAccount(){
    string username;
    string password;
    string name;
    // create username
    do{
        cout << "Enter your username (username must not have '+' and whitespace characters): ";  
        cin >> ws;
        getline(cin, username);
        if (containsPlusOrWhitespace(username)){
            cout << "Username must not have '+' and whitespace characters" << '\n';
            continue;
        }
        if (users.find(username) != users.end()){
            cout << "Username has already existed, please choose another one" << '\n';
        }
        else{
            break;
        }
    } while(1);
    
    // get name
    cout << "Enter your name: ";
    cin >> ws;  
    getline(cin, name);
    
    // create password
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    do{
        password = "";
        cout << "Enter your password (password must not have '+' characters): ";
        cin >> ws;
        while (1){
            char c = cin.get();
            if (c == '+'){
                cout << "\n" << "Password contains '+' character, please enter a new password '\n'";
                cout << "Enter your password (password must not have '+' characters): ";
                password = "";
                continue;
            }
            if (c == '\n'){
                break;
            }
            password += c;
        }
        cout << '\n';
        cout << "Confirm your password: ";
        string confirmPassword = "";
        cin >> ws;
        while (1){
            char c = cin.get();
            if (c == '\n'){
                break;
            }
            confirmPassword += c;
        }
        cout << '\n';
        if (confirmPassword != password){
            cout << "Not match" << endl;
        }
        else{
            break;
        }
    }while (1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // store account
    ofstream accountFile;
    accountFile.open("./user/account.txt", ios::app);
    accountFile << (username + "+" + password + "+" + "0" + "+" + name + '\n');
    accountFile.close();

    users[username] = make_tuple(password, 0, name);
}
void userLogin(){
    string username;
    string password;
    do{
        cout << "Enter your username: ";
        cin >> ws;
        getline(cin, username);
        if (users.find(username) == users.end()){
            cout << "Username doesn't existed, please try again" << '\n';
        }
        else{
            break;
        }
    } while(1);
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    do{
        password = "";
        cout << "Enter your password: ";
        cin >> ws;
        while (1){
            char c = cin.get();
            if (c == '\n'){
                break;
            }
            password += c;
        }
        cout << '\n';
        if (password == get<0>(users[username])){
            cout << "Logged in" << '\n';
            currUser = username;
            break;
        }
    }while (1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
void adminLogin(){
    string username;
    string password;
    do{
        cout << "Enter your username: ";  
        cin >> username;
        if (admins.find(username) == admins.end()){
            cout << "Username doesn't existed, please try again" << '\n';
        }
        else{
            break;
        }
    } while(1);
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    do{
        password = "";
        cout << "Enter your password: ";
        cin >> ws;
        while (1){
            char c = cin.get();
            if (c == '\n'){
                break;
            }
            password += c;
        }
        cout << '\n';
        if (password == admins[username]){
            cout << "Logged in" << '\n';
            currUser = username;
            break;
        }
    }while (1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
void createAdminAccount(){
    string username;
    string password;

    // create username
    do{
        cout << "Enter your username (username must not have '+' and whitespace characters): ";  
        cin >> ws;
        getline(cin, username);
        if (containsPlusOrWhitespace(username)){
            cout << "Username must not have '+' and whitespace characters" << '\n';
            continue;
        }
        if (admins.find(username) != admins.end()){
            cout << "Username has already existed, please choose another one" << '\n';
        }
        else{
            break;
        }
    } while(1);

    // create password
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    do{
        string bankPassword;
        cout << "Enter bank's password: ";
        cin >> bankPassword;
        cout << '\n';
        if (bankPassword != "hello"){
            cout << "Wrong Password" << '\n';
            continue;
        }
        password = "";
        cout << "Enter your password (password must not have whitespace character): ";
        cin >> ws;
        while (1){
            char c = cin.get();
            if (c == ' '){
                cout << "\n" << "Password contains whitespace character, please enter a new password '\n'";
                cout << "Enter your password (password must not have whitespace characters): ";
                password = "";
                continue;
            }
            if (c == '\n'){
                break;
            }
            password += c;
        }
        cout << '\n';
        cout << "Confirm your password: ";
        string confirmPassword = "";
        cin >> ws;
        while (1){
            char c = cin.get();
            if (c == '\n'){
                break;
            }
            confirmPassword += c;
        }
        cout << '\n';
        if (confirmPassword != password){
            cout << "Not match" << endl;
        }
        else{
            break;
        }
    }while (1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // store account
    ofstream accountFile;
    accountFile.open("./user/admin.txt", ios::app);
    accountFile << (username + " " + password + '\n');
    accountFile.close();

    admins[username] = password;
}
void displayUserMenu(){
    system("clear");
    int exit = 0;
    int choice;
    do{
        cout << "--------------------" << '\n';
        cout << "Hello " << get<2>(users[currUser]) << '\n';
        cout << "USER'S MENU" << '\n';
        cout << "--------------------" << '\n';
        cout << "Please choose a number" << '\n';
        cout << "1. Check Balance" << '\n';
        cout << "2. Transfer Money" << '\n';
        cout << "3. Withdrawn Money" << '\n';
        cout << "4. View Transmission History" << '\n';
        cout << "5. View Withdrawal History" << '\n';
        cout << "6. View Deposit History" << '\n';
        cout << "7. Exit" << '\n';
        string input;
        do{
            cout << "--------------------" << '\n';
            cout << "Your choice: ";
            cin >> input;
            if (!isNumber(input)){
                cout << "Please Enter A Number" << '\n';
                continue;
            }
            else{
                choice = stoi(input);
                break;
            }  
        }while(1);
        
        switch (choice){
            case 1: {
                system("clear");
                cout << "--------------------" << '\n';
                cout << "RESULT" << '\n';
                checkBalance(currUser, users);
                cout << '\n' << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 2: {
                system("clear");
                string userAccount;
                cout << "TRANSFER MONEY" << '\n';
                do {
                    
                    cout << "Enter User's Account: ";
                    cin >> userAccount;
                    cout << '\n';
                    if (userAccount != currUser && users.find(userAccount) != users.end()){
                        break;
                    }
                    else {
                        cout << "Try again" << '\n';
                    }
                } while(1);

                do {
                    string amount;
                    cout << "Enter The Amount Of Money (Or enter 'n' to exit): ";
                    cin >> amount;
                    if (amount == "n"){
                        break;
                    }
                    if (!isNumber(amount)){
                        cout << "Please Enter A Number" << '\n';
                    }
                    else if (stoi(amount) > get<1>(users[currUser])){
                        cout << "You don't have enough money" << '\n';
                        cout << "Enter 0 to continue" << '\n';
                        string tmp;
                        cin >> tmp;
                        break;
                    }
                    else{
                        // enter password
                        termios oldt;
                        tcgetattr(STDIN_FILENO, &oldt);
                        termios newt = oldt;
                        newt.c_lflag &= ~ECHO;
                        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                        string password;
                        do{
                            password = "";
                            cout << "Enter your password: ";
                            cin >> ws;
                            while (1){
                                char c = cin.get();
                                if (c == '\n'){
                                    break;
                                }
                            password += c;
                            }
                            cout << '\n';
                            if (password == get<0>(users[currUser])){
                                break;
                            }
                        }while (1);
                        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                        // transfer money
                        transferMoney(currUser, userAccount, stoi(amount), users,transmissionRecords);
                        cout << "SUCCESSFULLY TRANSFER MONEY" << '\n';
                        cout << "Enter 0 To Return" << '\n';
                        string in;
                        cin >> in;
                        break;
                    }
                }while (1);
                break;
            }
            case 3: {
                system("clear");
                cout << "WITHDRAWN MONEY" << '\n';
                do {
                    string amount;
                    cout << "Enter The Amount Of Money (Or enter 'n' to exit): ";
                    cin >> amount;
                    if (amount == "n"){
                        break;
                    }
                    if (!isNumber(amount)){
                        cout << "Please Enter A Number" << '\n';
                    }
                    else if (stoi(amount) > get<1>(users[currUser])){
                        cout << "You don't have enough money" << '\n';
                        cout << "Enter 0 to continue" << '\n';
                        string tmp;
                        cin >> tmp;
                        break;
                    }
                    else{
                        // enter password
                        termios oldt;
                        tcgetattr(STDIN_FILENO, &oldt);
                        termios newt = oldt;
                        newt.c_lflag &= ~ECHO;
                        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                        string password;
                        do{
                            password = "";
                            cout << "Enter your password: ";
                            cin >> ws;
                            while (1){
                                char c = cin.get();
                                if (c == '\n'){
                                    break;
                                }
                            password += c;
                            }
                            cout << '\n';
                            if (password == get<0>(users[currUser])){
                                break;
                            }
                        }while (1);
                        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                        // transfer money
                        withdrawnMoney(stoi(amount), currUser, users, withdrawnRecords);
                        cout << "SUCCESSFULLY WITHDRAWN MONEY" << '\n';
                        cout << "Enter 0 To Return" << '\n';
                        string in;
                        cin >> in;
                        break;
                    }
                }while (1);
                break;
            }
            case 4: {
                // system("clear");
                // cout << "TRANSMISSION HISTORY" << '\n';
                // log
                system("clear");
                cout << "ENTER THE START DATE" << '\n';
                string fromDate = getInputDate();
                // log
                cout << "ENTER THE END DATE" << '\n';
                string toDate = getInputDate();
                system("clear");
                cout << "TRANSMISSION HISTORY" << '\n';
                viewTransmissionHistory(currUser, fromDate, toDate, transmissionRecords, users);
                cout << '\n' << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 5:{
                system("clear");
                cout << "ENTER THE START DATE" << '\n';
                string fromDate = getInputDate();
                // log
                cout << "ENTER THE END DATE" << '\n';
                string toDate = getInputDate();
                system("clear");
                cout << "WITHDRAWAL HISTORY" << '\n';
                viewWithdrawalHistory(currUser, fromDate, toDate, withdrawnRecords);
                cout << '\n' << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 6:{
                system("clear");
                cout << "ENTER THE START DATE" << '\n';
                string fromDate = getInputDate();
                // log
                cout << "ENTER THE END DATE" << '\n';
                string toDate = getInputDate();
                system("clear");
                cout << "WITHDRAWAL HISTORY" << '\n';
                cout << "DEPOSIT HISTORY" << '\n';
                viewDepositHistory(currUser, fromDate, toDate, depositRecords);
                cout << '\n' << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 7: {
                exit = 1;
                save();
                break;
            }
            default: {
                system("clear");
                cout << "Invalid Number" << '\n';
                cout << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
            }
        }
        system("clear");
    } while(!exit);
}
void displayAdminMenu(){
    system("clear");
    int exit = 0;
    int choice;
    do{
        cout << "--------------------" << '\n';
        cout << "ADMIN'S MENU" << '\n';
        cout << "--------------------" << '\n';
        cout << "Please choose a number" << '\n';
        cout << "1. Deposit Money" << '\n';
        cout << "2. Check Balance" << '\n';
        cout << "3. Exit" << '\n';
         string input;
        do{
            cout << "--------------------" << '\n';
            cout << "Your choice: ";
            cin >> input;
            if (!isNumber(input)){
                cout << "Please Enter A Number" << '\n';
                continue;
            }
            else{
                choice = stoi(input);
                break;
            }  
        }while(1);
        switch (choice){
            case 1: {
                system("clear");
                string userAccount;
                cout << "DEPOSIT MONEY" << '\n';
                do {
                    
                    cout << "Enter User's Account: ";
                    cin >> userAccount;
                    cout << '\n';
                    if (users.find(userAccount) != users.end()){
                        break;
                    }
                    else {
                        cout << "User not found" << '\n';
                    }
                } while(1);

                do {
                    string amount;
                    cout << "Enter The Amount Of Money: ";
                    cin >> amount;
                    cout << '\n';
                    if (!isNumber(amount)){
                        cout << "Please Enter A Number" << '\n';
                    }
                    else{
                        // enter password
                        termios oldt;
                        tcgetattr(STDIN_FILENO, &oldt);
                        termios newt = oldt;
                        newt.c_lflag &= ~ECHO;
                        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                        string password;
                        do{
                            password = "";
                            cout << "Enter admin's password: ";
                            cin >> ws;
                            while (1){
                                char c = cin.get();
                                if (c == '\n'){
                                    break;
                                }
                            password += c;
                            }
                            cout << '\n';
                            if (password == admins[currUser]){
                                break;
                            }
                        }while (1);
                        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                        // deposit money
                        depositMoney(userAccount, currUser, amount, users, depositRecords);
                        cout << "SUCCESSFULLY DEPOSIT MONEY" << '\n';
                        cout << "Enter 0 To Return" << '\n';
                        string in;
                        cin >> in;
                        break;
                    }
                }while (1);
                break;
            }
            case 2: {
                system("clear");
                string userAccount;
                cout << "CHECK BALANCE" << '\n';
                do {
                    
                    cout << "Enter User's Account: ";
                    cin >> userAccount;
                    cout << '\n';
                    if (users.find(userAccount) != users.end()){
                        checkBalance(userAccount, users);
                        break;
                    }
                    else {
                        cout << "User not found" << '\n';
                    }
                } while(1);
                cout << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 3: {
                exit = 1;
                save();
                break;
            }
            default: {
                system("clear");
                cout << "Invalid Number" << '\n';
                cout << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
            }
        }
        system("clear");
    } while(!exit);
}
void displayMenu(){
    int exit = 0;
    int choice;
    do{
        cout << "--------------------" << '\n';
        cout << "MENU" << '\n';
        cout << "--------------------" << '\n';
        cout << "Please choose a number" << '\n';
        cout << "1. Login as an user" << '\n';
        cout << "2. Login as an admin" << '\n';
        cout << "3. Create an user account" << '\n';
        cout << "4. Create an admin account" << '\n';
        cout << "5. Exit" << '\n';
        string input;
        do{
            cout << "--------------------" << '\n';
            cout << "Your choice: ";
            cin >> input;
            if (!isNumber(input)){
                cout << "Please Enter A Number" << '\n';
                continue;
            }
            else{
                choice = stoi(input);
                break;
            }  
        }while(1);
        switch (choice){
            case 1: {
                system("clear");
                cout << "--------------------" << '\n';
                cout << "Login as an user" << '\n';
                userLogin();
                cout << '\n' << "Enter 0 To Continue" << '\n';
                string in;
                cin >> in;
                displayUserMenu();
                break;
            }
            case 2: {
                system("clear");
                cout << "--------------------" << '\n';
                cout << "Login as an admin" << '\n';
                adminLogin();
                cout << '\n' << "Enter 0 To Continue" << '\n';
                string in;
                cin >> in;
                displayAdminMenu();
                break;
            }
            case 3:{
                system("clear");
                cout << "--------------------" << '\n';
                cout << "Create an user account" << '\n';
                createUserAccount();
                cout << '\n' << "Enter 0 To Continue" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 4:{
                system("clear");
                cout << "--------------------" << '\n';
                cout << "Create an admin account" << '\n';
                createAdminAccount();
                cout << '\n' << "Enter 0 To Continue" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 5: {
                exit = 1;
                break;
            }
            default: {
                system("clear");
                cout << "Invalid Number" << '\n';
                cout << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
            }
        }
        system("clear");
    } while(!exit);
}
int main(){
    system("clear");
    initiate();
    displayMenu();
}