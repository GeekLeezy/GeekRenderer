#include "cull.h"

//视锥剔除
bool pointInside(const vec3& v, const vec4& p) {
	return p.x * v.x + p.y * v.y + p.z * v.z + p.w >= 0;
}

bool Cull::frustumCull(const vec4& v1, const vec4& v2, const vec4& v3)
{
	glm::vec3 minPoint, maxPoint;

	//计算世界下的包围盒
	minPoint.x = std::min(v1.x, std::min(v2.x, v3.x));
	minPoint.y = std::min(v1.y, std::min(v2.y, v3.y));
	minPoint.z = std::min(v1.z, std::min(v2.z, v3.z));
	maxPoint.x = std::max(v1.x, std::max(v2.x, v3.x));
	maxPoint.y = std::max(v1.y, std::max(v2.y, v3.y));
	maxPoint.z = std::max(v1.z, std::max(v2.z, v3.z));

	for (auto i = 0; i < 5; i++)
	{
		if (!pointInside(minPoint, frustumPlanes[i]) && !pointInside(maxPoint, frustumPlanes[i])) {
			return false;
		}
	}

	// Far 剔除时只保留完全在内的
	if (!pointInside(minPoint, frustumPlanes[5]) || !pointInside(maxPoint, frustumPlanes[5])) {
		return false;
	}

	return true;
}

//裁剪空间剔除
bool Cull::clipSpaceCull(const vec4& v1, const vec4& v2, const vec4& v3)
{
	if (v1.w <= cull_near && v2.w <= cull_near && v3.w <= cull_near)
		return false;
	if (v1.w >= cull_far && v2.w <= cull_far && v3.w <= cull_far)
		return false;
	if (v1.x <= v1.w || v1.y <= v1.w || v1.z <= v1.w)
		return true;
	if (v2.x <= v2.w || v2.y <= v2.w || v2.z <= v2.w)
		return true;
	if (v3.x <= v3.w || v3.y <= v3.w || v3.z <= v3.w)
		return true;
	return false;
}

//面剔除
bool Cull::faceCull(const vec4& v1, const vec4& v2, const vec4& v3)
{
	vec3 s1 = vec3(v2) - vec3(v1);
	vec3 s2 = vec3(v3) - vec3(v1);

	vec3 normal = normalize(cross(s1, s2));
	vec3 view = vec3(0, 0, 1);

	return dot(normal, view) < 0;
}

std::vector<vec4> Cull::setFrustumPlanes(const mat4& vp)
{
	std::vector<vec4> planes(6);

	//left, right, bottom, top, near, far
	for (int i = 0; i < 6; i++)
	{
		int j = i / 2;
		if (i % 2 == 0) 
		{
			planes[i].x = vp[0][3] + vp[0][j];
			planes[i].y = vp[1][3] + vp[1][j];
			planes[i].z = vp[2][3] + vp[2][j];
			planes[i].w = vp[3][3] + vp[3][j];
		}
		else
		{
			planes[i].x = vp[0][3] - vp[0][j];
			planes[i].y = vp[1][3] - vp[1][j];
			planes[i].z = vp[2][3] - vp[2][j];
			planes[i].w = vp[3][3] - vp[3][j];
		}
	}

	return planes;
}

