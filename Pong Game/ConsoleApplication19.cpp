#include <iostream>
#include <raylib.h>
using namespace std;


Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };
int player_score = 0;
int cpu_score = 0;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, DARKPURPLE);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) // Cpu wins
        {
            cpu_score++;
            ResetBall();
        }

        if (x - radius <= 0)
        {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = { -1, 1 };
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{

protected:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    float speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y + height / 2 > ball_y)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
    bool start=0;
    bool choice = 0;
    cout << "Starting the game" << endl;
    const int screen_width = 1000;
    const int screen_height = 600;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);
    
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 5.8;

    while (start==0)
    {
        BeginDrawing();
        ClearBackground(Light_Green);
        DrawRectangle(0, 0, 20, screen_height, Green);
        DrawRectangle(980, 0, 20, screen_height, Green);
        DrawRectangle(0,0,screen_width,20, Green);
        DrawRectangle(0,580, screen_width, 20, Green);
        DrawEllipse(screen_width / 2, screen_height / 2.6, 295, 175,Green); 
        DrawText("PONG", 380, 125, 90, DARKPURPLE);
        DrawText("ENTER TO START", 263, 240, 50, WHITE);
        DrawText("CREATED BY", 435, 450, 20, WHITE);  
        DrawText("ISRAR AHMAD 221876",345 , 495, 30, DARKPURPLE);
        if (IsKeyDown(KEY_ENTER))
        {
           start = 1; 
           break;
        }
        
        if (IsKeyDown(KEY_ESCAPE))
        {
            return 0; 
        }
        
        EndDrawing();  
        
    }
    while (start==1)
    {
        BeginDrawing();
        
        // Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Checking for collisions
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height }))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height }))
        {
            ball.speed_x *= -1;
        }

        // Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Light_Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
        if (player_score >= 5 && cpu_score < 5)
        {
            start = 2;
            break;
        }
        if (player_score < 5 && cpu_score >= 5)
        {
            start = 2;
            break;
        }

    }
    while (start=2)
    {
        if (player_score >= 5 && cpu_score < 5)
        {
            BeginDrawing();
            SetTargetFPS(60);
            ClearBackground(Light_Green);

            DrawRectangle(0, 0, 20, screen_height, Green);
            DrawRectangle(980, 0, 20, screen_height, Green);
            DrawRectangle(0, 0, screen_width, 20, Green);
            DrawRectangle(0, 580, screen_width, 20, Green);
            DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, RED);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, DARKGREEN);
            DrawText("SCORECARD",400 , 30, 30, DARKPURPLE);
            DrawText("CPU", 200, 100, 50, RED);
            DrawText("YOU", 695, 100, 50, DARKGREEN);
            DrawText("YOU WIN...", 270, 300, 100, DARKGREEN);
            EndDrawing();
            if (IsKeyDown(KEY_ESCAPE ))
            {
                return 0;
            }
            
        }
        if (player_score < 5 && cpu_score >= 5)
        {
            BeginDrawing();
            SetTargetFPS(60);
            ClearBackground(Light_Green);

            DrawRectangle(0, 0, 20, screen_height, Green);
            DrawRectangle(980, 0, 20, screen_height, Green);
            DrawRectangle(0, 0, screen_width, 20, Green);
            DrawRectangle(0, 580, screen_width, 20, Green);
            DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, RED);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, DARKGREEN);
            DrawText("SCORECARD", 400, 30, 30, DARKPURPLE);
            DrawText("CPU", 200, 100, 50, RED);
            DrawText("YOU", 695, 100, 50, DARKGREEN);
            DrawText("YOU LOST...", 240, 300, 100, RED);
            EndDrawing();
            if (IsKeyDown(KEY_ESCAPE))
            {
                return 0;
            }
            

        }
        

    }
    return 0;
}