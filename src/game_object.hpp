#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "raylib.h"
#include "raymath.h"

class Scene;

class GameObject {
public:
    virtual ~GameObject() {};

    // Master matrix tracking position and orientation
    Matrix transform = MatrixIdentity();

    // Getters
    // TODO: Add Quaternion Getter
    Vector3 get_position() const;
    Vector3 get_forward() const;
    Vector3 get_up() const;
    Vector3 get_right() const;

    // Setters
    // TODO: Add Quaternion Setter
    void set_position_x(float x);
    void set_position_y(float y);
    void set_position_z(float z);
    void set_position(float x, float y, float z);
    void set_position(Vector3 position);

    // Global Movement
    virtual void move_global_x(float deltaX);
    virtual void move_global_y(float deltaY);
    virtual void move_global_z(float deltaZ);
    virtual void move_global(float deltaX, float deltaY, float deltaZ);
    virtual void move_global(Vector3 delta);
    virtual void move_global_vel(Vector3 velocity);

    // Local Movement
    virtual void move_local_right(float distance);
    virtual void move_local_up(float distance);
    virtual void move_local_forward(float distance);

    // Rotations
    // TODO: Add Quaternion Rotation
    virtual void rotate_pitch(float angleRad); // Rotation around Right axis
    virtual void rotate_yaw(float angleRad); // Rotation around Up axis
    virtual void rotate_roll(float angleRad); // Rotation around Forward axis
    virtual void rotate(float deltaPitch, float deltaYaw, float deltaRoll);

    // Frame by frame behavior
    virtual void on_update(Scene& scene);
    virtual void on_before_update(Scene& scene);
    virtual void on_after_update(Scene& scene);

protected:
    virtual void update(Scene& scene) {};
    virtual void before_update(Scene& scene) {};
    virtual void after_update(Scene& scene) {};
};

#endif // GAMEOBJECT_HPP
