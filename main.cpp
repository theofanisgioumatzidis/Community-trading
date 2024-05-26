#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;

const float lowest_stock_fraction {0.1};

void fetch_stock_prices(std::map<std::string,float> &stock_prices){
    ifstream input_file( "stocks.txt" );
    if (!input_file.is_open()) {
        cerr << "Error loading stock pricing" << endl;
        exit(0);
    }
    string key;
    float value;
    while (input_file >> key >> value) {
        stock_prices[key] = value;
    }
    input_file.close();
}

void login(std::vector<std::string> &login_details, float &balance, std::map<std::string,float> &portfolio){
    while(true){
        // getting login info from user
        string email, password;
        cout << "Please type your login info" << endl;
        cout << "email: ";
        cin >> email;
        cout << "password: ";
        cin >> password;

        
        ifstream input_file( "profiles.txt" );
        if (!input_file.is_open()){
            cerr << "Error opening profiles txt" << endl;
            exit(0);
        }
        
        // retrieve the profile info
        string key,value,rest_of_line;
        bool login_happend{false};
        while (input_file >> key >> value) {
            if(key == email && value == password){
                login_happend = true;
                login_details.push_back(email);
                login_details.push_back(password);
                input_file >> balance;
                getline(input_file,rest_of_line);
                istringstream stream(rest_of_line);
                string name;
                float quantity;
                while (stream >> name >> quantity) {
                    portfolio[name] = quantity;
                }
            }
            input_file.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        input_file.close();
        if(!login_happend){
            char correct;
            cout << "Wrong login info. Do you want to try again? [Yes -> Y, No -> N] ";
            cin >> correct;
            if((correct == 'Y') || (correct == 'y'))
                continue;
            exit(0);
        }
        break;
    }
    
}

void new_register(map<string,float> stock_prices){
    // getting login info from user
    string email, password;
    cout << "Please type your email and a password as login to your new account" << std::endl;
    cout << "email: ";
    cin >> email;
    cout << "password: ";
    cin >> password;

    // checking if the email is on a email format
    const regex pattern
      ("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    if(!regex_match(email, pattern)){
        cerr << email << " is not an proper email address" << endl;
        return;
    }

    // checking if the email is already being used for a different account
    ifstream input_file( "profiles.txt" );
    string file_email,rest_of_line;
    bool login_happend{false};
    while (input_file >> file_email) {
        if(file_email == email ){
            cout << email << " is already being used for an account" << endl;
            return;
        }
        input_file.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    ofstream output_file;
    output_file.open("profiles.txt", ios_base::app);
    output_file << email << " " << password << " 0.0 ";
    for(auto& [key,value] : stock_prices)
        output_file << key << " 0.0 ";

    cout << "New account was created successfully." << endl;

}

void update_profile(vector<string> login_details, float balance, map<string,float> portfolio){
    ifstream input_file( "profiles.txt" );
    if (!input_file.is_open()){
        cerr << "Error opening profiles txt" << endl;
        std::exit(0);
    }

    // copping the contents of the profile file
    vector<string> lines;
    string line;
    int currentLine {}, line_to_update;

    // Read all lines from the file and store them in a vector
    while (getline(input_file, line)) {
        lines.push_back(line);
        istringstream stream(line);
        string email,password;
        stream >> email >> password;
        if(email == login_details[0] && password == login_details[1])
            line_to_update = currentLine;
        currentLine++;
    }
    input_file.close();
    string profile_info;
    profile_info = login_details[0] + ' ' + login_details[1] + ' ';
    profile_info += to_string(round(balance*10)/10);
    profile_info += ' '; 
    for(auto& [key,value] : portfolio){
        profile_info += key + ' ';
        profile_info += std::to_string(round(value*10)/10) + ' ';
    }
    lines[line_to_update] = profile_info;
    ofstream output_file("profiles.txt");
    for (const auto& outputLine : lines) {
        output_file << outputLine << std::endl;
    }  
    output_file.close();
}


void print_situation(float balance, map<string,float> portfolio){
    cout << "Your current balance is : $" << balance << endl;
    cout << "And you own the following stocks : " << endl;
    for (const auto& [key, value] : portfolio){
        if (value < (lowest_stock_fraction/10)) continue;
        cout << value << " stocks of " << key << endl;
    }
    cout << endl;
}

void deposit(float &balance){
    float adding {0.0};
    char correct{'N'};
    cout << "Type the amound of money you want to deposit: ";
    cin >> adding;
    cout << "You are depositing $" << adding << " into your account. Do you want to proceed with the transaction? [Yes -> Y, No -> N] ";
    cin >> correct;
    if((correct == 'Y') || (correct == 'y')) 
        balance += adding;
}
void buy(float &balance,map<std::string,float> &portfolio, std::map<std::string,float> stock_prices){
    string stock;
    float quantity;
    char correct{'N'};

    while(true){
        cout << "The availiable stocks are the following: " << endl;
        for (const auto& [key, value] : stock_prices)
            std::cout << key << " = $" << value << endl;
        cout << "Which stock you want to buy and how many?(You can buy fractions from 0.1 of a stock)" << endl;
        cout << "[type the name of the stock and the quantity with one decimal point]: ";
        cin >> stock >> quantity;
        quantity = round(quantity*10)/10;
        
        if ( (stock_prices[stock]*quantity - balance ) > lowest_stock_fraction){
            cout << "Insufficient funds" << endl;
            cout << "Do you want to deposit funds? [Yes -> Y, No -> N] ";
            cin >> correct;
            cout << endl;
            if((correct == 'Y') || (correct == 'y')){
                deposit(balance);
                continue;
            }
            
            break; 
        }
        cout << "You are purchasing " << quantity << " of " << stock << " stock with total cost of : $" << stock_prices[stock]*quantity << endl;
        cout << "Do you want to proceed with the transaction? [Yes -> Y, No -> N] ";
        cin >> correct;
        if((correct == 'Y') || (correct == 'y')){
            balance -= stock_prices[stock]*quantity;
            portfolio[stock] += quantity;
        }else{
            break;

            }
        break;
    }
    

}

void sell(float &balance , map<string,float> &portfolio , map<string,float> stock_prices ){
    string stock;
    float quantity;
    char correct{'N'};

    while(true){
        print_situation(balance,portfolio);
    
        cout << "Which stock you want to sell and how many?(You can sell fractions from 0.1 of a stock)" << endl;
        cout << "[type the name of the stock and the quantity with one decimal point]: ";
        cin >> stock >> quantity;
        quantity = round(quantity*10)/10;
        
        if ( (quantity - portfolio[stock] ) > lowest_stock_fraction){
            std::cout << "Insufficient amount of stocks" << endl;
            break; 
        }
        cout << "You are selling " << quantity << " of " << stock << " stock with total value of : $" << stock_prices[stock]*quantity << endl;
        cout << "Do you want to proceed with the transaction? [Yes -> Y, No -> N] ";
        cin >> correct;
        if((correct == 'Y') || (correct == 'y')){
            balance += stock_prices[stock]*quantity;
            portfolio[stock] -= quantity;
        }else{
            break;
        }
        break;
    }
}

int main(){

    vector<string> login_details;
    float balance {};
    string action;
    map<string,float> portfolio;
    map<string,float> stock_prices;
    fetch_stock_prices(stock_prices);
    cout << "Wellcome to Community Trader" << endl;
    while(true){
        cout << "Do you have an account or you want to create a new one?" << endl;
        cout << "[Login -> LOGIN, Create an account -> CREATE]";
        string action;
        cin >> action;
        if (action == "LOGIN"){
            login(login_details, balance, portfolio);
            break;
        }else if(action == "CREATE"){
            new_register(stock_prices);
        }else{
            cout << "Non identifiable action" << endl;
            continue;
        }
    }
    
    
    print_situation(balance,portfolio);
    while(true){
        
        cout << "What do you want to do? ACTIONS[ Deposit funds-> DEPOSIT, Buy-> BUY, Sell-> SELL , Stop trading -> STOP]";
        string action;
        cin >> action;
        if (action == "DEPOSIT"){
            deposit(balance);
        }else if ( action == "BUY"){
            buy(balance,portfolio, stock_prices);
        }else if ( action == "SELL"){
            sell(balance,portfolio, stock_prices);       
        //}else if ( action == "LOGOUT"){
           //logout()
        }else if ( action == "STOP"){
           break;
        }else{
            cout << "Non identifiable action" << endl;
            continue;

        }
        update_profile( login_details, balance, portfolio);
        print_situation(balance,portfolio);
    }
    // save changes to the file
    // logout
    //check the limits lowest_stock_fraction
    return 0;
}