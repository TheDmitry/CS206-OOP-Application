#include <format>
#include <iostream>

#include "external/vector3.h"
#include "vrheadset.h"

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

std::shared_ptr<AbstractItem> VRHeadset::clone() const {
  return make_shared<VRHeadset>(*this);
}

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

void VRHeadset::setWidth(int value) {
  width = value;
};

void VRHeadset::setHeight(int value) {
  height = value;
};

void VRHeadset::setModelName(string name) {
  modelName = name;
};

std::string VRHeadset::print() const {
  auto out = ostringstream{};
  print(out);
  return out.str();
}

void VRHeadset::print(std::ostream &os) const {
  const auto &vga = getAngles();
  const auto &vgp = getPosition();

  os << QObject::tr("Specification of").toStdString() << format(" {}:", getModelName()) << endl;
  os << "\t" << QObject::tr("Angles").toStdString()
     << format(": x: {}, y: {}, z: {}", vga.x, vga.y, vga.z) << endl;
  os << "\t" << QObject::tr("Position").toStdString()
     << format(": x: {}, y: {}, z: {}", vgp.x, vgp.y, vgp.z) << endl;
  os << "\t" << QObject::tr("Connection").toStdString() << format(": {}", isConnected()) << endl;
  os << "\t" << QObject::tr("Refresh Rate").toStdString() << format(": {} Hz", getRefreshRate())
     << endl;
}

ostream &operator<<(ostream &os, VRHeadset const &vr) {
  vr.print(os);

  return os;
}

bool VRHeadset::operator==(VRHeadset const &vr) const {
  return getModelName() == vr.getModelName() && getRefreshRate() == vr.getRefreshRate()
         && getWidth() == vr.getWidth() && getHeight() == vr.getHeight();
}

bool VRHeadset::operator!=(VRHeadset const &vr) const {
  return !(*this == vr);
}

bool VRHeadset::operator==(shared_ptr<AbstractItem> const &item) const {
  auto const &vr = dynamic_pointer_cast<VRHeadset>(item);
  return vr && *this == *vr;
}

bool VRHeadset::operator!=(shared_ptr<AbstractItem> const &item) const {
  return !(*this == item);
}
