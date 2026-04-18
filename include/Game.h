#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <string>
#include "Capsule.h"
#include "Enemy.h"
// #include "Turret.h"
// #include "Projectile.h"
// #include "TargetQueue.h"
#include "Spawner.h"

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
    enum class BackgroundId {
        LOADING,
        MENU,
        INTRO,
        GAMEPLAY
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
    void resetIntroSequence();
    void loadIntroTextFromFile();
    void loadBackgroundTextures();
    void drawBackgroundForState(GameState state);
    BackgroundId backgroundForState(GameState state) const;

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
    sf::Text introText;
    std::string introTextContent;

    Capsule capsule;

    // Example timer (useful for intro/loading screens)
    float stateTimer;
    float introTextY;
    float introScrollSpeed;
    float introFadeTopFraction;
    float introFadeBottomFraction;
    float introBackgroundFadeAlpha;
    float introBackgroundFadeSeconds;
    sf::RectangleShape introFadeOverlay;
    std::vector<Enemy> enemies;
    // std::vector<Turret> turrets;
    // std::vector<Projectile> projectiles;
    // TargetQueue targetQueue;
    Spawner spawner;

    bool gameOver;
    bool gameWon;

    static constexpr std::size_t kBackgroundCount = 4u;
    std::array<sf::Texture, kBackgroundCount> backgroundTextures;
    std::array<bool, kBackgroundCount> backgroundLoaded;
};