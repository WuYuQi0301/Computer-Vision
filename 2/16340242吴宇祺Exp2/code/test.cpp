#include "copy.hpp"

int main(int argc, char const *argv[])
{
    string path = "./test_Data/lena.bmp";

    string root1 = "./result/sigma/2/";
    canny Canny1(path, root1, 3, 2, 40, 60); 
    
    string root2 = "./result/sigma/1/";
    canny Canny2(path,root2, 3, 1, 40, 60);
    
    string root3 = "./result/sigma/3/";
    canny Canny3(path,root3, 3, 3, 40, 60);
    
    string root4 = "./result/sigma/4/";
    canny Canny4(path,root4, 3, 4, 40, 60);
    

    string root5 = "./result/lowThreshold/1/";
    canny Canny5(path, root5, 3, 2, 20, 60);
    
    string root6 = "./result/lowThreshold/2/";
    canny Canny6(path, root6, 3, 2, 30, 60);  //good
    
    string root7 = "./result/lowThreshold/3/";
    canny Canny7(path, root7, 3, 2, 40, 60);

    string root8 = "./result/lowThreshold/4/";
    canny Canny8(path, root8, 3, 2, 50, 60);



    string root9 = "./result/highThreshold/1/";
    canny Canny9(path, root9, 3, 2, 40, 50);

    string root10 = "./result/highThreshold/2/";
    canny Canny10(path, root10, 3, 2, 40, 60);

    string root11 = "./result/highThreshold/3/";
    canny Canny11(path, root11, 3, 2, 40, 70);

    string root12 = "./result/highThreshold/4/";
    canny Canny12(path, root12, 3, 2, 40, 80);


    string root14 = "./result/shortSupp/";
    canny Canny14(path, root14, 3, 2, 40, 60, 1);
    
    return 0;
}