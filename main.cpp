#include "engine/hotreload/watch.hpp"
#include "engine/engine.hpp"
int main()
{

    intiwatch();
    Engine app("Vulkan Window");
    app.Run();

    return EXIT_SUCCESS;
}