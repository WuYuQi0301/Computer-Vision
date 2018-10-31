#include "Paint.cpp"
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
	Paint myImg;
	myImg.Display();
	// myImg.ChangeColor();
	// myImg.BlueCircle();
	// myImg.YellowCircle();
	// myImg.BlueLine();
	// myImg.Save("2.bmp");

	myImg.ChangeColor();
	myImg.MyBlueCircle();
	myImg.MyellowCircle();
	myImg.MyBlueLine();
	myImg.Save("2_test.bmp");

	return 0;
}