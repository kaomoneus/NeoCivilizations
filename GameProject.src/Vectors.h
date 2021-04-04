struct T3DPoint
{
	float X;
	float Y;
	float Z;
	T3DPoint operator + (const T3DPoint &Point)
	{
		T3DPoint Res;
		Res.X=Point.X+X;
		Res.Y=Point.Y+Y;
		Res.Z=Point.Z+Z;
		return Res;
	}
	T3DPoint operator - (const T3DPoint &Point)
	{
		T3DPoint Res;
		Res.X=Point.X-X;
		Res.Y=Point.Y-Y;
		Res.Z=Point.Z-Z;
		return Res;
	}
	T3DPoint operator = (const float* Coords)
	{
		T3DPoint Res;
		Res.X = Coords[0];
		Res.Y = Coords[1];
		Res.Z = Coords[2];
	}

	T3DPoint operator * (const T3DPoint &Point)
	{
		T3DPoint Res;
		Res.X = X*Point.X;
		Res.Y = Y*Point.Y;
		Res.Z = Z*Point.Z;
		return Res;
	}

	T3DPoint operator * (const float A)
	{
		T3DPoint Res;
		Res.X = A*X;
		Res.Y = A*Y;
		Res.Z = A*Z;
		return Res;
	}



	POINT Get2DPoint()
	{
		POINT Res;
		Res.x = X;
		Res.y = Y;
		return Res;
	}

};

struct T3DAngle
{
	float Angle;
	int X;
	int Y;
	int Z;
};




class TVector
{
public:

	T3DPoint Radius;

	TVector(T3DPoint Start, T3DPoint End);
	TVector(T3DPoint RadiusVector);
	TVector(float X = 0,  float Y = 0, float Z = 0);
	
	TVector operator + (const TVector &V);
	TVector operator - (const TVector &V);
	TVector operator * (const TVector &V);
	TVector operator ^ (const TVector &V);

	TVector operator = (const T3DPoint P)
	{
		TVector V;
		V.Radius = P;
		return V;
	}

	float Length() 
	{
		return 	sqrt( (double)Radius.X*Radius.X+Radius.Y*Radius.Y+Radius.Z*Radius.Z );
	}


	T3DPoint GetBasis();

	void Normalize();


};


class T4Region
{
public:
	POINT Vertex[4];

//	T4Region(POINT Points[4]);
	
	bool CheckPoint(POINT Point);
};



