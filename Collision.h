#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_
#include "main.h"
struct Rect {
	D3DXVECTOR3	position;
	D3DXVECTOR3 size;
};
struct Circle {
	D3DXVECTOR3	position;
	float radius;
};
bool intersect(Rect a, Rect b);
bool sphereIntersect(Circle a, Circle b);
#endif