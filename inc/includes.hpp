#include <GL/glut.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind/bind.hpp>
#include <windows.h>
using namespace cv;
using namespace std;
using namespace boost::placeholders;
#include "uart.hpp"
#include "Obstacle.hpp"
#include "Bullet.hpp"
#include "game.hpp"
#include "main.hpp"
#include "anim.hpp"