#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <iostream>

#include "point.hpp"
struct Rectangle {
  // position of center of rectangle
  double x{};
  double y{};
  // height and width of rectangle
  double w{};
  double h{};

  bool contains(const Point& p) {
    return (p.x() >= x - w && p.x() <= x + w && p.y() <= y + h &&
            p.y() >= y - h);
  }
};

class Quad_tree {
  const int m_capacity{};
  Rectangle m_boundary{};
  bool m_divided = false;

 public:
  std::vector<Point*> points;
  Quad_tree* northeast;
  Quad_tree* northwest;
  Quad_tree* southeast;
  Quad_tree* southwest;

  Quad_tree(int capacity, const Rectangle& boundary)
      : m_capacity{capacity}, m_boundary{boundary} {};
  /*
  Quad_tree(const Quad_tree& tree)
      : m_capacity{tree.m_capacity}, m_boundary{tree.m_boundary} {}  // does
  this work??
 */
  void subdivide() {
    // ne stands for north east
    Rectangle ne = Rectangle{m_boundary.x + m_boundary.w / 2.,
                             m_boundary.y + m_boundary.h / 2.,
                             m_boundary.w / 2., m_boundary.h / 2.};
    northeast = new Quad_tree{m_capacity, ne};

    Rectangle nw = Rectangle{m_boundary.x - m_boundary.w / 2.,
                             m_boundary.y + m_boundary.h / 2.,
                             m_boundary.w / 2., m_boundary.h / 2.};
    northwest = new Quad_tree{m_capacity, nw};

    Rectangle se = Rectangle{m_boundary.x + m_boundary.w / 2.,
                             m_boundary.y - m_boundary.h / 2.,
                             m_boundary.w / 2., m_boundary.h / 2.};
    southeast = new Quad_tree{m_capacity, se};

    Rectangle sw = Rectangle{m_boundary.x - m_boundary.w / 2.,
                             m_boundary.y - m_boundary.h / 2.,
                             m_boundary.w / 2., m_boundary.h / 2.};
    southwest = new Quad_tree{m_capacity, sw};
    m_divided = true;
  }

  void insert(Point* point_ptr) {
    if (point_ptr && m_boundary.contains(*point_ptr)) {
      if (static_cast<int>(points.size()) < m_capacity && !m_divided) {
        points.push_back(point_ptr);
      } else {
        if (!m_divided) {
          subdivide();
          for (auto& inserted_points : points) {
            northeast->insert(inserted_points);
            northwest->insert(inserted_points);
            southeast->insert(inserted_points);
            southwest->insert(inserted_points);
          }
          points.clear();
        }
        northeast->insert(point_ptr);
        northwest->insert(point_ptr);
        southeast->insert(point_ptr);
        southwest->insert(point_ptr);
      }
    }
  }

  bool square_collide(double range, Point& p) const {
    if (p.x() + range < m_boundary.x - m_boundary.w ||
        p.x() - range > m_boundary.x + m_boundary.w ||
        p.y() + range < m_boundary.y - m_boundary.h ||
        p.y() - range > m_boundary.y + m_boundary.h) {
      return false;
    }
    return true;
  }

  void query(double range, Point& p, std::vector<Point*>& in_range) const {
    if (!square_collide(range, p)) {
      return;
    }

    for (auto& point : points) {
      if ((*point - p).distance() < range * 2) {
        in_range.push_back(point);
      }
    }

    if (m_divided) {
      northeast->query(range, p, in_range);
      northwest->query(range, p, in_range);
      southeast->query(range, p, in_range);
      southwest->query(range, p, in_range);
    }
  }

  void display(sf::RenderWindow& window) {
    sf::RectangleShape rect;
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(1);
    rect.setFillColor(sf::Color(0, 255, 0, 0));
    sf::Vector2f pos;

    pos =
        sf::Vector2f(m_boundary.x - m_boundary.w, m_boundary.y - m_boundary.h);
    rect.setPosition(pos);
    rect.setSize(sf::Vector2f(m_boundary.w * 2, m_boundary.h * 2));
    window.draw(rect);
    if (m_divided) {
      northwest->display(window);
      northeast->display(window);
      southeast->display(window);
      southwest->display(window);
      return;
    }
  }

  void print_tree() {
    for (auto& point_ptr : points) {
      std::cout << (*point_ptr).x() << " , ";
    }
    std::cout << '\n';
    if (m_divided) {
      northeast->print_tree();
      northwest->print_tree();
      southeast->print_tree();
      southwest->print_tree();
    }
  }

  void delete_tree() {
    if (m_divided) {
      northeast->delete_tree();
      northwest->delete_tree();
      southeast->delete_tree();
      southwest->delete_tree();
      if (!(northeast->m_divided)) {
        delete northeast;
        northeast = nullptr;
        delete northwest;
        northwest = nullptr;
        delete southeast;
        southeast = nullptr;
        delete southwest;
        southwest = nullptr;
        m_divided = false;
      }
    }
  }
};
#endif
