#include "./Config.h"

namespace data {

const char Config::FONT_FILE[] = "Riffic.ttf";

float Config::DSP_SCALE = 1.0;

const int Config::TOTAL_LEVELS = 626;

const float Config::DEFAULT_TRANSITION_TIME = 0.8f;

const char Config::EFX_BTN_TOUCH[] = "efx_touch.mp3";
const char Config::EFX_WIN[] = "level_complete.mp3";
const char Config::EFX_FAIL[] = "level_failed.mp3";
const char Config::EFX_SCENE_CHANGE[] = "scene_change.mp3";
const char Config::EFX_FOOT[] = "footstep.mp3";

int Config::TILE_SIZE = 64;

const int Config::REWARDED_UNDO_COUNT = 5;

}  // namespace data
