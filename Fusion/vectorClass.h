class Vector
{
	public:
	double x,y;
	
	Vector()
		{
		x = 0;
		y = 0;
		}
	
	Vector(int X,int Y)
		{
		x = X;
		y = Y;
		}
	
 	Vector operator=(Vector B)
		{
		x = B.x;
		y = B.y;
		return *this;
		}
	
	void set(double X,double Y)
		{
		x = X;
		y = Y;
		return ;
		}
	
	Vector operator+(Vector B)
		{
		Vector Sum;
		Sum.x = x + B.x;
		Sum.y = y + B.y;
		return Sum;
		}
	
	Vector operator-(Vector B)
		{
		Vector Difference;
		Difference.x = x - B.x;
		Difference.y = y - B.y;
		return Difference;
		}	
				
	double operator|(Vector B)
		{
		double dotProduct;
		dotProduct = x * B.x + y * B.y;
		return dotProduct;
		}		
	
	void print()
		{
		cout<<"("<<x<<","<<y<<")"<<endl;
		}

	double operator~()
		{
		double modulus;
		modulus = sqrt(x*x + y*y);
		return modulus;
		}

	Vector operator/(double scalar)
		{
		Vector returnVector;
		returnVector.x = x/scalar;
		returnVector.y = y/scalar;
		return returnVector;
		}
	
	Vector operator*(double scalar)
		{
		Vector returnVector;
		returnVector.x = x*scalar;
		returnVector.y = y*scalar;
		return returnVector;
		}
	
	Vector operator*(int scalar)
		{
		Vector returnVector;
		returnVector.x = x*scalar;
		returnVector.y = y*scalar;
		return returnVector;
		}
	
	void show()
		{
		initCanvas("Vector Representation",400,400);
		Line Xaxis(0,200,400,200);
		Xaxis.setColor(COLOR(255,0,0));
		Line Yaxis(200,0,200,400);
		Yaxis.setColor(COLOR(255,0,0));
	
		Line VectorInPlane(200,200,200+50*x,200+50*y);
		getClick();
		}		
};
