#include "Physics.hpp"

// Function to create a collision box based on an original SDL_Rect
SDL_Rect createCollisionBox(const SDL_Rect& originalRect, int shrinkX, int shrinkY) {
    SDL_Rect collisionBox;

    // Adjust the position to center the collision box
    collisionBox.x = originalRect.x + shrinkX;
    collisionBox.y = originalRect.y + shrinkY;

    // Adjust the width and height
    collisionBox.w = originalRect.w - shrinkX * 2;
    collisionBox.h = originalRect.h - shrinkY * 2;

    // Ensure the width and height are not negative
    if (collisionBox.w < 0) collisionBox.w = 0;
    if (collisionBox.h < 0) collisionBox.h = 0;

    return collisionBox;
}

// funcao que verifica a colisao entre objetos
bool checkCollisionBetweenTwoRects(int ax, int ay, int aw, int ah, int cx, int cy, int cw, int ch) {
    int bx, by, dx, dy;
    bx = ax + aw;
    by = ay + ah;
    dx = cx + cw;
    dy = cy + ch;
    return (((ax > dx) || (bx < cx) || (ay > dy) || (by < cy)));
}

bool checkProjectilesCollision(const SDL_Rect& playerPosition, std::vector<Projectile>& projectiles,
                               const bool shouldErase) {
    bool notCollidedWithProjectile = true;
    for (auto projectile = projectiles.begin(); projectile != projectiles.end();) {
        bool _notCollidedWithProjectile = checkCollisionBetweenTwoRects(
            playerPosition.x, playerPosition.y, playerPosition.w, playerPosition.h,
            projectile->position.x, projectile->position.y, projectile->position.w,
            projectile->position.h);
        notCollidedWithProjectile = (notCollidedWithProjectile && _notCollidedWithProjectile);

        if (!_notCollidedWithProjectile && shouldErase) {
            // Collision detected, remove projectile
            projectile = projectiles.erase(projectile);  // Erase returns the next iterator
        } else {
            ++projectile;  // Move to the next element
        }
    }

    return notCollidedWithProjectile;
}

bool checkCollision(const SDL_Rect& firstPlayerPosition, const SDL_Rect& secondPlayerPosition,
                    const SDL_Rect& wall, const SDL_Rect& menu,
                    std::vector<Projectile>& projectiles, const bool checkProjectiles,
                    const bool shouldErase) {
    bool notCollidedWithProjectile = true;
    if (checkProjectiles) {
        notCollidedWithProjectile =
            checkProjectilesCollision(firstPlayerPosition, projectiles, shouldErase);
    }

    return (checkCollisionBetweenTwoRects(firstPlayerPosition.x, firstPlayerPosition.y,
                                          firstPlayerPosition.w, firstPlayerPosition.h, wall.x,
                                          wall.y, wall.w, wall.h) &&
            checkCollisionBetweenTwoRects(firstPlayerPosition.x, firstPlayerPosition.y,
                                          firstPlayerPosition.w, firstPlayerPosition.h,
                                          secondPlayerPosition.x, secondPlayerPosition.y,
                                          secondPlayerPosition.w, secondPlayerPosition.h) &&
            checkCollisionBetweenTwoRects(firstPlayerPosition.x, firstPlayerPosition.y,
                                          firstPlayerPosition.w, firstPlayerPosition.h, menu.x,
                                          menu.y, menu.w, menu.h) &&
            notCollidedWithProjectile);
}

void fireProjectile(Player& player, std::vector<Projectile>& projectiles,
                    size_t& projectiles_counter) {
    if (player.firingState == PlayerStateEnum::IDLE) {
        Projectile projectile = Projectile(projectiles_counter);

        switch (player.direction) {
            case static_cast<int>(DirectionEnum::UP): {
                projectile.position.x = player.position.x + 12;
                projectile.position.y = player.position.y - 20;
                projectile.position.w = 12;
                projectile.position.h = 12;
                projectile.setDirection(DirectionEnum::UP);

                player.firingState = PlayerStateEnum::FIRING;
                player.firingCooldown = PLAYER_FIRING_TIMER;
            } break;
            case static_cast<int>(DirectionEnum::DOWN): {
                projectile.position.x = player.position.x + 12;
                projectile.position.y = player.position.y + 40;
                projectile.position.w = 12;
                projectile.position.h = 12;
                projectile.setDirection(DirectionEnum::DOWN);

                player.firingState = PlayerStateEnum::FIRING;
                player.firingCooldown = PLAYER_FIRING_TIMER;
            } break;
            case static_cast<int>(DirectionEnum::RIGHT): {
                projectile.position.x = player.position.x + 40;
                projectile.position.y = player.position.y + 12;
                projectile.position.w = 12;
                projectile.position.h = 12;
                projectile.setDirection(DirectionEnum::RIGHT);

                player.firingState = PlayerStateEnum::FIRING;
                player.firingCooldown = PLAYER_FIRING_TIMER;
            } break;
            case static_cast<int>(DirectionEnum::LEFT): {
                projectile.position.x = player.position.x - 20;
                projectile.position.y = player.position.y + 12;
                projectile.position.w = 12;
                projectile.position.h = 12;
                projectile.setDirection(DirectionEnum::LEFT);

                player.firingState = PlayerStateEnum::FIRING;
                player.firingCooldown = PLAYER_FIRING_TIMER;
            } break;

            default:
                break;
        }

        projectile.movingCooldown = PROJECTILE_MOVING_COOLDOWN;
        projectiles.push_back(projectile);
        projectiles_counter++;
    }
}

void processProjectilesPhysics(std::vector<Projectile>& projectiles) {
    for (auto projectile = projectiles.begin(); projectile != projectiles.end();) {
        if (projectile->movingCooldown > 0) {
            projectile->movingCooldown--;
        } else {
            switch (projectile->direction) {
                case static_cast<int>(DirectionEnum::UP): {
                    projectile->position.y = projectile->position.y - PROJECTILE_MOVING_UNITS;
                    projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                } break;
                case static_cast<int>(DirectionEnum::DOWN): {
                    projectile->position.y = projectile->position.y + PROJECTILE_MOVING_UNITS;
                    projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                } break;
                case static_cast<int>(DirectionEnum::RIGHT): {
                    projectile->position.x = projectile->position.x + PROJECTILE_MOVING_UNITS;
                    projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                } break;
                case static_cast<int>(DirectionEnum::LEFT): {
                    projectile->position.x = projectile->position.x - PROJECTILE_MOVING_UNITS;
                    projectile->movingCooldown = PROJECTILE_MOVING_COOLDOWN;
                } break;

                default:
                    break;
            }
        }
        if (projectile->position.x < -10 || projectile->position.x > 1000 ||
            projectile->position.y < -10 || projectile->position.y > 1000) {
            projectile = projectiles.erase(projectile);  // erase returns the next valid iterator
        } else {
            ++projectile;  // Only increment if no deletion occurred
        }
    }
}