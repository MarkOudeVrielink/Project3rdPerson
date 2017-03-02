#include <TGUI/TGUI.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/World.hpp"

#include "mgengine\Collision\CollisionManager.h"
#include "mgengine\Core\ControlledActor.h"
#include "mgengine\Core\ObjectActor.h"
#include "mgengine/LevelEditor/LevelEditor.h"
#include "mgengine\Behaviours\PlayerBehaviour.h"
#include "mgengine\Collision\CollisionFilters.h"
using namespace std;
class Menu :public AbstractBehaviour
{
public:
	Menu(World * pWolrd, LevelEditorBehaviour * pLevelEditor);
	void InitializeMenu(tgui::Gui * pGuiRef);
	virtual void update(float pStep);
private:
	void ToLevelEditor();
	void StartGame();
	void HideMenu();
	void Exit();
	void setActive(bool pActive);
	bool getActive();
	bool _active;
	World* _world;
	tgui::Panel::Ptr _panel;
	LevelEditorBehaviour * _levelEditor;
	sf::RenderWindow* _window;

};
