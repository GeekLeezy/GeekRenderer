#pragma once

#pragma once
#include "vertex.h"

class VerToFrag
{
public:
	vec4 worldPos;
	vec4 fragPos;

	vec4 color;
	vec3 normal;
	vec2 texture;

	//记录透视除法前的fragPos.w
	float Z;

	VerToFrag() = default;

	VerToFrag(
		const vec4& _wolrdPos,
		const vec4& _fragPos,
		const vec4& _color,
		const vec3& _normal,
		const vec2& _texture) : worldPos(_wolrdPos), fragPos(_fragPos), color(_color), normal(_normal), texture(_texture), Z(1.0f) {}

	VerToFrag(const VerToFrag& v2f) :
		worldPos(v2f.worldPos), fragPos(v2f.fragPos), color(v2f.color), normal(v2f.normal), texture(v2f.texture), Z(v2f.Z) {}

	~VerToFrag() = default;
};

static VerToFrag lerp(const VerToFrag& f1, const VerToFrag& f2, const float& scale)
{
	VerToFrag f;

	f.worldPos = (1 - scale) * f1.worldPos + scale * f2.worldPos;
	f.fragPos = (1 - scale) * f1.fragPos + scale * f2.fragPos;

	f.normal = (1 - scale) * f1.normal + scale * f2.normal;
	f.texture = (1 - scale) * f1.texture + scale * f2.texture;

	f.color = (1 - scale) * f1.color + scale * f2.color;
	f.Z = (1 - scale) * f1.Z + scale * f2.Z;

	return f;
}

static VerToFrag barycenterLerp(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3, const vec2& pos)
{
	VerToFrag f;

	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;
	vec4 p3 = f3.fragPos;

	float a = (-(pos.x - p2.x) * (p3.y - p2.y) + (pos.y - p2.y) * (p3.x - p2.x)) / (-(p1.x - p2.x) * (p3.y - p2.y) + (p1.y - p2.y) * (p3.x - p2.x));
	float b = (-(pos.x - p3.x) * (p1.y - p3.y) + (pos.y - p3.y) * (p1.x - p3.x)) / (-(p2.x - p3.x) * (p1.y - p3.y) + (p2.y - p3.y) * (p1.x - p3.x));
	float c = 1 - a - b;

	//透视插值矫正
	float z1 = f1.Z;
	float z2 = f2.Z;
	float z3 = f3.Z;

	float z = a * z1 + b * z2 + c * z3;

	f.fragPos = (a * f1.fragPos * z1 + b * f2.fragPos * z2 + c * f3.fragPos * z3) / z;
	//f.fragPos.x = pos.x;
	//f.fragPos.y = pos.y;


	f.worldPos = (a * f1.worldPos * z1 + b * f2.worldPos * z2 + c * f3.worldPos * z3) / z;
	f.normal = (a * f1.normal * z1 + b * f2.normal * z2 + c * f3.normal * z3) / z;
	f.texture = (a * f1.texture * z1 + b * f2.texture * z2 + c * f3.texture * z3) / z;
	f.color = (a * f1.color * z1 + b * f2.color * z2 + c * f3.color * z3) / z;

	return f;
}

