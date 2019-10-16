#pragma once
#include <string>

class ImageHelper
{
public:
	static unsigned int LoadTexture_Filp(std::string path, std::string directory = "Images/");
	static unsigned int LoadTexture(std::string path,std::string directory = "Images/");
};
