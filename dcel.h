/**
 * @file dcel.h
 * @brief This Header implements the DCEL Data Structure
 * @author M Bhargav 2020A7PS0025H
 * @author Tumu Akshar 2020A7PS0003H
 * @author P V Rohith 2020A7PS0100H
 * @author D V Sasanka 2020A7PS0005H
 * 
 * @date 24-03-23
*/
#include <bits/stdc++.h>
#include "helper_functions.h"

using namespace std;

/**
 * @brief A Class representing the Doubly Connected Edge List(DCEL)
*/
class DCEL
{
    private:
    set<Face*> faces;/** A Set of Face Pointers to the Faces of the DCEL */
    vector<Edge*> edges;/**< A Vector of Edge Pointers to the Edges of the DCEL*/
    unordered_map<Vertex*,bool> notches;/** A Unordered Map to determine if a Vertex is a Notch or not*/
    vector<Edge *> diagonal_list;/**< A Vector of Edge Pointers to the Diagonals Edges of the DCEL*/
    map<Vertex*,set<Vertex*>> LP; ;/**< A Map from Vertices to a Set of Vertices which represents the left chain for the given vertex*/
    vector<bool> LDP;/**A Vector of Boolena Values to check if an Edge is Delaunay or not*/
    public:
    vector<Vertex*> vertices;/**< A Vector of Vertex Pointers to the Vertices of the DCEL*/
    
    public:
    /**
     * @brief Add the specified vertex to the DCEL and Add an Edge if required
     * 
     * @param x1 The X Coordinate 
     * @param y1 The Y Coordinate
     * 
     * This Function creates a new Vertex Object
     * Adds the Vertex if it is the first Vertex
     * If There are More than one vertices then Forms an Edge between the last vertex and the current vertex
     * 
     * @see Vertex , addEdge() , vertices
     * @return void
    */
    void addVertex(double x1,double y1)
    {
        Vertex * v = new Vertex();
        v->x=x1;
        v->y=y1;
        //If this is the first vertex then just push
        if(vertices.size()==0)
        {
            vertices.push_back(v);
            return;
        }
        // Adding a new edge between the last vertex and the current vertex
        int last =vertices.size()-1;
        Vertex *v_old = vertices[last];
        addEdge(v_old,v);
        vertices.push_back(v);
        return;
    }
 
    /**
     * @brief Add an Edge to the DCEL and updates the prev and next edges
     * 
     * @param old_ver The Vertex already in the Polygon 
     * @param new_ver The Vertex we are adding so that edge is formed between them
     * 
     * This Function creates a new Edge Object
     * Performs the corresponding Edge Functionalities 
     * Adds the Edge to the DCEL
     * 
     * @see Edge , edges 
     * @return void
    */
    void addEdge(Vertex* old_ver,Vertex* new_ver)
    {
        //Creating two new edges
        Edge * e1 = new Edge();
        e1->org=old_ver;
        Edge * e2 = new Edge();
        e2->org=new_ver;
        //Link the two edges
        e1->twinEdge=e2;
        e2->twinEdge=e1;
        //If the edge is the first one set other pointers to NULL
        if(edges.size()==0)
        {
            edges.push_back(e1);
            e1->next=NULL;
            e2->next=NULL;
            e1->prev=NULL;
            e2->prev=NULL;
            return;
        }
        // Linking the new edges to the previous ones
        edges[edges.size()-1]->next=e1;
        e1->prev=edges[edges.size()-1];
        e1->next=NULL;
        // for twin
        edges[edges.size()-1]->twinEdge->prev=e2;
        e2->next=edges[edges.size()-1]->twinEdge;
        e2->prev=NULL;
        // adding the edge
        edges.push_back(e1);
        return;
    }
    
        /**
     * @brief This function joins the first and the last vertices of the Polygon by adding a new edge
     * 
     * Create a new edge e1 connecting last vertex to the first vertex 
     * Create a twin edge from first to last vertex
     * Set the necessary pointers for the two edges
     * 
     * @note This function assumes that the polygon is simple, closed and has atleast two edges 
     * 
     * @see Edge , edges
     * @return void
    */
    void join()
    {
        if(edges.size()<2)
        {
            return;
        }
        Edge * last = edges[edges.size()-1];
        // creating a joining edge
        Edge * e1= new Edge();
        e1->org=last->twinEdge->org;
        // creating twin of e1
        Edge * e2 = new Edge();
        e2->org=vertices[0];
        e1->twinEdge=e2;
        e2->twinEdge=e1;
 
        // setting prev and next for e1
        edges[edges.size()-1]->next=e1;
        e1->prev=edges[edges.size()-1];
        e1->next=edges[0];
        edges[0]->prev=e1;
 
        // setting  prev and next for e2
        edges[edges.size()-1]->twinEdge->prev=e2;
        e2->next=edges[edges.size()-1]->twinEdge;
        e2->prev=edges[0]->twinEdge;
        edges[0]->twinEdge->next=e2;

        //Adding the edge to Edge list 
        edges.push_back(e1);

        add_notches();
        addFace(edges[0]);
        return;
    }
 
    private:
    /**
     * @brief Adding a Face to the DCEL 
     * 
     * @param incident The incident edge of the Face 
     * 
     * Function adds a face to the DCEL given the incident edge
     * 
     * @see Face , faces
     * @return void 
     * 
    */
    void addFace(Edge* incident)
    {
        //Create a new Face
        Face * f = new Face();
        //Set its incident edge
        f->incidentEdge=incident;
        //Insert into the Faces Vector 
        faces.insert(f);
    }
    
    /**
     * @brief Adds Notches based on the angle between the vertices  
     * 
     * This function calculates the angles between every three consecutive vertices and checks if middle vertex has a notch or not
     * If the Angle is greater than 180 its a notch at the vertex otherwise no
     * This function adds all such vertices.
     * 
     * @see vertices , notches , ang
     * @return void
    */
    void add_notches()
    {
        //Calculate the size of n
        int n = vertices.size();
        for(int i=0;i<n;i++)
        {
            //Calculate the angular between the three consecutive vertices
            double ans = ang(vertices[(i)%n],vertices[(i+1)%n],vertices[(i+2)%n]);
            //If angle < 0 add a notch
            if(ans<0)
            {
                notches[vertices[(i+1)%n]]=true;
            }
            else
            {
                notches[vertices[(i+1)%n]]=false;
            }
        }
    }

    public:
    /**
     * @brief The Function adds diagonals between appropriate vertices
     * 
     * @param polygon The Vector of Vertices of the Polygon
     * 
     * This function searches for a face that contains both first and last vertices between whom the diagonal need to be added.
     * It adds a diagonal between them if they are not adjacent.
     * The diagonal added is of the form of two half edges one from last to first vertex and first to last
     * The diagonal is added to the Edge List and two new faces are created 
     * This function also updates the list of polygon vertices in the LP Map
     * 
     * @see Vertex , Face , adjacent() , LP , faces , Edge
     * @return void
    */
    void addDiagonal(vector<Vertex*>& polygon)
    {
        Vertex* v1 = polygon[0];
        Vertex* v2 = polygon[polygon.size()-1];
 
        Edge* v1Start = NULL;
        Face* req_face = NULL;
        Edge* v2Start = NULL;
        for(auto face : faces){
            int cnt = 0;
            Edge* start = face->incidentEdge;
            Edge* curr = start->next;
            if(v1 == start->org)
            {
                v1Start = start;
                cnt++;
            }
            if(v2 == start->org)
            {
                v2Start = start;
                cnt++;
            }
            while(curr != start)
            {
                if(v1 == curr->org)
                {
                    v1Start = curr;
                    cnt++;
                }
                if(v2 == curr->org)
                {
                    v2Start = curr;
                    cnt++;
                }
                curr = curr->next;
            }
            if(cnt == 2)
            {
                req_face = face;
                break;
            }
        }
        if(!adjacent(v2,v1))
        {
            Edge * new_edge = new Edge();
            new_edge->org = v2;
            new_edge->next = v1Start;
            new_edge->prev = v2Start->prev;
            Edge * new_edge_twin = new Edge();
            new_edge_twin->org = v1;
            new_edge_twin->next = v2Start;
            new_edge_twin->prev= v1Start->prev;
            new_edge->twinEdge = new_edge_twin;
            new_edge_twin->twinEdge = new_edge;
 
            v2Start->prev->next = new_edge;
            v1Start->prev->next = new_edge_twin;
            v2Start->prev = new_edge_twin;
            v1Start->prev = new_edge;
 
            diagonal_list.push_back(new_edge);
            LP[new_edge->org].insert(new_edge->twinEdge->org);  

            LP[new_edge->twinEdge->org].insert(new_edge->org);
 
            faces.erase(req_face);
            addFace(new_edge);
            addFace(new_edge_twin);
        }
    }
 
    /**
     * @brief The Function checks if two vertices are adjacent
     * 
     * @param v1 The First vertex
     * @param v2 The Second vertex
     * The function checks if the two vertices are adjacent
     * @see Vertex , LP
     * @return true if the points are adjacent.
    */
    bool adjacent(Vertex* v1,Vertex* v2)
    {
        if(LP[v1].find(v2)!=LP[v1].end() || LP[v2].find(v1)!=LP[v2].end())
        {
            return true;
        }
        return false;
    }
 
    /**
     * @brief An internal function which checks one of the necessary condition to remove a diagonal between two vertices.
     * 
     * @param vs Pointer to first vertex
     * @param vt Pointer to second vertex
     * 
     * It checks number of diagonals the two vertexes contain. 
     * If both vertices have diagonals count greater than 2 or if one of them have and the other is not a notch or both are not notches.
     * If any of the above cases is satisfied then return true.
     * 
     * @see LP , notches
     * @return true if the condition is met.
    */
    bool merge_checker(Vertex *vs, Vertex *vt)
    {
        int size_s =0;
        int size_t = 0;
        if(LP.find(vs)!=LP.end())
        {
            size_s=LP[vs].size();
        }
        if(LP.find(vt)!=LP.end())
        {
            size_t=LP[vt].size();
        }
        return (size_s>2 && size_t>2) || (size_s>2 && !notches[vt]) || (size_t>2 && !notches[vs]) || (!notches[vs] && !notches[vt]);
    }
 
    /**
     * @brief Removes a Diagonal from the Polygon
     * 
     * @param diagonal The Diagonal Edge to be removed
     * 
     * This function removes the diagonal from the polygon
     * It first finds the faces that are adjacent to the diagonal edge and removes them
     * Then the function removes the diagonal edge and its twin edge
     * Now the function adds a new face to fill the void left by removing the diagonal
     * 
     * @see Face , Edge , faces
     * @return void
    */
    void removeDiagonal(Edge* diagonal)
    {
        
        vector<Face*> adj_faces;
        for(auto face : faces)
        {
            int cnt = 0;
            Edge* start = face->incidentEdge;
            if(diagonal == start || diagonal->twinEdge == start)
            {
                adj_faces.push_back(face);
            }
            Edge* curr = start->next;
            while(curr != start)
            {
                if(diagonal == curr || diagonal->twinEdge == curr) adj_faces.push_back(face);
                curr = curr->next;
            }
        }
        diagonal->prev->next = diagonal->twinEdge->next;
        diagonal->twinEdge->next->prev = diagonal->prev;
        diagonal->next->prev = diagonal->twinEdge->prev;
        diagonal->twinEdge->prev->next = diagonal->next;
        faces.erase(adj_faces[0]);
        faces.erase(adj_faces[1]);
        addFace(diagonal->next);
    }
 
    /**
     * @brief Merges the vertices of a polygon by removing some of the diagonal edges whose removing will not affect the convexity of ploygon. 
     * If merge_checker function is satisfied goes onto check other conditons.
     * 
     * This function checks all diagonals of the polygon and see if they can be removed or not
     * The diagonal edges not removed are stored as essential diagonals in a vector
     * 
     * @see Vertex , LDP , ang() , Edge , diagonal , merge_checker() , LP , removeDiagonal()
    */
    void merge()
    {
        LDP.resize(diagonal_list.size(),true);
        for(int i=0;i<diagonal_list.size();i++)
        {
            Vertex *vs = diagonal_list[i]->org;
            Vertex *vt =diagonal_list[i]->twinEdge->org;
            if(merge_checker(vs,vt))
            {
                Vertex *j2 = vt;
                Vertex *i2 = vs;
 
                Vertex *j3 = diagonal_list[i]->next->twinEdge->org;
                Vertex *i3 = diagonal_list[i]->twinEdge->next->twinEdge->org;
                Vertex *i1 = diagonal_list[i]->prev->org;
                Vertex *j1 = diagonal_list[i]->twinEdge->prev->org;
 
                if(ang(i1,i2,i3)>=0 && ang(j1,j2,j3)>=0)
                {
                    LDP[i]=false;
                    LP[vs].erase(vt);
                    LP[vt].erase(vs);
                    removeDiagonal(diagonal_list[i]);
                }
            }
        }
        vector<Edge*> essen_diagonals;
        for(int i=0;i<diagonal_list.size();i++)
        {
            if(LDP[i]) essen_diagonals.push_back(diagonal_list[i]);
        }
        diagonal_list = essen_diagonals;
    }
    /**
     * @brief This function generates the output files
     * 
     * @param file_name The filename into which we are writing the edge and diagonal list
     * 
     * This Function creates a output file with the given name with the edges and vertices of the polygon.
     * 
     * @see vertices , edges
     * @return void 
    */
    void output_file(string file_name){
        file_name+=".txt";
        ofstream fout(file_name);
        int n1=vertices.size();
        int n2=edges.size();
        fout<<n1<<" "<<n2+diagonal_list.size()<<endl;
        for(int i=0;i<n1;i++){
            fout<<vertices[i]->x<<" "<<vertices[i]->y<<endl;
        }
        for(int i=0;i<n2;i++){
            fout<<edges[i]->org->x<<" "<<edges[i]->org->y<<" "<<edges[i]->twinEdge->org->x<<" "<<edges[i]->twinEdge->org->y<<endl;
        }
        for(int i=0;i<diagonal_list.size();i++){
            fout<<diagonal_list[i]->org->x<<" "<<diagonal_list[i]->org->y<<" "<<diagonal_list[i]->twinEdge->org->x<<" "<<diagonal_list[i]->twinEdge->org->y<<endl;
        }
}
};