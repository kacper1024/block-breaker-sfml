#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
using namespace std;

// SZABLON KONWERSJI DOWOLNEGO TYPU NA TYP STRING
template<typename T>
std::string to_string(const T& obj)
{
	std::stringstream ss;
	ss << obj;
	return ss.str();
}

// EKRAN STARTOWY
void GameStart(sf::RenderWindow &window)
{
	window.clear(sf::Color::Blue);
	sf::Text text;
	sf::Font font;
	font.loadFromFile("sansation.ttf");
	text.setFont(font);
	text.setString(L"Brick Breaker\n\n\n\n\n\n\nSpacja = Start gry\n\nEsc = Wyjście z gry");
	text.setPosition(300, 100);
	window.draw(text);
	window.display();
}

// EKRAN PRZEGRANEJ
void GameOver(sf::RenderWindow& window)
{
	window.clear(sf::Color::Red);
	sf::Text text;
	sf::Font font;
	font.loadFromFile("sansation.ttf");
	text.setFont(font);
	text.setString(L"Przegrałeś!\nWciśnij Spację, aby kontynuować!\nWciśnij Esc, aby wyjść z gry");
	text.setPosition(200, 300);
	window.draw(text);
	window.display();
}

// EKRAN WYGRANEJ
void YouWin(sf::RenderWindow& window)
{
	window.clear(sf::Color::Green);
	sf::Text text;
	sf::Font font;
	font.loadFromFile("sansation.ttf");
	text.setFont(font);
	text.setString(L"Wygrałeś!\nWciśnij Spację, aby grać od nowa!\nWciśnij Esc, aby wyjść z gry");
	text.setPosition(200, 300);
	window.draw(text);
	window.display();
}

// SZABLON KOLIZJI POMIĘDZY DWOMA OBIEKTAMI
template <class T1, class T2> bool isIntersecting(T1& a, T2& b)
{
	return a.right() >= b.left() && a.left() <= b.right() && a.bottom() >= b.top() && a.top() <= b.bottom();
}

// WYKRYWANIE KOLIZJI POMIĘDZY PALETKĄ A PIŁECZKĄ
bool collisionTest(Paddle& paddle, Ball& ball)
{
	if (!isIntersecting(paddle, ball))
		return false;

	ball.moveUp();

	if (ball.getPosition().x < paddle.getPosition().x)
	{
		ball.moveLeft();
	}

	else if (ball.getPosition().x > paddle.getPosition().x)
	{
		ball.moveRight();
	}

	return true;
}

// WYKRYWANIE KOLIZJI POMIĘDZY PIŁECZKĄ A KAFELKAMI
bool collisionTest(Block& block, Ball& ball)
{
	if (!isIntersecting(block, ball))
		return false;

	block.destroy();

	float overlapLeft{ ball.right() - block.left() };
	float overlapRight{ block.right() - ball.left() };
	float overlapTop{ ball.bottom() - block.top() };
	float overlapBottom{ block.bottom() - ball.top() };

	bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
	bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

	float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
	float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

	if (abs(minOverlapX) < abs(minOverlapY))
	{
		ballFromLeft ? ball.moveLeft() : ball.moveRight();
	}
	else
	{
		ballFromTop ? ball.moveUp() : ball.moveDown();
	}

	return true;
}

// PROGRAM GŁÓWNY GRY ******************************************
int main()
{
	// IMPLEMENTACJA ZEGARA GRY
	sf::Clock* clock = nullptr;

	// TŁO GRY
	sf::Texture background;
	if (!background.loadFromFile("background.jpg"))
	{
		exit(1);
	}
	sf::Sprite r;
	r.setTexture(background);

	// CZCIONKA
	sf::Font font1;
	font1.loadFromFile("sansation.ttf");

	// PUNKTACJA
	unsigned int score = 0;

	// STRUMIEŃ WYJŚCIA TEKSTU
	ostringstream sScore;
	sScore << "Punkty: ";

	// PARAMETRY TEKSTU PUNKTACJI
	sf::Text game_score;
	game_score.setCharacterSize(30);
	game_score.setPosition({ 0,0 });
	game_score.setFont(font1);
	game_score.setString(sScore.str());

	// FLAGI WYGRANEJ I PRZEGRANEJ
	bool game_over = false;
	bool you_win = false;

	// PARAMETRY LICZBY PUNKTÓW
	sf::Text game_score_nr;
	game_score_nr.setCharacterSize(30);
	game_score_nr.setPosition({ 105,0 });
	game_score_nr.setFont(font1);
	game_score_nr.setString(sScore.str());

	// TWORZENIE KLAS DŹWIĘKOWYCH
	sf::Sound a;
	sf::SoundBuffer buffer;
	sf::Sound b;
	sf::SoundBuffer buffer2;
	sf::Sound c;
	sf::SoundBuffer buffer3;
	sf::Sound d;
	sf::SoundBuffer buffer4;
	a.setVolume(100);

	// WCZYTYWANIE EFEKTÓW DŹWIĘKOWYCH GRY
	buffer.loadFromFile("ball.wav");
	a.setBuffer(buffer);
	buffer2.loadFromFile("hit.wav");
	b.setBuffer(buffer2);
	buffer3.loadFromFile("you_lose.wav");
	c.setBuffer(buffer3);
	buffer4.loadFromFile("you_win.wav");
	d.setBuffer(buffer4);

	// TWORZENIE PIŁECZKI
	Ball ball(400, 540);

	// TWORZENIE PALETKI
	Paddle paddle(400, 580);

	// TWORZENIE OKNA GRY I LIMITU KLATEK NA SEKUNDĘ
	sf::RenderWindow oknoAplikacji(sf::VideoMode(800, 600), "Brick Breaker");
	oknoAplikacji.setFramerateLimit(60);

	// TWORZENIE ZDARZENIA
	sf::Event event;

	// TWORZENIE KAFELEK
	unsigned blocksX{ 10 }, blocksY{ 5 }, blockWidth{ 63 }, blockHeight{ 20 };
	vector<Block> blocks;
	for (unsigned int i = 0; i < blocksY; i++)
	{
		for (unsigned int j = 0; j < blocksX; j++)
		{
			blocks.emplace_back((j + 1) * (blockWidth + 10), (i + 2) * (blockHeight + 5), blockWidth, blockHeight);
		}
	}

	// WYWOŁANIE EKRANU STARTOWEGO
	while (true)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) break;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) return 0;
		else GameStart(oknoAplikacji);
	}

	// MUZYKA
	sf::Music m;
	m.openFromFile("music.ogg");
	//m.play();
	//m.setLoop(1);
	//m.setVolume(10);

	// PĘTLA GŁÓWNA GRY
	while (true)
	{

		if (!game_over && !you_win)
		{
			// CZYSZCZENIE EKRANU
			oknoAplikacji.clear(sf::Color::Black);
			oknoAplikacji.pollEvent(event);

			// PRZEGRANA
			if (ball.shape.getPosition().y > 585)
			{
				c.play();
				clock = new sf::Clock;
				game_over = true;
			}

			// WYGRANA
			if (blocks.empty())
			{
				d.play();
				clock = new sf::Clock;
				you_win = true;
			}

			// WYJŚCIE Z GRY
			if (event.type == sf::Event::Closed)
			{
				oknoAplikacji.close();
				break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				oknoAplikacji.close();
				break;
			}

			// TEST EKRANU WYGRANEJ
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				blocks.clear();
			}

			// RUCH PIŁECZKI
			ball.update();

			// RUCH PALETKI
			paddle.update();

			// WYKRYWANIE KOLIZJI POMIĘDZY PALETKĄ A PIŁECZKĄ
			if (collisionTest(paddle, ball))
				a.play();

			// WYJŚCIE Z GRY
			if (event.type == sf::Event::Closed)
			{
				oknoAplikacji.close();
				break;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				oknoAplikacji.close();
				break;
			}

			// WYKRYWANIE KOLIZJI POMIĘDZY PIŁECZKĄ A KAFELKAMI
			for (auto& block : blocks)
				if (collisionTest(block, ball))
				{
					score += 10;
					b.play();
					break;
				}

			// PROCES NISZCZENIA KAFELEK I USUWANIA ICH Z PLANSZY
			auto iterator = remove_if(begin(blocks), end(blocks), [](Block& block) {return block.isDestroyed(); });
			blocks.erase(iterator, end(blocks));

			// RYSOWANIE NA EKRANIE GRY PIŁECZKI I PALETKI
			oknoAplikacji.draw(r);
			oknoAplikacji.draw(ball);
			oknoAplikacji.draw(paddle);

			// RYSOWANIE NA EKRANIE GRY BLOKU KAFELEK
			for (auto& block : blocks)
				oknoAplikacji.draw(block);

			// RYSOWANIE NAPISU PUNKTACJI ORAZ PRZEMIANA TYPU CAŁKOWITEGO ZMIENNEJ NA TYP STRING
			game_score_nr.setString(to_string(score));
			oknoAplikacji.draw(game_score);
			oknoAplikacji.draw(game_score_nr);

			// WYŚWIETLANIE EKRANU GRY
			oknoAplikacji.display();
		}

		// WARUNEK PRZEGRANEJ
		else if (game_over)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				break;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				game_over = 0;
				score = 0;
				ball.setPosition(400, 560);
				paddle.setPosition(400, 580);
				blocks.clear();
				for (unsigned int i = 0; i < blocksY; i++)
				{
					for (unsigned int j = 0; j < blocksX; j++)
					{
						blocks.emplace_back((j + 1) * (blockWidth + 10), (i + 2) * (blockHeight + 5), blockWidth, blockHeight);
					}
				}
			}
			else
				GameOver(oknoAplikacji);
		}

		// WARUNEK WYGRANEJ
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				break;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				you_win = 0;
				score = 0;
				ball.setPosition(400, 560);
				paddle.setPosition(400, 580);
				blocks.clear();
				for (unsigned int i = 0; i < blocksY; i++)
				{
					for (unsigned int j = 0; j < blocksX; j++)
					{
						blocks.emplace_back((j + 1) * (blockWidth + 10), (i + 2) * (blockHeight + 5), blockWidth, blockHeight);
					}
				}
			}
			else
				YouWin(oknoAplikacji);
		}
	}

	// USUWANIE ZEGARA I ZATRZYMANIE MUZYKI
	m.stop();
	delete clock;

	return 0;
}
