#include "std_lib_facilities.h"

class Token
{
public:
	char kind;
	double value;
	
	// constructors
	Token(char ch, double val = 0.0) : kind(ch), value(val) {}
};

class Token_stream
{
public:
	// Constructor
	Token_stream() : full(false), buffer(' ') {}
	Token get();				// get a token
	void putback(Token t);		// put a Token back to stream buffer
private:
	bool full;					// is there a Token in the buffer?
	Token buffer;				// where we keep a token from putback()
};

// putback() method puts its argument back to the Token_stream buffer
void Token_stream::putback(Token t)
{
	// precondition check
	if (full) error("putback() into a full buffer");
	buffer = t;		// copy t to buffer
	full = true;	// now buffer is full
}

Token Token_stream::get()
{
	Token temp{ ' ' };	// temp storage for the returns

	// do we already have a Token ready?
	if (full)
	{	
		// remove token from buffer
		full = false;
		return buffer;
	}
	else
	{
		char ch;
		cin >> ch;

		switch (ch)
		{
		case ';':	// end an expression
		case 'q':	// quit program
		case '(': case ')': case '+': case '-': case '*': case '/':
			temp = Token(ch);
			break;
		case '.':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		{
			cin.putback(ch);			// put a digit back to the input stream
			double val;	
			cin >> val;					// read a floating-point number
			return Token{ '8', val };	// '8' represent a number
		}
		default:
			error("Bad token");
		}
	}
	return temp;
}

Token_stream ts;		// provides get() and putback()
// deal with + and -
double expression();	// declaration so that primary() can call expression()
// deal with *, /, and %
double term();	 
// deal with numbers and parentheses
double primary();

int main()
{
	try
	{
		double val = 0;
		while (cin)
		{
			Token t = ts.get();
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

// deal with + and -
double expression()
{
	double left = term();	// read and evaluate a Term
	Token t = ts.get();		// get the next token from token stream

	while (true)
	{
		switch (t.kind)
		{
		case '+':
			left += term();	// evaluate Term then add
			t = ts.get();
			break;
		case '-':
			left -= term();	// evaluate Term then substract
			t = ts.get();
			break;
		default:
			ts.putback(t);	// put t back into the token stream
			return left;	// no more - or +. return the answer
		}
	}
}

// deal with *, /, %
double term()
{
	double left = primary();
	Token t = ts.get();		// get the next token from token stream

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
			ts.putback(t);	// put t back to token stream
			return left;
		}
	}
}

// deal with numbers and parentheses
double primary()
{
	double temp{};	// temp storage for the returns

	Token t = ts.get();
	switch (t.kind)
	{
	case '(':		// handle '(' expression ')'
	{
		double d = expression();
		t = ts.get();
		if (t.kind != ')') error("')' expected");
		return d;	// return expression result to caller
		break;
	}
	case '8':		// '8' represent numbers
		return t.value;	// return the number value to caller
	default:
		error("primary expected");
	}
}