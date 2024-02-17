#include <vector>
#include <cmath>
#undef abs
#if !defined(UTILS)
#define UTILS

bool cmp_s(MazeSquare *s1, MazeSquare *s2)
{
    if (s1 != nullptr && s2 != nullptr)
    {
        if (s1->i == s2->i && s2->j == s1->j)
        {
            return true;
        }
    }
    return false;
}

class Vector2
{
public:
    Vector2() : _x(0.), _y(0.) {}
    Vector2(float x, float y) : _x(x), _y(y) {}

    float norm1() const { return std::abs(_x) + std::abs(_y); }
    float norm2() const { return std::sqrt(_x * _x + _y * _y); }
    void normalize()
    {
        _x /= norm2();
        _y /= norm2();
    }
    Vector2 normalized() const
    {
        Vector2 out = *this;
        out.normalize();
        return out;
    }

    Vector2 operator-(const Vector2 &other) const { return {_x - other._x, _y - other._y}; }
    Vector2 operator+(const Vector2 &other) const { return {_x + other._x, _y + other._y}; }
    Vector2 operator*(float f) const { return {_x * f, _y * f}; }

    bool operator==(const Vector2 &other) const { return std::abs(_x - other._x) < 1e-5 && std::abs(_y - other._y) < 1e-5; }
    bool operator!=(const Vector2 &other) const { return !(*this == other); }

    float x() const { return _x; }
    float y() const { return _y; }

    float dot(const Vector2 &other) const { return _x * other._x + _y * other._y; }
    float cross(const Vector2 &other) const { return _x * other._y - _y * other._x; }
    float angle(const Vector2 &m) const { return std::atan2(cross(m), dot(m)); }
    float angle() const { return std::atan2(_y, _x); }

private:
    float _x, _y;
};

inline float moduloPi(float a) // return angle in [-pi; pi]
{
    return (a < 0.0) ? (std::fmod(a - M_PI, 2*M_PI) + M_PI) : (std::fmod(a + M_PI, 2*M_PI) - M_PI);
}

#endif // UTILS