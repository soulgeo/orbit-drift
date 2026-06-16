#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "sputnik/ecs/component.hpp"
#include <raylib.h>

namespace Sputnik {

    class Engine;

    class TransformComponent : public Component {
        Matrix transform_;
        Matrix previous_transform_;
        Matrix visual_transform_;

        Engine* engine_;

    public:
        TransformComponent(GameObject* owner, Engine* engine);
        ~TransformComponent();

        // Getters
        Matrix get_transform() const;
        Matrix get_visual_transform() const;

        Vector3 get_position() const;
        Vector3 get_previous_position() const;
        Vector3 get_visual_position() const;
        Vector3 get_forward() const;
        Vector3 get_up() const;
        Vector3 get_right() const;

        Quaternion get_rotation() const;

        // Setters
        void set_position_x(float x);
        void set_position_y(float y);
        void set_position_z(float z);
        void set_position(float x, float y, float z);
        void set_position(Vector3 position);

        void set_rotation(Quaternion rotation);

        // Global Movement
        virtual void move_global_x(float deltaX);
        virtual void move_global_y(float deltaY);
        virtual void move_global_z(float deltaZ);
        virtual void move_global(float deltaX, float deltaY, float deltaZ);
        virtual void move_global(Vector3 delta);
        virtual void move_global_vel(Vector3 velocity, float dt);

        // Local Movement
        virtual void move_local_right(float distance);
        virtual void move_local_up(float distance);
        virtual void move_local_forward(float distance);

        // Rotations
        virtual void rotate_pitch(float angleRad); // Rotation around Right axis
        virtual void rotate_yaw(float angleRad); // Rotation around Up axis
        virtual void rotate_roll(float angleRad); // Rotation around Forward axis
        virtual void rotate(float deltaPitch, float deltaYaw, float deltaRoll);

        void fixed_update() override;
        void update() override;
    };

}

#endif // TRANSFORM_COMPONENT_HPP
