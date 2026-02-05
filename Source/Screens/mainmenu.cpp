#include "mainmenu.h"

MainMenu::MainMenu()
{
	std::cout << "Welcome to Pixel Dynasty!\n";
}

MainMenu::~MainMenu()
{
	
}

void MainMenu::Update()
{
	UserInterface();
}

void MainMenu::Render()
{

}

void MainMenu::UserInterface()
{
    Application& app = Application::GetInstance();

    ImGuiIO& io = ImGui::GetIO();

    // Window size
    ImVec2 windowSize(400, 200);

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

    /*
    	This code is so hard codded its ugly but i dont care (for now)
    */
    const char* text = "Pixel Dynasty!";
    float textWidth = ImGui::CalcTextSize(text).x;
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - textWidth) * 0.5f); // Center text horizontally
    ImGui::Text("%s", text);
    textWidth = ImGui::CalcTextSize("Begin").x;
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x-textWidth) * 0.5f);
    ImGui::Button("Begin");
    textWidth = ImGui::CalcTextSize("Exit").x;
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x-textWidth) * 0.5f);
    ImGui::Button("Exit");

    ImGui::End();
}