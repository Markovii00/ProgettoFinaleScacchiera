#include <iostream>
#include <cctype>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <regex>

using namespace std;

short* conversion(smatch& m) 
{
    short fromCol = toupper(m.str(1)[0]) - 65;
    short fromRow = 8 - stoi(m.str(2));
    short toCol = toupper(m.str(3)[0]) - 65;
    short toRow = 8 - stoi(m.str(4));

    short ret[4] = {fromRow, fromCol, toRow, toCol};
    short *array = &ret[0];
    cout << "funziona\n";
    return array;   
}

int main()
{
    
    string input;
    smatch coordinates;

    short a;
    short b;
    short c;
    short d;

    cout << "ao";

    regex input_filter("^([a-hA-H]){1}([1-8]){1} ([a-hA-H]){1}([1-8]){1}$");

    do{
        cout << "\n\nINSERT MOVE : ";
        getline(cin, input); 
        regex_search(input, coordinates, input_filter);
    }while(!regex_match(input, input_filter));

    short *mosse = conversion(coordinates);
                a = *(mosse);
                b = *(mosse + 1);
                c = *(mosse + 2);
                d = *(mosse + 3);

    cout << a << b << c << d;
}
