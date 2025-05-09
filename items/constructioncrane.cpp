#include "constructioncrane.h"

#include <QObject>

using namespace std;

ConstructionCrane::ConstructionCrane(
  float weight, float height, Vector3 position, Vector3 angles, std::string name, std::string serial)
  : weight{weight}
  , height{height}
  , angles{angles}
  , position{position}
  , modelName{name}
  , serial{serial} {}
ConstructionCrane::ConstructionCrane(float weight,
                                     float height,
                                     float ang_x,
                                     float ang_y,
                                     float ang_z,
                                     float pos_x,
                                     float pos_y,
                                     float pos_z,
                                     std::string name,
                                     std::string serial)
  : weight{weight}
  , height{height}
  , angles{ang_x, ang_y, ang_z}
  , position{pos_x, pos_y, pos_z}
  , modelName{name}
  , serial{serial} {}
ConstructionCrane::ConstructionCrane()
  : weight{0.0f}
  , height{0.0f}
  , angles{0, 0, 0}
  , position{0, 0, 0}
  , modelName{""}
  , serial{""} {}

std::shared_ptr<AbstractItem> ConstructionCrane::clone() const {
  return make_shared<ConstructionCrane>(*this);
}

std::string ConstructionCrane::print() const {
  auto out = ostringstream{};
  print(out);
  return out.str();
}

void ConstructionCrane::print(std::ostream &os) const {
  const auto &vga = getAngles();
  const auto &vgp = getPosition();

  os << QObject::tr("Specification of").toStdString() << format(" {}:", getModelName()) << endl;
  os << "\t" << QObject::tr("Angles").toStdString()
     << format(": x: {}, y: {}, z: {}", vga.x, vga.y, vga.z) << endl;
  os << "\t" << QObject::tr("Position").toStdString()
     << format(": x: {}, y: {}, z: {}", vgp.x, vgp.y, vgp.z) << endl;
  os << "\t" << QObject::tr("Height").toStdString() << format(": {}", getHeight()) << endl;
  os << "\t" << QObject::tr("Weight").toStdString() << format(": {} Hz", getWeight()) << endl;
  os << "\t" << QObject::tr("Serial").toStdString() << format(": {} Hz", getSerial()) << endl;
}

bool ConstructionCrane::operator==(ConstructionCrane const &vr) const {
  return getModelName() == vr.getModelName() && getSerial() == vr.getSerial()
         && getWeight() == vr.getWeight() && getHeight() == vr.getHeight();
}

bool ConstructionCrane::operator!=(ConstructionCrane const &vr) const {
  return !(*this == vr);
}

bool ConstructionCrane::operator==(shared_ptr<AbstractItem> const &item) const {
  auto const &vr = dynamic_pointer_cast<ConstructionCrane>(item);
  return vr && *this == *vr;
}

bool ConstructionCrane::operator!=(shared_ptr<AbstractItem> const &item) const {
  return !(*this == item);
}

float ConstructionCrane::getHeight() const {
  return height;
}
float ConstructionCrane::getWeight() const {
  return weight;
}

Vector3 ConstructionCrane::getAngles() const {
  return angles;
}

Vector3 ConstructionCrane::getPosition() const {
  return position;
}

std::string ConstructionCrane::getModelName() const {
  return modelName;
}
std::string ConstructionCrane::getSerial() const {
  return serial;
}

void ConstructionCrane::setHeight(float value) {
  height = value;
}

void ConstructionCrane::setWeight(float value) {
  weight = value;
}

void ConstructionCrane::setPosition(Vector3 value) {
  position = value;
}

void ConstructionCrane::setPosition(float x, float y, float z) {
  position = Vector3{x, y, z};
}

void ConstructionCrane::setAngles(Vector3 value) {
  angles = value;
}
void ConstructionCrane::setAngles(float x, float y, float z) {
  angles = Vector3{x, y, z};
}

void ConstructionCrane::setModelName(std::string value) {
  modelName = value;
}
void ConstructionCrane::setSerial(std::string value) {
  serial = value;
}
