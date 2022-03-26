#ifndef __MAIN_H__
#define __MAIN_H__
extern int height;
extern int width;
extern int ballNum;
extern int obsNum;
extern int animNum;
extern int obsPerLayer;
extern int maxLife;
extern int maxAccum;
extern int score;
extern HANDLE uart;
extern bool finish;

extern unsigned int ObsTex[];
extern vector<vector<Point>> ObsVertex;
extern vector<Point2f> ObsRatio;

extern unsigned int ballTex[];
extern vector<vector<Point>> ballVertex;
extern vector<Point2f> ballRatio;

extern unsigned int animTex[];
extern vector<vector<Point>> animVertex;
extern vector<Point2f> animRatio;

extern unsigned int kyokoTex[];
extern vector<vector<Point>> kyokoVertex;
extern vector<Point2f> kyokoRatio;
extern const int kyokoNum;

extern unsigned int madokaTex[];
extern vector<vector<Point>> madokaVertex;
extern vector<Point2f> madokaRatio;
extern const int madokaNum;

extern unsigned int sayakaTex[];
extern vector<vector<Point>> sayakaVertex;
extern vector<Point2f> sayakaRatio;
extern const int sayakaNum;
#endif