#include <iostream>
#include <stack>

class variable {
    public:
        int value;
        int type;
        char* lexeme;
};

class forallstmt {
    public:
        variable upper_bound, lower_bound;
};