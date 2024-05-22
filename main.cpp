#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
const float lowest_stock_fraction {0.1};



void print_situation(float balance, std::map<std::string,float> porfolio){
    std::cout << "Your current balance is : $" << balance << std::endl;
    std::cout << "And you own the following stocks : " << std::endl;
    for (const auto& [key, value] : porfolio){
        if (value < (lowest_stock_fraction/10)) continue;
        std::cout << value << " stocks of " << key << std::endl;
    }
    std::cout << std::endl;
}

void deposit(float &balance){
    float adding {0.0};
    char correct{'N'};
    std::cout << "Type the amound of money you want to deposit: ";
    std::cin >> adding;
    std::cout << "You are depositing $" << adding << " into your account. Do you want to proceed with the transaction? [Yes -> Y, No -> N] ";
    std::cin >> correct;
    if((correct == 'Y') || (correct == 'y')) 
        balance += adding;
}
void buy(float &balance,std::map<std::string,float> &porfolio, std::map<std::string,float> stock_prices){
    std::string stock;
    float quantity;
    char correct{'N'};

    while(true){
        std::cout << "The availiable stocks are the following: " << std::endl;
        for (const auto& [key, value] : stock_prices)
            std::cout << key << " = $" << value << std::endl;
        std::cout << "Which stock you want to buy and how many?(You can buy fractions from 0.1 of a stock)" << std::endl;
        std::cout << "[type the name of the stock and the quantity with one decimal point]: ";
        std::cin >> stock >> quantity;
        quantity = round(quantity*10)/10;
        
        if ( (stock_prices[stock]*quantity - balance ) > lowest_stock_fraction){
            std::cout << "Insufficient funds" << std::endl;
            std::cout << "Do you want to deposit funds? [Yes -> Y, No -> N] ";
            std::cin >> correct;
            std::cout << std::endl;
            if((correct == 'Y') || (correct == 'y')){
                deposit(balance);
                continue;
            }
            
            break; 
        }
        std::cout << "You are purchasing " << quantity << " of " << stock << " stock with total cost of : $" << stock_prices[stock]*quantity << std::endl;
        std::cout << "Do you want to proceed with the transaction? [Yes -> Y, No -> N] ";
        std::cin >> correct;
        if((correct == 'Y') || (correct == 'y')){
            balance -= stock_prices[stock]*quantity;
            porfolio[stock] += quantity;
        }else{
            std::cout << " BREAK " << std::endl;
            break;

            }
        break;
    }
    

}

void sell(float &balance , std::map<std::string,float> &porfolio , std::map<std::string,float> stock_prices ){
    std::string stock;
    float quantity;
    char correct{'N'};

    while(true){
        print_situation(balance,porfolio);
    
        std::cout << "Which stock you want to sell and how many?(You can sell fractions from 0.1 of a stock)" << std::endl;
        std::cout << "[type the name of the stock and the quantity with one decimal point]: ";
        std::cin >> stock >> quantity;
        quantity = round(quantity*10)/10;
        
        if ( (quantity - porfolio[stock] ) > lowest_stock_fraction){
            std::cout << "Insufficient amount of stocks" << std::endl;
            break; 
        }
        std::cout << "You are selling " << quantity << " of " << stock << " stock with total value of : $" << stock_prices[stock]*quantity << std::endl;
        std::cout << "Do you want to proceed with the transaction? [Yes -> Y, No -> N] ";
        std::cin >> correct;
        if((correct == 'Y') || (correct == 'y')){
            balance += stock_prices[stock]*quantity;
            porfolio[stock] -= quantity;
        }else{
            //std::cout << " BREAK " << std::endl;
            break;

            }
        break;
    }
    

}

int main(){


    float balance {};
    std::map<std::string,float> porfolio {{"TESLA", 0.0}, {"TOYOTA", 0.0}, {"MICROSOFT", 0.0}};
    std::map<std::string,float> stock_prices {{"TESLA", 275.3}, {"TOYOTA", 55.0}, {"MICROSOFT", 77.7}};

    std::cout << "Wellcome to Community Trader"<< std::endl;
    print_situation(balance,porfolio);
    std::string action;
    while(true){
        
        std::cout << "What do you want to do? ACTIONS[ Deposit funds-> DEPOSIT, Buy-> BUY, Sell-> SELL , Stop trading-> STOP]";
        std::cin >> action;
        if (action == "STOP") break;
        if (action == "DEPOSIT"){
            deposit(balance);
        }else if ( action == "BUY"){
            buy(balance,porfolio, stock_prices);
        }else if ( action == "SELL"){
            sell(balance,porfolio, stock_prices);       
        }else if ( action == "STOP"){
           // return 0;
        }   
        print_situation(balance,porfolio);
    }
    // actions: incert money, buy, sell, show porfolio
    //offline saving of the balance porfolio ect
    return 0;
}