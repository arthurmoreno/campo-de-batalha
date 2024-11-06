// SpriteSet.cpp
#include "SpriteSet.hpp"

#include <stdexcept>

SpriteSet::SpriteSet(const std::string& name) : name_(name) {}

void SpriteSet::addRect(const std::string& name, int x, int y, int w, int h) {
    SDL_Rect rect = {x, y, w, h};
    rects_[name] = rect;
}

const SDL_Rect& SpriteSet::getRect(const std::string& name) const {
    auto it = rects_.find(name);
    if (it != rects_.end()) {
        return it->second;
    }
    throw std::out_of_range("Sprite name not found: " + name);
}

bool SpriteSet::hasRect(const std::string& name) const { return rects_.find(name) != rects_.end(); }

size_t SpriteSet::size() const { return rects_.size(); }

const std::string& SpriteSet::getName() const { return name_; }