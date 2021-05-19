#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>

struct Point{
	double x,y;
	Point(double tx,double ty) : x(tx),y(ty) {}
	Point() : x(0.0),y(0.0) {}
};

//ax+by+c=0 Line
struct Line{
	double a,b,c;
	int32_t votes;
	Line() : a(0.0), b(0.0), c(0.0), votes(0) {}
	Line(double ta, double tb, double tc) : a(ta), b(tb), c(tc), votes(0) {}
	
};

void readFile(std::vector<Point> &points){
	std::string line, bit;
	Point tmp_p;
	std::ifstream FILE("benchmark_hepta.dat");
	if(FILE.is_open()){
		//getline until the end of file
		while(getline(FILE,line)){
			std::stringstream ss(line);
			getline(ss,bit,',');
			tmp_p.x=stod(bit);
			getline(ss,bit,',');
			tmp_p.y=stod(bit);
			points.push_back(tmp_p);		
		}
	}
	//check loaded points
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		std::cout<<"x="<<itr->x<<" y"<<itr->y<<std::endl;
	}
}

void lineAug(std::vector<Line> &lines,Point p1, Point p2){
	double a,b,c;
	//equation incorrect
	a=-(p2.y-p1.y);
	b=(p2.x-p1.x);
	c=-( a*p1.x+b*p1.y  );
	Line tmp_l(a,b,c);
	lines.push_back(tmp_l);
}

double linepointDist(Line l,Point p){
	//zero division check should be done
	if( (l.a == 0) && (l.b == 0) ){
		return __DBL_MAX__
	}else{
		return ( std::abs(l.a*p.x+l.b*p.y+l.c)/(l.a*l.a+l.b*l.b) );
	}
}

void RANSAC(std::vector<Point> &points,int32_t N,double T){
	std::vector<Line> lines;
	std::random_device rd;
	std::uniform_int_distribution<int32_t> dis(0,points.size()-1);
	int32_t v_max=0,best_line=0;
	int32_t counter=0;	

	//line generate
	for(int i=0;i<N;i++){
		lineAug(lines, points[dis(rd)], points[dis(rd)]);
	}
	
	//count votes
	for(int i=0;i<N;i++){
		for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
			if(linepointDist(lines[i],*itr)<T){
				counter++;
			}
		}
		//votes update
		lines[i].votes=counter;
		//counter reset
		counter=0;
	}
	
	//find best line and print all
	for(int i=0;i<N;i++){
		std::cout<<"line"<<i<<": "<<lines[i].a<<"x+"<<lines[i].b<<"y+"<<lines[i].c<<" | votes="<<lines[i].votes<<std::endl;
		if(lines[i].votes>v_max){
			v_max=lines[i].votes;
			best_line=i;
		}
	}
	std::cout<<"selected line : "<<lines[best_line].a<<"x+"<<lines[best_line].b<<"y+"<<lines[best_line].c<<" | votes="<<lines[best_line].votes<<std::endl;
	
	
}

int main(){
	std::vector<Point> points;
	int32_t N=30;
	double T=1.0;	

	//timer
	auto strt=std::chrono::steady_clock::now();

	readFile(points);

	RANSAC(points,N,T);

	std::cout<<"RANSAC"<<std::endl;

	auto end=std::chrono::steady_clock::now();
	
	std::chrono::duration<double> elapsed=end-strt;

	std::cout<<"elapsed time = "<<elapsed.count()<<std::endl;	

	return 0;
}
