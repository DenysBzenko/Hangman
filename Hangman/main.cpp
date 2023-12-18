#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

const int MAX_TRIES = 6;

class Hangman {
public:
    Hangman(const std::string& word) : secretWord(word), guessedWord(word.length(), '_'), wrongTries(0) {}

    bool guess(char letter) {
        bool found = false;
        for (size_t i = 0; i < secretWord.length(); i++) {
            if (secretWord[i] == letter) {
                guessedWord[i] = letter;
                found = true;
            }
        }
        if (!found) {
            wrongTries++;
        }
        return found;
    }

    bool isGameOver() const {
        return wrongTries >= MAX_TRIES || guessedWord == secretWord;
    }

    bool isWin() const {
        return guessedWord == secretWord;
    }

    int getWrongTries() const {
        return wrongTries;
    }

    const std::string& getGuessedWord() const {
        return guessedWord;
    }

private:
    std::string secretWord;
    std::string guessedWord;
    int wrongTries;
};

class Level {
public:
    enum class Difficulty { Easy, Medium, Hard };
    Level(Difficulty difficulty) : difficulty(difficulty) {}

    Difficulty getDifficulty() const {
        return difficulty;
    }

    

private:
    Difficulty difficulty;
};

class Interface {
public:
    Interface() : window(sf::VideoMode(512, 512), "Hangman"), currentLevel(Level::Difficulty::Easy), hangmanGame(nullptr) {
        if (!initResources()) {
            throw std::runtime_error("Failed to load resources");
        }
        setupUI();
    }

    ~Interface() {
        delete hangmanGame;
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Text playButton, levelButton, exitButton;
    sf::Text easyButton, mediumButton, hardButton;
    sf::Text guessedWordText;
    bool showLevelOptions = false;
    Level currentLevel;
    Hangman* hangmanGame;

    bool initResources() {
        if (!backgroundTexture.loadFromFile("D:\\KSE\\paradigm\\Hangman c++\\Hangman\\Hangman\\photo_2023-12-18_09-59-18.jpg")) {
            return false;
        }
        background.setTexture(backgroundTexture);
        background.setScale(float(window.getSize().x) / backgroundTexture.getSize().x, float(window.getSize().y) / backgroundTexture.getSize().y);

        if (!font.loadFromFile("D:\\KSE\\paradigm\\sfm\\Project1\\Boomboom.otf")) {
            return false;
        }

        return true;
    }

    void setupUI() {
        playButton.setFont(font);
        playButton.setString("Play");
        playButton.setCharacterSize(24);
        playButton.setFillColor(sf::Color::Black);
        playButton.setPosition(400, 50);

        levelButton.setFont(font);
        levelButton.setString("Level");
        levelButton.setCharacterSize(24);
        levelButton.setFillColor(sf::Color::Black);
        levelButton.setPosition(400, 100);

        exitButton.setFont(font);
        exitButton.setString("Exit");
        exitButton.setCharacterSize(24);
        exitButton.setFillColor(sf::Color::Black);
        exitButton.setPosition(400, 150);

        easyButton.setFont(font);
        easyButton.setString("Easy");
        easyButton.setCharacterSize(20);
        easyButton.setFillColor(sf::Color::Green);
        easyButton.setPosition(400, 200);

        mediumButton.setFont(font);
        mediumButton.setString("Medium");
        mediumButton.setCharacterSize(20);
        mediumButton.setFillColor(sf::Color::Yellow);
        mediumButton.setPosition(400, 250);

        hardButton.setFont(font);
        hardButton.setString("Hard");
        hardButton.setCharacterSize(20);
        hardButton.setFillColor(sf::Color::Red);
        hardButton.setPosition(400, 300);

        guessedWordText.setFont(font);
        guessedWordText.setCharacterSize(24);
        guessedWordText.setFillColor(sf::Color::White);
        guessedWordText.setPosition(50, 550);
    }

    void resetGame() {
        delete hangmanGame;
        hangmanGame = new Hangman("EXAMPLE"); 
    }

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseEvents(event.mouseButton);
            }

            if (event.type == sf::Event::TextEntered) {
                if (hangmanGame && event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    hangmanGame->guess(enteredChar);

                    if (hangmanGame->isGameOver()) {
                        
                    }
                }
            }
        }
    }

    void handleMouseEvents(const sf::Event::MouseButtonEvent& mouseEvent) {
        if (showLevelOptions) {
            if (easyButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(mouseEvent.x, mouseEvent.y)))) {
                currentLevel = Level(Level::Difficulty::Easy);
                showLevelOptions = false;
            }
            else if (mediumButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(mouseEvent.x, mouseEvent.y)))) {
                currentLevel = Level(Level::Difficulty::Medium);
                showLevelOptions = false;
            }
            else if (hardButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(mouseEvent.x, mouseEvent.y)))) {
                currentLevel = Level(Level::Difficulty::Hard);
                showLevelOptions = false;
            }
        }
        else {
            if (playButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(mouseEvent.x, mouseEvent.y)))) {
                resetGame();
            }
            else if (levelButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(mouseEvent.x, mouseEvent.y)))) {
                showLevelOptions = true;
            }
            else if (exitButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(mouseEvent.x, mouseEvent.y)))) {
                window.close();
            }
        }
    
    }

    void render() {
        window.clear();
        window.draw(background);

        if (!showLevelOptions) {
            window.draw(playButton);
            window.draw(levelButton);
            window.draw(exitButton);
        }
        else {
            window.draw(easyButton);
            window.draw(mediumButton);
            window.draw(hardButton);
        }

        if (hangmanGame) {
            guessedWordText.setString(hangmanGame->getGuessedWord());
            window.draw(guessedWordText);
        }

        window.display();
    }
};

int main() {
    try {
        Interface interface;
        interface.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

                       