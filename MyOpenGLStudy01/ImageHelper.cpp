#include "ImageHelper.h"
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>

const std::string ImageHelper::defaultPath = "Images/";


unsigned int ImageHelper::LoadTexture_Filp(std::string path, std::string directory, bool isSRGB)
{
	//这个是翻转读取的图片
	//因为OPENGL的UV是反着的 要么1-UV.Y  要么翻转图片
	stbi_set_flip_vertically_on_load(true);
	auto out = LoadTexture(path, directory);
	stbi_set_flip_vertically_on_load(false);
	return out;
}

//isSRGB 的作用把读取的图片的颜色 pow(2.2)
//即在Gamma空间制作的纹理，经过Gamma校正->pow(1/2.2)后存储的纹理
unsigned int ImageHelper::LoadTexture(std::string path, std::string directory, bool isSRGB)
{
	path = directory + path;

	unsigned int textureID;
	glGenTextures(1, &textureID);


	// 加载并生成纹理
	int width, height, nrChannels;
	//用stb载入图片 然后 自动填充 width height 和 通道数量
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;

		if (isSRGB)
		{
			switch (nrChannels)
			{
			case 1:
				internalFormat = dataFormat = GL_RED;
				break;
			case 2:
				internalFormat = dataFormat = GL_RG;
				break;
			case 3:
				internalFormat = GL_SRGB;
				dataFormat = GL_RGB;
				break;
			case 4:
				internalFormat = GL_SRGB_ALPHA;
				dataFormat = GL_RGBA;
				break;
			}
		}
		else
		{
			switch (nrChannels)
			{
			case 1:
				internalFormat = dataFormat = GL_RED;
				break;
			case 2:
				internalFormat = dataFormat = GL_RG;
				break;
			case 3:
				internalFormat = dataFormat = GL_RGB;
				break;
			case 4:
				internalFormat = dataFormat = GL_RGBA;
				break;
			}
		}


		glBindTexture(GL_TEXTURE_2D, textureID);

		//第一个是 纹理类型 1D  2D  3D
		//第二个是 当前图片的mipmap层级,可以自动生成,也可以手动修改层级 设置图片
		//第三个是 颜色类型 gamma(srgb) linear
		//第四个是 纹理的宽度
		//第五个是 纹理的高度
		//第六个是 总是为0 历史遗留问题
		//第七个是 图片颜色格式 R RG RGB RGBA
		//第八个是 图片单个颜色的数据类型
		//第九个是 图片的数据
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		//自动生成mipmap
		glGenerateMipmap(GL_TEXTURE_2D);


		// 为当前绑定的纹理对象设置环绕、过滤方式
		// S->X T->Y W->Z 如果是3D 的则有W
		//GL_TEXTURE_WRAP 是重复方式  GL_TEXTURE_???_FILTER 是放大缩小的滤波方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	//释放图片
	stbi_image_free(data);

	return textureID;
}


unsigned int ImageHelper::LoadCubemap(std::vector<std::string> faces, std::string directory)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	//	cubemap 六个方向
	//  GL_TEXTURE_CUBE_MAP_POSITIVE_X	右
	// 	GL_TEXTURE_CUBE_MAP_NEGATIVE_X	左
	// 	GL_TEXTURE_CUBE_MAP_POSITIVE_Y	上
	// 	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	下
	// 	GL_TEXTURE_CUBE_MAP_POSITIVE_Z	后
	// 	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	前

	int width, height, nrChannels;

	if (faces.size() != 6)
	{
		std::cout << "cubemap count must be six\n";
		return 0;
	}

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load((directory + faces[i]).c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			//cubemap 六个方向 实际是一个递增的数字  所以可以用for循环
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			             data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path:" << faces[i] << std::endl;
		}

		//释放资源
		stbi_image_free(data);
	}


	//cubemap 的缩放   和  重复方式 因为是3D 的 所以是 STR
	//GL_CLAMP_TO_EDGE 卡到边界
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

unsigned int ImageHelper::LoadHDR_Filp(std::string path,std::string directory)
{
	//这个是翻转读取的图片
//因为OPENGL的UV是反着的 要么1-UV.Y  要么翻转图片
	stbi_set_flip_vertically_on_load(true);
	auto out = LoadTexture(path, directory);
	stbi_set_flip_vertically_on_load(false);
	return out;
}

unsigned int ImageHelper::LoadHDR(std::string path, std::string directory)
{
	path = directory + path;

	int width, height, nrComponents;
	//用 float 的读取 因为是HDR
	float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;

	if(data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image."<<std::endl;
	}

}