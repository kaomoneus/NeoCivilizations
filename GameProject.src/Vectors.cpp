#include "StdAfx.h"
#include <math.h>
#include "Vectors.h"




bool PointInAngle(POINT Vertex, POINT Ray1, POINT Ray2, POINT Target, POINT Point)
{
	bool t1, t2, p1, p2;


	int x,y,x1,y1,x2,y2;

	x = Target.x;
	y = Target.y;
	
	x1 = Vertex.x;
	y1 = Vertex.y;

	x2 = Ray1.x;
	y2 = Ray1.y;

	t1 = (y <= ((x - x1)*(y2 - y1)/(x2 - x1 + 0.00001) + y1));

	x2 = Ray2.x;
	y2 = Ray2.y;

	t2 = (y <= ((x - x1)*(y2 - y1)/(x2 - x1 + 0.00001) + y1));


	x = Point.x;
	y = Point.y;

	x2 = Ray1.x;
	y2 = Ray1.y;

	p1 = (y <= ((x - x1)*(y2 - y1)/(x2 - x1 + 0.00001) + y1));
	
	x = Point.x;
	y = Point.y;

	x2 = Ray2.x;
	y2 = Ray2.y;

	p2 = (y <= ((x - x1)*(y2 - y1)/(x2 - x1 + 0.00001) + y1));

	if((p1 == t1)&&(p2 == t2))
		return true;
	else
		return false;
}




TVector::TVector(T3DPoint Start, T3DPoint End)
{
	Radius = End - Start;
};

TVector::TVector(T3DPoint RadiusVector)
{
	Radius = RadiusVector;
};


TVector::TVector(float X, float Y, float Z)
{
	Radius.X = X;
	Radius.Y = Y;
	Radius.Z = Z;
}



TVector	TVector::operator + (const TVector &V)
{
	TVector Res = TVector(0,0,0);
	Res.Radius = Radius + V.Radius;
	return Res;
}

TVector	TVector::operator - (const TVector &V)
{
	TVector Res = TVector(0,0,0);
	Res.Radius = Radius - V.Radius;
	return Res;
}




TVector	TVector::operator * (const TVector &V)
{
	TVector Res = TVector(0,0,0);
	Res.Radius = Radius*V.Radius;
	return Res;
}

TVector	TVector::operator ^ (const TVector &V)
{
	TVector Res = TVector(0,0,0);
	Res.Radius.X = Radius.Y*V.Radius.Z - Radius.Z*V.Radius.Y;
	Res.Radius.Y = Radius.Z*V.Radius.X - Radius.X*V.Radius.Z;
	Res.Radius.Z = Radius.X*V.Radius.Y - Radius.Y*V.Radius.X;

	return Res;
}


void TVector::Normalize()
{
	float L = Length();
	Radius.X = Radius.X/L;
	Radius.Y = Radius.Y/L;
	Radius.Z = Radius.Z/L;
}
/*
T4Region::T4Region(POINT Points[4])
{
	for (int i = 0; i < 4; i++)
	{
		Vertex[i] = Points[i];
	}

}
*/		

bool T4Region::CheckPoint(POINT Point)
{
	if(PointInAngle(Vertex[0],Vertex[3],Vertex[1],Vertex[2],Point)&&
	   PointInAngle(Vertex[1],Vertex[0],Vertex[2],Vertex[3],Point)&&
	   PointInAngle(Vertex[2],Vertex[1],Vertex[3],Vertex[0],Point)&&
	   PointInAngle(Vertex[3],Vertex[2],Vertex[0],Vertex[1],Point))
			return true;
	else
			return false;
}


/*
extern TVector NormalTriangle(float p1x,float p1y,float p1z,
					   float p2x,float p2y,float p2z,
					   float p3x,float p3y,float p3z)
{
	TVector Normal(0,0,0);

	TVector V1(p2x - p1x, p2y - p2y, p2z - p1z);
	TVector V2(p2x - p3x, p2y - p3y, p2z - p3z);

	Normal = V1^V2;

	Normal.Radius = Normal.Radius*(-1/Normal.Length());

	return Normal;
}
*/