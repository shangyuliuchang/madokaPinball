#include "includes.hpp"

int height = 1000;
int width = 1000;
int ballNum = 100;
int obsNum = 100;
int animNum = 10;
int obsPerLayer = 5;
int maxLife = 200;
int maxAccum = 2000;
int score = 0;

float mouseX, mouseY;
HANDLE uart = NULL;
bool finish = false;

unsigned int ObsTex[10];
vector<vector<Point>> ObsVertex;
vector<Point2f> ObsRatio;

unsigned int ballTex[10];
vector<vector<Point>> ballVertex;
vector<Point2f> ballRatio;

unsigned int animTex[10];
vector<vector<Point>> animVertex;
vector<Point2f> animRatio;

unsigned int kyokoTex[100];
vector<vector<Point>> kyokoVertex;
vector<Point2f> kyokoRatio;
const int kyokoNum = 74;

unsigned int madokaTex[120];
vector<vector<Point>> madokaVertex;
vector<Point2f> madokaRatio;
const int madokaNum = 116;

unsigned int sayakaTex[100];
vector<vector<Point>> sayakaVertex;
vector<Point2f> sayakaRatio;
const int sayakaNum = 93;

vector<vector<Point>> backGroundVertex;
vector<Point2f> backGroundRatio;
unsigned int backGroundTex[1];

void motionEvent(int x, int y)
{
    mouseX = x;
    mouseY = y;
}
void mouseEvent(int button, int state, int x, int y)
{
    // if (event == EVENT_LBUTTONDOWN)
    // {
    //     if (currentBall == -1)
    //     {
    //         emitX = x;
    //         emitY = y;
    //         currentBall = 0;
    //     }
    // }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (game.emit.emitState == EMIT_STATE_IDLE && Bullet::bulletNum[game.emit.emitMode] > 0)
        {
            game.emit.emitX = x;
            game.emit.emitY = y;
            game.emit.emitCnt = 0;
            game.emit.emitState = EMIT_STATE_EMITTING;
        }
        else if (game.emit.emitState == EMIT_STATE_PAUSE)
        {
            Anim::addAnim({0.2f, 0.2f, 0.0f, 0.4f},
                          {Point2f(width / 2, 50), Point2f(width / 2, 50), Point2f(width / 2, 50), Point2f(width / 2, 50), Point2f(width / 2, 50)},
                          {Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f)},
                          {0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
                          {3, 3, 3, 2},
                          animTex,
                          {Point2f(1024, 1024), Point2f(1024, 1024), Point2f(1024, 1024), Point2f(1024, 1024), Point2f(1024, 1024)},
                          {60.0f, 60.0f, 0.0f, 0.0f, 0.0f});
            game.emit.emitState = EMIT_STATE_EMITTING;
            if (Bullet::bulletReDir[game.emit.emitMode])
            {
                game.emit.emitX = x;
                game.emit.emitY = y;
            }
        }
    }
}
void display(void)
{
    // a = Mat(height, width, CV_8UC3, Scalar(0, 0, 0));
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, backGroundTex[0]);
    glEnable(GL_TEXTURE_2D);
    glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(width / 2 - 500, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(width / 2 + 500, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(width / 2 + 500, height);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(width / 2 - 500, height);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    for (int i = 0; i < obsNum; i++)
    {
        obstacles[i].draw();
        // obstacles[i].draw(a, Scalar(0, 255, 0));
    }
    for (int i = 0; i < ballNum; i++)
    {
        balls[i].draw();
    }
    for (int i = 0; i < Bullet::bulletNum.size(); i++)
    {
        char n[10];
        if (i == game.emit.emitMode)
            sprintf(n, "%d", Bullet::bulletNum[i] - game.emit.emitCnt);
        else
            sprintf(n, "%d", Bullet::bulletNum[i]);
        // glColor3f(0.5, 0.5, 0.5);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // putText(m, n, Point(centerX, centerY), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
        glRasterPos2f(width / 2 + (i - game.emit.emitMode) * 40.0f, height - 100);
        drawString(n);
    }
    for (int i = 0; i < Bullet::bulletNum.size(); i++)
    {
        if (i == game.emit.emitMode)
        {
            if (Bullet::bulletNum[i] > 0)
                drawCircle(width / 2 + (i - game.emit.emitMode) * 40.0f, height - 50, 20, atan2f(mouseX - width / 2.0f, mouseY - 50) * 57.3f, i);
            else
                drawCircle(width / 2 + (i - game.emit.emitMode) * 40.0f, height - 50, 20, 0.0f, i, 0.5);
            if (!Bullet::bulletReload[i] && Bullet::bulletNum[i] < ballNum)
                drawArc(width / 2 + (i - game.emit.emitMode) * 40.0f, height - 50, 20, (float)(game.accum % Bullet::bulletPrice[i]) / (Bullet::bulletPrice[i]) * 360);
        }
        else
        {
            if (Bullet::bulletNum[i] > 0)
                drawCircle(width / 2 + (i - game.emit.emitMode) * 40.0f, height - 50, 10, 0.0f, i);
            else
                drawCircle(width / 2 + (i - game.emit.emitMode) * 40.0f, height - 50, 10, 0.0f, i, 0.5);
            if (!Bullet::bulletReload[i] && Bullet::bulletNum[i] < ballNum)
                drawArc(width / 2 + (i - game.emit.emitMode) * 40.0f, height - 50, 10, (float)(game.accum % Bullet::bulletPrice[i]) / (Bullet::bulletPrice[i]) * 360);
        }
    }
    Anim::playAll();
    glFlush();
    // imshow("a", a);
    Sleep(1);
    // glClear(GL_COLOR_BUFFER_BIT);
    // glFlush();
}
void changeSize(GLsizei w, GLsizei h)
{
    width = w;
    height = h;
}
void loadTex(const vector<string> &texNames, vector<vector<Point>> &vertexes, vector<Point2f> &ratio, unsigned int *tex, bool detectEdge = true)
{
    glGenTextures(texNames.size(), tex);
    for (int i = 0; i < texNames.size(); i++)
    {
        Mat a = imread(texNames[i], -1);
        cvtColor(a, a, COLOR_BGR2RGBA);

        if (detectEdge)
        {
            vector<Mat> chs;
            vector<vector<Point>> contours;
            split(a, chs);
            findContours(chs[3], contours, RETR_EXTERNAL, CHAIN_APPROX_TC89_KCOS);
            int maxIndex = 0;
            for (int j = 0; j < contours.size(); j++)
            {
                if (contourArea(contours[j]) > contourArea(contours[maxIndex]))
                {
                    maxIndex = j;
                }
            }
            convexHull(Mat(contours[maxIndex]), contours[0], true);
            approxPolyDP(contours[0], contours[0], 10, true);
            vertexes.push_back(contours[0]);
            ratio.push_back(Point2f(1.0f / a.cols, 1.0f / a.rows));
        }

        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        if (a.channels() == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, 3, a.cols, a.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, a.data);
        else if (a.channels() == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, 4, a.cols, a.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, a.data);
    }
}
void loadTex(void)
{
    vector<string> texNames = {"img/madoka.jpg"};
    loadTex(texNames, backGroundVertex, backGroundRatio, backGroundTex);
    // texNames.clear();
    // for(int i=1;i<=3;i++){
    //     char tmp[100];
    //     sprintf(tmp, "witch%d.png", i);
    //     texNames.push_back(tmp);
    // }
    texNames = vector<string>({"img/witch1.png", "img/witch2.png", "img/witch3.png"});
    loadTex(texNames, ObsVertex, ObsRatio, ObsTex);
    texNames = vector<string>({"img/qb.png", "img/bomb.png", "img/mami.png", "img/madoka.png", "img/saber.png", "img/spear.png"});
    loadTex(texNames, ballVertex, ballRatio, ballTex);
    loadTex({"img/arrow.png", "img/boom.png", "img/pause1.png", "img/pause2.png", "img/saber1.png", "img/saber2.png", "img/saber3.png"}, animVertex, animRatio, animTex, false);

    texNames.clear();
    char name[100];
    for (int i = 0; i < kyokoNum; i++)
    {
        sprintf(name, "img/kyoko/frame%02d.png", i);
        texNames.push_back(name);
    }
    loadTex(texNames, kyokoVertex, kyokoRatio, kyokoTex, false);

    texNames.clear();
    for (int i = 0; i < madokaNum; i++)
    {
        sprintf(name, "img/madoka/frame%03d.png", i);
        texNames.push_back(name);
    }
    loadTex(texNames, madokaVertex, madokaRatio, madokaTex, false);

    texNames.clear();
    for (int i = 0; i < sayakaNum; i++)
    {
        sprintf(name, "img/sayaka/frame%02d.png", i);
        texNames.push_back(name);
    }
    loadTex(texNames, sayakaVertex, sayakaRatio, sayakaTex, false);
    // glGenTextures(texNames.size(), texture);
    // texVertex.push_back({Point2f(0, 0)});
    // texRatio.push_back(1.0f);
    // for (int i = 0; i < texNames.size(); i++)
    // {
    //     Mat a = imread(texNames[i], -1);
    //     cvtColor(a, a, COLOR_BGR2RGBA);

    //     if (i > 0)
    //     {
    //         vector<Mat> chs;
    //         vector<vector<Point>> contours;
    //         split(a, chs);
    //         findContours(chs[3], contours, RETR_EXTERNAL, CHAIN_APPROX_TC89_KCOS);
    //         convexHull(Mat(contours[0]), contours[0], true);
    //         approxPolyDP(contours[0], contours[0], 10, true);
    //         texVertex.push_back(contours[0]);
    //         float maxX, minX;
    //         minX = maxX = contours[0][0].x;
    //         for (int j = 0; j < contours[0].size(); j++)
    //         {
    //             if (contours[0][j].x < minX)
    //                 minX = contours[0][j].x;
    //             else if (contours[0][j].x > maxX)
    //                 maxX = contours[0][j].x;
    //         }
    //         texRatio.push_back((float)(maxX - minX) / (a.cols));
    //     }

    //     if (i == 0)
    //         a.convertTo(a, -1, 0.3f, 0);
    //     glBindTexture(GL_TEXTURE_2D, texture[i]);
    //     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     if (a.channels() == 3)
    //         glTexImage2D(GL_TEXTURE_2D, 0, 3, a.cols, a.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, a.data);
    //     else if (a.channels() == 4)
    //         glTexImage2D(GL_TEXTURE_2D, 0, 4, a.cols, a.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, a.data);
    // }
}
void onPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'c':
        initGame();
        break;
    }
    if (key >= '1' && key <= '9' && game.emit.emitState == EMIT_STATE_IDLE)
    {
        if (key - '1' < Bullet::bulletNum.size() && Bullet::bulletNum[key - '1'] > 0)
        {
            Bullet::switchTo(key - '1');
        }
    }
    else if (key == '\t' && game.emit.emitState == EMIT_STATE_EMITTING && Bullet::bulletReDir[game.emit.emitMode])
    {
        game.emit.emitState = EMIT_STATE_PAUSE;
        Anim::addAnim({0.1f, 0.1f, 0.2f, 0.1f}, {Point2f(width / 2, 50), Point2f(width / 2, 50), Point2f(width / 2, 50), Point2f(width / 2, 50), Point2f(width / 2, 50)}, {Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f)}, {0.5f, 0.5f, 0.5f, 0.5f, 0.5f}, {2, 3, 3, 3}, animTex, {Point2f(1024, 1024), Point2f(1024, 1024), Point2f(1024, 1024), Point2f(1024, 1024), Point2f(1024, 1024)}, {0.0f, 0.0f, 0.0f, 60.0f, 60.0f});
    }
}
int main(int argc, char *argv[])
{
    pthread_t th;
    pthread_t calcTh;
    balls = new Bullet[ballNum]();
    obstacles = new Obstacle[obsNum]();
    animations = new Anim[animNum]();
    uartInit();
    pthread_create(&th, NULL, dispScore, NULL);
    // Mat a = Mat(height, width, CV_8UC3, Scalar(0, 0, 0));
    // imshow("a", a);
    // setMouseCallback("a", mouseEvent);
    srand(time(NULL));
    int key;

    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
    glutCreateWindow("a");
    loadTex();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(6);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glutDisplayFunc(&display);
    glutMouseFunc(&mouseEvent);
    glutPassiveMotionFunc(&motionEvent);
    glutKeyboardFunc(&onPress);
    glutReshapeFunc(changeSize);
    glutIdleFunc(&display);
    initGame();
    pthread_create(&calcTh, NULL, calc, NULL);
    // glutDisplayFunc(&display);
    glutMainLoop();
    // while (1)
    // {
    //     key = waitKey(10);
    //     if (key == 'q')
    //         break;
    //     if (key == 'c')
    //     {
    //         initGame();
    //     }
    // }
    int *ret;
    finish = true;
    pthread_join(th, (void **)&ret);
    pthread_join(calcTh, (void **)&ret);
    delete[] balls;
    delete[] obstacles;
    delete[] animations;
    return 0;
}