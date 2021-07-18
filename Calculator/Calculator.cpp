// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "std_lib_facilities.h"

class Token
{
public:
    char kind;
    double value;
};

class Token_stream
{
public:
    Token_stream();         // makes a Token_stream that reads from cin
    Token get();            // get a token
    void putback(Token t);  // put a token back
private: 
    bool full{ false };     // is there a token in the buffer
    Token buffer;           // where we pout token in the buffer
};

void Token_stream::putback(Token t)
{
    if (full) error("putback() into full buffer");
    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;

    switch (ch)
    {
    case ';':
    case 'q':
    case '(': case ')': case '+': case '-': case '*': case '/':
        return Token{ ch };

    case '.':
    case '0': case '1': case '2': case '3': case '4': 
    case '5': case '6': case '7': case '8': case '9':
    {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token{ '8', val };
    }
    default:
        error("Bad token");
    }
}

vector<Token> tok;

Token get_token();
double expression();
double primary();
double term();

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

Token get_token()
{
    Token t;
    return t;
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
            left -= expression();
            t = get_token();
            break;
        default:
            return left;
        }
    }
}

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
    double d;
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
            d = primary();
            if (d == 0) error("divide by zero");
            left /= d;
            t = get_token();
            break;
        default:
            return left;
        }
    }
}