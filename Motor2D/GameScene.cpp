#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "GameScene.h"
#include "j1UIManager.h"
#include "InputManager.h"

GameScene::GameScene() : j1Module()
{
	name.append("scene");
}

// Destructor
GameScene::~GameScene()
{}

// Called before render is available
bool GameScene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool GameScene::Start()
{
	//Load Map
	App->map->Load("game_map.tmx", map_id);

	//Load collision map
	if (App->map->Load("collision.tmx", collider_id) == true)
	{
		int width, height;
		uchar* buffer = NULL;
		App->map->CreateWalkabilityMap(width, height, &buffer, collider_id);

		RELEASE_ARRAY(buffer);
	}


	debug = false;
	game_paused = false;

	//TODO: Crear la UI que suporti l'estructura de Shortcuts
	/*
		- Window:
			-Label + Button/inputbox * ShortCut
			-If Button/inputbox enter -> Append attributes a l'xml
	*/

	return true;
}

// Called each loop iteration
bool GameScene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool GameScene::Update(float dt)
{
	App->map->Draw(map_id);

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;
	if (debug)
		App->map->Draw(collider_id);

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		game_paused = !game_paused;

	//TODO: iterar i comprovar shortcut necessari
	list<ShortCut*>::iterator it = App->input_manager->shortcuts_list.begin();

	while (it != App->input_manager->shortcuts_list.end())
	{
		if ((*it)->name == "camera_right" && (*it)->active)
			App->render->camera.x -= 5;
		if ((*it)->name == "camera_left" && (*it)->active)
			App->render->camera.x += 5;
		if ((*it)->name == "camera_down" && (*it)->active)
			App->render->camera.y -= 5;
		if ((*it)->name == "camera_up" && (*it)->active)
			App->render->camera.y += 5;

		++it;
	}
	

	return true;
}

// Called each loop iteration
bool GameScene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool GameScene::CleanUp()
{
	LOG("Freeing Game Scene");

	return true;
}


bool GameScene::GamePaused()const
{
	return game_paused;
}