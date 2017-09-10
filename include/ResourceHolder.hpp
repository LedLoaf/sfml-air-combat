/*

Template resource holder class. Works well with sf::Texture, sf::Font, sf::SoundBuffer. May work with custom resources as well.
This class is not compatible with sf::Music due to the different semantics of the class (e.g. openFromFile() vs. loadFromFile()).
The load() method is overloaded to accomodate sf::Shader, which has two loadFromFile signatures:
	bool loadFromFile(const std::string& filename, sf::Shader::Type type);
	bool loadFromFile(const std::string& vertexShaderFilename, std::string& fragmentShaderFilename);

*/

#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder
{
	public:
		void load(Identifier id, const std::string& filename);
		template <typename Parameter>
		void load(Identifier id, const std::string& filename, const Parameter& secondParam);
		Resource& get(Identifier id);
		const Resource& get(Identifier id) const;

	private:
		void insertResource(Identifier id, std::unique_ptr<Resource> resource);

	private:
		std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#endif // RESOURCEHOLDER_HPP
