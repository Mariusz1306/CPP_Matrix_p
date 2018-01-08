#include <math.h>
#include <fstream>
#include "rcmatrix.h"

int main(){
    CMatrix a;
    CMatrix b(5, 5, 1);
    CMatrix c(5, 5, 1, 2);
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<c<<std::endl;
    return 0;
}
