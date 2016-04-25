#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "j1Module.h"
#include "p2Log.h"
#include "j1App.h"
#include "PugiXml\src\pugixml.hpp"
#include <queue>

enum INPUT_TYPE
{
	DOWN,
	UP,
	REPEAT
};

struct ShortCut
{
	ShortCut()
	{}

	ShortCut(INPUT_TYPE _type, string _name, string _command)
	{
		type = _type;
		name = _name;
		command = _command;
	}

	INPUT_TYPE	 type;
	bool		 active;
	string		 name;
	string		 command;
};

class InputManager: public j1Module
{
public:

	InputManager();

	// Destructor
	virtual ~InputManager();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadShortcutsInfo();

public:
	list<ShortCut*>			shortcuts_list;
	//queue, guarda inputs
	queue<const char*>		input_queue;

	string					inputs_file_path;
};

#endif // __INPUT_MANAGER_H__