#pragma once
#include <vector>
#include <glm/glm.hpp>

using namespace glm;

class FrameBuffer
{
public:
	int width;
	int height;
	unsigned char* colorBuffer;
	std::vector<float> depthBuffer;

	//MSAA
	//bool MSAA;
	//std::vector<float> superColorBuffer;
	//std::vector<float> superDepthBuffer;

	FrameBuffer(const int& w, const int& h, unsigned char* buffer)
	{
		width = w;
		height = h;
		colorBuffer = buffer;
		depthBuffer.resize(w * h, 1.0);
	}


	void writeColor(const int& x, const int& y, const vec4& color);
	void writeDepth(const int& x, const int& y, const float& depth);
	float getDepth(const int& x, const int& y) const;

	void clearColorBuffer();
	void clearDepthBuffer();

	~FrameBuffer() = default;
};

