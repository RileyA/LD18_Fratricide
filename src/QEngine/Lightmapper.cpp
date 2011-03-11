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
#include "Lightmapper.h"
#include "Engine.h"
#include "LogicObject.h"
#include "Logger.h"

#include "ScriptManager.h"
#include "ChaiWrapper.h"

#include "GfxUtilities.h"

#include "Halton.h"
#include "QEMath.h"

namespace QE
{

	inline Ogre::uint8 getPixel(Ogre::uint8* img,int xa,int ya,int offs,bool& bounds,int wa=512)
	{
		if(xa<0||ya<0||xa>=wa||ya>=wa)
		{
			bounds = false;
			return 0;
		}
		bounds = true;
		return *(img+xa*4+ya*4*wa+offs);
	}

	inline void blurAndModulateChannel(Ogre::uint8* out,Ogre::uint8* in,int x,int y,int cc,int w,int radius,unsigned int offset)
	{
		//cc=4;
		//x*=cc;
		//y*=cc;

		Real total = 0.f;
		int count = 0;
		//int cp = x+y*w;
		bool bounds = true;
		Real temp;

			for(int _y=0;_y<(radius*2+1);++_y)
			{
				for(int _x=0;_x<(radius*2+1);++_x)
				{
					int x_ = (_x-radius);
					int y_ = (_y-radius);
					temp = getPixel(in,x+x_,y+y_,offset,bounds);
					if(bounds)//&&temp!=241)
					{
						total+=temp/255.f;
						++count;
					}
				}
			}

		total/=count;

		//Real value1 = *(in+x+y*w+offset);
		//value1/=255.f;
		Real value2 = getPixel(out,x,y,offset,bounds);
		value2*=total;
		*(out+x*4+y*4*w+offset) = static_cast<Ogre::uint8>(value2);
	}

	inline void modulateChannel(Ogre::uint8* out,Ogre::uint8* in,int x,int y,int cc,int w,int radius,unsigned int offset)
	{
		bool bounds = false;
		Real value1 = getPixel(in,x,y,offset,bounds);
		//value1/=255.f;
		Real value2 = getPixel(out,x,y,offset,bounds);
		value2*=value1;
		*(out+x*4+y*4*w+offset) = static_cast<Ogre::uint8>(value1);
	}

	void swap(Real& val1,Real& val2)
	{
		Real temp = val1;
		val1 = val2;
		val2 = temp;
	}

	Ogre::Vector2 segIntersection(Ogre::Vector2 ptA1, Ogre::Vector2 ptA2, Ogre::Vector2 ptB1, Ogre::Vector2 ptB2)
	{
		bool infiniSlopeA = false;
		bool infiniSlopeB = false;
		Real segASlope;
		Real segBSlope;
		Real segAIntercept;
		Real segBIntercept;
		Real xInter;
		Real yInter;

		if(ptA2.x-ptA1.x==0)
		{
			segASlope = 5000;
			segAIntercept = 0;
			infiniSlopeA = true;
		}
		else
		{
			segASlope = (ptA2.y-ptA1.y)/(ptA2.x-ptA1.x);
			segAIntercept = ptA1.y-(segASlope*ptA1.x);
		}

		if(ptB2.x-ptB1.x==0)
		{
			segBSlope = 5000;
			segBIntercept = 0;
			infiniSlopeB = true;
		}
		else
		{
			segBSlope = (ptB2.y-ptB1.y)/(ptB2.x-ptB1.x);
			segBIntercept = ptB1.y-(segBSlope*ptB1.x);
		}

		if(infiniSlopeA&&infiniSlopeB)
		{
			std::cout<<"NOT GOOD\n";
			return Ogre::Vector2(0,0);
		}

		if(!infiniSlopeA&&!infiniSlopeB)
		{
			xInter = (segAIntercept-segBIntercept)/(segBSlope-segASlope);
			yInter = segASlope*xInter+segAIntercept;
			return Ogre::Vector2(xInter,yInter);
		}

		if(infiniSlopeA)
		{
			xInter = ptA1.x;
			yInter = segBSlope*xInter+segBIntercept;
			return Ogre::Vector2(xInter,yInter);
		}
		else if(infiniSlopeB)
		{
			xInter = ptB1.x;
			yInter = segASlope*xInter+segAIntercept;
			return Ogre::Vector2(xInter,yInter);
		}
		else
		{
			return Ogre::Vector2(0,0);
		}


		  /*float bx = ptA2.x - ptA1.x;
		  float by = ptA2.y - ptA1.y;
		  float dx = ptB2.x - ptB1.x;
		  float dy = ptB2.y - ptB1.y;

		  float b_dot_d_perp = bx * dy - by * dx;

		  if(b_dot_d_perp == 0) return Ogre::Vector2(0,0);

		  float cx = ptB1.x - ptA1.x;
		  float cy = ptB1.y - ptA1.y;
		  float t = (cx * dy - cy * dx) / b_dot_d_perp;

		  if(t < 0 || t > 1) return Ogre::Vector2(0,0);

		  float u = (cx * by - cy * bx) / b_dot_d_perp;

		  if(u < 0 || u > 1) return Ogre::Vector2(0,0);

		  return Ogre::Vector2(ptA1.x+t*bx, ptA1.y+t*by);*/
	}

	void expandTriangle(Ogre::Vector2 points[3],unsigned int nrPixels,Real pixelSize,Ogre::Vector2& out1,Ogre::Vector2& out2,Ogre::Vector2& out3)
	{
		Ogre::Vector2 center = (points[0]+points[1]+points[2])/3;
		Ogre::Vector2 AB = (points[0] + points[1])/2;
		Ogre::Vector2 BC = (points[1] + points[2])/2;
		Ogre::Vector2 CA = (points[2] + points[0])/2;

		Ogre::Vector2 ABout = AB - center;
		ABout.normalise();
		ABout*=(nrPixels*pixelSize);

		Ogre::Vector2 BCout = BC - center;
		BCout.normalise();
		BCout*=(nrPixels*pixelSize);

		Ogre::Vector2 CAout = CA - center;
		CAout.normalise();
		CAout*=(nrPixels*pixelSize);

		Ogre::Vector2 A_AB = points[0] + ABout;
		Ogre::Vector2 B_AB = points[1] + ABout;

		Ogre::Vector2 B_BC = points[1] + BCout;
		Ogre::Vector2 C_BC = points[2] + BCout;

		Ogre::Vector2 C_CA = points[2] + CAout;
		Ogre::Vector2 A_CA = points[0] + CAout;

		Ogre::Vector2 A = segIntersection(B_AB,A_AB,C_CA,A_CA);
		if(A==Ogre::Vector2(0,0))
		{
			//std::cout<<"uh oh!!!\n";
		}

		Ogre::Vector2 B = segIntersection(A_AB,B_AB,C_BC,B_BC);
		if(B==Ogre::Vector2(0,0))
		{
			//std::cout<<"uh oh!!!\n";
		}

		Ogre::Vector2 C = segIntersection(B_BC,C_BC,A_CA,C_CA);
		if(C==Ogre::Vector2(0,0))
		{
			//std::cout<<"uh oh!!!\n";
		}

		out1 = A;
		out2 = B;
		out3 = C;
	}

	struct pixelPair
	{
		pixelPair(int x_, int y_)
			:x(x_),y(y_)
		{

		}
		pixelPair()
			:x(0),y(0)
		{

		}
		int x;
		int y;
	};

	std::vector<pixelPair> line(Real x0, Real x1, Real y0, Real y1)
	{
		std::vector<pixelPair> pixels;
		 bool steep = abs(y1 - y0) > abs(x1 - x0);
		 if (steep)
		 {
			 swap(x0, y0);
			 swap(x1, y1);
		 }
		 if (x0 > x1)
		 {
			 swap(x0, x1);
			 swap(y0, y1);
		 }
		 int deltax = x1 - x0;
		 int deltay = abs(y1 - y0);
		 int error = deltax / 2;
		 int ystep;
		 int y = y0;
		 if (y0 < y1)
		 {
			ystep = 1;
		 }
		 else
		 {
			ystep = -1;
		 }
		 for (int x = x0; x < x1;++x)
		 {
			 if(steep)
			 {
				pixels.push_back(pixelPair(y,x));
			 }
			 else
			 {
				pixels.push_back(pixelPair(x,y));
			 }

				error = error - deltay;
			 if(error < 0)
			 {
				 y = y + ystep;
				 error = error + deltax;
			 }
		 }
		 return pixels;
	}

	Real compress(Real in)
	{
		return (in+1)/2;
	}

	Lightmapper::Lightmapper(QEngine* engine,String bucket,String name):LogicObject(engine,bucket,name)
	{
		mType = "Lightmapper";
		mObject = 0;
	}

	Lightmapper::~Lightmapper()
	{
		if(mInitialized)
		{
			deinitialize();
		}
	}

	void Lightmapper::update(double delta)
	{
		// nuthin much to do unless I decide to add a progressive update type thing...
	}

	void Lightmapper::deinitialize()
	{
        mInitialized = false;
		Logger::getSingletonPtr()->logMessage("Lightmapper Destroyed.");
	}

	void Lightmapper::setObject(StaticPhysicsObject* obj)
	{
		if(obj)
		{
			mObject = obj;
		}
	}

	void Lightmapper::calculateTexture()
	{
		if(mObject)
		{
			// do work
			Ogre::Entity *entity = mObject->getRenderedObject()->getEntity();

            // mesh data to retrieve
            size_t vertex_count;
            size_t index_count;
            Ogre::Vector3 *vertices;
            Ogre::Vector2 *texcoords;
            unsigned long *indices;

            // get the mesh information
			getMeshInformation(entity->getMesh(), vertex_count, vertices, texcoords, index_count, indices,
							  entity->getParentNode()->_getDerivedPosition(),
                              entity->getParentNode()->_getDerivedOrientation(),
                              entity->getParentNode()->_getDerivedScale());

			// grab the texture buffer of the second texture of the first mat/pass
			Ogre::TexturePtr tex = static_cast<Ogre::TexturePtr>(Ogre::TextureManager::getSingletonPtr()->getByName(
				entity->getSubEntity(0)->getMaterial()->getBestTechnique()->getPass(0)->getTextureUnitState(1)->getTextureName()));

			Ogre::HardwarePixelBufferSharedPtr texBuf = tex->getBuffer();

			unsigned int texWidth = tex->getWidth();
			unsigned int texHeight = tex->getHeight();

			texBuf->lock(Ogre::HardwareBuffer::HBL_NORMAL);

			// get access to raw texel data
			Ogre::uint8* data = (Ogre::uint8*)texBuf->getCurrentLock().data;

			Ogre::uint8* dataCopy = new Ogre::uint8[texWidth*texHeight*4];
			//dataCopy = (Ogre::uint8*)texBuf->getCurrentLock().data;

			/*for (unsigned int y = 0; y < texHeight; y++)
			{
				for (unsigned int x = 0; x < texWidth; x++)
				{
					*(data+y*texWidth*4+x*4) = 255;// maybe???
					//data++;
					*(data+y*texWidth*4+x*4+1) = 255;// maybe???
					//data++;
					*(data+y*texWidth*4+x*4+2) = 255;// maybe???
					//data++;
					*(data+y*texWidth*4+x*4+3) = 255;// maybe???
					//data++;
				}
			}*/

			// go through every texel...
			for (unsigned int y = 0; y < texHeight; y++)
			{
				for (unsigned int x = 0; x < texWidth; x++)
				{
					*(dataCopy+x*4+(y*texWidth*4)) = 241;// maybe???
					*(dataCopy+x*4+(y*texWidth*4)+1) = 241;// maybe???
					*(dataCopy+x*4+(y*texWidth*4)+2) = 241;// maybe???
					*(dataCopy+x*4+(y*texWidth*4)+3) = 255;// maybe???
					//dataCopy++;
				}
			}

			//int write = 0;


            for (int i = 0; i < static_cast<int>(index_count); i += 3)
			{

			std::map<int,std::map<int,int> > px;
			std::map<int,int> px_filled;
				//if(write==0)
				//{
				//std::cout<<"TRIANGLE: "<<i<<"/"<<index_count<<"\n";

				// do stuff here

				// first off we need to get all of the pixels that fall into this triangle
				//vertices[indices[i]]... i+1...i+2
				//texcoords[same stuffs]

				Ogre::Vector3 normal = (vertices[indices[i]]-vertices[indices[i+1]]).crossProduct(vertices[indices[i+2]]-vertices[indices[i+1]]);
				normal.normalise();
				normal = -normal;

				//Vector3 center = (vertices[indices[i]]+vertices[indices[i+1]]+vertices[indices[i+2]])/3;

				//QE::RaycastReport rep = mEngine->getPhysicsManager()->raycastSimple(Vector3(0,0,0),center,(center).length()*0.95f);

				//std::cout<<"Normal: "<<normal.x<<" "<<normal.y<<" "<<normal.z<<"\n";

				Real maxX = -50000;
				Real minX = 50000;
				Real maxY = -50000;
				Real minY = 50000;

				for(unsigned int j=0;j<3;++j)
				{
					if(texcoords[indices[i+j]].x>maxX)
					{
						maxX = texcoords[indices[i+j]].x;
					}
					if(texcoords[indices[i+j]].x<minX)
					{
						minX = texcoords[indices[i+j]].x;
					}

					if(texcoords[indices[i+j]].y>maxY)
					{
						maxY = texcoords[indices[i+j]].y;
					}
					if(texcoords[indices[i+j]].y<minY)
					{
						minY = texcoords[indices[i+j]].y;
					}
				}

				minX*=texWidth;
				maxX*=texWidth;
				minY*=texWidth;
				maxY*=texWidth;

				minX-=3;
				minY-=3;
				maxX+=3;
				maxY+=3;

				if(maxY>=texWidth)
				{
					maxY = texWidth-1;
				}
				if(maxX>=texWidth)
				{
					maxX = texWidth-1;
				}
				if(minX<0)
				{
					minX = 0;
				}
				if(minY<0)
				{
					minY = 0;
				}

				int uminX = Ogre::Math::Floor(minX);
				int umaxX = Ogre::Math::Ceil(maxX);
				int uminY = Ogre::Math::Floor(minY);
				int umaxY = Ogre::Math::Ceil(maxY);
				//if(true)
				//{

				// Bresenham
				/*std::vector<pixelPair> pp = line(texcoords[indices[i]].x*texWidth,texcoords[indices[i+1]].x*texWidth,texcoords[indices[i]].y*texHeight,texcoords[indices[i+1]].y*texHeight);

				for(unsigned int k=0;k<pp.size();++k)
				{
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4) = 0;
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4+1) = 0;
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4+2) = 0;
				}

				pp = line(texcoords[indices[i]].x*texWidth,texcoords[indices[i+2]].x*texWidth,texcoords[indices[i]].y*texHeight,texcoords[indices[i+2]].y*texHeight);

				for(unsigned int k=0;k<pp.size();++k)
				{
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4) = 0;
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4+1) = 0;
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4+2) = 0;
				}

				pp = line(texcoords[indices[i+2]].x*texWidth,texcoords[indices[i+1]].x*texWidth,texcoords[indices[i+2]].y*texHeight,texcoords[indices[i+1]].y*texHeight);

				for(unsigned int k=0;k<pp.size();++k)
				{
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4) = 0;
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4+1) = 0;
					*(((Ogre::uint8*)texBuf->getCurrentLock().data)+texWidth*(pp[k].y*4)+pp[k].x*4+2) = 0;
				}*/



				Ogre::Vector2 startVectors[3] = {texcoords[indices[i]],texcoords[indices[i+1]],texcoords[indices[i+2]]};

				Ogre::Vector2 A = texcoords[indices[i]];
				Ogre::Vector2 B = texcoords[indices[i+1]];
				Ogre::Vector2 C = texcoords[indices[i+2]];

				//Ogre::Vector2 cent = (A+B+C)/3;

				/*if(i==300)
				{
					std::cout<<"Point A: "<<A.x<<" "<<A.y<<"\n";
					std::cout<<"Point B: "<<B.x<<" "<<B.y<<"\n";
					std::cout<<"Point C: "<<C.x<<" "<<C.y<<"\n";
				}

				//expandTriangle(startVectors,1,1.f/texWidth,A,B,C);

				if(i==300)
				{
					std::cout<<"Point A: "<<A.x<<" "<<A.y<<"\n";
					std::cout<<"Point B: "<<B.x<<" "<<B.y<<"\n";
					std::cout<<"Point C: "<<C.x<<" "<<C.y<<"\n";
				}*/



				//Real scale =

				//while(texWidth>)
				// Compute vectors
				Ogre::Vector2 v0 = C - A;
				Ogre::Vector2 v1 = B - A;

				Real dot00 = v0.dotProduct(v0);
				Real dot01 = v0.dotProduct(v1);
				Real dot11 = v1.dotProduct(v1);

				Real invDenom = 1 / (dot00 * dot11 - dot01 * dot01);

				//std::vector<pixelPair> px;

				Ogre::uint8 shadowColorR = 80;
				Ogre::uint8 shadowColorG = 80;
				Ogre::uint8 shadowColorB = 80;

				Halton halt = Halton();

				for ( int y = uminY; y < umaxY; y++)
				{
					for ( int x = uminX; x < umaxX; x++)
					{
						// grab the normal (sign may vary but it's irrelevant...
						//Ogre::Vector3 normal = v0.crossProduct(v1);


						//Ogre::Plane pln = Ogre::Plane(normal,0);

						Ogre::Vector2 P = Ogre::Vector2((static_cast<Real>(x)+0.5f)/static_cast<Real>(texWidth),(static_cast<Real>(y)+0.5f)/static_cast<Real>(texHeight));

						Ogre::Vector2 v2 = P - A;

						// Compute dot products
						Real dot02 = v0.dotProduct(v2);
						Real dot12 = v1.dotProduct(v2);

						//dot00 = dot(v0, v0)
						//dot01 = dot(v0, v1)
						//dot02 = dot(v0, v2)
						//dot11 = dot(v1, v1)
						//dot12 = dot(v1, v2)

						// Compute barycentric coordinates
						Real u = (dot11 * dot02 - dot01 * dot12) * invDenom;
						Real v = (dot00 * dot12 - dot01 * dot02) * invDenom;

						//std::cout<<"u="<<u<<" v="<<v<<"\n";

						unsigned int nrRays = 25;


						// Check if point is in triangle
						if((u > 0) && (v > 0) && (u + v < 1))
						{
							px[x][y] = 1;//.push_back(x,y);
							//Vector3 center = (vertices[indices[i]]+vertices[indices[i+1]]+vertices[indices[i+2]])/3;
							Vector3 center2 = vertices[indices[i]]+v*(vertices[indices[i+1]]-vertices[indices[i]])+u*(vertices[indices[i+2]]-vertices[indices[i]]);
							Real hits = 0.f;

							for(unsigned int i=0;i<nrRays;++i)
							{
								Ogre::Quaternion quat = Ogre::Vector3(0,0,0).getRotationTo(normal);

								Ogre::Vector3 ranVect(0,0,0);

									float hout[2];
									halt.GetNext(hout);

									//hout[0] = (hout[0]+randFloat(0.f,1.f))/2.f;
									//hout[1] = (hout[1]+randFloat(0.f,1.f))/2.f;


								  float xi = Ogre::Math::Cos(Ogre::Radian(2*Ogre::Math::PI*hout[0]))*Ogre::Math::Sqr(1-hout[1]);//randFloat(-1.f, 1.f); // random float between -1 and 1
								  float zi = Ogre::Math::Sin(Ogre::Radian(2*Ogre::Math::PI*hout[0]))*Ogre::Math::Sqr(1-hout[1]);//randFloat(-1.f, 1.f);
								  float yi = hout[2];//randFloat(-1.f, 1.f);
								  //if (xi * xi + yi * yi + zi * zi > 1) continue; // ignore ones outside unit

								   // sphere



								   ranVect = Vector3(xi, yi, zi);
								   ranVect.normalise();// success!

								Vector3 targetDir = ranVect;//normal.randomDeviant(Ogre::Radian(Ogre::Degree(80.0f)));
								//targetDir.normalize();
								//targetDir*=0.5f;
								QE::RaycastReport rep = mEngine->getPhysicsManager()->raycastSimple(center2+targetDir*0.01f,targetDir,2.f);
								if(rep.hit)
								{
									hits+=1.f;
								}
							}

							/*QE::RaycastReport rep = mEngine->getPhysicsManager()->raycastSimple(Vector3(1.f,0.f,0.f),center2,(center2).length()*0.999f);
							if(rep.hit){++hits;};
							rep = mEngine->getPhysicsManager()->raycastSimple(Vector3(-0.1f,0.f,0.f),center2,(center2).length()*0.999f);
							if(rep.hit){++hits;};
							rep = mEngine->getPhysicsManager()->raycastSimple(Vector3(0.f,0.1f,0.f),center2,(center2).length()*0.999f);
							if(rep.hit){++hits;};
							rep = mEngine->getPhysicsManager()->raycastSimple(Vector3(0.f,-0.1f,0.f),center2,(center2).length()*0.999f);
							if(rep.hit){++hits;};
							rep = mEngine->getPhysicsManager()->raycastSimple(Vector3(0.f,0.f,0.1f),center2,(center2).length()*0.999f);
							if(rep.hit){++hits;};
							rep = mEngine->getPhysicsManager()->raycastSimple(Vector3(0.f,0.f,-0.1f),center2,(center2).length()*0.999f);
							if(rep.hit){++hits;};*/

							//std::cout<<"Coloring!\n";
							// if so then paint it the color of the normal!
							if(hits>0.f)
							{
								px[x][y] = 2;
								*(dataCopy+texWidth*(y*4)+x*4) = 155-(hits/static_cast<float>(nrRays))*155+50;//255-((255-shadowColorR)*(hits/6));//static_cast<Ogre::uint8>(compress(normal.x)*255);//r
								*(dataCopy+texWidth*(y*4)+x*4+1) = 155-(hits/static_cast<float>(nrRays))*155+50;//255-((255-shadowColorG)*(hits/6));//static_cast<Ogre::uint8>(compress(normal.y)*255);//g
								*(dataCopy+texWidth*(y*4)+x*4+2) = 155-(hits/static_cast<float>(nrRays))*155+50;//255-((255-shadowColorB)*(hits/6));//static_cast<Ogre::uint8>(compress(normal.z)*255);//b
							}
							else
							{
								*(dataCopy+texWidth*(y*4)+x*4) = 205;//static_cast<Ogre::uint8>(compress(normal.x)*255);//r
								*(dataCopy+texWidth*(y*4)+x*4+1) = 205;//static_cast<Ogre::uint8>(compress(normal.y)*255);//g
								*(dataCopy+texWidth*(y*4)+x*4+2) = 205;//static_cast<Ogre::uint8>(compress(normal.z)*255);//b
							}
								//*(dataCopy+texWidth*(y*4)+x*4) = static_cast<Ogre::uint8>(compress(normal.x)*255);//r
								//*(dataCopy+texWidth*(y*4)+x*4+1) = static_cast<Ogre::uint8>(compress(normal.y)*255);//g
								//*(dataCopy+texWidth*(y*4)+x*4+2) = static_cast<Ogre::uint8>(compress(normal.z)*255);//b
							//use the barycentric coords to interpolate the texcoords
							//position
						}
					}
				}


					// post process the extra pixels in!
				bool padPixels = false;
				unsigned int nrPx = 2;
				if(padPixels)
				{
					std::vector<int> pixelsToFill_1;
					std::vector<int> pixelsToFill_2;

					std::map<int,std::map<int,int> >::iterator iter = px.begin();;

						while(iter!=px.end())
						{
							std::map<int,int>::iterator iter2 = iter->second.begin();
							while(iter2!=iter->second.end())
							{
								//if(iter2->second==2)
								//{
								bool tmp = false;
								Ogre::uint8 redV = getPixel(dataCopy,iter->first,iter2->first,0,tmp);
								Ogre::uint8 greenV = getPixel(dataCopy,iter->first,iter2->first,1,tmp);
								Ogre::uint8 blueV = getPixel(dataCopy,iter->first,iter2->first,2,tmp);

									unsigned int x_ = iter->first+1;//1
									unsigned int y_ = iter2->first;
									if(getPixel(dataCopy,x_,y_,0,tmp)==241){
									*(dataCopy+x_*4+(y_*4*texWidth)) = redV;
									*(dataCopy+x_*4+(y_*4*texWidth+1)) = greenV;
									*(dataCopy+x_*4+(y_*4*texWidth+2)) = blueV;}

									x_-=2;//-1
									if(getPixel(dataCopy,x_,y_,0,tmp)==241){
									*(dataCopy+x_*4+(y_*4*texWidth)) = redV;
									*(dataCopy+x_*4+(y_*4*texWidth+1)) = greenV;
									*(dataCopy+x_*4+(y_*4*texWidth+2)) = blueV;}

									x_+=1;//0
									y_+=1;//1
									if(getPixel(dataCopy,x_,y_,0,tmp)==241){
									*(dataCopy+x_*4+(y_*4*texWidth)) = redV;
									*(dataCopy+x_*4+(y_*4*texWidth+1)) = greenV;
									*(dataCopy+x_*4+(y_*4*texWidth+2)) = blueV;}

									y_-=2;//-1
									if(getPixel(dataCopy,x_,y_,0,tmp)==241){
									*(dataCopy+x_*4+(y_*4*texWidth)) = redV;
									*(dataCopy+x_*4+(y_*4*texWidth+1)) = greenV;
									*(dataCopy+x_*4+(y_*4*texWidth+2)) = blueV;}

									y_+=2;//1
									x_+=1;//1
									if(getPixel(dataCopy,x_,y_,0,tmp)==241){
									*(dataCopy+x_*4+(y_*4*texWidth)) = redV;
									*(dataCopy+x_*4+(y_*4*texWidth+1)) = greenV;
									*(dataCopy+x_*4+(y_*4*texWidth+2)) = blueV;}

									y_-=2;//-1
									x_-=2;//-1
									if(getPixel(dataCopy,x_,y_,0,tmp)==241){
									*(dataCopy+x_*4+(y_*4*texWidth)) = redV;
									*(dataCopy+x_*4+(y_*4*texWidth+1)) = greenV;
									*(dataCopy+x_*4+(y_*4*texWidth+2)) = blueV;}

									x_+=2;//1
									//y_; //-1
									if(getPixel(dataCopy,x_,y_,0,tmp)==241){
									*(dataCopy+x_*4+(y_*4*texWidth)) = redV;
									*(dataCopy+x_*4+(y_*4*texWidth+1)) = greenV;
									*(dataCopy+x_*4+(y_*4*texWidth+2)) = blueV;}

									y_+=2;//1
									x_-=2;//-1
									if(getPixel(dataCopy,x_,y_,0,tmp)==241){
									*(dataCopy+x_*4+(y_*4*texWidth)) = redV;
									*(dataCopy+x_*4+(y_*4*texWidth+1)) = greenV;
									*(dataCopy+x_*4+(y_*4*texWidth+2)) = blueV;}

								//}
								++iter2;
							}

							++iter;
						}
						/*for(unsigned int m=0;m<pixelsToFill_1.size();++m)
						{
							*(dataCopy+pixelsToFill_1[m]) = shadowColorR;
							*(dataCopy+pixelsToFill_1[m]+1) = shadowColorG;
							*(dataCopy+pixelsToFill_1[m]+2) = shadowColorB;
						}

						if(nrPx>=2)
						{
							for(unsigned int m=0;m<pixelsToFill_2.size();++m)
							{
								*(dataCopy+pixelsToFill_2[m]) = shadowColorR;
								*(dataCopy+pixelsToFill_2[m]+1) = shadowColorG;
								*(dataCopy+pixelsToFill_2[m]+2) = shadowColorB;
							}
						}*/
					}
				//}
			}

			for (unsigned int y = 0; y < texHeight; y++)
			{
				for (unsigned int x = 0; x < texWidth; x++)
				{
					//blurAndModulatePixel(data,dataCopy,x,y,4,texWidth,2);

					//blurAndModulateChannel(data,dataCopy,x,y,4,texWidth,2,0);
					//blurAndModulateChannel(data,dataCopy,x,y,4,texWidth,2,1);
					//blurAndModulateChannel(data,dataCopy,x,y,4,texWidth,2,2);

					modulateChannel(data,dataCopy,x,y,4,texWidth,2,0);
					modulateChannel(data,dataCopy,x,y,4,texWidth,2,1);
					modulateChannel(data,dataCopy,x,y,4,texWidth,2,2);

					//blurAndModulate(data,dataCopy,x,y,4,texWidth,2);
					//blurAndModulate(data,dataCopy,x,y,4,texWidth,2);

					//*(data+x*4+(y*texWidth*4)) /= *(dataCopy+x*4+(y*texWidth*4));// maybe???
					//*(data+x*4+(y*texWidth*4)+1) /= *(dataCopy+x*4+(y*texWidth*4)+1);// maybe???
					//*(data+x*4+(y*texWidth*4)+2) /= *(dataCopy+x*4+(y*texWidth*4)+2);// maybe???
					//*(data+x*4+(y*texWidth*4)+3) /= *(dataCopy+x*4+(y*texWidth*4)+3);// maybe???
					//dataCopy++;
				}
			}



			delete[] dataCopy;

			Ogre::PixelFormat pf = Ogre::PF_R8G8B8A8;
			//Ogre::PixelBox pb(mWidth, mHeight, 1, pf, data);
			//texBuf->blitToMemory(pb);
			Ogre::Image().loadDynamicImage(data, texWidth, texHeight, 1, pf, false, 1, 0).save("lightmapTest.png");

			texBuf->unlock();

			// free the verticies and indicies memory
            delete[] vertices;
            delete[] indices;
            delete[] texcoords;
		}
	}

	Lightmapper* Lightmapper::toLightmapper(LogicObject* obj)
	{
		return dynamic_cast<Lightmapper*>(obj);
	}

	void Lightmapper::registerClass(ScriptManager* scriptMgr)
	{
		scriptMgr->getWrapper()->getChai()->add(chaiscript::user_type<Lightmapper>(), "Lightmapper");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&Lightmapper::toLightmapper), "toLightmapper");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&Lightmapper::createLightmapper), "createLightmapper");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&Lightmapper::setObject), "setObject");
		scriptMgr->getWrapper()->getChai()->add(chaiscript::fun(&Lightmapper::calculateTexture), "calculateTexture");
	}

	Lightmapper* Lightmapper::createLightmapper(QEngine* engine,String bucket,String name)
	{
		return new Lightmapper(engine,bucket,name);
	}


}


/*								if(iter->second.find(iter2->first+1)==iter->second.end()&&px_filled.find(texWidth*((iter2->first+1)*4)+(iter->first)*4)==px_filled.end())
								{
									if(iter2->second==2)
									{
										pixelsToFill_1.push_back(texWidth*((iter2->first+1)*4)+(iter->first)*4);
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first)*4) /= shadowColorR;
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first)*4+1) /= shadowColorG;
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first)*4+2) /= shadowColorB;
										//px[iter->first][iter2->first+1] = 2;
										px_filled[texWidth*((iter2->first+1)*4)+(iter->first)*4] = 2;
									}
								}
								// -y
								if(iter->second.find(iter2->first-1)==iter->second.end()&&px_filled.find(texWidth*((iter2->first-1)*4)+(iter->first)*4)==px_filled.end())
								{
									if(iter2->second==2)
									{
										pixelsToFill_1.push_back(texWidth*((iter2->first-1)*4)+(iter->first)*4);
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first)*4) /= shadowColorR;
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first)*4+1) /= shadowColorG;
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first)*4+2) /= shadowColorB;
										//px[iter->first][iter2->first-1] = 2;
										px_filled[texWidth*((iter2->first-1)*4)+(iter->first)*4] = 2;
									}
								}
								// +x
								if(px.find(iter->first+1)==px.end()&&px_filled.find(texWidth*((iter2->first)*4)+(iter->first+1)*4)==px_filled.end())
								{
									if(iter2->second==2)
									{
										pixelsToFill_1.push_back(texWidth*((iter2->first)*4)+(iter->first+1)*4);
										//*(dataCopy+texWidth*((iter2->first)*4)+(iter->first+1)*4) /= shadowColorR;
										//*(dataCopy+texWidth*((iter2->first)*4)+(iter->first+1)*4+1) /= shadowColorG;
										//*(dataCopy+texWidth*((iter2->first)*4)+(iter->first+1)*4+2) /= shadowColorB;
										//px[iter->first+1][iter2->first] = 2;
										px_filled[texWidth*((iter2->first+1)*4)+(iter->first+1)*4] = 2;
									}
								}
								// -x
								if(px.find(iter->first-1)==px.end()&&px_filled.find(texWidth*((iter2->first)*4)+(iter->first-1)*4)==px_filled.end())
								{
									if(iter2->second==2)
									{
										pixelsToFill_1.push_back(texWidth*((iter2->first)*4)+(iter->first-1)*4);
										//*(dataCopy+texWidth*((iter2->first)*4)+(iter->first-1)*4) /= shadowColorR;
										//*(dataCopy+texWidth*((iter2->first)*4)+(iter->first-1)*4+1) /= shadowColorG;
										//*(dataCopy+texWidth*((iter2->first)*4)+(iter->first-1)*4+2) /= shadowColorB;
										//px[iter->first-1][iter2->first] = 2;
										px_filled[texWidth*((iter2->first)*4)+(iter->first-1)*4] = 2;
									}
								}
								// +y +x
								if(iter->second.find(iter2->first+1)==iter->second.end()&&px.find(iter->first+1)==px.end()&&px_filled.find(texWidth*((iter2->first+1)*4)+(iter->first+1)*4)==px_filled.end())
								{
									if(iter2->second==2)
									{
										pixelsToFill_2.push_back(texWidth*((iter2->first+1)*4)+(iter->first+1)*4);
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first+1)*4) /= shadowColorR;
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first+1)*4+1) /= shadowColorG;
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first+1)*4+2) /= shadowColorB;
										//px[iter->first+1][iter2->first+1] = 2;
										px_filled[texWidth*((iter2->first+1)*4)+(iter->first+1)*4] = 2;
									}
								}
								// -y -x
								if(iter->second.find(iter2->first-1)==iter->second.end()&&px.find(iter->first-1)==px.end()&&px_filled.find(texWidth*((iter2->first-1)*4)+(iter->first-1)*4)==px_filled.end())
								{
									if(iter2->second==2)
									{
										pixelsToFill_2.push_back(texWidth*((iter2->first-1)*4)+(iter->first-1)*4);
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first-1)*4) /= shadowColorR;
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first-1)*4+1) /= shadowColorG;
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first-1)*4+2) /= shadowColorB;
										//px[iter->first-1][iter2->first-1] = 2;
										px_filled[texWidth*((iter2->first-1)*4)+(iter->first-1)*4] = 2;
									}
								}
								// -y +x
								if(iter->second.find(iter2->first-1)==iter->second.end()&&px.find(iter->first+1)==px.end()&&px_filled.find(texWidth*((iter2->first-1)*4)+(iter->first+1)*4)==px_filled.end())
								{
									if(iter2->second==2)
									{
										pixelsToFill_2.push_back(texWidth*((iter2->first-1)*4)+(iter->first+1)*4);
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first+1)*4) /= shadowColorR;
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first+1)*4+1) /= shadowColorG;
										//*(dataCopy+texWidth*((iter2->first-1)*4)+(iter->first+1)*4+2) /= shadowColorB;
										//px[iter->first+1][iter2->first-1] = 2;
										px_filled[texWidth*((iter2->first-1)*4)+(iter->first+1)*4] = 2;
									}
								}
								// +y -x
								if(iter->second.find(iter2->first+1)==iter->second.end()&&px.find(iter->first-1)==px.end()&&px_filled.find(texWidth*((iter2->first+1)*4)+(iter->first-1)*4)==px_filled.end())
								{
									if(iter2->second==2)
									{
										pixelsToFill_2.push_back(texWidth*((iter2->first+1)*4)+(iter->first-1)*4);
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first-1)*4) /= shadowColorR;
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first-1)*4+1) /= shadowColorG;
										//*(dataCopy+texWidth*((iter2->first+1)*4)+(iter->first-1)*4+2) /= shadowColorB;
										//px[iter->first-1][iter2->first+1] = 2;
										px_filled[texWidth*((iter2->first+1)*4)+(iter->first-1)*4] = 2;
									}
								}*/
