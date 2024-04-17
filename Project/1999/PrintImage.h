#pragma once
#include <map>
#include "WholeImage.h"
#include "BuildingImage.h"
#include "SetPosition.h"

typedef int(*WholeImagePtr)[WHOLE_IMAGE_X];
typedef int(*SmallImagePtr)[SMALL_X];
typedef int(*MediumImagePtr)[MEDIUM_X];
typedef int(*LargeImagePtr)[LARGE_X];

typedef int(*LogoImagePtr)[START_LOGO_X];
typedef int(*MenuImagePtr)[MENU_IMAGE_X];

class PrintImage : WholeImage, BuildingImage
{
private:
	SetPosition* to = new SetPosition;

public:

	PrintImage();
	~PrintImage();

	// Get Image Array
	WholeImagePtr GetStartImage() { return this->StartImage; }
	MenuImagePtr GetMenuImage() { return this->MenuImage; }
	LogoImagePtr GetLogoImage() { return this->LogoImage; }

	SmallImagePtr GetHosPitalImage() { return this->Hospital; }
	SmallImagePtr GetArmySmallImage() { return this->ArmySmall;  }
	MediumImagePtr GetAPTImage() { return this->APT; }
	MediumImagePtr GetArmyMediumImage() { return this->ArmyMedium;  }
	MediumImagePtr GetChurchImage() { return this->Church; }
	MediumImagePtr GetCathedralImage() { return this->Cathedral; }
	LargeImagePtr GetParkImage() { return this->Park; }
	LargeImagePtr GetArmyLargeImage() { return this->ArmyLarge; }

	// Convert Image
	void ConvertLogoImage(int y, int Image[][START_LOGO_X]);

	void ConvertWholeImage(int y, int Image[][WHOLE_IMAGE_X]);
	void ConvertSmallImage(int y, int Image[][SMALL_X]);
	void ConvertMediumImage(int y, int Image[][MEDIUM_X]);
	void ConvertLargeImage(int y, int Image[][LARGE_X]);
	void ConvertMenuImage(int y, int Image[][MENU_IMAGE_X]);

	// Rolling Image
	void RollingWholeImage(int y, int Image[][WHOLE_IMAGE_X], int index, int HalfX);

	// PrintText
	void PrintMenuText();
	void PrintPressText();
};

