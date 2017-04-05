#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	auto resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename))
	{
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}
	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
	auto resource = std::make_unique<Resource>();
	if (!resource->loadFromFile(filename, secondParam))
	{
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}
	insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource))); // insert returns a pair, where first = iterator and second = bool denoting the success of insertion
	assert(inserted.second); // instead of runtime_error, since this would actually be a logic fault and need to be corrected at debug stage
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());
	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());
	return *found->second;
}

// forward declaration(s) for specific use(s) of template
template class ResourceHolder<sf::Texture, TextureID>;
template class ResourceHolder<sf::Font, FontID>;
template class ResourceHolder<sf::SoundBuffer, SoundEffectID>;

