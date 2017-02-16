#ifndef COLLISIONCONFIG_H
#define COLLISIONCONFIG_H
namespace CF{

#define BIT(x) (1<<(x))

	enum collisiontypes {
		COL_NOTHING		= 0,
		COL_PLAYER		= BIT(0),
		COL_ENEMY		= BIT(1),
		COL_BULLET		= BIT(2),
		COL_PICKUP		= BIT(3)
	};

	const short playerCollidesWith	= COL_BULLET | COL_ENEMY | COL_PICKUP;
	const short enemyCollidesWith	= COL_BULLET | COL_PLAYER;
	const short bulletCollidesWith	= COL_PLAYER | COL_ENEMY;
	const short pickupCollidesWith	= COL_PLAYER;
}

#endif // !COLLISIONCONFIG_H

