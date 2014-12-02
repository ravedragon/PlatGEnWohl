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

#include "lvl_bgo.h"
#include "../../data_configs/config_manager.h"

LVL_Bgo::LVL_Bgo()
{
    type = LVLBGO;
    data = NULL;
    animated=false;
    animator_ID=0;
}

LVL_Bgo::~LVL_Bgo()
{
    if(physBody && worldPtr)
    {
      worldPtr->DestroyBody(physBody);
      physBody->SetUserData(NULL);
      physBody = NULL;
    }
}

//float LVL_Bgo::posX()
//{
//    return data->x;
//}

//float LVL_Bgo::posY()
//{
//    return data->y;
//}

void LVL_Bgo::init()
{
    if(!worldPtr) return;
    setSize(texture.w, texture.h);

    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set( PhysUtil::pix2met( data->x+posX_coefficient ),
        PhysUtil::pix2met(data->y + posY_coefficient ) );
    bodyDef.userData = (void*)dynamic_cast<PGE_Phys_Object *>(this);
    physBody = worldPtr->CreateBody(&bodyDef);

    b2PolygonShape shape;

    shape.SetAsBox(PhysUtil::pix2met(posX_coefficient), PhysUtil::pix2met(posY_coefficient) );

    b2Fixture * bgo = physBody->CreateFixture(&shape, 1.0f);
    bgo->SetSensor(true);
    bgo->SetFriction( 0 );
}

void LVL_Bgo::render(float camX, float camY)
{
    QRectF bgoG = QRectF(posX()-camX,
                           posY()-camY,
                           width,
                           height);

    AniPos x(0,1);

    if(animated) //Get current animated frame
        x = ConfigManager::Animator_BGO[animator_ID].image();

    glEnable(GL_TEXTURE_2D);
    glColor4f( 1.f, 1.f, 1.f, 1.f);

    glBindTexture( GL_TEXTURE_2D, texId );

    glBegin( GL_QUADS );
        glTexCoord2f( 0, x.first );
        glVertex2f( bgoG.left(), bgoG.top());

        glTexCoord2f( 1, x.first );
        glVertex2f(  bgoG.right(), bgoG.top());

        glTexCoord2f( 1, x.second );
        glVertex2f(  bgoG.right(),  bgoG.bottom());

        glTexCoord2f( 0, x.second );
        glVertex2f( bgoG.left(),  bgoG.bottom());
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
