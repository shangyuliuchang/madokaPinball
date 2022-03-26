#ifndef __BULLET_H__
#define __BULLET_H__
#include "main.hpp"
class Bullet
{
public:
    float x, y, vx, vy, ax, ay, radius, k;
    float w, angle;
    bool valid, everHit, display;
    unsigned int type;
    LARGE_INTEGER t1, t2, tc;
    double time;
    bool inline beyondScreen()
    {
        return (x < 0 || y < 0 || x > width || y > height);
    }
    float inline distance(float _x, float _y)
    {
        return sqrtf(powf(_x - x, 2) + powf(_y - y, 2));
    }
    void inline reset(float _x, float _y, float _vx = 0.0f, float _vy = 0.0f, float _ax = 0.0f, float _ay = 0.0f, float _radius = 20.0f, float _k = 0.9f, unsigned int _type = 0, float _angle = 0.0f, bool _display = true)
    {
        x = _x;
        y = _y;
        vx = _vx;
        vy = _vy;
        ax = _ax;
        ay = _ay;
        radius = _radius;
        k = _k;
        valid = true;
        everHit = false;
        w = 0;
        angle = 0;
        type = _type;
        angle = _angle;
        display = _display;
    }
    Bullet(float _x, float _y, float _vx = 0.0f, float _vy = 0.0f, float _ax = 0.0f, float _ay = 0.0f, float _radius = 20.0f, float _k = 0.9f, unsigned int _type = 0);
    void move();
    void draw(Mat &m, Scalar color = Scalar(0, 255, 0));
    void draw();
    Bullet();
    static vector<int> bulletNum;
    static vector<int> bulletAtt;
    static vector<bool> bulletLimited;
    static vector<bool> bulletReDir;
    static vector<bool> bulletReload;
    static vector<bool> bulletReflect;
    static vector<int> bulletPrice;
    static vector<float> bulletExplode;
    static vector<float> bulletSize;
    static vector<bool> bulletRotate;
    static vector<bool> bulletDisplay;
    static vector<bool> bulletAccum;
    static void switchTo(unsigned int id);
};
extern Bullet *balls;
void drawCircle(float x, float y, float radius, float angle = 0.0f, unsigned int type = 0, float r = 1.0f);
void drawArc(float x, float y, float radius, float angle);
#endif