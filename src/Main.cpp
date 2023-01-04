#include <SDL.h>
#include "WindowHandler.h"

class Application
{
private:
    WindowHandler *handler;
    SDL_Window *window;
    SDL_Event event;
    bool running;
    char *window_name = "SDL2 Vulkan Demo";

    void Init()
    {
        window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
        handler = new WindowHandler(window, window_name);
        running = true;
    }

    void MainLoop()
    {
        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }

            handler->AcquireNextImage();
            handler->ResetCommandBuffer();
            handler->BeginCommandBuffer();

            VkClearColorValue clear_color = {0, 255, 0, 255}; // RGBA
            VkClearDepthStencilValue clear_depth_stencil = {1.0f, 0};
            handler->BeginRenderPass(clear_color, clear_depth_stencil);
            
            handler->EndRenderPass();
            handler->EndCommandBuffer();
            handler->QueueSubmit();
            handler->QueuePresent();
        }
    }

    void Cleanup()
    {
        SDL_DestroyWindow(window);
        window = nullptr;

        delete handler;
        handler = nullptr;

        SDL_Quit();
    }

public:
    void Run()
    {
        SDL_Init(SDL_INIT_EVERYTHING);
        Init();
        MainLoop();
        Cleanup();
    }
};

int main(int argc, char *argv[])
{
    Application app;

    try
    {
        app.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}