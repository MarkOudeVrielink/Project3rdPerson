#ifndef RESOURCEIDENTIFIERS_H
#define RESOURCEIDENTIFIERS_H

namespace sf
{
	class Font;	
	class SoundBuffer;
}

namespace Music {
	enum ID {
		MenuTheme,
		MissionTheme_1,
		MissionTheme_2,
		BossTheme,
		GameOverTheme,
	};
}

namespace SoundEffect {
	enum ID {
		Button,
		Player_Shoot,
		Player_Hit,
		Player_Nova,
		Player_Overheat,
		Drop_Pick,
		Explosion,
		Enemy_Death_1,
		Enemy_Death_2,
		Enemy_Hit
	};
}

namespace Meshes {
	enum ID {
		Player,
		Enemy,
		Bullet,
		BackGround
	};
}

namespace Materials {
	enum ID {
		Player,
		Enemy,
		Bullet,
		BackGround
	};
}

//Forward declaration of the SFMLResourceHolder.
template <typename Resource, typename Identifier>
class SFMLResourceHolder;

typedef SFMLResourceHolder<sf::SoundBuffer, SoundEffect::ID>	SoundBufferHolder;
//typedef SFMLResourceHolder<sf::Font, Fonts::ID>						FontHolder;

#endif // !RESOURCEIDENTIFIERS_H

