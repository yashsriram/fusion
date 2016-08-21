#include<simplecpp>
#include<iostream>
#include<fstream>
#include"vectorClass.h"
#include<string>

using namespace std;

int Dimension;//Dimension of Square Canvas
int maxE;//Maximum number of elements on Table (excluding the center one)
double gloX = 0,gloY = 0;//Global X and Y to assign the getClick X and Y
int randomVar = 0;//random variable for color and name
int cR = 1,cG	 = 2,cB = 3;//random variables for colors
bool comboContinue ;

void getaClick()
    {
    const int dividingConstant = 65536;
    int point;
    point = getClick();
    gloX = point/dividingConstant;
    gloY = point%dividingConstant;
    }//Puts the X and Y value of the Click in 'gloX' , 'gloY'

double vectorangle(double x1,double y1,double x2,double y2)
	{
    double cosineofangle,angle;
    if((x2-x1) != 0)
    	{
        cosineofangle=((x2-x1)/sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)));
        angle=arccosine(cosineofangle);
        if(y2-y1>=0)return angle;
        if(y2-y1<0)return 360-angle;
		}


    if((x2-x1) == 0)
    	{
    	if(y2-y1>0) return 90;
        if(y2-y1<0) return 270;
        }
    return 0;
	}
                //(the order of input matters) OUTPUT -pi to pi
                //include cpp or simple cpp

struct Element
    {
    //
    double radius; 
    double x,y;
    //data part
    
    Circle C;
    Text T;
    //graphics part
    int name;//name that appears on the screen
    int SectorNo;//the sector number which it is in
    int pointedByIndex;//has the index of the Table.'E' pointer which points to this in the heap
    
    Element()
        {
		initialize();
		createElementOnScreen();
        }
      
    void initialize()
    	{
    	SectorNo = -1;
    	pointedByIndex = -1;//not Pointed by any Table.'E[i]' pointer
    	radius = Dimension/10 - Dimension/50 ;
		name = 0;
		nameSet();
		x = Dimension/2;
		y = Dimension/2;
		return;
    	}//assigns the values of the data variables for the first time     
        
    void createElementOnScreen()
    	{
    	Circle Source1(Dimension/2,Dimension/2,radius);
		C = Source1;
		colorSet();
		Text Source2(Dimension/2,Dimension/2,name);
		T = Source2;
		T.setColor(COLOR(60,226,10));
		return;
    	}//graphically creates the 'Element' for the first time  
    
    void resetTheElementUsingSectorNo(double SectorAngle)
    	{
    	x = Dimension/2 + (2*Dimension/5)*cosine(SectorAngle*SectorNo);
    	y = Dimension/2 + (2*Dimension/5)*sine(SectorAngle*SectorNo);
    	moveElementToXY();
    	return;
    	}// 'x' and 'y' reset using 'SectorNo' and Table.'SectorAngle'
    
    void moveElementToXY()
    	{
    	T.moveTo(x,y);
    	C.moveTo(x,y);
    	C.setFill(1);
    	}//'C' 'T' are movedTo new positon according to new 'x' 'y'
    
    void updateNameAndColor(int newName)
    	{
    	name = newName;
    	Text Source(x,y,name);
    	T = Source;
    	T.setColor(COLOR(60,226,10));
    	colorSet();
    	}
    
    void Details()
    	{
    	cout<<"Name = "<<name<<endl;
    	cout<<"pointedByIndex = "<<pointedByIndex<<endl;
    	cout<<"SectorNo = "<<SectorNo<<endl;
    	}
    
    void nameSet()
        {
        if(randomVar == 0)
        	{
        	name = 1;
        	}
        if(randomVar == 1)
        	{
        	name = 2;
        	}
        if(randomVar == 2)
        	{
        	name = 3;
        	}
        if(randomVar == 3)
        	{
        	name = 4;
        	}
        if(randomVar == 4)
        	{
        	name = 5;
        	}		
        return;
        }    
        
    void colorSet()
        {
        if(name%5 == 1)
        	{
        	C.setColor(COLOR(20+cR,50+cG,name*cB));
        	//C.setFill(1);
        	}
        if(name%5 == 2)
        	{
        	C.setColor(COLOR(name*cR,20+cG,50+cB));
        	//C.setFill(1);
        	}
        if(name%5 == 3)
        	{
        	C.setColor(COLOR(20+cR,name*cG,50+cB));
        	//C.setFill(1);
        	}
        if(name%5 == 4)
        	{
        	C.setColor(COLOR(50+cR,20+cG,name*cB));
        	//C.setFill(1);
        	}
        if(name%5 == 0)
        	{
        	C.setColor(COLOR(name*cR,50+cG,20+cB));
        	//C.setFill(1);
        	}		
        return;
        }    
        
    ~Element()
    	{
		
    	}
    };



class Table
    {
    int noE;
    int score;
    int highestNumber;
    Element** E; 
    double SectorAngle;
    string UserName;
    ifstream HighScoreFileInput;
    ofstream HighScoreFileOutput;				
    ofstream AllScores;
    
    public:
    
    Table()
        {
        initialize();
        
        Text noEDisplayBox(Dimension-Dimension/12,Dimension-Dimension/12-Dimension/50,"ELEMENTS ON BOARD");
        noEDisplayBox.setColor(COLOR(60,226,10));
       
        Text highestNumberDisplayBox(Dimension/12,Dimension/12-Dimension/50,"HIGHEST NUMBER ACHIEVED");
        highestNumberDisplayBox.setColor(COLOR(60,226,10));
        
        Text scoreDisplayBox(Dimension-Dimension/12,Dimension/12-Dimension/50,"SCORE");
        scoreDisplayBox.setColor(COLOR(60,226,10));
        
        Circle exitBox(Dimension/12,Dimension-Dimension/12,Dimension/10);
        Text Exit(Dimension/12,Dimension-Dimension/12,"EXIT");
        Exit.setColor(COLOR(60,226,10));
       
       while(true)
            {
            Text noEDisplay(Dimension-Dimension/12,Dimension-Dimension/12,noE);
            noEDisplay.setColor(COLOR(60,226,10));
           
            Text highestNumberDisplay(Dimension/12,Dimension/12,highestNumber);
            highestNumberDisplay.setColor(COLOR(60,226,10));
            
            Text scoreDisplay(Dimension-Dimension/12,Dimension/12,score);
            scoreDisplay.setColor(COLOR(60,226,10));	
            
            randomNewElement();
            anyCombo();
            resetHighestNumber();
            
            if(noE >= 13)
            	{
            	Text GameOver(Dimension/2,Dimension/2 - 10,"Game Over o_0");
            	GameOver.setColor(COLOR(60,226,10));
            	wait(2);
            	{
            	Text FinalScoreMessage(Dimension/2,Dimension/2 + 10,"Your Score is");
            	FinalScoreMessage.setColor(COLOR(60,226,10));
            	wait(2);
            	}
            	Text FinalScore(Dimension/2,Dimension/2 + 10,score);
            	FinalScore.setColor(COLOR(60,226,10));
            	
            	wait(2);
            	
            	resetHighScore();
            	}
            }
        }
        
    void initialize()
    	{
    	noE = 0;
        score = 0;
        highestNumber = 0;
        maxE = 13;
        SectorAngle = 0;
        HighScoreFileInput.open(".HighScore.txt");
        if(!HighScoreFileInput.is_open())
        	{
        	cout<<"Error Opening HighScore file"<<endl;
        	exit(true);
        	}
        // 
        //cout<<"Enter the Dimension"<<endl;
        //cin>>Dimension;
        Dimension = 1000;
        cout<<"Enter your Name (No spaces)"<<endl;
        cin>>UserName;
        initCanvas("Game0n",Dimension,Dimension);
        //square canvas of sidelenght 'Dimension'
        
        E = new Element* [maxE];
        for(int i = 0;i < maxE;i++)
            {
            E[i] = NULL;
            }//initializes the all 'E[i]' pointers to NULL as no elements are there 
        Rectangle B0(Dimension/2,Dimension/2,Dimension,Dimension);
        B0.setColor(COLOR(0,0,0));
        B0.setFill(1);
        B0.imprint(); 
        Circle B1(Dimension/2,Dimension/2,Dimension/2-10);
        B1.setColor(COLOR(20,20,20));
        B1.setFill(1);
        B1.imprint();
        Circle B2(Dimension/2,Dimension/2,Dimension/10);
        B2.setColor(COLOR(5,5,5	));
        B2.setFill(1);
        B2.imprint();
        
        //B1 B2 are boundary circles
    	return;
    	}//inititalizes the 'Table' part    
	
	
	void resetHighestNumber()
	    {
	    for(int i=0;i<maxE;i++)
            {
            if(E[i] != NULL)
                {
                if(E[i]->name > highestNumber) highestNumber = E[i]->name;
                }
            }
	    return; 
	    }
   		 
    void randomNewElement()
    	{
    	//Max number of elements are 12. A transition of 13 is possible if it Decreases 'noE' (or) the Game Ends then
         
        if(noE == 0)
        	{
        		{
		    	Element newElement ;//A new Element Comes in the center
		    	userClick(newElement); 
		    	
		    	randomVar = (randomVar + 1) % 5;
		    	}//newElement in stack is removed immediately
			
			return;
        	}
        
        
        if(noE > 0 && noE < 13 )
        	{		            
            	{
            	Element newElement ;//A new Element comes in the center
            	userClick(newElement);
            	
            	randomVar = (randomVar + 1) % 5;
            	}//newElement in stack is removed immediately
            
            return;
            }

    	return;
    	}//Element type var 'newElement' is "GIVEN" a pointer 'noE' ,Element.'SectorNo' and 'SectorAngle' are taken care of 
	
	void userClick(Element newElement)
		{
		int nullPointerIndex ;
		for(nullPointerIndex=0;nullPointerIndex<maxE;nullPointerIndex++)
			{
			if(E[nullPointerIndex] == NULL){break;}
			}
		if(nullPointerIndex == maxE){nullPointerIndex = -1;}
		//if there is any E[i] pointer left pointing to NULL 'nullPointerIndex' has its index (or) if no such pointer is present then it is set to value -1
		
		//cout<<nullPointerIndex<<endl;
		
		E[nullPointerIndex] = new Element;
		*E[nullPointerIndex] = newElement;
		E[nullPointerIndex]-> pointedByIndex = nullPointerIndex;	
		//E[nullPointerIndex] is set to the new incomming Element 
		//'noE' 'sectorAngle' Element.'SectorNo' are TO BE re-assigned
		
		if(noE == 0)
			{
			SectorAngle = 360 ;
			}
		//cout<<SectorAngle<<endl;
		getaClick();
        Vector Click(gloX,gloY);
        Vector Center(Dimension/12,Dimension-Dimension/12);
        
        if(~(Center - Click) <= Dimension/10)
        	{
        	resetHighScore();
        	}
        //High Score Insertion >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>	
        double theta;
        theta = vectorangle(Dimension/2,Dimension/2,gloX,gloY);//theta has the 0 to 359 value 
		int SectorNoSource ;
		
		SectorNoSource = (theta/SectorAngle);
		E[nullPointerIndex]->SectorNo = SectorNoSource;
	
		for(int i=0;i<maxE;i++)
			{
			if(E[i] != NULL && i != nullPointerIndex)
				{
				if(E[i]->SectorNo > E[nullPointerIndex]->SectorNo )
					{
					E[i]->SectorNo++;
					}
				}
			}
			
		if(noE != 0)
			{
			E[nullPointerIndex]->SectorNo++;
			}//Increases all the Element.'SectorNo' by 1 whose value is greater than E[nullPointerIndex]->SectorNo And
			 //Also increases E[nullPointerIndex]->SectorNo by 1 for the noE != 0 case   
			
		noE++; 
		if(noE == 0)
			{
			SectorAngle = 360 ;
			}
		if(noE > 0)
			{
			SectorAngle = 360.0/noE;
			}
		if(noE < 0)
			{
			cout<<"Some Problem with the Code"<<endl;
			}
		
		arrTheElements();
		
		return;	
		}//A pointer pointing to NULL is declared to point 'Element' type in heap and the 'newElement' is assigned to it   
   		 //noE is also increased ,Element.'SectorNo' is assigned and 'pointedByIndex' and 'SectorAngle' is taken care of	
	
    void anyCombo()
    	{
		
    	Element* Dup[maxE];
    	for(int i=0;i<maxE;i++)
    		{
    		Dup[i] = NULL;
    		}//All the 'Dup'pointers point to 'NULL'
    	
    	for(int i=0;i<maxE;i++)
			{
			if(E[i] != NULL )
				{
				Dup[E[i]->SectorNo] = E[i];
				}
			}//'Dup' Pointers set such that Dup[i] points to E[j] whose E[i]->SectorNo is i
			 //'Dup' Pointers are stored in stack and points to heap 
			 //As 'E' pointers already point to same Elements in heap ,the heap memeory pointed to at by 'Dup' pointers must NOT be freed(deleted)  
    	
    	int comboChecker = 0;
    	int startingPoint;
		for(int i=0;i<noE;i++)
			{
			if(noE < 3)
						{
						comboChecker = 0;
						break;
						}
						
			if((Dup[i]->name)%5 == 1)
				{
				startingPoint = i;
				comboChecker = 1;
				while(true)
					{
					int CW = (startingPoint + comboChecker)%noE;
					int ACW = (startingPoint - comboChecker)%noE;
					if(ACW<0)
						{
						ACW = ACW + noE;
						}
					//'CW' and 'ACW' go round the circle 
					
					if((abs(CW - ACW) == 1 || abs(CW - ACW) == noE - 1) && Dup[CW]->name == Dup[ACW]->name)
						{		
						break;
						}	
					
					if((Dup[CW]->name != Dup[ACW]->name)||CW == ACW)
						{
						comboChecker--;
						break;
						}
					
					if(Dup[CW]->name == Dup[ACW]->name)
						{
						comboChecker++;
						}
						
					
					}
				}
			if(comboChecker > 0)
				{
				comboContinue = true;
				break;
				}		
			}//comboChecker has the number of mixings for the first-comming name == 'plus(5)' Element only
		
		//cout<<comboChecker<<endl;
		
		if(comboChecker == 0)
			{
			return;
			}
		
//This part is executed only if comboChecker > 0 ----------------------------------------------------------------------------
		Text Mixing(Dimension/2,Dimension/2,"MIXING ELEMENTS");
		Mixing.setColor(COLOR(60,226,10));	
		
		int newName = 0;
		
		for(int i=1;i<=comboChecker;i++)
			{
			int CW = (startingPoint + i)%noE;
			int ACW = (startingPoint - i)%noE;
			if(ACW<0)
				{
				ACW = ACW + noE;
				}
					
			newName = newName + Dup[CW]->name + 1;
			
			/*
			E[Dup[CW]->pointedByIndex]->C.scale(1.5);
			E[Dup[ACW]->pointedByIndex]->C.scale(1.5);
			wait(0.3);
							
			E[Dup[CW]->pointedByIndex]->C.scale(2/3);
			E[Dup[ACW]->pointedByIndex]->C.scale(2/3);
			wait(0.3);*/
			
			bubblingEffect(E[Dup[CW]->pointedByIndex] , E[Dup[ACW]->pointedByIndex]);
									
			E[Dup[CW]->pointedByIndex] = NULL;
			delete Dup[CW];
			Dup[CW] = NULL;	
				
			E[Dup[ACW]->pointedByIndex] = NULL;
			delete Dup[ACW];
			Dup[ACW] = NULL;
								
			score = score + 1;
			}//'score' taken care of
			 //startingPoint.'name' is increased as 'newName' but not assigned
			 //all combo 'E[i]' memory is freed ,set to point 'NULL' , corresponding 'Dup[j]' is set to 'NULL'
			
			{
			Dup[(startingPoint)%noE]->updateNameAndColor(newName);
			}//startingPoint.'name' is taken care of
    		
    	if(Dup[0] != NULL)
			{   
			//cout<<"Dup[0] != NULL case"<<endl; 	
			if(startingPoint == 0)
				{
				for(int i=1;true;i++)
					{
					int point = (startingPoint + comboChecker + i)%noE;				
					if(Dup[point] == NULL)break;
					if(point == 0)cout<<"Some Problem with the Code"<<endl;
					Dup[point]->SectorNo = Dup[point]->SectorNo - comboChecker;
					if(point == (noE - 1))cout<<"Some Problem with the Code"<<endl;
					}
				}
				
			else if(startingPoint > 0)
				{
				int SectorNoChanger = Dup[startingPoint]->SectorNo;	 
				SectorNoChanger = SectorNoChanger - comboChecker; 
				Dup[startingPoint]->SectorNo = SectorNoChanger;

				for(int i=1;true;i++)
					{
					int point = (startingPoint + comboChecker + i)%noE;
					if(point == 0)break;
					Dup[point]->SectorNo = Dup[point]->SectorNo - 2*comboChecker;
					if(point == (noE - 1))break;
					}
				}
			}
			
    	else if(Dup[0] == NULL)
    		{	
    		//cout<<"Dup[0] == NULL case"<<endl;
    		Dup[startingPoint]->SectorNo = 0;
    		
    		int noTimes = noE - 2 * comboChecker - 1;
    		for(int i=1;i <= noTimes;i++)
    			{
    			int point = (startingPoint + comboChecker + i)%noE; 
    			Dup[point]->SectorNo = i%(noE - 2 * comboChecker);
    			}
    		}//Element.'SectorNo' is taken care of 
    	
    	noE = noE - 2 * comboChecker;//'noE' is taken care of
    		
    	if(noE == 0)	
    		{
    		SectorAngle = 360;
    		}
		if(noE > 0)	
    		{
    		SectorAngle = 360.0/noE;
    		}
		if(noE < 0)	
    		{
    		cout<<"Some Problem with the Code"<<endl;
    		}//'SectorAngle' is taken care of    		
    
    	comboTheElements();
    		
    	return;
    	}
    
    void arrTheElements()
		{
		for(int i=0;i<maxE;i++)
			{
			if(E[i] != NULL )
				{
				E[i]->resetTheElementUsingSectorNo(SectorAngle);
				}
			}
		return;
		}
    
    void comboTheElements()
		{
		for(int i=0;i<maxE;i++)
			{
			if(E[i] != NULL )
				{
				E[i]->resetTheElementUsingSectorNo(SectorAngle);
				}
			}
		//cout<<"out of loop"<<endl;
		if(comboContinue)anyCombo();	
		return;
		}
    
    int NonNullNumber()
    	{
    	int count = 0 ;
    	for(int i = 0;i<maxE;i++)
    		{
    		if(E[i] != NULL)
    			{
    			count++;
    			}
    		}
    	return count;
    	}
    
    void resetHighScore()
        {
	string line;
	HighScoreFileInput >> line;
	int length = line.length();
	int HighScore = 0;
	
	for(int i=0;i<length;i++)
		{
		if(line[i] == '0')HighScore = HighScore * 10 + 0;
		else if(line[i] == '1')HighScore = HighScore * 10 + 1;
		else if(line[i] == '2')HighScore = HighScore * 10 + 2;
		else if(line[i] == '3')HighScore = HighScore * 10 + 3;
		else if(line[i] == '4')HighScore = HighScore * 10 + 4;
		else if(line[i] == '5')HighScore = HighScore * 10 + 5;
		else if(line[i] == '6')HighScore = HighScore * 10 + 6;
		else if(line[i] == '7')HighScore = HighScore * 10 + 7;
		else if(line[i] == '8')HighScore = HighScore * 10 + 8;
		else if(line[i] == '9')HighScore = HighScore * 10 + 9;
		}
	HighScoreFileInput.close();
	//cout<<HighScore<<endl;
	
	AllScores.open(".AllScores.txt" , ios :: app);
	if(!AllScores.is_open())
			{
			cout<<"Error Opening HighScore file"<<endl;
			exit(true);
			}
	string UserNamePrint = "  -  " + UserName;
	AllScores << score << UserNamePrint <<endl;
	AllScores.close();
	
	if(score > HighScore)
		{
		HighScoreFileOutput.open("HighScore.txt");
		if(!HighScoreFileOutput.is_open())
			{
			cout<<"Error Opening HighScore file"<<endl;
			exit(true);
			}
		HighScoreFileOutput << score << UserNamePrint <<endl;
		HighScoreFileOutput.close();
		}	
	exit(true);
	return;
	}
        
    
    void bubblingEffect(Element *E1 , Element *E2)
		{
		double x1 = E1->x;
		double y1 = E1->y;
		
		double x2 = E2->x;
		double y2 = E2->y;
		
		double radius1 = E1->radius;
		double radius2 = E2->radius;
		
		double waitTime = 0.005;
		
		double tempRadius1 = radius1;
		double tempRadius2 = radius2;
		
		double range = Dimension/40;
		int radChanger = 1;
		int halfCycleCounter = 0;
		while(true)
			{
			E1->C.reset(x1,y1,tempRadius1);
			E2->C.reset(x2,y2,tempRadius2);
			tempRadius1 = tempRadius1 + radChanger;
			tempRadius2 = tempRadius2 + radChanger;
			if(tempRadius1 >= radius1 + range || tempRadius2 >= radius2 + range)
				{
				radChanger = -radChanger;
				halfCycleCounter++;
				}
			if(tempRadius1 <= radius1 - range || tempRadius1 <= radius2 - range)
				{
				radChanger = -radChanger;
				halfCycleCounter++;
				}
			if(halfCycleCounter == 2 && (tempRadius1 == radius1 || tempRadius2 == radius2))break;
			wait(waitTime);
			}
		}
    	
    ~Table()
        {
        delete [] E;
        closeCanvas();
        }    
    };


main_program
	{
    Table A;
    }
