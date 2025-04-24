#pragma once

#include <string>

using namespace std;

class Vector3 {
 public:
  float x,y,z;

  Vector3();
  Vector3(const float x, const float y, const float z);
  void debug();

  Vector3 &operator+=(Vector3 const &v);
  Vector3 &operator-=(Vector3 const &v);
  Vector3 &operator/=(Vector3 const &v);
  Vector3 &operator*=(Vector3 const &v);

  Vector3 &operator+=(const float n);
  Vector3 &operator-=(const float n);
  Vector3 &operator/=(const float n);
  Vector3 &operator*=(const float n);

  Vector3 operator-() const;
  Vector3 &operator=(Vector3 const &v);

  static Vector3 vectorFromString(const string stringified);
  // DONE: read v3(f,f,f) from *.db, get Vector3(f,f,f)
  static string vectorToString(const Vector3 vector);
  // DONE: write Vector3(f,f,f) to v3(f,f,f) string. Fairly easy to do.

  friend ostream &operator<<(ostream &os, Vector3 const &vec);
};

Vector3 operator+(Vector3 const &v1, Vector3 const &v2);
Vector3 operator-(Vector3 const &v1, Vector3 const &v2);

Vector3 operator+(Vector3 const v, const float n);
Vector3 operator+(const float n, Vector3 const &v);
Vector3 operator-(Vector3 const &v, const float n);
Vector3 operator-(const float n, Vector3 const &v);

Vector3 operator*(Vector3 const &v, const float n);
Vector3 operator*(const float n, Vector3 const &v);
Vector3 operator/(Vector3 const &v, const float n);
Vector3 operator/(const float n, Vector3 const &v);
