# Game Engine Roadmap - Making 200-250 Line Games

## Current State
- **Engine:** ~2,000 lines (good baseline)
- **Simple Game (Snake):** ~1,000 lines (can be reduced to 200-250)
- **Gap:** Missing high-level features that reduce boilerplate

---

## Phase 1: Core Engine Features (CRITICAL) ⭐

### 1. **Enhanced UI System** (Button/Input abstraction)
**Current Problem:** Buttons manually handled in each scene
```cpp
// Current: 15-20 lines per button
if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    if (mouseX >= restart.rect.x && mouseX <= restart.rect.x + restart.rect.w &&
        mouseY >= restart.rect.y && mouseY <= restart.rect.y + restart.rect.h) {
        // Handle click
    }
}
```

**Solution - UIButton Component:**
```cpp
// Target: 2-3 lines per button
UIButton restartBtn("Restart", {WINDOW_WIDTH/2, WINDOW_HEIGHT/2});
if (restartBtn.IsClicked(event)) {
    // Handle click
}
```

**Files to Create:**
- `include/engine/ui/ui_button.hpp`
- `src/engine/ui/ui_button.cpp`
- `include/engine/ui/ui_elements.hpp` (base class)

**Savings:** 80-120 lines per game

---

### 2. **Enhanced Element Positioning** (Remove manual rect math)
**Current Problem:** Manual positioning math repeated everywhere
```cpp
// Current: 5 lines of boilerplate per element
TextElement text = textEntity.get<TextElement>();
textEntity.set<TextElement>({text.texture,
                            SDL_FRect{text.rect.x - text.rect.w / 2, 
                                     text.rect.y - text.rect.h / 2, 
                                     text.rect.w, text.rect.h}});
```

**Solution - Element Helper Methods:**
```cpp
// Target: 1 line
CenterText(textEntity, renderer, world, font, "Score: 100", WINDOW_WIDTH/2, 20);
```

**Features Needed:**
- `CenterText()` - Auto-centers text
- `CenterImage()` - Auto-centers images
- `AlignUI()` - Align multiple elements (left, right, center, vertical spacing)
- Anchor points (TOP_LEFT, CENTER, BOTTOM_RIGHT, etc.)

**Files to Modify:**
- `include/engine/element/text_element.hpp` (add helper struct)
- `include/engine/element/image_element.hpp` (add helper struct)
- `include/engine/engine.hpp` (add global helper functions)

**Savings:** 40-60 lines per game

---

### 3. **Scene/Game State Base Class**
**Current Problem:** Every scene repeats input handling patterns
```cpp
// Current: Pause, MainMenu, GameOver all have 30+ lines of identical button code
if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT) {
    if (!prevMouseButtonState) {
        prevMouseButtonState = true;
        // ... button checks repeated 3x
    }
}
```

**Solution - UIScene Base Class:**
```cpp
// Target: Inherit from UIScene, define buttons once
class MenuScene : public UIScene {
    MenuScene() : UIScene() {
        AddButton("play", "Play", {CENTER, 200});
        AddButton("quit", "Quit", {CENTER, 250});
    }
    
    void OnButtonClicked(string buttonId) override {
        if (buttonId == "play") manager->PushSeane("PlaySeane");
    }
};
```

**Files to Create:**
- `include/engine/ui/ui_scene.hpp` (base class with button management)
- `src/engine/ui/ui_scene.cpp`

**Savings:** 60-100 lines per UI scene (×3 for your game = 180-300 lines)

---

### 4. **Input Handling System**
**Current Problem:** Input scattered across HandleEvents()
- No easy way to bind keys to actions
- No axis input (for continuous movement)
- Debouncing logic repeated

**Solution - InputManager:**
```cpp
// Target usage
inputMgr->BindKey(SDLK_UP, "move_up");
inputMgr->BindKey(SDLK_SPACE, "jump");

void Update() {
    if (inputMgr->IsKeyDown("move_up")) { /* move */ }
    if (inputMgr->IsKeyPressed("jump")) { /* jump once */ }
}
```

**Features:**
- Key binding system (string-based actions)
- Axis input (joystick/analog support)
- Input buffering (for fighting games)
- Rebinding support

**Files to Create:**
- `include/engine/input/input_manager.hpp`
- `src/engine/input/input_manager.cpp`

**Savings:** 30-50 lines per game

---

## Phase 2: Physics & Collision (IMPORTANT) ⭐⭐

### 5. **Box2D Integration**
**Current Problem:** Manual collision detection
```cpp
// Current: Per-game collision code
if (snakeBody[0].gridX == food.gridX && snakeBody[0].gridY == food.gridY) {
    score += 10;
}
```

**Solution - Physics Component:**
```cpp
// Target
flecs::entity food = world.entity()
    .set<Transform>({x, y})
    .set<Rigidbody>({1.0f, DYNAMIC})
    .set<BoxCollider>({width, height})
    .set<Tag_Food>();  // For filtering

// Query collisions
world.query<Collision>().each([](Collision &c) {
    if (c.a.has<Tag_Snake>() && c.b.has<Tag_Food>()) {
        // Handle collision
    }
});
```

**Files to Create:**
- `include/engine/physics/physics_manager.hpp`
- `src/engine/physics/physics_manager.cpp`
- `include/engine/physics/components.hpp` (Transform, Rigidbody, Collider, etc.)

**Savings:** 50-80 lines (no manual collision logic)

---

## Phase 3: Nice-to-Have Features (OPTIONAL) ⭐

### 6. **Particle System**
- For food eaten effects, collision effects
- Generic emitter + particle pool
- ~200 lines → reusable for all games

### 7. **Animation System**
- SpriteSheet support
- Frame-based animation
- Blend between animations

### 8. **Audio Manager** (Extend existing)
- Sound effect pooling
- Music crossfade
- Volume control per type

### 9. **Config/Level System**
```cpp
// Load game config from JSON/YAML
Config cfg = ConfigLoader::Load("game.yml");
int gridWidth = cfg.GetInt("game.grid_width");
```

### 10. **Screen/Resolution Manager**
- Handle different resolutions
- Aspect ratio scaling
- Fullscreen support

---

## Target Game Structure: 200-250 Lines

```cpp
// game.cpp - TOTAL GAME CODE

#include "engine.hpp"

class SnakeGame : public UIScene {
public:
    SnakeGame() : UIScene() {
        // Initialize game (20 lines)
        snakeBody.push_back({20, 14});
        snakeBody.push_back({19, 14});
        snakeBody.push_back({18, 14});
        SpawnFood();
        
        // Setup input (5 lines)
        inputMgr->BindKey(SDLK_UP, "up");
        inputMgr->BindKey(SDLK_DOWN, "down");
        // ... other keys
    }
    
    void Update(float dt) override {
        // Game logic (40 lines)
        if (inputMgr->IsKeyDown("up")) direction = UP;
        // ... movement, collision detection
        
        if (IsGameOver()) OnGameOver();
    }
    
    void OnGameOver() override {
        // Transition (5 lines)
        seaneManager->PushSeane("GameOverScene");
    }
    
    // Render handled by engine query system (5 lines)
    void Render() override { /* Use engine's automatic rendering */ }
    
private:
    std::deque<Pos> snakeBody;  // Simplified data
    Pos food;
    Direction direction = RIGHT;
    // ... minimal state
};

// UI Scene (GameOverScene): 30-40 lines
class GameOverScene : public UIScene {
    // Setup buttons in constructor
    // OnButtonClicked() handles restart/menu/quit
    // All rendering automated
};

// Menu scene: 25-30 lines
class MenuScene : public UIScene {
    // Just button definitions
    // All logic automated
};

// Main setup: 10 lines
int main() {
    Engine engine;
    engine.Init();
    
    auto mgr = engine.GetSeaneManager();
    mgr->AddSeane("Menu", std::make_unique<MenuScene>());
    mgr->AddSeane("Game", std::make_unique<SnakeGame>());
    mgr->PushSeane("Menu");
    
    engine.Run();
}
```

**Total: ~230 lines ✅**

---

## Implementation Priority

### Week 1: (Phase 1 - UI System)
1. ✅ UIButton component
2. ✅ UIScene base class
3. ✅ Element positioning helpers
4. Refactor 3 existing UI scenes using new system

### Week 2: (Phase 1 - Input)
5. ✅ InputManager
6. Refactor PlaySeane to use InputManager
7. Test with existing game

### Week 3: (Phase 2 - Physics)
8. ✅ Box2D integration
9. Create physics manager
10. Refactor collision detection

### Week 4+: (Phase 3 - Polish)
11. Particle system
12. Animation system
13. Config system
14. Audio improvements

---

## Estimated Results

| Feature | Lines Saved | Effort |
|---------|-------------|--------|
| UIButton + UIScene | 180-300 | 4-5 hours |
| Element Positioning | 40-60 | 1-2 hours |
| InputManager | 30-50 | 2-3 hours |
| Box2D Physics | 50-80 | 3-4 hours |
| **TOTAL** | **300-490 lines** | **10-14 hours** |

### After Optimization
- **Current:** 1,019 lines of game code
- **Target:** 200-250 lines per game
- **Reusability:** Engine grows to 2,500-3,000 lines (one-time investment)
- **Payoff:** Every new game is 75% smaller, 50% faster to develop

---

## Code Examples

### Before (Current)
```cpp
// PlaySeane.cpp: 377 lines
// PauseSeane.cpp: 142 lines
// GameOverSeane.cpp: 120 lines
// MenuSeane.cpp: 93 lines
// TOTAL: 732 lines for one simple game
```

### After (Optimized)
```cpp
// SnakeGame.cpp: 200 lines (all game logic + all scenes)
// SAVINGS: 532 lines (-73%)
```

---

## Next Steps

1. **Run this analysis** - Which features matter most for your game style?
2. **Pick Phase 1 features** - Start with UIButton + UIScene (biggest impact)
3. **Refactor existing game** - Use new features in Snake game
4. **Create new test game** - Build a simple game in <250 lines to validate

Would you like me to start implementing any of these features?

---

## Questions to Guide Development

- **Q1:** Do you want physics-based games? → Prioritize Box2D
- **Q2:** Do you want fast UI prototyping? → Prioritize UIScene
- **Q3:** Do you need animations? → Add animation system early
- **Q4:** Will you use particle effects? → Add particle system
- **Q5:** Do you need multiple game types? → Make engine super generic

**Answer these and we can fine-tune the roadmap!**
