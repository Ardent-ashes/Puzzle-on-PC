#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include"./collectcoin.cpp"
const int Start_SIZE = 100;
const int END_SIZE = 50;
int page1=1;
int page2=0;
struct Start {
    int x, y;
};

void pageone();
void pagetwo();
void handleEventspageone(bool&);
void handleEventspagetwo(bool&);
Start st;
Start end;
bool loadBackgroundImageone() {
    SDL_Surface* backgroundSurface = IMG_Load("page1.jpg");
    if (backgroundSurface == nullptr) {
        printf("Failed to load background image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    if (backgroundTexture == nullptr) {
        printf("Failed to create background texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}
bool loadImage() {
    SDL_Surface* backgroundSurface = IMG_Load("bg.jpg");
    if (backgroundSurface == nullptr) {
        printf("Failed to load background image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    if (backgroundTexture == nullptr) {
        printf("Failed to create background texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}
void handleEventspageone(bool& quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
                int prevBlockX = st.x;
                int prevBlockY = st.y;
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                      if (mouseX >=0 && mouseX <= 0 + 100 && mouseY >= 0 &&  mouseY<= 0 + 100)
                      {
                        
                        page1=0;
                        page2=1;
                        return;
                      }
                }
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
                
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                      if (mouseX >=0 && mouseX <= 0 + 100 && mouseY >= 200 &&  mouseY<= 200 + 100)
                      {
                        
                        close();
                        return;
                      }
                }
        }
    }
}

void handleEventspagetwo(bool& quit) {
    
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
               
                if(e.button.button==SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                      if (mouseX >=0 && mouseX <= 50 && mouseY >= 0 &&  mouseY<=50)
                      {
                        page2=0;
                        page1=1;
                        return;
                        
                      }
                }
            

            
        }
    }
}

void pageone()
{
    loadBackgroundImageone();
     
     bool quit = false;

    
    while (!quit) {
        handleEventspageone(quit);

         SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        if(page1==1)
        {

        SDL_Rect startRect = { st.x, st.y, 100, 100 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 153, 255); // Color of the player
        //SDL_RenderFillRect(renderer, &startRect);
         SDL_Rect exRect = { st.x, 200, 100, 100 };
        SDL_SetRenderDrawColor(renderer, 0, 0, 153, 255); // Color of the player
        //SDL_RenderFillRect(renderer, &exRect);
        }  
        else
        {
            return;
        } 

        
        SDL_RenderPresent(renderer);
        }


}

void pagetwo()
{
    loadImage();
     

    end.x = 0; 
    end.y = 0; 

    
     bool quit = false;

    
    while (!quit) {
        handleEventspagetwo(quit);
       

         SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        if(page2==1)
        {

         
        SDL_Rect rRect = { end.x, end.y, 50, 50 };
        SDL_SetRenderDrawColor(renderer, 153,0, 0, 255); 
        SDL_RenderFillRect(renderer, &rRect);
        }
        else
        {
            return;
        }
        SDL_RenderPresent(renderer);
        }


}

void controll()
{
   
     bool quit = false;

    
    while (!quit) {
        handleEventspagetwo(quit);

         SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        pageone();
        if(page2==1)
        {
            game();
            pagetwo();
        }
        if(page1==1)
        {
           
            pageone();
        }

}
}








