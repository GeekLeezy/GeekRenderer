#include "clip.h"

//判断点是否在直线内部
bool pointInside(const vec4& L, const vec4& p) {
	//常数D需要将w分量相乘
	return L.x * p.x + L.y * p.y + L.z * p.z + L.w * p.w <= 0;
}

//判断三角形三个顶点是否都在NDC内
bool vertexsInside(const std::vector<VerToFrag>& v)
{
	for (auto i = 0; i < v.size(); i++)
	{
		float x = v[i].fragPos.x;
		float y = v[i].fragPos.y;
		float z = v[i].fragPos.z;
		float w = v[i].fragPos.w;

		if (x > w || y > w || z > w || x < -w || y < -w || z < -w)
			return false;

	}

	return true;
}

//顶点插值求交点
VerToFrag getIntersect(const VerToFrag& f1, const VerToFrag& f2, const vec4& L)
{
	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;

	float d1 = L.x * p1.x + L.y * p1.y + L.z * p1.z + L.w * p1.w;
	float d2 = L.x * p2.x + L.y * p2.y + L.z * p2.z + L.w * p2.w;

	//通过两点到面的距离插值
	float w = d1 / (d1 - d2);

	return lerp(f1, f2, w);
}

std::vector<VerToFrag> Clip::clipSpaceClip(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3)
{
	std::vector<VerToFrag> output = { f1, f2, f3 };

	//若全部顶点都在NDC中 直接返回这些顶点
	if (vertexsInside(output))
		return output;

	for (int i = 0; i < clipRect.size(); i++)
	{
		vec4 L = clipRect[i];
		std::vector<VerToFrag> input(output);
		output.clear();

		for (int j = 0; j < input.size(); j++)
		{
			VerToFrag vNow = input[j];
			VerToFrag vPre = input[(j + input.size() - 1) % input.size()];

			//当前点在裁剪线内
			if (!pointInside(L, vNow.fragPos))
			{
				//上一点不在裁剪线内
				if (pointInside(L, vPre.fragPos))
				{
					VerToFrag f = getIntersect(vNow, vPre, L);
					output.push_back(f);
				}
				output.push_back(vNow);
			}
			//当前点不在裁剪先内 && 上一点在裁剪线内
			//加入交点
			else if (!pointInside(L, vPre.fragPos))
			{
				VerToFrag f = getIntersect(vNow, vPre, L);
				output.push_back(f);
			}
		}
	}

	return output;
}