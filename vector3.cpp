#include "vector3.h"
#include <format>

using namespace std;

Vector3::Vector3() : x{0}, y{0}, z{0} {}

Vector3::Vector3(float x, float y, float z) : x{x}, y{y}, z{z} {}

void Vector3::debug() {
  #ifdef DEBUG
  cout << "[Vector3] x:" << x << " y:" << y << " z:" << z << endl;
  #endif
}

Vector3 Vector3::vectorFromString(const string stringified) {
  // v3(f1,f2,f3) -> Vector3{f1, f2, f3} !!!
  // v3 qualifies Vector3
  size_t prnBegin = stringified.find('(');
  size_t prnEnd = stringified.find(')');

  if (prnBegin == string::npos || prnEnd == string::npos) {
    throw invalid_argument(format("Invalid Vector3 definition, given: {}", stringified));
  }

  string qualifier = stringified.substr(0, prnBegin);

  if (qualifier != "v3") {
    throw invalid_argument("Invalid qualifier for Vector3 in db file. Should be v3(float, float, float)");
  }

  string stringiedValues = stringified.substr(prnBegin + 1, prnEnd - prnBegin - 1);
  float values[3];
  size_t pos = 0;
  size_t i = 0;

  while (pos < stringiedValues.length()) {
    if (i > 2) {
      throw invalid_argument("Invalid Vector3 provided in db file. Should be "
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

string Vector3::vectorToString(const Vector3 vector) {
  return format("v3({},{},{})", vector.x, vector.y, vector.z);
}

Vector3 &Vector3::operator+=(Vector3 const &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3 &Vector3::operator-=(Vector3 const &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3 &Vector3::operator/=(Vector3 const &v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

Vector3 &Vector3::operator*=(Vector3 const &v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
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

Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

Vector3 &Vector3::operator=(Vector3 const &v) {
  x = v.x;
  y = v.y;
  z = v.z;

  return *this;
}

Vector3 operator+(Vector3 const &v1, Vector3 const &v2) {
  return (Vector3)v1 += (Vector3)v2;
}

Vector3 operator-(Vector3 const &v1, Vector3 const &v2) {
  return (Vector3)v1 -= (Vector3)v2;
}

Vector3 operator+(Vector3 const v, const float n) {
  return (Vector3)v += n;
}

Vector3 operator+(const float n, Vector3 const &v) {
  return (Vector3)v += n;
}

Vector3 operator-(Vector3 const &v, const float n) {
  return (Vector3)v -= n;
}

Vector3 operator-(const float n, Vector3 const &v) {
  return (Vector3)v -= n;
}

// SCALAR ONLY
Vector3 operator*(Vector3 const &v, const float n) {
  return (Vector3)v *= n;
}

Vector3 operator*(const float n, Vector3 const &v) {
  return (Vector3)v *= n;
}

Vector3 operator/(Vector3 const &v, const float n) {
  return (Vector3)v /= n;
}

Vector3 operator/(const float n, Vector3 const &v) {
  return (Vector3)v /= n;
}
