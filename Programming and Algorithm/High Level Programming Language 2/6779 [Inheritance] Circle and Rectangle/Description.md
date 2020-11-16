# [Inheritance]Circle and Rectangle

# Circle and Rectangle

# Description
<p class="MsoNormal" style="layout-grid-mode: char; margin-left: 24.75pt; text-indent: -24.75pt; mso-list: l0 level2 lfo1; tab-stops: list 24.75pt"><span style="font-family: 宋体; mso-bidi-font-size: 10.0pt; mso-ascii-font-family: &quot;Times New Roman&quot;; mso-hansi-font-family: &quot;Times New Roman&quot;">以点（</span><span lang="EN-US">Point</span><span style="font-family: 宋体; mso-bidi-font-size: 10.0pt; mso-ascii-font-family: &quot;Times New Roman&quot;; mso-hansi-font-family: &quot;Times New Roman&quot;">）类为基类，重新派生类 Rectangle、Circle。坐标原点为(0,0)，矩形水平放置，由左下方的顶点和长宽定义。</span></p>
<p class="MsoNormal" style="layout-grid-mode: char; margin-left: 24.75pt; text-indent: -24.75pt; mso-list: l0 level2 lfo1; tab-stops: list 24.75pt"><span style="font-family: 宋体; mso-bidi-font-size: 10.0pt; mso-ascii-font-family: &quot;Times New Roman&quot;; mso-hansi-font-family: &quot;Times New Roman&quot;">圆由圆心和半径定义。派生类操作判断任一坐标点是在图形内，还是在图形的边缘上，还是在图形外。</span></p>
<p class="MsoNormal" style="layout-grid-mode: char; margin-left: 24.75pt; text-indent: -24.75pt; mso-list: l0 level2 lfo1; tab-stops: list 24.75pt"><span style="font-family: 宋体; mso-bidi-font-size: 10.0pt; mso-ascii-font-family: &quot;Times New Roman&quot;; mso-hansi-font-family: &quot;Times New Roman&quot;">缺省初始化图形退化为坐标原点（0，0）</span></p>
<p class="MsoNormal" style="layout-grid-mode: char; margin-left: 24.75pt; text-indent: -24.75pt; mso-list: l0 level2 lfo1; tab-stops: list 24.75pt"><span style="font-family: 宋体; mso-bidi-font-size: 10.0pt; mso-ascii-font-family: &quot;Times New Roman&quot;; mso-hansi-font-family: &quot;Times New Roman&quot;">要求拷贝构造函数和定义默认构造函数</span></p>
<p class="MsoNormal" style="layout-grid-mode: char; margin-left: 24.75pt; text-indent: -24.75pt; mso-list: l0 level2 lfo1; tab-stops: list 24.75pt"><span style="font-family: 宋体; mso-bidi-font-size: 10.0pt; mso-ascii-font-family: &quot;Times New Roman&quot;; mso-hansi-font-family: &quot;Times New Roman&quot;">编程测试类设计是否正确。</span></p>
<p class="MsoNormal" style="layout-grid-mode: char; margin-left: 24.75pt; text-indent: -24.75pt; mso-list: l0 level2 lfo1; tab-stops: list 24.75pt">class Point{</p>
<div>private:</div>
<div>double x,y;</div>
<div>public:</div>
<div>Point();</div>
<div>Point(double xv,double yv);</div>
<div>Point(Point&amp; pt);</div>
<div>double getx();</div>
<div>double gety();</div>
<div>};</div>
<div>&nbsp;</div>
<div>
<div>类Circle 和类Rectangle 都包含成员函数：int position(Point &amp;pt)。&nbsp;</div>
<div>&nbsp;</div>
<div>假如点A落在圆内或是矩形内，则该函数返回-1；如果落在圆上或是矩形上，则返回0,；如果落在外面，则返回1。</div>
<div>&nbsp;</div>
<div>主函数：</div>
<div>
<div>int main()</div>
<div>{</div>
<div>Circle cc1(3,4,5);</div>
<div>Rectangle rt1(0,0,6,8);</div>
<div>Point p1(0,0);</div>
<div>cout&lt;&lt;&quot;point p1:&quot;;</div>
<div>switch(rt1.position(p1))</div>
<div>{</div>
<div>&nbsp;case 0:cout&lt;&lt;&quot;on-rectangle&quot;&lt;&lt;endl;break;</div>
<div>&nbsp;case -1:cout&lt;&lt;&quot;inside-rectangle&quot;&lt;&lt;endl;break;</div>
<div>&nbsp;case 1:cout&lt;&lt;&quot;outside-rectangle&quot;&lt;&lt;endl;break;</div>
<div>}</div>
<div>switch(cc1.position(p1))</div>
<div>{</div>
<div>&nbsp;case 0:cout&lt;&lt;&quot;on-circle&quot;&lt;&lt;endl;break;</div>
<div>&nbsp;case -1:cout&lt;&lt;&quot;inside-circle&quot;&lt;&lt;endl;break;</div>
<div>&nbsp;case 1:cout&lt;&lt;&quot;outside-circle&quot;&lt;&lt;endl;break;</div>
<div>}</div>
<div>return 0;</div>
<div>}</div>
</div>
</div>
<div>
<div>&nbsp;</div>
</div>

# Input
<p>&nbsp;none</p>

# Output
<p>&nbsp;none</p>

# Sample_Input
```

```

# Sample_Ouput
```

```

# Hint
<div>&nbsp;&nbsp;const double PI=3.14;</div>
<div>&nbsp;</div>
<div>&nbsp;you only need to submit the class:Point 、Circle、Rectangle&nbsp;</div>

