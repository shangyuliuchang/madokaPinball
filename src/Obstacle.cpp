#include "includes.hpp"

void drawString(const char *str)
{
    static int isFirstCall = 1;
    static GLuint lists;
    if (isFirstCall)
    {
        isFirstCall = 0;
        lists = glGenLists(128);
        wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
    }
    for (; *str != '\0'; ++str)
        glCallList(lists + *str);
}
Obstacle::Obstacle()
{
    points.clear();
    valid = false;
    life = 0;
    centerX = centerY = 0;
    fixed = false;
    tex = 10000;
    speed = 0.0f;
}
Obstacle::Obstacle(const vector<Point2f> &_points)
{
    points = _points;
    centerX = centerY = 0;
    for (int i = 0; i < points.size(); i++)
    {
        centerX += points[i].x;
        centerY += points[i].y;
    }
    centerX /= points.size();
    centerY /= points.size();
    if (!points.empty())
        points.push_back(points[0]);
    valid = true;
    life = 1;
    fixed = false;
    tex = 10000;
    speed = 1.0f;
}
Obstacle::Obstacle(const vector<Point2f> &_points, unsigned int _tex, float ratioX, float ratioY, float angle, float _speed)
{
    points = _points;
    centerX = centerY = 0;
    for (int i = 0; i < points.size(); i++)
    {
        centerX += points[i].x;
        centerY += points[i].y;
    }
    centerX /= points.size();
    centerY /= points.size();
    if (!points.empty())
        points.push_back(points[0]);
    valid = true;
    life = 1;
    fixed = false;
    tex = _tex;
    speed = _speed;
    period = rand() % 10 + 40;
    phase = rand() % period;
    dAngle = (float)(rand() % 10 + 10);
    texPts.clear();
    if (tex < ObsVertex.size())
    {
        for (int i = ObsVertex[tex].size() - 1; i >= 0; i--)
        {
            texPts.push_back(Point2f(ObsVertex[tex][i].x * ratioX, ObsVertex[tex][i].y * ratioY));
        }
    }
    for (int i = 0; i < points.size(); i++)
    {
        float dis = sqrtf(powf(centerX - points[i].x, 2) + powf(centerY - points[i].y, 2));
        float ang = atan2f(points[i].y - centerY, points[i].x - centerX);
        points[i].x = centerX + dis * cosf(ang + angle / 57.3f);
        points[i].y = centerY + dis * sinf(ang + angle / 57.3f);
    }
}
void Obstacle::rotate(float angle)
{
    for (int i = 0; i < points.size(); i++)
    {
        float dis = sqrtf(powf(centerX - points[i].x, 2) + powf(centerY - points[i].y, 2));
        float ang = atan2f(points[i].y - centerY, points[i].x - centerX);
        points[i].x = centerX + dis * cosf(ang + angle / 57.3f);
        points[i].y = centerY + dis * sinf(ang + angle / 57.3f);
    }
}
void Obstacle::init(const vector<Point2f> &_points)
{
    points = _points;
    centerX = centerY = 0;
    for (int i = 0; i < points.size(); i++)
    {
        centerX += points[i].x;
        centerY += points[i].y;
    }
    centerX /= points.size();
    centerY /= points.size();
    if (!points.empty())
        points.push_back(points[0]);
    valid = true;
    life = 1;
    fixed = false;
}
Point2f Obstacle::collision(Point2f center, float radius, float vx, float vy, int att, bool acc)
{
    if (valid)
    {
        for (int i = 0; i < points.size() - 1; i++)
        {
            float A = points[i].y - points[i + 1].y;
            float B = points[i + 1].x - points[i].x;
            float C = 0 - (A * points[i].x + B * points[i].y);
            float dis = fabsf((A * center.x + B * center.y + C) / sqrtf(powf(A, 2) + powf(B, 2)));
            if (dis <= radius)
            {
                float len = sqrtf(powf(points[i].x - points[i + 1].x, 2) + powf(points[i].y - points[i + 1].y, 2));
                Point2f mid = Point2f((points[i].x + points[i + 1].x) / 2.0f, (points[i].y + points[i + 1].y) / 2.0f);
                float pDis = sqrtf(powf(center.x - mid.x, 2) + powf(center.y - mid.y, 2) - powf(dis, 2));
                if (pDis <= len / 2.0f && (points[i + 1].y - points[i].y) * vx + (points[i].x - points[i + 1].x) * vy <= 0.0f)
                {
                    attack(att, acc);
                    return Point2f((points[i + 1].y - points[i].y) / len * (radius - dis) / radius * 10000.0f, (points[i].x - points[i + 1].x) / len * (radius - dis) / radius * 10000.0f);
                }
            }
        }
        for (int i = 0; i < points.size() - 1; i++)
        {
            float A = points[i].y - points[i + 1].y;
            float B = points[i + 1].x - points[i].x;
            float C = 0 - (A * points[i].x + B * points[i].y);
            float dis = fabsf((A * center.x + B * center.y + C) / sqrtf(powf(A, 2) + powf(B, 2)));
            if (dis <= radius)
            {
                float len = sqrtf(powf(points[i].x - points[i + 1].x, 2) + powf(points[i].y - points[i + 1].y, 2));
                Point2f mid = Point2f((points[i].x + points[i + 1].x) / 2.0f, (points[i].y + points[i + 1].y) / 2.0f);
                float pDis = sqrtf(powf(center.x - mid.x, 2) + powf(center.y - mid.y, 2) - powf(dis, 2));
                if (powf(points[i].x - center.x, 2) + powf(points[i].y - center.y, 2) < powf(radius, 2) && (center.x - points[i].x) * vx + (center.y - points[i].y) * vy <= 0)
                {
                    float dis1 = sqrtf(powf(points[i].x - center.x, 2) + powf(points[i].y - center.y, 2));
                    attack(att, acc);
                    return Point2f((center.x - points[i].x) / dis1 * (radius - dis1) / radius * 10000.0f, (center.y - points[i].y) / dis1 * (radius - dis1) / radius * 10000.0f);
                }
            }
        }
    }
    return Point2f(0, 0);
}
Obstacle Obstacle::poly(float x, float y, int n, float len, float angle)
{
    vector<Point2f> p;
    for (int i = 0; i < n; i++)
    {
        p.push_back(Point2f(x + len * cosf(6.2831853f * i / n + angle / 57.3f), y + len * sinf(6.2831853f * i / n + angle / 57.3f)));
    }
    return Obstacle(p, 1, 0.01f);
    // return Obstacle(p);
}
Obstacle Obstacle::texObs(float x, float y, int n, float _len, float angle, float _speed)
{
    vector<Point2f> p;
    // if (n == 1)
    // {
    //     p.push_back(Point2f(x - 2 / len * 50, y - 52 / len * 50));
    //     p.push_back(Point2f(x - 32 / len * 50, y - 22 / len * 50));
    //     p.push_back(Point2f(x - 22 / len * 50, y + 48 / len * 50));
    //     p.push_back(Point2f(x + 28 / len * 50, y + 48 / len * 50));
    //     p.push_back(Point2f(x + 28 / len * 50, y - 22 / len * 50));
    // }
    float centerX = 0, centerY = 0;
    float minX, maxX, minY, maxY;
    float len = 0.0f;
    if (n < ObsVertex.size())
    {
        minX = maxX = ObsVertex[n][0].x;
        minY = maxY = ObsVertex[n][0].y;
        for (int i = 0; i < ObsVertex[n].size(); i++)
        {
            if (ObsVertex[n][i].x > maxX)
                maxX = ObsVertex[n][i].x;
            if (ObsVertex[n][i].x < minX)
                minX = ObsVertex[n][i].x;
            if (ObsVertex[n][i].y > maxY)
                maxY = ObsVertex[n][i].y;
            if (ObsVertex[n][i].y < minY)
                minY = ObsVertex[n][i].y;
        }
        if (maxX - minX > len)
            len = maxX - minX;
        if (maxY - minY > len)
            len = maxY - minY;
        for (int i = 0; i < ObsVertex[n].size(); i++)
        {
            centerX += ObsVertex[n][i].x;
            centerY += ObsVertex[n][i].y;
        }
        centerX /= ObsVertex[n].size();
        centerY /= ObsVertex[n].size();
        for (int i = ObsVertex[n].size() - 1; i >= 0; i--)
        {
            p.push_back(Point2f((ObsVertex[n][i].x - centerX) / len * _len + x, y + (ObsVertex[n][i].y - centerY) / len * _len));
        }
    }
    return Obstacle(p, n, ObsRatio[n].x, ObsRatio[n].y, angle, _speed);
}
Obstacle Obstacle::rect(float x1, float x2, float y1, float y2)
{
    vector<Point2f> p;
    p.push_back(Point2f(x1, y1));
    p.push_back(Point2f(x2, y1));
    p.push_back(Point2f(x2, y2));
    p.push_back(Point2f(x1, y2));
    return Obstacle(p);
}
void Obstacle::draw(Mat &m, Scalar color, float thickness)
{
    if (valid)
    {
        for (int i = 0; i < points.size() - 1; i++)
        {
            line(m, points[i], points[i + 1], color, thickness);
        }
        char n[10];
        sprintf(n, "%d", life);
        putText(m, n, Point(centerX, centerY), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
    }
}
void Obstacle::draw()
{
    if (valid)
    {
        if (tex < ObsVertex.size())
        {
            glColor4f(0.5, 0.5, 0.5, 1.0f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBindTexture(GL_TEXTURE_2D, ObsTex[tex]);
            glEnable(GL_TEXTURE_2D);
            glBegin(GL_POLYGON);
            for (int i = 0; i < points.size() - 1; i++)
            {
                glTexCoord2f(texPts[i].x, texPts[i].y);
                glVertex2f(points[i].x, height - points[i].y);
            }
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
        else
        {
            for (int i = 0; i < points.size() - 1; i++)
            {
                // line(m, points[i], points[i + 1], color, thickness);
                // glColor3f(0.5, 0.5, 0.5);
                glColor4f(0.5, 0.5, 0.5, 1.0f);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBegin(GL_LINES);
                glVertex2f(points[i].x, height - points[i].y);
                glVertex2f(points[i + 1].x, height - points[i + 1].y);
                glEnd();
            }
        }
        char n[10];
        sprintf(n, "%d", life);
        // glColor3f(0.5, 0.5, 0.5);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // putText(m, n, Point(centerX, centerY), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
        glRasterPos2f(centerX, height - centerY);
        drawString(n);
    }
}

void createObs(int num)
{
    int obstacleNum = 0;
    bool noValid = true;
    for (int i = 3; i < obsNum; i++)
        if (obstacles[i].valid)
            noValid = false;
    if (noValid && score > 1000)
    {
        if (game.state == BEFORE_BOSS_MONITOR)
        {
            obstacles[BOSS_MONITOR] = Obstacle::texObs(width / 2 + (rand() % 200 - 100), height - 100, 3, 512, 0, 0.2f);
            obstacles[BOSS_MONITOR].life = 2000;
            game.state = DURING_BOSS_MONITOR;
        }
        if (game.state == AFTER_BOSS_MONITOR)
        {
            obstacles[BOSS_BUTAI] = Obstacle::texObs(width / 2 + (rand() % 200 - 100), height - 100, 4, 512, 0, 0.2f);
            obstacles[BOSS_BUTAI].life = 2000;
            game.state = DURING_BOSS_BUTAI;
        }
        if (game.state == AFTER_BOSS_BUTAI)
        {
            obstacles[BOSS_OKTAVIA] = Obstacle::texObs(width / 2 + (rand() % 200 - 100), height - 100, 6, 512, 0, 0.2f);
            obstacles[BOSS_OKTAVIA].life = 1000;
            game.state = DURING_BOSS_OKTAVIA;
        }
    }

    obstacleNum = 0;
    for (int i = 10; i < obsNum; i++)
    {
        if (obstacles[i].valid == false)
        {
            obstacles[i] = Obstacle::texObs(rand() % 1000 + width / 2 - 500, rand() % 100 + height - 200, rand() % ObsNum, 100, rand() % 360, (rand() % 1000 + 1000) / 2000.0f);
            obstacles[i].life = rand() % maxLife;
            obstacleNum++;
            if (obstacleNum >= num)
                break;
        }
    }
    if (obstacles[BOSS_BUTAI].valid)
    {
        obstacleNum = 0;
        for (int i = 10; i < obsNum; i++)
        {
            if (obstacles[i].valid == false)
            {
                obstacles[i] = Obstacle::texObs(rand() % 100 + obstacles[BOSS_BUTAI].centerX - 50, rand() % 100 + obstacles[BOSS_BUTAI].centerY - 50, 5, 100, rand() % 360, (rand() % 1000 + 3000) / -10000.0f);
                obstacles[i].life = rand() % 100;
                obstacleNum++;
                if (obstacleNum >= 1)
                    break;
            }
        }
    }
    if (obstacles[BOSS_OKTAVIA].valid)
    {
        obstacleNum = 0;
        for (int i = 10; i < obsNum; i++)
        {
            if (obstacles[i].valid == false)
            {
                obstacles[i] = Obstacle::texObs(rand() % 400 + obstacles[BOSS_OKTAVIA].centerX - 200, rand() % 100 + obstacles[BOSS_OKTAVIA].centerY - 50, 5, 100, rand() % 360, (rand() % 1000 + 3000) / -5000.0f);
                obstacles[i].life = rand() % 10;
                obstacleNum++;
                if (obstacleNum >= 3)
                    break;
            }
        }
    }
}
void clearObs()
{
    for (int i = 0; i < obsNum; i++)
        obstacles[i].valid = false;
    obstacles[0] = Obstacle::rect(-1000, width / 2 - 500, 0, height);
    obstacles[1] = Obstacle::rect(width / 2 + 500, width + 1000, 0, height);
    obstacles[2] = Obstacle::rect(0, width, -100, 0);
    obstacles[0].life = 20000;
    obstacles[1].life = 20000;
    obstacles[2].life = 20000;
    obstacles[0].fixed = true;
    obstacles[1].fixed = true;
    obstacles[2].fixed = true;
    obstacles[0].valid = true;
    obstacles[1].valid = true;
    obstacles[2].valid = true;
}
void Obstacle::attack(int att, bool acc)
{
    if (valid)
    {
        if (life < 10000)
        {
            if (life >= att)
            {
                life -= att;
                score += att;
                if (acc)
                    game.addAccum(att);
            }
            else
            {
                if (acc)
                    game.addAccum(life);
                score += life;
                life = 0;
            }
        }
        if (life <= 0)
            valid = false;
    }
}
Obstacle *obstacles;