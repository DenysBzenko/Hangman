#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstring>

class WordList {
public:
    static const char* getWordsForLevel(int level) {
        switch (level) {
        case 1:
            return easyWords[rand() % easyWordCount];
        case 2:
            return mediumWords[rand() % mediumWordCount];
        case 3:
            return hardWords[rand() % hardWordCount];
        default:
            return nullptr;
        }
    }

    static const int easyWordCount = 5;
    static const int mediumWordCount = 5;
    static const int hardWordCount = 5;

private:
    static const char* easyWords[];
    static const char* mediumWords[];
    static const char* hardWords[];
};

const char* WordList::easyWords[] = { "CAT", "DOG", "SUN", "MILK", "FISH" };
const char* WordList::mediumWords[] = { "APPLE", "TIGER", "HORSE", "CHAIR", "PHONE" };
const char* WordList::hardWords[] = { "COMPUTER", "ELEPHANT", "JUPITER", "UNIVERSE", "SYMPHONY" };

class Hangman {
public:
    Hangman(int level) {
        srand(static_cast<unsigned int>(time(nullptr)));
        const char* wordList = WordList::getWordsForLevel(level);  
        if (wordList) {
            word_len = strlen(wordList);
            lives = 7;

            strcpy_s(word, wordList);

            strcpy_s(word, wordList);

            if (!font.loadFromFile("D:\\KSE\\paradigm\\sfm\\Project1\\Boomboom.otf")) {
                std::cerr << "Error loading font" << std::endl;
            }

            text.setFont(font);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            text.setPosition(50.f, 50.f);

            // Initialize the displayed word with underscores
            displayWord = std::string(word_len, '_');
            gameWon = false;
        }
    }

    void runGame(sf::RenderWindow& window) {
        while (window.isOpen()) {
            handleEvents(window);
            update(window);
            render(window);

            if (gameWon) {
                text.setString("Congratulations! You have won!\nPress 'R' to restart.");
                window.draw(text);
                window.display();

                // Handle restart option
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    else if (event.type == sf::Event::TextEntered && event.text.unicode == 'R')
                        Restart();
                }
            }
        }
    }

private:
    unsigned short random_word_index;
    unsigned short word_len;
    unsigned short lives;
    std::string displayWord;
    char word[50];
    bool letter_found;
    bool gameWon;

    sf::Font font;
    sf::Text text;

    void AddLetter(char letter) {
        letter = static_cast<char>(toupper(static_cast<int>(letter)));
        for (unsigned short i = 0; i < word_len; i++) {
            if (letter == word[i]) {
                displayWord[i] = letter;
                letter_found = true;
            }
        }
    }

    bool WordFound() {
        return displayWord.find('_') == std::string::npos;
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
                        sf::sleep(sf::seconds(2));
                        gameWon = true;
                    }
                    if (lives == 0) {
                        text.setString("You ran out of lives.\nThe word was: " + std::string(WordList::getWordsForLevel(1)));
                        window.draw(text);
                        window.display();
                        sf::sleep(sf::seconds(2));
                        window.close();
                    }
                }
            }
        }
    }

    void update(sf::RenderWindow& window) {
        // Any additional game logic can be added here
    }

    void render(sf::RenderWindow& window) {
        window.clear();
        text.setString(displayWord);
        window.draw(text);
        window.display();
    }

    void Restart() {
        lives = 3;
        gameWon = false;
        const char* wordList = WordList::getWordsForLevel(1);
        random_word_index = rand() % WordList::easyWordCount;
        word_len = strlen(wordList);
        strcpy_s(word, wordList);
        displayWord = std::string(word_len, '_');
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
    sf::Text easyButton, mediumButton, hardButton;

    bool levelSelectionActive;
    int selectedLevel;

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

        levelSelectionActive = false;
        selectedLevel = 1;
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
            Hangman hangmanGame(selectedLevel);
            hangmanGame.runGame(window);
        }
        else if (levelButton.getGlobalBounds().contains(mouseX, mouseY)) {
            levelSelectionActive = !levelSelectionActive;
            toggleButtonsVisibility(!levelSelectionActive);
        }
        else if (exitButton.getGlobalBounds().contains(mouseX, mouseY)) {
            window.close();
        }
        else if (levelSelectionActive) {
            handleLevelSelectionClick(mouseX, mouseY);
        }
    }

    void handleLevelSelectionClick(int mouseX, int mouseY) {
        if (easyButton.getGlobalBounds().contains(mouseX, mouseY)) {
            selectedLevel = 1;
            levelSelectionActive = false;
            toggleButtonsVisibility(true);
        }
        else if (mediumButton.getGlobalBounds().contains(mouseX, mouseY)) {
            selectedLevel = 2;
            levelSelectionActive = false;
            toggleButtonsVisibility(true);
        }
        else if (hardButton.getGlobalBounds().contains(mouseX, mouseY)) {
            selectedLevel = 3;
            levelSelectionActive = false;
            toggleButtonsVisibility(true);
        }
    }

    void update() {
        float speed = 1.0f;

        sf::Vector2f currentPosition = titleText.getPosition();

        currentPosition.x -= speed;

        if (currentPosition.x + titleText.getGlobalBounds().width < 0) {
            currentPosition.x = window.getSize().x;
        }

        titleText.setPosition(currentPosition);
    }

    void render() {
        window.clear();
        window.draw(background);
        window.draw(titleText);
        window.draw(playButton);
        window.draw(levelButton);
        window.draw(exitButton);

        if (levelSelectionActive) {
            window.draw(easyButton);
            window.draw(mediumButton);
            window.draw(hardButton);
        }

        window.display();
    }

    void updateTitlePosition() {
        titleText.setPosition((window.getSize().x - titleText.getGlobalBounds().width) / 2, 50.f);
    }

    void updateBackgroundScale() {
        background.setScale(
            static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );
    }

    void toggleButtonsVisibility(bool isVisible) {
        playButton.setFillColor(isVisible ? sf::Color::Black : sf::Color::Transparent);
        levelButton.setFillColor(isVisible ? sf::Color::Black : sf::Color::Transparent);
        exitButton.setFillColor(isVisible ? sf::Color::Black : sf::Color::Transparent);
    }
};

int main() {
    Interface interface;
    interface.run();

    return 0;
}