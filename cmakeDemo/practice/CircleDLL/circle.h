#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include <iostream>

#ifdef CIRCLE_DLL
    #define CIRCLE_EXPORT __declspec(dllexport)
#else
    #define CIRCLE_EXPORT __declspec(dllimport)
#endif


//虚基类,用户只知道这个类
class CIRCLE_EXPORT ExportCircle
{
public:
    virtual ~ExportCircle() = default; //调用默认虚析构,以支持多台
    virtual void setRadius(double r) = 0;
    virtual double getRadius() const = 0;
    virtual void print() const = 0;

};

//内部实现类,不向外暴露
class Circle : public ExportCircle
{
public:
    Circle() = default;
    ~Circle() = default;
    double getRadius() const override;
    void setRadius(double r) override;
    void print() const override;
    constexpr double getArea() const;
    constexpr double getPerimeter() const;
private:
    double m_radius = 0.0;
};


extern  "C" CIRCLE_EXPORT ExportCircle *getClassObj();
extern  "C" CIRCLE_EXPORT void releaseClassObj(ExportCircle *);


#endif // __CIRCLE_H__