#include "header.hpp"
#include <raylib.h>

int screenHeight = 900;
int screenWidth = 1000;

float playArea = screenHeight * 0.8;

int main() {
  int dimension = 10;
  float boxS = (float)playArea / dimension;
  float padding = boxS * 0.05;
  std::vector<bool> isActive(dimension * dimension, true);

  InitWindow(screenWidth, screenHeight, "MineSweeper");
  SetTargetFPS(60);

  initGame(playArea, boxS, dimension * dimension, padding);
  std::vector<bool> isBomb = setBombs(playArea, padding, boxes, isActive);
  std::vector<int> adjacentBombs = numbersCalc(isBomb, dimension);
  float playLeft = (float)(GetScreenWidth() - playArea) / 2,
        playTop = (float)(GetScreenHeight() - playArea) / 2;

  Rectangle play = {playLeft, playTop, (float)playArea, (float)playArea};
  Rectangle playBorder = {playLeft - padding, playTop - padding,
                          playArea + 2 * (float)padding,
                          playArea + 2 * (float)padding};
  Rectangle exit = {screenWidth * 0.94f, screenHeight * 0.01f, 50.0, 50.0};

  bool isGameOver = false;
  bool isWin = false;
  while (!WindowShouldClose()) {
    bool isMouseOnExit = CheckCollisionPointRec(GetMousePosition(), exit);
    BeginDrawing();

    ClearBackground(Color(28, 188, 248, 255));
    DrawRectangleRec(exit, isMouseOnExit ? RED : GRAY);
    DrawText("X", screenWidth * 0.94f + 6, screenHeight * 0.01f - 1, 60, BLACK);
    if (isMouseOnExit && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
      break;
    DrawRectangleRec(play, BROWN);
    DrawRectangleLinesEx(playBorder, 5, Color(37, 98, 23, 255));

    bombsIcon(isBomb, boxes);
    displayNumbers(boxes, isBomb, adjacentBombs);
    overlay(playArea, boxS, dimension * dimension, boxes, isActive, isBomb,
            padding, isGameOver, adjacentBombs, isWin);
    if (isGameOver) {
      Rectangle retryB = {(float)(screenWidth - 700) / 2, screenHeight * 0.91f,
                          700, 50};
      bool isOnRetry = CheckCollisionPointRec(GetMousePosition(), retryB);
      DrawText("GAME OVER",
               (float)(GetScreenWidth() - MeasureText("GAME OVER", 150)) / 2,
               (float)GetScreenHeight() / 2 - 50, 150, RED);

      DrawRectangleRec(retryB, isOnRetry ? GREEN : RED);
      DrawText("RETRY", (float)(screenWidth - MeasureText("RETRY", 40)) / 2,
               screenHeight * 0.92f, 40, BLACK);
      if (isOnRetry && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isGameOver = false;
        boxes.clear();
        items.clear();
        adjacentBombs.clear();
        isActive.assign(isActive.size(), true);
        initGame(playArea, boxS, dimension * dimension, padding);
        isBomb = setBombs(playArea, padding, boxes, isActive);
        adjacentBombs = numbersCalc(isBomb, dimension);
      }
    }

    if (checkWin(isActive, isBomb)) {
      isWin = true;
      Rectangle replayB = {(float)(screenWidth - 700) / 2, screenHeight * 0.91f,
                          700, 50};
      bool isOnReplay = CheckCollisionPointRec(GetMousePosition(), replayB);
      DrawText("YOU WON",
               (float)(GetScreenWidth() - MeasureText("YOU WON", 150)) / 2,
               (float)GetScreenHeight() / 2 - 50, 150, GREEN);

      DrawRectangleRec(replayB, isOnReplay ? GREEN : BLUE);
      DrawText("REPLAY", (float)(screenWidth - MeasureText("REPLAY", 40)) / 2,
               screenHeight * 0.92f, 40, BLACK);
      if (isOnReplay && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isGameOver = false;
        isWin = false;
        boxes.clear();
        items.clear();
        adjacentBombs.clear();
        isActive.assign(isActive.size(), true);
        initGame(playArea, boxS, dimension * dimension, padding);
        isBomb = setBombs(playArea, padding, boxes, isActive);
        adjacentBombs = numbersCalc(isBomb, dimension);
      }
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
