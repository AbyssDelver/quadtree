//code by abyss delver, created 07/07/2023
#include <SFML/Graphics.hpp>
#include <random>

#include "point.hpp"
#include "quadtree.hpp"
//########costants###########
namespace constants{
constexpr double mean{750.};
constexpr double std{300.}; 
constexpr double window_width{1500.};
constexpr double window_height{1500.};
constexpr int tree_capacity{5};
constexpr int point_number{100};
}
//###########################


double gaussian(double a, double b) {
  std::random_device rd;
  std::mt19937 eng(rd());
  std::normal_distribution<double> unif{a, b};
  return unif(eng);
}

void draw_point(sf::RenderWindow &window, Point& point){
  sf::CircleShape shape(10);
  shape.setFillColor(sf::Color::Blue);
  shape.setPosition(point.x(), point.y());
  window.draw(shape);
}

int main(){
std::vector<Point> point_vector;
Rectangle box{constants::window_width/2., constants::window_height/2. ,constants::window_width/2., constants::window_height/2.};
Quad_tree tree{constants::tree_capacity, box};

for(int i = 0; i < constants::point_number; ++i){
  point_vector.push_back(Point {gaussian(constants::mean, constants::std), gaussian(constants::mean, constants::std)});
}

for(auto& point : point_vector){
  //WTF IS HAPPENING HERE, WHY DO ALL THE VARIABLES INSIDE TREE MEMBER POINTS CHANGE HERE?????
  tree.insert(&point);
}

sf::RenderWindow window;
  window.create(sf::VideoMode(constants::window_width, constants::window_height), "boids!", sf::Style::Default);

  // SFML loop. After each loop the window is updated
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }
    // makes the window return black
    window.clear(sf::Color::Black);
    
    for(auto point : point_vector){
      draw_point(window, point);
    }

    tree.display(window);
    window.display();
  }
  tree.delete_tree();
  return 0;
}