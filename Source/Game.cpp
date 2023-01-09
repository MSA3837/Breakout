#include "game.h"

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

SpriteRenderer* Renderer;

GameObject* Player;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    ResourceManager::LoadShader("Source/Shaders/Sprite.vrtx", "Source/Shaders/Sprite.frag", nullptr, "sprite");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    Shader shader = ResourceManager::GetShader("sprite");

    Renderer = new SpriteRenderer(shader);

    ResourceManager::LoadTexture("Source/Sprites/background.jpg", false, "background");
    ResourceManager::LoadTexture("Source/Sprites/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("Source/Sprites/block.png", false, "block");
    ResourceManager::LoadTexture("Source/Sprites/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("Source/Sprites/paddle.png", true, "paddle");

    GameLevel one; 
    one.Load("Source/Levels/Standard.lvl", this->Width, this->Height / 2);

    GameLevel two; 
    two.Load("Source/Levels/FewSmallGaps.lvl", this->Width, this->Height / 2);

    GameLevel three; 
    three.Load("Source/Levels/SpaceInvaders.lvl", this->Width, this->Height / 2);

    GameLevel four; 
    four.Load("Source/Levels/BounceGalore.lvl", this->Width, this->Height / 2);

    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);

    this->Level = 1;

    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);

    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;

        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        Texture2D background = ResourceManager::GetTexture("background");

        Renderer->DrawSprite(background, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

        this->Levels[this->Level].Draw(*Renderer);

        Player->Draw(*Renderer);
    }
}
