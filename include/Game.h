#pragma once

#include <SFML/Graphics.hpp>
#include "Capsule.h"
// #include "Enemy.h"
// #include "Turret.h"
// #include "Projectile.h"
// #include "TargetQueue.h"
// #include "Spawner.h"

class Game {
public:
    Game();
    void run();

private:
    // ------- STATE ENUM -------
    enum class GameState {
        LOADING,
        MENU,
        INTRO,
        TUTORIAL,
        PLAYING,
        WIN,
        LOSE
    };

    GameState currentState;
    
    // ------- CORE LOOP -------
    void processEvents();
    void update(float dt);
    void render();

    // ------- STATE HELPERS -------
    void processLoadingEvents(const sf::Event& event);
    void processMenuEvents(const sf::Event& event);
    void processIntroEvents(const sf::Event& event);
    void processTutorialEvents(const sf::Event& event);
    void processPlayingEvents(const sf::Event& event);
    void processWinEvents(const sf::Event& event);
    void processLoseEvents(const sf::Event& event);

    void updateLoading(float dt);
    void updateMenu(float dt);
    void updateIntro(float dt);
    void updateTutorial(float dt);
    void updatePlaying(float dt);
    void updateWin(float dt);
    void updateLose(float dt);

    void renderLoading();
    void renderMenu();
    void renderIntro();
    void renderTutorial();
    void renderPlaying();
    void renderWin();
    void renderLose();

    // ------- SFML WINDOW -------
    sf::RenderWindow window;
    sf::Font font;
    sf::Text debugText;

    Capsule capsule;

    // Example timer (useful for intro/loading screens)
    float stateTimer;
    // std::vector<Enemy> enemies;
    // std::vector<Turret> turrets;
    // std::vector<Projectile> projectiles;
    // TargetQueue targetQueue;
    // Spawner spawner;

    bool gameOver;
    bool gameWon;

    
};