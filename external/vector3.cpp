#include <cmath>
#include <format>

#include "vector3.h"

using namespace std;

Vector3::Vector3() : x{0}, y{0}, z{0} {}

Vector3::Vector3(const float x, const float y, const float z)
  : x{x}
  , y{y}
  , z{z} {}

void Vector3::debug() {
#ifdef DEBUG
  cout << "[Vector3] x:" << x << " y:" << y << " z:" << z << endl;
#endif
}

Vector3 Vector3::vectorFromString(string const &stringified) {
  size_t prnBegin = stringified.find('(');
  size_t prnEnd = stringified.find(')');

  if (prnBegin == string::npos || prnEnd == string::npos) {
    throw runtime_error(format("Invalid Vector3 definition, given: {}", stringified));
  }

  string qualifier = stringified.substr(0, prnBegin);

  if (qualifier != "v3") {
    throw runtime_error("Invalid qualifier for Vector3. Should be v3(float, float, float)");
  }

  string stringiedValues = stringified.substr(prnBegin + 1, prnEnd - prnBegin - 1);
  float values[3];
  size_t pos = 0;
  size_t i = 0;

  while (pos < stringiedValues.length()) {
    if (i > 2) {
      throw runtime_error("Invalid Vector3. Should be "
                          "v3(float, float, float)");
    }

    size_t comma = stringiedValues.find(',', pos);
    if (comma == string::npos) comma = stringiedValues.length();

    float value = stof(stringiedValues.substr(pos, comma - pos));

    values[i] = value;

    pos = comma + 1;
    i++;
  }

  return Vector3{values[0], values[1], values[2]};
}

string Vector3::vectorToString(Vector3 const &vec) {
  return format("v3({},{},{})", vec.x, vec.y, vec.z);
}

ostream &operator<<(ostream &os, Vector3 const &vec) {
  os << Vector3::vectorToString(vec);

  return os;
}

Vector3 &Vector3::operator+=(Vector3 const &vec) {
  x += vec.x;
  y += vec.y;
  z += vec.z;
  return *this;
}

Vector3 &Vector3::operator-=(Vector3 const &vec) {
  x -= vec.x;
  y -= vec.y;
  z -= vec.z;
  return *this;
}

Vector3 &Vector3::operator/=(Vector3 const &vec) {
  x /= vec.x;
  y /= vec.y;
  z /= vec.z;
  return *this;
}

Vector3 &Vector3::operator*=(Vector3 const &vec) {
  x *= vec.x;
  y *= vec.y;
  z *= vec.z;
  return *this;
}

Vector3 &Vector3::operator+=(const float n) {
  x += n;
  y += n;
  z += n;
  return *this;
}

Vector3 &Vector3::operator-=(const float n) {
  x -= n;
  y -= n;
  z -= n;
  return *this;
}

Vector3 &Vector3::operator/=(const float n) {
  x /= n;
  y /= n;
  z /= n;
  return *this;
}

Vector3 &Vector3::operator*=(const float n) {
  x *= n;
  y *= n;
  z *= n;
  return *this;
}

bool Vector3::operator>(Vector3 const &vec) const {
  return length() > vec.length();
}

bool Vector3::operator<(Vector3 const &vec) const {
  return length() < vec.length();
}

bool Vector3::operator>=(Vector3 const &vec) const {
  return length() >= vec.length();
}

bool Vector3::operator<=(Vector3 const &vec) const {
  return length() <= vec.length();
}

bool Vector3::operator==(Vector3 const &vec) const {
  return x == vec.x && y == vec.y && z == vec.z;
}

double Vector3::distance(Vector3 const &vec) const {
  return sqrt(pow(x - vec.x, 2) + pow(y - vec.y, 2) + pow(z - vec.z, 2));
}

double Vector3::length() const {
  return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

Vector3 operator+(Vector3 vec1, Vector3 const &vec2) {
  return vec1 += vec2;
}

Vector3 operator-(Vector3 vec1, Vector3 const &vec2) {
  return vec1 -= vec2;
}

Vector3 operator+(Vector3 vec, const float n) {
  return vec += n;
}

Vector3 operator+(const float n, Vector3 vec) {
  return vec += n;
}

Vector3 operator-(Vector3 vec, const float n) {
  return vec -= n;
}

Vector3 operator-(const float n, Vector3 vec) {
  return vec -= n;
}

// SCALAR ONLY
Vector3 operator*(Vector3 vec, const float n) {
  return vec *= n;
}

Vector3 operator*(float const &n, Vector3 v) {
  return v *= n;
}

Vector3 operator/(Vector3 v, const float n) {
  return v /= n;
}

Vector3 operator/(float const &n, Vector3 v) {
  return v /= n;
}
