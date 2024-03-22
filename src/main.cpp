#include <iostream>
#include <glm/glm.hpp>
#include "App.h"

int main() {
    const auto app = std::make_unique<App>("Reach", 600, 800);
    app->Run();
    return 0;
}