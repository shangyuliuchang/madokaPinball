#ifndef __ANIM_H__
#define __ANIM_H__

class Anim
{
public:
    vector<float> sep;
    vector<float> t;
    vector<Point2f> pos;
    vector<Point2f> scale;
    vector<Point3f> color;
    vector<float> alpha;
    vector<float> angle;
    LARGE_INTEGER tStart, tNow, tc;
    vector<unsigned int> frame;
    unsigned int *tex;
    bool valid;
    void reset(const vector<float> &_sep, const vector<Point2f> &_pos, const vector<Point3f> &_color, const vector<float> &_alpha,
               const vector<unsigned int> &_frame, unsigned int *_tex, const vector<Point2f> &_scale, const vector<float> &_angle);
    void play();
    static void addAnim(const vector<float> &_sep, const vector<Point2f> &_pos, const vector<Point3f> &_color, const vector<float> &_alpha,
                        const vector<unsigned int> &_frame, unsigned int *_tex, const vector<Point2f> &_scale, const vector<float> &_angle);
    static void playAll();
    Anim();
};

extern Anim *animations;
#endif