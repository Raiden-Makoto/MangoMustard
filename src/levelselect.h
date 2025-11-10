#pragma once

struct LevelData {
    bool unlocked;
};

int UpdateLevelSelect(int selectedLevel, LevelData levels[], int levelCount);
