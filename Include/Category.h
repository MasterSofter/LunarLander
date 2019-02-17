#ifndef CATEGORY_H_
#define CATEGORY_H_


// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None				= 0,
		SceneLunarModule	= 1 << 1,
		Moon				= 1 << 2,
        SoundEffect			= 1 << 3
	};
}

#endif // CATEGORY_H_
