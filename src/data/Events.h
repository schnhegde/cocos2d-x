#ifndef SRC_DATA_EVENTS_H_
#define SRC_DATA_EVENTS_H_

namespace data {
class Events {
 public:
  static const char GAME_OVER[];

  static const char GAME_MOVE_COMPLETED[];
  static const char GAME_UNDO_COMPLETED[];
  static const char GAME_COMPLETED[];
  static const char GAME_UNDO[];
  static const char GAME_RESUMED[];
  static const char REWARDED_VIDEO_COMPLETED[];
  static const char NO_VIDEOS[];
  static const char REWARDED_VIDEO_CANCELLED[];

  static const char ADS_SHOWING[];
  static const char ADS_CLOSED[];
};
}  // namespace data

#endif  // SRC_DATA_EVENTS_H_
