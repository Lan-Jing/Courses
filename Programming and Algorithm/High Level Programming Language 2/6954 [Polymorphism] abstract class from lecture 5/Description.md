# [Polymorphism]abstract class from lecture 5

## Description
There are 4 classes, ```Figure``` is the base class，both ```Triangle```, ```Rectangle``` and ``` Circle``` are all inherited from it.
```Figure``` class is like following:
```
class FIGURE {
public:
	void set_size(double x, double y = 0);
	virtual double get_area() = 0;	// get_area()被声明为纯虚函数
protected:
	double x_size, y_size;
};
```
You should implement ```Figure```, ```Triangle```, ```Rectange```, ```Circle``` class.
## Output
```
Area of triangle is 60
Area of rectangle is 120
Area of circle is 706.858

```
