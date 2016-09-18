#ifndef DBR_INPUT_BINDING_HPP
#define DBR_INPUT_BINDING_HPP

#include <type_traits>
#include <functional>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

namespace dbr
{
	namespace input
	{
		template<typename T>
		class Binding
		{
			static constexpr sf::Event::EventType EventRelease = std::is_same<T, sf::Mouse::Button>::value ? sf::Event::MouseButtonReleased : sf::Event::KeyReleased;
			static constexpr sf::Event::EventType EventPress = std::is_same<T, sf::Mouse::Button>::value ? sf::Event::MouseButtonPressed : sf::Event::KeyPressed;
			
			public:
				using Callback = std::function<void(const sf::Time&)>;

				explicit Binding(T in, const Callback& c, bool p = false)
				:	input(in),
					callback(c),
					onPress(p)
				{}

				T getInput() const
				{
					return input;
				}

				bool operator()(const sf::Event& ev) const
				{
					bool released = ev.type == EventRelease;
					bool pressed = ev.type == EventPress;
					bool myInput = (std::is_same<T, sf::Mouse::Button>::value ? ev.mouseButton.button : ev.key.code) == input;

					// early exit if this event isn't our responsiblity
					if(!pressed && !released || !myInput)
						return false;

					sf::Time elapsed = sf::Time::Zero;

					if(onPress)
					{
						if(released)
							return false;
					}
					else
					{
						elapsed = clock.restart();

						if(pressed)
							return false;
					}

					callback(elapsed);

					return true;
				}

			private:
				T input;

				Callback callback;

				// if true, means if key is pressed, if false, means if key is released
				bool onPress;

				// keeps track of the time the key has been/was held for
				mutable sf::Clock clock;
		};
	}
}

#endif
