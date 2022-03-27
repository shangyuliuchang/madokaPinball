#ifndef __GAME_H__
#define __GAME_H__

#define EMIT_STATE_EMITTING 0
#define EMIT_STATE_IDLE 1
#define EMIT_STATE_WAITING 2
#define EMIT_STATE_PAUSE 3
#define EMIT_MODE_QB 0
#define EMIT_MODE_BOMB 1
#define EMIT_MODE_TIMESTOP 2
#define EMIT_MODE_ARROW 3
#define EMIT_MODE_SABER 4
#define EMIT_MODE_SPEAR 5

#define BOSS_MONITOR 3
#define BOSS_BUTAI 4

#define BEFORE_BOSS_MONITOR 0
#define DURING_BOSS_MONITOR 1
#define AFTER_BOSS_MONITOR 2
#define DURING_BOSS_BUTAI 3
#define AFTER_BOSS_BUTAI 4

// #define DEBUG 1

class Game
{
public:
    struct emit_t
    {
        int emitMode;
        int emitState;
        int emitCnt;
        float emitX;
        float emitY;
    };
    emit_t emit;
    int accum = 0;
    int raise = 0;
    int emitWait = 0;
    int state = BEFORE_BOSS_MONITOR;
    void inline addAccum(int n)
    {
        if (accum + n <= maxAccum)
            accum += n;
        else
            accum = maxAccum;
    }
};
extern Game game;
void *calc(void *arg);
void initGame(void);
#endif