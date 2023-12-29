#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>

using namespace std;

unordered_map<string, tuple<string, int, string>> users; // user: (password, amount, name)
unordered_map<string, string> admins; // admin: password
int author = 0; // 0-not logined, 1-user, 2-admin
string currUser;
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
void initiate(){
    ifstream accountFile("./user/account.txt");
    string userInfo;
    while (getline(accountFile, userInfo)){
        vector<string> info = split(userInfo, '+');
        users[info[0]] = make_tuple(info[1], stoi(info[2]), info[3]);
    }
    accountFile.close();

    ifstream adminFile("./user/admin.txt");
    string adminInfo;
    while (getline(adminFile, adminInfo)){
        vector<string> info = split(adminInfo, ' ');
        admins[info[0]] = info[1];
    }
    adminFile.close();
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
void createUserAccount(){
    string username;
    string password;
    string name;
    // create username
    do{
        cout << "Enter your username: ";  
        cin >> username;
        if (users.find(username) != users.end()){
            cout << "Username has already existed, please choose another one" << '\n';
        }
        else{
            break;
        }
    } while(1);
    
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
        cin >> username;
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
        cout << "Enter your username: ";  
        cin >> username;
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
        cout << "Enter your password: ";
        cin >> password;
        cout << '\n';
        cout << "Confirm your password: ";
        string confirmPassword;
        cin >> confirmPassword;
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
void viewTranmissionHistory(){
    ifstream tranmissionFile("./user/transmission.txt");
    string line;
    while (getline(tranmissionFile, line)){
        vector<string> arr = split(line, ' ');
        string fromUser = arr[0];
        string toUser = arr[1];
        if (currUser == fromUser){
            cout << "You transfered " + arr[2] + " to " + get<2>(users[arr[1]]) + " on " + split(arr[3], '/')[0] + " at " + split(arr[3], '/')[1] << '\n'; 
        }
        else if (currUser == toUser){
            cout << get<2>(users[fromUser]) + " transfered " + arr[2] + " to you " + " on " + split(arr[3], '/')[0] + " at " + split(arr[3], '/')[1] << '\n'; 
        }
    }
}
void viewWithdrawalHistory(){
    ifstream withdrawnFile("./user/withdrawn.txt");
    string line;
    while (getline(withdrawnFile, line)){
        vector<string> arr = split(line, ' ');
        if (arr[0] != currUser){
            continue;
        }
        cout << "You withdrawned " + arr[1] + " on " + split(arr[2], '/')[0] + " at " + split(arr[2], '/')[1] << '\n';
    }
}
void viewDepositHistory(){
    ifstream depositFile("./user/deposit.txt");
    string line;
    while (getline(depositFile, line)){
        vector<string> arr = split(line, ' ');
        if (arr[0] != currUser){
            continue;
        }
        cout << "You deposited " + arr[2] + " on " + split(arr[3], '/')[0] + " at " + split(arr[3], '/')[1] << '\n';
    }
}
void transferMoney(string username, int amount){
    get<1>(users[currUser]) -= amount;
    get<1>(users[username]) += amount;
    string date = getDate();
    ofstream transmissionFile;
    transmissionFile.open("./user/transmission.txt", ios::app);
    transmissionFile << currUser + " " + username + " " + to_string(amount) + " " + date + '\n';
    transmissionFile.close();
    return;
}
void withdrawnMoney(int amount){
    get<1>(users[currUser]) -= amount;
    string date = getDate();
    ofstream transmissionFile;
    transmissionFile.open("./user/withdrawn.txt", ios::app);
    transmissionFile << currUser + " " + to_string(amount) + " " + date + '\n';
    transmissionFile.close();
    return;
}
void checkamount(string user){
    cout << "Your balance is: " << get<1>(users[user]);
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
                checkamount(currUser);
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
                        transferMoney(userAccount, stoi(amount));
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
                        withdrawnMoney(stoi(amount));
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
                system("clear");
                cout << "TRANSMISSION HISTORY" << '\n';
                viewTranmissionHistory();
                cout << '\n' << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 5:{
                system("clear");
                cout << "WITHDRAWAL HISTORY" << '\n';
                viewWithdrawalHistory();
                cout << '\n' << "Enter 0 To Return" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 6:{
                system("clear");
                cout << "DEPOSIT HISTORY" << '\n';
                viewDepositHistory();
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
                        // transfer money
                        get<1>(users[userAccount]) += stoi(amount);
                        ofstream depositFile;
                        depositFile.open("./user/deposit.txt", ios::app);
                        depositFile << userAccount + " " + currUser + " " + amount + ' ' + getDate() + '\n';
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
                        cout << userAccount << "'s balance: " << get<1>(users[userAccount]) << '\n';
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