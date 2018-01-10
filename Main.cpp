#include <math.h>
#include <fstream>
#include "Matrix.h"

int main(){
    CMatrix a;
    CMatrix b(5, 5, 1);
    CMatrix c(5, 5, 1, 2);
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<c<<std::endl;
	int x1;
	x1 = c[2][1];
	std::cout<<x1<<std::endl;
    return 0;
}
