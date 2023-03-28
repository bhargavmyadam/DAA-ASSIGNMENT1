/**
 * @file helper_structs.h
 * @brief This Header implements the different structs needed to implement DCEL
 * @author M Bhargav 2020A7PS0025H
 * @author Tumu Akshar 2020A7PS0003H
 * @author P V Rohith 2020A7PS0100H
 * @author D V Sasanka 2020A7PS0005H
 * 
 * @date 24-03-23
*/

#include <bits/stdc++.h>

using namespace std;

/**
 * @brief A struct representing the 2D Coordiantes of a Plane having Double Precision
*/
struct Vertex
{
    double x;/**< The value of the X coordinate */
    double y;/**< The value of the Y coordinate */
};

/**
 * @brief A struct representing the edge of a Doubly Connected Edge List(DCEL)
*/
struct Edge
{
    Vertex* org; /**< A Vertex pointer to the origin vertex of the edge*/
    Edge* twinEdge; /**< An Edge pointer to twin or opposite edge of the given edge*/
    Edge* prev;/**< An Edge pointer to the previous edge of the given edge in the given face*/
    Edge* next;/**< An Edge pointer to the next edge of the given edge in the given face*/

};

/**
 * @brief A struct representing the Face in a Doubly Connected Edge List(DCEL)
*/
struct Face
{
    Edge* incidentEdge;/**< An Edge pointer to the incident edge in the given face*/
};