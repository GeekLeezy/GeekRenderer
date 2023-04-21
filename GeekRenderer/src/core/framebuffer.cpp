#include "framebuffer.h"

float saturate(const float& val)
{
	if (val > 1.0f)
	{
		return 1.0f;
	}
	if (val < 0.0f)
	{
		return 0.0f;
	}
	return val;
}

//��������Ϊ���ص���ɫ
void FrameBuffer::writeColor(const int& x, const int& y, const vec4& color)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return;
	}
	unsigned char* p = colorBuffer;
	int pos = y * width + x;

	/*colorBuffer[pos * 4] = color.b;
	colorBuffer[pos * 4 + 1] = color.g;
	colorBuffer[pos * 4 + 2] = color.r;
	colorBuffer[pos * 4 + 3] = color.a;*/
	*(p + pos * 4) = saturate(color.b) * 255;
	*(p + pos * 4 + 1) = saturate(color.g) * 255;
	*(p + pos * 4 + 2) = saturate(color.r) * 255;
	*(p + pos * 4 + 3) = saturate(color.a) * 255;

}

//����Ȼ�����д����º�����
void FrameBuffer::writeDepth(const int& x, const int& y, const float& depth)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return;
	}

	float* p = depthBuffer.data();
	int pos = y * width + x;
	*(p + pos) = depth;
}

//��ȡ���ͼ�е����
float FrameBuffer::getDepth(const int& x, const int& y) const
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return 1.0f;
	}

	int pos = y * width + x;
	return depthBuffer[pos];
}

//������Ļ��ɫ����
void FrameBuffer::clearColorBuffer()
{
	unsigned char* p = colorBuffer;
	if (p != NULL)
	{
		for (int i = 0; i < width * height * 4; i += 4) 
		{
			*(p + i) = 0;
			*(p + i + 1) = 0;
			*(p + i + 2) = 0;
			*(p + i + 3) = 0;
		}
	}	
}

//������Ļ��Ȼ���
void FrameBuffer::clearDepthBuffer()
{
	float* p = depthBuffer.data();
	if (p != NULL)
	{
		for (int i = 0; i < width * height; ++i)
			*(p + i) = 1.0f;
	}
}


