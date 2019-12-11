namespace xy {

class XY {
public:
  int x;
  int y;

  XY(int x, int y);
  XY operator-(const XY& other) const;
  XY Normalise() const;
  bool operator==(const XY& other) const;
  double Magnitude() const;
  double Angle() const;
};

} // namespace xy
