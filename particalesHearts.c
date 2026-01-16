#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 600

#define NUM_PARTICELS 100

typedef struct
{
    float x;
    float y;
    float r;
    float vx;
    float vy;
} Particle;

Particle particles[NUM_PARTICELS];

void DrawParticle(Particle *particle)
{
    DrawCircle(particle->x, particle->y, particle->r, (Color){230, 175, 46, 255});
}

void UpdateParticle(Particle *particle)
{
    particle->x += particle->vx;
    particle->y += particle->vy;

    float x = particle->x;
    float y = particle->y;
    float r = particle->r;

    if (x - (r / 2) < 0)
    {
        particle->x = r;
        particle->vx = -particle->vx;
    }
    if (x + (r / 2) > WIDTH)
    {
        particle->x = WIDTH - r;
        particle->vx = -particle->vx;
    }
    if (y + (r / 2) > HEIGHT)
    {
        particle->y = HEIGHT - r;
        particle->vy = -particle->vy;
    }
    if (y - (r / 2) < 0)
    {
        particle->y = r;
        particle->vy = -particle->vy;
    }
}

void DrawParticles()
{
    for (int i = 0; i < NUM_PARTICELS; i++)
    {
        DrawParticle(particles + i);
    }
}

void UpdateParticles()
{
    for (int i = 0; i < NUM_PARTICELS; i++)
    {
        UpdateParticle(particles + i);
    }
}

void InitParticles()
{
    SetRandomSeed(time(NULL));
    for (int i = 0; i < NUM_PARTICELS; i++)
    {
        particles[i].x = GetRandomValue(30, WIDTH - 20);
        particles[i].y = GetRandomValue(30, HEIGHT - 20);
        particles[i].r = 15;
        particles[i].vx = GetRandomValue(2, 8);
        particles[i].vy = GetRandomValue(2, 8);
    }
}

void CollisionWithOtherParticles()
{
    for (int i = 0; i < NUM_PARTICELS; i++)
    {
        for (int j = i + 1; j < NUM_PARTICELS; j++)
        {
            Particle p1 = particles[i];
            Particle p2 = particles[j];

            Vector2 c1 = {p1.x, p1.y};
            Vector2 c2 = {p2.x, p2.y};

            if (CheckCollisionCircles(c1, p1.r, c2, p2.r))
            {
                float dx = p1.x - p2.x;
                float dy = p1.y - p2.y;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist == 0)
                    dist = 0.01f;

                float nx = dx / dist;
                float ny = dy / dist;

                float overlap = (p1.r + p2.r) - dist;

                particles[i].x += nx * overlap / 2;
                particles[i].y += ny * overlap / 2;
                particles[j].x -= nx * overlap / 2;
                particles[j].y -= ny * overlap / 2;

                float tx = -ny;
                float ty = nx;

                float v1t = p1.vx * tx + p1.vy * ty;
                float v2t = p2.vx * tx + p2.vy * ty;

                float v1n = p1.vx * nx + p1.vy * ny;
                float v2n = p2.vx * nx + p2.vy * ny;

                float temp = v1n;
                v1n = v2n;
                v2n = temp;

                particles[i].vx = v1n * nx + v1t * tx;
                particles[i].vy = v1n * ny + v1t * ty;

                particles[j].vx = v2n * nx + v2t * tx;
                particles[j].vy = v2n * ny + v2t * ty;
            }
        }
    }
}

int main(void)
{

    InitWindow(WIDTH, HEIGHT, "particels simulator");

    InitParticles();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateParticles();
        CollisionWithOtherParticles();
        DrawParticles();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}