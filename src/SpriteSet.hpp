// SpriteSet.h
#ifndef SPRITESET_H
#define SPRITESET_H

#include <string>
#include <unordered_map>

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <SDL2/SDL.h>
#elif defined(__linux__)
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class SpriteSet {
   public:
    // Constructor
    SpriteSet(const std::string& name);

    // Add a sprite rectangle with a name
    void addRect(const std::string& name, int x, int y, int w, int h);

    // Retrieve a sprite rectangle by name
    const SDL_Rect& getRect(const std::string& name) const;

    // Check if a sprite exists
    bool hasRect(const std::string& name) const;

    // Get the number of rectangles
    size_t size() const;

    // Get the name of the sprite set
    const std::string& getName() const;

   private:
    std::string name_;
    std::unordered_map<std::string, SDL_Rect> rects_;
};

#endif  // SPRITESET_H