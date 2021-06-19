#pragma once
#include "box2d.h"
typedef std::vector<b2Vec2> Vector2dVector;

namespace triangulate {
bool Process(const Vector2dVector &contour,
					Vector2dVector &result);


};

