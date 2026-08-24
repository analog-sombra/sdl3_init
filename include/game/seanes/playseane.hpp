#ifndef __PLAY_SEANE_HPP__
#define __PLAY_SEANE_HPP__

#include <SDL3/SDL.h>
#include "engine/assets_manager.hpp"
#include "engine/seane/seane.hpp"
#include "const.hpp"
#include "engine/seane/seane_manager.hpp"
#include <flecs.h>
#include "engine/element/text_element.hpp"
#include "engine/element/rect_element.hpp"
#include <fmt/format.h>
#include <fmt/core.h>
#include <vector>
#include <deque>

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class PlaySeane : public Seane
{
    // Grid constants
    static constexpr int GRID_COLS = 40;
    static constexpr int GRID_ROWS = 28;
    static constexpr float GAP_SIZE = 1.0f;
    float gridCellSize;
    float gridStartY;  // Y offset after top bar

    // Snake
    struct SnakeSegment
    {
        int gridX;
        int gridY;
    };
    std::deque<SnakeSegment> snakeBody;
    Direction currentDirection = Direction::RIGHT;
    Direction nextDirection = Direction::RIGHT;
    float moveTimer = 0.0f;
    static constexpr float MOVE_SPEED = 0.1f;  // Move every 0.1 seconds
    bool pauseKeyPressed = false;  // Debounce flag for pause key

    // Food
    struct Food
    {
        int gridX;
        int gridY;
    };
    Food food;

    flecs::entity player;
    flecs::entity good;
    flecs::entity scoreEntity;
    flecs::entity backgroundEntity;
    int score = 0;

    void CreateGrid();
    void CreateSnake();
    void MoveSnake();
    void RenderSnake();
    void UpdateSnakeDirection();
    void SpawnFood();
    void CheckCollision();
    void UpdateScore();
    void CheckGameOver();

public:
    PlaySeane(AssetsManager *assetsManager, SDL_Renderer *renderer);
    ~PlaySeane();
    void Render();
    void Update(float deltaTime); // Now accepts deltaTime parameter
    void HandleEvents(SDL_Event *event);
    void Reset();  // Reset game state for new game
};

#endif // __PLAY_SEANE_HPP__