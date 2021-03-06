#include "csearchingrobot.h"
#include <QRandomGenerator>
#include "cmap.h"

CSearchingRobot::CSearchingRobot(CMap *m)
    :CRobot(QRandomGenerator::global()->bounded(-map_size/2, map_size/2),
              QRandomGenerator::global()->bounded(-map_size/2, map_size/2),
              (QRandomGenerator::global()->bounded(0, 36)*M_PI/18),
              100, m)
{
}

CSearchingRobot::CSearchingRobot(qreal xv, qreal yv, CMap *m)
    :CRobot(xv, yv, 0, 100, m)
{
}

CSearchingRobot::CSearchingRobot(qreal xv, qreal yv, qreal anglev, qreal rangev, CMap *m)
    :CRobot(xv, yv, anglev, rangev, m)
{
}

CSearchingRobot::~CSearchingRobot()
{
    map->deleteFromMap(this);
}

//seek treasures
void CSearchingRobot::move()
{
    //check if there are treasures or obstacles nearby
    std::vector<CObject*> neighboors = map->getNeighboorsList(this);
    std::vector<CTreasure*> treasures;
    std::vector<CObstacle*> obstacles;
    std::vector<CNonMovable*> others;
    std::vector<CRobot*> robots;
    for(unsigned int i=0; i<neighboors.size(); i++)
    {
        CMovable *mobject = dynamic_cast<CMovable*>(neighboors[i]);
        if(mobject)
        {
            CObstacle *obstacle = dynamic_cast<CObstacle*>(mobject);
            if(obstacle)
            {
                obstacles.push_back(obstacle);
            }
            else
            {
                CRobot *robot = dynamic_cast<CRobot*>(mobject);
                if(robot)
                    robots.push_back(robot);
            }
        }
        else
        {
            CNonMovable *nmobject = dynamic_cast<CNonMovable*>(neighboors[i]);
            if(nmobject)
            {
                CTreasure *treasure = dynamic_cast<CTreasure*>(nmobject);
                if(treasure)
                {
                    if(distance(treasure) < collisionDistance(treasure))
                    treasures.push_back(treasure);
                }
                else
                {
                    others.push_back(nmobject);
                }
            }
        }
    }

    //check if there are any objects that might cause collisions
    unsigned int n_objects = others.size() + obstacles.size() + robots.size();
    if(n_objects!=0)
    {
        avoid(others, robots, obstacles);
        return;
    }

    else if(!(x <= map_size/2 && x >= -map_size/2 && y <= map_size/2 && y >= -map_size/2))
    {
        returnToMap();
        return;
    }

    if(treasures.size() != 0)
    {
        unsigned int closest = 0;
        qreal closest_distance = range;
        for(unsigned int i=0; i<treasures.size(); i++)
        {
            if(distance(treasures[i]) < closest_distance)
            {
                closest = i;
                closest_distance = distance(treasures[i]);
            }
        }
        goTo(treasures.at(closest));
    }

    else if(x <= map_size/2 && x >= -map_size/2 && y <= map_size/2 && y >= -map_size/2)
    {
        moveRandomly();
    }
}

//check objects nearby
//collect colliding treasures than move
void CSearchingRobot::update()
{
    //check if there are treasures nearby
    std::vector<CObject*> neighboors = map->getNeighboorsList(this);
    std::vector<CTreasure*> treasures;
    for(unsigned int i=0; i<neighboors.size(); i++)
    {
        CNonMovable *nmobject = dynamic_cast<CNonMovable*>(neighboors[i]);
        if(nmobject)
        {
            CTreasure *treasure = dynamic_cast<CTreasure*>(nmobject);
            if(treasure)
            {
                if(distance(treasure) < collisionDistance(treasure))
                treasures.push_back(treasure);
            }
        }
    }

    //if there are treasures collect them
    if(treasures.size() != 0)
    {
        for(unsigned i=0; i<treasures.size(); i++)
        {
            collect(treasures[i]);
        }
    }

    move();
}

void CSearchingRobot::collect(CTreasure *treasure)
{
    addItem(treasure);
    treasure->getCollected();
}
