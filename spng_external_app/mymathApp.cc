#include <iostream>
#include "spng.h"

using namespace std;

int main(int argc, char const *argv[])
{
    char const * a =  spng_version_string();
//    int b = add(1, 1);
    cout << "spng_version_string : " << a <<endl;
//    cout << "1加1等于" << b <<endl;
    return 0;
}
