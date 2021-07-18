// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "std_lib_facilities.h"

class Token
{
public:
    char kind;
    double value;
};

Token get_token()
{

}

vector<Token> tok;

double primary()
{
    Token t = get_token();
    switch (t.kind)
    {
    case '(':
    {   double d = expression();
    t = get_token();
    if (t.kind != ')') error("')' expected");        
    return d;
    }
    case '8':
        return t.value;
    default:
        error("primary expected");
    }
}

double term()
{
    double left = primary();
    Token t = get_token();
    while (true)
    {
        switch (t.kind)
        {
        case '*':
            left *= primary();
            t = get_token();
            break;
        case '/':
            double d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            t = get_token();
            break;
        default:
            return left;
        }
    }
}

double expression()
{
    double left = term();
    Token t = get_token();
    while (true)
    {
        switch (t.kind)
        {
        case '+':
            left += expression();
            t = get_token();
            break;
        case '-':
            left - +expression();
            t = get_token();
            break;
        default:
            return left;
        }
    }
}

int main()
{
    try
    {
        while (cin)
            cout << expression() << '\n';
        keep_window_open();
    }
    catch (exception& e)
    {
        cerr << e.what() << '\n';
        keep_window_open();
        return 1;
    }
    catch (...)
    {
        cerr << "exception \n";
        keep_window_open();
        return 2;
    }
    return 0;
}