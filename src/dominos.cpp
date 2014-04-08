/*
 * Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/* 
 * Base code for CS 296 Software Systems Lab 
 * Department of Computer Science and Engineering, IIT Bombay
 * Instructor: Parag Chaudhuri
 */
#include <iostream>
#include <math.h>
#include "cs296_base.hpp"
#include "render.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "GL/freeglut.h"
#endif

#include <cstring>
using namespace std;

#include "dominos.hpp"

namespace cs296 {

    /** Constructor of our World
     * Initializes all objects and joints
     */
    dominos_t::dominos_t() {
        m_world->SetContactListener(&myContactListenerInstance);
        trig_reset = true;

        // Creating all objects
        b2Body* ground = dominos_t::createGround();
        b2Body* barrel = dominos_t::createBarrel();
        b2Body* bar = dominos_t::createBar();
        b2Body* slide = dominos_t::createSlide();
        b2Body* trigger = dominos_t::createTrigger();
        casing = dominos_t::createCasing(0.25f, 44.0f);
        bullet = dominos_t::createBullet(-4.5f, 44.0f);
        b2Body* striker_assembly = dominos_t::createStrikerAssembly();
        b2Body* fixed = dominos_t::createFixed();
        b2Body* cap = dominos_t::createCap();
        b2Body* spacer_sleeve = dominos_t::createSpacerSleeve();

        b2Body* magazine = dominos_t::createMagazine();

        // Creating all joints
        dominos_t::connectStrikerAssemblyWithSpacerSleeve(striker_assembly, spacer_sleeve);
        dominos_t::connectStrikerAssemblyWithCap(striker_assembly, cap);
        dominos_t::connectSpacerSleeveWithCap(spacer_sleeve, cap);
        dominos_t::connectSlideWithFixed(slide, fixed);

        dominos_t::connectTriggerWithGround(trigger, ground);
        dominos_t::connectTriggerWithBar(trigger, bar);

        dominos_t::connectGroundWithBar(ground, bar);

        dominos_t::connectSlideWithSpacerSleeve(slide, spacer_sleeve);
    }

    b2Body* dominos_t::createBarrel() {	
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(14, 0.5);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &polygonShape;
        fd->filter.categoryBits = BARREL;
        fd->filter.maskBits = STRIKER | CARTRIDGE | BARREL;
        fd->density = 10;
        b2BodyDef bd;
        bd.position.Set(-21.0f, 46.5f);
        bd.type = b2_dynamicBody;
        //bd.linearVelocity=b2Vec2(10,0);
        barrel = m_world->CreateBody(&bd);
        barrel->CreateFixture(fd);
        polygonShape.SetAsBox(6, 1, b2Vec2(20, 0.5), 0);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd);

        b2Vec2 vertices[4];
        vertices[0].Set(-14, -4.5);
        vertices[1].Set(16, -4.5);
        vertices[2].Set(17, -5.5);
        vertices[3].Set(-14, -5.5);
        polygonShape.Set(vertices, 4);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd);

        vertices[0].Set(16, -4.5);
        vertices[1].Set(18, -4.5);
        vertices[2].Set(20.5, -7);
        vertices[3].Set(19, -7.5);
        polygonShape.Set(vertices, 4);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd);

        vertices[0].Set(18, -4.5);
        vertices[1].Set(20, -4.5);
        vertices[2].Set(20.75, -5.25);
        vertices[3].Set(18.75, -5.25);
        polygonShape.Set(vertices, 4);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd); ///the hole where sliding_lock fits

        vertices[0].Set(20, -4.5);
        vertices[1].Set(24, -4.5);
        vertices[2].Set(26, -6.5);
        vertices[3].Set(22, -6.5);
        polygonShape.Set(vertices, 4);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd);

        return barrel;
    }

    b2Body* dominos_t::createBar() {
        b2PolygonShape shape;
        shape.SetAsBox(12.0f, 0.75f, b2Vec2(0, 0), 0.25f);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->density = 10;
        fd->filter.categoryBits = TRIGGER;
        fd->filter.maskBits = STRIKER;
        b2BodyDef bd;
        bd.position.Set(18.75f, 36.25f);
        bd.type = b2_dynamicBody;
        //bd.angularVelocity = 0.5f;
        bar = m_world->CreateBody(&bd);
        bar->CreateFixture(fd);

        b2Vec2 vertices[6];
        vertices[0].Set(6, 3.5);
        vertices[1].Set(7, 3.5);
        //vertices[2].Set(1,1);
        vertices[2].Set(10, 4.5);
        vertices[3].Set(10, 5.5);
        vertices[4].Set(9, 5.5);
        shape.Set(vertices, 5);
        fd->shape = &shape;
        bar->CreateFixture(fd); ///sear of the bar (cocks the striker)

        return bar;
    }

    b2Body* dominos_t::createSlide() {
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(13.75, 0.5);
		b2FixtureDef *fd = new b2FixtureDef;
		fd->shape = &polygonShape;
		//fd->filter.categoryBits = SPRING;
		//fd->filter.maskBits = EVERYTHING;
		fd->density = 1000;
		b2BodyDef bd;
		bd.position.Set(-20.75f, 47.5f);
		bd.type = b2_dynamicBody;
		//bd.linearVelocity=b2Vec2(10,0);
		slide = m_world->CreateBody(&bd);
		slide->CreateFixture(fd);
		polygonShape.SetAsBox(0.5f, 1.0f, b2Vec2(-13.5, -9), 0.0f);
		fd->shape = &polygonShape;
		//fd->filter.categoryBits = SPRING;
		//fd->filter.maskBits = EVERYTHING | SPRING;
		slide->CreateFixture(fd);
		polygonShape.SetAsBox(0.25f, 1.5f, b2Vec2(-12.75, -9), 0);
		fd->shape = &polygonShape;
		slide->CreateFixture(fd);
		polygonShape.SetAsBox(4.525, 1.5, b2Vec2(30.375, -1), 0);
		fd->shape = &polygonShape;
		slide->CreateFixture(fd); ///helps the barrel to slide back
		polygonShape.SetAsBox(0.001, 0.43, b2Vec2(-13.75, -7), 0);
		slide->CreateFixture(fd); ///small part which helps the barrel to slide back
		polygonShape.SetAsBox(0.5, 0.25, b2Vec2(26.35, -2.75), 0);
		slide->CreateFixture(fd); ///to narrow the striker chamber at the front
		polygonShape.SetAsBox(0.5, 0.25, b2Vec2(26.35, -4.75), 0);
		slide->CreateFixture(fd); ///to narrow the striker chamber at the front
		b2Vec2 vertices[5];
		vertices[0].Set(29.375 - 2.525, 1.5 - 6);
		vertices[1].Set(29.375 - 2.525, -1.5 - 6);
		vertices[2].Set(29.375 - 2.025, -1.5 - 6);
		vertices[3].Set(29.375 + 2.525, 0 - 6);
		vertices[4].Set(29.375 + 2.525, 1.5 - 6);
		polygonShape.Set(vertices, 5);
		fd->shape = &polygonShape;
		fd->filter.categoryBits = TRIGGER;
		fd->filter.maskBits = EVERYTHING;
		fd->density = 0;
		slide->CreateFixture(fd); ///for pushing the new bullet in the mag, so that it springs up after some time (interface between magazine and other parts of the gun)
		polygonShape.SetAsBox(5, 0.5, b2Vec2(36.875, -5.5), 0);
		fd->shape = &polygonShape;
		slide->CreateFixture(fd); ///same as above, keep new bullet down and then bring it up

		return slide;
    }

    b2Body* dominos_t::createTrigger() {
	    //b2Vec2 vertices[4];
	    //vertices[0].Set(0,0);
	    //vertices[1].Set(0,-8);
	    //vertices[2].Set(-2,-8);
	    //vertices[3].Set( -6, 0);
	    b2Vec2 vertices[3];
	    vertices[0].Set(2, -5);
	    vertices[1].Set(-2, -11);
	    vertices[2].Set(-4, -1);
	    b2PolygonShape polygonShape;
	    polygonShape.Set(vertices, 3);
	    b2FixtureDef *fd = new b2FixtureDef;
	    fd->shape = &polygonShape;
	    fd->density = 10;
        fd->filter.categoryBits = TRIGGER;
        fd->filter.maskBits = STRIKER;
	    b2BodyDef bd;
	    bd.position.Set(9.0f, 39.25f);
	    bd.type = b2_dynamicBody;
	    bd.angularVelocity = 5.0f;
	    trigger = m_world->CreateBody(&bd);
	    trigger->CreateFixture(fd);

	    return trigger;
    }

    b2Body* dominos_t::createCasing(float x, float y) {
        b2PolygonShape shape;
        shape.SetAsBox(3.75, 1.9);
        b2BodyDef bd;
        bd.position.Set(x, y);
        bd.type = b2_dynamicBody;
        b2Body* aCasing = m_world->CreateBody(&bd);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->density = .1;
        aCasing->CreateFixture(fd);
        shape.SetAsBox(0.25, 1, b2Vec2(4.00, 0), 0);
        fd->shape = &shape;
        aCasing->CreateFixture(fd);
        shape.SetAsBox(0.25, 1.9, b2Vec2(4.5, 0), 0);
        fd->shape = &shape;
        aCasing->CreateFixture(fd);
        aCasing->SetUserData(new int(2));
        fd->filter.categoryBits = CARTRIDGE;
        fd->filter.maskBits = MAGAZINE | STRIKER | BARREL | CARTRIDGE;

        return aCasing;
    }

	b2Body* dominos_t::createBullet(float x, float y) {
        b2PolygonShape shape;
        shape.SetAsBox(1.0f, 0.5f);
        b2BodyDef bd;
        bd.position.Set(x, y);
        bd.type = b2_dynamicBody;
        b2Body* aBullet = m_world->CreateBody(&bd);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->restitution = 1;
        fd->density = 1;
        aBullet->CreateFixture(fd);
        //bullet->SetUserData(new int(2));
        fd->filter.categoryBits = CARTRIDGE;
        fd->filter.maskBits = MAGAZINE | STRIKER | BARREL | CARTRIDGE;
        
        return aBullet;
    }

    b2Body* dominos_t::createStrikerAssembly() {
        b2PolygonShape shape;
        shape.SetAsBox(3.0f, 1.0f);

        b2BodyDef bd;
        bd.position.Set(11.0f, 44.0f);
        bd.type = b2_dynamicBody;
        striker_assembly = m_world->CreateBody(&bd);
        striker_assembly->CreateFixture(&shape, 0.0f);

        b2Vec2 vertices[5];
        vertices[0].Set(-3, -0.7);
        vertices[1].Set(-3, 0.3);
        vertices[2].Set(-4, 0.3);
        vertices[3].Set(-5, -0.2);
        vertices[4].Set(-4, -0.7);
        b2PolygonShape polygonShape;
        polygonShape.Set(vertices, 5);
        striker_assembly->CreateFixture(&polygonShape, 0.0f); ///firing pin

        shape.SetAsBox(2.0f, 0.5f, b2Vec2(5.0f, 0.0f), 0.0f);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->filter.categoryBits = STRIKER;
        fd->filter.maskBits = CARTRIDGE | MAGAZINE | TRIGGER;
        striker_assembly->CreateFixture(fd);
        shape.SetAsBox(6.0f, 1.0f, b2Vec2(13.0f, 0.0f), 0.0f);
        fd->shape = &shape;
        fd->filter.categoryBits = STRIKER;
        fd->filter.maskBits = STRIKER | CARTRIDGE | TRIGGER | BARREL;
        striker_assembly->CreateFixture(fd); ///striker
        shape.SetAsBox(0.5f, 1.0f, b2Vec2(18.5f, -2.0f), 0.0f);
        fd->shape = &shape;
        fd->restitution = 1;
        striker_assembly->CreateFixture(fd); ///nose
        striker_assembly->SetUserData(new int(1));

        return striker_assembly;
    }

    b2Body* dominos_t::createFixed() {
        b2Vec2 vertices[4];
        vertices[0].Set(0, 0);
        vertices[1].Set(1, 0);
        vertices[2].Set(2, -1);
        vertices[3].Set(1, -1);
        b2PolygonShape polygonShape;
        polygonShape.Set(vertices, 4);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &polygonShape;
        //fd->filter.categoryBits = SPRING;
        //fd->filter.maskBits = EVERYTHING;
        fd->density = 10;
        b2BodyDef bd;
        bd.position.Set(0.5, 40);
        //bd.type = b2_dynamicBody;
        fixed = m_world->CreateBody(&bd);
        fixed->CreateFixture(fd); ///sliding_lock

        polygonShape.SetAsBox(0.25f, 1.5f, b2Vec2(-4.75, -1.5), 0); ///right_spring_holder
        fd->shape = &polygonShape;
        fixed->CreateFixture(fd);
        polygonShape.SetAsBox(1.0f, 0.001f, b2Vec2(20, 1.8), 0);
        fd->shape = &polygonShape;
        fd->restitution = 1;
        fd->filter.categoryBits = BARREL;
        fd->filter.maskBits = EVERYTHING | BARREL | TRIGGER | STRIKER;
        fixed->CreateFixture(fd); ///casing_remover
        //polygonShape.SetAsBox(0.001f, 1.0f,b2Vec2(18,1.2),0);
        //fd->shape = &polygonShape;
        //fd->restitution = 1;
        //fixed->CreateFixture(fd); ///
        fixed->SetUserData(new int(3));

        return fixed;
    }

    b2Body* dominos_t::createCap() {
        b2PolygonShape shape;
        shape.SetAsBox(0.25f, 1.5f);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->filter.categoryBits = BARREL;
        fd->filter.maskBits = EVERYTHING | BARREL | STRIKER;
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(14.5f, 44.0f);
        cap = m_world->CreateBody(&bd);
        cap->CreateFixture(fd);
        shape.SetAsBox(0.5f, 1.0f, b2Vec2(0.75f, 0.0f), 0.0f);
        fd->shape = &shape;
        cap->CreateFixture(fd);

        return cap;
    }
    
    b2Body* dominos_t::createSpacerSleeve() {
        b2PolygonShape shape;
        shape.SetAsBox(6.0f, 1.5f);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->filter.categoryBits = STRIKER;
        fd->filter.maskBits = STRIKER | BARREL | CARTRIDGE | TRIGGER;

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(29.0f, 44.0f);
        spacer_sleeve = m_world->CreateBody(&bd);

        spacer_sleeve->CreateFixture(fd);

        return spacer_sleeve;
    }

    b2Body* dominos_t::createGround() {
        b2EdgeShape shape;
        shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));

        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->filter.categoryBits = EVERYTHING;
        fd->filter.maskBits = EVERYTHING;

        b2Body* ground;
        b2BodyDef bd;
        bd.position.Set(0, 0);
        ground = m_world->CreateBody(&bd);
        ground->CreateFixture(fd);

        return ground;
    }

    b2Body* dominos_t::createCartridge(float x, float y) {
        b2Body* casing = dominos_t::createCasing(x, y);
      //  b2Body* bullet = dominos_t::createBullet(x-4.75, y);
        return casing;
    }

    b2Body* dominos_t::createMagazine() {

        float x = 8.0f;
        float y = 28.0f;
        float width = 10.5f;
        float height = 12.0f;
        float wallWidth = 0.5f;


        b2PolygonShape magazineShape;
        magazineShape.SetAsBox(wallWidth, height-4);

        b2BodyDef bd;
        bd.position.Set(x, y);
        bd.type = b2_dynamicBody;
        magazine = m_world->CreateBody(&bd);

        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &magazineShape;
        fd->density = 100;
        magazine->CreateFixture(fd);
        magazineShape.SetAsBox(wallWidth, height, b2Vec2(width, 0), 0);
        fd->shape = &magazineShape;
        magazine->CreateFixture(fd);

        magazineShape.SetAsBox(width/2+3, wallWidth, b2Vec2(width/2, -height), 0);
        fd->shape = &magazineShape;
        magazine->CreateFixture(fd);

        fd->filter.categoryBits = MAGAZINE;
        fd->filter.maskBits = EVERYTHING;


        for (int i = 1; i < 7; ++i)
        {
            dominos_t::createCartridge(x+5, y-height+3.5*i);
        }






        b2BodyDef followerBody;
        followerBody.position.Set(x, y-height+2*wallWidth);
        followerBody.type = b2_dynamicBody;
        follower = m_world->CreateBody(&followerBody);

        b2FixtureDef *followerFixture = new b2FixtureDef;
        b2PolygonShape shape;
        shape.SetAsBox(5.5f, 0.5f, b2Vec2(width/2, 0), 0);
        followerFixture->shape = &shape;
        followerFixture->restitution = 1;
        //followerFixture->density = 1;
        follower->CreateFixture(followerFixture);

        b2PrismaticJointDef prismaticJointDef;
        prismaticJointDef.bodyA = magazine;
        prismaticJointDef.bodyB = follower;
        prismaticJointDef.collideConnected = false;

        prismaticJointDef.localAxisA.Set(0,1);
        prismaticJointDef.localAnchorA.Set(0, -15);
        prismaticJointDef.localAnchorB.Set(0, 0);

        prismaticJointDef.enableLimit = true;
        prismaticJointDef.lowerTranslation = 0;
        prismaticJointDef.upperTranslation = 29;
        
        prismaticJointDef.enableMotor = true;
        prismaticJointDef.maxMotorForce = 200;
        prismaticJointDef.motorSpeed = 150;

        (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef );




        
        return magazine;
    }



    /** 
    * Connects the striker assembly and the spacer sleeve so that they can only move horizontal
    */
    void dominos_t::connectStrikerAssemblyWithSpacerSleeve(b2Body* striker_assembly, b2Body* spacer_sleeve) {
        b2PrismaticJointDef prismaticJointDef;
        prismaticJointDef.collideConnected = true;
        prismaticJointDef.enableLimit = true;
        prismaticJointDef.upperTranslation = 201;
        prismaticJointDef.localAxisA.Set(1, 0);

        prismaticJointDef.bodyA = striker_assembly;
        prismaticJointDef.bodyB = spacer_sleeve;
        (b2PrismaticJoint*) m_world->CreateJoint(&prismaticJointDef);
    }

    void dominos_t::connectStrikerAssemblyWithCap(b2Body* striker_assembly, b2Body* cap) {
        b2PrismaticJointDef prismaticJointDef;
        prismaticJointDef.collideConnected = true;
        prismaticJointDef.enableLimit = true;
        prismaticJointDef.upperTranslation = 201;
        prismaticJointDef.localAxisA.Set(1, 0);

        prismaticJointDef.bodyA = striker_assembly;
        prismaticJointDef.bodyB = cap;
        (b2PrismaticJoint*) m_world->CreateJoint(&prismaticJointDef); ///striker_assembly and cap
    }

    void dominos_t::connectSpacerSleeveWithCap(b2Body* spacer_sleeve, b2Body* cap) {
        b2PrismaticJointDef prismaticJointDef;
        prismaticJointDef.collideConnected = true;
        prismaticJointDef.enableLimit = true;
        prismaticJointDef.upperTranslation = 201;
        prismaticJointDef.localAxisA.Set(1, 0);

        //prismaticJointDef.enableMotor = true;
        //prismaticJointDef.maxMotorForce = 500; ///spring action
        //prismaticJointDef.motorSpeed = -sqrt(100 - (14.75 + cap->GetPosition().x - spacer_sleeve->GetPosition().x)*(cap->GetPosition().x - spacer_sleeve->GetPosition().x));
        prismaticJointDef.bodyA = spacer_sleeve;
        prismaticJointDef.bodyB = cap;
        prismaticJointDef.lowerTranslation = -14.5; ///stop spring right before it reaches natural length
        (b2PrismaticJoint*) m_world->CreateJoint(&prismaticJointDef); ///cap and spacer_sleeve
    }

    void dominos_t::connectSlideWithFixed(b2Body* slide, b2Body* fixed) {
        b2PrismaticJointDef prismaticJointDef;
        prismaticJointDef.collideConnected = true;
        prismaticJointDef.enableLimit = true;
        prismaticJointDef.upperTranslation = 201;
        prismaticJointDef.localAxisA.Set(1, 0);

        prismaticJointDef.bodyA = slide;
        prismaticJointDef.bodyB = fixed;
        prismaticJointDef.lowerTranslation = -14.5; ///stop spring right before it reaches natural length
        prismaticJointDef.localAnchorA.Set(21.25, -7.5);
        prismaticJointDef.localAxisA.Set(1, 0);
        (b2PrismaticJoint*) m_world->CreateJoint(&prismaticJointDef); ///slide and fixed
    }

    /** Pulls trigger
    * 
    */
    void dominos_t::connectTriggerWithGround(b2Body* trigger, b2Body* ground) {
        b2RevoluteJointDef jointDef;
        jointDef.bodyA = trigger;
        jointDef.bodyB = ground;

        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.55;
        //jointDef.upperAngle =  -100;
        jointDef.localAnchorA.Set(-4, -1);
        jointDef.localAnchorB.Set(5, 38.25);

        (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
    }

    /** Connects trigger and bar
    *
    */
    void dominos_t::connectTriggerWithBar(b2Body* trigger, b2Body* bar) {
        b2RevoluteJointDef jointDef;
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.55;
        //jointDef.upperAngle =  -100;
        jointDef.bodyA = trigger;
        jointDef.bodyB = bar;
        jointDef.localAnchorA.Set(-1, -6);
        jointDef.localAnchorB.Set(-10.75, -3);

        (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
    }
    
    void dominos_t::connectGroundWithBar(b2Body* ground, b2Body* bar) {
        // Joint
        b2WheelJointDef wheelJointDef;
        /// From Box2D source code:  A line joint. This joint provides one degree of freedom: translation along an axis fixed
        /// in body1. You can use a joint limit to restrict the range of motion and a joint motor to drive the motion 
        /// or to model joint friction.
        //wheelJointDef.Initialize(trigger, bar, bar->GetWorldCenter(), *worldAxis);
        wheelJointDef.localAxisA = b2Vec2(2, -1);
        wheelJointDef.localAxisA.Normalize();
        wheelJointDef.bodyA = ground;
        wheelJointDef.bodyB = bar;
        wheelJointDef.localAnchorA.Set(30, 39.25);
        wheelJointDef.localAnchorB.Set(11.25, 3);
        wheelJointDef.frequencyHz = 0;

        (b2WheelJoint*) m_world->CreateJoint(&wheelJointDef);
    }

    void dominos_t::connectSlideWithSpacerSleeve(b2Body* slide, b2Body* spacer_sleeve) {
        b2WeldJointDef weldJointDef;
        weldJointDef.localAnchorA.Set(49.75, -3.5);
        weldJointDef.bodyA = slide;
        weldJointDef.bodyB = spacer_sleeve;
        (b2WeldJoint*) m_world->CreateJoint(&weldJointDef); ///slide and spacer_sleeve
    }

    void dominos_t::connectMagazine(b2Body* left, b2Body* bottom, b2Body* right) {
    }


    void dominos_t::step(settings_t* settings) {
        base_sim_t::step(settings);
        if (coll) {
            coll = false;
            bullet->ApplyLinearImpulse(b2Vec2(-5000000, 0), bullet->GetWorldCenter(), true);
            casing->ApplyLinearImpulse(b2Vec2(3000, 0), casing->GetWorldCenter(), true);
            slide->ApplyLinearImpulse(b2Vec2(5000000, 0), slide->GetWorldCenter(), true); ///provides impulses to the bullet, casing and slide when striker contacts casing
        }
        else if (loll) {
            loll = false;
            casing->ApplyLinearImpulse(b2Vec2(0, 10000), casing->GetWorldCenter(), true); ///removes empty casing during recoil
        }
        //cout<<striker_assembly->GetPosition().x - bar->GetPosition().x<<endl;
        if ((trig_reset) && (striker_assembly->GetPosition().x - bar->GetPosition().x > 7)) {
            //cout<<"dude"<<endl;
            trig_reset = false;
            trigger->ApplyAngularImpulse(-15000, true); ///resets the position of trigger during recoil
        }

        //slidex = slide->GetPosition().x;
        //fixedx = fixed->GetPosition().x;
        //int slidev = slide->GetLinearVelocity().x;

        if (-21.25 + fixed->GetPosition().x - slide->GetPosition().x > 0.001) {
            slide->SetLinearVelocity(b2Vec2(0, 0));
            slide->SetTransform(b2Vec2(-21.25 + fixed->GetPosition().x, slide->GetPosition().y), 0); ///ensures that the spring between fixed and slide does not keep on oscillating
        }
        slide->ApplyForce(b2Vec2(1000000 * (-21.25 + fixed->GetPosition().x - slide->GetPosition().x), 0), slide->GetWorldCenter(), true); ///spring between fixed and slide
        //fixed->ApplyForce(b2Vec2(1000000 * (21.25 + slide->GetPosition().x - fixed->GetPosition().x), 0), fixed->GetWorldCenter(), true);///spring between fixed and slide

        spacer_sleeve->ApplyForce(b2Vec2(1000 * (14.75 + cap->GetPosition().x - spacer_sleeve->GetPosition().x), 0), spacer_sleeve->GetWorldCenter(), true); ///spring between cap and spacer_sleeve
        cap->ApplyForce(b2Vec2(1000 * (-14.75 + spacer_sleeve->GetPosition().x - cap->GetPosition().x), 0), cap->GetWorldCenter(), true); ///spring between cap and spacer_sleeve
    }


    void MyContactListener::BeginContact(b2Contact* contact) {
        //check if fixture A was a ball
        void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
        int* a = (int*) (bodyUserDataA);
        int* b = (int*) (bodyUserDataB);

        if ((a != NULL) && (b != NULL)) { ///striker strikes casing
            if (((*a == 1) && (*b == 2)) || ((*a == 2) && (*b == 1))) {
                coll = true;
            } else if (((*a == 3) && (*b == 2)) || ((*a == 2) && (*b == 3))) { ///casing hits casing_remover
                loll = true;
            }
        }
    }

    // Startes finally the actual Simulation 
    sim_t *sim = new sim_t("Glock 23 Simulation", dominos_t::create);
}
