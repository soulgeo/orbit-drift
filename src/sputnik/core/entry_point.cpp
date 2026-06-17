#include "sputnik/core/application.hpp"
#include <memory>

int main() {
    auto app = Sputnik::create_application();
    app->run();
    return 0;
}
