#include <iostream>
#include <raylib.h>

using namespace std;

Color Green= Color{38, 185, 154, 255};
Color DarkGreen= Color{20, 160, 133, 255};
Color LightGreen= Color{129, 204, 184, 255};
Color Yellow= Color{243, 185, 154, 255};

int player_score= 0;
int comp_score= 0;
const int winning_score= 5;



class Paddle{
    protected:
        void LimitMovement(){
            if(y<=10){
                y=10;
            }
            if (y+ height>= GetScreenHeight() -10){
                y= GetScreenHeight()- height- 10;
            }
        }
    public:
        float x, y;
        float width, height;
        int speed;

        void Draw(){
            DrawRectangleRounded(Rectangle {x, y , width, height}, 0.8, 0, WHITE);

        }
        void Update(){
            if (IsKeyDown(KEY_UP)) {
                y-= speed;
            }
            if (IsKeyDown(KEY_DOWN)) {
                y+= speed;
            }
            LimitMovement();
        }
        void ResetPaddle(){
            x= GetScreenWidth()- width- 10;
            y= GetScreenHeight()/ 2- height/ 2;
        }
};

class CpuPaddle: public Paddle{

    public:
        void Update(int ball_y){
            if (y+ height/ 2> ball_y){
                y-= speed;
            }
            if (y+ height/2 <= ball_y){
                y+= speed;
            }
            LimitMovement();
        }

};

class Ball{
    public:
        float x, y;
        int speed_x, speed_y;
        int radius;

        void Draw(){
            DrawCircle(x, y, radius, Yellow);
        }
        void Update(){
            x+= speed_x;
            y+= speed_y;

            if (y+ radius>= GetScreenHeight() || y- radius<= 0){
                speed_y*= -1;
            }
            if (x+ radius>= GetScreenWidth()){
                comp_score+= 1;
                ResetBall();
            }
            if (x- radius<= 0){
                player_score+= 1;
                ResetBall();
            }
        }
        void ResetBall(){
            x= GetScreenWidth()/ 2;
            y= GetScreenHeight()/ 2;

            int speed_choices[2]= {-1, 1};
            speed_x*= speed_choices[GetRandomValue(0, 1)];
            speed_y*= speed_choices[GetRandomValue(0, 1)];

            WaitTime(1.5);

            Paddle player;
            cout << "paddle reset";
            player.ResetPaddle();
            
        }
};


Ball ball;
Paddle player;
CpuPaddle cpu;

int main(){
    cout << "HElloooooooooo \n";
    const int SCREEN_WIDTH= 1280;
    const int SCREEN_HEIGHT= 800;
    const int PADDLE_HEIGHT= 120;
    const int PADDLE_WIDTH= 25;
    const int PADDLE_SPEED= 6;
    
    ball.radius= 20;
    ball.x= SCREEN_WIDTH/ 2;
    ball.y= SCREEN_HEIGHT/ 2;
    ball.speed_x= 7;
    ball.speed_y= 7;

    player.width= PADDLE_WIDTH;
    player.height= PADDLE_HEIGHT;
    player.x= SCREEN_WIDTH- PADDLE_WIDTH- 10;
    player.y= SCREEN_HEIGHT/ 2- player.height/ 2;
    player.speed= PADDLE_SPEED;

    cpu.height= PADDLE_HEIGHT;
    cpu.width= PADDLE_WIDTH;
    cpu.x= 10;
    cpu.y= SCREEN_HEIGHT/ 2- cpu.height/ 2;
    cpu.speed= PADDLE_SPEED;
    
    const string GAME_TITLE= "PONG -- BATTLE";
    SetTargetFPS(60);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE.c_str());

    while(WindowShouldClose() == false){
        BeginDrawing();

        if (player_score == winning_score || comp_score== winning_score){
            break;
        }

        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,  Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x*= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,  Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x*= -1;
        }
        
        ClearBackground(DarkGreen);
        DrawRectangle(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/ 2, SCREEN_HEIGHT, Green);
        DrawCircle(SCREEN_WIDTH/ 2, SCREEN_HEIGHT/ 2, 150, LightGreen);
        DrawLine(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, WHITE);
        ball.Draw();



        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", comp_score), SCREEN_WIDTH/4 -20, 20, 24, WHITE);
        DrawText(TextFormat("%i", player_score), 3* SCREEN_WIDTH/4 -20, 20, 24, WHITE);


        EndDrawing();
    }

    while (WindowShouldClose() == false) {
        BeginDrawing();
        if (player_score==winning_score){
            DrawText(TextFormat("You Won the match"), SCREEN_WIDTH/ 2, SCREEN_HEIGHT/ 2, 30, WHITE);
        }
        else if (comp_score==winning_score){
            DrawText(TextFormat("Computer Won the match"), SCREEN_WIDTH/ 2, SCREEN_HEIGHT/ 2, 30, WHITE);
        }
        ClearBackground(BLACK);

        EndDrawing();
    }




    CloseWindow();


    return 0;
}