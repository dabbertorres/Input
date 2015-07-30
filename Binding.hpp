#ifndef SWIFT_BINDING_HPP
#define SWIFT_BINDING_HPP

#include <type_traits>
#include <functional>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

namespace swift
{
	template<typename T>
	class Binding
	{
#if !defined _MSC_VER || _MSC_VER >= 1900
		constexpr sf::Event::EventType EventRelease = std::is_same<T, sf::Mouse::Button>::value ? sf::Event::MouseButtonReleased : sf::Event::KeyReleased;
		constexpr sf::Event::EventType EventPress = std::is_same<T, sf::Mouse::Button>::value ? sf::Event::MouseButtonPressed : sf::Event::KeyPressed;
#else
		const sf::Event::EventType EventRelease = std::is_same<T, sf::Mouse::Button>::value ? sf::Event::MouseButtonReleased : sf::Event::KeyReleased;
		const sf::Event::EventType EventPress = std::is_same<T, sf::Mouse::Button>::value ? sf::Event::MouseButtonPressed : sf::Event::KeyPressed;
#endif
		public:
			using Callback = std::function<void(const sf::Time&)>;

			explicit Binding(T i, const Callback& c, bool p = false)
			:	input(i),
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

#endif
