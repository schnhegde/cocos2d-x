#ifndef SRC_DATA_CONFIG_H_
#define SRC_DATA_CONFIG_H_

namespace data {

class Config {
 public:
  static const char FONT_FILE[];

  static float DSP_SCALE;

  static const int REWARDED_UNDO_COUNT;

  static const int TOTAL_LEVELS;

  static const float DEFAULT_TRANSITION_TIME;

  static const char EFX_BTN_TOUCH[];
  static const char EFX_WIN[];
  static const char EFX_FAIL[];
  static const char EFX_FOOT[];
  static const char EFX_SCENE_CHANGE[];

  static int TILE_SIZE;
};

}  // namespace data

#endif  // SRC_DATA_CONFIG_H_
