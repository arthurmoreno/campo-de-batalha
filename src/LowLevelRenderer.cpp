#include "LowLevelRenderer.hpp"

// *****************************
// Low level rendering functions
// *****************************

SDL_Texture* createTextureFromBMPWithGreenBG(SDL_Renderer* renderer,
                                             const std::string& spriteFileName) {
    SDL_Surface* tmpSurface = nullptr;
    tmpSurface = SDL_LoadBMP(spriteFileName.c_str());
    SDL_SetColorKey(tmpSurface, SDL_TRUE, SDL_MapRGB(tmpSurface->format, 0, 255, 0));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return texture;
}

SDL_Texture* createTextureFromBMP(SDL_Renderer* renderer, const std::string& spriteFileName) {
    SDL_Surface* tmpSurface = nullptr;
    tmpSurface = SDL_LoadBMP(spriteFileName.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return texture;
}

// funcao para carregar os tanques na tela
void drawPlayer(int playerDirection, SDL_Rect position, SDL_Texture* playerTexture,
                SpriteSet& playerSpriteSet, SDL_Renderer* renderer) {
    SDL_Rect playerSpriteRect;
    switch (playerDirection) {
        case static_cast<int>(DirectionEnum::UP):
            playerSpriteRect = playerSpriteSet.getRect("UP");
            break;
        case static_cast<int>(DirectionEnum::DOWN):
            playerSpriteRect = playerSpriteSet.getRect("DOWN");
            break;
        case static_cast<int>(DirectionEnum::RIGHT):
            playerSpriteRect = playerSpriteSet.getRect("RIGHT");
            break;
        case static_cast<int>(DirectionEnum::LEFT):
            playerSpriteRect = playerSpriteSet.getRect("LEFT");
            break;
        default:
            playerSpriteRect = playerSpriteSet.getRect("DOWN");
            break;
    }
    SDL_RenderCopy(renderer, playerTexture, &playerSpriteRect, &position);
}

// funcao para carregar os tanques na tela
void drawExplosion(int explosionFrame, SDL_Rect position, SDL_Texture* explosionTexture,
                   SpriteSet& explosionSpriteSet, SDL_Renderer* renderer) {
    SDL_Rect explosionSpriteRect;
    switch (explosionFrame) {
        case 0:
            explosionSpriteRect = explosionSpriteSet.getRect("FIRST");
            break;
        case 1:
            explosionSpriteRect = explosionSpriteSet.getRect("SECOND");
            break;
        case 2:
            explosionSpriteRect = explosionSpriteSet.getRect("THIRD");
            break;
        case 3:
            explosionSpriteRect = explosionSpriteSet.getRect("FORTH");
            break;
        default:
            explosionSpriteRect = explosionSpriteSet.getRect("FIRST");
            break;
    }
    SDL_RenderCopy(renderer, explosionTexture, &explosionSpriteRect, &position);
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y,
                SDL_Color color) {
    // Render text to an SDL_Surface
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!textSurface) {
        SDL_Log("TTF_RenderUTF8_Blended Error: %s", TTF_GetError());
        return;
    }

    // Convert SDL_Surface to SDL_Texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Set destination rectangle for rendering the text
    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};

    // Render the texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Function to render an integer as text
void renderInteger(SDL_Renderer* renderer, TTF_Font* font, int number, int x, int y,
                   SDL_Color color) {
    // Convert integer to string
    std::string text = std::to_string(number);

    // Render text to an SDL_Surface
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!textSurface) {
        SDL_Log("TTF_RenderUTF8_Blended Error: %s", TTF_GetError());
        return;
    }

    // Convert SDL_Surface to SDL_Texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Set destination rectangle for rendering the text
    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};

    // Render the texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
