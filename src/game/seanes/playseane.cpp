#include "game/seanes/playseane.hpp"
#include "game/seanes/gameoverseane.hpp"
#include <cstdlib>
#include <ctime>

PlaySeane::PlaySeane(AssetsManager *assetsManager, SDL_Renderer *renderer) : Seane(assetsManager, renderer)
{
    assetsManager->LoadFont("ui_default", "./assets/font/candy.otf", 24);
    assetsManager->LoadTexture("food", "./assets/images/food.png");
    assetsManager->LoadTexture("bg", "./assets/images/bg.png");

    // Calculate grid cell size based on window dimensions
    gridStartY = 40.0f;  // Below the top bar
    float playableHeight = WINDOW_HEIGHT - gridStartY;
    gridCellSize = (WINDOW_WIDTH - (GRID_COLS - 1) * GAP_SIZE) / GRID_COLS;

    // basic text
    auto font = assetsManager->GetFont("ui_default");

    scoreEntity = CreateText(renderer, world, font.get(), fmt::format("Score: {}", score), WINDOW_WIDTH / 2, 20);

    TextElement score = scoreEntity.get<TextElement>();
    scoreEntity.set<TextElement>({score.texture,
                                  SDL_FRect{score.rect.x - score.rect.w / 2, score.rect.y - score.rect.h / 2, score.rect.w, score.rect.h}});

    CreateRect(world, SDL_FRect{0.f, 0.f, WINDOW_WIDTH, 40.f});

    // Create the grid
    CreateGrid();

    // Create the snake
    CreateSnake();

    // Spawn initial food
    srand(static_cast<unsigned>(time(nullptr)));
    SpawnFood();
}

void PlaySeane::CreateGrid()
{
    // Create grid lines/cells
    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            float x = col * (gridCellSize + GAP_SIZE);
            float y = gridStartY + row * (gridCellSize + GAP_SIZE);

            CreateRect(world, SDL_FRect{x, y, gridCellSize, gridCellSize});
        }
    }
}

void PlaySeane::CreateSnake()
{
    // Initialize snake with 3 segments in the middle of the grid
    snakeBody.push_back({GRID_COLS / 2, GRID_ROWS / 2});
    snakeBody.push_back({GRID_COLS / 2 - 1, GRID_ROWS / 2});
    snakeBody.push_back({GRID_COLS / 2 - 2, GRID_ROWS / 2});
}

void PlaySeane::SpawnFood()
{
    bool validPosition = false;
    while (!validPosition)
    {
        food.gridX = rand() % GRID_COLS;
        food.gridY = rand() % GRID_ROWS;

        // Make sure food doesn't spawn on snake
        validPosition = true;
        for (const auto &segment : snakeBody)
        {
            if (segment.gridX == food.gridX && segment.gridY == food.gridY)
            {
                validPosition = false;
                break;
            }
        }
    }
}

void PlaySeane::CheckCollision()
{
    // Check if snake head ate food
    if (snakeBody[0].gridX == food.gridX && snakeBody[0].gridY == food.gridY)
    {
        score += 10;
        UpdateScore();
        SpawnFood();
    }
}

PlaySeane::~PlaySeane()
{
}

void PlaySeane::Render()
{
    // Render background
    auto bgTexture = this->assetsManager->GetTexture("bg");
    if (bgTexture)
    {
        SDL_FRect bgRect{0.f, gridStartY, WINDOW_WIDTH, WINDOW_HEIGHT - gridStartY};
        SDL_RenderTexture(this->renderer, bgTexture.get(), nullptr, &bgRect);
    }

    // Render top bar
    auto rectQuery = world.query<RectElement>();
    rectQuery.each([this](flecs::entity e, RectElement &rectElement)
                   {
                       SDL_FRect rect{
                           rectElement.rect.x,
                           rectElement.rect.y,
                           rectElement.rect.w,
                           rectElement.rect.h
                       };
                       // Only render top bar, skip grid
                       if (rect.y < gridStartY)
                       {
                           SDL_SetRenderDrawColor(this->renderer, 100, 100, 100, 255);
                           SDL_RenderFillRect(this->renderer, &rect);
                       }
                   });

    // Render snake
    RenderSnake();

    // Render score text
    auto q3 = world.query<TextElement>();
    q3.each([this](flecs::entity e, TextElement &textelement)
            {
            SDL_FRect rect{
                textelement.rect.x,
                textelement.rect.y,
                textelement.rect.w,
                textelement.rect.h
            };
            SDL_RenderTexture(this->renderer, textelement.texture, nullptr, &rect); });
}

void PlaySeane::RenderSnake()
{
    // Render food using the loaded food image
    auto foodTexture = this->assetsManager->GetTexture("food");
    if (foodTexture)
    {
        float foodX = food.gridX * (gridCellSize + GAP_SIZE);
        float foodY = gridStartY + food.gridY * (gridCellSize + GAP_SIZE);
        SDL_FRect foodRect{foodX, foodY, gridCellSize, gridCellSize};
        SDL_RenderTexture(this->renderer, foodTexture.get(), nullptr, &foodRect);
    }

    // Render snake
    for (size_t i = 0; i < snakeBody.size(); i++)
    {
        float x = snakeBody[i].gridX * (gridCellSize + GAP_SIZE);
        float y = gridStartY + snakeBody[i].gridY * (gridCellSize + GAP_SIZE);

        SDL_FRect snakeRect{x, y, gridCellSize, gridCellSize};

        if (i == 0)
        {
            // Head - brighter green
            SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
        }
        else
        {
            // Body - darker green
            SDL_SetRenderDrawColor(this->renderer, 0, 180, 0, 255);
        }
        SDL_RenderFillRect(this->renderer, &snakeRect);
    }
}

void PlaySeane::Update(float deltaTime)
{
    moveTimer += deltaTime;

    if (moveTimer >= MOVE_SPEED)
    {
        moveTimer = 0.0f;
        UpdateSnakeDirection();
        MoveSnake();
        CheckGameOver();
    }
}

void PlaySeane::UpdateSnakeDirection()
{
    // Check if the new direction is not opposite to the current direction
    if (currentDirection == Direction::UP && nextDirection == Direction::DOWN)
        return;
    if (currentDirection == Direction::DOWN && nextDirection == Direction::UP)
        return;
    if (currentDirection == Direction::LEFT && nextDirection == Direction::RIGHT)
        return;
    if (currentDirection == Direction::RIGHT && nextDirection == Direction::LEFT)
        return;

    currentDirection = nextDirection;
}

void PlaySeane::MoveSnake()
{
    // Calculate new head position
    int newX = snakeBody[0].gridX;
    int newY = snakeBody[0].gridY;

    switch (currentDirection)
    {
    case Direction::UP:
        newY--;
        break;
    case Direction::DOWN:
        newY++;
        break;
    case Direction::LEFT:
        newX--;
        break;
    case Direction::RIGHT:
        newX++;
        break;
    }

    // Add new head
    snakeBody.push_front({newX, newY});

    // Check if snake ate food
    bool ateFoodFlag = (newX == food.gridX && newY == food.gridY);

    // Remove tail only if NOT eating food
    if (!ateFoodFlag)
    {
        snakeBody.pop_back();
    }

    // Check collision with food
    CheckCollision();
}

void PlaySeane::UpdateScore()
{
    // Delete old score text
    scoreEntity.destruct();
    
    // Create new score text with same position
    auto font = this->assetsManager->GetFont("ui_default");
    scoreEntity = CreateText(this->renderer, this->world, font.get(), fmt::format("Score: {}", score), WINDOW_WIDTH / 2, 20);
    
    // Center the text
    TextElement scoreText = scoreEntity.get<TextElement>();
    scoreEntity.set<TextElement>({scoreText.texture,
                                  SDL_FRect{scoreText.rect.x - scoreText.rect.w / 2, scoreText.rect.y - scoreText.rect.h / 2, scoreText.rect.w, scoreText.rect.h}});
}

void PlaySeane::CheckGameOver()
{
    int headX = snakeBody[0].gridX;
    int headY = snakeBody[0].gridY;

    // Check if snake hit the walls
    if (headX < 0 || headX >= GRID_COLS || headY < 0 || headY >= GRID_ROWS)
    {
        // Game Over - hit wall
        auto manager = GetSeaneManager();
        if (manager)
        {
            // Create new GameOverSeane with current score
            manager->DeleteSeane("GameOverSeane");
            manager->AddSeane("GameOverSeane", 
                            std::make_unique<GameOverSeane>(this->assetsManager, this->renderer, score),
                            manager);
            manager->PopSeane();  // Remove play seane
            manager->PushSeane("GameOverSeane");
        }
        return;
    }

    // Check if snake hit itself
    for (size_t i = 1; i < snakeBody.size(); i++)
    {
        if (snakeBody[i].gridX == headX && snakeBody[i].gridY == headY)
        {
            // Game Over - hit self
            auto manager = GetSeaneManager();
            if (manager)
            {
                // Create new GameOverSeane with current score
                manager->DeleteSeane("GameOverSeane");
                manager->AddSeane("GameOverSeane", 
                                std::make_unique<GameOverSeane>(this->assetsManager, this->renderer, score),
                                manager);
                manager->PopSeane();  // Remove play seane
                manager->PushSeane("GameOverSeane");
            }
            return;
        }
    }
}

void PlaySeane::Reset()
{
    // Clear the flecs world and recreate all entities
    world = flecs::world();
    
    // Reset all game state variables
    snakeBody.clear();
    currentDirection = Direction::RIGHT;
    nextDirection = Direction::RIGHT;
    moveTimer = 0.0f;
    pauseKeyPressed = false;
    score = 0;
    
    // Recreate all visual elements
    auto font = assetsManager->GetFont("ui_default");
    
    // Recreate score text
    scoreEntity = CreateText(renderer, world, font.get(), fmt::format("Score: {}", score), WINDOW_WIDTH / 2, 20);
    TextElement scoreText = scoreEntity.get<TextElement>();
    scoreEntity.set<TextElement>({scoreText.texture,
                                  SDL_FRect{scoreText.rect.x - scoreText.rect.w / 2, scoreText.rect.y - scoreText.rect.h / 2, scoreText.rect.w, scoreText.rect.h}});
    
    // Recreate top bar
    CreateRect(world, SDL_FRect{0.f, 0.f, WINDOW_WIDTH, 40.f});
    
    // Recreate the grid
    CreateGrid();
    
    // Recreate the snake
    CreateSnake();
    
    // Spawn new food
    SpawnFood();
}

void PlaySeane::HandleEvents(SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat == 0)
    {
        switch (event->key.key)
        {
        case SDLK_ESCAPE:  // ESC to pause
        {
            if (!pauseKeyPressed)
            {
                pauseKeyPressed = true;
                auto manager = GetSeaneManager();
                if (manager)
                {
                    manager->PushSeane("PauseSeane");
                }
            }
            break;
        }
        case SDLK_UP:
            nextDirection = Direction::UP;
            break;
        case SDLK_DOWN:
            nextDirection = Direction::DOWN;
            break;
        case SDLK_LEFT:
            nextDirection = Direction::LEFT;
            break;
        case SDLK_RIGHT:
            nextDirection = Direction::RIGHT;
            break;
        }
    }
    else if (event->type == SDL_EVENT_KEY_UP)
    {
        if (event->key.key == SDLK_ESCAPE)
        {
            pauseKeyPressed = false;
        }
    }
}