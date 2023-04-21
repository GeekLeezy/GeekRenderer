#pragma once

#include "texture.h"

class Material
{
public:
	vec4 diffuse;
	vec4 specular;
	int glossy;

	Texture* mainTexture;

	Material() : diffuse(vec4(1.0f)), specular(vec4(1.0f)), glossy(32), mainTexture(nullptr) {}
	Material(
		const vec4& D,
		const vec4& S,
		const int& G
	) : diffuse(D), specular(S), glossy(G), mainTexture(nullptr) {}

	void setTexture(Texture* t) 
	{
		mainTexture = t;
	}

	~Material() = default;

};


