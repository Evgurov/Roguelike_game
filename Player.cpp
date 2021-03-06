#include "Player.h"


bool Player::Moved() const
{
    if(coords.x == old_coords.x && coords.y == old_coords.y)
        return false;
    else
        return true;
}

void Player::MoveTo(Point newpos) {
    coords.x = newpos.x;
    coords.y = newpos.y;
    old_coords.x = newpos.x;
    old_coords.y = newpos.y;
}

void Player::ProcessInput(Actions action, int width, int height, int* tileMap, int& gameOverFlag, int& doorOpened)
{   
    int curTileX = (coords.x + tileSize / 2) / tileSize;
    int curTileY = (coords.y + tileSize / 2) / tileSize;

    int curTileNumber = curTileY * width + curTileX;

    int tileAboveCur = tileMap[curTileNumber - width];
    int tileUnderCur = tileMap[curTileNumber + width];
    int tileOnLeftCur = tileMap[curTileNumber - 1];
    int tileOnRightCur = tileMap[curTileNumber + 1];

    int centerCoordX = coords.x + tileSize/2;
    int centerCoordY = coords.y + tileSize/2;

    if (tileMap[curTileNumber] == 2 || tileMap[curTileNumber] == 6) {
        gameOverFlag = 1;
    }
    else {
        int move_dist = move_speed * 1;
        switch(action) {
            case Actions::INTERACTION:
                if (tileAboveCur == 3) {
                    doorOpened = 1;
                } else if (tileAboveCur == 4) {
                    doorOpened = 2;
                } else if (tileAboveCur == 5) {
                    gameOverFlag = 2;
                }
            break;
            case Actions::UP:
                if (((tileAboveCur != 1) && (tileAboveCur != 3) && (tileAboveCur != 4) && (tileAboveCur != 5)) || (centerCoordY % tileSize > tileSize / 2)) {
                    old_coords.y = coords.y;
                    coords.y -= move_dist;
                }
            break;
            case Actions::DOWN:
                if ((tileUnderCur != 1) || (centerCoordY % tileSize < tileSize / 2)) {
                    old_coords.y = coords.y;
                    coords.y += move_dist;
                }
            break;
            case Actions::LEFT:
                if ((tileOnLeftCur != 1) || (centerCoordX % tileSize > tileSize / 2)) {
                    old_coords.x = coords.x;
                    coords.x -= move_dist;
                }
            break;
            case Actions::RIGHT:
                if ((tileOnRightCur != 1) || (centerCoordX % tileSize < tileSize / 2)) {
                    old_coords.x = coords.x;
                    coords.x += move_dist;
                }
            break;
            default:
            break;
        }
    }
}

void Player::Draw(Image &screen, Image &backGround)
{
    for (int i = 0; i < screen.Width(); i++)
        for (int j = 0; j < screen.Height(); j++)
            screen.PutPixel(i, j, backGround.GetPixel(i, j));

    if(Moved()) {
        for(int y = old_coords.y; y <= old_coords.y + tileSize; ++y) {
            for(int x = old_coords.x; x <= old_coords.x + tileSize; ++x) {
                screen.PutPixel(x, y, backGround.GetPixel(x,y));
            }
        }
        old_coords = coords;
    }

    for(int y = coords.y; y < coords.y + tileSize; ++y) {
        for(int x = coords.x; x < coords.x + tileSize; ++x) {
            screen.AddPixel(x, y, playerSprite.GetPixel(x - coords.x, y - coords.y));
        }
    }
}
