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

namespace cs296
{
	//dominos_t::b2Body* cap;
	//dominos_t::b2Body* spacer_sleeve;
  /**  This is the constructor \n
   * This is the documentation block for the constructor.
   */
  dominos_t::dominos_t()
  {
	  m_world->SetContactListener(&myContactListenerInstance);
	  trig_reset = true;
	    enum _entityCategory {
    EVERYTHING =          0x0001,
    SPRING =     0x0002,
    NOCAP = 0x0003,
    COMPRES = 0x0004,
  };
	  
	  //Ground
   /** The ground has an EdgeShape between co-ordinates (-90, 0) and (90, 0).
     * 
     */ 
    b2Body* b1;
    {
      
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
      b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &shape;
      fd->filter.categoryBits = SPRING;
	  fd->filter.maskBits = EVERYTHING;
      b2BodyDef bd; 
      b1 = m_world->CreateBody(&bd); 
      b1->CreateFixture(fd);
    }
    
	
	  
	//Striker Assembly
	/** Firing pin, striker, nose
	 * make a prismatic joint between striker and spring, striker and cap
	 */
	 //b2Body* striker_assembly;
	 {
		 b2PolygonShape shape;
		 shape.SetAsBox(3.0f, 1.0f);
	
		 b2BodyDef bd;
         bd.position.Set(11.0f, 24.0f);
         bd.type = b2_dynamicBody;
         striker_assembly = m_world->CreateBody(&bd);
         striker_assembly->CreateFixture(&shape, 0.0f);
         
         
         b2Vec2 vertices[5];
		 vertices[0].Set(-3,  -0.7);
		 vertices[1].Set(-3,  0.3);
		 vertices[2].Set( -4, 0.3);
		 vertices[3].Set(-5,  -0.2);
		 vertices[4].Set( -4, -0.7);
		 b2PolygonShape polygonShape;
		 polygonShape.Set(vertices, 5);
		 striker_assembly->CreateFixture(&polygonShape, 0.0f); ///firing pin

		 shape.SetAsBox(2.0f, 0.5f, b2Vec2(5.0f,0.0f),0.0f);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &shape;
		 fd->filter.categoryBits = NOCAP;
		 fd->filter.maskBits = EVERYTHING;
         striker_assembly->CreateFixture(fd);
         shape.SetAsBox(6.0f, 1.0f,b2Vec2(13.0f,0.0f),0.0f);
         fd->shape = &shape;
         fd->filter.categoryBits = SPRING;
		 fd->filter.maskBits = EVERYTHING | SPRING;
         striker_assembly->CreateFixture(fd); ///striker
         shape.SetAsBox(0.5f, 1.0f, b2Vec2(18.5f,-2.0f),0.0f);
         fd->shape = &shape;
         fd->restitution = 1;
         striker_assembly->CreateFixture(fd); ///nose
         striker_assembly->SetUserData(new int(1));
      }  
      
     //b2Body* cap;
     {
		 b2PolygonShape shape;
		 shape.SetAsBox(0.25f, 1.5f);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &shape;
		 fd->filter.categoryBits = SPRING;
		 fd->filter.maskBits = EVERYTHING | SPRING;
		 b2BodyDef bd;
		 bd.type = b2_dynamicBody;
         bd.position.Set(14.5f, 24.0f);
         cap=m_world->CreateBody(&bd);
         cap->CreateFixture(fd);
         shape.SetAsBox(0.5f, 1.0f,b2Vec2(0.75f,0.0f),0.0f);
         fd->shape = &shape;
         cap->CreateFixture(fd);
        }
        
     //b2Body* spacer_sleeve;
     {
		 b2PolygonShape shape;
		 shape.SetAsBox(6.0f, 1.5f);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &shape;
		 fd->filter.categoryBits = SPRING;
		 fd->filter.maskBits = EVERYTHING;
		 b2BodyDef bd;
		 bd.type = b2_dynamicBody;
         bd.position.Set(29.0f, 24.0f);
         spacer_sleeve=m_world->CreateBody(&bd);
         spacer_sleeve->CreateFixture(fd);
        }
	
	  b2PrismaticJointDef prismaticJointDef;
	  prismaticJointDef.bodyA = striker_assembly;
	  prismaticJointDef.bodyB = spacer_sleeve;
	  prismaticJointDef.collideConnected = true;
	  prismaticJointDef.enableLimit = true;
	  prismaticJointDef.upperTranslation = 201;
	  prismaticJointDef.localAxisA.Set(1,0);
	  (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef ); ///striker_assembly and spacer_sleeve
	  prismaticJointDef.bodyB = cap;
	  prismaticJointDef.enableLimit = true;
	  (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef ); ///striker_assembly and cap
	  prismaticJointDef.bodyA = spacer_sleeve;
	  //prismaticJointDef.enableMotor = true;
	  //prismaticJointDef.maxMotorForce = 500; ///spring action
	  //prismaticJointDef.motorSpeed = -sqrt(100 - (14.75 + cap->GetPosition().x - spacer_sleeve->GetPosition().x)*(cap->GetPosition().x - spacer_sleeve->GetPosition().x));
	  prismaticJointDef.enableLimit = true;
	  prismaticJointDef.lowerTranslation = -14.5; ///stop spring right before it reaches natural length
	  (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef ); ///cap and spacer_sleeve
	  
	//b2Body* bullet;
	{
		b2PolygonShape shape;
		 shape.SetAsBox(1.0f, 0.5f);
		 b2BodyDef bd;
         bd.position.Set(-4.5f, 24.0f);
         bd.type = b2_dynamicBody;
         bullet=m_world->CreateBody(&bd);
         b2FixtureDef *fd = new b2FixtureDef;
         fd->shape = &shape;
         fd->restitution = 1;
         //fd->density = 1;
         bullet->CreateFixture(fd);
         //bullet->SetUserData(new int(2));
	}
	
	//b2Body* casing;
	{
		b2PolygonShape shape;
		shape.SetAsBox(3.75,1.9);
		b2BodyDef bd;
        bd.position.Set(0.25f, 24.0f);
        bd.type = b2_dynamicBody;
        casing=m_world->CreateBody(&bd);
		b2FixtureDef *fd = new b2FixtureDef;
		fd->shape = &shape;
		fd->density = 1;
		casing->CreateFixture(fd);
		shape.SetAsBox(0.25,1,b2Vec2(4.00,0),0);
		fd->shape = &shape;
		casing->CreateFixture(fd);
		shape.SetAsBox(0.25,1.9,b2Vec2(4.5,0),0);
		fd->shape = &shape;
		casing->CreateFixture(fd);
		casing->SetUserData(new int(2));
	}
	
	//b2Body* trigger;
	{
		 //b2Vec2 vertices[4];
		 //vertices[0].Set(0,0);
		 //vertices[1].Set(0,-8);
		 //vertices[2].Set(-2,-8);
		 //vertices[3].Set( -6, 0);
		 b2Vec2 vertices[3];
		 vertices[0].Set(2,-5);
		 vertices[1].Set(-2,-11);
		 vertices[2].Set(-4,-1);
		 b2PolygonShape polygonShape;
		 polygonShape.Set(vertices, 3);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &polygonShape;
		 //fd->filter.categoryBits = SPRING;
		 //fd->filter.maskBits = EVERYTHING;
		 fd->density = 10;
		 b2BodyDef bd;
         bd.position.Set(9.0f, 19.25f);
         bd.type = b2_dynamicBody;
         bd.angularVelocity = 5.0f;
         trigger = m_world->CreateBody(&bd);
         trigger->CreateFixture(fd);
	}
	
	b2RevoluteJointDef jointDef;
	jointDef.bodyA= trigger;
	jointDef.bodyB= b1;
	jointDef.localAnchorA.Set(-4,-1);
    jointDef.localAnchorB.Set(5,18.25);
    jointDef.enableLimit = true;
    jointDef.lowerAngle = -0.55;
    //jointDef.upperAngle =  -100;
	(b2RevoluteJoint*)m_world->CreateJoint(&jointDef);
	
	//b2Body* bar;
	{
		 b2PolygonShape shape;
		 shape.SetAsBox(12.0f, 0.75f, b2Vec2(0,0),0.25f);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &shape;
		 //fd->filter.categoryBits = SPRING;
		 //fd->filter.maskBits = EVERYTHING;
		 fd->density = 10;
		 b2BodyDef bd;
         bd.position.Set(18.75f, 16.25f);
         bd.type = b2_dynamicBody;
         //bd.angularVelocity = 0.5f;
         bar = m_world->CreateBody(&bd);
         bar->CreateFixture(fd);
         
         b2Vec2 vertices[6];
		 vertices[0].Set(6,3.5);
		 vertices[1].Set(7,3.5);
		 //vertices[2].Set(1,1);
		 vertices[2].Set(10,4.5);
		 vertices[3].Set(10,5.5);
		 vertices[4].Set(9,5.5);
		 shape.Set(vertices, 5);
		 fd->shape = &shape;
		 bar->CreateFixture(fd);
	}
	
	jointDef.bodyB= bar;
	jointDef.localAnchorA.Set(-1,-6);
	jointDef.localAnchorB.Set(-10.75,-3);
	(b2RevoluteJoint*)m_world->CreateJoint(&jointDef);
	
	b2Vec2* worldAxis = new b2Vec2(2, -1);
 
	b2WheelJointDef wheelJointDef;
	/// Box2D source code:  A line joint. This joint provides one degree of freedom: translation along an axis fixed in body1. You can use a joint limit to restrict the range of motion and a joint motor to drive the motion or to model joint friction.
	//wheelJointDef.Initialize(trigger, bar, bar->GetWorldCenter(), *worldAxis);
	wheelJointDef.bodyA = b1;
	wheelJointDef.bodyB = bar;
	wheelJointDef.localAxisA = b2Vec2(2,-1);
	wheelJointDef.localAxisA.Normalize();
	wheelJointDef.localAnchorA.Set(30,19.25);
	wheelJointDef.localAnchorB.Set(11.25,3); 
	wheelJointDef.frequencyHz=0;
	(b2WheelJoint*)m_world->CreateJoint(&wheelJointDef);
	
	
	//b2Body* barrel;
	{
		 b2PolygonShape polygonShape;
		 polygonShape.SetAsBox(14,0.5);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &polygonShape;
		 //fd->filter.categoryBits = SPRING;
		 //fd->filter.maskBits = EVERYTHING;
		 fd->density = 10;
		 b2BodyDef bd;
         bd.position.Set(-21.0f, 26.5f);
         bd.type = b2_dynamicBody;
         //bd.linearVelocity=b2Vec2(10,0);
         barrel = m_world->CreateBody(&bd);
         barrel->CreateFixture(fd);
         polygonShape.SetAsBox(6,1,b2Vec2(20,0.5),0);
         fd->shape = &polygonShape;
         barrel->CreateFixture(fd);
         
         b2Vec2 vertices[4];
		 vertices[0].Set(-14,-4.5);
		 vertices[1].Set(16,-4.5);
		 vertices[2].Set(17,-5.5);
		 vertices[3].Set(-14,-5.5);
		 polygonShape.Set(vertices, 4);
		 fd->shape = &polygonShape;
         barrel->CreateFixture(fd);
		 
		 vertices[0].Set(16,-4.5);
		 vertices[1].Set(18,-4.5);
		 vertices[2].Set(20.5,-7);
		 vertices[3].Set(19,-7.5);
		 polygonShape.Set(vertices, 4);
		 fd->shape = &polygonShape;
         barrel->CreateFixture(fd);
         
         vertices[0].Set(18,-4.5);
		 vertices[1].Set(20,-4.5);
		 vertices[2].Set(20.75,-5.25);
		 vertices[3].Set(18.75,-5.25);
		 polygonShape.Set(vertices, 4);
		 fd->shape = &polygonShape;
         barrel->CreateFixture(fd);
         
         vertices[0].Set(20,-4.5);
		 vertices[1].Set(24,-4.5);
		 vertices[2].Set(26,-6.5);
		 vertices[3].Set(22,-6.5);
		 polygonShape.Set(vertices, 4);
		 fd->shape = &polygonShape;
         barrel->CreateFixture(fd);
	}
	
	
	//b2Body* slide;
	{
		 b2PolygonShape polygonShape;
		 polygonShape.SetAsBox(13.75,0.5);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &polygonShape;
		 //fd->filter.categoryBits = SPRING;
		 //fd->filter.maskBits = EVERYTHING;
		 fd->density = 1000;
		 b2BodyDef bd;
         bd.position.Set(-20.75f, 27.5f);
         bd.type = b2_dynamicBody;
         //bd.linearVelocity=b2Vec2(10,0);
         slide = m_world->CreateBody(&bd);
         slide->CreateFixture(fd);
		 polygonShape.SetAsBox(0.5f, 1.0f,b2Vec2(-13.5,-9),0.0f);
		 fd->shape = &polygonShape;
		 //fd->filter.categoryBits = SPRING;
		 //fd->filter.maskBits = EVERYTHING | SPRING;
         slide->CreateFixture(fd);
         polygonShape.SetAsBox(0.25f, 1.5f, b2Vec2(-12.75,-9),0);
         fd->shape = &polygonShape;
         slide->CreateFixture(fd);
         polygonShape.SetAsBox(4.525,1.5,b2Vec2(30.375,-1),0);
         fd->shape = &polygonShape;
         slide->CreateFixture(fd);
         polygonShape.SetAsBox(0.001,0.43,b2Vec2(-13.75,-7),0);
         slide->CreateFixture(fd);
         polygonShape.SetAsBox(0.5,0.25,b2Vec2(26.35,-2.75),0);
         slide->CreateFixture(fd);
         polygonShape.SetAsBox(0.5,0.25,b2Vec2(26.35,-4.75),0);
         slide->CreateFixture(fd);
         b2Vec2 vertices[5];
         vertices[0].Set(29.375-2.525,1.5-6);
		 vertices[1].Set(29.375-2.525,-1.5-6);
		 vertices[2].Set(29.375-2.025,-1.5-6);
		 vertices[3].Set(29.375+2.525,0-6);
		 vertices[4].Set(29.375+2.525,1.5-6);
         polygonShape.Set(vertices,5);
         fd->shape = &polygonShape;
         fd->filter.categoryBits = COMPRES;
		 fd->filter.maskBits = EVERYTHING | NOCAP | SPRING;
		 fd->density = 0;
         slide->CreateFixture(fd);
         polygonShape.SetAsBox(5,0.5,b2Vec2(36.875,-5.5),0);
         fd->shape = &polygonShape;
         slide->CreateFixture(fd); ///for bringing up the new bullet
    }
    
    //b2Body* fixed;
    {
		b2Vec2 vertices[4];
		 vertices[0].Set(0,0);
		 vertices[1].Set(1,0);
		 vertices[2].Set(2,-1);
		 vertices[3].Set( 1, -1);
		 b2PolygonShape polygonShape;
		 polygonShape.Set(vertices, 4);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &polygonShape;
		 //fd->filter.categoryBits = SPRING;
		 //fd->filter.maskBits = EVERYTHING;
		 fd->density = 10;
		 b2BodyDef bd;
         bd.position.Set(0.5,20);
         //bd.type = b2_dynamicBody;
         fixed = m_world->CreateBody(&bd);
         fixed->CreateFixture(fd); ///sliding_lock
         
         polygonShape.SetAsBox(0.25f, 1.5f,b2Vec2(-4.75,-1.5),0); ///right_spring_holder
         fd->shape = &polygonShape;
         fixed->CreateFixture(fd);
         polygonShape.SetAsBox(1.0f, 0.001f,b2Vec2(20,1.8),0);
         fd->shape = &polygonShape;
         fd->restitution = 1;
         fd->filter.categoryBits = COMPRES;
		 fd->filter.maskBits = EVERYTHING | NOCAP | SPRING;
         fixed->CreateFixture(fd); ///
         //polygonShape.SetAsBox(0.001f, 1.0f,b2Vec2(18,1.2),0);
         //fd->shape = &polygonShape;
         //fd->restitution = 1;
         //fixed->CreateFixture(fd); ///
         fixed->SetUserData(new int(3));
	}
	
		  prismaticJointDef.bodyA = slide;
		  prismaticJointDef.bodyB = fixed;
		  prismaticJointDef.localAnchorA.Set(21.25,-7.5);
		  //prismaticJointDef.collideConnected = false;
		  //prismaticJointDef.enableLimit = true;
		  //prismaticJointDef.upperTranslation = 201;
		  prismaticJointDef.localAxisA.Set(1,0);
		  (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef ); ///slide and fixed
		  
		  b2WeldJointDef weldJointDef;
		  weldJointDef.bodyA = slide;
		  weldJointDef.bodyB = spacer_sleeve;
		  weldJointDef.localAnchorA.Set(49.75,-3.5);
		  (b2WeldJoint*)m_world->CreateJoint( &weldJointDef ); ///slide and spacer_sleeve
		  
		  
    }
  

  sim_t *sim = new sim_t("Dominos", dominos_t::create);
  
  void dominos_t::step(settings_t* settings) {
	base_sim_t::step(settings);
	if(coll) {
		coll = false;
		bullet->ApplyLinearImpulse(b2Vec2(-5000000,0),bullet->GetWorldCenter(),true);
		casing->ApplyLinearImpulse(b2Vec2(3000,0),casing->GetWorldCenter(),true);
		slide->ApplyLinearImpulse(b2Vec2(5000000,0),slide->GetWorldCenter(),true);
	}
	
	else if(loll) {
		loll = false;
		casing->ApplyLinearImpulse(b2Vec2(0,10000),casing->GetWorldCenter(),true);
	}
	//cout<<striker_assembly->GetPosition().x - bar->GetPosition().x<<endl;
	if((trig_reset) && (striker_assembly->GetPosition().x - bar->GetPosition().x > 7)) {
		//cout<<"dude"<<endl;
		trig_reset = false;
		trigger->ApplyAngularImpulse(-15000,true);
	}
		
	//slidex = slide->GetPosition().x;
	//fixedx = fixed->GetPosition().x;
	int slidev = slide->GetLinearVelocity().x;
	
	if(-21.25 + fixed->GetPosition().x - slide->GetPosition().x > 0.001) {
		slide->SetLinearVelocity(b2Vec2(0,0));
		slide->SetTransform(b2Vec2(-21.25 + fixed->GetPosition().x,slide->GetPosition().y),0);
	}
	slide->ApplyForce(b2Vec2(1000000 * (-21.25 + fixed->GetPosition().x - slide->GetPosition().x), 0), slide->GetWorldCenter(), true);///spring between fixed and slide
	//fixed->ApplyForce(b2Vec2(1000000 * (21.25 + slide->GetPosition().x - fixed->GetPosition().x), 0), fixed->GetWorldCenter(), true);///spring between fixed and slide
	
    spacer_sleeve->ApplyForce(b2Vec2(1000 * (14.75 + cap->GetPosition().x - spacer_sleeve->GetPosition().x), 0), spacer_sleeve->GetWorldCenter(), true);///spring between cap and spacer_sleeve
	cap->ApplyForce(b2Vec2(1000 * (-14.75 + spacer_sleeve->GetPosition().x - cap->GetPosition().x), 0), cap->GetWorldCenter(), true);///spring between cap and spacer_sleeve

}

  	
}

