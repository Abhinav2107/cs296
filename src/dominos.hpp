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
#include "cs296_base.hpp"
using namespace std;
#ifndef _DOMINOS_HPP_
#define _DOMINOS_HPP_

namespace cs296 {
    bool coll = false;
    bool loll = false;
    bool magnet = false;
    //bool trig_reset = true;

    /**
    *
    */
    // What is this for?
    class MyContactListener : public b2ContactListener {
    public:
        void BeginContact(b2Contact* contact);
    };

    /** This is the class that sets up the Box2D simulation world
    * Contains all Elements in the world and...?
	*/
    class dominos_t : public base_sim_t {
    public:
        MyContactListener myContactListenerInstance;
        b2Body* cap;
        b2Body* spacer_sleeve;
        b2Body* slide;
        b2Body* fixed;
        b2Body* striker_assembly;
        b2Body* bullet;
        b2Body* casing;
        b2Body* barrel;
        b2Body* bar;
        b2Body* trigger;
        b2Body* magazine;
        b2Body* follower;
        b2Body* new_bullet[7];
        b2Body* new_casing[7];
        
        bool trig_reset;
        enum _entityCategory {
            EVERYTHING = 0x0001,
            SPRING = 0x0002,
            NOCAP = 0x0003,
            COMPRES = 0x0004,
            BARREL = 4,
            MAGAZINE = 8,
            CARTRIDGE = 16,
            FIREMECHANICS = 32,
            TRIGGER = 64
        };

    	/**
    	*
    	*/
    	dominos_t();

	    /** Performs a single step in the simulation
	    * Extends the standard step-Function by...?
	    */
        void step(settings_t* settings);
        
    	/**
    	*
    	*/
    	static base_sim_t* create() { return new dominos_t; }
        
    private:

	    /** Creates the Ground of the Simulation
	    * The ground has an EdgeShape between co-ordinates (-90, 0) and (90, 0).
	     */
    	b2Body* createGround();
   	
	    /** Creates the Barrel
	    * The Barrel is basicly a pipe where the bullet flies trough when fired.
	    */
    	b2Body* createBarrel();
    	b2Body* createBar();
    	b2Body* createSlide();
    	b2Body* createTrigger();
    	b2Body* createCasing(float x = 0.25, float y = 34, int i = 0);
    	b2Body* createBullet(float x = -4.5, float y = 34, int i = 0);
		
		/** Striker Assembly consisting of Firing pin, striker, nose
		 * Make a prismatic joint between striker and spring, striker and cap
		 */
    	b2Body* createStrikerAssembly();
    	b2Body* createFixed();
    	b2Body* createCap();
    	b2Body* createSpacerSleeve();
    	b2Body* createMagazine();
    	b2Body* createCartridge(float x, float y, int i);

    	void connectStrikerAssemblyWithSpacerSleeve(b2Body* striker_assembly, b2Body* spacer_sleeve);
    	void connectStrikerAssemblyWithCap(b2Body* striker_assembly, b2Body* cap);
    	void connectSpacerSleeveWithCap(b2Body* spacer_sleeve, b2Body* cap);
    	void connectSlideWithFixed(b2Body* slide, b2Body* fixed);
    	void connectTriggerWithGround(b2Body* trigger, b2Body* ground);
    	void connectTriggerWithBar(b2Body* trigger, b2Body* bar);
    	void connectGroundWithBar(b2Body* ground, b2Body* bar);
    	void connectSlideWithSpacerSleeve(b2Body* slide, b2Body* spacer_sleeve);
    	void connectMagazine(b2Body* left, b2Body* bottom, b2Body* right);
    };
}

#endif
