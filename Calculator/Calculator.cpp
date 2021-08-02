/*
	Simple Calculator

	Originally written by Bjarne Stroustrup
	Rewritten by Jaka Pratama Widiyanto

	This program implements a basic expression calculator.
	Input from cin, output to cout.
	The grammar for input is

	Statement:
		Expression
		Print
		Quit
	Print:
		;
	Quit:
		q
	Expression:
		Term 
		Expression + Term
		Expression - Term
	Term:
		Primary
		Term * Primary
		Term / Primary
		Term % Primary
	Primary
		Number
		( Expression )
		- Primary
		+ Primary
	Number:
		floating-point literal

	Input comes from cin through the Token_stream called ts.
	
	The grammar for calculator with variable:
	Calculation:
		Statement
		Print
		Quit
		Calculation Statement
	Statement:
		Declaration
		Expression
	Declaration:
		"let" Name "=" Expression
*/

#include "std_lib_facilities.h"

// constants
const char number = '8';
const char quit = 'q';
const char print = ';';
const char prompt = '>';
const char result = '=';

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
	void ignore(char c);
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
		case '(': 
		case ')': 
		case '+': 
		case '-': 
		case '*': 
		case '/': 
		case '%':
			return Token{ch};
			break;
		case '.':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		{
			cin.putback(ch);			// put a digit back to the input stream
			double val;	
			cin >> val;					// read a floating-point number
			return Token{ number, val };
		}
		default:
			error("Bad token");
		}
	}
}

void Token_stream::ignore(char c)
	// c represents kind of token
{
	if (full && c == buffer.kind)
	{
		full = false;
		return;
	}
	full = false;

	// search input
	char ch = 0;
	while (cin >> ch)
	{
		if (ch == c) return;
	}
}

class Variable
{
public:
	string name;
	double value;
};

Token_stream ts;		// provides get() and putback()
// deal with + and -
double expression();	// declaration so that primary() can call expression()
// deal with *, /, and %
double term();	 
// deal with numbers and parentheses
double primary();
// get expression from user and start process
void calculate();
// clean up buffer
void clean_up_mess();
// handle expression and declaration
double statement();

Vector<Variable> var_table;

double get_value(string s)
	// return the value of the variable named s
{
	for (const Variable& v : var_table)
		if (v.name == s) return v.value;
	error("get: undefined variable ", s);
}

void set_value(string s, double d)
	// set the value of variable named s to d
{
	for (Variable& v : var_table)
	{
		if (v.name == s)
		{
			v.value = d;
			return;
		}
	}
	error("set: undefined variable ", s);
}

int main()
{
	try
	{
		calculate();
		keep_window_open();
		return 0;
	}
	catch (exception& e)
	{
		cerr << e.what() << '\n';
		keep_window_open("~~");
		return 1;
	}
	catch (...)
	{
		cerr << "exception \n";
		keep_window_open("~~");
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
		case '%':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left = fmod(left, d);
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
	case number:
		return t.value;	// return the number value to caller
	case '-':
		return -primary();
	case '+':
		return primary();
	default:
		error("primary expected");
	}
}

// deal with user input and start evaluate the expression
void calculate()
{
	while (cin)
	try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t = ts.get();	// discard all print
		if (t.kind == quit) return;
		ts.putback(t);
		cout << result << statement() << '\n';
	}
	catch (exception& e)
	{
		cerr << e.what() << '\n';	// write error messages
		clean_up_mess();
	}
}

void clean_up_mess()
{
	while (cin)
	{
		ts.ignore(print);
	}
}

double statement()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case let:
		return declaration();
	default:
		ts.putback(t);
		return expression();
	}
}

bool is_declared(string var)
	// is var already in var_table?
{
	for (const Variable& v : var_table)
		if (v.name == var) return true;
	return false;
}

double define_name(string var, string val)
{
	if (is_declared(var)) error(var, " declared twice");
	var_table.push_back(Variable(var, val));
	return val;
}

double declaration()
	// assume we have seen "let"
	// handle: name = expression
	// declare a variable called "name" with the initial value "expression"
{
	Token t = ts.get();
	if (t.kind == name) error("name expected in declaration");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of ", var_name);

	double d = expression();
	define_name(var_name, d);
	return d;
}