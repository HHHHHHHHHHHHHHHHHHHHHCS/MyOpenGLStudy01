#pragma once
#include <string>
#include <vector>

class ImageHelper
{
public:
	static unsigned int LoadTexture_Filp(std::string path, std::string directory = "Images/");
	static unsigned int LoadTexture(std::string path, std::string directory = "Images/");
	static unsigned int LoadCubemap(std::vector<std::string> faces, std::string directory = "Images/");
};
