#include "includes.hpp"

Game game;
void initGame(void)
{
#ifdef DEBUG
    game.accum = maxAccum;
    score = 2000;
#else
    score = 0;
    game.accum = 0;
#endif
    game.raise = 0;
    game.emitWait = 0;
    for (int i = 0; i < ballNum; i++)
        balls[i].valid = false;
    clearObs();
    createObs(obsPerLayer);
    game.emit.emitState = EMIT_STATE_IDLE;
    Bullet::bulletNum = vector<int>({50, 0, 0, 0, 0, 0});
    Bullet::bulletAtt = vector<int>({1, 100, 2, 200, 200, 0});
    Bullet::bulletPrice = vector<int>({0, 1000, 5, 1000, 1000, 1000});
    Bullet::bulletLimited = vector<bool>({false, true, false, true, true, true});
    Bullet::bulletReDir = vector<bool>({false, false, true, false, false, false});
    Bullet::bulletReload = vector<bool>({true, false, false, false, false, false});
    Bullet::bulletReflect = vector<bool>({true, true, true, false, false, false});
    Bullet::bulletRotate = vector<bool>({true, true, true, false, false, false});
    Bullet::bulletExplode = vector<float>({0.0f, 500.0f, 0.0f, 0.0f, 0.0f, 0.0f});
    Bullet::bulletSize = vector<float>({20.0f, 50.0f, 20.0f, 100.0f, 100.0f, 1000.0f});
    Bullet::bulletDisplay = vector<bool>({true, true, true, false, false, false});
    Bullet::bulletAccum = vector<bool>({true, false, true, false, false, false});
    game.emit.emitMode = EMIT_MODE_QB;
    game.emit.emitState = EMIT_STATE_IDLE;
    game.emit.emitCnt = 0;
    obsPerLayer = 5;
    maxLife = 200;
    maxAccum = 2000;
#ifdef DEBUG
    game.state = DURING_BOSS_BUTAI;
#else
    game.state = BEFORE_BOSS_MONITOR;
#endif
}
void oneRound(const boost::system::error_code &e, boost::asio::steady_timer *t)
{
    if(!finish){
        t->expires_at(t->expiry() + boost::asio::chrono::milliseconds(10));
        t->async_wait(boost::bind(oneRound, boost::asio::placeholders::error, t));
    }

    static int emitCnt = 0;
    if (game.emit.emitState != EMIT_STATE_PAUSE)
        for (int i = 0; i < ballNum; i++)
            balls[i].move();

    if (game.emit.emitState == EMIT_STATE_WAITING)
    {
        bool allAvailable = true;
        for (int i = 0; i < ballNum; i++)
            if (balls[i].valid)
                allAvailable = false;
        if (allAvailable)
        {
            if (game.raise == 0)
            {
                if (!Bullet::bulletReload[game.emit.emitMode])
                {
                    Bullet::bulletNum[game.emit.emitMode] -= game.emit.emitCnt;
                    game.accum -= game.emit.emitCnt * Bullet::bulletPrice[game.emit.emitMode];
                    game.emit.emitCnt = 0;
                }
                if (Bullet::bulletNum[game.emit.emitMode] == 0)
                    Bullet::switchTo(0);
            }
            if (game.raise < 100)
            {
                game.raise++;
                for (int i = 0; i < obsNum; i++)
                    if (obstacles[i].valid && !obstacles[i].fixed && obstacles[i].speed > 0.0f)
                    {
                        for (int j = 0; j < obstacles[i].points.size(); j++)
                            obstacles[i].points[j].y -= obstacles[i].speed;
                        obstacles[i].centerY -= obstacles[i].speed;
                        if (obstacles[i].centerY < 0.0f)
                            initGame();
                    }
            }
            else
            {
                game.raise = 0;
                game.emit.emitCnt = 0;
                game.emit.emitState = EMIT_STATE_IDLE;
                createObs(obsPerLayer);
            }
        }
    }

    emitCnt++;
    if (emitCnt >= 10)
    {
        emitCnt = 0;
        if (game.emit.emitState == EMIT_STATE_EMITTING && game.emit.emitCnt < Bullet::bulletNum[game.emit.emitMode])
        {
            float v0 = 2000;
            float tmp = sqrtf(powf(game.emit.emitX - width / 2, 2) + powf(game.emit.emitY - 50, 2));
            float angle = atan2f(game.emit.emitX - width / 2.0f, game.emit.emitY - 50) * 57.3f;
            if (game.emitWait == 0)
            {
                if (game.emit.emitMode == EMIT_MODE_ARROW)
                {
                    Anim::addAnim({4.6f, 0.0f, 0.2f, 0.0f, 0.2f}, {Point2f(width / 2 - 500 * sinf(angle / 57.3f), 50 - 500 * cosf(angle / 57.3f)), Point2f(width / 2 - 500 * sinf(angle / 57.3f), 50 - 500 * cosf(angle / 57.3f)), Point2f(width / 2 - 500 * sinf(angle / 57.3f), 50 - 500 * cosf(angle / 57.3f)), Point2f(width / 2 + 500 * sinf(angle / 57.3f), 50 + 500 * cosf(angle / 57.3f)), Point2f(width / 2 + 600 * sinf(angle / 57.3f), 50 + 600 * cosf(angle / 57.3f)), Point2f(width / 2 + 1600 * sinf(angle / 57.3f), 50 + 1600 * cosf(angle / 57.3f))},
                                    {Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f)}, {0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f}, {0, 0, 0, 0, 0}, animTex,
                                    {Point2f(512.0f, 2048.0f), Point2f(512.0f, 2048.0f), Point2f(512.0f, 2048.0f), Point2f(512.0f, 2048.0f), Point2f(512.0f, 2048.0f), Point2f(512.0f, 2048.0f)}, {angle - 180, angle - 180, angle - 180, angle - 180, angle - 180, angle - 180});

                    vector<float> t(madokaNum, 0.04f);
                    vector<Point2f> pos(madokaNum + 1, Point2f(width / 2, height / 2));
                    vector<Point3f> color(madokaNum + 1, Point3f(1.0f, 1.0f, 1.0f));
                    vector<float> alpha(madokaNum + 1, 0.9f);
                    vector<unsigned int> frame;
                    vector<Point2f> size(madokaNum + 1, Point2f(1024, 1024));
                    vector<float> animAngle(madokaNum + 1, 0.0f);
                    frame.clear();
                    for (int i = 0; i < madokaNum; i++)
                        frame.push_back(i);
                    Anim::addAnim(t, pos, color, alpha, frame, madokaTex, size, animAngle);
                    game.emitWait = 30;
                }
                if (game.emit.emitMode == EMIT_MODE_SABER)
                {
                    Anim::addAnim({3.7f, 0.0f, 0.5f, 0.0f, 0.2f, 0.2f}, {Point2f(width / 2, game.emit.emitY), Point2f(width / 2, game.emit.emitY), Point2f(width / 2, game.emit.emitY), Point2f(width / 2, game.emit.emitY), Point2f(width / 2, game.emit.emitY), Point2f(width / 2, game.emit.emitY), Point2f(width / 2, game.emit.emitY)},
                                    {Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f), Point3f(1.0f, 1.0f, 1.0f)},
                                    {0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
                                    {4, 4, 4, 4, 5, 6},
                                    animTex,
                                    {Point2f(1000, 300), Point2f(1000, 300), Point2f(1000, 300), Point2f(1000, 0), Point2f(1024, 512), Point2f(1024, 512), Point2f(1024, 512)},
                                    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
                    vector<float> t(sayakaNum, 0.04f);
                    vector<Point2f> pos(sayakaNum + 1, Point2f(width / 2, height / 2));
                    vector<Point3f> color(sayakaNum + 1, Point3f(1.0f, 1.0f, 1.0f));
                    vector<float> alpha(sayakaNum + 1, 0.9f);
                    vector<unsigned int> frame;
                    vector<Point2f> size(sayakaNum + 1, Point2f(1024, 1024));
                    vector<float> angle(sayakaNum + 1, 0.0f);
                    frame.clear();
                    for (int i = 0; i < sayakaNum; i++)
                        frame.push_back(i);
                    Anim::addAnim(t, pos, color, alpha, frame, sayakaTex, size, angle);
                    game.emitWait = 24;
                }
                if (game.emit.emitMode == EMIT_MODE_SPEAR)
                {
                    vector<float> t(kyokoNum, 0.04f);
                    vector<Point2f> pos(kyokoNum + 1, Point2f(width / 2, height / 2));
                    vector<Point3f> color(kyokoNum + 1, Point3f(1.0f, 1.0f, 1.0f));
                    vector<float> alpha(kyokoNum + 1, 0.9f);
                    vector<unsigned int> frame;
                    vector<Point2f> size(kyokoNum + 1, Point2f(1024, 1024));
                    vector<float> angle(kyokoNum + 1, 0.0f);
                    frame.clear();
                    for (int i = 0; i < kyokoNum; i++)
                        frame.push_back(i);
                    for (int i = 60; i <= kyokoNum; i++)
                        alpha[i] = 1.0f - 1.0f / (1 + kyokoNum - 60.0f) * (i - 60);
                    Anim::addAnim(t, pos, color, alpha, frame, kyokoTex, size, angle);

                    float exp;
                    for (int j = 10; j < obsNum; j++)
                    {
                        if (obstacles[j].valid && !obstacles[j].fixed)
                        {
                            exp = (height - obstacles[j].centerY) * 0.05f;
                            if (obstacles[j].centerY < height - 100)
                            {
                                obstacles[j].centerY += 200;
                                for (int i = 0; i < obstacles[j].points.size(); i++)
                                    obstacles[j].points[i].y += 200;
                            }
                            else
                            {
                                obstacles[j].centerY += 100;
                                for (int i = 0; i < obstacles[j].points.size(); i++)
                                    obstacles[j].points[i].y += 100;
                            }
                            obstacles[j].attack(exp, Bullet::bulletAccum[game.emit.emitMode]);
                        }
                    }
                    game.emitWait = 19;
                }
            }

            if (game.emitWait > 0)
                game.emitWait--;

            if (game.emitWait == 0)
            {
                if (game.emit.emitMode == EMIT_MODE_SABER)
                    balls[game.emit.emitCnt].reset(0, game.emit.emitY, v0, 0.0f, 0, 0,
                                                    Bullet::bulletSize[game.emit.emitMode], 0.9f, game.emit.emitMode, angle, Bullet::bulletDisplay[game.emit.emitMode]);
                else 
                    balls[game.emit.emitCnt].reset(width / 2, 50, v0 / tmp * (game.emit.emitX - width / 2), v0 / tmp * (game.emit.emitY - 50), 0, 2000,
                                                    Bullet::bulletSize[game.emit.emitMode], 0.9f, game.emit.emitMode, angle, Bullet::bulletDisplay[game.emit.emitMode]);

                game.emit.emitCnt++;
                if (Bullet::bulletLimited[game.emit.emitMode])
                {
                    game.emit.emitState = EMIT_STATE_WAITING;
                }
            }
        }
        else if (game.emit.emitState == EMIT_STATE_EMITTING)
            game.emit.emitState = EMIT_STATE_WAITING;
    }

    for (int i = 0; i < Bullet::bulletNum.size(); i++)
    {
        if (!Bullet::bulletReload[i])
        {
            Bullet::bulletNum[i] = game.accum / Bullet::bulletPrice[i];
            if (Bullet::bulletNum[i] > ballNum)
                Bullet::bulletNum[i] = ballNum;
        }
    }
    if (game.emit.emitState != EMIT_STATE_PAUSE)
    {
        for (int i = 10; i < obsNum; i++)
        {
            if (obstacles[i].valid && !obstacles[i].fixed)
            {
                obstacles[i].phase++;
                if (obstacles[i].phase >= obstacles[i].period)
                {
                    obstacles[i].phase = 0;
                    obstacles[i].rotate(obstacles[i].dAngle);
                    obstacles[i].dAngle *= -1.0f;
                }
            }
        }
        for (int i = 1; i < obsNum; i++)
        {
            if (obstacles[i].valid && obstacles[i].fixed == false && obstacles[i].speed < 0.0f)
            {
                for (int j = 0; j < obstacles[i].points.size(); j++)
                    obstacles[i].points[j].y += obstacles[i].speed;
                obstacles[i].centerY += obstacles[i].speed;
                if (obstacles[i].centerY < 0.0f)
                    initGame();
            }
        }
    }
    if (game.state == DURING_BOSS_MONITOR && obstacles[BOSS_MONITOR].valid == false)
    {
        game.state = AFTER_BOSS_MONITOR;
        Bullet::bulletAtt[0] = 2;
    }
    if (game.state == DURING_BOSS_BUTAI && obstacles[BOSS_BUTAI].valid == false)
    {
        game.state = AFTER_BOSS_BUTAI;
        Bullet::bulletAtt[0] = 3;
    }
    if (game.state == DURING_BOSS_OKTAVIA && obstacles[BOSS_OKTAVIA].valid == false)
    {
        game.state = AFTER_BOSS_OKTAVIA;
        Bullet::bulletAtt[0] = 4;
    }
}
void *calc(void *args){
    boost::asio::io_context ioCtx;
    boost::asio::steady_timer t(ioCtx, boost::asio::chrono::milliseconds(10));
    t.async_wait(boost::bind(oneRound, boost::asio::placeholders::error, &t));
    ioCtx.run();
    return NULL;
}