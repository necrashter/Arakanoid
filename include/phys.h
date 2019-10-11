#ifndef _PHYS_H_
#define _PHYS_H_

typedef float phys_t; // type of position, velocity etc.

template <class T>
class vector_phys{
public:
	T x;
	T y;
	vector_phys(T a=0,T b=0){
		x=a;
		y=b;
	}
	vector_phys operator -(){
		vector_phys res;
		res.x=-x;
		res.y=-y;
		return res;
	}
	vector_phys operator *(T number){
		vector_phys res;
		res.x=x*number;
		res.y=y*number;
		return res;
	}
	vector_phys operator +(vector_phys lhs){
		vector_phys res;
		res.x=x+lhs.x;
		res.y=y+lhs.y;
		return res;
	}

	void flip_horizontal(){
		x=-x;
	}

	void flip_vertical(){
		y=-y;
	}
};


template <class T>
class box_phys{
public:
	T x;
	T y;
	T w;
	T h;
	box_phys(T a=0,T b=0,T c=0, T d=0){
		x=a;
		y=b;
		w=c;
		h=d;
	}

	bool intersects(const box_phys& other){
		return (x< other.x+other.w) && (x+w > other.x) &&
				(y < other.y+other.h) && (y+h > other.y);
	}

	// horizontal intersection, but not inclusion
	bool intersectsHorizontal(const box_phys& other){
		return (x< other.x+other.w) && (x+w > other.x)
				&& !(x+w<other.x+other.w && x>other.x);
	}
	// veritcal intersection, but not inclusion
	bool intersectsVertical(const box_phys& other){
		return (y < other.y+other.h) && (y+h > other.y)
				&& !(y+h < other.y+other.h && y>other.y);
	}

	box_phys& operator +=(vector_phys<T> lhs){
		x+=lhs.x;
		y+=lhs.y;
		return *this;
	}
};

#endif
