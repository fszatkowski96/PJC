#include "cfightingrobot.h"
#include <QRandomGenerator>
#include "ccleaningrobot.h"
#include "csearchingrobot.h"
#include "cmap.h"
#include "cbullet.h"
#include "cgbullet.h"

CFightingRobot::CFightingRobot(CMap *m)
    :CRobot(QRandomGenerator::global()->bounded(-map_size/2, map_size/2),
              QRandomGenerator::global()->bounded(-map_size/2, map_size/2),
              QRandomGenerator::global()->bounded(0, 2*M_PI),
              150, m)
{
}

CFightingRobot::CFightingRobot(qreal xv, qreal yv, CMap *m)
    :CRobot(xv, yv, 0, 100, m)
{
}

CFightingRobot::CFightingRobot(qreal xv, qreal yv, qreal anglev, qreal rangev, CMap *m)
    :CRobot(xv, yv, anglev, rangev, m)
{
}

CFightingRobot::~CFightingRobot()
{
    map->deleteFromMap(this);
}

void CFightingRobot::move()
{
    std::vector<CObject*> neighboors = map->getNeighboorsList(this);
    std::vector<CRobot*> robots;
    for(unsigned int i=0; i<neighboors.size(); i++)
    {
        CMovable *mobject = dynamic_cast<CMovable*>(neighboors.at(i));
        CRobot *robot = dynamic_cast<CRobot*>(mobject);
        if(robot)
        {
            robots.push_back(robot);
        }
    }

    if(robots.size() != 0)
    {
        unsigned int closest = 0;
        qreal closest_distance = range;
        for(unsigned int i=0; i<robots.size(); i++)
        {
            qreal distance = (robots.at(i)->getX()-x)*(robots.at(i)->getX()-x)+(robots.at(i)->getY()-y)*(robots.at(i)->getY()-y);
            if(distance < closest_distance)
            {
                closest = i;
                closest_distance = distance;
            }
        }
        goTo(robots[closest]);
    }

    else
        moveRandomly();
}

//attack if colliding with robot
//stay if obstacle is nearby
//move randomly if there are none
void CFightingRobot::update()

{
    std::vector<CObject*> neighboors = map->getNeighboorsList(this);
    std::vector<CRobot*> robots;
    std::vector<CObstacle*> obstacles;
    std::vector<CNonMovable*> others;
    std::vector<CRobot*> empty;
    for(unsigned int i=0; i<neighboors.size(); i++)
    {
        CMovable *mobject = dynamic_cast<CMovable*>(neighboors.at(i));
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
                {
                    robots.push_back(robot);
                }
            }
        }
        else
        {
            CNonMovable *nmobject = dynamic_cast<CNonMovable*>(neighboors.at(i));
            others.push_back(nmobject);
        }
    }

    //if there are any robots attack
    if(robots.size() != 0)
    {
        unsigned int closest = 0;
        qreal closest_distance = range;
        for(unsigned int i=0; i<robots.size(); i++)
        {
            qreal distance = (robots.at(i)->getX()-x)*(robots.at(i)->getX()-x)+(robots.at(i)->getY()-y)*(robots.at(i)->getY()-y);
            distance = sqrt(distance);
            if(distance < closest_distance)
            {
                closest = i;
                closest_distance = distance;
            }
        }
        attack(robots[closest]);
    }
    unsigned int n_objects = others.size() + obstacles.size();
    if(n_objects!=0)
        avoid(others, empty, obstacles);

    else if(!(x <= map_size/2 && x >= -map_size/2 && y <= map_size/2 && y >= -map_size/2))
    {
        returnToMap();
    }

    else
        move();
}

void CFightingRobot::attack(CRobot *robot)
{

    qreal x1 = cos(angle);
    qreal y1 = sin(angle);
    qreal x2 = (robot->getX()-x);
    qreal y2 = (robot->getY()-y);
    qreal dot = x1*x2 + y1*y2;
    qreal det = x1*y2 - y1*x2;

    qreal diff = atan2(det, dot);

    if((abs(diff) <= M_PI/18) && !QRandomGenerator::global()->bounded(0,5))
    {
        CBullet *bullet = new CBullet(map, this);
        map->addObject(bullet);
        CGBullet *gbullet = new CGBullet(bullet);
        map->addGObject(gbullet);
        map->getScene()->addItem(gbullet);
    }
}
