#include "EngineCore.h"
#include "TextureAsset.h"
#include "RenderSystem.h"

IMPLEMENT_DYNAMIC_CLASS(TextureAsset);

void TextureAsset::Initialize()
{
	Asset::Initialize();

	SDL_Surface* image = IMG_Load(filepath.c_str());

	if (RenderSystem::Instance().HasRenderer() == true)
	{
		texture = SDL_CreateTextureFromSurface(&RenderSystem::Instance().GetRenderer(), image);
		SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

	}
	printf("%s\n", SDL_GetError()); //Returns "Invalid Renderer".
	SDL_FreeSurface(image);
}

void TextureAsset::Destroy()
{
	Asset::Destroy();
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;

	}
}
