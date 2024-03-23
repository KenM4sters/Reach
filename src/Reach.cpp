#include "Helpers/Includes.h"
#include "App.h"

class Reach : public App {
    public:
        Reach(std::string name, uint16_t w, uint16_t h)
            : App(name, w, h) 
        {

        }
        ~Reach() 
        {

        }
};


int main() {
    std::unique_ptr<Reach> reach = std::make_unique<Reach>("Reach", 600, 800);
    reach->Run();
    return 0;
}