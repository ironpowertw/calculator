#include <iostream>
#include <string>
#include <vector>
#include "calc.h"
using namespace std;

int main()
{
    vector<op> postfixc;
    string infixc;
    
    getline(cin, infixc);
    postfixc = IntoPost(infixc);
    cout << "postfix: ";
    vPrint(postfixc);
    int ans = PosttoAns(postfixc);
    cout << "RESULT: " << ans << endl;
    
}
