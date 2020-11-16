# [Exception] Hex to decimal

# Hex to decimal

# Description
<p>Write a function that parses a hex number as a string into a decimal integer. <br />
The function header is as follows:<br />
<br />
int <strong>parseHex</strong>(const char * const hexString)<br />
<br />
For example, hexString &quot;A5&quot; is 165 (10*16+5=165). So, parseHex(&quot;A5&quot;) returns 165.<br />
Your implementation of parseHex function should throw a <strong>runtime_error</strong> exception if the binaryString is not a hex string.<br />
The test function f():<br />
void f()<br />
{<br />
&nbsp; try<br />
&nbsp; {<br />
&nbsp;&nbsp;&nbsp; cout &lt;&lt; parseHex(&quot;10&quot;) &lt;&lt; endl;<br />
&nbsp;&nbsp;&nbsp; cout &lt;&lt; parseHex(&quot;ABC&quot;) &lt;&lt; endl;<br />
&nbsp;&nbsp;&nbsp; cout &lt;&lt; parseHex(&quot;9G3&quot;) &lt;&lt; endl;<br />
&nbsp; }<br />
&nbsp; catch (runtime_error ex)<br />
&nbsp; {<br />
&nbsp;&nbsp;&nbsp; cout &lt;&lt; ex.what() &lt;&lt; endl;<br />
&nbsp; }<br />
}<br />
should output:<br />
16<br />
2748<br />
Hex number format error<br />
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


