#include <cmath>
#include <iostream>
#include <set>
#include <numeric>

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
  XYZ position_;
  XYZ velocity_;
  std::set<std::pair<int, int>> x_seen_;
  std::set<std::pair<int, int>> y_seen_;
  std::set<std::pair<int, int>> z_seen_;

public:
  Planet(std::string name, int x, int y, int z) : name_(name), position_(x, y, z), velocity_(0, 0, 0) {}

  void UpdateVelocity(Planet& other) {
    ApplyGravity(position_.x, other.position_.x, &velocity_.x, &other.velocity_.x);
    ApplyGravity(position_.y, other.position_.y, &velocity_.y, &other.velocity_.y);
    ApplyGravity(position_.z, other.position_.z, &velocity_.z, &other.velocity_.z);
  }

  void ApplyVelocity() {
    position_ += velocity_;
  }

  void RecordState(long unsigned int step) {
    x_seen_.insert(std::pair<int,int>(position_.x, velocity_.x));
    y_seen_.insert(std::pair<int,int>(position_.y, velocity_.y));
    z_seen_.insert(std::pair<int,int>(position_.z, velocity_.z));
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

  bool XMatchesPrevious() {
    return x_seen_.find(std::pair<int,int>(position_.x, velocity_.x)) != x_seen_.end();
  }

  bool YMatchesPrevious() {
    return y_seen_.find(std::pair<int,int>(position_.y, velocity_.y)) != y_seen_.end();
  }

  bool ZMatchesPrevious() {
    return z_seen_.find(std::pair<int,int>(position_.z, velocity_.z)) != z_seen_.end();
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
      x_cycle == -1 &&
      io.XMatchesPrevious() &&
      europa.XMatchesPrevious() &&
      ganymede.XMatchesPrevious() &&
      callisto.XMatchesPrevious()
    ) {
      x_cycle = i;
      std::cout << "X cycle: " << i << std::endl;
    }

    if (
      y_cycle == -1 &&
      io.YMatchesPrevious() &&
      europa.YMatchesPrevious() &&
      ganymede.YMatchesPrevious() &&
      callisto.YMatchesPrevious()
    ) {
      y_cycle = i;
      std::cout << "Y cycle: " << i << std::endl;
    }

    if (
      z_cycle == -1 &&
      io.ZMatchesPrevious() &&
      europa.ZMatchesPrevious() &&
      ganymede.ZMatchesPrevious() &&
      callisto.ZMatchesPrevious()
    ) {
      z_cycle = i;
      std::cout << "Z cycle: " << i << std::endl;
    }

    if (i % 1000000 == 0) {
      std::cout << i << std::endl;
    }

    io.RecordState(i);
    europa.RecordState(i);
    ganymede.RecordState(i);
    callisto.RecordState(i);

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

  std::cout << std::lcm(std::lcm(x_cycle+1, y_cycle+1), z_cycle+1) << std::endl;
}
