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
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 totalForce = glm::vec3(0.0f, 0.0f, 0.0f);

    // Constructor
    PhysicsSphere(glm::vec3 initialPosition, glm::vec3 initialVelocity, float initialRad = 5.0f, float initialMass = 50.0f, float initialRestitution = 0.5f, glm::vec3 col = glm::vec3(0.0f, 0.0f, 0.0f))
        : position(initialPosition), velocity(initialVelocity), radius(initialRad), mass(initialMass), restitution(initialRestitution), colour(col), rotation(glm::angleAxis(glm::radians(90.f), glm::vec3(0.f, .0f, 1.f))) {}


    void ApplyForce(glm::vec3& force)
    {
        totalForce += force;
    }

    void Update(float deltaTime)
    {
        acceleration = totalForce / mass;
        velocity += acceleration * deltaTime;
        position += velocity * deltaTime;
        totalForce = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};