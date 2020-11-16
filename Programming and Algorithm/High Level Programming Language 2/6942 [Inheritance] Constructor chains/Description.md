# [Inheritance]Constructor chains

# Constructor chains

# Description
<div>&nbsp;</div>
<div style="text-indent: 21pt"><font size="3">There are four classes A, B, C and D. They all have the&nbsp;same form as follows:&nbsp;</font></div>
<div>&nbsp;</div>
<div><span style="color: #0000ff"><font size="3">class&nbsp;X</font></span></div>
<div><span style="color: #0000ff"><font size="3">{<br />
public:<br />
&nbsp;&nbsp;&nbsp; X() { cout &lt;&lt; &quot;In X()&quot; &lt;&lt; endl; }<br />
&nbsp;&nbsp;&nbsp; ~X() { cout &lt;&lt; &quot;In ~X()&quot; &lt;&lt; endl; }<br />
};</font></span></div>
<div>&nbsp;</div>
<div style="text-indent: 21pt"><font size="3">X belongs to { A, B, C, D }. These four classes have some kind of inheritance relationship. You are to find the right&nbsp;one to generate the desired output, which should come from the constructors and destructors.&nbsp;</font></div>
<div>&nbsp;</div>
<div style="text-indent: 21pt"><font size="3">Your submitted&nbsp;source code should&nbsp;include all the&nbsp;implementation of the A, B, C and D classes. </font></div>
<div style="text-indent: 21pt"><font size="3">No main() function should be included.</font></div>
<div>&nbsp;</div>
<div><strong><font size="3">Note</font></strong><font size="3">: the main() function of the test framework&nbsp;looks like this: </font></div>
<div><font size="3">------------------------------------------------------------------------------</font></div>
<div><font size="3">int main()<br />
{<br />
&nbsp;&nbsp;&nbsp; D d;<br />
&nbsp;&nbsp;&nbsp; return 0;<br />
}</font></div>
<div><font size="3">------------------------------------------------------------------------------</font></div>

# Input


# Output
<div><font size="3">In B()<br />
In C()<br />
In B()<br />
In A()<br />
In D()<br />
In ~D()<br />
In ~A()<br />
In ~B()<br />
In ~C()<br />
In ~B()</font></div>

# Sample_Input
```

```

# Sample_Ouput
```

```

# Hint


