#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <chrono>
#include <cstdint>

struct Point{
	double x=0.0;
	double y=0.0;
};

// ax+by+c=0
struct Line{
	double a=0.0;
	double b=0.0;
	double c=0.0;
	int32_t vote=0;
};

//read data from file
void readData(std::vector<Point> &points){
	std::ifstream file("benchmark_hepta.dat");
	std::string line,bit;
	Point tmp_p;
	if(file.is_open()){
		while(getline(file,line)){
			std::stringstream ss(line);
			getline(ss,bit,',');
			tmp_p.x=stod(bit);		
			getline(ss,bit,',');
			tmp_p.y=stod(bit);
			//only x,y point used
			points.push_back(tmp_p);
		}
	}
	//test if file read completed
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		std::cout<<"x="<<itr->x<<" y="<<itr->y<<std::endl;
	}
}

//line feature extraction a,b,c
void lineFeature(Point p1, Point p2,std::vector<Line> &lines){
	Line tmp_line;
	tmp_line.a=(p2.y-p1.y);
	tmp_line.b=(p2.x-p1.x);
	tmp_line.c=-(tmp_line.a*p1.x+tmp_line.b*p1.y);
	lines.push_back(tmp_line);
}

int32_t randNum(int32_t size){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int32_t> dis(0, size-1);
	return dis(gen);
}

double linePoint_dist(Line l,Point p){
	return std::abs(l.a*p.x+l.b*p.y+l.c)/(l.a*l.a+l.b*l.b);
}

void votecounter(std::vector<Point> &points, Line &l, int32_t T){
	int32_t counter=0;
	for(std::vector<Point>::iterator itr=points.begin();itr!=points.end();itr++){
		//test distance function
		//std::cout<<"distance = "<<linePoint_dist(l,(*itr))<<std::endl;
		if(linePoint_dist(l,(*itr))<T){
			counter++;
			//std::cout<<"counted"<<std::endl;
		}

	}

	l.vote=counter;

}

void RANSAC(std::vector<Point> &points,std::vector<Line> &lines, int32_t T, int32_t N){
	Line selected_line;
	int32_t max_vote=0;
	int32_t size=points.size();
	
	for(int i=0;i<N;i++){
		lineFeature(points[randNum(size)],points[randNum(size)],lines);
		votecounter(points,lines[i],T);
	}



	for(std::vector<Line>::iterator itr=lines.begin();itr!=lines.end();itr++){
	
		if( itr->vote > max_vote ){
			max_vote=itr->vote;
			selected_line=(*itr);
		}

	}

	//line tester
	//for(std::vector<Line>::iterator itr=lines.begin();itr!=lines.end();itr++){
	//	std::cout<<"Line: "<<itr->a<<"x+"<<itr->b<<"y+"<<itr->c<<" vote="<<itr->vote<<std::endl;
	//}

	std::cout<<"Line: "<<selected_line.a<<"x+"<<selected_line.b<<"y+"<<selected_line.c<<std::endl;
}

int main(){
	std::vector<Point> points;
	std::vector<Line> lines;
	int32_t T=1, N=20;
	//timer
	auto start = std::chrono::steady_clock::now();	

	readData(points);
	RANSAC(points,lines, T, N);	

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	
	

	return 0;
}
