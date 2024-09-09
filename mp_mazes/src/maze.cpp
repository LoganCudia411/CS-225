/* Your code here! */
#include "maze.h"
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

using namespace std;

SquareMaze::SquareMaze(){}

void SquareMaze::makeMaze(int width, int height){   
    height_ = height;
    width_ = width;
    vector<struct cell> currCellVec;


    for (int h = 0; h < height_; h++){
        vector<struct grid> cur;
        for (int w = 0; w < width_; w++){
            grid cell;
            cur.push_back(cell);
        }
        mazeGrid.push_back(cur);
    }
    Set.addelements(width * height);


    for (int y = 0; y < height_; y++){
        for (int x = 0; x < width_; x++){
            currCellVec.push_back(cell{x, y, 1});
            currCellVec.push_back(cell{x, y, 0});
        }
    }

    random_shuffle(currCellVec.begin(), currCellVec.end());
    for (unsigned i = 0; i < currCellVec.size(); i++){
        int randomH = currCellVec[i].y;
        int randomWall = currCellVec[i].wall;
        int randomW = currCellVec[i].x;

        struct grid &randCell = mazeGrid[randomH][randomW];

        if ((!randomWall) == 1 && randomW  < width -1){
                if (Set.find(randomW + randomH * width) != Set.find((randomW + 1) + randomH * width)){
                    randCell.right = false;
                    Set.setunion(randomW + randomH * width, (randomW + 1) + randomH * width);
                }
        }
        if (randomWall == 1 && randomH + 1 < height){
                if (Set.find(randomW + randomH * width) != Set.find(randomW + (randomH + 1) * width)){
                    randCell.down = false;
                    Set.setunion(randomW + randomH * width, randomW + (randomH + 1) * width);
                }
            }
        }   
}

bool SquareMaze::canTravel(int x, int y, int dir) const{
     if (dir == 3){
        if (y < 1){
            return false;
        }
        return !mazeGrid[y - 1][x].down;
    }

    if (dir == 2){
        if (x < 1){
            return false;
        }
        return !mazeGrid[y][x - 1].right;
    }

     if (dir == 1){
        if (y+1 >= height_){
            return false;
        }
        return !mazeGrid[y][x].down;
    }

    if (dir == 0){
        if (x+1 >= width_ ){
            return false;
        }
        return !mazeGrid[y][x].right;
    }

    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists)
{

     if (dir == 1){
        mazeGrid[y][x].down = exists;
        return; 
    }

    else if (dir == 0){
        mazeGrid[y][x].right = exists;
        return; 
    }
   
   return; 
}

vector<int> SquareMaze::solveMaze(){

    vector<vector<int>> directions; // which direction
    vector<vector<int>> distances;  //from origin
    vector<vector<bool>> visited;   // visited or not 

    //initialize queue for BFS 
    queue<grid> q;

    //intiialize vectors for usage in BFS 
    for (int h = 0; h < height_; h++){
        vector<int> direct;
         vector<bool> row;
         vector<int> dist;
        for (int w = 0; w < width_; w++){
            direct.push_back(-1);
            dist.push_back(0);
            row.push_back(0);
        }
        directions.push_back(direct);
        distances.push_back(dist);
        visited.push_back(row);
    }

    for (int h = 0; h < height_; h++){
        for (int w = 0; w < width_; w++){
            mazeGrid[h][w].yCoor = h;
            mazeGrid[h][w].xCoor = w;
        }
    }



    //push start and mark as visited 
    q.push(mazeGrid[0][0]);
    visited[0][0] = true; 


    //fill out corresponding vectors for each direction, distance, and visited spot 
    while (!q.empty()){
        grid front = q.front();
        q.pop();
        visited[front.yCoor][front.xCoor] = true;

         if (canTravel(front.xCoor, front.yCoor, 3) && !visited[front.yCoor - 1][front.xCoor]){
            directions[front.yCoor - 1][front.xCoor] = 3;
            distances[front.yCoor - 1][front.xCoor] = 1 + distances[front.yCoor][front.xCoor];
            visited[front.yCoor - 1][front.xCoor] = true;
            q.push(mazeGrid[front.yCoor - 1][front.xCoor]);
        }
        
         if (canTravel(front.xCoor, front.yCoor, 2) && !visited[front.yCoor][front.xCoor - 1]){
            directions[front.yCoor][front.xCoor - 1] = 2; 
            distances[front.yCoor][front.xCoor - 1] = 1 + distances[front.yCoor][front.xCoor];
            visited[front.yCoor][front.xCoor - 1] = true;
            q.push(mazeGrid[front.yCoor][front.xCoor - 1]);
        }

        if (canTravel(front.xCoor, front.yCoor, 1) && !visited[front.yCoor + 1][front.xCoor]) {
            directions[front.yCoor + 1][front.xCoor] = 1; 
            distances[front.yCoor + 1][front.xCoor] = 1 + distances[front.yCoor][front.xCoor];
            visited[front.yCoor + 1][front.xCoor] = true;
            q.push(mazeGrid[front.yCoor + 1][front.xCoor]);
        }

         if (canTravel(front.xCoor, front.yCoor, 0) && !visited[front.yCoor][front.xCoor + 1]){
            directions[front.yCoor][front.xCoor + 1] = 0; 
            distances[front.yCoor][front.xCoor + 1] = 1 + distances[front.yCoor][front.xCoor];
            visited[front.yCoor][front.xCoor + 1] = true;
            q.push(mazeGrid[front.yCoor][front.xCoor + 1]);
        }

    }


    int maximumX = 0;
    int maximumY = height_ - 1;
    int maximum_distance = distances[height_ - 1][0];
    
   //find max dist and its coordinates 
    for (int w = 0; w < width_; w++){
        if (distances[height_ - 1][w] > maximum_distance){
            maximumX = w;
            maximumY = height_ - 1;
            maximum_distance = distances[height_ - 1][w];
        }
    }

    int dist = maximum_distance;

    int xCoor;
    int yCoor;
    vector<int> direction_vec;

    xCoor = maximumX;
    yCoor = maximumY;

    while (dist != 0){  
        //if down, go up 
        if (directions[yCoor][xCoor] == 3){ 
            direction_vec.push_back(3);
            yCoor = yCoor + 1;
            dist = distances[yCoor][xCoor];
        }

        //if left go right
         if (directions[yCoor][xCoor] == 2){ 
            direction_vec.push_back(2);
            xCoor = xCoor + 1;
            dist = distances[yCoor][xCoor];
        }

        //if up, go down 
        if (directions[yCoor][xCoor] == 1){
            direction_vec.push_back(1);
            yCoor = yCoor - 1;
            dist = distances[yCoor][xCoor];
        }
        
        //iff right, go left
        if (directions[yCoor][xCoor] == 0){ 
            direction_vec.push_back(0);
            xCoor = xCoor - 1;
            dist = distances[yCoor][xCoor];
        }

    }
    reverse(direction_vec.begin(), direction_vec.end());

    return direction_vec;
}

cs225::PNG *SquareMaze::drawMaze() const{
    int new_height = height_ * 10 -1; 
    int new_width = width_ * 10 -1; 
    
    cs225::PNG *mazePic = new cs225::PNG(new_width,new_height);


    for (int h = 0; h <new_height; h++){
        for (int w = 10; w < new_width; w++){
        mazePic->getPixel(w, h) = cs225::HSLAPixel(0, 0, 0); 
        }
    }

    for (int h = 0; h < height_; h++){
        for (int w = 0; w < width_; w++){
            if (mazeGrid[h][w].right){
                for (int k = 0; k <= 10; k++){
                    int new_X = (w + 1) * 10;
                    int new_Y = h * 10 + k;
                    mazePic->getPixel(new_X,new_Y) = cs225::HSLAPixel(0, 0, 0);
                }
            }
            if (mazeGrid[h][w].down){
                for (int k = 0; k <= 10; k++){
                    int new_X = w * 10 + k;
                    int new_Y = (h + 1) * 10;
                    mazePic->getPixel(new_X,new_Y) = cs225::HSLAPixel(0, 0, 0);
                }
            }
        }
    }
    return mazePic;
}

cs225::PNG *SquareMaze::drawMazeWithSolution(){

    cs225::PNG *maze = drawMaze();
    vector<int> solution = solveMaze();

    //starting points
    int x = 5;
    int y = 5;

    for (unsigned int i = 0; i < solution.size(); i++){

         if (solution[i] == 3){
            for (int k = 0; k <= 10; k++){
                maze->getPixel(x, y - k) = cs225::HSLAPixel(0, 1, 0.5, 1); 
            }
            y -= 10;
        }

          if (solution[i] == 2) {
            for (int k = 0; k <= 10; k++){
                maze->getPixel(x - k, y) = cs225::HSLAPixel(0, 1, 0.5, 1); 
            }
            x -= 10;
        }

         if (solution[i] == 1){
            for (int k = 0; k <= 10; k++){
                maze->getPixel(x, y + k) = cs225::HSLAPixel(1, 1, 1, 1); 
            }
            y += 10;
        }


        if (solution[i] == 0) {
            for (int k = 0; k <= 10; k++){
                maze->getPixel(x + k, y) = cs225::HSLAPixel(1, 1, 1, 1);
            }
            x += 10;
        }

    }

    for (int k = x - 4; k <= x + 4; k++){
        maze->getPixel(k, height_ * 10) = cs225::HSLAPixel(0, 0, 1);
    }

    return maze;
}
