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

    /*! Constructor of our World.
     * 
     * This constructor initializes all objects and joints.
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
        casing = dominos_t::createCasing();
        bullet = dominos_t::createBullet();
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

    /**
     * Creates and returns the barrel of the gun. 
     */
    b2Body* dominos_t::createBarrel() { 
        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(14, 0.5);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &polygonShape;
        //fd->filter.categoryBits = SPRING;
        //fd->filter.maskBits = EVERYTHING;
        fd->density = 20;
        b2BodyDef bd;
        bd.position.Set(-21.0f, 36.5f);
        bd.type = b2_dynamicBody;
        //bd.linearVelocity=b2Vec2(10,0);
        barrel = m_world->CreateBody(&bd);
        barrel->CreateFixture(fd);
        polygonShape.SetAsBox(6, 1, b2Vec2(20, 0.5), 0);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd);

        double old_friction = fd->friction;
        fd->friction = 100;
        
        // Upper stopper
        b2Vec2 vertices2[3];
        vertices2[0].Set(17.5, -0.57);
        vertices2[1].Set(23, -0.5);
        vertices2[2].Set(17.5, -0.5);
        polygonShape.Set(vertices2, 3);
//        polygonShape.SetAsBox(.001, .05, b2Vec2(17.5, -.5), 0);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd);
        
        // lower stopper
        b2Vec2 vertices3[3];
        vertices3[0].Set(17.5, -4.4);
        vertices3[1].Set(23, -4.5);
        vertices3[2].Set(17.5, -4.5);
        polygonShape.Set(vertices3, 3);
//        polygonShape.SetAsBox(.001, .05, b2Vec2(17.5, -4.5), 0);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd);

        fd->friction = old_friction;

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
        //fd->friction = 1;
        barrel->CreateFixture(fd); ///the hole where sliding_lock fits
        b2Vec2 vertic[5];
        vertic[0].Set(20, -4.5);
        vertic[1].Set(23, -4.5);
        vertic[2].Set(25, -5.5);
        vertic[3].Set(26, -6.5);
        vertic[4].Set(22, -6.5);
        polygonShape.Set(vertic, 5);
        fd->shape = &polygonShape;
        barrel->CreateFixture(fd);

        return barrel;
    }
    
    /**
     * Creates and returns the bar of the striker. 
     */
    b2Body* dominos_t::createBar() {
        b2PolygonShape shape;
        shape.SetAsBox(12.0f, 0.75f, b2Vec2(0, 0), 0.25f);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->filter.categoryBits = SPRING;                                   ///
        //fd->filter.maskBits = EVERYTHING;
        fd->density = 10;
        b2BodyDef bd;
        bd.position.Set(18.75f, 26.25f);
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
    
	/**
	 * Creates and returns the slide of the gun. 
	 */
	
    b2Body* dominos_t::createSlide() {
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(13.75, 0.5);
		b2FixtureDef *fd = new b2FixtureDef;
		fd->shape = &polygonShape;
		//fd->filter.categoryBits = SPRING;
		//fd->filter.maskBits = EVERYTHING;
		fd->density = 2000;
		b2BodyDef bd;
		bd.position.Set(-20.75f, 37.5f);
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
		polygonShape.SetAsBox(0.001, 0.43, b2Vec2(-12.75, -7), 0);
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
		fd->filter.categoryBits = COMPRES;
		fd->filter.maskBits = EVERYTHING | NOCAP | SPRING | CARTRIDGE;
		fd->density = 0;
		slide->CreateFixture(fd); ///for pushing the new bullet in the mag, so that it springs up after some time (interface between magazine and other parts of the gun)
		polygonShape.SetAsBox(5, 0.5, b2Vec2(36.875, -5.7), 0);
		fd->shape = &polygonShape;
		slide->CreateFixture(fd); ///same as above, keep new bullet down and then bring it up
		
		polygonShape.SetAsBox(0.5, 1.25, b2Vec2(26.35, -6.25), 0);
		fd->shape = &polygonShape;
		slide->CreateFixture(fd);
		return slide;
    }

	/**
	 * Creates and returns the trigger of the gun. 
	 */
	
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
        fd->filter.categoryBits = SPRING;
        //fd->filter.maskBits = EVERYTHING;
        fd->density = 10;
        b2BodyDef bd;
        bd.position.Set(9.0f, 29.25f);
        bd.type = b2_dynamicBody;
        bd.angularVelocity = 5.0f;
        trigger = m_world->CreateBody(&bd);
        trigger->CreateFixture(fd);

        return trigger;
    }


    /**
     * Creates and returns the casing of the ith bullet at the given co-ordinates.
     * @param float x the x co-ordinate of the casing.
     * @param flaot y the y co-ordinate of the casing.
     * @param int i the index of the bullet whose casing needs to be created. 
     */
    b2Body* dominos_t::createCasing(float x, float y, int i) {
        b2PolygonShape shape;
        shape.SetAsBox(3.75, 1.9);
        b2BodyDef bd;
        bd.position.Set(x, y);
        bd.type = b2_dynamicBody;
        new_casing[i] = m_world->CreateBody(&bd);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->filter.categoryBits = CARTRIDGE;
        fd->filter.maskBits = EVERYTHING | CARTRIDGE | MAGAZINE | COMPRES;
        fd->shape = &shape;
        fd->density = 10;
        //fd->restitution = 0;
        new_casing[i]->CreateFixture(fd);
        shape.SetAsBox(0.25, 1, b2Vec2(4.00, 0), 0);
        fd->shape = &shape;
        new_casing[i]->CreateFixture(fd);
        shape.SetAsBox(0.25, 1.9, b2Vec2(4.5, 0), 0);
        fd->shape = &shape;
        new_casing[i]->CreateFixture(fd);
        new_casing[i]->SetUserData(new int(i));

        return new_casing[i];
    }
	
	/**
	 * Creates and returns the ith bullet at the given co-ordinates.
	 * @param float x the x co-ordinate of the bullet.
	 * @param flaot y the y co-ordinate of the bullet.
	 * @param int i the index of the bullet which needs to be created. 
	 */
	
	b2Body* dominos_t::createBullet(float x, float y, int i) {
        //b2PolygonShape shape;
        //shape.SetAsBox(1.0f, 0.5f);
        b2Vec2 vertices[6];
        vertices[0].Set(1,1.9);
        vertices[1].Set(0.5, 1.5);
        vertices[2].Set(0, 0.5);
        vertices[3].Set(0,-0.5);
        vertices[4].Set(0.5, -1.5);
        vertices[5].Set(1, -1.9);
        b2PolygonShape shape;
        shape.Set(vertices, 6);
        b2BodyDef bd;
        bd.position.Set(x, y);
        bd.type = b2_dynamicBody;
        new_bullet[i] = m_world->CreateBody(&bd);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        //fd->restitution = 1;
        fd->filter.categoryBits = CARTRIDGE;
        fd->filter.maskBits = EVERYTHING | CARTRIDGE | MAGAZINE | COMPRES;
        fd->density = 1;
        new_bullet[i]->CreateFixture(fd);
        //new_bullet[i]->SetUserData(new int(i));
        
        return new_bullet[i];
    }

    /**
     * Creates and returns the striker assembly. 
     */
    b2Body* dominos_t::createStrikerAssembly() {
        b2PolygonShape shape;
        shape.SetAsBox(3.0f, 1.0f);

        b2BodyDef bd;
        bd.position.Set(11.0f, 34.0f);
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
        fd->filter.categoryBits = NOCAP;
        fd->filter.maskBits = EVERYTHING;
        striker_assembly->CreateFixture(fd);
        shape.SetAsBox(6.0f, 1.0f, b2Vec2(13.0f, 0.0f), 0.0f);
        fd->shape = &shape;
        fd->filter.categoryBits = SPRING;
        fd->filter.maskBits = EVERYTHING | SPRING;
        striker_assembly->CreateFixture(fd); ///striker
        shape.SetAsBox(0.5f, 1.0f, b2Vec2(18.5f, -2.0f), 0.0f);
        fd->shape = &shape;
        //fd->restitution = 1;
        striker_assembly->CreateFixture(fd); ///nose
        striker_assembly->SetUserData(new int(1));

        return striker_assembly;
    }

    /**
     * Creates and returns the body corresponding to the fixed objects in the simulation. 
     */
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
        bd.position.Set(0.5, 30);
        //bd.type = b2_dynamicBody;
        fixed = m_world->CreateBody(&bd);
        fixed->CreateFixture(fd); ///sliding_lock

        polygonShape.SetAsBox(0.25f, 1.5f, b2Vec2(-4.75, -1.5), 0); ///right_spring_holder
        fd->shape = &polygonShape;
        fixed->CreateFixture(fd);
        polygonShape.SetAsBox(1.0f, 0.001f, b2Vec2(19, 1.8), 0);
        fd->shape = &polygonShape;
        fd->restitution = 1;
        fd->filter.categoryBits = COMPRES;
        fd->filter.maskBits = EVERYTHING | NOCAP | SPRING | CARTRIDGE;
        fixed->CreateFixture(fd); ///casing_remover
        //polygonShape.SetAsBox(0.001f, 1.0f,b2Vec2(18,1.2),0);
        //fd->shape = &polygonShape;
        //fd->restitution = 1;
        //fixed->CreateFixture(fd); ///
        fixed->SetUserData(new int(3));

        return fixed;
    }

    /**
     * Creates and returns the cap of the gun. 
     */
    b2Body* dominos_t::createCap() {
        b2PolygonShape shape;
        shape.SetAsBox(0.25f, 1.5f);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->filter.categoryBits = SPRING;
        fd->filter.maskBits = EVERYTHING | SPRING;
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(14.5f, 34.0f);
        cap = m_world->CreateBody(&bd);
        cap->CreateFixture(fd);
        shape.SetAsBox(0.5f, 1.0f, b2Vec2(0.75f, 0.0f), 0.0f);
        fd->shape = &shape;
        cap->CreateFixture(fd);

        return cap;
    }
      
    /**
     * Creates and returns the spacer sleeve of the striker assembly. 
     */
    b2Body* dominos_t::createSpacerSleeve() {
        b2PolygonShape shape;
        shape.SetAsBox(6.0f, 1.5f);
        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->filter.categoryBits = SPRING;
        fd->filter.maskBits = EVERYTHING;
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(29.0f, 34.0f);
        spacer_sleeve = m_world->CreateBody(&bd);
        spacer_sleeve->CreateFixture(fd);

        return spacer_sleeve;
    }
    /**
     * Creates and returns the ground in the simulation. 
     */
    b2Body* dominos_t::createGround() {
        b2EdgeShape shape;
        shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));

        b2FixtureDef *fd = new b2FixtureDef;
        fd->shape = &shape;
        fd->filter.categoryBits = SPRING;
        fd->filter.maskBits = EVERYTHING;

        b2Body* ground;
        b2BodyDef bd;
        ground = m_world->CreateBody(&bd);
        ground->CreateFixture(fd);

        return ground;
    }
    
    /**
     * Creates and bullet-casing catridge by calling the createCasing and createBullet functions and returns the casing object.
     * @param float x the x co-ordinate of the catridge.
     * @param float y the y co-ordinate of the catridge.
     * @param int i the index of the bullet.
     */
    b2Body* dominos_t::createCartridge(float x, float y, int i) {
        new_casing[i] = dominos_t::createCasing(x+1.3f, y, i);
        new_bullet[i] = dominos_t::createBullet(x-3.5f, y, i);
        
        b2WeldJointDef jointDef;
        jointDef.bodyA = new_casing[i];
        jointDef.bodyB = new_bullet[i];
        jointDef.localAnchorA.Set(-4.8,0);
        (b2WeldJoint*)m_world->CreateJoint(&jointDef);
        
        return new_casing[i];
    }
    
    /**
     * Creates and returns the magazine of the gun by calling the createCatridge function multilpe times. 
     */
    b2Body* dominos_t::createMagazine() {

        float x = 8.0f;
        float y = 28.0f;
        float width = 10.5f;
        float height = 12.0f;
        float wallWidth = 0.5f;


        b2PolygonShape magazineShape;
        magazineShape.SetAsBox(wallWidth, height-1, b2Vec2(1,-1),0);

        b2BodyDef bd;
        bd.position.Set(x, y-10.0f);
        bd.type = b2_dynamicBody;
        magazine = m_world->CreateBody(&bd);

        b2FixtureDef *fd = new b2FixtureDef;
        fd->filter.categoryBits = MAGAZINE;
        fd->filter.maskBits = EVERYTHING | CARTRIDGE;
        fd->shape = &magazineShape;
        fd->density = 100;
        magazine->CreateFixture(fd);
        magazineShape.SetAsBox(2*wallWidth, height+0.5f, b2Vec2(wallWidth+width+1.2, 0.5), 0);
        fd->shape = &magazineShape;
        magazine->CreateFixture(fd);

        magazineShape.SetAsBox(width/2+3, wallWidth, b2Vec2(width/2, -height), 0);
        fd->shape = &magazineShape;
        magazine->CreateFixture(fd);



        for (int i = 1; i < 7; ++i)
        {
            dominos_t::createCartridge(x+5, y-height+3.8*i-9.8, i);
        }






        b2BodyDef followerBody;
        followerBody.position.Set(x, y-height+2*wallWidth-10);
        followerBody.type = b2_dynamicBody;
        follower = m_world->CreateBody(&followerBody);

        b2FixtureDef *followerFixture = new b2FixtureDef;
        b2PolygonShape shape;
        shape.SetAsBox(4.8f, 0.5f, b2Vec2(1+width/2, 0), 0);
        followerFixture->shape = &shape;
        //followerFixture->restitution = 1;
        //followerFixture->density = 1;
        follower->CreateFixture(followerFixture);

        b2PrismaticJointDef prismaticJointDef;
        prismaticJointDef.bodyA = magazine;
        prismaticJointDef.bodyB = follower;
        prismaticJointDef.collideConnected = true;

        prismaticJointDef.localAxisA.Set(0,1);
        prismaticJointDef.localAnchorA.Set(0, -15);
        prismaticJointDef.localAnchorB.Set(0, 0);

        prismaticJointDef.enableLimit = true;
        prismaticJointDef.lowerTranslation = 0;
        prismaticJointDef.upperTranslation = 29;
        
        prismaticJointDef.enableMotor = true;
        prismaticJointDef.maxMotorForce = 200000;
        prismaticJointDef.motorSpeed = 20;

        (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef );

        b2WeldJointDef jointDef;
        jointDef.bodyA = fixed;
        jointDef.bodyB = magazine;
        jointDef.localAnchorA.Set(7.5,-12);
        (b2WeldJoint*)m_world->CreateJoint(&jointDef);


        
        return magazine;
    }

    /**
     * Creates the joint for connecting the Striker Assembly and the Spacer Sleeve.
     * @param b2Body* striker_assembly the Striker Assembly Object.
     * @param b2Body* spacer_sleeve the Spacer Sleeve Object.
     */
    void dominos_t::connectStrikerAssemblyWithSpacerSleeve(b2Body* striker_assembly, b2Body* spacer_sleeve) {
        // Connects the striker assembly and the spacer sleeve so that they can only move horizontal
        b2PrismaticJointDef prismaticJointDef;
        prismaticJointDef.collideConnected = true;
        prismaticJointDef.enableLimit = true;
        prismaticJointDef.upperTranslation = 201;
        prismaticJointDef.localAxisA.Set(1, 0);

        prismaticJointDef.bodyA = striker_assembly;
        prismaticJointDef.bodyB = spacer_sleeve;
        (b2PrismaticJoint*) m_world->CreateJoint(&prismaticJointDef);
    }

    /**
     * Creates the joint for connecting the Striker Assembly and the Cap.
     * @param b2Body* striker_assembly the Striker Assembly Object.
     * @param b2Body* cap the Cap Object.
     */
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

    /**
     * Creates the joint for connecting the Spacer Sleeve and the Cap.
     * @param b2Body* spacer_sleeve the Spacer Sleeve Object.
     * @param b2Body* cap the Cap Object
     */
    
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


	/**
	 * Creates the joint for connecting the Slide and the Fixed objects.
	 * @param b2Body* slide the Slide Object.
	 * @param b2Body* fixed the Fixed Object
	 */
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

	/**
	 * Creates the joint for connecting the Trigger and the Ground.
	 * @param b2Body* trigger the Trigger Object.
	 * @param b2Body* ground the Ground Object
	 */
    void dominos_t::connectTriggerWithGround(b2Body* trigger, b2Body* ground) {
        // Pulls trigger
        b2RevoluteJointDef jointDef;
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.55;
        //jointDef.upperAngle =  -100;
        jointDef.localAnchorA.Set(-4, -1);
        jointDef.localAnchorB.Set(5, 28.25);

        jointDef.bodyA = trigger;
        jointDef.bodyB = ground;
        (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
    }

	/**
	 * Creates the joint for connecting the Trigger and the Bar.
	 * @param b2Body* trigger the Trigger Object.
	 * @param b2Body* bar the Bar Object
	 */
    void dominos_t::connectTriggerWithBar(b2Body* trigger, b2Body* bar) {
        //Connects trigger and bar
        b2RevoluteJointDef jointDef;
        jointDef.enableLimit = true;
        jointDef.lowerAngle = -0.55;
        //jointDef.upperAngle =  -100;
        jointDef.localAnchorA.Set(-1, -6);
        jointDef.localAnchorB.Set(-10.75, -3);

        jointDef.bodyA = trigger;
        jointDef.bodyB = bar;
        (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
    }

    /**
     * Creates the joint for connecting the Ground and the Bar.
     * @param b2Body* ground the Ground Object.
     * @param b2Body* bar the Bar Object
     */
    void dominos_t::connectGroundWithBar(b2Body* ground, b2Body* bar) {
        // Joint
        b2WheelJointDef wheelJointDef;
        /// From Box2D source code:  A line joint. This joint provides one degree of freedom: translation along an axis fixed
        /// in body1. You can use a joint limit to restrict the range of motion and a joint motor to drive the motion 
        /// or to model joint friction.
        //wheelJointDef.Initialize(trigger, bar, bar->GetWorldCenter(), *worldAxis);
        wheelJointDef.localAxisA = b2Vec2(2, -1);
        wheelJointDef.localAxisA.Normalize();
        wheelJointDef.localAnchorA.Set(30, 29.25);
        wheelJointDef.localAnchorB.Set(11.25, 3);
        wheelJointDef.frequencyHz = 0;

        wheelJointDef.bodyA = ground;
        wheelJointDef.bodyB = bar;
        (b2WheelJoint*) m_world->CreateJoint(&wheelJointDef);
    }


    /**
     * Creates the joint for connecting the Slide and the Spacer Sleeve.
     * @param b2Body* slide the Slide Object.
     * @param b2Body* spacer_sleeve the Spacer Sleeve Object
     */
    void dominos_t::connectSlideWithSpacerSleeve(b2Body* slide, b2Body* spacer_sleeve) {
        b2WeldJointDef weldJointDef;
        weldJointDef.localAnchorA.Set(49.75, -3.5);
        weldJointDef.bodyA = slide;
        weldJointDef.bodyB = spacer_sleeve;
        (b2WeldJoint*) m_world->CreateJoint(&weldJointDef); ///slide and spacer_sleeve
    }
    
    void dominos_t::connectMagazine(b2Body* left, b2Body* bottom, b2Body* right) {
    }

	/**
	 * The Step function which manipulates the positions  and velocitys based on certain conditions.
	 * @param settings_t* settings The Settings of the simulation.
	 */
    void dominos_t::step(settings_t* settings) {
        base_sim_t::step(settings);
        if (coll) {
            coll = false;
            bullet->ApplyLinearImpulse(b2Vec2(-5000000, 0), bullet->GetWorldCenter(), true);
            casing->ApplyLinearImpulse(b2Vec2(30000, 0), casing->GetWorldCenter(), true);
            slide->ApplyLinearImpulse(b2Vec2(10000000, 0), slide->GetWorldCenter(), true); ///provides impulses to the bullet, casing and slide when striker contacts casing
        }
        else if (loll) {
            loll = false;
            casing->ApplyLinearImpulse(b2Vec2(0, 100000), casing->GetWorldCenter(), true); ///removes empty casing during recoil
        }
        //cout<<striker_assembly->GetPosition().x - bar->GetPosition().x<<endl;
        if (trig_reset || (striker_assembly->GetPosition().x - bar->GetPosition().x > 7)) {
           // cout<<"dude"<<endl;
            trig_reset = false;
  //          trigger->ApplyAngularImpulse(-15000, true); ///resets the position of trigger during recoil
        }
        //cout<<new_bullet[6]->GetPosition().y - slide->GetPosition().y + 4.75<<endl;
        if (magnet && (new_casing[6]->GetPosition().y > slide->GetPosition().y - 3.75)) {
            magnet = false;
            new_casing[6]->SetLinearVelocity(b2Vec2(0,0));
            new_casing[6]->SetTransform(b2Vec2(0.25+slide->GetPosition().x+20.75,34+slide->GetPosition().y-37.5), 0);
            
        }

            trigger->ApplyTorque(-13000, true); ///resets the position of trigger during recoil
        double pos_dif = striker_assembly->GetPosition().x - bar->GetPosition().x;
        if(triggered) {
            trigger->ApplyTorque(100000, true); ///resets the position of trigger during recoil
            if(pos_dif < -9) {
               // trigger->ApplyTorque(15000000, true); ///resets the position of trigger during recoil
                triggered = false;
                }
            }


        if (-21.25 + fixed->GetPosition().x - slide->GetPosition().x > 0.001) {
            slide->SetLinearVelocity(b2Vec2(0, 0));
            slide->SetTransform(b2Vec2(-21.25 + fixed->GetPosition().x, slide->GetPosition().y), 0); ///ensures that the spring between fixed and slide does not keep on oscillating
        }
        slide->ApplyForce(b2Vec2(2000000 * (-21.25 + fixed->GetPosition().x - slide->GetPosition().x), 0), slide->GetWorldCenter(), true); ///spring between fixed and slide
        //fixed->ApplyForce(b2Vec2(1000000 * (21.25 + slide->GetPosition().x - fixed->GetPosition().x), 0), fixed->GetWorldCenter(), true);///spring between fixed and slide

        spacer_sleeve->ApplyForce(b2Vec2(1000 * (14.75 + cap->GetPosition().x - spacer_sleeve->GetPosition().x), 0), spacer_sleeve->GetWorldCenter(), true); ///spring between cap and spacer_sleeve
        cap->ApplyForce(b2Vec2(1000 * (-14.75 + spacer_sleeve->GetPosition().x - cap->GetPosition().x), 0), cap->GetWorldCenter(), true); ///spring between cap and spacer_sleeve
    }

	/**
	 * The callback function for collision which takes certain actions depending on which bodies have collided.
	 * @param b2Contact* contact the Contact Object contaning information about the current collision.
	 */
    void MyContactListener::BeginContact(b2Contact* contact) {
        //check if fixture A was a ball
        void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();
        int* a = (int*) (bodyUserDataA);
        int* b = (int*) (bodyUserDataB);

        if ((a != NULL) && (b != NULL)) { ///striker strikes casing
            if (((*a == 1) && (*b == 0)) || ((*a == 0) && (*b == 1))) {
                coll = true;
            } else if (((*a == 3) && (*b == 0)) || ((*a == 0) && (*b == 3))) { ///casing hits casing_remover
                loll = true;
                cout<<"dick"<<endl;
            }
            
            //if ((*a == 0) || (*b == 0)) cout<<"bsdkf"<<endl;
      
        }
    }

        /// Starts the actual simulation. 
    sim_t *sim = new sim_t("Glock 23 Simulation", dominos_t::create);
}
