#include "InputManager.h"
#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1FileSystem.h"
#include "j1Input.h"

InputManager::InputManager(): j1Module()
{
	name.append("input_manager");
}

// Destructor
InputManager::~InputManager()
{}

// Called when before render is available
bool InputManager::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	//Carregar Shortcuts info i guardar a shortcut_list
	inputs_file_path = conf.child("shortcuts_path").attribute("value").as_string();

	return ret;
}

// Call before first frame
bool InputManager::Start()
{
	bool ret = true;

	LoadShortcutsInfo();

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

	while (!input_queue.empty())
		input_queue.pop();

	//TODO: Save inputs inside queue
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for (int i = 0; i < MAX_KEYS; i++)
	{
		if (keys[i] == 1)
		{
			SDL_Scancode state = (SDL_Scancode)i;

			input_queue.push(SDL_GetScancodeName(state));
		}
	}

	return ret;
}

// Called after all Updates
bool InputManager::PostUpdate()
{
	bool ret = true;

	//TODO: Set active = true a shortcuts que coincideixin amb el contingut de la queue
	if (!input_queue.empty())
	{
		//iterar tota la queue i fer la comprovació amb cada shortcut
		//shortcut->command ha d'estar en MAJÚSCULA
		if (input_queue.front() == shortcuts_list.front()->command)
				shortcuts_list.front()->active = true;
	}
	

	return ret;
}

// Called before quitting
bool InputManager::CleanUp()
{
	bool ret = true;

	shortcuts_list.clear();

	return ret;
}

bool InputManager::LoadShortcutsInfo()
{
	bool ret = true;

	//TODO: Load shortcuts info

	pugi::xml_document	shortcuts_file;
	pugi::xml_node		shortcut;

	char* buf;
	int size = App->fs->Load(inputs_file_path.c_str(), &buf);
	pugi::xml_parse_result result = shortcuts_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
	{
		LOG("Could not load xml file %s. PUGI error: &s", inputs_file_path.c_str(), result.description());
		return false;
	}
	else
		shortcut = shortcuts_file.child("inputs_data");

	for (shortcut = shortcut.child("shortcut"); shortcut && ret; shortcut = shortcut.next_sibling("shortcut"))
	{
		ShortCut* short_cut = new ShortCut();

		string type_tmp = shortcut.child("TYPE").attribute("value").as_string();
		if (type_tmp == "UP")
			short_cut->type = UP;
		if (type_tmp == "DOWN")
			short_cut->type = DOWN;
		if (type_tmp == "REPEAT")
			short_cut->type = REPEAT;

		short_cut->name = shortcut.child("name").attribute("value").as_string();
		short_cut->command = shortcut.child("command").attribute("value").as_string();
		short_cut->active = false;

		shortcuts_list.push_back(short_cut);
	}

	return ret;
}