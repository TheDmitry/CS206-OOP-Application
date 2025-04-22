#include "vector3.h"
#include "vrheadset.h"
#include <string>
#include <iostream>
#include <format>

using namespace std;

VRHeadset::VRHeadset(
  int width, int height, float refreshRate, Vector3 angles, Vector3 position, string modelName)
  : width{width}
  , height{height}
  , refreshRate{refreshRate}
  , angles{angles}
  , position{position}
  , modelName{modelName} {}

VRHeadset::VRHeadset(int width,
                     int height,
                     float refreshRate,
                     float angle_x,
                     float angle_y,
                     float angle_z,
                     float x,
                     float y,
                     float z,
                     string modelName)
  : width{width}
  , height{height}
  , refreshRate{refreshRate}
  , angles{angle_x, angle_y, angle_z}
  , position{x, y, z}
  , modelName{modelName} {}

VRHeadset::VRHeadset()
  : width{0}
  , height{0}
  , refreshRate{0}
  , angles{0, 0, 0}
  , position{0, 0, 0}
  , modelName{""} {}

int VRHeadset::getWidth() const { return width; };
int VRHeadset::getHeight() const { return height; };
float VRHeadset::getRefreshRate() const { return refreshRate; }
string VRHeadset::getModelName() const { return modelName; }
Vector3 VRHeadset::getAngles() const { return angles; }
Vector3 VRHeadset::getPosition() const { return position; }
bool VRHeadset::isConnected() const { return connected; }
HeadsetType VRHeadset::getType() const { return HeadsetType::Full; }

void VRHeadset::setRefreshRate(float refR) { refreshRate = refR; }

void VRHeadset::connect() { connected = true; }
void VRHeadset::disconnect() { connected = false; }

void VRHeadset::setAngles(float x, float y, float z) {
  Vector3 vec{x, y, z};

  angles = Vector3{vec.x, vec.y, vec.z};
}
void VRHeadset::setAngles(Vector3 angles) { this->angles = angles; }

void VRHeadset::setPosition(float x, float y, float z) {
  Vector3 vec{x, y, z};

  position = Vector3{vec.x, vec.y, vec.z};
}
void VRHeadset::setPosition(Vector3 position) { this->position = position; }

ostream& operator<<(ostream &os, VRHeadset const &vr) {
  const auto& vga = vr.getAngles();
  const auto& vgp = vr.getPosition();

  os << format("Спецификация модели {}:", vr.getModelName()) << endl;
  os << format("\tУглы вращения: x: {}, y: {}, z: {}", vga.x, vga.y, vga.z) << endl;
  os << format("\tПоложение в пространстве: x: {}, y: {}, z: {}", vgp.x, vgp.y, vgp.z) << endl;
  os << format("\tПодключение: {}", vr.isConnected()) << endl;
  os << format("\tЧастота обновления экрана: {} Гц", vr.getRefreshRate()) << endl;

  return os;
}

bool VRHeadset::operator==(VRHeadset const &vr) const {
  return this->getModelName() == vr.getModelName() &&
         this->getRefreshRate() == vr.getRefreshRate() &&
         this->getWidth() == vr.getWidth() &&
         this->getHeight() == vr.getHeight();
}

bool VRHeadset::operator!=(VRHeadset const &vr) const {
  return this->getModelName() != vr.getModelName() ||
         this->getRefreshRate() != vr.getRefreshRate() ||
         this->getWidth() != vr.getWidth() ||
         this->getHeight() != vr.getHeight();
}
