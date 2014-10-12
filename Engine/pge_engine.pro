#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T22:51:01
#
#-------------------------------------------------

QT += core gui
#QT += widgets

TARGET = pge_engine
TEMPLATE = app
CONFIG += c++11

DEFINES += PGE_ENGINE

INCLUDEPATH += "../_Libs/" "../Editor/file_formats"

LIBS += -lSDL2
win32: LIBS += -lSDL2main
win32: LIBS += libversion
unix:  LIBS += -lglut -lGLU

SOURCES += main.cpp \
    ../_Libs/Box2D/Collision/Shapes/b2ChainShape.cpp \
    ../_Libs/Box2D/Collision/Shapes/b2CircleShape.cpp \
    ../_Libs/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    ../_Libs/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    ../_Libs/Box2D/Collision/b2BroadPhase.cpp \
    ../_Libs/Box2D/Collision/b2CollideCircle.cpp \
    ../_Libs/Box2D/Collision/b2CollideEdge.cpp \
    ../_Libs/Box2D/Collision/b2CollidePolygon.cpp \
    ../_Libs/Box2D/Collision/b2Collision.cpp \
    ../_Libs/Box2D/Collision/b2Distance.cpp \
    ../_Libs/Box2D/Collision/b2DynamicTree.cpp \
    ../_Libs/Box2D/Collision/b2TimeOfImpact.cpp \
    ../_Libs/Box2D/Common/b2BlockAllocator.cpp \
    ../_Libs/Box2D/Common/b2Draw.cpp \
    ../_Libs/Box2D/Common/b2Math.cpp \
    ../_Libs/Box2D/Common/b2Settings.cpp \
    ../_Libs/Box2D/Common/b2StackAllocator.cpp \
    ../_Libs/Box2D/Common/b2Timer.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2Contact.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    ../_Libs/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2Joint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    ../_Libs/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    ../_Libs/Box2D/Dynamics/b2Body.cpp \
    ../_Libs/Box2D/Dynamics/b2ContactManager.cpp \
    ../_Libs/Box2D/Dynamics/b2Fixture.cpp \
    ../_Libs/Box2D/Dynamics/b2Island.cpp \
    ../_Libs/Box2D/Dynamics/b2World.cpp \
    ../_Libs/Box2D/Dynamics/b2WorldCallbacks.cpp \
    ../_Libs/Box2D/Rope/b2Rope.cpp \
    ../Editor/file_formats/file_formats.cpp \
    ../Editor/file_formats/file_lvl.cpp \
    ../Editor/file_formats/file_lvlx.cpp \
    ../Editor/file_formats/file_npc_txt.cpp \
    ../Editor/file_formats/file_wld.cpp \
    ../Editor/file_formats/file_wldx.cpp \
    ../Editor/file_formats/lvl_filedata.cpp \
    ../Editor/file_formats/npc_filedata.cpp \
    ../Editor/file_formats/pge_x.cpp \
    ../Editor/file_formats/smbx64.cpp \
    ../Editor/file_formats/wld_filedata.cpp \
    graphics.cpp \
    init.cpp

HEADERS  += \
    ../_Libs/Box2D/Box2D.h \
    ../_Libs/Box2D/Collision/Shapes/b2ChainShape.h \
    ../_Libs/Box2D/Collision/Shapes/b2CircleShape.h \
    ../_Libs/Box2D/Collision/Shapes/b2EdgeShape.h \
    ../_Libs/Box2D/Collision/Shapes/b2PolygonShape.h \
    ../_Libs/Box2D/Collision/Shapes/b2Shape.h \
    ../_Libs/Box2D/Collision/b2BroadPhase.h \
    ../_Libs/Box2D/Collision/b2Collision.h \
    ../_Libs/Box2D/Collision/b2Distance.h \
    ../_Libs/Box2D/Collision/b2DynamicTree.h \
    ../_Libs/Box2D/Collision/b2TimeOfImpact.h \
    ../_Libs/Box2D/Common/b2BlockAllocator.h \
    ../_Libs/Box2D/Common/b2Draw.h \
    ../_Libs/Box2D/Common/b2GrowableStack.h \
    ../_Libs/Box2D/Common/b2Math.h \
    ../_Libs/Box2D/Common/b2Settings.h \
    ../_Libs/Box2D/Common/b2StackAllocator.h \
    ../_Libs/Box2D/Common/b2Timer.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2CircleContact.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2Contact.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2ContactSolver.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    ../_Libs/Box2D/Dynamics/Contacts/b2PolygonContact.h \
    ../_Libs/Box2D/Dynamics/Joints/b2DistanceJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2FrictionJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2GearJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2Joint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2MotorJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2MouseJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2PulleyJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2RopeJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2WeldJoint.h \
    ../_Libs/Box2D/Dynamics/Joints/b2WheelJoint.h \
    ../_Libs/Box2D/Dynamics/b2Body.h \
    ../_Libs/Box2D/Dynamics/b2ContactManager.h \
    ../_Libs/Box2D/Dynamics/b2Fixture.h \
    ../_Libs/Box2D/Dynamics/b2Island.h \
    ../_Libs/Box2D/Dynamics/b2TimeStep.h \
    ../_Libs/Box2D/Dynamics/b2World.h \
    ../_Libs/Box2D/Dynamics/b2WorldCallbacks.h \
    ../_Libs/Box2D/Rope/b2Rope.h \
    ../Editor/file_formats/file_formats.h \
    ../Editor/file_formats/lvl_filedata.h \
    ../Editor/file_formats/npc_filedata.h \
    ../Editor/file_formats/wld_filedata.h \
    graphics.h

FORMS    +=
