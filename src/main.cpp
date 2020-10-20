#include <iostream>
#include "scanner.h"
#include "parser.hpp"
#include "compiler.h"

using namespace mathy;
using namespace std;

int main(int argc, char **argv) {
    mathy::Compiler i;
    int res = i.parse();
    cout << "Parse complete. Result = " << res << endl;
    return res;
}
