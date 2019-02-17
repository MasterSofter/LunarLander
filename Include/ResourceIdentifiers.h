#ifndef RESOURCEIDENTIFIERS_H_
#define RESOURCEIDENTIFIERS_H_


// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		LunarModule,
		LunarModule_left,
		LunarModule_right,
		LunarModule_crash,
		TitleScreen,
		MissionScreen,
		Buttons,
		Flame,
		Moon,
		Earth
	};
}

namespace Shaders
{
	enum ID
	{
		BrightnessPass,
		DownSamplePass,
		GaussianBlurPass,
		AddPass,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		Digital
	};
}

namespace SoundEffect
{
	enum ID
	{
		AlliedGunfire,
		EnemyGunfire,
		Explosion,
		Explosion2,
		LaunchMissile,
		CollectPickup,
		Button,
	};
}

namespace Music
{
	enum ID
	{
		MenuTheme,
		MissionTheme,
	};
}


// Forward declaration and a few _type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;

#endif // RESOURCEIDENTIFIERS_H_
