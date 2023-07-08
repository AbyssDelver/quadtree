#ifndef POINT_HPP
#define POINT_HPP

#include <cmath>

class Point {
  double m_x{};
  double m_y{};

 public:
  Point(double x = 0, double y = 0) : m_x{x}, m_y{y} {};
  Point(const Point& a) : m_x{a.m_x}, m_y{a.m_y} {};
  // aggiungere distruttore

  // returns m_x, m_y
  double x() const { return m_x; }
  double y() const { return m_y; }

  // distance of Point from origin
  double distance() const { return std::sqrt(m_x * m_x + m_y * m_y); }

  // implementation of rotation trasoformation.
  // parameter assumed to be in radians
  void rotate(double angle) {
    double temp{};
    temp = m_x * std::cos(angle) - m_y * std::sin(angle);
    m_y = m_x * std::sin(angle) + m_y * std::cos(angle);
    m_x = temp;
  }

  Point& operator=(const Point& a) {
    m_x = a.x();
    m_y = a.y();
    return *this;
  }
};

// defines Point (that is mathematical 2-vector) operations
inline Point operator+(const Point& a, const Point& b) {
  Point sum{a.x() + b.x(), a.y() + b.y()};
  return sum;
}

inline Point operator-(const Point& a, const Point& b) {
  Point diff{a.x() - b.x(), a.y() - b.y()};
  return diff;
}

inline Point operator*(double c, const Point& a) {
  Point res{c * a.x(), c * a.y()};
  return res;
}


struct Boid {
  Point r{};  // position
  Point v{};  // velocity
};

#endif