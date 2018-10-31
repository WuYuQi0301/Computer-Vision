#include <iostream>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

int main(int argc, char* argv[]) {

    CImg<unsigned char> testImage1("lena.bmp");
    CImg<unsigned char> testImage2(testImage1.width(), testImage1.height(), 1, 3);
    CImg<unsigned char> testImage3(testImage1.width(), testImage1.height(), 1, 3, 0);
    CImg<unsigned char> testImage4(testImage1.width(), testImage1.height(), 1, 3,0);
    CImg<unsigned char> testImage5(testImage1.width(), testImage1.height(), 1, 3,0);
    CImg<unsigned char> testImage6(testImage1.width(), testImage1.height(), 1, 3,0);
    CImg<unsigned char> testImage7(testImage1.width(), testImage1.height(), 1, 3,0);
    for (int i = 0; i < testImage1.width(); i ++)
        for (int j = 0; j < testImage1.height(); j ++)
            for (int k = 0; k < 3; k ++)
                testImage2.atXYZC(i, j, 1, k) = testImage1.atXYZC(i, j, 1, k);
    testImage1.display("1");
    testImage2.display("COPY");
    testImage3.display("3");
    testImage4.display("4");
    testImage5.display("5");
    testImage6.display("6");
    testImage7.display("7");
    return 0;
}