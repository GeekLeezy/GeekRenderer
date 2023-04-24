#include "clip.h"

//�жϵ��Ƿ���ֱ���ڲ�
bool pointInside(const vec4& L, const vec4& p) {
	//����D��Ҫ��w�������
	return L.x * p.x + L.y * p.y + L.z * p.z + L.w * p.w <= 0;
}

//�ж����������������Ƿ���NDC��
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

//�����ֵ�󽻵�
VerToFrag getIntersect(const VerToFrag& f1, const VerToFrag& f2, const vec4& L)
{
	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;

	float d1 = L.x * p1.x + L.y * p1.y + L.z * p1.z + L.w * p1.w;
	float d2 = L.x * p2.x + L.y * p2.y + L.z * p2.z + L.w * p2.w;

	//ͨ�����㵽��ľ����ֵ
	float w = d1 / (d1 - d2);

	return lerp(f1, f2, w);
}

std::vector<VerToFrag> Clip::clipSpaceClip(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3)
{
	std::vector<VerToFrag> output = { f1, f2, f3 };

	//��ȫ�����㶼��NDC�� ֱ�ӷ�����Щ����
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

			//��ǰ���ڲü�����
			if (!pointInside(L, vNow.fragPos))
			{
				//��һ�㲻�ڲü�����
				if (pointInside(L, vPre.fragPos))
				{
					VerToFrag f = getIntersect(vNow, vPre, L);
					output.push_back(f);
				}
				output.push_back(vNow);
			}
			//��ǰ�㲻�ڲü����� && ��һ���ڲü�����
			//���뽻��
			else if (!pointInside(L, vPre.fragPos))
			{
				VerToFrag f = getIntersect(vNow, vPre, L);
				output.push_back(f);
			}
		}
	}

	return output;
}