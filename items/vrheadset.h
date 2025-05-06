#pragma once

#include "external/vector3.h"
#include "items/abstractitem.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

enum HeadsetType {
  Portable,
  Full
};

class VRHeadset : public AbstractItem {
public:
  VRHeadset(int width,
            int height,
            float refreshRate,
            Vector3 angles,
            Vector3 position,
            std::string modelName);
  VRHeadset(int width,
            int height,
            float refreshRate,
            float angle_x,
            float angle_y,
            float angle_z,
            float x,
            float y,
            float z,
            std::string modelName);
  VRHeadset();
  ~VRHeadset() = default;

  std::shared_ptr<AbstractItem> clone() const override;

  int getWidth() const;
  int getHeight() const;
  float getRefreshRate() const;
  Vector3 getAngles() const;
  Vector3 getPosition() const;
  std::string getModelName() const;
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

  void setModelName(std::string name);

  std::string print() const override;
  void print(std::ostream &os) const override;

  friend std::ostream &operator<<(std::ostream &os, std::shared_ptr<AbstractItem> const &item);

  bool operator==(std::shared_ptr<AbstractItem> const &item) const override;
  bool operator!=(std::shared_ptr<AbstractItem> const &item) const override;

  bool operator==(VRHeadset const &vr) const;
  bool operator!=(VRHeadset const &vr) const;

private:
  int width, height;
  float refreshRate;
  bool connected;
  Vector3 angles, position;
  std::string modelName;
};
