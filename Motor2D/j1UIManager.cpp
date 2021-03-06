#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "UILabel.h"
#include "UIImage.h"
#include "UIButton.h"
#include "j1Window.h"
#include "UIInputBox.h"
#include <stdio.h>



j1UIManager::j1UIManager() : j1Module()
{
	name.append("gui");
	debug = false;
}

// Destructor
j1UIManager::~j1UIManager()
{}

// Called before render is available
bool j1UIManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading UIManager atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1UIManager::Start()
{
	bool ret = true;

	atlas = App->tex->Load(atlas_file_name.data());

	return ret;
}

// Update all UIManagers
bool j1UIManager::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1UIManager::PostUpdate()
{
	return true;
}

bool j1UIManager::Update(float dt)
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_UP)
		debug = !debug;

	GetMouseInput();

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		SetNextFocus();
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && pressed_last_frame == true)
		pressed_last_frame = false;

	return ret;
}

// Called before quitting
bool j1UIManager::CleanUp()
{
	LOG("Freeing UIManager");
	bool ret = true;

	gui_elements.clear();

	return ret;
}

void j1UIManager::EraseElement(UIEntity* entity)
{
	gui_elements.remove(entity);
}

// const getter for atlas
SDL_Texture* j1UIManager::GetAtlas() const
{
	return atlas;
}

// class UIManager ---------------------------------------------------

UILabel* j1UIManager::CreateLabel(const char* text, const int x, const int y, j1Module* listener)
{
	UILabel* label = new UILabel(text, x, y);
	label->listener = listener;
	gui_elements.push_back(label);

	return label;
}

UIImage* j1UIManager::CreateImage(SDL_Rect _section, const int x, const int y, j1Module* listener)
{
	UIImage* img = new UIImage(_section, x, y);
	img->listener = listener;
	gui_elements.push_back(img);

	return img;
}

UIButton* j1UIManager::CreateButton(const char* _text, const int x, const int y, SDL_Rect section_idle, SDL_Rect section_pressed, SDL_Rect section_hover, j1Module* listener)
{
	UIButton* button = new UIButton(_text, x, y, section_idle, section_pressed, section_hover);
	button->listener = listener;
	gui_elements.push_back(button);

	return button;
}

UIEntity* j1UIManager::GetMouseHover() const
{
	p2Point<int> mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	list<UIEntity*>::const_reverse_iterator item = gui_elements.rbegin();

	while (item != gui_elements.rend())
	{
		SDL_Rect rect = (*item)->GetScreenRect();
		if (mouse.PointInRect(rect.x, rect.y, rect.w, rect.h) == true)
		{
			return (*item);
		}
		++item;
	}

	return NULL;
}


//Utilities -------------------------------------------------------------------------------------------------------------------------------------
void j1UIManager::GetMouseInput()
{
	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
	{
		gui_pressed = GetMouseHover();
		if (gui_pressed != NULL)
		if (gui_pressed->focusable)
		{
			if (focus)
				focus->isFocus = false;
			focus = gui_pressed;
			gui_pressed->isFocus = true;
		}
	}

	if (gui_pressed)
		gui_pressed->Drag();

	if (App->input->GetMouseButtonDown(1) == KEY_UP)
	{
		gui_pressed = NULL;
	}
}

void j1UIManager::SetNextFocus()
{
	list<UIEntity*>::iterator item = gui_elements.begin();

	unsigned int min_x, min_y;
	App->win->GetWindowSize(min_x, min_y);
	int win_x = min_x;

	int focus_x, focus_y;
	if (focus != NULL)
		focus->GetScreenPos(focus_x, focus_y);
	else
		focus_x = focus_y = 0;

	UIEntity* best_match = NULL;
	while (item != gui_elements.end())
	{
		if ((*item)->focusable && (*item) != focus && (*item)->IsVisible())
		{
			SDL_Rect pos = (*item)->GetScreenRect();
			if (pos.y < min_y && pos.y > focus_y)
			{
				best_match = (*item);
				min_y = pos.y;
				min_x = win_x;
			}
			else
			{
				if (pos.x < min_x && pos.y == focus_y && focus_x < pos.x)
				{
					best_match = (*item);
					min_x = pos.x;
				}
			}
		}
		++item;
	}

	if (best_match != NULL)
	{
		if (focus != NULL)
			focus->isFocus = false;
		focus = best_match;
		focus->isFocus = true;
	}
}