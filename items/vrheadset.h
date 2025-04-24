#pragma once

#include "external/vector3.h"
#include "items/abstractitem.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

enum HeadsetType {
  Portable,
  Full
};

class VRHeadset : public AbstractItem {
public:
  VRHeadset(int width, int height, float refreshRate, Vector3 angles, Vector3 position, string modelName);
  VRHeadset(int width, int height, float refreshRate, float angle_x, float angle_y, float angle_z, float x, float y, float z, string modelName);
  VRHeadset();
  ~VRHeadset() = default;

  int getWidth() const;
  int getHeight() const;
  float getRefreshRate() const;
  Vector3 getAngles() const;
  Vector3 getPosition() const;
  string getModelName() const;
  bool isConnected() const;
  HeadsetType getType() const;

  void setRefreshRate(float refR);

  void connect();
  void disconnect();

  void setAngles(float x, float y, float z);
  void setAngles(Vector3 angles);

  void setPosition(float x, float y, float z);
  void setPosition(Vector3 position);

  void setWidth(int value);
  void setHeight(int value);

  void setModelName(string name);

  friend ostream &operator<<(ostream &os, VRHeadset const &vr);

  bool operator==(VRHeadset const &vr) const;
  bool operator!=(VRHeadset const& vr) const;


  static const vector<string> providerSupportedSchemeArgs;
  static const map<string, size_t> providerScheme;
  static const string providerName;

private:

  int width, height;
  float refreshRate;
  bool connected;
  Vector3 angles, position;
  string modelName;
};
