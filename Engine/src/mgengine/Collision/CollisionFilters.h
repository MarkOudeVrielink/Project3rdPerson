#ifndef COLLISIONCONFIG_H
#define COLLISIONCONFIG_H
namespace CF{

#define BIT(x) (1<<(x))

	enum collisiontypes {
		COL_NOTHING			= 0,
		COL_PLAYER			= BIT(0),
		COL_ENEMY			= BIT(1),
		COL_PLAYERBULLET	= BIT(2),
		COL_PLAYERNOVA		= BIT(3),
		COL_ENEMYBULLET		= BIT(4),
		COL_PICKUP			= BIT(5)
	};

	const short enemyCollidesWith		= COL_PLAYERBULLET | COL_PLAYER | COL_PLAYERNOVA;
	const short playerCollidesWith		= COL_ENEMYBULLET | COL_ENEMY | COL_PICKUP;
	const short playerBulletCollidesWith= COL_ENEMY;
	const short enemyBulletCollidesWith = COL_PLAYER;
	const short playerNovaCollidesWith	= COL_ENEMY;
	const short pickupCollidesWith		= COL_PLAYER;
}

#endif // !COLLISIONCONFIG_H

