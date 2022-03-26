#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

class Obstacle
{
public:
    vector<Point2f> points;
    vector<Point2f> texPts;
    unsigned int tex = 0;
    int period;
    int phase;
    float dAngle;
    bool valid, fixed;
    int life;
    float centerX, centerY;
    float speed;
    Obstacle();
    Obstacle(const vector<Point2f> &_points);
    Obstacle(const vector<Point2f> &_points, unsigned int _tex, float ratioX = 1.0f, float ratioY = 1.0f, float angle = 0.0f, float _speed = 1.0f);
    void init(const vector<Point2f> &_points);
    void rotate(float angle);
    Point2f collision(Point2f center, float radius, float vx = 0, float vy = 0, int att = 1, bool acc = true);
    static Obstacle poly(float x, float y, int n, float len = 10.0f, float angle = 0.0f);
    static Obstacle texObs(float x, float y, int n, float _len = 10.0f, float angle = 0.0f, float _speed = 1.0f);
    static Obstacle rect(float x1, float x2, float y1, float y2);
    void draw(Mat &m, Scalar color = Scalar(0, 255, 0), float thickness = 1.0f);
    void draw();
    void attack(int att, bool acc = true);
};

extern Obstacle *obstacles;
void createObs(int num);
void clearObs();
void drawString(const char *str);
#endif