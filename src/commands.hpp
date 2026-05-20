#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "game_object.hpp"

//================================================================================== 
// Base Command Interface
class Command {
public:
    virtual ~Command() {}
    virtual void execute(GameObject& object) = 0;
};

//================================================================================== 
// Translational Movement Commands

class MoveForwardCommand : public Command {
private:
    float speed;
public:
    MoveForwardCommand(float moveSpeed) : speed(moveSpeed) {}
    
    void execute(GameObject& object) override {
        object.moveLocalForward(speed);
    }
};

class MoveBackCommand : public Command {
private:
    float speed;
public:
    MoveBackCommand(float moveSpeed) : speed(moveSpeed) {}
    
    void execute(GameObject& object) override {
        object.moveLocalForward(-speed);
    }
};

class MoveRightCommand : public Command {
private:
    float speed;
public:
    MoveRightCommand(float moveSpeed) : speed(moveSpeed) {}
    
    void execute(GameObject& object) override {
        object.moveLocalRight(speed);
    }
};

class MoveLeftCommand : public Command {
private:
    float speed;
public:
    MoveLeftCommand(float moveSpeed) : speed(moveSpeed) {}
    
    void execute(GameObject& object) override {
        object.moveLocalRight(-speed);
    }
};

//================================================================================== 
// Rotational Commands (Pitch, Yaw, Roll)

class PitchUpCommand : public Command {
private:
    float speed;
public:
    PitchUpCommand(float pitchSpeed) : speed(pitchSpeed) {}
    
    void execute(GameObject& object) override {
        object.rotatePitch(speed);
    }
};

class PitchDownCommand : public Command {
private:
    float speed;
public:
    PitchDownCommand(float pitchSpeed) : speed(pitchSpeed) {}
    
    void execute(GameObject& object) override {
        object.rotatePitch(-speed);
    }
};

class YawRightCommand : public Command {
private:
    float speed;
public:
    YawRightCommand(float yawSpeed) : speed(yawSpeed) {}
    
    void execute(GameObject& object) override {
        object.rotateYaw(speed);
    }
};

class YawLeftCommand : public Command {
private:
    float speed;
public:
    YawLeftCommand(float yawSpeed) : speed(yawSpeed) {}
    
    void execute(GameObject& object) override {
        object.rotateYaw(-speed);
    }
};

class RollCWCommand : public Command {
private:
    float speed;
public:
    RollCWCommand(float rollSpeed) : speed(rollSpeed) {}
    
    void execute(GameObject& object) override {
        object.rotateRoll(speed);
    }
};

class RollCCWCommand : public Command {
private:
    float speed;
public:
    RollCCWCommand(float rollSpeed) : speed(rollSpeed) {}
    
    void execute(GameObject& object) override {
        object.rotateRoll(-speed);
    }
};

#endif // COMMANDS_HPP
