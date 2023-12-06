#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include"./menu.cpp"

int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Failed to initialize SDL." << std::endl;
        return -1;
    }
    
     
    controll(); 
    
    close();

    return 0;
}