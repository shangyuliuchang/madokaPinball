#include "includes.hpp"

void Anim::reset(const vector<float> &_sep, const vector<Point2f> &_pos, const vector<Point3f> &_color, const vector<float> &_alpha,
                 const vector<unsigned int> &_frame, unsigned int *_tex, const vector<Point2f> &_scale, const vector<float> &_angle)
{
    sep = _sep;
    pos = _pos;
    color = _color;
    alpha = _alpha;
    frame = _frame;
    tex = _tex;
    angle = _angle;
    scale = _scale;

    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&tStart);

    t.clear();
    float tmp = 0.0f;
    t.push_back(0.0f);
    for (int i = 0; i < sep.size(); i++)
    {
        tmp += sep[i];
        t.push_back(tmp);
    }
    valid = true;
}
Anim::Anim()
{
    sep.clear();
    pos.clear();
    color.clear();
    alpha.clear();
    frame.clear();
    tex = NULL;
    angle.clear();
    scale.clear();
    valid = false;
}
void Anim::play()
{
    if (valid)
    {
        QueryPerformanceCounter(&tNow);
        float now = (double)(tNow.QuadPart - tStart.QuadPart) / (double)tc.QuadPart;
        if (now > t[t.size() - 1])
        {
            valid = false;
            return;
        }
        for (int i = 1; i < t.size(); i++)
        {
            if (now < t[i])
            {
                float ratio = (t[i] - now) / (t[i] - t[i - 1]);
                float x = ratio * pos[i - 1].x + (1 - ratio) * pos[i].x;
                float y = ratio * pos[i - 1].y + (1 - ratio) * pos[i].y;
                float r = ratio * color[i - 1].x + (1 - ratio) * color[i].x;
                float g = ratio * color[i - 1].y + (1 - ratio) * color[i].y;
                float b = ratio * color[i - 1].z + (1 - ratio) * color[i].z;
                float a = ratio * alpha[i - 1] + (1 - ratio) * alpha[i];
                float ang = ratio * angle[i - 1] + (1 - ratio) * angle[i];
                unsigned int f = frame[i - 1];
                float sclX = ratio * scale[i - 1].x + (1 - ratio) * scale[i].x;
                float sclY = ratio * scale[i - 1].y + (1 - ratio) * scale[i].y;
                sclX /= 2.0f;
                sclY /= 2.0f;

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBindTexture(GL_TEXTURE_2D, tex[f]);
                glEnable(GL_TEXTURE_2D);
                glColor4f(r, g, b, a);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(x - sclX * cosf(ang / 57.3f) + sclY * sinf(ang / 57.3f), height - y - sclX * sinf(ang / 57.3f) - sclY * cosf(ang / 57.3f));
                // glVertex2f(width / 2 - 500, 0);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f(x + sclX * cosf(ang / 57.3f) + sclY * sinf(ang / 57.3f), height - y + sclX * sinf(ang / 57.3f) - sclY * cosf(ang / 57.3f));
                // glVertex2f(width / 2 + 500, 0);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(x + sclX * cosf(ang / 57.3f) - sclY * sinf(ang / 57.3f), height - y + sclX * sinf(ang / 57.3f) + sclY * cosf(ang / 57.3f));
                // glVertex2f(width / 2 + 500, height);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(x - sclX * cosf(ang / 57.3f) - sclY * sinf(ang / 57.3f), height - y - sclX * sinf(ang / 57.3f) + sclY * cosf(ang / 57.3f));
                // glVertex2f(width / 2 - 500, height);
                glEnd();
                glDisable(GL_TEXTURE_2D);
                break;
            }
        }
    }
}
void Anim::addAnim(const vector<float> &_sep, const vector<Point2f> &_pos, const vector<Point3f> &_color, const vector<float> &_alpha,
                   const vector<unsigned int> &_frame, unsigned int *_tex, const vector<Point2f> &_scale, const vector<float> &_angle)
{
    for (int i = 0; i < animNum; i++)
    {
        if (animations[i].valid == false)
        {
            animations[i].reset(_sep, _pos, _color, _alpha, _frame, _tex, _scale, _angle);
            break;
        }
    }
}
void Anim::playAll()
{
    for (int i = 0; i < animNum; i++)
        animations[i].play();
}
Anim *animations;