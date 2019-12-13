#include <cmath>
#include <iostream>
#include <set>
#include <numeric>
#include <tuple>

class XYZ {
public:
  int x;
  int y;
  int z;

  XYZ(int x, int y, int z) : x(x), y(y), z(z) {}

  XYZ Copy() {
    return XYZ(x, y, z);
  }

  XYZ& operator+=(const XYZ& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  bool operator<(const XYZ& other) const {
    if (x < other.x) return true;
    if (x > other.x) return false;
    if (y < other.y) return true;
    if (y > other.y) return false;
    if (z < other.z) return true;
    return false;
  }
};

std::ostream& operator<<(std::ostream& output, const XYZ& xyz) {
  return output << "XYZ(" << xyz.x << ", " << xyz.y << ", " << xyz.z << ")";
}

void ApplyGravity(const int position1, const int position2, int* velocity1, int* velocity2) {
  if (position1 < position2) {
    (*velocity1)++;
    (*velocity2)--;
  } else if (position1 > position2) {
    (*velocity1)--;
    (*velocity2)++;
  }
}

class Planet {
private:
  std::string name_;

public:
  XYZ position_;
  XYZ velocity_;

  Planet(std::string name, int x, int y, int z) : name_(name), position_(x, y, z), velocity_(0, 0, 0) {}

  void UpdateVelocity(Planet& other) {
    ApplyGravity(position_.x, other.position_.x, &velocity_.x, &other.velocity_.x);
    ApplyGravity(position_.y, other.position_.y, &velocity_.y, &other.velocity_.y);
    ApplyGravity(position_.z, other.position_.z, &velocity_.z, &other.velocity_.z);
  }

  void ApplyVelocity() {
    position_ += velocity_;
  }

  int TotalEnergy() {
    return (
      std::abs(position_.x) +
      std::abs(position_.y) +
      std::abs(position_.z)
    ) * (
      std::abs(velocity_.x) +
      std::abs(velocity_.y) +
      std::abs(velocity_.z)
    );
  }

  std::ostream& Output(std::ostream& output) const {
    return output << name_ << "(position = " << position_ << ", velocity = " << velocity_ << ")";
  }
};

std::ostream& operator<<(std::ostream& output, const Planet& planet) {
  return planet.Output(output);
}

int main() {
  // My input
  Planet io("Io", -1, 7, 3);
  Planet europa("Europa", 12, 2, -13);
  Planet ganymede("Ganymede", 14, 18, -8);
  Planet callisto("Callisto", 17, 4, -4);

  // First Example
  // Planet io("Io", -1, 0, 2);
  // Planet europa("Europa", 2, -10, -7);
  // Planet ganymede("Ganymede", 4, -8, 8);
  // Planet callisto("Callisto", 3, 5, -1);

  std::tuple<int,int,int,int> initial_x(
    io.position_.x,
    europa.position_.x,
    ganymede.position_.x,
    callisto.position_.x
  );
  std::tuple<int,int,int,int> initial_y(
    io.position_.y,
    europa.position_.y,
    ganymede.position_.y,
    callisto.position_.y
  );
  std::tuple<int,int,int,int> initial_z(
    io.position_.z,
    europa.position_.z,
    ganymede.position_.z,
    callisto.position_.z
  );

  int x_cycle = -1;
  int y_cycle = -1;
  int z_cycle = -1;

  int i = 0;

  while (x_cycle == -1 || y_cycle == -1 || z_cycle == -1) {
    if (i == 1000) {
      std::cout << "Energy after 1000 steps: " << (
        io.TotalEnergy() +
        europa.TotalEnergy() +
        ganymede.TotalEnergy() +
        callisto.TotalEnergy()
      ) << std::endl;
      std::cout << io << "\n" << europa << "\n" << ganymede << "\n" << callisto << std::endl;
    }

    if (
      i != 0 &&
      x_cycle == -1 &&
      io.velocity_.x &&
      europa.velocity_.x &&
      ganymede.velocity_.x &&
      callisto.velocity_.x &&
      initial_x == std::tuple<int,int,int,int>(
        io.position_.x,
        europa.position_.x,
        ganymede.position_.x,
        callisto.position_.x
      )
    ) {
      x_cycle = i;
      std::cout << "X cycle: " << i << std::endl;
    }

    if (
      i != 0 &&
      y_cycle == -1 &&
      io.velocity_.y &&
      europa.velocity_.y &&
      ganymede.velocity_.y &&
      callisto.velocity_.y &&
      initial_y == std::tuple<int,int,int,int>(
        io.position_.y,
        europa.position_.y,
        ganymede.position_.y,
        callisto.position_.y
      )
    ) {
      y_cycle = i;
      std::cout << "Y cycle: " << i << std::endl;
    }

    if (
      i != 0 &&
      z_cycle == -1 &&
      io.velocity_.z &&
      europa.velocity_.z &&
      ganymede.velocity_.z &&
      callisto.velocity_.z &&
      initial_z == std::tuple<int,int,int,int>(
        io.position_.z,
        europa.position_.z,
        ganymede.position_.z,
        callisto.position_.z
      )
    ) {
      z_cycle = i;
      std::cout << "Z cycle: " << i << std::endl;
    }

    if (i % 1000000 == 0) {
      std::cout << i << std::endl;
    }

    io.UpdateVelocity(europa);
    io.UpdateVelocity(ganymede);
    io.UpdateVelocity(callisto);
    europa.UpdateVelocity(ganymede);
    europa.UpdateVelocity(callisto);
    ganymede.UpdateVelocity(callisto);

    io.ApplyVelocity();
    europa.ApplyVelocity();
    ganymede.ApplyVelocity();
    callisto.ApplyVelocity();

    i++;
  }

  std::cout << std::lcm(std::lcm(x_cycle, y_cycle), z_cycle) << std::endl;
}
