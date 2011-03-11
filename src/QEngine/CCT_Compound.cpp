// Copyright Riley Adams 2010

// This file is part of QEngine.

// QEngine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// QEngine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with QEngine.  If not, see <http://www.gnu.org/licenses/>.


#include "stdafx.h"
#include "QEngine.h"
#include "CCT_Primitive.h"
#include "CCT_Compound.h"

// NOTE! This is some of the ugliest code ever to exist , it was originally written and rewritten about 30 times on a tight deadline for a game with PhysX like 2 years ago,
// then really horribly adapted to Bullet. It works pretty darn well as a character controller that can rotate dynamically with gravity, but don't expect the code to make any sense.

namespace QE
{
	// constructor. choose the number of primitives and the height/radius
	// the offset value is generated based off this (the spheres are placed evenly along the compound capsule shape
	QE::CCTCompound::CCTCompound(unsigned int n, float height_, float radius_,btDynamicsWorld* scene,const btVector3& posi)
	{
		skinWidth = 0.0125f;
		// NEEDS TWEAKING?
		increment = 0.00125f;
		Scene = scene;
		radius = radius_;
		height = height_;
		numberPrimitives = n;
		offset = height/((float)n-1);
		unsigned int g = 0;
		while(g<n)
		{
			cctp.push_back(new CCTPrimitive(radius,increment,skinWidth,posi+NxVec3(0,offset*(float)g,0),Scene));
			g++;
		}
		// position expressed as the center of the bottommost sphere (as it is the rotation point for orient-ing)
		position = posi;//NxVec3(0,radius,0);

		NxCapsule sweepCap = NxCapsule(height,radius);//(NxSegment(NxVec3(0,radius,0),NxVec3(0,height-radius,0)),radius);
		NxCapsule overCap = NxCapsule(height,radius+0.02f);//(NxSegment(NxVec3(0,radius,0),NxVec3(0,height-radius,0)),radius + 0.2f);

		upVector = NxVec3(0,1,0);
		mUpInterp = 0;
	}

	CCTCompound::~CCTCompound()
	{
		unsigned int a = cctp.size();
		while(a>0)
		{
			a--;
			delete cctp[a];
		}
		cctp.clear();
	}


	void CCTCompound::disableQs()
	{
		//for(unsigned int a = 0;a<cctp.size();++a)
		//{
			//cctp[a]->act->getShapes()[0]->setFlag(NX_SF_DISABLE_SCENE_QUERIES,true);
		//}
	}
	void CCTCompound::enableQs()
	{
		//for(unsigned int a = 0;a<cctp.size();++a)
		//{
			//cctp[a]->act->getShapes()[0]->setFlag(NX_SF_DISABLE_SCENE_QUERIES,false);
		//}
	}

	// move the CCT in a given direction/magnitude, it will slide and all that if such features are enabled (see various setters and getters later in this header.)
	void CCTCompound::move(const btVector3& dira,bool slidingEa,double deltaT,bool zeroG)
	{
		btVector3 dir = dira;
		bool slidingE = slidingEa;

		/// do this at the start so that I don't have to do it again anywhere (sqrt's = malo).
		float mags = dir.length();
		//slidingE = false;
		unsigned int a = cctp.size();
		unsigned int b = 0;
		float maxDist = -500.0f;
		float maxDistEh = -500.0f;
		NxVec3 firstBestDir;
		int maxIndex = -1;
		std::vector<moveStruct> ms;
		ms.clear();
		bool startTest = false;
		NxVec3 hitDist2;
		NxVec3 oldPosi;

		bool eh = false;/// if this worked, but isn't an optimal path...
		int maxIndexEh = -1;


		NxVec3 position001 = position;

		NxVec3 subUpVector = upVector;

		Ogre::Vector3 upVV = NxVector3(upVector);
		if(upVV.angleBetween(Ogre::Vector3::UNIT_X)<Ogre::Degree(1.f))
		{
			upVV = Ogre::Vector3::UNIT_X;
		}
		if(upVV.angleBetween(Ogre::Vector3::UNIT_Y)<Ogre::Degree(1.f))
		{
			//std::cout<<"um? testing?\n";
			upVV = Ogre::Vector3::UNIT_Y;
		}
		if(upVV.angleBetween(Ogre::Vector3::UNIT_Z)<Ogre::Degree(1.f))
		{
			upVV = Ogre::Vector3::UNIT_Z;
		}
		if(upVV.angleBetween(Ogre::Vector3::NEGATIVE_UNIT_X)<Ogre::Degree(1.f))
		{
			upVV = Ogre::Vector3::NEGATIVE_UNIT_X;
		}
		if(upVV.angleBetween(Ogre::Vector3::NEGATIVE_UNIT_Y)<Ogre::Degree(1.f))
		{
			upVV = Ogre::Vector3(0.00001f,-1,0);
		}
		if(upVV.angleBetween(Ogre::Vector3::NEGATIVE_UNIT_Z)<Ogre::Degree(1.f))
		{
			upVV = Ogre::Vector3::NEGATIVE_UNIT_Z;
		}
		subUpVector = Vector3Nx(upVV);
		subUpVector.normalize();

		///////////////////////////////////
		/*if(slidingE)
		{

		setCollision(false);

		NxSweepQueryHit sqh = sweepCapsuleDetail(getPosition()+subUpVector*0.3,dir,subUpVector);

		if(sqh.hitShape)
		{
			NxVec3 svect = dir - (dir.dot(sqh.normal))*sqh.normal;//
			//NxVec3 originalFinal = ms[maxIndex].finalPos;
			NxVec3 total;
			total = (svect*(1-sqh.t) + dir*sqh.t);
			if(total.magnitude()>dir.magnitude())
			{
				total.normalize();
				total*=mags;
			}
			/// will this work???
			dir = total;
		}
		}*/
		///////////////////////////////////

		if(slidingE&&!zeroG)
		{
			Ogre::Plane plnn = Ogre::Plane(upVV,0);
			Ogre::Vector3 wtftmp = plnn.projectVector(NxVector3(dir));
			wtftmp.normalise();
			wtftmp*=dir.length();
			dir = Vector3Nx(wtftmp);
		}






		/*bool done = false;
		bool almost_done = false;
		while(!done)
		{
			if(almost_done)
			{
				done = true;
			}
			ms.clear();
			b = 0;*/
			//////////////
			// MAIN LOOP
			/////////////
			while(b<a)
			{
				setCollision(false);
				NxVec3 originalP = cctp[b]->position;
				if(slidingE)
				{
					ms.push_back(cctp[b]->move(dir*10.0f,true,false));
				}
				else
				{
					ms.push_back(cctp[b]->move(dir,false,false));
				}

				NxVec3 hitDist = Vector3Nx(ms[b].change);
				//std::cout<<"dist: "<<hitDist.length()<<"\n";

				////////////
				////OVERSTEP
				////////////

				if(slidingE)
				{
					if(ms[b].change.length()>mags)
					{
						float mgs = ms[b].change.length();
						NxVec3 oldCh = Vector3Nx(ms[b].change);
						NxVec3 dmy = Vector3Nx(ms[b].change);
						dmy.normalize();
						ms[b].change.normalize();
						ms[b].change *= mags;
						dmy*=(mgs - mags);
						ms[b].finalPos = NxVector3(originalP) + ms[b].change;
					}
				}

				NxVec3 firstDir = Vector3Nx(ms[b].change);
				hitDist = Vector3Nx(ms[b].change);
				setCollision(false);

				////////////
				// SLIDING
				////////////

				bool slide = true;
				bool slid = false;

					if(slidingE)
					{
						if(ms[b].change.length()<dir.length())//&&a<=0
						{
								Ogre::Vector3 upV = NxVector3(subUpVector);//Ogre::Vector3::UNIT_Y;
								bool dynn = false;
								if(ms[b].act)
								{
									if(!ms[b].act->isStaticOrKinematicObject())
									{
										dynn = false;
									}
								}
								if(ms[b].act&&!dynn)//&&Ogre::Degree(Ogre::Math::Abs(upV.angleBetween(NxVector3(ms[b].hit.normal))))<=Ogre::Degree(120.0f))
								{
									//std::cout<<"Sliding!\n";
									//std::cout<<"normal: "<<ms[b].hit.normal.x<<" "<<ms[b].hit.normal.y<<" "<<ms[b].hit.normal.z<<"\n";

									NxVec3 svect;
									/*if(Ogre::Degree(Ogre::Math::Abs(NxVector3(ms[b].change).angleBetween(NxVector3(ms[b].hit.normal))))<=Ogre::Degree(60.0f))
									{
										svect = ms[b].hit.normal;
									}
									else
									{*/
									svect = dir - (dir.dot(Vector3Nx(ms[b].normal)))*Vector3Nx(ms[b].normal);//
									//}
									NxVec3 originalFinal = Vector3Nx(ms[b].finalPos);
									NxVec3 total;
									/////////////////////////////////////////////////////
									/*Ogre::Plane pln(NxVector3(subUpVector),0);

									/// the
									Ogre::Vector3 colp = NxVector3(ms[b].hit.point-cctp[b]->getPosition());
									Ogre::Vector3 colp2 = pln.projectVector(colp);

									float dist = pln.getDistance(colp);

									float slen = colp2.squaredLength();

									bool topCheck = true;
									bool botCheck = true;
									if(b==0)
									{
										botCheck = false;
									}
									if(b==numberPrimitives-1)
									{
										topCheck = false;
									}

									bool inH = false;

									if(Ogre::Math::Abs(dist)<radius&&((dist>0&&topCheck)||(dist<0&&botCheck)))
									{
										inH = true;
									}

									bool inR = false;

									if(slen<radius*radius)
									{
										inR = true;
									}

									if(inR&&inH)
									{
										/// move backwards along the motion dir so that the point is outside the radius
										//float maga = ms[b].change.magnitude();//sqrt...
										//float leng = Ogre::Math::Sqrt(slen);

										/// try moving back along colp2???

										//float dBack = radius - leng;

										//colp2.normalise();
										//colp2*=leng;

										ms[b].change -= Vector3Nx(colp2);///=maga;
										//maga -= dBack;
										//ms[b].change//*=maga;
									}
									else
									{
										ms[b].change -= Vector3Nx(colp2*0.005);///=maga;
									}*/

									///////////////////////////////////////////////////////

									total = ((svect) + Vector3Nx(ms[b].change*0.7f));
									if(total.length()>dir.length())
									{
										total.normalize();
										total*=dir.length();
										//std::cout<<"???\n";
									}
									moveStruct mv = cctp[b]->move(total,slidingE,false);
									hitDist = Vector3Nx(mv.change);
									ms[b].change = mv.change;
									ms[b].finalPos = mv.finalPos;
									ms[b].point = mv.point;
									ms[b].normal = mv.normal;

									/*if(mv.act)
									{

											std::cout<<"hit: "<<mv.act->getGroup()<<"\n";
											if(mv.act->isDynamic())
											{
											std::cout<<"WTF\n";

											}

									}*/
									NxVec3 newFinal = Vector3Nx(ms[b].finalPos);
									NxVec3 svct = newFinal - originalFinal;
									slid = true;
								}
							}
						}

						Ogre::Vector3 perpendicular = NxVector3(subUpVector).crossProduct(ms[b].change);
						perpendicular.normalise();
						perpendicular*=0.05f;// 10cm may or may not be too much...

				bool failed = true;
				if(setPosition(Vector3Nx(ms[b].finalPos)-subUpVector*(offset*b),false,subUpVector,false))
				{

					if(sweepCapsule(getPosition(),Vector3Nx(ms[b].change),subUpVector)||offset<0.2f)// if the player is crouching then let the
					{
						failed = false;
						if(Ogre::Math::Abs(ms[b].change.length())>maxDist)
						{
							if(b==0)
							{
								startTest = true;
							}
							maxDist = Ogre::Math::Abs(ms[b].change.length());
							maxIndex = b;
							firstBestDir = firstDir;
							hitDist2 = hitDist;
						}
					}
				}
				else if(setPosition(Vector3Nx(ms[b].finalPos)-subUpVector*(offset*b) + subUpVector*0.001f,false,subUpVector,false))//ms[0].change.magnitude()<0.05f||
				{
					bool dynamo = false;
						if(ms[b].act)
						{
							if(!ms[b].act->isStaticOrKinematicObject())
							{
								dynamo = false;
							}
						}
							if(!dynamo&&(sweepCapsule(getPosition() + subUpVector*0.001f,Vector3Nx(ms[b].change),subUpVector)||offset<0.2f))
							{
								failed = false;

								if(Ogre::Math::Abs(ms[b].change.length())>maxDist)
								{
									ms[b].finalPos += NxVector3(subUpVector*0.001f);
									maxDist = Ogre::Math::Abs(ms[b].change.length());
									ms[b].change += NxVector3(subUpVector*0.001f);
									maxIndex = b;
									firstBestDir = firstDir;
									hitDist2 = hitDist;
								}
							}
				}
				else if(setPosition(Vector3Nx(ms[b].finalPos)-subUpVector*(offset*b) + subUpVector*0.05f,false,subUpVector,false))//ms[0].change.magnitude()<0.05f||
				{
					bool dynamo = false;
						if(ms[b].act)
						{
							if(!ms[b].act->isStaticOrKinematicObject())
							{
								dynamo = false;
							}
						}
							if(!dynamo&&(sweepCapsule(getPosition() + subUpVector*0.05f,Vector3Nx(ms[b].change),subUpVector)||offset<0.2f))
							{
								failed = false;

								if(Ogre::Math::Abs(ms[b].change.length())>maxDist)
								{
									ms[b].finalPos += NxVector3(subUpVector*0.05f);
									maxDist = Ogre::Math::Abs(ms[b].change.length());
									ms[b].change += NxVector3(subUpVector*0.05f);
									maxIndex = b;
									firstBestDir = firstDir;
									hitDist2 = hitDist;
								}
							}
				}

				////////////////////////////////////////
				////TRY AN OFFSET ON A PERPENDICULAR VECTOR TO THE MOVE!
				////////////////////////////////////////

				else if(slidingE&&setPosition(Vector3Nx(ms[b].finalPos)-subUpVector*(offset*b) + Vector3Nx(perpendicular),false,subUpVector,false))//ms[0].change.magnitude()<0.05f||
				{
					bool dynamo = false;
						if(ms[b].act)
						{
							if(!ms[b].act->isStaticOrKinematicObject())
							{
								dynamo = false;
							}
						}
							if(!dynamo&&(sweepCapsule(getPosition() + Vector3Nx(perpendicular),Vector3Nx(ms[b].change),subUpVector)||offset<0.2f))
							{
								failed = false;
								eh = true;
								if(Ogre::Math::Abs(ms[b].change.length())>maxDistEh)
								{
									//std::cout<<"BUMP!\n";
									ms[b].finalPos += perpendicular;
									maxDistEh = Ogre::Math::Abs(ms[b].change.length());
									ms[b].change += perpendicular;
									maxIndexEh = b;
									firstBestDir = firstDir;
									hitDist2 = hitDist;
								}
							}
				}

				/// and inverse it!
				else if(slidingE&&setPosition(Vector3Nx(ms[b].finalPos)-subUpVector*(offset*b) - Vector3Nx(perpendicular),false,subUpVector,false))//ms[0].change.magnitude()<0.05f||
				{
					bool dynamo = false;
						if(ms[b].act)
						{
							if(!ms[b].act->isStaticOrKinematicObject())
							{
								dynamo = false;
							}
						}
							if(!dynamo&&(sweepCapsule(getPosition() - Vector3Nx(perpendicular),Vector3Nx(ms[b].change),subUpVector)||offset<0.2f))
							{
								failed = false;
								eh = true;
								if(Ogre::Math::Abs(ms[b].change.length())>maxDistEh)
								{
									//std::cout<<"BUMP!\n";
									ms[b].finalPos -= (perpendicular);
									maxDistEh = Ogre::Math::Abs(ms[b].change.length());
									ms[b].change -= (perpendicular);
									maxIndexEh = b;
									firstBestDir = firstDir;
									hitDist2 = hitDist;
								}
							}
				}

				if(failed&&slidingE)
				{
					//std::cout<<"failure...\n";
				}

				///////////////
				////END OF LOOP
				///////////////
				b++;
			}
			/*float mags2;
			/// check the max index-ed object for movement, if not 100%, throw in another pass (I will have to disable per primitve sliding after this...)
			if(maxIndex>0)
			{
				NxVec3 motionDirec = ms[maxIndex].finalPos - cctp[maxIndex]->getPosition();
				mags2 = motionDirec.magnitude();
			}
			else
			{
				mags2 = 0.0;
			}

			if(mags2<mags*0.975&&slidingE)
			{
				almost_done = true;
				//setCollision(false);
				//bool sucessa = setPosition(ms[maxIndex].finalPos-subUpVector*(offset*maxIndex),true,subUpVector,false);
				NxSweepQueryHit sqh = sweepCapsuleDetail(getPosition(),dir,getUpVector());

				if(sqh.hitShape)
				{
					if(mags2>0)
					{
						NxVec3 svect = dir - (dir.dot(sqh.normal))*sqh.normal;//
						//NxVec3 originalFinal = ms[maxIndex].finalPos;
						NxVec3 total;
						total = ((svect) + (ms[maxIndex].change));
						if(total.magnitude()>dir.magnitude())
						{
							total.normalize();
							total*=dir.magnitude();
						}
						/// will this work???
						dir = total;
					}
					else
					{
						NxVec3 svect = dir - (dir.dot(sqh.normal))*sqh.normal;//
						//NxVec3 originalFinal = ms[maxIndex].finalPos;
						NxVec3 total;
						total = (svect);
						if(total.magnitude()>dir.magnitude())
						{
							total.normalize();
							total*=dir.magnitude();
						}
						/// will this work???
						dir = total;
					}
				}
				else
				{
					break;// no collision, no normal, this sucks...
				}
				//setCollision(false);
				//if(!sucessa)
				//{
				//	break;
				//}
				//else
				//{
				//}
			}
			else
			{
				break;
			}


		}*/

		///////////////////
		////ACTUAL MOVEMENT
		///////////////////

		bool tempp = false;
		if(maxIndex>-1&&maxIndex<numberPrimitives||eh)
		{
			tempp = true;

			///////////////////
			//////POSITIONING
			///////////////////
			bool sucess = false;
			if(maxIndex>-1&&maxIndex<numberPrimitives)
			{

				setCollision(false);
				NxVec3 pso = position;
				//setPosition(position,false,upVector,true);
				sucess = setPosition(Vector3Nx(ms[maxIndex].finalPos)-subUpVector*(offset*maxIndex),true,subUpVector,false);
				pso -= position;
				NxVec3 start = pso;
				start.setY(ms[maxIndex].point.y);

				//std::cout<<"Height: "<<(ms[maxIndex].finalPos-NxVector3(subUpVector)*(offset*maxIndex)).y<<"\n";


				btRigidBody* ugh =0;
				SweepClosestCall ht;
				bool slider = false;
			}
			else if(eh)/// if there wasn't a better route.. then... eh...
			{
				//std::cout<<"EH??????\n";
				setCollision(false);
				NxVec3 pso = position;
				//setPosition(position,false,upVector,true);
				sucess = setPosition(Vector3Nx(ms[maxIndexEh].finalPos)-subUpVector*(offset*maxIndexEh),true,subUpVector,false);
				pso -= position;
				NxVec3 start = pso;
				start.setY(ms[maxIndexEh].point.y);


				btRigidBody* ugh =0;
				SweepClosestCall ht;
				bool slider = false;
			}



			//if(ms[1].hit.hitShape&&slidingE)
			//{
			//	std::cout<<"normal: "<<ms[maxIndex].hit.normal.x<<" "<<ms[maxIndex].hit.normal.y<<" "<<ms[maxIndex].hit.normal.z<<"\n";
			//}


			/////////////////
			////FINAL CHECK...
			/////////////////

			if(!sucess)
			{
				//std::cout<<"Ah s---\n";
			}
			else
			{

			}
			setCollision(true);
		}
		else
		{

			// since I actually thought this system out (unlike past iterations) no resetting is necessary, and so nothing is done here.
		}

		/*if(slidingE)
		{

			/// no collisions for now, I need to perfect the basic rig *DONE!

			NxVec3 intermed = position - position001;

			bool dynamic = false;
			if(maxIndex>-1&&maxIndex<numberPrimitives)
			{
				if(ms[maxIndex].act)
				{
					if(!ms[maxIndex].act->isStaticOrKinematicObject())
					{
						dynamic = true;
					}
				}
			}

			//NxU32 flags = NX_SF_DYNAMICS;
			//NxSweepQueryHit* result;
			//result = new NxSweepQueryHit [5];
			//sweepCap = NxCapsule(NxSegment(position001,position001+subUpVector*((offset*(numberPrimitives-1)))),radius);
			NxVec3 potential = intermed;
			if(!tempp||intermed.isZero()||dynamic)
			{
				potential = dir;
			}
			potential.normalize();
			potential*=dir.length();

			ConvexCollisionsCallback result = ConvexCollisionsCallback();//position+(subUpVector*(height/2))),position+potential+(subUpVector*(height/2))));
			result.m_collisionFilterGroup = QE::COL_STATICS;
			result.m_collisionFilterMask = QE::COL_DYNAMICS|QE::COL_STATICS;
			sweepCap = NxCapsule(height,radius);
			Ogre::Quaternion quat = Ogre::Vector3(0,1,0).getRotationTo(NxVector3(subUpVector));
			btQuaternion ori = btQuaternion(quat.x,quat.y,quat.z,quat.w);
			btCapsuleShape tempShape = btCapsuleShape(sweepCap.radius,sweepCap.height);
			tempShape.setMargin(0.f);
			Scene->convexSweepTest(&tempShape,btTransform(ori,position+(subUpVector*(height/2))),btTransform(ori,position+potential+(subUpVector*(height/2))),result);


			//unsigned int y = Scene->linearCapsuleSweep(sweepCap,potential,flags,NULL,5,result,NULL,0|1|2|5,0);
			unsigned int y = 5;
			while(y>0)
			{
				y--;
				if(result.m_hitCollisionObject[y])
				{
					NxVec3 length = potential;//potential;//potential - intermed;
					length.normalize();
					length*=dir.length()-intermed.length();//2 m/s
					/// adds the force at a local pos, but by half so it's a bit closer to the center of mass (although the force is in the local coord frame of the actor, not the CMass frame... so issues may arise..)
					//result.m_hitCollisionObject[y]
					if(!result.m_hitCollisionObject[y]->isStaticOrKinematicObject())
					{
					std::cout<<"HIT!\n";

						dynamic_cast<btRigidBody*>(result.m_hitCollisionObject[y])->activate(true);// wake it up...
						dynamic_cast<btRigidBody*>(result.m_hitCollisionObject[y])->applyForce(length*200.5f*18.f,btVector3(0,0,0));
					}
					//result[y].hitShape->getActor().addForceAtPos(length*40.0f,Vector3Nx(result[y].point),NX_IMPULSE,true);//at local pos when I have the time to tweak this all...
				}
			}
			//delete [] result;
		}*/

		//setCollision(true);
	}

	// sets the position (returns false if the overlap fails and the move isn't posible...)
	// this has to stagger all the primitives appropriatly

	// position MUST be from the the center of the lowest primitive
	bool CCTCompound::setPosition(const btVector3& pos,bool set,const btVector3& UP,bool report)
	{
		setCollision(false);

				unsigned int a = 0;//cctp.size();
				while(a<cctp.size())
				{
					//a--;
					if(!set)
					{
						if(!cctp[a]->setPosition(pos + UP*(offset*a),false))
						{
							if(report)
							{

							}
							return false;
						}
					}

					else
					{
						if(!cctp[a]->setPosition(pos + UP*(offset*a),true))
						{
							return false;
						}
					}
					a++;
				}
				if(set)
				{
					position = pos;
				}

			setCollision(true);
			return true;


	}

	/*bool CCTCompound::setPosition(const btVector3& pos,bool set,const btVector3& UP)
	{
		setCollision(false);

				unsigned int a = 0;//cctp.size();
				while(a<cctp.size())
				{
					double oldSW = cctp[a]->skinWidth;
					cctp[a]->skinWidth = 0;
					double oldR = cctp[a]->sph.radius;
					cctp[a]->sph.radius = cctp[a]->radius;
					//a--;
					if(!set)
					{
						if(!cctp[a]->setPosition(pos + UP*(offset*a),false))
						{

							return false;
						}
					}

					else
					{
						if(!cctp[a]->setPosition(pos + UP*(offset*a),true))
						{
							return false;
						}
					}
					cctp[a]->sph.radius = static_cast<float>(oldR);
					cctp[a]->skinWidth = static_cast<float>(oldSW);
					a++;
				}
				if(set)
				{
					position = pos;
				}

			setCollision(true);
			return true;


	}*/

	void CCTCompound::setCollision0(unsigned int except,bool collision)
	{
		/*unsigned int a = 0;
		while(a<cctp.size())
		{
			if(a!=except)
			{
				if(!collision)
				{
					cctp[a]->act->raiseActorFlag(NX_AF_DISABLE_COLLISION);
				}
				else
				{
					cctp[a]->act->clearActorFlag(NX_AF_DISABLE_COLLISION);
				}
			}
			a++;
		}*/
	}

	void CCTCompound::setCollision(bool collision)
	{
		/*unsigned int a = 0;
		while(a<cctp.size())
		{
			if(!collision)
			{
				cctp[a]->act->raiseActorFlag(NX_AF_DISABLE_COLLISION);
			}
			else
			{
				cctp[a]->act->clearActorFlag(NX_AF_DISABLE_COLLISION);
			}
			a++;
		}*/
	}

	// in this one the pos is passed straight from the original pos, so it is the center of the lowest one again
	bool CCTCompound::sweepCapsule(const btVector3& pos,const btVector3& dir,const btVector3& UP)
	{
		SweepClosestCall result;
		result.m_collisionFilterGroup = QE::COL_STATICS;
		result.m_collisionFilterMask = QE::COL_DYNAMICS|QE::COL_STATICS;
		sweepCap = NxCapsule(height,radius);
		Ogre::Quaternion quat = Ogre::Vector3(0,1,0).getRotationTo(NxVector3(UP));
		btQuaternion ori = btQuaternion(quat.x,quat.y,quat.z,quat.w);
		btCapsuleShape tempShape = btCapsuleShape(sweepCap.radius,sweepCap.height);
		Scene->convexSweepTest(&tempShape,btTransform(ori,pos+(UP*(height/2))),btTransform(ori,pos+dir+(UP*(height/2))),result);

		if(result.hasHit())
		{
			return false;
		}

		return true;
	};

	// in this one the pos is passed straight from the original pos, so it is the center of the lowest one again
	SweepClosestCall CCTCompound::sweepCapsuleDetail(const btVector3& pos,const btVector3& dir,const btVector3& UP)
	{
		//NxU32 flags = NX_SF_STATICS|NX_SF_DYNAMICS;
		SweepClosestCall result;
		result.m_collisionFilterGroup = QE::COL_STATICS;
		result.m_collisionFilterMask = QE::COL_DYNAMICS|QE::COL_STATICS;
		sweepCap = NxCapsule(height,radius);
		Ogre::Quaternion quat = Ogre::Vector3(0,1,0).getRotationTo(NxVector3(UP));
		btQuaternion ori = btQuaternion(quat.x,quat.y,quat.z,quat.w);
		btCapsuleShape tempShape = btCapsuleShape(sweepCap.radius,sweepCap.height);
		Scene->convexSweepTest(&tempShape,btTransform(ori,pos+(UP*(height/2))),btTransform(ori,pos+dir+(UP*(height/2))),result);

		return result;
	};


	NxVec3 CCTCompound::setUpVector(const btVector3& newUpa,double delta)//,double timeSpeed)
	{
		btVector3 newUp = newUpa;
		newUp.normalize();
		bool slidingE = false;
		unsigned int a = cctp.size();
		unsigned int b = 0;
		// very high, the goal is a small or zero angle...
		float maxDist = 50000.0f;
		NxVec3 maxUPV;
		NxVec3 firstBestDir;
		int maxIndex = -1;
		int maxIndex_ = -1;
		Ogre::Vector3 maxATemp = Ogre::Vector3(0,0,0);
		Ogre::Vector3 maxPTemp = Ogre::Vector3(0,0,0);
		std::vector<moveStruct> ms;
		ms.clear();
		bool startTest = false;
		NxVec3 hitDist2;

		// a vector of vectors hehehe...
		std::vector<Vector3> oPoses;
		/// save it all here for the pushing function to use
		unsigned int g = 0;
		while(g<cctp.size())
		{
			oPoses.push_back(NxVector3(cctp[g]->getPosition()));
			g++;
		}

		NxVec3 position001 = position;

		/*if(Ogre::Degree(NxVector3(newUp).angleBetween(Ogre::Vector3(0,1,0)))<Ogre::Degree(0.1f))
		{
			newUp = NxVec3(0,1,0);
		}
		if(Ogre::Degree(NxVector3(newUp).angleBetween(Ogre::Vector3(0,-1,0)))<Ogre::Degree(0.1f))
		{
			newUp = NxVec3(0,-1,0);
		}
		if(Ogre::Degree(NxVector3(newUp).angleBetween(Ogre::Vector3(1,0,0)))<Ogre::Degree(0.1f))
		{
			newUp = NxVec3(1,0,0);
		}
		if(Ogre::Degree(NxVector3(newUp).angleBetween(Ogre::Vector3(-1,0,0)))<Ogre::Degree(0.1f))
		{
			newUp = NxVec3(-1,0,0);
		}*/

		/////////////
		//// SLOWING
		/////////////

		// 225 degrees/second
		float start = 225.0f;
		float end = 40.0f;

		NxVec3 current = getUpVector();
		Ogre::Degree de = Ogre::Degree(NxVector3(current).angleBetween(NxVector3(newUp)));

		Ogre::Degree speed;

		if(de.valueDegrees()>30.0f)
		{
			speed = Ogre::Degree(225.0f*static_cast<Ogre::Real>(delta));
			mUpInterp = 0;
		}
		else
		{
			mUpInterp = ((de.valueDegrees()/30.f)+((1-(de.valueDegrees()/30.f))));
			float mUpInterp2 = (start*(de.valueDegrees()/30.f)+((end*(1-(de.valueDegrees()/30.f)))));
			speed = Ogre::Degree(mUpInterp2*static_cast<Ogre::Real>(delta));
		}

		float dist_ = de.valueDegrees();
		float speed_ = speed.valueDegrees();
		//if(dist_>=179.999f)
		//{
		//	newUp = Vector3Nx(Ogre::Root::getSingletonPtr()->getSceneManager("smgr")->getCamera("MainCam")->getDerivedDirection());
		//	newUp.normalize();
		//	de = Ogre::Degree(NxVector3(current).angleBetween(NxVector3(newUp)));
		//	dist_ = de.valueDegrees();
		//}
		if(dist_<=speed_)
		{
			newUp = newUp;
		}
		else
		{
				/*if(Ogre::Root::getSingletonPtr()->getSceneManager("smgr")->hasSceneNode("QUATERNIONSLERPINGNODE"))
				{
					Ogre::Root::getSingletonPtr()->getSceneManager("smgr")->destroySceneNode("QUATERNIONSLERPINGNODE");
				}
				Ogre::SceneNode* nd = Ogre::Root::getSingletonPtr()->getSceneManager("smgr")->createSceneNode("QUATERNIONSLERPINGNODE");*/

			Ogre::Vector3 rotV = Ogre::Vector3::UNIT_Y;//NxVector3(current);//NxVector3(current.cross(newUp));

			if(NxVector3(current).angleBetween(NxVector3(newUp))>Ogre::Radian(Ogre::Degree(179.999f)))
			{
				newUp+=NxVec3(0.0f,0.0f,0.01f);

				if(NxVector3(current).angleBetween(NxVector3(newUp))>Ogre::Radian(Ogre::Degree(179.999f)))
				{
					newUp+=NxVec3(0.01f,0.0f,0.0f);
					if(NxVector3(current).angleBetween(NxVector3(newUp))>Ogre::Radian(Ogre::Degree(179.999f)))
					{
						newUp+=NxVec3(0.0f,0.01f,0.0f);
					}
				}

			}

			Ogre::Quaternion q1 = Ogre::Quaternion::IDENTITY;//rotV.getRotationTo(NxVector3(current),Ogre::Vector3::UNIT_X);
			Ogre::Quaternion q2 = NxVector3(current).getRotationTo(NxVector3(newUp),Ogre::Vector3::UNIT_X);
			Ogre::Quaternion q3 = Ogre::Quaternion::Slerp(speed_/dist_,q1,q2,true);
			newUp = Vector3Nx(q3*NxVector3(current));
			newUp.normalize();

				/*nd->setOrientation(Ogre::Quaternion::IDENTITY);
				nd->setDirection(NxVector3(current),Ogre::SceneNode::TS_LOCAL,Ogre::Vector3::UNIT_Y);
				Ogre::Quaternion qu1 = nd->getOrientation();
				nd->setOrientation(Ogre::Quaternion::IDENTITY);
				nd->setDirection(NxVector3(newUp),Ogre::SceneNode::TS_LOCAL,Ogre::Vector3::UNIT_Y);
				Ogre::Quaternion qu2 = nd->getOrientation();
				Ogre::Quaternion qu3 = Ogre::Quaternion::Slerp(speed_/dist_,qu1,qu2,true);
				newUp = Vector3Nx(qu3*Ogre::Vector3::UNIT_Y);
				newUp.normalize();*/

				/// consider reworking the above to work in relation to the player view vector (so that it is more predictable from the player's view...

		}

		//////////////
		// MAIN LOOP
		/////////////


		unsigned int c = 0;
		while(c<cctp.size())
		{
			b = 0;
			/// do the loop below...

			///if the up vector at the end is more than a certain margin off from the target, then don't break...

			/// store the up vector and position from each at the end, and the margin, after the loop terminates, find the smallest margin and use it!... ? ... profit!
			b = c;

			unsigned int d = 0;
			while(d<b)
			{
				ms.push_back(moveStruct(NxVec3(0,0,0),NxVec3(0,0,0),btVector3(0,0,0),btVector3(0,0,0),NULL));
				d++;
			}

			while(b==c)
			//while(b<a)
			{

				setCollision(false);
				NxVec3 orPos = cctp[b]->getPosition();
				//rotate about the bottom primitive

				//if(())

				NxVec3 trPos = cctp[c]->getPosition() + newUp*offset*(static_cast<Real>(b-c));

				NxVec3 dir = trPos - orPos;//orPos-trPos;
				float mags = dir.length();
				NxVec3 originalP = cctp[b]->position;

				/*std::cout<<"mag: "<<mags<<"\n";
				if(magnitude==0)
				{

				}*/

				//if(b!=c)
				//{

					if(slidingE)
					{
						ms.push_back(cctp[b]->move(dir*10.0f,slidingE));
					}
					else
					{
						ms.push_back(cctp[b]->move(dir,slidingE));
					}

					NxVec3 hitDist = Vector3Nx(ms[b].change);

					////////////
					////OVERSTEP
					////////////
					/*if(slidingE)
					{
						if(ms[b].change.magnitude()>mags)
						{
							float mgs = ms[b].change.magnitude();
							NxVec3 oldCh = ms[b].change;
							NxVec3 dmy = ms[b].change;
							dmy.normalize();
							ms[b].change.normalize();
							ms[b].change *= mags;
							dmy*=(mgs - mags);
							ms[b].finalPos = originalP + ms[b].change;
						}
					}*/


				//double ab = clock();

					////////////
					// SLIDING
					////////////
					bool slide = true;
					bool slid = false;
					/*if(slidingE)
					{
							if(ms[b].hit.hitShape)
							{
								if(ms[b].change.magnitude()<dir.magnitude())
								{
										NxVec3 svect = dir - (dir.dot(ms[b].hit.normal))*ms[b].hit.normal;//
										NxVec3 originalFinal = ms[b].finalPos;
										NxVec3 total;
										total = ((svect) + (ms[b].change));
										if(total.magnitude()>dir.magnitude())
										{
											total.normalize();
											total*=dir.magnitude();
										}

										//double ab2 = clock();
										moveStruct mv = cctp[b]->move(total,slidingE);
										/// 0.169 seconds???
										//double bc2 = clock();
										//double cd2 = (bc2-ab2)/1000;
										//if(cd2>0.004)
										//{
										//	std::cout<<cd2<<" : "<<b<<" CRAP2!"<<"\n";
										//}

										hitDist = mv.change;
										ms[b].change = mv.change;
										ms[b].finalPos = mv.finalPos;
										ms[b].hit = mv.hit;
										NxVec3 newFinal = ms[b].finalPos;
										NxVec3 svct = newFinal - originalFinal;
										slid = true;
									//}
								}
							}
					}*/



					NxVec3 firstDir = Vector3Nx(ms[b].change);
					setCollision(false);

				//}

				//////////////////
				//// SWEEP/OVERLAP
				//////////////////
				NxVec3 resultb;
				if(b!=c)
				{
				resultb = Vector3Nx(ms[b].finalPos) - cctp[0]->position;
				resultb.normalize();
				}
				else
				{
				resultb = newUp;
				}
				//if(c==0&&b!=0)
				//{
				//	/// fallthrough....
				//}
				if(setPosition(Vector3Nx(ms[b].finalPos)-newUp*(offset*b),false,resultb,false))
				{
					float angleBet;
					NxVec3 resulta;

					//if(b==c)
					//{
					//	resulta = newUp;
					//	angleBet = 0.0f;
					//}
					if(b==c)
					{
						resulta = newUp;
						angleBet = 0.0f;

						//Ogre::Vector3 posTarget = NxVector3(cctp[c]->getPosition() + newUp*offset*(b-c));

					}
					else
					{
						/// the target position
						resulta = Vector3Nx(ms[b].finalPos) - cctp[c]->getPosition();//cctp[c]->getPosition() + newUp*offset*(b-c);
						Ogre::Vector3 target = NxVector3(newUp);

						//Ogre::Vector3 posTarget = NxVector3(cctp[c]->getPosition() + newUp*offset*(b-c));

						Ogre::Vector3 resultant = NxVector3(resulta);
						angleBet = Ogre::Math::Abs(Ogre::Degree(target.angleBetween(resultant)).valueDegrees());

					}
					/*else if(b<=c)
					{
						Ogre::Vector3 target = NxVector3(newUp);
						if(b==c)
						{
							/// but b isn't 0 since it didn't hit the first if, so set c as 0.
							resulta = ms[b].finalPos - cctp[0]->position;
						}
						else
						{
							resulta = cctp[c]->position - ms[b].finalPos;
						}
						Ogre::Vector3 resultant = NxVector3(resulta);
						angleBet = Ogre::Math::Abs(Ogre::Degree(target.angleBetween(resultant)).valueDegrees());
					}
					else
					{
						Ogre::Vector3 target = NxVector3(newUp);
						resulta = ms[b].finalPos - cctp[c]->position;
						resulta.normalize();
						Ogre::Vector3 resultant = NxVector3(resulta);
						angleBet = Ogre::Math::Abs(Ogre::Degree(target.angleBetween(resultant)).valueDegrees());
					}*/
						if(angleBet<maxDist)
						{
							if(b==c)
							{
								startTest = true;
							}

							if(getPosition().distance2(Vector3Nx(ms[b].finalPos)-newUp*(offset*b))>0.5)
							{

							}
							else
							{

							maxDist = angleBet;
							maxUPV = resulta;
							maxIndex = b;/// the 'winning' segment
							maxIndex_ = c;/// the segment it's rotating about
							maxATemp = NxVector3(resulta);
							maxPTemp = NxVector3(Vector3Nx(ms[b].finalPos)-newUp*(offset*b));

							firstBestDir = firstDir;
							hitDist2 = hitDist;
							}

							///std::cout<<"yay??\n";
						}
				}

				/*double bc = clock();
				double cd = (bc-ab)/1000;
				if(cd>0.004)
				{
					std::cout<<cd<<" : "<<b<<" CRAP!"<<"\n";
				}*/

				///////////////
				////END OF LOOP
				///////////////

				b++;
			}
			ms.clear();
			if(maxDist<0.01f&&maxDist>0)
			{
				///std::cout<<"BREAK!\n";
				break;
			}
		c++;
		}

		//if(offset<0.2f)
		//{
		//	maxIndex = 4;
		//	maxIndex_ = 4;
		//}
		//std::cout<<"Index: "<<maxIndex<<" Index_: "<<maxIndex_<<"\n";

		bool tempp = false;

		if(maxIndex>-1&&maxIndex<numberPrimitives)
		{
			tempp = true;


			/// okay, collisions now
			/// 1. save the original positions at the very start of this function
			/// 2. get the direction from each one from the start to the ideal end (plus a bit) based on the new up vector
			/// 3. try moving each one...
			/// 4. based on the amount that they managed to move and all, push the actors that they collide with..


			//unsigned int h = 0;
			//while(h<cctp.size())
			//{
			//	if(h!=maxIndex_)
			//	{
			//		//std::cout<<"h = "<<h<<"\n";
			//		NxVec3 orPos = Vector3Nx(oPoses[h]);//cctp[h]->getPosition();
			//		NxVec3 trPos = cctp[maxIndex_]->getPosition() + newUp*offset*(static_cast<Real>(h)-maxIndex_);
			//		NxVec3 idealDir = trPos - orPos;
			//		idealDir*=5.1f;

			//		moveStruct tstr = cctp[h]->move(idealDir,false);

			//		if(tstr.act)
			//		{
			//				if(tstr.act->isStaticOrKinematicObject())
			//				{
			//					NxVec3 length = idealDir;//potential;//potential - intermed;
			//					length.normalize();
			//					length*=idealDir.length()-btVector3(Vector3Nx(tstr.finalPos)-Vector3Nx(oPoses[h])).length();//2 m/s
			//					/// adds the force at a local pos, but by half so it's a bit closer to the center of mass (although the force is in the local coord frame of the actor, not the CMass frame... so issues may arise..)
			//					
			//					
			//					//tstr.act->addForceAtPos(length*65.0f,(tstr.point),NX_IMPULSE,true);//at local pos when I have the time to tweak this all...
			//					///std::cout<<"REALLY??? force added??!! yay!\n";
			//				}
			//		}
			//	}

			//	++h;
			//}




			///////////////////
			//////POSITIONING
			///////////////////
			setCollision(false);
			NxVec3 pso = position;
			//bool sucess = setPosition(ms[maxIndex].finalPos-newUp*(offset*maxIndex),true,upVector,false);
			//hmm...

			///if(getPosition().distanceSquared(Vector3Nx(maxPTemp))>0.5)
			///{

			///}

			//std::cout<<"setPos: "<<maxPTemp.x<<" "<<maxPTemp.y<<" "<<maxPTemp.z<<"\n";
			//std::cout<<"setUp: "<<maxATemp.x<<" "<<maxATemp.y<<" "<<maxATemp.z<<"\n";

			bool sucess = setPosition(Vector3Nx(maxPTemp),true,Vector3Nx(maxATemp),false);
			pso -= position;

			//std::cout<<"Segment: "<<maxIndex<<" About Segment "<<maxIndex_<<"\n";

			//NxVec3 start = pso;
			//start.y = ms[maxIndex].hit.point.y;

			upVector = maxUPV;

			/////////////////
			////FINAL CHECK...
			/////////////////

			if(!sucess)
			{
				//std::cout<<"Ah s---\n";
			}
			else
			{

			}

			/*if(slidingE)
			{
				NxVec3 intermed = position - position001;

				bool dynamic = false;
				if(maxIndex>-1&&maxIndex<numberPrimitives)
				{
					if(ms[maxIndex].act)
					{
						if(ms[maxIndex].act->isDynamic())
						{
							dynamic = true;
						}
					}
				}

				NxU32 flags = NX_SF_DYNAMICS;
				NxSweepQueryHit* result;
				result = new NxSweepQueryHit [5];
				sweepCap = NxCapsule(NxSegment(position001,position001+maxUPV*((offset*numberPrimitives-1))),radius);
				NxVec3 potential = intermed;
				if(!tempp||intermed.isZero()||dynamic)
				{
					potential = dir;
				}
				c
				potential.normalize();
				potential*=dir.magnitude();
				unsigned int y = Scene->linearCapsuleSweep(sweepCap,potential,flags,NULL,5,result,NULL,0|1|2|5,0);

				while(y>0)
				{
					y--;
					if(result[y].hitShape)
					{
						NxVec3 length = potential;//potential;//potential - intermed;
						length.normalize();
						length*=dir.magnitude()-intermed.magnitude();//2 m/s
						/// adds the force at a local pos, but by half so it's a bit closer to the center of mass (although the force is in the local coord frame of the actor, not the CMass frame... so issues may arise..)
						result[y].hitShape->getActor().addForceAtPos(length*40.0f,(result[y].point),NX_IMPULSE,true);//at local pos when I have the time to tweak this all...
					}
				}
				delete [] result;
			}*/

			setCollision(true);
		}
		else
		{
			maxUPV = upVector;
		}
		setCollision(true);

		return maxUPV;
	};

}
