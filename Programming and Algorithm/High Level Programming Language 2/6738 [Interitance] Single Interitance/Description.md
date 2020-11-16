# [Interitance] Single Interitance

实现一个简单的单重继承的程序，该程序由两个类构成：Time、ExtTime，其中后者是前者的派生类，具体描述信息如下所示：
```c++
// 基类
class Time {
 public:
  void Set(int hours, int minutes, int seconds);
  void Increment();  // 注意“进位”
  void Write() const;  // 已实现
  Time (int initHrs, int initMins, int initSecs); 
  Time (); // 默认初始化为0

 private :
  int hrs;     // 有效取值范围为：[0,24)
  int mins;  // 有效取值范围：[0,60)
  int secs;  // 有效取值范围：[0,60)
};

// 派生类
enum ZoneType {EST, CST, MST, PST, EDT, CDT, MDT, PDT } ;

class ExtTime:public Time {
 public:
  ExtTime (int initHrs, int initMins, int initSecs, ZoneType initZone);
  ExtTime ();
  void Set (int hours, int minutes, int seconds, ZoneType timeZone);
  void Write () const; // 例如时区为CDT，时间为9点5分58秒，则显示为：09:05:58 CDT
 private:
  ZoneType zone ;
};
```