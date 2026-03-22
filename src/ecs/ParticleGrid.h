//
// Created by taylo on 3/18/2026.
//

#ifndef INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
#define INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
#include <cstdlib>
#include <vector>
#include <SDL3/SDL_render.h>

enum class CellType {
    Empty,
    Sand,
    Stone
};

struct Cell {
    CellType type = CellType::Empty;
};

class ParticleGrid {
public:
    ParticleGrid(int windowWidth, int windowHeight, int cellSize) : cellSize(cellSize) {
        width = windowWidth / cellSize;
        height = windowHeight / cellSize;
        cells.resize(height * width);
    };

    bool inBounds(int x, int y) {
        return x >= 0 && x < width && y >= 0 && y < height;
    };

    Cell &at(int x, int y) {
        return cells[y * width + x];
    };

    bool isEmpty(int x, int y) {
        return inBounds(x, y) && cells[y * width + x].type == CellType::Empty;
    };

    void swapCells(int x1, int y1, int x2, int y2) {
        std::swap(at(x1, y1), at(x2, y2));
    }

    void update() {
        for (int y = height - 1; y >= 0; y--) {
            for (int x = 0; x < width; x++) {
                {
                    if (at(x, y).type == CellType::Sand) {
                        updateParticle(x, y);
                    }
                }
            }
        }
    }

    void updateParticle(int x, int y) {
        //check directly below
        if (isEmpty(x, y + 1)) {
            swapCells(x, y, x, y + 1);
            return;
        }

        //randomize checking left or right first to add some variance to particle movements
        bool leftFirst = rand() % 2 == 0;

        if (leftFirst) {
            if (isEmpty(x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
            if (isEmpty(x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
        } else {
            if (isEmpty(x + 1, y + 1)) {
                swapCells(x + 1, y + 1, x, y);
                return;
            }
            if (isEmpty(x - 1, y + 1)) {
                swapCells(x - 1, y + 1, x, y);
                return;
            }
        }
    }

    void spawnParticleAtMouse(int mouseX, int mouseY) {
        int gx = mouseX / cellSize;
        int gy = mouseY / cellSize;

        if (inBounds(gx, gy)) {
            at(gx, gy).type = CellType::Sand;
        }
    }

    void render(SDL_Renderer *renderer, float camOffx = 0.0f, float camOffy = 0.0f) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Cell &cell = at(x, y);

                if (cell.type == CellType::Empty) continue;

                SDL_FRect rect;
                rect.x = x * cellSize - camOffx;
                rect.y = y * cellSize - camOffy;
                rect.w = (float) cellSize;
                rect.h = (float) cellSize;

                switch (cell.type) {
                    case CellType::Sand:
                        SDL_SetRenderDrawColor(renderer, 220, 190, 80, 255);
                        break;
                    case CellType::Stone:
                        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                        break;
                    case CellType::Empty:
                    default:
                        continue;
                }

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

private:
    int width;
    int height;
    int cellSize;
    std::vector<Cell> cells;
};

#endif //INC_8051TUTORIAL_PARTICLEGRIDSYSTEM_H
