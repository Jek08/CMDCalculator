#include "std_lib_facilities.h"

class Token
{
public:
	char kind;
	double value;
private:
	Token ts.get();
	double expression();
	double term();
	double primary();
};

class Token_stream
{
public:
	Token_stream();
	Token get();
	void putback(Token t);
private:

};

double Token::expression()
{
	double left = term();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case '+':
			left += term();
			t = ts.get();
			break;
		case '-':
			left -= term();
			t = ts.get();
			break;
		default:
			ts.putback(t);
			return left;
		}
	}
}

double Token::term()
{
	double left = primary();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case '*':
			left *= primary();
			t = ts.get();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			t = ts.get();
			break;
		}
		default:
			return left;
			break;
		}
	}
}

double Token::primary()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case '(':
	{
		double d = expression();
		t = ts.get();
		if (t.kind != ')') error("')' expected");
		return d;
		break;
	}
	case '8':
		return t.value;
	default:
		error("primary expected");
	}
}

int main()
{
	try 
	{
		double val = 0;
		while (cin)
		{
			token t = ts.get();
			if (t.kind == 'q') break;
			if (t.kind == ';')
				cout << "=" << val << '\n';
			else
				ts.putback(t);
			val = expression();
		}
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