// =====================
// framework source code
// =====================
#include <iostream>
#include <iomanip>
#include<string>
#include<algorithm>
#include "source.h"
using namespace std;

int main() {
    MyPoint p1,p2;
    cout << fixed << setprecision(2)
           << p1.getX() << " " << p1.getY() << " " 
           << p1.distance(p2) << endl;
    double x, y;
    while(cin >> x >> y)
    {
   	  MyPoint p3 = MyPoint(x,y);
      cout << fixed << setprecision(2)
           << p3.getX() << " " << p3.getY() << " " 
           << p3.distance(p1) << endl;
    }
	return 0;
}
                                
