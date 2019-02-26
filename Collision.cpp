#include "Collision.h"
bool intersect(Rect a, Rect b) {
	if (a.position.x < b.position.x + b.size.x &&
		a.position.x + a.size.x > b.position.x &&
		a.position.y < b.position.y + b.size.y &&
		a.size.y + a.position.y > b.position.y) {
		// collision detected!
		return true;
	}
	return false;
}
bool sphereIntersect(Circle a, Circle b) {
	float distance = (float)sqrt((a.position.x - b.position.x) * (a.position.x - b.position.x) +
		(a.position.y - b.position.y) * (a.position.y - b.position.y));
	return distance < (a.radius + b.radius);
}
bool pointRectIntersect(D3DXVECTOR2 point, Rect rect) {
	float pointX1 = rect.position.x - rect.size.x / 2.f;
	float pointX2 = rect.position.x + rect.size.x / 2.f;

	float pointY1 = rect.position.y - rect.size.y / 2.f;
	float pointY2 = rect.position.y + rect.size.y / 2.f;


	if (((pointX1 <= point.x) && (point.x <= pointX2)) && ((pointY1 <= point.y) && (point.y <= pointY2))) {
		return true;
	}
	return false;
}