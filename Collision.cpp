#include "Collision.h"
bool intersect(Rect a, Rect b) {
	return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z);
}
bool sphereIntersect(Circle a, Circle b) {
	float distance = (float)sqrt((a.position.x - b.position.x) * (a.position.x - b.position.x) +
		(a.position.y - b.position.y) * (a.position.y - b.position.y) +
		(a.position.z - b.position.z) * (a.position.z - b.position.z));
	return distance < (a.radius + b.radius);
}