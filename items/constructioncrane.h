#pragma once

#include <QObject>

#include "abstractitem.h"
#include "external/vector3.h"

class ConstructionCrane : public AbstractItem {
public:
  ConstructionCrane(float weight,
                    float height,
                    Vector3 position,
                    Vector3 angles,
                    std::string name,
                    std::string serial);
  ConstructionCrane(float weight,
                    float height,
                    float ang_x,
                    float ang_y,
                    float ang_z,
                    float pos_x,
                    float pos_y,
                    float pos_z,
                    std::string name,
                    std::string serial);
  ConstructionCrane();
  ~ConstructionCrane() = default;

  std::shared_ptr<AbstractItem> clone() const override;

  std::string print() const override;
  void print(std::ostream &os) const override;

  bool operator==(std::shared_ptr<AbstractItem> const &item) const override;
  bool operator!=(std::shared_ptr<AbstractItem> const &item) const override;

  bool operator==(ConstructionCrane const &vr) const;
  bool operator!=(ConstructionCrane const &vr) const;

  float getHeight() const;
  float getWeight() const;

  Vector3 getAngles() const;
  Vector3 getPosition() const;

  std::string getModelName() const;
  std::string getSerial() const;

  void setHeight(float value);
  void setWeight(float value);

  void setPosition(Vector3 value);
  void setPosition(float x, float y, float z);

  void setAngles(Vector3 value);
  void setAngles(float x, float y, float z);

  void setModelName(std::string value);
  void setSerial(std::string value);

private:
  float weight, height;
  Vector3 angles, position;
  std::string modelName, serial;
};
