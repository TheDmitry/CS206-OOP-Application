#pragma once

#include <string>

class Vector3 {
 public:
  float x,y,z;

  Vector3();
  Vector3(const float x, const float y, const float z);
  void debug();

  Vector3 &operator+=(Vector3 const &vec);
  Vector3 &operator-=(Vector3 const &vec);
  Vector3 &operator/=(Vector3 const &vec);
  Vector3 &operator*=(Vector3 const &vec);

  Vector3 &operator+=(const float n);
  Vector3 &operator-=(const float n);
  Vector3 &operator/=(const float n);
  Vector3 &operator*=(const float n);

  // Equality
  bool operator>(Vector3 const &vec) const;
  bool operator<(Vector3 const &vec) const;
  bool operator>=(Vector3 const &vec) const;
  bool operator<=(Vector3 const &vec) const;
  bool operator==(Vector3 const &vec) const;

  Vector3 operator-() const;

  double distance(Vector3 const &vec) const;
  double length() const;

  static Vector3 vectorFromString(std::string const &stringified);
  // DONE: read v3(f,f,f) from *.db, get Vector3(f,f,f)
  static std::string vectorToString(Vector3 const &vec);
  // DONE: write Vector3(f,f,f) to v3(f,f,f) string. Fairly easy to do.

  friend std::ostream &operator<<(std::ostream &os, Vector3 const &vec);
};

Vector3 operator+(Vector3 vec1, Vector3 const &vec2);
Vector3 operator-(Vector3 vec1, Vector3 const &vec2);

Vector3 operator+(Vector3 vec, const float n);
Vector3 operator+(const float n, Vector3 vec);
Vector3 operator-(Vector3 vec, const float n);
Vector3 operator-(const float n, Vector3 vec);

Vector3 operator*(Vector3 vec, const float n);
Vector3 operator*(const float n, Vector3 vec);
Vector3 operator/(Vector3 vec, const float n);
Vector3 operator/(const float n, Vector3 vec);
