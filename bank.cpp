#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>
using namespace std;

map<string, tuple<string, int>> users; // user: (password, amount)
map<string, string> admins; // admin: password
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
        vector<string> info = split(userInfo, ' ');
        users[info[0]] = make_tuple(info[1], stoi(info[2]));
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
int isNumber(string number){
    return number.find_first_not_of("0123456789") == std::string::npos;
}
void createUserAccount(){
    string username;
    string password;

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
    accountFile << (username + " " + password + " " + '0' + '\n');
    accountFile.close();

    users[username] = make_tuple(password, 0);
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
void checkamount(string user){
    cout << "Your balance is: " << get<1>(users[user]);
}

void displayUserMenu(){
    system("clear");
    int exist = 0;
    int choice;
    do{
        cout << "--------------------" << '\n';
        cout << "USER'S MENU" << '\n';
        cout << "--------------------" << '\n';
        cout << "Please choose a number" << '\n';
        cout << "1. Check Balance" << '\n';
        cout << "2. Transfer Money" << '\n';
        cout << "3. Exist" << '\n';
        cout << "--------------------" << '\n';
        cout << "Your choice: ";
        cin >> choice;
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
                    cout << "Enter The Amount Of Money: ";
                    cin >> amount;
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
                exist = 1;
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
    } while(!exist);
}
void displayAdminMenu(){
    system("clear");
    int exist = 0;
    int choice;
    do{
        cout << "--------------------" << '\n';
        cout << "ADMIN'S MENU" << '\n';
        cout << "--------------------" << '\n';
        cout << "Please choose a number" << '\n';
        cout << "1. Deposit Money" << '\n';
        cout << "2. Check Balance" << '\n';
        cout << "3. Exist" << '\n';
        cout << "--------------------" << '\n';
        cout << "Your choice: ";
        cin >> choice;
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
                        depositFile << currUser + " " + userAccount + " " + amount + ' ' + getDate();
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
                        cout << userAccount << "'s balance: " << get<1>(users[userAccount]);
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
                exist = 1;
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
    } while(!exist);
}
void displayMenu(){
    int exist = 0;
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
        cout << "5. Exist" << '\n';
        cout << "--------------------" << '\n';
        cout << "Your choice: ";
        cin >> choice;
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
                cout << "Create an user account" << '\n';
                createAdminAccount();
                cout << '\n' << "Enter 0 To Continue" << '\n';
                string in;
                cin >> in;
                break;
            }
            case 5: {
                exist = 1;
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
    } while(!exist);
}
void exist(){
    ofstream userFile("./user/account.txt");
    for (auto i : users){
        string account = i.first;
        string password = get<0>(i.second);
        int amount = get<1>(i.second);
        userFile << account + " " + password + " " + to_string(amount) + '\n'; 
    } 
    userFile.close();

    ofstream adminFile("./user/admin.txt");
    for (auto i : admins){
        string adminName = i.first;
        string password = i.second;
        adminFile << adminName + " " + password + '\n';
    }
}
int main(){
    system("clear");
    initiate();
    displayMenu();
    exist();
}