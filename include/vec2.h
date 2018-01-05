#ifndef VEC2_H
#define VEC2_H

#include <iostream>
#include <math.h>
#include <stdlib.h>

template <class T> class Vec2 {
  public:
    Vec2() { x_ = y_ = 0; }
    Vec2(T x, T y) { x_ = x; y_ = y; }
    void Set(const T &x, const T &y) { x_ = x; y_ = y; }
    T GetX() const { return x_; }
    T GetY() const { return y_; }
    void SetX(T x) { x_ = x; }
    void SetY(T y) { y_ = y; }
    void AddX(T &x) { x_ += x; }
    void AddY(T &y) { y_ += y; }
    void Clear() { x_ = y_ = 0; }
    void Normalize() {
      T m = sqrt((x_ * x_) + (y_ * y_));
      if (m != 0) { x_ /= m; y_ /= m; }
    }

    T DotProduct(const Vec2 &v) const {
      return x_ * v.GetX() + y_ * v.GetY();
    }

    T GetDistance(const Vec2 &v) const {
      T dx = v.GetX() - x_;
      T dy = v.GetY() - y_;
      return sqrt(dx * dx + dy * dy);
    }

    static T DotProduct(const Vec2 &v1, const Vec2 &v2) {
      return v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY();
    }

    static T GetDistance(const Vec2 &v1, const Vec2 &v2) {
      T dx = v2.GetX() - v1.GetX();
			T dy = v2.GetY() - v1.GetY();
			return sqrt(dx * dx + dy * dy);
    }

    Vec2<T> GetVectorTo(const Vec2 &v) const {
			Vec2<T> temp(v.GetX() - x_, v.GetY() - y_);
			temp.Normalise();
			return temp;
		}

    static Vec2<T> GetVectorTo(const Vec2 &v1, const Vec2 &v2) {
			Vec2<T> temp(v2.GetX() - v1.GetX(), v2.GetY() - v1.GetY());
			temp.Normalise();
			return temp;
    }

    Vec2 operator+(const Vec2 &v) const { return Vec2<T>(x_ + v.GetX(), y_ + v.GetY()); }
		void operator+=(const Vec2 &v) { x_ += v.GetX(); y_ += v.GetY(); }
		Vec2 operator-(const Vec2 &v) const { return Vec2<T>(x_ - v.GetX(), y_ - v.GetY()); }
		void operator-=(const Vec2 &v) { x_ -= v.GetX(); y_ -= v.GetY(); }
		Vec2 operator*(const Vec2 &v) const { return Vec2<T>(x_ * v.GetX(), y_ * v.GetY()); }
		Vec2 operator*(const T &v) const { return Vec2<T>(x_ * v, y_ * v); }
		void operator*=(const T &v) { x_ *= v; y_ *= v; }
		void operator*=(const Vec2 &v) { x_ *= v.GetX(); y_ *= v.GetY(); }
		Vec2 operator/(const T &v) const { return Vec2<T>(x_ / v, y_ / v); }
		void operator/=(const T &v) { x_ /= v; y_ /= v; }

    void Dump() {
      std::cout << "X: " << x_ << "\t Y: " << y_ << std::endl;
    }

    T x_, y_;
};

#endif
