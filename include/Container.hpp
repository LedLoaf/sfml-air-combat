#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Component.hpp"

#include <vector>
#include <memory>

namespace GUI
{

class Container : public Component
{
	public:
						Container();

		void			pack(std::shared_ptr<Component> component);

		virtual bool	isSelectable() const;
		virtual void	handleEvent(const sf::Event& event);

	private:
		virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool			hasSelection() const;
		void			select(std::size_t index);
		void			selectNext();
		void			selectPrevious();

	private:
		std::vector<std::shared_ptr<Component>>	mChildren;
		int										mSelectedChild;
};

}

#endif // CONTAINER_HPP

