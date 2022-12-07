#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Pickup.h"
#include "Coin.h"
#include "Bomb.h"
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

int main()
{
	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Start with the GAME_OVER state
	State state = State::GAME_OVER;

	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	float X = VideoMode::getDesktopMode().width;
	float Y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),"Snake Game", Style::Fullscreen);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	bool paused = true;

	Text messageText;
    // We need to choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    // Set the font to our message
    messageText.setFont(font);
    // Assign the actual message
    messageText.setString("GAME INSTRUCTION: \n1. Collect coins to increase your size and HP to prepare for the attack. \n2. Avoid bombs and survive until time runs out. \nClick ENTER to start.");
    // Make it really big 
    messageText.setCharacterSize(30);

    //Choose a color
    messageText.setFillColor(Color::White);

    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top +
        textRect.height / 2.0f);
    messageText.setPosition(resolution.x / 2.0f, resolution.y / 2.0f);

	// Here is our clock for timing everything
	Clock clock;

	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Create an instance of the Player class
	Snake player;

	// The boundaries of the arena
	IntRect arena;

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/grass.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Create a couple of pickups
	//Pickup healthPickup(1);
	//Pickup ammoPickup(2);

	// Prepare the coin
	Texture textureCoin;
	textureCoin.loadFromFile("graphics/coin_all.png");
	VertexArray coinSpin;
    coinSpin.setPrimitiveType(Quads);
    coinSpin.resize(5);

    const int COIN_SHEET_WIDTH = 160;
    const float FRAME_TIME_S = 0.2f;

    Vector2f coinPosition = { resolution.x / 2, resolution.y / 2 };

    coinSpin[0].position = coinPosition + Vector2f(0, 0);
    coinSpin[1].position = coinPosition + Vector2f(COIN_SHEET_WIDTH, 0);
    coinSpin[2].position = coinPosition + Vector2f(COIN_SHEET_WIDTH, COIN_SHEET_WIDTH);
    coinSpin[3].position = coinPosition + Vector2f(0, COIN_SHEET_WIDTH);
	coinSpin[4].position = coinPosition + Vector2f(0,0);

    int frame = 0;
	Time animate_time;

    Time dt;


	// Prepare the bomb
	Texture textureBomb;
	textureBomb.loadFromFile("graphics/bomb_all.png");
	VertexArray bombExpl;
    bombExpl.setPrimitiveType(Quads);
    bombExpl.resize(9);

	const int BOMB_SHEET_WIDTH = 200;

	bombExpl[0].position = coinPosition + Vector2f(0, 0);
    bombExpl[1].position = coinPosition + Vector2f(BOMB_SHEET_WIDTH, 0);
    bombExpl[2].position = coinPosition + Vector2f(BOMB_SHEET_WIDTH, BOMB_SHEET_WIDTH);
    bombExpl[3].position = coinPosition + Vector2f(0, BOMB_SHEET_WIDTH);
	bombExpl[4].position = coinPosition + Vector2f(0,0);
	bombExpl[5].position = coinPosition + Vector2f(0, 0);
    bombExpl[6].position = coinPosition + Vector2f(BOMB_SHEET_WIDTH, 0);
    bombExpl[7].position = coinPosition + Vector2f(BOMB_SHEET_WIDTH, BOMB_SHEET_WIDTH);
    bombExpl[8].position = coinPosition + Vector2f(0, BOMB_SHEET_WIDTH);

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/bomb.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/coin.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);

	// We will add a ball in the next chapter
	/*
	Coin ball(100, 100);
	bool m_Spawned = true;
	if (m_Spawned)
	{
		Coin (0, 0);
	}
	*/
	// The main game loop
	while (window.isOpen())
	{
		if (paused)
		{
			// Draw our message
			window.draw(messageText);
			window.display();
		}
		dt = clock.restart(); 
		animate_time += dt;
		if (animate_time >= seconds(FRAME_TIME_S))
			{
				frame++;
				frame %= 5;
				animate_time = Time::Zero;
			}
			// Set the texture coordinates of each vertex
			int frameOffset = COIN_SHEET_WIDTH * frame;

			coinSpin[0].texCoords = Vector2f(0, 0 + frameOffset);
			coinSpin[1].texCoords = Vector2f(COIN_SHEET_WIDTH, 0 + frameOffset);
			coinSpin[2].texCoords = Vector2f(COIN_SHEET_WIDTH, COIN_SHEET_WIDTH + frameOffset);
			coinSpin[3].texCoords = Vector2f(0, COIN_SHEET_WIDTH + frameOffset);
			coinSpin[4].position = coinPosition + Vector2f(0, 0 + frameOffset);
		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				paused = false;
				// Pause a game while playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING)
				{
					
				}

			}
		}


		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				player.moveUp();
				//hit.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				player.moveDown();
				//hit.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				player.moveLeft();
				//hit.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				player.moveRight();
				//hit.play();
			}

		}// End WASD while playing

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Enter)
			{
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				// Prepare thelevel
				// We will modify the next two lines later
				//arena.width = 1870;
				//arena.height = 1030;
				//arena.left = 50;
				//arena.top = 50;

				arena.width = X - 50;
				arena.height = Y - 50;
				arena.left = 50;
				arena.top = 50;

				// We will modify this line of code later
				int tileSize = 50;

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				// Reset the clock so there isn't a frame jump
				clock.restart();


			}
		}// End levelling up

		/*
		****************
		UPDATE THE FRAME
		****************
		*/
		if (state == State::PLAYING)
		{
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			//ball.update(dt);
			/*
			// Has the ball hit the bat?
			if (spriteCoin.getPosition().intersects(player.getPosition()))
			{
				// Hit detected so reverse the ball and score a point
				ball.reboundBatOrTop();
				splat.play();
				m_Spawned = false;
			}
			*/

		}// End updating the scene

		/*
		**************
		Draw the scene
		**************
		*/

		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);
			// Draw the background
			window.draw(spriteBackground);
			// Draw the player
			window.draw(player.getSprite());
			window.draw(coinSpin, &textureCoin);
			window.draw(bombExpl, &textureBomb);
			//window.draw(ball.getShape());

		}

		if (state == State::LEVELING_UP)
		{
		}

		if (state == State::PAUSED)
		{
		}

		if (state == State::GAME_OVER)
		{
		}

		window.display();

	}// End game loop

	return 0;
}