#ifndef MOUSEMANAGER_HPP
#define MOUSEMANAGER_HPP

#include <map>
#include <string>
#include <functional>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

namespace swift
{
	class MouseManager
	{
		public:
			MouseManager() {}
			~MouseManager() {};
			
			sf::Vector2i getPosition(sf::Window* win = nullptr)
			{
				if(win != nullptr)
					return sf::Mouse::getPosition(*win);
				return sf::Mouse::getPosition();
			}
			
			void newBinding(const std::string& n, sf::Mouse::Button b, std::function<void()> f = [](){return true;}, bool onPress = false)
			{
				bindings.emplace(std::make_pair(n, ButtonBinding(b, f, onPress)));
			}

			bool operator()(sf::Event& e)
			{
				for(auto &b : bindings)
				{
					if(b.second(e))
						return b.second(e);
				}
				
				return false;
			}

		private:
			class ButtonBinding
			{
				public:
					explicit ButtonBinding(sf::Mouse::Button b, std::function<void()> f, bool p)
					{
						button = b;
						onPress = p;
						func = f;
					}

					~ButtonBinding()
					{
					}

					sf::Mouse::Button getButton() const
					{
						return button;
					}

					bool operator()(sf::Event& e)
					{
						if(e.mouseButton.button == button)
						{
							if(onPress)
							{
								if(e.type == sf::Event::MouseButtonPressed)
								{
									return this->call();
								}
							}
							else
							{
								if(e.type == sf::Event::MouseButtonReleased)
								{
									return this->call();
								}
							}
						}
						return false;
					}

				private:
					bool call()
					{
						if(!func)
							return false;

						func();

						return true;
					}

					sf::Mouse::Button button;

					std::function<void()> func;

					bool onPress;	// if true, means if key is pressed, if false, means if key is released
			};

			std::map<std::string, ButtonBinding> bindings;

	};
}

#endif // MOUSEMANAGER_HPP
