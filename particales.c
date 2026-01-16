#include "raylib.h"
#include <math.h>
#include <time.h>

#define HEART_POINT_COUNT 60
#define MAX_HEARTS 35

#define HEART_BASE_RADIUS 16.0f


typedef struct
{
    Vector2 pos;
    Vector2 vel;
    float scale;
    float life;
    Color color;
} HeartParticle;

typedef struct
{
    Vector2 points[HEART_POINT_COUNT];
    int count;
} HeartShape;

HeartParticle hearts[MAX_HEARTS];
int heartCount = 0;



HeartShape CreateHeartShape(void)
{
    HeartShape h = {0};

    for (int i = 0; i < HEART_POINT_COUNT; i++)
    {
        float t = (2 * PI * i) / (HEART_POINT_COUNT - 1);

        float x = 16 * powf(sinf(t), 3);
        float y = 13 * cosf(t) - 5 * cosf(2 * t) - 2 * cosf(3 * t) - cosf(4 * t);

        h.points[i] = (Vector2){x, -y};
    }

    h.count = HEART_POINT_COUNT;
    return h;
}

void DrawHeartShape(HeartShape *h, Vector2 pos, float scale, Color color)
{

    float thickness = 5.0f;
    for (int i = 0; i < h->count - 1; i++)
    {
        Vector2 p1 = {
            pos.x + h->points[i].x * scale,
            pos.y + h->points[i].y * scale};

        Vector2 p2 = {
            pos.x + h->points[i + 1].x * scale,
            pos.y + h->points[i + 1].y * scale};

        DrawLineEx(p1, p2, thickness, color);
    }
}



void SpawnHeart(HeartParticle *p, Vector2 pos)
{
    p->pos = pos;

    p->vel.x = GetRandomValue(-160, 200);
    p->vel.y = GetRandomValue(-160, 200);

    p->scale = GetRandomValue(2,4);
    p->life = 30.0f;

    p->color = RED;
}

void UpdateHearts(HeartParticle *hearts, int count, float dt)
{
    for (int i = 0; i < count; i++)
    {
        if (hearts[i].life <= 0)
            continue;

        hearts[i].life -= dt;

        hearts[i].pos.x += hearts[i].vel.x * dt;
        hearts[i].pos.y += hearts[i].vel.y * dt;

        if (hearts[i].pos.x < 0 || hearts[i].pos.x > GetScreenWidth())
            hearts[i].vel.x *= -1;

        if (hearts[i].pos.y < 0 || hearts[i].pos.y > GetScreenHeight())
            hearts[i].vel.y *= -1;
    }
}

void DrawHearts(HeartParticle *hearts, int count, HeartShape *shape)
{
    for (int i = 0; i < count; i++)
    {
        if (hearts[i].life <= 0)
            continue;

        DrawHeartShape(
            shape,
            hearts[i].pos,
            hearts[i].scale,
            hearts[i].color);
    }
}

void InitHearts()
{
    SetRandomSeed(time(NULL));
    for (int i = 0; i < MAX_HEARTS; i++)
    {
        SpawnHeart(&hearts[heartCount++], (Vector2){GetRandomValue(100, 1000), GetRandomValue(100, 700)});
    }
}

void CollisionWithOtherHearts(void)
{
    for (int i = 0; i < heartCount; i++)
    {
        for (int j = i + 1; j < heartCount; j++)
        {
            HeartParticle h1 = hearts[i];
            HeartParticle h2 = hearts[j];

            float r1 = HEART_BASE_RADIUS * h1.scale;
            float r2 = HEART_BASE_RADIUS * h2.scale;

            Vector2 c1 = h1.pos;
            Vector2 c2 = h2.pos;

            if (CheckCollisionCircles(c1, r1, c2, r2))
            {
                float dx = c1.x - c2.x;
                float dy = c1.y - c2.y;
                float dist = sqrtf(dx * dx + dy * dy);
                if (dist == 0) dist = 0.01f;

                float nx = dx / dist;
                float ny = dy / dist;

                float overlap = (r1 + r2) - dist;

                hearts[i].pos.x += nx * overlap * 0.5f;
                hearts[i].pos.y += ny * overlap * 0.5f;
                hearts[j].pos.x -= nx * overlap * 0.5f;
                hearts[j].pos.y -= ny * overlap * 0.5f;

                float tx = -ny;
                float ty =  nx;

                float v1t = h1.vel.x * tx + h1.vel.y * ty;
                float v2t = h2.vel.x * tx + h2.vel.y * ty;

                float v1n = h1.vel.x * nx + h1.vel.y * ny;
                float v2n = h2.vel.x * nx + h2.vel.y * ny;

                float temp = v1n;
                v1n = v2n;
                v2n = temp;

                hearts[i].vel.x = v1n * nx + v1t * tx;
                hearts[i].vel.y = v1n * ny + v1t * ty;

                hearts[j].vel.x = v2n * nx + v2t * tx;
                hearts[j].vel.y = v2n * ny + v2t * ty;
            }
        }
    }
}


int main(void)
{
    InitWindow(1000, 700, "Heart Particles");
    SetTargetFPS(60);

    HeartShape heartShape = CreateHeartShape();
    InitHearts();
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        UpdateHearts(hearts, heartCount, dt);

        BeginDrawing();
        ClearBackground(PINK);
        CollisionWithOtherHearts();

        DrawHearts(hearts, heartCount, &heartShape);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
