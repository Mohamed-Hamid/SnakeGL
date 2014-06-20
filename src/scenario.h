#ifndef BASE_H
    #define BASE_H
    #include "base.h"
#endif

#include "snake.cpp"

class Scenario
{
private:
    Point food;
    vector<Point> barriers;

    double xEye, yEye, zEye;
    double xCenter, yCenter, zCenter;
    double xUp, yUp, zUp;

    void draw_axis();
    void draw_food();
    void draw_barrier();
public:
    int camera_mode;
    Snake snake;
    Scenario();
    void add_barrier();
    void change_food_pos();
    void draw_board();
    void draw_objects();
    ObjectType has_collision(Point p);
    void change_camera_pos();
    void set_camera();
    void reset();
};