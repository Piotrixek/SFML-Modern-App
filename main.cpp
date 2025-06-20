#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

enum class AppState { LOGIN, REGISTER, HOME };
enum class InputFieldFocus { NONE, EMAIL, PASSWORD, CONFIRM_PASSWORD };
bool isMouseOver(const sf::FloatRect &bounds, const sf::Vector2i &mousePos) {
    return bounds.contains(static_cast<sf::Vector2f>(mousePos));
}
std::string registeredEmail = "";
std::string registeredPassword = "";
std::string loggedInUserEmail = "";
int main() {
    const unsigned int WINDOW_WIDTH = 1000;
    const unsigned int WINDOW_HEIGHT = 600;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Login/Register UI (SFML)", sf::Style::Default);
    window.setFramerateLimit(60);
    AppState currentState = AppState::LOGIN;
    InputFieldFocus currentFocus = InputFieldFocus::NONE;
    sf::Color bgColor(245, 245, 245);
    sf::Color leftPanelColor(255, 255, 255);
    sf::Color rightPanelColor(200, 200, 200);
    sf::Color primaryTextColor(0, 0, 0);
    sf::Color secondaryTextColor(100, 100, 100);
    sf::Color inputBgColor(240, 240, 240);
    sf::Color inputBorderColor(220, 220, 220);
    sf::Color inputFocusBorderColor(173, 98, 108);
    sf::Color buttonColor(173, 98, 108);
    sf::Color buttonTextColor(255, 255, 255);
    sf::Color buttonHoverColor(193, 118, 128);
    sf::Color linkColor(0, 100, 200);
    sf::Color linkHoverColor(0, 150, 255);
    sf::Font font;
    const std::string fontFilename = "arial.ttf";
    if (!font.loadFromFile(fontFilename)) {
        std::cerr << "\n--- FATAL ERROR ---\nFailed to load font: " << fontFilename
                  << "\nEnsure it's in the executable directory.\n--------------------\n"
                  << std::endl;
#ifdef _WIN32
        system("pause");
#endif
        return EXIT_FAILURE;
    }
    const float leftPanelRatio = 0.4f;
    const float leftPanelWidth = WINDOW_WIDTH * leftPanelRatio;
    const float rightPanelWidth = WINDOW_WIDTH * (1.0f - leftPanelRatio);
    const float padding = 25.0f;
    const float inputHeight = 40.0f;
    const float inputWidth = leftPanelWidth - 3 * padding;
    const float buttonHeight = 45.0f;
    sf::RectangleShape leftPanel(sf::Vector2f(leftPanelWidth, WINDOW_HEIGHT));
    leftPanel.setFillColor(leftPanelColor);
    sf::RectangleShape rightPanel(sf::Vector2f(rightPanelWidth, WINDOW_HEIGHT));
    rightPanel.setPosition(leftPanelWidth, 0);
    rightPanel.setFillColor(rightPanelColor);
    sf::Texture rightPanelTexture;
    sf::Sprite rightPanelSprite;
    bool textureLoaded = false;
    const std::string textureFilename = "background_image.png";
    if (rightPanelTexture.loadFromFile(textureFilename)) {
        textureLoaded = true;
        rightPanelSprite.setTexture(rightPanelTexture);
        float texWidth = static_cast<float>(rightPanelTexture.getSize().x);
        float texHeight = static_cast<float>(rightPanelTexture.getSize().y);
        float scaleX = rightPanelWidth / texWidth;
        float scaleY = WINDOW_HEIGHT / texHeight;
        float scale = std::max(scaleX, scaleY);
        rightPanelSprite.setScale(scale, scale);
        rightPanelSprite.setPosition(leftPanelWidth, 0);
        rightPanelTexture.setRepeated(false);
        rightPanelSprite.setTextureRect(
            sf::IntRect(0, 0, static_cast<int>(rightPanelWidth / scale), static_cast<int>(WINDOW_HEIGHT / scale)));
    } else {
        std::cerr << "\n--- WARNING ---\nCould not load background texture: " << textureFilename
                  << "\n---------------\n"
                  << std::endl;
    }
    std::string emailInputString;
    std::string passwordInputString;
    std::string confirmPasswordInputString;
    sf::Text loginTitleText("Hello Again!", font, 32);
    loginTitleText.setFillColor(primaryTextColor);
    loginTitleText.setPosition(padding * 1.5f, padding * 2.0f);
    sf::Text loginSubtitleText("Please login to continue", font, 14);
    loginSubtitleText.setFillColor(secondaryTextColor);
    loginSubtitleText.setPosition(
        loginTitleText.getPosition().x,
        loginTitleText.getPosition().y + loginTitleText.getGlobalBounds().height + padding * 0.5f);
    float currentY = loginSubtitleText.getPosition().y + loginSubtitleText.getGlobalBounds().height + padding * 1.5f;
    sf::Text emailLabel("Email", font, 12);
    emailLabel.setFillColor(secondaryTextColor);
    emailLabel.setPosition(loginTitleText.getPosition().x, currentY);
    currentY += emailLabel.getGlobalBounds().height + padding * 0.2f;
    sf::RectangleShape emailInputBg(sf::Vector2f(inputWidth, inputHeight));
    emailInputBg.setFillColor(inputBgColor);
    emailInputBg.setOutlineThickness(1);
    emailInputBg.setOutlineColor(inputBorderColor);
    emailInputBg.setPosition(emailLabel.getPosition().x, currentY);
    sf::Text emailText("", font, 14);
    emailText.setFillColor(primaryTextColor);
    emailText.setPosition(emailInputBg.getPosition().x + padding * 0.5f,
                          emailInputBg.getPosition().y + inputHeight / 2.0f - emailText.getCharacterSize() / 1.5f);
    currentY += inputHeight + padding * 0.5f;
    sf::Text passwordLabel("Password", font, 12);
    passwordLabel.setFillColor(secondaryTextColor);
    passwordLabel.setPosition(emailLabel.getPosition().x, currentY);
    currentY += passwordLabel.getGlobalBounds().height + padding * 0.2f;
    sf::RectangleShape passwordInputBg(sf::Vector2f(inputWidth, inputHeight));
    passwordInputBg.setFillColor(inputBgColor);
    passwordInputBg.setOutlineThickness(1);
    passwordInputBg.setOutlineColor(inputBorderColor);
    passwordInputBg.setPosition(passwordLabel.getPosition().x, currentY);
    sf::Text passwordText("", font, 14);
    passwordText.setFillColor(primaryTextColor);
    passwordText.setPosition(
        passwordInputBg.getPosition().x + padding * 0.5f,
        passwordInputBg.getPosition().y + inputHeight / 2.0f - passwordText.getCharacterSize() / 1.5f);
    currentY += inputHeight + padding * 0.2f;
    sf::Text recoveryText("Recovery Password", font, 11);
    recoveryText.setFillColor(secondaryTextColor);
    recoveryText.setPosition(
        passwordInputBg.getPosition().x + passwordInputBg.getSize().x - recoveryText.getGlobalBounds().width, currentY);
    currentY = recoveryText.getPosition().y + recoveryText.getGlobalBounds().height + padding;
    sf::RectangleShape signInButton(sf::Vector2f(inputWidth, buttonHeight));
    signInButton.setFillColor(buttonColor);
    signInButton.setPosition(passwordInputBg.getPosition().x, currentY);
    sf::Text signInText("Sign In", font, 16);
    signInText.setFillColor(buttonTextColor);
    sf::FloatRect signInBounds = signInText.getLocalBounds();
    signInText.setOrigin(signInBounds.left + signInBounds.width / 2.0f, signInBounds.top + signInBounds.height / 2.0f);
    signInText.setPosition(signInButton.getPosition().x + signInButton.getSize().x / 2.0f,
                           signInButton.getPosition().y + signInButton.getSize().y / 2.0f);
    currentY += buttonHeight + padding * 1.5f;
    sf::Text switchToRegisterText("Need an account? Register here", font, 12);
    switchToRegisterText.setFillColor(linkColor);
    switchToRegisterText.setPosition(leftPanelWidth / 2.0f - switchToRegisterText.getGlobalBounds().width / 2.0f,
                                     currentY);
    sf::Text registerTitleText("Create Account", font, 32);
    registerTitleText.setFillColor(primaryTextColor);
    registerTitleText.setPosition(padding * 1.5f, padding * 2.0f);
    sf::Text registerSubtitleText("Fill in the details below", font, 14);
    registerSubtitleText.setFillColor(secondaryTextColor);
    registerSubtitleText.setPosition(
        registerTitleText.getPosition().x,
        registerTitleText.getPosition().y + registerTitleText.getGlobalBounds().height + padding * 0.5f);
    float confirmPasswordY = passwordInputBg.getPosition().y + passwordInputBg.getSize().y + padding * 0.5f;
    sf::Text confirmPasswordLabel("Confirm Password", font, 12);
    confirmPasswordLabel.setFillColor(secondaryTextColor);
    confirmPasswordLabel.setPosition(emailLabel.getPosition().x, confirmPasswordY);
    confirmPasswordY += confirmPasswordLabel.getGlobalBounds().height + padding * 0.2f;
    sf::RectangleShape confirmPasswordInputBg(sf::Vector2f(inputWidth, inputHeight));
    confirmPasswordInputBg.setFillColor(inputBgColor);
    confirmPasswordInputBg.setOutlineThickness(1);
    confirmPasswordInputBg.setOutlineColor(inputBorderColor);
    confirmPasswordInputBg.setPosition(confirmPasswordLabel.getPosition().x, confirmPasswordY);
    sf::Text confirmPasswordText("", font, 14);
    confirmPasswordText.setFillColor(primaryTextColor);
    confirmPasswordText.setPosition(
        confirmPasswordInputBg.getPosition().x + padding * 0.5f,
        confirmPasswordInputBg.getPosition().y + inputHeight / 2.0f - confirmPasswordText.getCharacterSize() / 1.5f);
    currentY = confirmPasswordInputBg.getPosition().y + confirmPasswordInputBg.getSize().y + padding;
    sf::RectangleShape registerButton(sf::Vector2f(inputWidth, buttonHeight));
    registerButton.setFillColor(buttonColor);
    registerButton.setPosition(confirmPasswordInputBg.getPosition().x, currentY);
    sf::Text registerButtonText("Register", font, 16);
    registerButtonText.setFillColor(buttonTextColor);
    sf::FloatRect registerButtonBounds = registerButtonText.getLocalBounds();
    registerButtonText.setOrigin(registerButtonBounds.left + registerButtonBounds.width / 2.0f,
                                 registerButtonBounds.top + registerButtonBounds.height / 2.0f);
    registerButtonText.setPosition(registerButton.getPosition().x + registerButton.getSize().x / 2.0f,
                                   registerButton.getPosition().y + registerButton.getSize().y / 2.0f);
    currentY += buttonHeight + padding * 1.5f;
    sf::Text switchToLoginText("Already have an account? Login here", font, 12);
    switchToLoginText.setFillColor(linkColor);
    switchToLoginText.setPosition(leftPanelWidth / 2.0f - switchToLoginText.getGlobalBounds().width / 2.0f, currentY);
    sf::Text welcomeText("Welcome!", font, 40);
    welcomeText.setFillColor(primaryTextColor);
    welcomeText.setPosition(WINDOW_WIDTH / 2.0f - welcomeText.getGlobalBounds().width / 2.0f, WINDOW_HEIGHT / 3.0f);
    sf::Text loggedInUserText("", font, 18);
    loggedInUserText.setFillColor(secondaryTextColor);
    sf::RectangleShape logoutButton(sf::Vector2f(150, buttonHeight));
    logoutButton.setFillColor(buttonColor);
    logoutButton.setPosition(WINDOW_WIDTH / 2.0f - logoutButton.getSize().x / 2.0f, WINDOW_HEIGHT * 0.6f);
    sf::Text logoutButtonText("Logout", font, 16);
    logoutButtonText.setFillColor(buttonTextColor);
    sf::FloatRect logoutButtonBounds = logoutButtonText.getLocalBounds();
    logoutButtonText.setOrigin(logoutButtonBounds.left + logoutButtonBounds.width / 2.0f,
                               logoutButtonBounds.top + logoutButtonBounds.height / 2.0f);
    logoutButtonText.setPosition(logoutButton.getPosition().x + logoutButton.getSize().x / 2.0f,
                                 logoutButton.getPosition().y + logoutButton.getSize().y / 2.0f);
    sf::RectangleShape cursor(sf::Vector2f(1.5f, 18.f));
    cursor.setFillColor(primaryTextColor);
    sf::Clock cursorClock;
    bool showCursor = true;
    sf::Text statusText("", font, 12);
    statusText.setFillColor(sf::Color::Red);
    float statusYBase =
        std::max(signInButton.getPosition().y, registerButton.getPosition().y) + buttonHeight + padding * 0.5f;
    statusText.setPosition(leftPanelWidth / 2.0f, statusYBase);
    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        bool mouseClicked = false;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mouseClicked = true;
                    InputFieldFocus newFocus = InputFieldFocus::NONE;
                    if (currentState == AppState::LOGIN || currentState == AppState::REGISTER) {
                        if (isMouseOver(emailInputBg.getGlobalBounds(), mousePos)) {
                            newFocus = InputFieldFocus::EMAIL;
                        } else if (isMouseOver(passwordInputBg.getGlobalBounds(), mousePos)) {
                            newFocus = InputFieldFocus::PASSWORD;
                        } else if (currentState == AppState::REGISTER &&
                                   isMouseOver(confirmPasswordInputBg.getGlobalBounds(), mousePos)) {
                            newFocus = InputFieldFocus::CONFIRM_PASSWORD;
                        }
                    }
                    currentFocus = newFocus;
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (currentFocus != InputFieldFocus::NONE) {
                    if (event.text.unicode == '\b') {
                        std::string *targetString = nullptr;
                        if (currentFocus == InputFieldFocus::EMAIL)
                            targetString = &emailInputString;
                        else if (currentFocus == InputFieldFocus::PASSWORD)
                            targetString = &passwordInputString;
                        else if (currentFocus == InputFieldFocus::CONFIRM_PASSWORD)
                            targetString = &confirmPasswordInputString;
                        if (targetString && !targetString->empty()) {
                            targetString->pop_back();
                        }
                    } else if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                        char enteredChar = static_cast<char>(event.text.unicode);
                        if (currentFocus == InputFieldFocus::EMAIL) {
                            emailInputString += enteredChar;
                        } else if (currentFocus == InputFieldFocus::PASSWORD) {
                            passwordInputString += enteredChar;
                        } else if (currentFocus == InputFieldFocus::CONFIRM_PASSWORD) {
                            confirmPasswordInputString += enteredChar;
                        }
                    }
                    emailText.setString(emailInputString);
                    passwordText.setString(std::string(passwordInputString.length(), '*'));
                    confirmPasswordText.setString(std::string(confirmPasswordInputString.length(), '*'));
                    showCursor = true;
                    cursorClock.restart();
                }
            }
        }
        std::string currentStatus = "";
        if (mouseClicked) {
            if (currentState == AppState::LOGIN) {
                if (isMouseOver(signInButton.getGlobalBounds(), mousePos)) {
                    std::cout << "Attempting login with Email: " << emailInputString << " Pass: " << passwordInputString
                              << std::endl;
                    if (emailInputString.empty() || passwordInputString.empty()) {
                        currentStatus = "Email and Password cannot be empty.";
                    } else if (emailInputString == registeredEmail && passwordInputString == registeredPassword) {
                        std::cout << "Login successful!" << std::endl;
                        currentState = AppState::HOME;
                        loggedInUserEmail = emailInputString;
                        loggedInUserText.setString("Logged in as: " + loggedInUserEmail);
                        loggedInUserText.setPosition(
                            WINDOW_WIDTH / 2.0f - loggedInUserText.getGlobalBounds().width / 2.0f,
                            welcomeText.getPosition().y + welcomeText.getGlobalBounds().height + padding);
                        emailInputString = "";
                        passwordInputString = "";
                        confirmPasswordInputString = "";
                        emailText.setString("");
                        passwordText.setString("");
                        confirmPasswordText.setString("");
                        currentFocus = InputFieldFocus::NONE;
                    } else {
                        std::cout << "Login failed!" << std::endl;
                        currentStatus = "Invalid email or password.";
                    }
                } else if (isMouseOver(switchToRegisterText.getGlobalBounds(), mousePos)) {
                    currentState = AppState::REGISTER;
                    emailInputString = "";
                    passwordInputString = "";
                    confirmPasswordInputString = "";
                    emailText.setString("");
                    passwordText.setString("");
                    confirmPasswordText.setString("");
                    currentFocus = InputFieldFocus::NONE;
                    currentStatus = "";
                }
            } else if (currentState == AppState::REGISTER) {
                if (isMouseOver(registerButton.getGlobalBounds(), mousePos)) {
                    std::cout << "Attempting registration..." << std::endl;
                    if (emailInputString.empty() || passwordInputString.empty() || confirmPasswordInputString.empty()) {
                        currentStatus = "All fields must be filled.";
                    } else if (passwordInputString != confirmPasswordInputString) {
                        currentStatus = "Passwords do not match.";
                    } else if (!registeredEmail.empty() && emailInputString == registeredEmail) {
                        currentStatus = "Email already registered.";
                    } else {
                        registeredEmail = emailInputString;
                        registeredPassword = passwordInputString;
                        std::cout << "Registration successful for: " << registeredEmail << std::endl;
                        currentStatus = "Registration successful! Please login.";
                        currentState = AppState::LOGIN;
                        emailInputString = "";
                        passwordInputString = "";
                        confirmPasswordInputString = "";
                        emailText.setString("");
                        passwordText.setString("");
                        confirmPasswordText.setString("");
                        currentFocus = InputFieldFocus::NONE;
                    }
                } else if (isMouseOver(switchToLoginText.getGlobalBounds(), mousePos)) {
                    currentState = AppState::LOGIN;
                    emailInputString = "";
                    passwordInputString = "";
                    confirmPasswordInputString = "";
                    emailText.setString("");
                    passwordText.setString("");
                    confirmPasswordText.setString("");
                    currentFocus = InputFieldFocus::NONE;
                    currentStatus = "";
                }
            } else if (currentState == AppState::HOME) {
                if (isMouseOver(logoutButton.getGlobalBounds(), mousePos)) {
                    currentState = AppState::LOGIN;
                    loggedInUserEmail = "";
                    currentFocus = InputFieldFocus::NONE;
                    currentStatus = "";
                }
            }
        }
        statusText.setString(currentStatus);
        statusText.setOrigin(statusText.getLocalBounds().width / 2.0f, 0);
        statusText.setPosition(leftPanelWidth / 2.0f, statusYBase);
        emailInputBg.setOutlineColor(currentFocus == InputFieldFocus::EMAIL ? inputFocusBorderColor : inputBorderColor);
        passwordInputBg.setOutlineColor(currentFocus == InputFieldFocus::PASSWORD ? inputFocusBorderColor
                                                                                  : inputBorderColor);
        confirmPasswordInputBg.setOutlineColor(currentFocus == InputFieldFocus::CONFIRM_PASSWORD ? inputFocusBorderColor
                                                                                                 : inputBorderColor);
        emailInputBg.setOutlineThickness(currentFocus == InputFieldFocus::EMAIL ? 2 : 1);
        passwordInputBg.setOutlineThickness(currentFocus == InputFieldFocus::PASSWORD ? 2 : 1);
        confirmPasswordInputBg.setOutlineThickness(currentFocus == InputFieldFocus::CONFIRM_PASSWORD ? 2 : 1);
        signInButton.setFillColor(isMouseOver(signInButton.getGlobalBounds(), mousePos) ? buttonHoverColor
                                                                                        : buttonColor);
        registerButton.setFillColor(isMouseOver(registerButton.getGlobalBounds(), mousePos) ? buttonHoverColor
                                                                                            : buttonColor);
        logoutButton.setFillColor(isMouseOver(logoutButton.getGlobalBounds(), mousePos) ? buttonHoverColor
                                                                                        : buttonColor);
        switchToRegisterText.setFillColor(isMouseOver(switchToRegisterText.getGlobalBounds(), mousePos) ? linkHoverColor
                                                                                                        : linkColor);
        switchToLoginText.setFillColor(isMouseOver(switchToLoginText.getGlobalBounds(), mousePos) ? linkHoverColor
                                                                                                  : linkColor);
        if (currentFocus != InputFieldFocus::NONE) {
            if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
                showCursor = !showCursor;
                cursorClock.restart();
            }
            sf::Text *activeText = nullptr;
            if (currentFocus == InputFieldFocus::EMAIL)
                activeText = &emailText;
            else if (currentFocus == InputFieldFocus::PASSWORD)
                activeText = &passwordText;
            else if (currentFocus == InputFieldFocus::CONFIRM_PASSWORD)
                activeText = &confirmPasswordText;
            if (activeText) {
                cursor.setPosition(activeText->findCharacterPos(activeText->getString().getSize()));
            }
        } else {
            showCursor = false;
        }
        window.clear(bgColor);
        switch (currentState) {
            case AppState::LOGIN:
                window.draw(leftPanel);
                if (textureLoaded)
                    window.draw(rightPanelSprite);
                else
                    window.draw(rightPanel);
                window.draw(loginTitleText);
                window.draw(loginSubtitleText);
                window.draw(emailLabel);
                window.draw(emailInputBg);
                window.draw(emailText);
                window.draw(passwordLabel);
                window.draw(passwordInputBg);
                window.draw(passwordText);
                window.draw(recoveryText);
                window.draw(signInButton);
                window.draw(signInText);
                window.draw(switchToRegisterText);
                window.draw(statusText);
                break;
            case AppState::REGISTER:
                window.draw(leftPanel);
                if (textureLoaded)
                    window.draw(rightPanelSprite);
                else
                    window.draw(rightPanel);
                window.draw(registerTitleText);
                window.draw(registerSubtitleText);
                window.draw(emailLabel);
                window.draw(emailInputBg);
                window.draw(emailText);
                window.draw(passwordLabel);
                window.draw(passwordInputBg);
                window.draw(passwordText);
                window.draw(confirmPasswordLabel);
                window.draw(confirmPasswordInputBg);
                window.draw(confirmPasswordText);
                window.draw(registerButton);
                window.draw(registerButtonText);
                window.draw(switchToLoginText);
                window.draw(statusText);
                break;
            case AppState::HOME:
                window.draw(welcomeText);
                window.draw(loggedInUserText);
                window.draw(logoutButton);
                window.draw(logoutButtonText);
                break;
        }
        if (showCursor && currentFocus != InputFieldFocus::NONE) {
            window.draw(cursor);
        }
        window.display();
    }
    return EXIT_SUCCESS;
}
