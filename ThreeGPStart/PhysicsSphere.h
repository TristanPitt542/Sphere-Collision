#include <vector>
#include "Helper.h"

class PhysicsSphere
{
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 colour;
    float mass;
    float radius;
    float restitution;
    glm::quat rotation;

    // Constructor
    PhysicsSphere(glm::vec3 initialPosition, glm::vec3 initialVelocity, float initialRad = 5.0f, float initialMass = 10.0f, float initialRestitution = 0.8, glm::vec3 col = glm::vec3(0.0f, 0.0f, 0.0f))
        : position(initialPosition), velocity(initialVelocity), radius(initialRad), mass(initialMass), restitution(initialRestitution), colour(col), rotation(glm::angleAxis(glm::radians(90.f), glm::vec3(0.f, .0f, 1.f))) {}

    void update(float deltaTime, float grav)
    {
        // Apply gravity
        velocity.y += grav * deltaTime;

        // Update position
        position += velocity * deltaTime * mass;

        if (velocity.y < 1.0f)
        {
            // Apply friction
            float frictionCoefficient = 0.001f;
            velocity.x *= (1.0f - frictionCoefficient);
            velocity.z *= (1.0f - frictionCoefficient);
        }
    }


};