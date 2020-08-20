#pragma once
#include <string>
#include <vector>

class ImageHelper
{
public:
	static const std::string defaultPath;
	static unsigned int LoadTexture_Filp(std::string path, std::string directory = defaultPath, bool isSRGB = false);
	static unsigned int LoadTexture(std::string path, std::string directory = defaultPath, bool isSRGB = false);
	static unsigned int LoadCubemap(std::vector<std::string> faces, std::string directory = defaultPath);
	static unsigned int LoadHDR_Filp(std::string path, std::string directory = defaultPath);
	static unsigned int LoadHDR(std::string path, std::string directory = defaultPath);
	static unsigned int BindHDRCubemap();
};
