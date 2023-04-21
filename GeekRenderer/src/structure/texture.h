#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <string>

using namespace glm;

class Texture
{
public:
	int width;
	int height;
	int channels;
	unsigned char* data;

	Texture() = default;

	Texture(const std::string& path);

	void loadTexture(const std::string& path);

	vec4 sampler2D(const vec2& texture);

	~Texture()
	{
		if (data)
			free(data);
	}

private:
	vec4 getColor(const int& x, const int& y);
};

