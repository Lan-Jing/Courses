# [Inheritance]The Triangle Class

# The Triangle Class

# Description
<p>Design a class named Triangle that extends <strong>GeometricObject </strong>class.<br />
The class contains:<br />
Three <strong>double</strong> data fields named <strong>side1</strong>, <strong>side2</strong>, and <strong>side3</strong> with default values <strong>1.0</strong> to denote three sides of the triangle.<br />
A no-arg constructor that creates a default triangle with color = &quot;blue&quot;, filled = true.<br />
A constructor that creates a triangle with the specified side1, side2, side3 and color = &quot;blue&quot;, filled = true.<br />
The accessor functions for all three data fields, named <strong>getSide1()</strong>, <strong>getSide2()</strong>, <strong>getSide3()</strong>.<br />
A function named <strong>getArea()</strong> that returns the area of this triangle.<br />
A function named <strong>getPerimeter()</strong> that returns the perimeter of the triangle.<br />
<br />
class GeometricObject<br />
{<br />
public:<br />
&nbsp; GeometricObject(string color, bool filled)<br />
&nbsp; {<br />
&nbsp;&nbsp;&nbsp; this-&gt;color = color;<br />
&nbsp;&nbsp;&nbsp; this-&gt;filled = filled;<br />
&nbsp; }<br />
&nbsp; <br />
&nbsp; string getColor()<br />
&nbsp; { return color; }<br />
&nbsp; void setColor(string color)<br />
&nbsp; { this-&gt;color = color; }<br />
&nbsp; bool isFilled()<br />
&nbsp; { return filled; }<br />
&nbsp; void setFilled(bool filled)<br />
&nbsp; { this-&gt;filled = filled;}<br />
&nbsp; string toString()<br />
&nbsp; {<br />
&nbsp;&nbsp;&nbsp; return &quot;Geometric object color &quot; + color +<br />
&nbsp;&nbsp;&nbsp; &quot; filled &quot; + ((filled) ? &quot;true&quot; : &quot;false&quot;);<br />
&nbsp; }<br />
private:<br />
&nbsp; string color;<br />
&nbsp; bool filled;<br />
};<br />
&nbsp;</p>

# Input


# Output


# Sample_Input
```

```

# Sample_Ouput
```

```

# Hint
<p>You may use "#include <cmath>".</p>
<p>Please submit the GeometricObject class and Triangle class.</p>
<p>Don't submit the main() function.</p>

