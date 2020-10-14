#include "_46_Text.h"
#include  <iostream>

#include  <ft2build.h>
#include FT_FREETYPE_H

int _46_Text::DoMain()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE:Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, "Fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE:Failed to laod font" << std::endl;
	}

	return 0;
}
