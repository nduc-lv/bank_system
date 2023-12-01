#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <fstream>
using namespace std;

map<string, tuple<string, int>> users; // user: (password, amount)
int author = 0; // 0-not logined, 1-user, 2-admin
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
}
void createAccount(){
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
}
void login(){
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
            break;
        }
    }while (1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
int main(){
    initiate();
    login();
}