#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstring>

class Hangman {
public:
    Hangman() {
        srand(static_cast<unsigned int>(time(nullptr)));
        const char* word_list[5] = { "AIRPLANE", "PROGRAMMING", "YOUTUBE", "GAME", "HANGMAN" };
        random_word_index = rand() % 5;
        word_len = strlen(word_list[random_word_index]);
        lives = 3;

        strcpy_s(word, word_list[random_word_index]);
        for (unsigned short i = 0; i < word_len; i++) {
            word[i] = '_';
        }

        if (!font.loadFromFile("D:\\KSE\\paradigm\\sfm\\Project1\\Boomboom.otf")) {
            std::cerr << "Error loading font" << std::endl;
            // Handle font loading error if necessary
        }

        text.setFont(font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setPosition(50.f, 50.f);
    }

    void runGame(sf::RenderWindow& window) {
        while (window.isOpen()) {
            handleEvents(window);
            update(window);
            render(window);
        }
    }

private:
    unsigned short random_word_index;
    unsigned short word_len;
    unsigned short lives;
    char word[50];
    bool letter_found;

    const char* word_list[5] = { "AIRPLANE", "PROGRAMMING", "YOUTUBE", "GAME", "HANGMAN" };

    sf::Font font;
    sf::Text text;

    void AddLetter(char letter) {
        letter = static_cast<char>(toupper(static_cast<int>(letter)));
        for (unsigned short i = 0; i < word_len; i++) {
            if (letter == word[i])
                word[i] = letter, letter_found = true;
        }
    }

    bool WordFound() {
        return strcmp(word, word_list[random_word_index]) == 0;
    }

    void handleEvents(sf::RenderWindow& window) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    letter_found = false;
                    char enteredChar = static_cast<char>(event.text.unicode);
                    AddLetter(enteredChar);
                    if (!letter_found) {
                        lives--;
                    }
                    if (WordFound()) {
                        text.setString("You have found the word!\n" + std::string(word));
                        window.draw(text);
                        window.display();
                        sf::sleep(sf::seconds(5));
                        window.close();
                    }
                    if (lives == 0) {
                        text.setString("You ran out of lives.\nThe word was: " + std::string(word_list[random_word_index]));
                        window.draw(text);
                        window.display();
                        sf::sleep(sf::seconds(5));
                        window.close();
                    }
                }
            }
        }
    }

    void update(sf::RenderWindow& window) {
        // Add any additional game logic update if needed
    }

    void render(sf::RenderWindow& window) {
        window.clear();
        text.setString(std::string(word));
        window.draw(text);
        window.display();
    }
};

class Interface {
public:
    Interface() {
        window.create(sf::VideoMode(800, 600), "Hangman Menu");
        window.setFramerateLimit(60);

        if (!backgroundTexture.loadFromFile("D:\\KSE\\paradigm\\Hangman c++\\Hangman\\Hangman\\photo_2023-12-18_09-59-18.jpg")) {
            std::cerr << "Error loading background image" << std::endl;
        }
        background.setTexture(backgroundTexture);
        updateBackgroundScale();

        if (!font.loadFromFile("D:\\KSE\\paradigm\\sfm\\Project1\\Boomboom.otf")) {
            std::cerr << "Error loading font" << std::endl;
        }

        initTextAndButtons();
        updateTitlePosition();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Font font;
    sf::Text playButton;
    sf::Text levelButton;
    sf::Text exitButton;
    sf::Text titleText;

    void initTextAndButtons() {
        playButton.setString("Play");
        levelButton.setString("Choose Level");
        exitButton.setString("Exit");

        playButton.setFont(font);
        levelButton.setFont(font);
        exitButton.setFont(font);

        playButton.setCharacterSize(30);
        levelButton.setCharacterSize(30);
        exitButton.setCharacterSize(30);

        playButton.setPosition(600, 200);
        levelButton.setPosition(600, 250);
        exitButton.setPosition(600, 300);

        
        playButton.setFillColor(sf::Color::Black);
        levelButton.setFillColor(sf::Color::Black);  
        exitButton.setFillColor(sf::Color::Black);  

        titleText.setString("Hangman Game");
        titleText.setFont(font);
        titleText.setCharacterSize(50);
        titleText.setFillColor(sf::Color::Black);
        
        
    
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                break;
            case sf::Event::Resized:
                updateTitlePosition();
                updateBackgroundScale();
                break;
            }
        }
    }

    void handleMouseClick(int mouseX, int mouseY) {
        if (playButton.getGlobalBounds().contains(mouseX, mouseY)) {
            Hangman hangmanGame;
            hangmanGame.runGame(window);
        }
        else if (levelButton.getGlobalBounds().contains(mouseX, mouseY)) {
            // Add logic for level selection
        }
        else if (exitButton.getGlobalBounds().contains(mouseX, mouseY)) {
            window.close();
        }
    }

    void update() {
        float speed = 1.0f; // Задайте швидкість руху тексту

        // Отримайте поточні координати тексту
        sf::Vector2f currentPosition = titleText.getPosition();

        // Змініть x-координату тексту, щоб він рухався вліво
        currentPosition.x -= speed;

        // Якщо текст вийшов за лівий край вікна, перемістіть його назад на правий край
        if (currentPosition.x + titleText.getGlobalBounds().width < 0) {
            currentPosition.x = window.getSize().x;
        }

        // Встановіть нові координати для тексту
        titleText.setPosition(currentPosition);

        // Додайте будь-які інші оновлення, які вам можуть знадобитися
    }


    void render() {
        window.clear();
        window.draw(background);
        window.draw(titleText);
        window.draw(playButton);
        window.draw(levelButton);
        window.draw(exitButton);
        window.display();
    }

    void updateTitlePosition() {
        // Calculate new position of the title relative to the window
        titleText.setPosition((window.getSize().x - titleText.getGlobalBounds().width) / 2, 50.f);
    }

    void updateBackgroundScale() {
        // Scale the background image to fit the window size
        background.setScale(
            static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );
    }
};


int main() {
    Interface interface;
    interface.run();

    return 0;
}
