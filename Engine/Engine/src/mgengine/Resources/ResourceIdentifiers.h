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
		ButtonSelect,
		Ambient,
		Player_Ship,
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
		Yogurt,
		Sushi,
		Sandwich,
		Potato,
		Pizza,
		Muffin,
		Boss,		
		BulletYogurt,
		BulletSushi,
		BulletSandwich,
		BulletPotato,
		BulletPizza,
		BulletMuffin,
		BulletBoss,
		Bullet,
		BackGround,
		Planet,
		Meteor,
		MeteorTrail,
		PickUp,
		AirFreshener,
		Apple,
		Fish,
		Drink,
		Tv,
		Banana,
		Explosion,
		PlayerTrail
	};
}

namespace Materials {
	enum ID {
		Player,
		Enemy,
		Yogurt,
		Sushi,
		Sandwich,
		Potato,
		Pizza,
		Muffin,
		Boss,
		BulletYogurt,
		BulletSushi,
		BulletSandwich,
		BulletPotato,
		BulletPizza,
		BulletMuffin,
		BulletBoss,
		Bullet,
		BackGround_0,
		BackGround_1,
		BackGround_2,
		BackGround_3,
		Planet,
		Meteor,
		MeteorTrail,
		PickUp,
		AirFreshener,
		Apple,
		Fish,
		Drink,
		Tv,
		Banana,
		Explosion
	};
}

//Forward declaration of the SFMLResourceHolder.
template <typename Resource, typename Identifier>
class SFMLResourceHolder;

typedef SFMLResourceHolder<sf::SoundBuffer, SoundEffect::ID>	SoundBufferHolder;
//typedef SFMLResourceHolder<sf::Font, Fonts::ID>						FontHolder;

#endif // !RESOURCEIDENTIFIERS_H

