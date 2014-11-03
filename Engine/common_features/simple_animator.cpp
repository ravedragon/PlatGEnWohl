/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "simple_animator.h"

//#include "logger.h"
#include <QtDebug>

SimpleAnimator::SimpleAnimator(bool enables, int framesq, int fspeed, int First, int Last, bool rev, bool bid)
{
    animated = enables;
    frameFirst = First;
    frameLast = Last;
    CurrentFrame = 0;

    isEnabled=false;

    pos1 = 0.0d;
    pos2 = 1.0f;

    bidirectional = bid;
    reverce = rev;

    speed=fspeed;
    framesQ = framesq;

    setFrame(frameFirst);
}

SimpleAnimator::~SimpleAnimator()
{}

//Returns images

AniPos SimpleAnimator::image(double frame)
{
    if((frame<0)||(frame>=framesQ))
        return AniPos(pos1, pos2);
    else
        return AniPos(frame/framesQ, frame/framesQ + 1.0d/framesQ);
}

//Animation process
void SimpleAnimator::nextFrame()
{
    if(reverce)
    { // Reverce animation
        CurrentFrame--;
        if(CurrentFrame<frameFirst)
        {
            if(bidirectional)
            {
                reverce=!reverce; // change direction on first frame
                CurrentFrame+=2;
            }
            else
            {
                 // Return to last frame;
                if(frameLast<0)
                    CurrentFrame=framesQ-1;
                else
                    CurrentFrame=frameLast;
            }
        }

    }
    else
    { // Direct animation
        CurrentFrame++;
        if(((CurrentFrame>=framesQ)&&(frameLast<0))||
           ((CurrentFrame>frameLast)&&(frameLast>=0)))
        {
            if(bidirectional)
            {
                reverce=!reverce; // change direction on last frame
                CurrentFrame-=2;
            }
            else
            {
                CurrentFrame=frameFirst; // Return to first frame;
            }
        }
    }

    pos1 = CurrentFrame/framesQ;
    pos2 = CurrentFrame/framesQ + 1.0d/framesQ;

    if(isEnabled)
        timer_id = SDL_AddTimer(speed, &SimpleAnimator::TickAnimation, this);
    else
        SDL_RemoveTimer(timer_id);

}


void SimpleAnimator::setFrame(int y)
{
    if(y>=framesQ) y= frameFirst;
    if(y<frameFirst) y = (frameLast<0)? framesQ-1 : frameLast;
    CurrentFrame = y;

    pos1 = CurrentFrame/framesQ;
    pos2 = CurrentFrame/framesQ + 1.0d/framesQ;
}

void SimpleAnimator::setFrames(int first, int last)
{
    if((frameFirst == first) && (frameLast == last)) return;
        frameFirst = first;
        frameLast = last;
        setFrame(frameFirst);
}

void SimpleAnimator::start()
{
    if(!animated) return;
    if(isEnabled) return;

    if((frameLast>0)&&((frameLast-frameFirst)<=1)) return; //Don't start singleFrame animation
    isEnabled=true;
    timer_id = SDL_AddTimer(speed, &SimpleAnimator::TickAnimation, this);
}

void SimpleAnimator::stop()
{
    if(!animated) return;
    if(!isEnabled) return;
    isEnabled=false;
    SDL_RemoveTimer(timer_id);
    setFrame(frameFirst);
}

unsigned int SimpleAnimator::TickAnimation(unsigned int x, void *p)
{
    Q_UNUSED(x);
    SimpleAnimator *self = reinterpret_cast<SimpleAnimator *>(p);
    self->nextFrame();
    return 0;
}


