#pragma once

#include "sputnik/core/engine.hpp"

namespace Sputnik {

    class Application {
    protected:
        std::unique_ptr<Engine> engine_;

    public:
        Application(const char* name);
        virtual ~Application();

        virtual void startup() {}
        void run();
    };

    std::unique_ptr<Application> create_application();

}
