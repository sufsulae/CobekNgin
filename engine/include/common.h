#pragma once
#ifndef SAMBAL_COMMON_H
#define SAMBAL_COMMON_H

#include "headers.h"
namespace sambal {
	namespace __internal__ {
		template<typename IDType> static IDType __baseObjectMaxId = 0;
		template<typename IDType> static std::vector<IDType> __baseObjectDeletedId = std::vector<IDType>();
	}
	//----FUNCTION----
	#define FUNC(name, retType, ...) retType(*name)(__VA_ARGS__)

	//----BASEOBJECT---
	template<typename IDType, class T> class BaseObject {
	private:
		IDType m_id;
	public:
		BaseObject() {
			auto len = __internal__::__baseObjectDeletedId<IDType>.size();
			if (len > 0) {
				m_id = __internal__::__baseObjectDeletedId<IDType>[0];
				__internal__::__baseObjectDeletedId<IDType>.erase(__internal__::__baseObjectDeletedId<IDType>.begin());
			}
			else {
				__internal__::__baseObjectMaxId<IDType>++;
				m_id = __internal__::__baseObjectMaxId<IDType>;
			}
		}
		~BaseObject() {
			__internal__::__baseObjectDeletedId<IDType>.push_back(m_id);
		}
		IDType get_id() { return m_id; }
		virtual std::string toString() {
			return typeid(T).name();
		}
		virtual size_t hashCode() {
			return typeid(T).hash_code();
		}
		bool operator == (BaseObject<IDType, T>& obj) {
			return this->hashCode() == obj.hashCode();
		}
		bool operator != (BaseObject<IDType, T>& obj) {
			return this->hashCode() != obj.hashCode();
		}
	};

	//---COMMON-TYPE---
	template<typename> struct Vec2;
	template<typename> struct Vec3;
	template<typename> struct Vec4;

	template<typename T> struct Vec2 {
		T x, y;
		Vec2(const T& x, const T& y) {
			this->x = x;
			this->y = y;
		}
		Vec2() : Vec2(NULL, NULL) { }

		Vec2 operator+(const Vec2& v) {
			return *(Vec2*)&(Eigen::Matrix<T, 2, 1>)(*(Eigen::Matrix<T, 2, 1>*)this + *(Eigen::Matrix<T, 2, 1>*)(&v));
		}
		Vec2 operator-(const Vec2& v) {
			return *(Vec2*)&(Eigen::Matrix<T, 2, 1>)(*(Eigen::Matrix<T, 2, 1>*)this - *(Eigen::Matrix<T, 2, 1>*)(&v));
		}
		Vec2 operator*(const T& v) {
			return *(Vec2*)&(Eigen::Matrix<T, 2, 1>)(*(Eigen::Matrix<T, 2, 1>*)this * v);
		}
		Vec2 operator/(const T& v) {
			return *(Vec2*)&(Eigen::Matrix<T, 2, 1>)(*(Eigen::Matrix<T, 2, 1>*)this / v);
		}
		bool operator==(const Vec2& v) {
			return !memcmp(this, &v, sizeof(Vec2));
		}
		bool operator!=(const Vec2& v) {
			return memcmp(this, &v, sizeof(Vec2));
		}
		void operator+=(const Vec2& v) {
			auto ptr = (Eigen::Matrix<T, 2, 1>*)&v;
			ptr->addTo(*(Eigen::Matrix<T, 2, 1>*)this);
		}
		void operator-=(const Vec2& v) {
			auto ptr = (Eigen::Matrix<T, 2, 1>*)&v;
			ptr->subTo(*(Eigen::Matrix<T, 2, 1>*)this);
		}
		void operator*=(const T& v) {
			auto ptr = (Eigen::Matrix<T, 2, 1>*)this;
			ptr->operator*=(v);
		}
		void operator/=(const T& v) {
			auto ptr = (Eigen::Matrix<T, 2, 1>*)this;
			ptr->operator/=(v);
		}
	};
	template<typename T> struct Vec3 {
		T x, y, z;
		Vec3(const T& x, const T& y, const T& z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vec3() : Vec3(NULL, NULL, NULL) { }

		Vec3 operator+(const Vec3& v) {
			return *(Vec3*)&(Eigen::Matrix<T, 3, 1>)(*(Eigen::Matrix<T, 3, 1>*)this + *(Eigen::Matrix<T, 3, 1>*)(&v));
		}
		Vec3 operator-(const Vec3& v) {
			return *(Vec3*)&(Eigen::Matrix<T, 3, 1>)(*(Eigen::Matrix<T, 3, 1>*)this - *(Eigen::Matrix<T, 3, 1>*)(&v));
		}
		Vec3 operator*(const T& v) {
			return *(Vec3*)&(Eigen::Matrix<T, 3, 1>)(*(Eigen::Matrix<T, 3, 1>*)this * v);
		}
		Vec3 operator/(const T& v) {
			return *(Vec3*)&(Eigen::Matrix<T, 3, 1>)(*(Eigen::Matrix<T, 3, 1>*)this / v);
		}
		bool operator==(const Vec3& v) {
			return !memcmp(this, &v, sizeof(Vec3));
		}
		bool operator!=(const Vec3& v) {
			return memcmp(this, &v, sizeof(Vec3));
		}
		void operator+=(const Vec3& v) {
			auto ptr = (Eigen::Matrix<T, 3, 1>*)&v;
			ptr->addTo(*(Eigen::Matrix<T, 3, 1>*)this);
		}
		void operator-=(const Vec3& v) {
			auto ptr = (Eigen::Matrix<T, 3, 1>*)&v;
			ptr->subTo(*(Eigen::Matrix<T, 3, 1>*)this);
		}
		void operator*=(const T& v) {
			auto ptr = (Eigen::Matrix<T, 3, 1>*)this;
			ptr->operator*=(v);
		}
		void operator/=(const T& v) {
			auto ptr = (Eigen::Matrix<T, 3, 1>*)this;
			ptr->operator/=(v);
		}
	};
	template<typename T> struct Vec4 {
		T x, y, z, w;
		Vec4(const T& x, const T& y, const T& z, const T& w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		Vec4() : Vec4(NULL, NULL, NULL) { }
		Vec4 operator+(const Vec4& v) {
			return *(Vec4*)&(Eigen::Matrix<T, 4, 1>)(*(Eigen::Matrix<T, 4, 1>*)this + *(Eigen::Matrix<T, 4, 1>*)(&v));
		}
		Vec4 operator-(const Vec4& v) {
			return *(Vec4*)&(Eigen::Matrix<T, 4, 1>)(*(Eigen::Matrix<T, 4, 1>*)this - *(Eigen::Matrix<T, 4, 1>*)(&v));
		}
		Vec4 operator*(const T& v) {
			return *(Vec4*)&(Eigen::Matrix<T, 4, 1>)(*(Eigen::Matrix<T, 4, 1>*)this * v);
		}
		Vec4 operator/(const T& v) {
			return *(Vec4*)&(Eigen::Matrix<T, 4, 1>)(*(Eigen::Matrix<T, 4, 1>*)this / v);
		}
		bool operator==(const Vec4& v) {
			return !memcmp(this, &v, sizeof(Vec4));
		}
		bool operator!=(const Vec4& v) {
			return memcmp(this, &v, sizeof(Vec4));
		}
		void operator+=(const Vec4& v) {
			auto ptr = (Eigen::Matrix<T, 4, 1>*)&v;
			ptr->addTo(*(Eigen::Matrix<T, 4, 1>*)this);
		}
		void operator-=(const Vec4& v) {
			auto ptr = (Eigen::Matrix<T, 4, 1>*)&v;
			ptr->subTo(*(Eigen::Matrix<T, 4, 1>*)this);
		}
		void operator*=(const T& v) {
			auto ptr = (Eigen::Matrix<T, 4, 1>*)this;
			ptr->operator*=(v);
		}
		void operator/=(const T& v) {
			auto ptr = (Eigen::Matrix<T, 4, 1>*)this;
			ptr->operator/=(v);
		}
	};
	template<typename T> struct Size {
		T width, height;
		Size(const T& width, const T& height) {
			this->width = width;
			this->height = height;
		}
		Size() : Size(NULL, NULL) {}
		Size operator+(const Size& v) {
			return *(Size*)&(Eigen::Matrix<T, 2, 1>)(*(Eigen::Matrix<T, 2, 1>*)this + *(Eigen::Matrix<T, 2, 1>*)(&v));
		}
		Size operator-(const Size& v) {
			return *(Size*)&(Eigen::Matrix<T, 2, 1>)(*(Eigen::Matrix<T, 2, 1>*)this - *(Eigen::Matrix<T, 2, 1>*)(&v));
		}
		Size operator*(const T& v) {
			return *(Size*)&(Eigen::Matrix<T, 2, 1>)(*(Eigen::Matrix<T, 2, 1>*)this * v);
		}
		Size operator/(const T& v) {
			return *(Size*)&(Eigen::Matrix<T, 2, 1>)(*(Eigen::Matrix<T, 2, 1>*)this / v);
		}
		bool operator==(const Size& v) {
			return !memcmp(this, &v, sizeof(Size));
		}
		bool operator!=(const Size& v) {
			return memcmp(this, &v, sizeof(Size));
		}
		void operator+=(const Size& v) {
			auto ptr = (Eigen::Matrix<T, 2, 1>*)&v;
			ptr->addTo(*(Eigen::Matrix<T, 2, 1>*)this);
		}
		void operator-=(const Size& v) {
			auto ptr = (Eigen::Matrix<T, 2, 1>*)&v;
			ptr->subTo(*(Eigen::Matrix<T, 2, 1>*)this);
		}
		void operator*=(const T& v) {
			auto ptr = (Eigen::Matrix<T, 2, 1>*)this;
			ptr->operator*=(v);
		}
		void operator/=(const T& v) {
			auto ptr = (Eigen::Matrix<T, 2, 1>*)this;
			ptr->operator/=(v);
		}
	};
	template<typename T> struct Color {
		T col0, col1, col2, col3;
		Color(const T& col0, const T& col1, const T& col2, const T& col3) {
			this->col0 = col0;
			this->col1 = col1;
			this->col2 = col2;
			this->col3 = col3;
		}
		Color() : Color(NULL, NULL, NULL, NULL) {}
	};
	template<typename T> struct Rect {
		T x, y, width, height;
		Rect(T x, T y, T width, T height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}
		Rect() : Rect(NULL, NULL, NULL, NULL) {}
		T xMin() { return x; }
		T xMax() { return x + width; }
		T yMin() { return y; }
		T yMax() { return y + height; }
		Vec2<T> center() { return Vec2<T>(xMax() / 2, yMax() / 2); }
	};

	template<typename T, uint row, uint column> struct Mat {
		T data[row * column];

		Mat operator+(const Mat& v) {
			return *(Mat*)&(Eigen::Matrix<T, row, column>)(*(Eigen::Matrix<T, row, column>*)this + *(Eigen::Matrix<T, row, column>*)(&v));
		}
		Mat operator-(const Mat& v) {
			return *(Mat*)&(Eigen::Matrix<T, row, column>)(*(Eigen::Matrix<T, row, column>*)this - *(Eigen::Matrix<T, row, column>*)(&v));
		}
		Mat operator*(const Mat& v) {
			return *(Mat*)&(Eigen::Matrix<T, row, column>)(*(Eigen::Matrix<T, row, column>*)this * *(Eigen::Matrix<T, row, column>*)(&v));
		}
		Mat operator*(const T& v) {
			return *(Mat*)&(Eigen::Matrix<T, row, column>)(*(Eigen::Matrix<T, row, column>*)this * v);
		}
		Mat operator/(const T& v) {
			return *(Mat*)&(Eigen::Matrix<T, row, column>)(*(Eigen::Matrix<T, row, column>*)this / v);
		}

		bool operator==(const Mat& v) {
			return !memcmp(this, &v, sizeof(Mat));
		}
		bool operator!=(const Mat& v) {
			return memcmp(this, &v, sizeof(Mat));
		}
		void operator+=(const Mat& v) {
			auto ptr = (Eigen::Matrix<T, row, column>*)&v;
			ptr->addTo(*(Eigen::Matrix<T, row, column>*)this);
		}
		void operator-=(const Mat& v) {
			auto ptr = (Eigen::Matrix<T, row, column>*)&v;
			ptr->subTo(*(Eigen::Matrix<T, row, column>*)this);
		}
		void operator*=(const T& v) {
			auto ptr = (Eigen::Matrix<T, row, column>*)this;
			ptr->operator*=(v);
		}
		void operator/=(const T& v) {
			auto ptr = (Eigen::Matrix<T, row, column>*)this;
			ptr->operator/=(v);
		}
		uint size_row() {
			return row;
		}
		uint size_column() {
			return column;
		}
		uint size() {
			return row * column;
		}
	};

	typedef Vec2<float> Vec2F;
	typedef Vec2<int> Vec2I;
	typedef Vec3<float> Vec3F;
	typedef Vec3<int> Vec3I;
	typedef Vec4<float> Vec4F;
	typedef Vec4<int> Vec4I;
	typedef Size<float> SizeF;
	typedef Size<int> SizeI;
	typedef Color<float> ColorF;
	typedef Color<byte> Color32;
	typedef Rect<float> RectF;
	typedef Rect<int> RectI;
}

#endif