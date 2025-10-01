/*
Interfaces are used to define the behaivior of clases that
implements them, making guidelines that shared with dicoupled
implementation
*/

class IPlayer {
public:
  virtual void play() = 0;
  virtual void pause() = 0;
  virtual void stop() {}; // By not defining them as pure they are optional
  virtual ~IPlayer() {}
};

class MP3 : IPlayer {
public:
  void play() override {};
  void pause() override {};
};

class Video : IPlayer {
public:
  void play() override {};
  void pause() override {};
  void stop() override {};
};
