SwiftInputManager
=================

A C++11 input manager for SFML.

Pretty simple to use, and header-only, so nice and easy to include in a project.

#Example:
```c++
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "InputManager.hpp"

int main(int argc, char** argv)
{
	sf::Window window(sf::VideoMode(800, 600, 32), "SFML");
	
	swift::InputManager<sf::Keyboard::Key> keyboard;
	swift::InputManager<sf::Mouse::Button> mouse;
	
	keyboard.add(sf::Keyboard::Escape, [&window]()
	{
		window.close();
	});
	
	mouse.add(sf::Mouse::Right, [&window]()
	{
		window.close();
	});
	
	while(window.isOpen())
	{
		sf::Event event;

		while(window.pollEvent(event))
		{
			keyboard(event);
			mouse(event);
			
			switch(event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				default:
					break;
			}
		}

		window.clear();
		window.display();
	}
	
	return 0;
}
```

#License
MIT License
