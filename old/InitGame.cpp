#include <random>
#include <raylib.h>
#include <vector>

struct item {
  float x_coord;
  float y_coord;
  char type;
};

std::vector<struct item> items;
std::vector<Rectangle> boxes;

void initGame(float playArea, int boxS, int numBox, float padding) {
  float playLeft = (float)(GetScreenWidth() - playArea) / 2,
        playTop = (float)(GetScreenHeight() - playArea) / 2;

  Rectangle play = {playLeft, playTop, (float)playArea, (float)playArea};
  Rectangle playBorder = {playLeft - padding, playTop - padding,
                          playArea + 2 * (float)padding,
                          playArea + 2 * (float)padding};

  float startLeft = playLeft, startTop = playTop;
  for (int i = 0; i < numBox; i++) {
    if (startLeft == playLeft + playArea) {
      startLeft = playLeft;
      startTop += boxS;
    }
    boxes.push_back({startLeft + padding, startTop + padding,
                     boxS - 2 * (float)padding, boxS - 2 * (float)padding});
    startLeft += boxS;
  }

  for (int i = 0; i < numBox; i++) {
    items.push_back({boxes[i].x + boxes[i].width / 2,
                     boxes[i].y + boxes[i].height / 2, 'b'});
  }
}

std::vector<bool> setBombs(float playArea, float padding,
                           std::vector<Rectangle> &boxes,
                           std::vector<bool> &isActive) {
  float playLeft = (float)(GetScreenWidth() - playArea) / 2,
        playTop = (float)(GetScreenHeight() - playArea) / 2;

  float startLeft = playLeft, startTop = playTop;

  std::vector<bool> isBomb(boxes.size(), false);

  for (int i = 0; i < boxes.size(); i++) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    if (dis(gen) < 0.3)
      isBomb[i] = true;
  }

  return isBomb;
}

std::vector<int> numbersCalc(std::vector<bool> &isBomb, int dimension) {
  std::vector<int> adjacentBombs(boxes.size());
  for (int i = 0; i < isBomb.size(); i++) {
    if (isBomb[i]) {
      if (i - 10 >= 0) {
        if (i - 10 - 1 >= i - 10 - (i % 10))
          adjacentBombs[i - 10 - 1]++;
        adjacentBombs[i - 10]++;
        if (i - 10 + 1 < i - (i % 10))
          adjacentBombs[i - 10 + 1]++;
      }
      if (i - 1 >= i - (i % 10))
        adjacentBombs[i - 1]++;
      if (i + 1 < i + 10 - (i % 10))
        adjacentBombs[i + 1]++;
      if (i + 10 < boxes.size()) {
        if (i + 10 - 1 >= i + 10 - (i % 10))
          adjacentBombs[i + 10 - 1]++;
        adjacentBombs[i + 10]++;
        if (i + 10 + 1 < i + 20 - (i % 10))
          adjacentBombs[i + 10 + 1]++;
      }
    }
  }
  for (int i = 0; i < adjacentBombs.size(); i++)
    if (isBomb[i])
      adjacentBombs[i] = -1;
  return adjacentBombs;
}
