#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../Component.h"
#include "../../MathUtils/Vector2D.h"

class Transform : public Component
{
public:
    Transform() : Component("Transform"), position(Vector2D(0,0)), scale(1) {};
    Transform(Vector2D position, float scale) : Component("Transform"), position(position), scale(scale) {};
    virtual ~Transform() {};
    void update(float deltaTime) {};
    void render() {};

    Vector2D& GetPosition() { return position; }
    float& GetScale() { return scale; }
private:
    Vector2D position;
    float scale;
};

#endif