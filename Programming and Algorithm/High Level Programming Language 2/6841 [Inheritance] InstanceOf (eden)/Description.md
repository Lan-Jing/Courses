# [Inheritance]InstanceOf (eden)

# Description
In Java, all class are inherited from Object (对象类), but c++ don't have such feature

Also In Java, function instanceOf can return class name

Please implement these class below in instance.h
```
class Object;
class Animal: public Object;
class Dog: public Animal;
class Cat: public Animal;
class Vehicle: public Object;
class Bus: public Vehicle;
class Car: public Vehicle;

/*
 * 返回这个对象实例实际的类型名
 * 例如: Object *obj = new Bus();
 * 则 instaceOf(obj) == "Bus"
 * 若 obj == NULL
 * 则 instanceOf(obj) == "NULL"
 */
string instanceOf(Object *obj);
```

The **relationship **of each class:
```
            Object
              /\
             /  \
        Animal  Vehicle
          /\      /\
         /  \    /  \
       Dog  Cat Bus  Car
```

## Sample Test
```
cout << instanceOf(NULL) << endl;

// 向上转型
Object *object = new Vehicle;
cout << instanceOf(object) << endl;
delete object;

// 向下转型
object = new Animal;
Animal *animal = dynamic_cast<Animal*>(object);
cout << instanceOf(animal) << endl;
```
## Sample Output
```
NULL
Vehicle
Animal
```