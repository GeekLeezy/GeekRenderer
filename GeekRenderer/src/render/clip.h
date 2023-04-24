#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../structure/v2f.h"

using namespace glm;

class Clip
{
public:
	std::vector<vec4> clipRect;

	Clip() 
	{
		//先进行near平面的裁剪 保证w > 0
		clipRect = {
			//near far
			vec4(0,0,1,1),vec4(0,0,-1,1),
			//left right
			vec4(1,0,0,1),vec4(-1,0,0,1),
			//top bottom
			vec4(0,-1,0,1),vec4(0,1,0,1)
		};
	}

	std::vector<VerToFrag> clipSpaceClip(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3);
};

