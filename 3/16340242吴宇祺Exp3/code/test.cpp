#include "copy.hpp"

int main(int argc, char const *argv[])
{
    // canny(string filename, string root,int num , int filter_size = 3,int sigmaIn = 2, int low = 40, int high = 60, int note = 0);

	string root = "../papers/";
    // canny coin0("../Dataset2/1.bmp", root, 1, 3, 2, 40, 60);
    // canny coin1("../Dataset2/11.bmp", root, 11, 3, 2, 40, 60);
    // canny coin2("../Dataset2/12.bmp", root, 12, 3, 2, 40, 60);
    // canny coin3("../Dataset2/2.bmp", root, 2, 3, 2, 40, 60);
    // canny coin4("../Dataset2/5.bmp", root, 5, 3, 2, 40, 60);
    // canny coin5("../Dataset2/6.bmp", root, 6, 3, 2, 40, 60);
    canny paper2("../Dataset1/paper2.bmp", root, 2);
    canny paper3("../Dataset1/paper3.bmp", root, 3);
    canny paper4("../Dataset1/paper4.bmp", root, 4);
    canny paper5("../Dataset1/paper5.bmp", root, 5);
    canny paper6("../Dataset1/paper6.bmp", root, 6);

	return 0;
}