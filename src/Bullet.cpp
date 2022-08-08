#include "includes.hpp"

void drawCircle(float x, float y, float radius, float angle, unsigned int type, float r)
{
    const static float Pi = 3.1415926f;
    const static int n = 20;
    // glColor3f(0.5, 0.5, 0.5);
    glColor4f(r, r, r, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glColor4f(0.5, 0.5, 0.5, 1.0f);
    // glBegin(GL_POLYGON);
    // for(int i=0;i<n;i++){
    //     glVertex2f(x + radius * cosf(2 * Pi * i / n), y + radius * sinf(2 * Pi * i / n));
    // }

    if (type < ballVertex.size())
    {
        float centerX = 0, centerY = 0;
        float minX, maxX, minY, maxY;
        float len = 0.0f;
        minX = maxX = ballVertex[type][0].x;
        minY = maxY = ballVertex[type][0].y;
        for (int i = 0; i < ballVertex[type].size(); i++)
        {
            if (ballVertex[type][i].x > maxX)
                maxX = ballVertex[type][i].x;
            if (ballVertex[type][i].x < minX)
                minX = ballVertex[type][i].x;
            if (ballVertex[type][i].y > maxY)
                maxY = ballVertex[type][i].y;
            if (ballVertex[type][i].y < minY)
                minY = ballVertex[type][i].y;
        }
        if (maxX - minX > len)
            len = maxX - minX;
        if (maxY - minY > len)
            len = maxY - minY;
        for (int i = 0; i < ballVertex[type].size(); i++)
        {
            centerX += ballVertex[type][i].x;
            centerY += ballVertex[type][i].y;
        }
        centerX /= ballVertex[type].size();
        centerY /= ballVertex[type].size();

        glBindTexture(GL_TEXTURE_2D, ballTex[type]);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_POLYGON);
        for (int i = 0; i < n; i++)
        {
            glTexCoord2f((centerX + len / 2.0f * cosf(2 * Pi * i / n)) * ballRatio[type].x, (centerY + len / 2.0f * sinf(2 * Pi * i / n)) * ballRatio[type].y);
            glVertex2f(x + radius * cosf(2 * Pi * i / n - angle / 57.3f), y - radius * sinf(2 * Pi * i / n - angle / 57.3f));
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}
void drawArc(float x, float y, float radius, float angle)
{
    const static float Pi = 3.1415926f;
    const static int n = 20;
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < n * angle / 360.0f; i++)
    {
        glBegin(GL_LINES);
        glVertex2f(x + radius * sinf(2 * Pi * i / n), y + radius * cosf(2 * Pi * i / n));
        glVertex2f(x + radius * sinf(2 * Pi * (i + 1) / n), y + radius * cosf(2 * Pi * (i + 1) / n));
        glEnd();
    }
}
Bullet::Bullet(float _x, float _y, float _vx, float _vy, float _ax, float _ay, float _radius, float _k, unsigned int _type)
{
    x = _x;
    y = _y;
    vx = _vx;
    vy = _vy;
    ax = _ax;
    ay = _ay;
    radius = _radius;
    k = _k;
    valid = false;
    everHit = false;
    w = 0;
    angle = 0;
    type = _type;
}
void Bullet::move()
{
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t2);
    time = (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
    t1 = t2;
    time /= 1.5f;
    if (valid)
    {
        if (everHit)
        {
            vx += ax * time;
            vy += ay * time;
        }
        x += vx * time;
        y += vy * time;
        if (Bullet::bulletRotate[type])
            angle += w * time;
        for (int i = 0; i < obsNum; i++)
        {
            Point2f rslt = obstacles[i].collision(Point2f(x, y), radius, vx, vy, Bullet::bulletAtt[type], Bullet::bulletAccum[type]);
            if (fabsf(rslt.x) > 0.1f || fabsf(rslt.y) > 0.1f)
            {
                float dot = (rslt.x * vx + rslt.y * vy) / (sqrt(powf(rslt.x, 2) + powf(rslt.y, 2)));
                if (dot <= 0.0f)
                {
                    if (Bullet::bulletReflect[type])
                    {
                        float dt = -sqrt(powf(rslt.x, 2) + powf(rslt.y, 2)) / 10000.0f * radius / dot;
                        x -= vx * dt;
                        y -= vy * dt;
                        vx -= 2.0f * dot * rslt.x / (sqrt(powf(rslt.x, 2) + powf(rslt.y, 2))) * k;
                        vy -= 2.0f * dot * rslt.y / (sqrt(powf(rslt.x, 2) + powf(rslt.y, 2))) * k;
                        x += vx * dt;
                        y += vy * dt;
                        vx += rslt.x * time;
                        vy += rslt.y * time;
                        w = (-rslt.x * vy + rslt.y * vx) / (sqrt(powf(rslt.x, 2) + powf(rslt.y, 2))) / radius * 10.0f;
                        everHit = true;
                    }
                    if (!obstacles[i].fixed && Bullet::bulletExplode[type] > 1.0f)
                    {
                        int exp;
                        for (int j = 0; j < obsNum; j++)
                        {
                            if (obstacles[j].valid && !obstacles[j].fixed)
                            {
                                exp = (int)(Bullet::bulletExplode[type] / sqrt(sqrt(powf(x - obstacles[j].centerX, 2) + powf(y - obstacles[j].centerY, 2))));
                                obstacles[j].attack(exp, Bullet::bulletAccum[type]);
                            }
                        }
                        Anim::addAnim({1.0f}, {Point2f(obstacles[i].centerX, obstacles[i].centerY), Point2f(obstacles[i].centerX, obstacles[i].centerY)}, {Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f)}, {1.0f, 0.0f}, {1}, animTex, {Point2f(512.0f, 512.0f), Point2f(512.0f, 512.0f)}, {0.0f, 0.0f});
                    }
                    if (i == BOSS_MONITOR)
                    {
                        if (Bullet::bulletExplode[type] > 1.0f)
                            obstacles[i].attack(10000, Bullet::bulletAccum[type]);
                        valid = false;
                    }
                    if (i == BOSS_BUTAI)
                    {
                        if (type == EMIT_MODE_ARROW)
                            obstacles[i].attack(800, Bullet::bulletAccum[type]);
                        valid = false;
                    }
                    if (i == BOSS_OKTAVIA)
                    {
                        if (type == EMIT_MODE_SPEAR)
                            obstacles[i].attack(10000, Bullet::bulletAccum[type]);
                        valid = false;
                    }
                }
            }
        }
        if (beyondScreen())
        {
            valid = false;
        }
    }
}
void Bullet::draw(Mat &m, Scalar color)
{
    if (valid)
    {
        // circle(m, Point(x, y), radius, color);
        drawCircle(x, y, radius);
    }
}
void Bullet::draw()
{
    if (valid && display)
    {
        // circle(m, Point(x, y), radius, color);
        drawCircle(x, height - y, radius, angle, type);
    }
}
Bullet::Bullet()
{
    x = 0;
    y = 0;
    vx = 0;
    vy = 0;
    ax = 0;
    ay = 0;
    k = 1.0f;
    valid = false;
    everHit = false;
    w = 0;
    angle = 0;
}
void Bullet::switchTo(unsigned int id)
{
    game.emit.emitCnt = 0;
    game.emit.emitMode = id;
}

Bullet *balls;
vector<int> Bullet::bulletNum;
vector<int> Bullet::bulletAtt;
vector<int> Bullet::bulletPrice;
vector<bool> Bullet::bulletLimited;
vector<bool> Bullet::bulletReload;
vector<bool> Bullet::bulletReDir;
vector<bool> Bullet::bulletReflect;
vector<float> Bullet::bulletExplode;
vector<float> Bullet::bulletSize;
vector<bool> Bullet::bulletRotate;
vector<bool> Bullet::bulletDisplay;
vector<bool> Bullet::bulletAccum;