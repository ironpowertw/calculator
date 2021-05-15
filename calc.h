/*=========================Calculator Implementation=======================*/
#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;
//	OPERATOR				PRECENDENCE
//	()						1
//
//	!						2				(logical not)
//	~						2				(bitwise not)
//	+						2				(uniary plus)
//	-						2				(uniary minus)
//
//	*						3				(mutiply)
//	/						3				(division)
//	%						3				(modulus)
//
//	+						4				(plus)
//	-						4				(minus)
//
//	<<						5				(bitwise <<)
//	>>						5				(bitwise >>)
//
//	==						6				(logical equal)
//	!=						6				(logical not equal)
//
//	&						7				(bitwise and)
//	^						8				(bitwise xor)
//	|						9				(bitwise or)
//
//	&&						10				(logical and)
//	||						11				(logical or)

class op {
public:
	op(const char&);
	op(const char*, const int&);
	op(const char&, const int&);
	op(const int&);
	void operator=(const op& b);
	bool operator<(const op& b) const;			//if this has a minor precedence (larger prec)
	bool operator>(const op& b) const;			//if this has a prior precedence (smaller prec)
	bool operator<=(const op& b) const;
	bool operator>=(const op& b) const;
	bool operator==(const op& b) const;
	bool operator==(const int& b) const;
	bool operator==(const char* b) const;

	string symb;								//operator symbol
	int prec;									//operator precedence
};

vector<op> IntoPost(string&);
int PosttoAns(vector<op>& postfixexp);
int eval(const int&, const int&, const op&);
int eval(const int&, const op&);
bool isOperator(char t);
op PrecImplent(int& i, const string& infixexp, stack<op>& post);
void sPrint(stack<op> post);
void vPrint(vector<op> post);

vector<op> IntoPost(string& infixexp) {
	stack<op> post;
	stack<int> pmark;												//mark of the existing parentheses
	vector<op> postfixexp;
	bool dswitch = true;											//if needed to create a new <op> for number
																	//true: needed | false: not needed
	pmark.push(0);													//view the whole input as a parentheses
	
	for (int i = 0; i < infixexp.length(); i++) {					//process each character
		//if ((i + 1) % 1000 == 0) { cout << "dealing with:\t\t" << i + 1 << endl; }
		cout << "READ:\t" << infixexp[i] << endl;
		if (isdigit(infixexp[i])) {
			if (dswitch) {											//create new <op>
				op d(infixexp[i]);
				postfixexp.push_back(d);
				dswitch = false;
			}
			else postfixexp.back().symb += infixexp[i];				//direct add the digit into the lasest number
		}
		else if (isOperator(infixexp[i])) {
			dswitch = true;											//turn on dswitch (encounter operator)
			if (infixexp[i] == '(') pmark.push(post.size());
			else if (infixexp[i] == ')') {
				cout << "POP:\t";
				while (post.size() > pmark.top()) {
					cout << post.top().symb << ", ";
					postfixexp.push_back(post.top());
					post.pop();
				}
				cout << endl;
				pmark.pop();
			}
			else {
				op opr = PrecImplent(i, infixexp, post);			//implemt precedence
				if (post.size() == pmark.top()) { 
					post.push(opr);
					cout << "PUSH:\t" << opr.symb << endl;
				}
				else if (post.size() > pmark.top()) {
					if (opr > post.top()) { 
						post.push(opr); 
						cout << "PUSH:\t" << opr.symb << endl;
					}
					else if (opr == 2 && post.top() == 2) { 
						post.push(opr); 
						cout << "PUSH:\t" << opr.symb << endl;
					}
					else if (opr == post.top()) {
						cout << "POP:\t" << post.top().symb << endl;
						postfixexp.push_back(post.top());
						post.pop(); post.push(opr);
						cout << "PUSH:\t" << opr.symb << endl;
					}
					else if (opr < post.top()) {
						cout << "POP:\t";
						while (post.size() > pmark.top()) {
							if (post.top() < opr) { break; }
							cout << post.top().symb << ", ";
							postfixexp.push_back(post.top());
							post.pop();
						}
						cout << endl;
						cout << "PUSH:\t" << opr.symb << endl;
						post.push(opr);
					}
				}
			}
		}
		else dswitch = true;
		//cout << "dealing with:\t\t" << infixexp[i] << endl;
		//cout << "operator stack:\t\t";
		//sPrint(post);
		//cout << "current postfix:\t";
		//vPrint(postfixexp);
		cout << endl;
	}
	cout << "END\t" << "POP:\t";
	while (!post.empty()) {
		cout << post.top().symb << ", ";
		postfixexp.push_back(post.top());
		post.pop();
	}
	cout << endl << endl;

	return postfixexp;
}

int PosttoAns(vector<op>& postfixexp) {
	stack<int> digits;								//stack of digit
	for (int i = 0; i < postfixexp.size(); i++) {
		//cout << "dealing with: " << postfixexp[i].symb << endl;
		if (postfixexp[i] == 0) { 
			//cout << "into digits" << endl;
			digits.push(stoi(postfixexp[i].symb)); 
		}
		else {
			//cout << "math operation" << endl;
			if (postfixexp[i] == 2) {							//only concern one integer (!, ~, +, -)
				int a = digits.top(); digits.pop();
				int c(eval(a, postfixexp[i]));
				//cout << "a = " << a
				//	<< ", opr = " << postfixexp[i].symb
				//	<< ", c = " << c << endl;
					 
				digits.push(c);
			}
			else {
				int b = digits.top(); digits.pop();
				int a = digits.top(); digits.pop();
				int c(eval(a, b, postfixexp[i]));
				//cout << "a = " << a
				//	<< ", b = " << b
				//	<< ", opr = " << postfixexp[i].symb
				//	<< ", c = " << c << endl;
				digits.push(c);
			}
		}
	}

	return digits.top();
}
int eval(const int& a, const int& b, const op& opr) {
	if (opr == "+") { return a + b; }
	if (opr == "-") { return a - b; }
	if (opr == "*") { return a * b; }
	if (opr == "/") { if (b == 0) { return 0; } return a / b; }
	if (opr == "%") { if (b == 0) { return 0; } return a % b; }
	if (opr == "&&") {
		if (a != 0 && b != 0) { return 1; }
		else { return 0; }
	}
	if (opr == "||") {
		if (a != 0 || b != 0) { return 1; }
		else return 0;
	}
	if (opr == "==") {
		if (a == b) { return 1; }
		else return 0;
	}
	if (opr == "!=") {
		if (a != b) { return 1; }
		else return 0;
	}
	if (opr == "&") { return (a & b); }
	if (opr == "^") { return (a ^ b); }
	if (opr == "|") { return (a | b); }
	if (opr == "<<") { return (a << b); }
	if (opr == ">>") { return (a >> b); }
}
int eval(const int& a, const op& opr) {
	if (opr == "~") { return (~a); }
	if (opr == "-") { return (-a); }
	if (opr == "+") { return (+a); }
	if (opr == "!") {
		if (a != 0) return 0;
		else return 1;
	}
}

bool isOperator(char t) {
	if (t == '(' || t == ')' || t == '+' || t == '-' || t == '*' ||
		t == '/' || t == '<' || t == '>' || t == '^' || t == '!' ||
		t == '=' || t == '&' || t == '|' || t == '~' || t == '%') {
		return true;
	}
	return false;
}
void sPrint(stack<op> post) {
	while (!post.empty()) {
		if (post.size() == 1) cout << post.top().symb;
		else cout << post.top().symb << ", ";
		post.pop();
	}
	cout << endl;
}
void vPrint(vector<op> post) {
	for (int i = 0; i < post.size(); i++) { cout << post[i].symb << " "; }
	cout << endl;
}
op PrecImplent(int& i, const string& infixexp, stack<op>& post) {
	if (infixexp.substr(i, 2) == ">>") { op opr(">>", 5); i += 1; return opr; }
	else if (infixexp.substr(i, 2) == "<<") { op opr("<<", 5); i += 1; return opr; }
	else if (infixexp.substr(i, 2) == "&&") { op opr("&&", 10); i += 1; return opr; }
	else if (infixexp.substr(i, 2) == "||") { op opr("||", 11); i += 1; return opr; }
	else if (infixexp.substr(i, 2) == "==") { op opr("==", 6); i += 1; return opr; }
	else if (infixexp.substr(i, 2) == "!=") { op opr("!=", 6); i += 1; return opr; }
	else if (infixexp[i] == '+' && i == 0) { op opr("+", 2); return opr; }
	else if (infixexp[i] == '-' && i == 0) { op opr("-", 2); return opr; }
   	else if (infixexp[i] == '+' && !isdigit(infixexp[i - 1]) && 
			infixexp[i - 1] != ')') { op opr("+", 2); return opr; }
	else if (infixexp[i] == '-' && !isdigit(infixexp[i - 1]) && 
			infixexp[i - 1] != ')') { op opr("-", 2); return opr; }
	else if (infixexp[i] == '!' || infixexp[i] == '~') { op opr(infixexp[i], 2); return opr; }
	else if (infixexp[i] == '*' || infixexp[i] == '/' || infixexp[i] == '%') { op opr(infixexp[i], 3); return opr; }
	else if (infixexp[i] == '+' || infixexp[i] == '-') { op opr(infixexp[i], 4); return opr; }
	else if (infixexp[i] == '&') { op opr(infixexp[i], 7); return opr; }
	else if (infixexp[i] == '^') { op opr(infixexp[i], 8); return opr; }
	else if (infixexp[i] == '|') { op opr(infixexp[i], 9); return opr; }
}
op::op(const char& t) {
	symb = t;
	prec = 0;
}
op::op(const char* t, const int& p) {
	symb = t;
	prec = p;
}
op::op(const char& t, const int& p) {
	symb = ""; symb += t;
	prec = p;
}
op::op(const int& t) {
	symb = to_string(t);
	prec = 0;
}
void op::operator=(const op& b) {
	symb = b.symb;
	prec = b.prec;
}
bool op::operator<(const op& b) const {
	if (prec > b.prec) { return true; }
	return false;
}
bool op::operator>(const op& b) const {
	if (prec < b.prec) { return true; }
	return false;
}
bool op::operator>=(const op& b) const {
	return !(*this < b);
}
bool op::operator<=(const op& b) const {
	return !(*this > b);
}
bool op::operator==(const op& b) const {
	if (prec == b.prec) { return true; }
	return false;
}
bool op::operator==(const int& b) const {
	if (prec == b) { return true; }
	return false;
}
bool op::operator==(const char* b) const {
	if (symb == b) { return true; }
	return false;
}