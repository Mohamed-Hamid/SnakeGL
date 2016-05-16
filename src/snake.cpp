#include "snake.h"


//
// Snake implementation
//
Snake::Snake()
{
}

void Snake::reset()
{
    points.clear();
    foodType.clear();
    int d = (rand() % 4) + 1;
    set_direction(d);

    Point p;
    p.x = 0.0f;
    p.y = GROUND_DIFF;
    p.z = 0.0f;

    points.push_front(p);

    foodType.push_back(0);

    grow();
}

void Snake::move()
{
    points.pop_back();
    grow();
}

void Snake::set_direction(int d)
{
    if ((d == DOWN && direction == UP) ||
        (direction == DOWN && d == UP) ||
        (d == LEFT && direction == RIGHT) ||
        (direction == LEFT && d == RIGHT))
    {
        return;
    }

    direction = d;
}

void Snake::setFoodType(int type)
{
    foodType.push_back(type);
}

void Snake::draw()
{
    // TODO: Draw cylindric snake.
    // It's more hard.
    // Use glut2Cylinder. 
    Point h = points[0];

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
        glTranslatef(h.x, h.y, h.z);
        glutSolidCube(0.5f);
    glPopMatrix();

    enable_2D_texture();
    int j = 1;
    for (size_t i = 1; i < points.size()-1; ++i)
    {   
        int type = foodType.at(j);
        switch(type){
            case 1:
                glBindTexture(GL_TEXTURE_2D, textures[SNAKE_TEXTURE_R]);
            break;
            case 2:
                glBindTexture(GL_TEXTURE_2D, textures[SNAKE_TEXTURE_G]);
            break;
            case 3:
                glBindTexture(GL_TEXTURE_2D, textures[SNAKE_TEXTURE_B]);
            break;
        }
        Point p = points.at(i);
        glPushMatrix();
            glTranslatef(p.x, p.y, p.z);
            glut2SolidCube(0.5f);
        glPopMatrix();
        j++;
    }

    disable_2D_texture();
}

Point Snake::head()
{
    return points[0];
}

Point Snake::tail()
{
    return points[points.size() - 1];
}

void Snake::grow(bool back)
{
    Point p;
    p.x = points[0].x;
    p.y = points[0].y;
    p.z = points[0].z;

    switch (direction)
    {
        case DOWN:
            p.z += 0.5f;
        break;
        case UP:
            p.z -= 0.5f;
        break;
        case LEFT:
            p.x -= 0.5f;
        break;
        case RIGHT:
            p.x += 0.5f;
        break;
    }

    if (back)
    {
        points.push_back(p);
    }
    else
    {
        points.push_front(p);
    }
}

bool Snake::has_collision(Point p)
{
    // Skip head. It's the same point.
    for (size_t i = 1; i < points.size(); ++i)
    {
        Point b = points.at(i);

        if (p.x == b.x && p.z == b.z)
        {
            return true;
        }
    }

    return false;
}

int Snake::size()
{
    return points.size();
}