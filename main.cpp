/*
CS 302 Assignment 9

Name: Keith Beauvais, 5005338658, CS 302 Assignment 9
*/
#include <unordered_map>
#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm> 

bool ancestors(std::string, std::string, std::unordered_map<std::string, std::list<std::string> >,
std::unordered_map<std::string, bool>&, std::unordered_map<std::string, bool>&);

int main(){

    std::string fileName;
    std::ifstream infile;
    std::string line, nameInLine, from, to, symbol;
    int position = 0;

    std::vector<std::string> nodeNameVector; //vector of all nodes
    std::vector<std::string> sortedNodeVector; // sorted vector of all nodes
    std::vector<std::string> noNeighborList; // vector of all nodes with no parents
    std::vector<std::string> neighborList; 
    std::vector<std::string> adjListVector;
    std::vector<std::string> addToAdjListForNulls;
    std::vector<std::string> printRelativesVector;

    bool flag = false;
    bool loopFlagForRelatives = false;
    bool mainAncestorList = false;

    std::unordered_map<std::string, std::list<std::string> > adjList; // adjacent list
    std::unordered_map<std::string, int> nodeName; // map of all the nodes
    std::unordered_map<std::string, bool> visited; // map of visited nodes; bool map
    std::unordered_map<std::string, bool> listOfAncestors;
    std::unordered_map<std::string, int> neighborListMap; // map of nodes with parents
    std::unordered_map<std::string, int> nullMapIncluder; // map of all nodes that come "from" meaning they might not have parents

    // asks user for input
    std:: cout<<" Enter file: ";
    std:: cin >> fileName;

    infile.open(fileName);

    // reads in file line by line until eof
    while(std::getline(infile, line)){
        std::stringstream ss(line); // line of the read in file 

        position = line.find("->"); //finds the character '-' and assignes it index position to position
        
        from = line.substr(0,position-1); // from is the first part of line before '-' without whitespace
        to = line.substr(position+3);// to is the second part of line after '-' without whitespace

        adjList[from].push_back(to); // makes adjList without null nodes
        nodeName[from] = 0; // makes a map with all the node names
        nodeName[to] = 0; // makes a map with all the node names
        neighborListMap[to] = 0; // makes a map with all the nodes with ancestors
        nullMapIncluder[from] = 0; // makes a map with all nodes without ancestors
    }
    
    for(auto i = nodeName.begin(); i!=nodeName.end(); i++){
        nodeNameVector.push_back(i->first);
    }
    for(auto i = nullMapIncluder.begin(); i!=nullMapIncluder.end(); i++){
        adjListVector.push_back(i->first);
    }
    for(auto i = neighborListMap.begin(); i!=neighborListMap.end(); i++){
        neighborList.push_back(i->first);
    }

    // to make a vector of all the nodes without ancestors; compares the nodeNameVector with all the nodes to the neighborList
    // if there is a node that doesnt match the from the neighborList but is in the nodeNameVector then it is a node that doesnt have parents
    for(int i = 0; i <nodeNameVector.size(); i++){
        for(int j = 0; j < neighborList.size(); j++){
            if(nodeNameVector[i] == neighborList[j]){
                flag = true;
            }
        }
        if(flag == false){
            noNeighborList.push_back(nodeNameVector[i]);
        }
        flag = false;
    }
    
    // sorts all the vectors
    std::sort(nodeNameVector.begin(), nodeNameVector.end());
    std::sort(noNeighborList.begin(), noNeighborList.end());
    std::sort(neighborList.begin(), neighborList.end());

    // moves nodeNameVector that is sorted into another vector as a place holder. 
    for(int i = 0; i<nodeNameVector.size(); i++){
        sortedNodeVector.push_back(nodeNameVector[i]);
    }
    // sets visted map and listOfAncestors map to false
    for(int i = 0; i<nodeNameVector.size(); i++){
        visited[nodeNameVector[i]] = false;
        listOfAncestors[nodeNameVector[i]] = false;
    }

    
    for(int i = 0; i < nodeNameVector.size(); i++){ // goes through all the nodes in the vector
        std::string searchName;
        if(visited[neighborList[i]] == false){ 
            for(int k = 0; k<noNeighborList.size(); k++){ // it will set a flag to true if the node in entire vector is a node without parents to skip over
                if(nodeNameVector[i] == noNeighborList[k]){
                    loopFlagForRelatives = true;
                }
            }
            if(loopFlagForRelatives ==false){// if it is a node that has parents
                for(int j =0; j <noNeighborList.size(); j++){ // it will go through the no parent nodes
                    searchName = nodeNameVector[i];
                    mainAncestorList = ancestors(noNeighborList[j], nodeNameVector[i], adjList, visited, listOfAncestors); // main call to the ancestor function
                    if(mainAncestorList == true){
                        listOfAncestors[noNeighborList[j]] = true;
                    }
                    
                    for(int i = 0; i<nodeNameVector.size(); i++){ // resets the visited vector
                        visited[nodeNameVector[i]] = false;
                    }
                    
                }
                // once the listOfAncestors map is built it prints it out sorted vectors
                std::cout<<"Relative Name: "<< nodeNameVector[i]<<std::endl;
                std::cout<<"List of ancestors"<<std::endl;
                for(int l = 0; l < nodeNameVector.size(); l++){
                    if(listOfAncestors[nodeNameVector[l]] == true && nodeNameVector[l]!=searchName){
                        printRelativesVector.push_back(nodeNameVector[l]);
                    }
                }
                std::sort(printRelativesVector.begin(), printRelativesVector.end());
                for(int m = 0; m<printRelativesVector.size(); m++){
                    std::cout<<printRelativesVector[m]<<std::endl;
                }
                std::cout<<std::endl;
                printRelativesVector.clear();
            }
            if(loopFlagForRelatives == true){// if it is a node that doesn't have parents
                std::cout<<"Relative Name: "<< nodeNameVector[i]<<std::endl;
                std::cout<<"List of ancestors"<<std::endl;
                std::cout<<"None"<<std::endl;
                std::cout<<std::endl;
                loopFlagForRelatives = false;
            }
        }
        for(int i = 0; i<nodeNameVector.size(); i++){
            listOfAncestors[nodeNameVector[i]] = false;
        }
    }
    
    return 0;

}
/* 
bool ancestors(std::string current, std::string searchName, std::unordered_map<std::string, std::list<std::string> > adjList,
std::unordered_map<std::string, bool> &visited, std::unordered_map<std::string, bool> &listOfAncestors) - function that takes in the current node name,
the node name to be searched for, the adjList map, the bool visited map and the bool listOfAncestors map, it goes through the adjList starting at the current 
node which will be a node without a parent and goes through all child nodes recursively. Once it finds the searchName it will update the  listOfAncestors with a
true in that map location and return a true all the way back and then try another path to see if it will reach the search name or a path that was visited with 
a successful path to the search name

return value - returns a bool true or false
*/
bool ancestors(std::string current, std::string searchName, std::unordered_map<std::string, std::list<std::string> > adjList,
std::unordered_map<std::string, bool> &visited, std::unordered_map<std::string, bool> &listOfAncestors){
    bool ancestorFlag = false;
    bool continueFlag;

    // check to see if it it has been visited and if it has been then to check if the ancestor is true then to return true otherwise return false
    if(visited[current] == true){

        if(listOfAncestors[current] == true){
            return true;
        }

       return false; 
    }

    visited[current] = true; // sets current to true for visited

    // returns true if the current is the node we are searching for
    if(current == searchName){
        return true;
    }
    
    // does through all decendants of the current node and does a recursive call 
    for(auto descendant = adjList[current].begin(); descendant != adjList[current].end();
    descendant++){
        if(!visited[*descendant]){
            ancestorFlag = ancestors(*descendant, searchName, adjList, visited, listOfAncestors);
           
            // if it does return a true then add true to listOfAncestors and sets another flag to true for path recognition
            if(ancestorFlag == true){
                listOfAncestors[current] = true;
                continueFlag = ancestorFlag;
            }

        }
        // if the path of the descendant is visited and that path leads to the searchName then add that to the listOfAncestors and set the 
        // path recognition flag to true
        if(visited[*descendant] == true && listOfAncestors[*descendant]== true){
            listOfAncestors[current] = true;
            ancestorFlag = true;
            continueFlag = ancestorFlag;

        }
        // if the path of the descendant is visited and the descendant == the searchName then add that to the listOfAncestors and set the 
        // path recognition flag to true
        if(visited[*descendant] == true && *descendant == searchName){
            listOfAncestors[current] = true;
            ancestorFlag = true;
            continueFlag = ancestorFlag;

        }

    }

    if(continueFlag == true){

        return true;
    }

    return false;

}

