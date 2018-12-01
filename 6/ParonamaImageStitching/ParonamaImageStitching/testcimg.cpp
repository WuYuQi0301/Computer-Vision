#include "CImg.h"

using namespace cimg_library;

int main() {
	CImg<float> t("../testData/1.JPG");
	t.display();
	return 0;
}