/**
 * @file main.cpp
 * @brief This cpp imports all the other header files to implement the Algorithm
 * @author M Bhargav 2020A7PS0025H
 * @author Tumu Akshar 2020A7PS0003H
 * @author P V Rohith 2020A7PS0100H
 * @author D V Sasanka 2020A7PS0005H
 * 
 * @date 24-03-23
*/
#include <bits/stdc++.h>
#include <ctime>
#include "dcel.h"
using namespace std;

/**
 * @brief The Function Splits the Polygon into Convex Polygon using Split Algorithm
 * 
 * @param dcel A pointer to the DCEL describing the polygon 
 * 
 * The function starts with all set of vertices and selects consecutive vertices from the start of original vertices list and forms a 
 * convex polygon by adding diagonal and then takes the rest of the polygon excluding the vertices of the newly formed convex polygon and 
 * performs the same function on the rest of the vertices.
 * 
 * @see DCEL , DCEL::vertices , DCEL::notches , isNotch() , getRectangle() , insideRect() , insidePolygon() , ang()
 * @return void
 */
void split(DCEL* dcel)
{
    vector<Vertex*> vertices = dcel->vertices;
    vector<vector<Vertex*>> polygons;
    
    while(vertices.size() > 3)
    {
        int n = vertices.size();
        Vertex* v1 = vertices[0];
        Vertex* v2 = vertices[1];
        vector<Vertex*> polygon;
        polygon.push_back(v1);
        polygon.push_back(v2);
        int i = polygon.size()-1;
        Vertex* new_v = vertices[i+1];
        while(ang(polygon[i-1],polygon[i],new_v) >= 0 &&ang(polygon[i],new_v,polygon[0]) >= 0 && ang(new_v,polygon[0],polygon[1]) >= 0)
        {
            polygon.push_back(new_v);
            i = polygon.size()-1;
            if(i+1 >= n) break;
            new_v = vertices[i+1];
        }
        if(polygon.size() != n)
        {
            vector<Vertex*> notches;
            for(int j=polygon.size();j<vertices.size();j++)
            {
                if(isNotch(vertices[j-1],vertices[j],vertices[(j+1)%vertices.size()]))
                {
                    notches.push_back(vertices[j]);
                }
            }
 
            Vertex* inside = NULL;
            do {
                inside = NULL;
                vector<double> rect = getRectangle(polygon);
                vector<Vertex*> req_notches;
                for(int j=0;j<notches.size();j++)
                {
                    if(insideRect(rect,notches[j])) req_notches.push_back(notches[j]);
                }
                for(int j=0;j<req_notches.size();j++)
                {
                    if(insidePolygon(polygon,req_notches[j]))
                    {
                        inside = req_notches[j];
                        break;
                    }
                }
                if(inside != NULL)
                {
                    removeFromSemiPlane(polygon,inside);
                }
 
            }while(inside != NULL);
            if(polygon.size() != 2)
            {
                polygons.push_back(polygon);
            }
            vector<Vertex*> new_vertices;
            for(int j=polygon.size()-1;j<vertices.size();j++)
            {
                new_vertices.push_back(vertices[j]);
            }
            new_vertices.push_back(polygon[0]);
            vertices = new_vertices;
        }
        else
        {
            polygons.push_back(polygon);
            break;
        }
    }
 
    //iterate through list of polygons and add diagnol to each polygon inside DCEL.
    for(auto& polygon:polygons){
        dcel->addDiagonal(polygon);
    }
}

/**
 * @brief This function performs the join operation after reading the vertices from the input file
 * 
 * @param dcel A pointer to the DCEL data structure
 * @param file_name The file name of the input file
 * 
 * The Function reads the file and initializes the vertices array
 * It then checks if points are clockwise then adds them from the start
 * If given in anticlockwise appends in reverse order
 * 
 * @see IsClockwise() , DCEL::addVertex() , DCEL::join()
 * @return n the no of coordinates of the polygon
*/
int input_file(DCEL * dcel,string file_name)
{
    file_name+=".txt";
    ifstream fin(file_name);
    int n;
    fin>>n;
    vector<pair<double,double>> mpl;
    for(int i=0;i<n;i++)
    {
        double x;
        double y;
        fin>>x>>y;
        mpl.push_back({x,y});
    }
    bool dir = IsClockwise(mpl);
    if(!dir)
    {
        for(int i=mpl.size()-1;i>=0;i--)
        {
            dcel->addVertex(mpl[i].first,mpl[i].second);
        }
    }
    else
    {
        for(int i=0;i<mpl.size();i++)
        {
            dcel->addVertex(mpl[i].first,mpl[i].second);
        }
    }
    dcel->join();
    return n;
}

/**
 * @brief The Main Function of the Program
 * 
 * It handles all the file reading part and also has initializations
 * It also has the timing analysis related code
 * It writes into 3 output files one just by plotting points, the second one just after splitting and third after merging
 * Writes into a text file that has all the timings for different inputs
 * 
 * @see input_file() , DCEL::merge() , split()
 * @return 0 for successful execution of the function
*/
int main()
{
    clock_t starting_time = clock();

    DCEL* dcel = new DCEL();
    int n = input_file(dcel,"input");
    dcel->output_file("output1");
    split(dcel);
    dcel->output_file("output2");
    dcel->merge();
    dcel->output_file("output3");

    clock_t ending_time = clock();
    double elapsed_time = static_cast<double>(ending_time-starting_time)/CLOCKS_PER_SEC;
    ofstream optfile;
    optfile.open("analysis.txt",ios_base::app);
    if(optfile.is_open())
    {
        optfile<<n<<" "<<elapsed_time<<endl;
        optfile.close();
    }
    return 0;
}