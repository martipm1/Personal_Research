#include "InputManager.h"
#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1FileSystem.h"

InputManager::InputManager()
{
	name.append = ("input_manager");
}

// Destructor
InputManager::~InputManager()
{}

// Called when before render is available
bool InputManager::Awake(pugi::xml_node&)
{
	bool ret = true;

	//Carregar Shortcuts info i guardar a shortcut_list

	return ret;
}

// Call before first frame
bool InputManager::Start()
{
	bool ret = true;

	return ret;
}

// Called before all Updates
bool InputManager::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool InputManager::Update(float dt)
{
	bool ret = true;

	//Guardar inputs a queue;

	return ret;
}

// Called after all Updates
bool InputManager::PostUpdate()
{
	bool ret = true;

	//Set active = true a shortcuts que coincideixin amb el contingut de la queue

	return ret;
}

// Called before quitting
bool InputManager::CleanUp()
{
	bool ret = true;

	shortcuts_list.clear();

	return ret;
}