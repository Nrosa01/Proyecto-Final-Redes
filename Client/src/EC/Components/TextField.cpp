#include "TextField.h"
#include "Transform.h"
#include "../../SDLUtils/SDLApp.h"
#include "../../SDLUtils/Texture.h"
#include "../Entity.h"

TextField::TextField(Texture *buttonImg, std::string font, int maxLength) : Component("TextField"), texture(buttonImg), font(font), maxLength(maxLength)
{
    this->text = "";
    this->transform = nullptr;
    this->wasOverLastFrame = false;
    selected = false;
}

TextField::~TextField() {}

void TextField::init()
{
    this->transform = this->ent->GetTransform();
    this->initialScale = transform->GetScale();
    this->textLine = this->ent->GetGame()->getTexture("textField");
    pulseSpeed = 0.75f;
    lineAlpha = 255;
}

void TextField::update(float deltaTime)
{
    bool mouseButtonPressed = Input()->IsMouseButtonPressed(0);

    if (mouseButtonPressed)
        selected = isMouseOver();

    if (selected && Input()->anyKeyWasPressed())
    {
        if (Input()->keyWasPressed(SDLK_BACKSPACE))
        {
            if (text.length() > 0)
                text.pop_back();
        }
        else
        {
            char c = Input()->GetLastKeyPressed();
            if (c >= 'a' && c <= 'z')
            {
                if (text.length() < maxLength)
                    text += c;
            }
        }
    }

    if (selected)
        timer += deltaTime * pulseSpeed;
    else
        timer -= deltaTime * pulseSpeed;

    //Clamp timer between 0 and 1
    if (timer > 1.0f)
        timer = 0.0f;
    else if (timer < 0.0f)
        timer = 0.0f;
    lineAlpha = 255 * (sin(timer * 3.14f));

    textLine->setAlpha((int)this->lineAlpha);
}

void TextField::render()
{
    SDL_Rect destRect;
    int textW = texture->getW() * transform->GetScale().x;
    int textH = texture->getH() * transform->GetScale().y;
    destRect.x = transform->GetPosition().getX() - textW / 2;
    destRect.y = transform->GetPosition().getY() - textH / 2;
    destRect.w = textW;
    destRect.h = textH;

    texture->render(destRect);
    textLine->render(destRect);

    if (text.size() == 0)
        return;

    int textPosX = transform->GetPosition().getX();
    int textPosY = transform->GetPosition().getY();
    SDL_Color defaultColor = {255, 255, 255, 255};
    this->ent->GetGame()->renderText(textPosX, textPosY, text.c_str(), font, defaultColor);
}

bool TextField::isPressed()
{
    bool mouseButtonPressed = Input()->IsMouseButtonPressed(0);
    bool mouseOver = isMouseOver();

    return mouseButtonPressed && mouseOver;
}

bool TextField::isMouseOver()
{
    Vector2D mousePos = Input()->GetMousePosition();

    // Check if mousePos is inside the button
    if (mousePos.getX() > transform->GetPosition().getX() - texture->getW() / 2 && mousePos.getX() < transform->GetPosition().getX() + texture->getW() / 2 &&
        mousePos.getY() > transform->GetPosition().getY() - texture->getH() / 2 && mousePos.getY() < transform->GetPosition().getY() + texture->getH() / 2)
    {
        return true;
    }
    else
        return false;
}