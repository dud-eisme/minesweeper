#include <raylib.h>
#include <string>
#include <vector>

void bombsIcon(std::vector<bool> &isBomb, std::vector<Rectangle> &boxes) {
  for (int i = 0; i < isBomb.size(); i++) {
    if (isBomb[i]) {
      DrawCircle(boxes[i].x + boxes[i].width / 2,
                 boxes[i].y + boxes[i].height / 2, boxes[i].height / 2, BLACK);
    }
  }
}

void displayNumbers(std::vector<Rectangle> boxes, std::vector<bool> &isBomb,
                    std::vector<int> adjacentBombs) {
  for (int i = 0; i < isBomb.size(); i++) {
    if (!isBomb[i]) {
      if (adjacentBombs[i] > 0) {
        std::string num = std::to_string(adjacentBombs[i]);
        DrawText(num.c_str(), boxes[i].x + boxes[i].width / 2,
                 boxes[i].y + boxes[i].height / 2, 40, BLACK);
      }
    }
  }
}

void floodFill(int idx, std::vector<bool> &isActive,
               std::vector<int> &adjacentBombs, std::vector<bool> &isBomb) {
  if (isBomb[idx])
    return;
  if (idx - 10 >= 0 && isActive[idx - 10] && adjacentBombs[idx - 10] >= 0) {
    isActive[idx - 10] = false;
    floodFill(idx - 10, isActive, adjacentBombs, isBomb);
  }
  if (idx - 1 >= idx - (idx % 10) && isActive[idx - 1] &&
      adjacentBombs[idx - 1] >= 0) {
    isActive[idx - 1] = false;
    floodFill(idx - 1, isActive, adjacentBombs, isBomb);
  }
  if (idx + 1 < idx + 10 - (idx % 10) && isActive[idx + 1] &&
      adjacentBombs[idx + 1] >= 0) {
    isActive[idx + 1] = false;
    floodFill(idx + 1, isActive, adjacentBombs, isBomb);
  }
  if (idx + 10 < isActive.size() && isActive[idx + 10] &&
      adjacentBombs[idx + 10] >= 0) {
    isActive[idx + 10] = false;
    floodFill(idx + 10, isActive, adjacentBombs, isBomb);
  }
}

void overlay(float playArea, float boxS, int numBox,
             std::vector<Rectangle> &boxes, std::vector<bool> &isActive,
             std::vector<bool> &isBomb, float padding, bool &isGameOver,
             std::vector<int> &adjacentBombs, bool &isWin) {

  for (int i = 0; i < numBox; i++) {
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), boxes[i]);
    if (isActive[i])
      DrawRectangleRec(boxes[i], Color(0, 0, 255, 255));
    if (!isGameOver && !isWin) {
      if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        isActive[i] = false;
        if (isBomb[i])
          isGameOver = true;
        floodFill(i, isActive, adjacentBombs, isBomb);
      }
      if (isActive[i] && isHovered)
        DrawRectangleRec(boxes[i], Color(0, 255, 202, 255));
    }
  }
}

bool checkWin(std::vector<bool> &isActive, std::vector<bool> &isBomb) {
  int count = 0;
  int numBombs = 0;
  for (int i = 0; i < isActive.size(); i++) {
    if (!isActive[i])
      count++;
    if (isBomb[i])
      numBombs++;
  }
  return isActive.size() - count == numBombs;
}
