#pragma once
#include "screen.h"
#include <vector>
#include <memory>
#include <iostream>
#include <string>

class MainMenu : public Screen
{
public:
	MainMenu();
  ~MainMenu();

	void Update() override;
	void Render() override;
private:
  void UserInterface();
};
