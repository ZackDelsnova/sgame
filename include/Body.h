#pragma once
#include <raylib.h>
#include <raymath.h>

class Body {
public:
	Vector3 position;
	Vector3 size;
	Color color;
	BoundingBox box;

	Body(Vector3 pos, Vector3 s, Color col);
	
	virtual void Update(float dt);
	virtual void Draw() const;

protected:
	void UpdateBoundingBox();
};
