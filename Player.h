#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

struct Point
{
  int x;
  int y;
};

enum class Actions
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  INTERACTION
};

struct Player
{
  explicit Player(const std::string& spritepath, Point pos = {.x = 10, .y = 10}) :
                 coords(pos), playerSprite(spritepath) {};

  bool Moved() const;
  void ProcessInput(Actions action, int width, int height, int* tileMap, int& gameOverFlag, int& doorOpened);
  void Draw(Image &screen, Image &backGround);
  void MoveTo(Point newpos);

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  int move_speed = 4;
  Image playerSprite;
};

#endif //MAIN_PLAYER_H
