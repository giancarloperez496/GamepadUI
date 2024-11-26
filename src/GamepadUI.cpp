#define _USE_MATH_DEFINES

#include <iostream>
#include <filesystem>
#include <cmath>
#include <sstream>
#include <SFML/Graphics.hpp>

const std::string WS_PATH = "D:/Projects/GamepadUI/";
const int WINDOW_X = 1280;
const int WINDOW_Y = 720;
const int DEADZONE = 5; //5 degree tilt deadzone
const int GREEN_BUTTON = 0;
const int RED_BUTTON = 1;
const int YELLOW_BUTTON = 3;
const int BLUE_BUTTON = 2;
const int ORANGE_BUTTON = 4;
int J0_BUTTONCOUNT;


void printJoysticks() {
    for (unsigned int i = 0; i < sf::Joystick::Count; i++) {
        if (sf::Joystick::isConnected(i)) {
            std::cout << "Joystick " << i << " is connected." << std::endl;
        }
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "GamepadUI");
    
    int J0_BUTTONCOUNT = sf::Joystick::getButtonCount(0);
    std::vector<bool> lastState(J0_BUTTONCOUNT, false);

    sf::Font font;
    std::string font_path = WS_PATH + "assets/arial.ttf";
    if (!font.loadFromFile(font_path)) {
        std::cout << "Could not open front: " << font_path << std::endl;
    }

    sf::Text axisText;
    axisText.setFont(font);
    axisText.setCharacterSize(20);
    axisText.setFillColor(sf::Color::White);
    axisText.setPosition(WINDOW_X*.025, WINDOW_Y*.75);
    
    sf::Texture guitar_texture;
    std::string guitar_path = WS_PATH + "assets/guitar.png";
    if (!guitar_texture.loadFromFile(guitar_path)) {
        std::cout << "Could not open texture: " << guitar_path << std::endl;
    }
    sf::Sprite guitar_sprite(guitar_texture);
    guitar_sprite.setOrigin(guitar_texture.getSize().x/2, guitar_texture.getSize().y/2);
    guitar_sprite.setPosition(1280/2, 720/2);
    guitar_sprite.setScale(0.33f, 0.33f);  

    sf::RectangleShape whammyBar;
    whammyBar.setSize(sf::Vector2f(0, 20));
    whammyBar.setFillColor(sf::Color::Yellow);
    whammyBar.setOrigin(0, 0);
    whammyBar.setPosition(WINDOW_X*.15, WINDOW_Y*.75);

    sf::RectangleShape squares[5];
    sf::Color buttonColors[5] = {
        sf::Color::Green,
        sf::Color::Red,
        sf::Color::Yellow,
        sf::Color::Blue,
        sf::Color(255, 165, 0)
    };
    for (int i = 0; i < 5; i++) {
        squares[i].setSize(sf::Vector2f(75, 75));
        squares[i].setPosition(WINDOW_X*0.125 + 150 * (i + 1), WINDOW_Y*.1);
        squares[i].setFillColor(buttonColors[i]);
    }

    bool buttonStates[5] = {false};

    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        /*
        Button Mapping
        0 - green
        1 - red
        2 - blue
        3 - yellow
        4 - orange
        R - tilt axis , [-100, ~+82] deg
        U - whammy axis, [-100, +100]
        */
        float r = sf::Joystick::getAxisPosition(0, sf::Joystick::R);
        float u = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
        float v = sf::Joystick::getAxisPosition(0, sf::Joystick::V);

        for (int i = 0; i < 5; i++) {
            bool currentState = false;
            switch (i) {
            case 2:
                currentState = sf::Joystick::isButtonPressed(0, 3);
                break;
            case 3:
                currentState = sf::Joystick::isButtonPressed(0, 2);
                break;
            default:
                currentState = sf::Joystick::isButtonPressed(0, i);
                break;
            }
            if (currentState != lastState[i]) {
                std::cout << "Button " << i << " is " << (currentState ? "pressed" : "released") << std::endl;
                lastState[i] = currentState;
            }
            if (currentState && !buttonStates[i]) {
                squares[i].setFillColor(sf::Color::White);
                buttonStates[i] = true;
            }
            else if (!currentState && buttonStates[i]) {
                squares[i].setFillColor(buttonColors[i]);
                buttonStates[i] = false;
            }
        }

        if (r > DEADZONE || r < -DEADZONE) {
            guitar_sprite.setRotation(r);
        }

        /*for (unsigned int i = 0; i < sf::Joystick::getButtonCount(0); i++) {
            bool currentState = sf::Joystick::isButtonPressed(0, i);
            if (currentState != lastState[i]) {
                std::cout << "Button " << i << " is " << (currentState ? "pressed" : "released") << std::endl;
                lastState[i] = currentState;
            }
        }*/

        float normalW = 100 * ((u + 100) / 200); //100 pixels max
        normalW = std::max(normalW, 5.0f);
        whammyBar.setSize(sf::Vector2f(normalW, 20));

        std::stringstream ss;
        ss << "Axis U: " << u;
        axisText.setString(ss.str());
        ss.str("");
        
        window.clear();
        for (int i = 0; i < 5; i++) {
            window.draw(squares[i]);
        }
        window.draw(guitar_sprite);
        window.draw(whammyBar);
        window.draw(axisText);
        window.display();
    }
}