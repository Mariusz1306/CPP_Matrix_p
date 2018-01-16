#include <math.h>
#include <fstream>
#include "Matrix.h"

int main(){
    CMatrix a(2,2,1);
    std::cout << "a:" << std::endl<< a << std::endl;
    CMatrix b(2,2,2);
    std::cout << "b:" << std::endl<< b << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "a:" << std::endl<< a << std::endl;
    std::cout << "b:" << std::endl<< b << std::endl;
    CMatrix c = a + b;
    std::cout << "c = a + b" << std::endl<< c << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "a:" << std::endl<< a << std::endl;
    std::cout << "b:" << std::endl<< b << std::endl;
    CMatrix d = a - b;
    std::cout << "d = a - b" << std::endl<< d << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "a:" << std::endl<< a << std::endl;
    std::cout << "b:" << std::endl<< b << std::endl;
    CMatrix e = a * b;
    std::cout << "e = a * b" << std::endl<< e << std::endl;
    std::cout << "------------------" << std::endl;
    CMatrix f(a);
    std::cout << "f:" << std::endl<< f << std::endl;
    f += b;
    std::cout << "f += b" << std::endl<< f << std::endl;
    std::cout << "a:" << std::endl<< a << std::endl;
    std::cout << "------------------" << std::endl;
    CMatrix g(a);
    std::cout << "g:" << std::endl<< g << std::endl;
    g -= b;
    std::cout << "g -= b" << std::endl<< g << std::endl;
    std::cout << "a:" << std::endl<< a << std::endl;
    std::cout << "------------------" << std::endl;
    CMatrix h(a);
    std::cout << "h:" << std::endl<< h << std::endl;
    std::cout << "b:" << std::endl<< b << std::endl;
    h *= b;
    std::cout << "h *= b" << std::endl<< h << std::endl;
    std::cout << "a:" << std::endl<< a << std::endl;
    std::cout << "------------------" << std::endl;
    CMatrix z;
    z = a;
    std::cout << "a:" << std::endl<< a << std::endl;
    std::cout << "z:" << std::endl<< z << std::endl;
    std::cout << "a==z:" << std::endl<< (a==z) << std::endl;
    std::cout << "a==b:" << std::endl<< (a==b) << std::endl;

    std::fstream fs;
    fs.open ("test.txt", std::fstream::in);
    CMatrix file(fs);
    std::cout << "file:" << std::endl<< file << std::endl;
    fs.close();

    CMatrix mariusz(9,9,7);
    std::cout << "mariusz:" << std::endl<< mariusz << std::endl;
    double d_mariusz = mariusz(2, 3);
    std::cout << "mariusz(2, 3): " << d_mariusz << std::endl;
    mariusz(2, 3) = 4;
    std::cout << "mariusz(2, 3) = 4" << std::endl;
    std::cout << "mariusz:" << std::endl<< mariusz << std::endl;

    return 0;
}


