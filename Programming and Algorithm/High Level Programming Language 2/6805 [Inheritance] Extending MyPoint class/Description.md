# [Inheritance]Extending MyPoint class

# Extending MyPoint class

# Description
<p>The <strong>MyPoint</strong> class below is created to model a point in a two-dimensional space.<br />
Create a class named <strong>ThreeDPoint</strong> to model a point in a three-dimensional space. Let <strong>ThreeDPoint</strong> be derived from <strong>MyPoint</strong>.<br />
x, y, z represent x-, y- and z-coordinates.<br />
class MyPoint<br />
{<br />
&nbsp; private:<br />
&nbsp;&nbsp;&nbsp; double x, y;<br />
&nbsp; public:<br />
&nbsp;&nbsp;&nbsp; // The no-arg constructor that contruccts a point with coordinates(0,0)<br />
&nbsp;&nbsp;&nbsp; MyPoint();<br />
<br />
&nbsp;&nbsp;&nbsp; MyPoint(double x, double y);<br />
&nbsp;&nbsp;&nbsp; double getX() const;<br />
&nbsp;&nbsp;&nbsp; double getY() const;<br />
<br />
&nbsp;&nbsp; &nbsp;//display the distance between two points in two-dimensional space.<br />
&nbsp;&nbsp;&nbsp; double distance(const MyPoint &amp;point); <br />
};<br />
<br />
class ThreeDPoint : public MyPoint<br />
{<br />
private:<br />
&nbsp; double z;<br />
public:<br />
&nbsp; // The no-arg constructor that contruccts a point with coordinates(0,0,0) &nbsp;<br />
&nbsp; ThreeDPoint();<br />
<br />
&nbsp; ThreeDPoint(double x, double y, double z);<br />
&nbsp; double getZ() const;<br />
<br />
&nbsp; //display the distance between two points in Three-dimensional space.<br />
&nbsp; double distance(const ThreeDPoint &amp;point); &nbsp;<br />
};</p>
<p>&nbsp;</p>

# Input


# Output


# Sample_Input
```

```

# Sample_Ouput
```

```

# Hint


