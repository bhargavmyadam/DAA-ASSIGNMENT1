/**
 * @file helper_functions.h
 * @brief This Header implements the different helper functions needed to implement DCEL
 * @author M Bhargav 2020A7PS0025H
 * @author Tumu Akshar 2020A7PS0003H
 * @author P V Rohith 2020A7PS0100H
 * @author D V Sasanka 2020A7PS0005H
 * 
 * @date 24-03-23
*/
#include <bits/stdc++.h>
#include <cmath>
#include "helper_structs.h"

#define PI 3.14159265/**< The value of PI used for the calculation. */

using namespace std;

double Convert(double radian);
double ang(Vertex* v1,Vertex* v2,Vertex* v3);
bool isNotch(Vertex*v1,Vertex* v2,Vertex* v3);
void  removeFromSemiPlane(vector<Vertex*>& polygon,Vertex* inside);
bool sameSide(Vertex *v1, Vertex*v2, Vertex*v3, Vertex*v4);
vector<double> getRectangle(vector<Vertex*>& polygon);
bool insideRect(vector<double> rect,Vertex* v);
bool insidePolygon(vector<Vertex*>& polygon,Vertex* v);
bool IsClockwise(vector<pair<double,double>> vertices);


/**
* @brief Convert the Angle in Radians to Degrees, The Precison is Double
* @param radian The Angle in Radian
* Convert the Radian to Degree by multiplying with 180 and dividing by PI
* @return The Angle in Degrees
*/
double Convert(double radian)
{
    double pi = 3.14159;
    // Return Converted Value
    return(radian * (180 / pi));
}

/**
* @brief Calculate the Internal Angle Contained by the three vertices
* 
* @param v1 The Vertex v1
* @param v2 The Vertex v2 (The Vertex at the center of the angle)
* @param v3 The Vertex v3
* 
* The function uses dot and determinent of the vectors formed by the three vertices 
* It calculates the Vectors having their start point at v2
* Calculate the Dot Product and Determinant Using the Formuale
* Use the Arctan2 Function to get the angle between them
* Return the result in degrees using the Convert Function
* 
* @see Convert
* @return The Angle formed by the three vertices in degrees
*/
double ang(Vertex* v1,Vertex* v2,Vertex* v3)
{
    // Extract the Coordinates 
    double x1 = v1->x; 
    double y1 = v1->y;
    double x2 = v2->x; 
    double y2 = v2->y;
    double x3 = v3->x; 
    double y3 = v3->y;
 
    //Components of the first Vector
    double X1 = x2-x1;
    double Y1 = y2-y1;
    //Components of the second Vector
    double X2 = x2-x3;
    double Y2 = y2-y3;
    //The Dot Product of the Vectors
    double dot = X1*X2+Y1*Y2;
    //The Determinant of the vectors
    double det = X1*Y2-X2*Y1;
    //The Angle in Degrees is returned 
    double result = Convert(atan2(det,dot));
    return result;
}

/**
 * @brief The Function determines if a notch is present at the middle vertex given three consecutive vertices
 * 
 * @param v1 The first vertex
 * @param v2 The second vertex(The vertex where we check if notch is present)
 * @param v3 The third vertex
 * 
 * Calculates the Angle between the edges formed by the vertices and returns true if angle is negative
 * 
 * @see ang
 * @return true if notch is present
*/
bool isNotch(Vertex*v1,Vertex* v2,Vertex* v3)
{
    return ang(v1,v2,v3) < 0;
}

/**
 * @brief Remove all the vertices in the given polygon that are on the same side of the line formed by first and last vertices as the given inside index
 * 
 * The Function modifies the vertex list by removing all the vertices that lie in the same semi plane as the given inside vertex
 * The Semi Plane is the line formed by first and last vertices of the polygon and inside vertex must lie on the opposite side of the line as the remaining vertices
 * 
 * @param polygon A vector of pointers to the vertices 
 * @param inside A pointer to the vertex that lies opposite side of the line for all remaining vertices of the polygon
 * 
 * @note This functions assumes that the vertices in the list are greater than two
 * 
 * @see Vertex , sameSide()
 * @return void  
*/
void  removeFromSemiPlane(vector<Vertex*>& polygon,Vertex* inside)
{
    //The last vertex
    Vertex* last = polygon[polygon.size()-1];
    //Remove the last vertex from vector
    polygon.pop_back();
    while(polygon.size() > 2)
    {
        Vertex* v = polygon[polygon.size()-1];
        //Remove the vertices if they are on same side as the vertex inside
        if(sameSide(polygon[0],inside,last,v)) polygon.pop_back();
        else break;
    }
    return;
}

/**
 * @brief Determines whether two vertices are on the same side of a line formed by other two
 * 
 * @param v1 The first vertex of the line
 * @param v2 The second vertex of the line
 * @param v3 The first vertex to check
 * @param v4 The second vertex to check
 * 
 * Find the Projections of v3 and v4 onto the line formed v1 and v2
 * If Projections have the same sign then they are on the same side
 * If not opposite sides
 * 
 * @return true if v3 and v4 are on same side of the line formed by v1 and v2
*/
bool sameSide(Vertex *v1, Vertex*v2, Vertex*v3, Vertex*v4)
{
    //Slope of the line with points v1 and v2
    double m = (v2->y - v1->y) / (v2->x - v1->x);
    //Y intercept of the line with points v1 and v2
    double c = v1->y - m*(v1->x);
    //Projection of V3 onto the line
    double p1 = v3->y - m*(v3->x) - c;
    //Projection of V4 onto the line
    double p2 = v4->y - m*(v4->x) - c;
    //If Both Projections have the same sign then return true
    if(p1*p2 >= 0) return true;
    else return false;
}
 
/**
 * @brief The Function returns coordinates of a rectangle formed by points
 * 
 * @param polygon The Vector of Vertices of the Polygon
 * 
 * The functions calculates the max, min of X and Y coordinates and stores it in a vector
 * @see Vertex
 * @return a Vector of the 4 points of double precision
*/
vector<double> getRectangle(vector<Vertex*>& polygon)
{
    vector<double> X_cor;
    vector<double> Y_cor;
    for(int i = 0; i < polygon.size();i++)
    {
        X_cor.push_back(polygon[i]->x);
        Y_cor.push_back(polygon[i]->y);
    }
    double X_cor_max = *max_element(X_cor.begin(),X_cor.end());
    double X_cor_min = *min_element(X_cor.begin(),X_cor.end());
    double Y_cor_max = *max_element(Y_cor.begin(),Y_cor.end());
    double Y_cor_min = *min_element(Y_cor.begin(),Y_cor.end());
    return {X_cor_min,X_cor_max,Y_cor_min,Y_cor_max};
}
 
/**
 * @brief The Function checks if the given point lies in the rectangle
 * 
 * @param rect The Vector of Vertices of the Rectangle
 * @param v The vertex v
 * The function checks if the given point lies in the rectangle
 * @see Vertex
 * @return true if point lies in the rectangle
*/
bool insideRect(vector<double> rect,Vertex* v)
{
    if((rect[0] < v->x && rect[1] > v->x) && (rect[2] < v->y && rect[3] > v->y))
    {
        return true;
    }
    return false;
}

/**
 * @brief The Function checks if the given point lies in the polygon
 * 
 * @param polygon The Vector of Vertices of the Polygon
 * @param v The vertex v
 * The function checks if the given point lies in the polygon
 * @see Vertex , DCEL::vertices
 * @note This function takes that no of vertices is greater than 2
 * @return true if point lies in the polygon
*/
bool insidePolygon(vector<Vertex*>& polygon,Vertex* v)
{
    int num = polygon.size();
    if(num <= 2)
    {
        return false;
    }
    int count = 0;
    for(int i = 0; i < num; i++)
    {
        Vertex* v1 = polygon[i];
        Vertex* v2 = polygon[(i+1)%num];
        if(((v1->y > v->y) != (v2->y > v->y)))
        {
            double p = (v2->x-v1->x)*(v->y-v1->y)/(v2->y-v1->y) + v1->x;
            if(v->x < p)
            {
                count++;
            }
        }
    }
    if(count % 2 == 1)
    {
        return true;
    }
    return false;
}

/**
 * @brief This Function Checks if The Vertices in the Input are in clockwise
 *  
 * @param vertices A vector of pairs having the X and Y Coordinates in Double Precision
 * 
 * @return true if vertices are in clockwise 
*/
bool IsClockwise(vector<pair<double,double>> vertices){
    double sum = 0.0;
    for (int i = 0; i < vertices.size(); i++) 
    {
        sum += (vertices[(i + 1) % vertices.size()].first - vertices[i].first) * (vertices[(i + 1) % vertices.size()].second + vertices[i].second);
    }
    return sum > 0.0;
}
