#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	brickGone = false;
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	const int brickCount = 1;

	brick.clear();
	brick.resize(brickCount);

	for (int i = 0; i < brickCount; ++i)
	{
		brick[i].width = 10;
		brick[i].height = 2;
		brick[i].x_position = i * 15;
		brick[i].y_position = 5;
		brick[i].doubleThick = true;
		brick[i].color = ConsoleColor::DarkBlue; // DarkCyan
	}
	
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}



bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < brick.size(); ++i) // brick[0]..brick[1].....brick[4]. >STOPS<
	{
		brick[i].Draw();
	}

	if(brickGone)
	{
		std::string msgWIN = "\nCONGRATULATIONNNNNNNNSSSS WINNNNNERRRRR WINNNERRRR\n\t\tCHICKEN DINNER\n\n\tPress 'R' to restart\n";
		
		int x = (WINDOW_WIDTH / 2) - (msgWIN.length() / 2);
		int y = WINDOW_HEIGHT / 2;

		std::cout << msgWIN;
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < brick.size(); ++i)
	{
		if (brick[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			brick[i].color = ConsoleColor(brick[i].color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (brick[i].color == 0)
			{
				brick.erase(brick.begin() + i);
			}
		}
	}
	

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (brick.size() == 0)
	{
		brickGone = true;
		ball.moving = false;
	}


	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
}
