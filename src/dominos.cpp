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
  /**  This is the constructor \n
   * This is the documentation block for the constructor.
   */
  dominos_t::dominos_t()
  {
	  
	  //Ground
   /** The ground has an EdgeShape between co-ordinates (-90, 0) and (90, 0).
     * 
     */ 
    b2Body* b1;
    {
      
      b2EdgeShape shape; 
      shape.Set(b2Vec2(-90.0f, 0.0f), b2Vec2(90.0f, 0.0f));
      b2BodyDef bd; 
      b1 = m_world->CreateBody(&bd); 
      b1->CreateFixture(&shape, 0.0f);
    }
    
	  enum _entityCategory {
    EVERYTHING =          0x0001,
    SPRING =     0x0002,
  };
	  
	//Striker Assembly
	/** Firing pin, striker, nose
	 * make a prismatic joint between striker and spring, striker and cap
	 */
	 b2Body* striker_assembly;
	 {
		 b2PolygonShape shape;
		 shape.SetAsBox(3.0f, 1.0f);
	
		 b2BodyDef bd;
         bd.position.Set(-31.0f, 8.0f);
         bd.type = b2_dynamicBody;
         striker_assembly = m_world->CreateBody(&bd);
         striker_assembly->CreateFixture(&shape, 0.0f);
         
         
         b2Vec2 vertices[5];
		 vertices[0].Set(-3,  -0.5);
		 vertices[1].Set(-3,  0.5);
		 vertices[2].Set( -4, 0.5);
		 vertices[3].Set(-5,  0);
		 vertices[4].Set( -4, -0.5);
		 b2PolygonShape polygonShape;
		 polygonShape.Set(vertices, 5);
		 striker_assembly->CreateFixture(&polygonShape, 0.0f); ///firing pin

		 shape.SetAsBox(2.0f, 0.5f, b2Vec2(5.0f,0.0f),0.0f);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &shape;
		 fd->filter.categoryBits = SPRING;
		 fd->filter.maskBits = EVERYTHING;
         striker_assembly->CreateFixture(fd);
         shape.SetAsBox(6.0f, 1.0f,b2Vec2(13.0f,0.0f),0.0f);
         fd->shape = &shape;
         striker_assembly->CreateFixture(fd); ///striker
         shape.SetAsBox(0.5f, 1.0f, b2Vec2(18.5f,-2.0f),0.0f);
         fd->shape = &shape;
         striker_assembly->CreateFixture(fd); ///nose
      }  
      
     b2Body* cap;
     {
		 b2PolygonShape shape;
		 shape.SetAsBox(0.25f, 1.5f);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &shape;
		 fd->filter.categoryBits = SPRING;
		 fd->filter.maskBits = EVERYTHING;
		 b2BodyDef bd;
		 bd.type = b2_dynamicBody;
         bd.position.Set(-27.75f, 8.0f);
         cap=m_world->CreateBody(&bd);
         cap->CreateFixture(fd);
         shape.SetAsBox(0.5f, 1.0f,b2Vec2(0.75f,0.0f),0.0f);
         fd->shape = &shape;
         cap->CreateFixture(fd);
        }
        
     b2Body* spacer_sleeve;
     {
		 b2PolygonShape shape;
		 shape.SetAsBox(6.0f, 1.5f);
		 b2FixtureDef *fd = new b2FixtureDef;
		 fd->shape = &shape;
		 fd->filter.categoryBits = SPRING;
		 fd->filter.maskBits = EVERYTHING;
		 b2BodyDef bd;
         bd.position.Set(-13.0f, 8.0f);
         spacer_sleeve=m_world->CreateBody(&bd);
         spacer_sleeve->CreateFixture(fd);
        }
	
	  b2PrismaticJointDef prismaticJointDef;
	  prismaticJointDef.bodyA = striker_assembly;
	  prismaticJointDef.bodyB = spacer_sleeve;
	  prismaticJointDef.collideConnected = true;
	  prismaticJointDef.enableLimit = true;
	  prismaticJointDef.upperTranslation = 24;
	  prismaticJointDef.localAxisA.Set(1,0);
	  (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef ); ///striker_assembly and spacer_sleeve
	  prismaticJointDef.bodyB = cap;
	  prismaticJointDef.enableLimit = true;
	  (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef ); ///striker_assembly and cap
	  prismaticJointDef.bodyA = spacer_sleeve;
	  prismaticJointDef.enableMotor = true;
	  prismaticJointDef.maxMotorForce = 500; ///spring action
	  prismaticJointDef.motorSpeed = -10;
	  prismaticJointDef.enableLimit = true;
	  prismaticJointDef.lowerTranslation = -20;
	  (b2PrismaticJoint*)m_world->CreateJoint( &prismaticJointDef ); ///cap and spacer_sleeve
	
	b2Body* trigger;
	{
		 b2Vec2 vertices[4];
		 vertices[0].Set(0,0);
		 vertices[1].Set(0,-3);
		 vertices[2].Set(-1,-3);
		 vertices[3].Set( -3, 0);
		 b2PolygonShape polygonShape;
		 polygonShape.Set(vertices, 4);
		 
		 b2BodyDef bd;
         bd.position.Set(10.0f, 8.0f);
         bd.type = b2_dynamicBody;
         bd.angularVelocity = 0.5f;
         trigger = m_world->CreateBody(&bd);
         trigger->CreateFixture(&polygonShape, 0.0f);
	}
	
	b2RevoluteJointDef jointDef;
	jointDef.bodyA= trigger;
	jointDef.bodyB= b1;
	jointDef.localAnchorA.Set(-2,-0.5);
    jointDef.localAnchorB.Set(8,7.5);
	(b2RevoluteJoint*)m_world->CreateJoint(&jointDef);
	
	b2Body* bar;
	{
		 b2PolygonShape shape;
		 shape.SetAsBox(4.0f, 0.5f, b2Vec2(0,0),0.25f);
		 
		 b2BodyDef bd;
         bd.position.Set(12.0f, 7.0f);
         bd.type = b2_dynamicBody;
         bd.angularVelocity = 0.5f;
         bar = m_world->CreateBody(&bd);
         bar->CreateFixture(&shape, 0.0f);
	}
	
	jointDef.bodyB= bar;
	jointDef.localAnchorA.Set(-0.5,-2);
	jointDef.localAnchorB.Set(-2.5,-1);
	//(b2RevoluteJoint*)m_world->CreateJoint(&jointDef);
	
	b2Vec2* worldAxis = new b2Vec2(2, -1);
 
	b2WheelJointDef wheelJointDef;
	/// Box2D source code:  A line joint. This joint provides one degree of freedom: translation along an axis fixed in body1. You can use a joint limit to restrict the range of motion and a joint motor to drive the motion or to model joint friction.
	//wheelJointDef.Initialize(trigger, bar, bar->GetWorldCenter(), *worldAxis);
	wheelJointDef.bodyA = b1;
	wheelJointDef.bodyB = bar;
	wheelJointDef.localAxisA = b2Vec2(2,-1);
	wheelJointDef.localAxisA.Normalize();
	wheelJointDef.localAnchorA.Set(14.5,8);
	wheelJointDef.localAnchorB.Set(2.5,1); 
	wheelJointDef.frequencyHz=0;
	//wheelJointDef.dampingRatio=1000000;
	//lineJointDef.lowerTranslation = -2.0;
	//lineJointDef.upperTranslation = 2.0;
	//lineJointDef.enableLimit = true;
	//lineJointDef.maxMotorForce = 200.0;
	//lineJointDef.motorSpeed = 10.0;
	//lineJointDef.enableMotor = true;
	 
	(b2WheelJoint*)m_world->CreateJoint(&wheelJointDef);
	//{
      //b2PolygonShape shape2;
      //shape2.SetAsBox(1.0f, 16.0f);
      //b2BodyDef bd3;
      //bd3.position.Set(-40.0f, 8.0f);
      ////bd3.type = b2_dynamicBody;
      //bd3.angularVelocity = -1.0f;
      //bd3.linearVelocity.Set(0.0f, 20.0f);
      //b2Body* body3 = m_world->CreateBody(&bd3);
      //b2FixtureDef *fd3 = new b2FixtureDef;
      //fd3->density = 0.01f;
      //fd3->shape = new b2PolygonShape;
      //fd3->shape = &shape2;
      //body3->CreateFixture(fd3);
    //}	 
    
          
    
    ////Top horizontal shelf
    ///** The top horizontal shelf has a PolygonShape of dimensions (6,0.25).\n
     //* The center of the shelf is at (-31, 30).
     //*
     //*/
    //{
      //b2PolygonShape shape;
      //shape.SetAsBox(6.0f, 0.25f);
	
      //b2BodyDef bd;
      //bd.position.Set(-31.0f, 30.0f);
      //b2Body* ground = m_world->CreateBody(&bd);
      //ground->CreateFixture(&shape, 0.0f);
    //}

    ////Dominos
    ///** The Dominos have PolygonShape of dimensions (0.2, 2.0). \n
     //* The center of the ith domino is at (-35.5 + i, 31.25). \n
     //* There are 10 dominos. \n
     //* They have a density of 20.
     //*
     //*/
    //{
      //b2PolygonShape shape;
      //shape.SetAsBox(0.1f, 1.0f);
	
      //b2FixtureDef fd;
      //fd.shape = &shape;
      //fd.density = 20.0f;
      //fd.friction = 0.1f;
		
      //for (int i = 0; i < 10; ++i)
	//{
	  //b2BodyDef bd;
	  //bd.type = b2_dynamicBody;
	  //bd.position.Set(-35.5f + 1.0f * i, 31.25f);
	  //b2Body* body = m_world->CreateBody(&bd);
	  //body->CreateFixture(&fd);
	//}
    //}
      
    ////Another horizontal shelf
    ///** The second horizontal shelf has a PolygonShape of dimensions (14, 0.5). \n
     //* Its center is located at (-20, 20).
     //*
     //*/
    //{
      //b2PolygonShape shape;
      //shape.SetAsBox(7.0f, 0.25f, b2Vec2(-20.f,20.f), 0.0f);
	
      //b2BodyDef bd;
      //bd.position.Set(1.0f, 6.0f);
      //b2Body* ground = m_world->CreateBody(&bd);
      //ground->CreateFixture(&shape, 0.0f);
    //}
    ////The long box on the left
    ///** The Long Box on the left has a PolygonShape SetAsBox of size (1, 16).\n
     //* It's position is (-40, 8).\n
     //* It's velocity is (0, 20).\n
     //* It's angular velocity is -1.\n
     //* It's density is 0.01
     //*
     //*/
    //{
      //b2PolygonShape shape2;
      //shape2.SetAsBox(1.0f, 16.0f);
      //b2BodyDef bd3;
      //bd3.position.Set(-40.0f, 8.0f);
      //bd3.type = b2_dynamicBody;
      //bd3.angularVelocity = -1.0f;
      //bd3.linearVelocity.Set(0.0f, 20.0f);
      //b2Body* body3 = m_world->CreateBody(&bd3);
      //b2FixtureDef *fd3 = new b2FixtureDef;
      //fd3->density = 0.01f;
      //fd3->shape = new b2PolygonShape;
      //fd3->shape = &shape2;
      //body3->CreateFixture(fd3);
    //}

    ////The Ball Hitting the Long Box
    ///** The ball has a CircleShape of radius 1. \n
     //*  It's position is (-50,10). \n
     //*  It has a linear velocity of (30,10). \n
     //*  It has a density of 2.
     //*
     //*/
  
    //{
    //b2Body* spherebody;	
      //b2CircleShape circle;
      //circle.m_radius = 1.0;
      //b2FixtureDef ballfd;
      //ballfd.shape = &circle;
      //ballfd.density = 2.0f;
      //ballfd.friction = 0.0f;
      //ballfd.restitution = 0.0f;
	  //b2BodyDef ballbd;
	  //ballbd.linearVelocity.Set(30.0f, 10.0f);
	  //ballbd.type = b2_dynamicBody;
	  //ballbd.position.Set(-50.0f, 10.0f);
	  //spherebody = m_world->CreateBody(&ballbd);
	  //spherebody->CreateFixture(&ballfd);
	
    //}
    //{
	////Projectile Hitting The Bob
    ///** The projectile has a CircleShape of radius 1. \n
     //*  It's position is (-50,50). \n
     //*  It has a linear velocity of (30,10). \n
     //*  It has a density of 2.
     //*
     //*/
      //b2Body* spherebody;
	
      //b2CircleShape circle;
      //circle.m_radius = 0.5;
      //b2FixtureDef ballfd;
      //ballfd.shape = &circle;
      //ballfd.density = 0.2f;
      //ballfd.friction = 0.0f;
      //ballfd.restitution = 0.0f;
	  //b2BodyDef ballbd;
	  //ballbd.linearVelocity.Set(40.0f, 0.0f);
	  //ballbd.type = b2_dynamicBody;
	  //ballbd.position.Set(-40.0f, 32.0f);
	  //spherebody = m_world->CreateBody(&ballbd);
	  //spherebody->CreateFixture(&ballfd);
	
    //}
    ////The pendulum that knocks the dominos off
    ///** The pendulum system consists of a revolute joint between a bob and a static body. \n
     //* The static body has a PolygonShape of dimensions (0.25,1.5) and is centered at (-36.5,28). \n
     //* The bob has a PolygonShape of dimensions (0.25,0.25), is centered at (-40,33) and has a density of 2. \n
     //* The anchor is set at (-37,40). 
     //* 
     //*/
    //{
      //b2Body* b2;
      //{
	//b2PolygonShape shape;
	//shape.SetAsBox(0.25f, 1.5f);
	  
	//b2BodyDef bd;
	//bd.position.Set(-36.5f, 29.0f);
	//b2 = m_world->CreateBody(&bd);
	//b2->CreateFixture(&shape, 10.0f);
      //}

    
      //b2Body* b4;
      //{
	//b2PolygonShape shape;
	//shape.SetAsBox(0.5f, 0.5f);
	  
	//b2BodyDef bd;
	//bd.type = b2_dynamicBody;
	//bd.position.Set(-37.0f, 32.0f);
	//b4 = m_world->CreateBody(&bd);
	//b4->CreateFixture(&shape, 2.0f);
      //}
	
      //b2RevoluteJointDef jd;
      //b2Vec2 anchor;
      //anchor.Set(-37.0f, 40.0f);
      //jd.Initialize(b2, b4, anchor);
      //m_world->CreateJoint(&jd);
    //}
  
    ////The train of small spheres
    ///** The train of spheres have a CircleShape of radius 0.5. \n
     //*  They have densities of 1. \n
     //*  There are 10 spheres. \n
     //*  The ith sphere has a position of (-22.2+i,26.6).
     //*
     //*/
    //{
      //b2Body* spherebody;
	
      //b2CircleShape circle;
      //circle.m_radius = 0.5;
	
      //b2FixtureDef ballfd;
      //ballfd.shape = &circle;
      //ballfd.density = 1.0f;
      //ballfd.friction = 0.0f;
      //ballfd.restitution = 0.0f;
	
      //for (int i = 0; i < 10; ++i)
	//{
	  //b2BodyDef ballbd;
	  //ballbd.type = b2_dynamicBody;
	  //ballbd.position.Set(-22.2f + i*1.0, 26.6f);
	  //spherebody = m_world->CreateBody(&ballbd);
	  //spherebody->CreateFixture(&ballfd);
	//}
    //}

    ////The pulley system
    ///** The pulley system consists of an open box, a bar and a pulley joint. 
     //*/

    //{
      //b2BodyDef *bd = new b2BodyDef;
      //bd->type = b2_dynamicBody;
      //bd->position.Set(-10,15);
      //bd->fixedRotation = true;
      
      ////The open box
      ///** The open box is made up of 3 polygonShape and is centered at (-10,15). \n
       //*  One of them has a size of (2,0.2) and have centers are located at (0,-1.9). \n
       //*  The other two have sizes of (2,0.2) and centers located at (2,0) and (-2,0) respectively. \n
       //*  It has the property of fixed rotation and a density of 10.
       //* 
       //*/
      //b2FixtureDef *fd1 = new b2FixtureDef;
      //fd1->density = 10.0;
      //fd1->friction = 0.5;
      //fd1->restitution = 0.f;
      //fd1->shape = new b2PolygonShape;
      //b2PolygonShape bs1;
      //bs1.SetAsBox(2,0.2, b2Vec2(0.f,-1.9f), 0);
      //fd1->shape = &bs1;
      //b2FixtureDef *fd2 = new b2FixtureDef;
      //fd2->density = 10.0;
      //fd2->friction = 0.5;
      //fd2->restitution = 0.f;
      //fd2->shape = new b2PolygonShape;
      //b2PolygonShape bs2;
      //bs2.SetAsBox(0.2,2, b2Vec2(2.0f,0.f), 0);
      //fd2->shape = &bs2;
      //b2FixtureDef *fd3 = new b2FixtureDef;
      //fd3->density = 10.0;
      //fd3->friction = 0.5;
      //fd3->restitution = 0.f;
      //fd3->shape = new b2PolygonShape;
      //b2PolygonShape bs3;
      //bs3.SetAsBox(0.2,2, b2Vec2(-2.0f,0.f), 0);
      //fd3->shape = &bs3;
       
      //b2Body* box1 = m_world->CreateBody(bd);
      //box1->CreateFixture(fd1);
      //box1->CreateFixture(fd2);
      //box1->CreateFixture(fd3);

      ////The bar
      ///** The bar has its center at (10,15). \n
       //* Its density is 34. 
       //* 
       //*/
      //bd->position.Set(10,15);	
      //fd1->density = 34.0;	  
      //b2Body* box2 = m_world->CreateBody(bd);
      //box2->CreateFixture(fd1);

      //// The pulley joint
      ///** The pulley joint consists of anchor points at (-10,15), (10,15), (10,20) and (10,20). \n
       //* The length ratio is 1.
       //* 
       //*/
      //b2PulleyJointDef* myjoint = new b2PulleyJointDef();
      //b2Vec2 worldAnchorOnBody1(-10, 15); // Anchor point on body 1 in world axis
      //b2Vec2 worldAnchorOnBody2(10, 15); // Anchor point on body 2 in world axis
      //b2Vec2 worldAnchorGround1(-10, 20); // Anchor point for ground 1 in world axis
      //b2Vec2 worldAnchorGround2(10, 20); // Anchor point for ground 2 in world axis
      //float32 ratio = 1.0f; // Define ratio
      //myjoint->Initialize(box1, box2, worldAnchorGround1, worldAnchorGround2, box1->GetWorldCenter(), box2->GetWorldCenter(), ratio);
      //m_world->CreateJoint(myjoint);
    //}

    ////The revolving horizontal platform
    ///** The revolving horizontal platform has a polygonShape of dimensions (2.2,0.2). \n
     //*  Its center is located at (14,14). \n
     //*  It has a density of 1. \n
     //*  It is connected to a hidden static body located at (14,16) via a revolute joint.
     //* 
     //*/
    //{
      //b2PolygonShape shape;
      //shape.SetAsBox(2.2f, 0.2f);
	
      //b2BodyDef bd;
      //bd.position.Set(14.0f, 14.0f);
      //bd.type = b2_dynamicBody;
      //b2Body* body = m_world->CreateBody(&bd);
      //b2FixtureDef *fd = new b2FixtureDef;
      //fd->density = 1.f;
      //fd->shape = new b2PolygonShape;
      //fd->shape = &shape;
      //body->CreateFixture(fd);

      //b2PolygonShape shape2;
      //shape2.SetAsBox(0.2f, 2.0f);
      //b2BodyDef bd2;
      //bd2.position.Set(14.0f, 16.0f);
      //b2Body* body2 = m_world->CreateBody(&bd2);

      //b2RevoluteJointDef jointDef;
      //jointDef.bodyA = body;
      //jointDef.bodyB = body2;
      //jointDef.localAnchorA.Set(0,0);
      //jointDef.localAnchorB.Set(0,0);
      //jointDef.collideConnected = false;
      //m_world->CreateJoint(&jointDef);
    //}

    ////The heavy sphere on the platform
    ///** The heavy sphere on the platform has a CircleShape of radius 1. \n
     //* Its center is located at (14,18) and it has a density of 50.
     //* 
     //*/
    //{
      //b2Body* sbody;
      //b2CircleShape circle;
      //circle.m_radius = 1.0;
	
      //b2FixtureDef ballfd;
      //ballfd.shape = &circle;
      //ballfd.density = 500.0f;
      //ballfd.friction = 0.0f;
      //ballfd.restitution = 0.0f;
      //b2BodyDef ballbd;
      //ballbd.type = b2_dynamicBody;
      //ballbd.position.Set(14.0f, 18.0f);
      //sbody = m_world->CreateBody(&ballbd);
      //sbody->CreateFixture(&ballfd);
    //}


    ////The see-saw system at the bottom
    ///** The see-saw is made up of a revolute joint between a static triangle wedge and a plank on top of it. \n
     //* The joint is anchored at (30,1.5).
     //* 
     //*/
    //{
      ////The triangle wedge
      ///** The triangular wedge has a PolygonShape consisting of 3 vertices located at (-1,0), (1,0) and (0,1.5). \n
       //* It has a density of 10 and center located at (30,0).
       //* 
       //*/
      //b2Body* sbody;
      //b2PolygonShape poly;
      //b2Vec2 vertices[3];
      //vertices[0].Set(-1,0);
      //vertices[1].Set(1,0);
      //vertices[2].Set(0,1.5);
      //poly.Set(vertices, 3);
      //b2FixtureDef wedgefd;
      //wedgefd.shape = &poly;
      //wedgefd.density = 10.0f;
      //wedgefd.friction = 0.0f;
      //wedgefd.restitution = 0.0f;
      //b2BodyDef wedgebd;
      //wedgebd.position.Set(30.0f, 0.0f);
      //sbody = m_world->CreateBody(&wedgebd);
      //sbody->CreateFixture(&wedgefd);

      ////The plank on top of the wedge
      ///** The plank has a PolygonShape of dimensions (15,0.2) and center at (30,1.5). \n
       //* It has a density of 1.
       //* 
       //*/
      //b2PolygonShape shape;
      //shape.SetAsBox(15.0f, 0.2f);
      //b2BodyDef bd2;
      //bd2.position.Set(30.0f, 1.5f);
      //bd2.type = b2_dynamicBody;
      //b2Body* body = m_world->CreateBody(&bd2);
      //b2FixtureDef *fd2 = new b2FixtureDef;
      //fd2->density = 1.f;
      //fd2->shape = new b2PolygonShape;
      //fd2->shape = &shape;
      //body->CreateFixture(fd2);

      //b2RevoluteJointDef jd;
      //b2Vec2 anchor;
      //anchor.Set(30.0f, 1.5f);
      //jd.Initialize(sbody, body, anchor);
      //m_world->CreateJoint(&jd);

      ////The light box on the right side of the see-saw
      ///** The light box on the see-saw has a PolygonShape of dimensions (2,2) and is centered at (40,2). \n
       //* It has a density of 0.01.
       //* 
       //*/
      //b2PolygonShape shape2;
      //shape2.SetAsBox(2.0f, 2.0f);
      //b2BodyDef bd3;
      //bd3.position.Set(40.0f, 2.0f);
      //bd3.type = b2_dynamicBody;
      //b2Body* body3 = m_world->CreateBody(&bd3);
      //b2FixtureDef *fd3 = new b2FixtureDef;
      //fd3->density = 0.001f;
      //fd3->shape = new b2PolygonShape;
      //fd3->shape = &shape2;
      //body3->CreateFixture(fd3);
    }
  

  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
