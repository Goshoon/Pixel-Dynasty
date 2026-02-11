#include "mainmenu.h"

MainMenu::MainMenu()
{
    std::cout << "Welcome to Pixel Dynasty!\n";
    Mix_HaltMusic(); // Stop any music from previous screen
    background = Application::GetInstance().GetTexture("main_menu_background");
    song = Application::GetInstance().GetSound("song_main_menu");
    Mix_PlayMusic(song, 0);
}

MainMenu::~MainMenu()
{
    Mix_HaltMusic();
}

void MainMenu::Update()
{}

void MainMenu::Render()
{
    Application::GetInstance().RenderImage(background,  0, 0);
    UserInterface();
}

void MainMenu::UserInterface()
{
    Application& app = Application::GetInstance();
    ImGuiIO& io = ImGui::GetIO();

    // Window size
    ImVec2 windowSize(225, 160);

    // Center window on screen
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(
        ImVec2(
            (io.DisplaySize.x - windowSize.x) * 0.5f,
            (io.DisplaySize.y - windowSize.y) * 0.5f
        ),
        ImGuiCond_Always
    );

    ImGui::Begin("Welcome to Pixel Dynasty",
        nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar
    );

    ImVec2 actualWindowSize = ImGui::GetWindowSize();
    float windowWidth = actualWindowSize.x;
    float windowHeight = actualWindowSize.y;

    // Calculate content height
    float contentHeight = 60.0f + 30.0f + 40.0f; // title + 2 buttons
    float topPadding = (windowHeight - contentHeight) * 0.5f;

    // Add vertical spacing to center
    ImGui::Dummy(ImVec2(0, topPadding));

    // Title with larger font
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::SetWindowFontScale(1.5f);
    const char* text = "Pixel Dynasty!";
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPosX((windowWidth - textSize.x) * 0.5f);
    ImGui::Text("%s", text);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopFont();

    // Add spacing between title and buttons
    ImGui::Spacing();
    ImGui::Spacing();

    // Buttons
    ImVec2 buttonSize(150, 40);

    // Begin Button
    ImGui::SetCursorPosX((windowWidth - buttonSize.x) * 0.5f);
    if (ImGui::Button("Begin", buttonSize)) nextScene = true;

    ImGui::Spacing();

    // Exit Button
    ImGui::SetCursorPosX((windowWidth - buttonSize.x) * 0.5f);
    if (ImGui::Button("Exit", buttonSize)) app.done = true;

    ImGui::End();
}