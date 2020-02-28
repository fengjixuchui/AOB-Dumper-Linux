#pragma once
#include "pch.h"
#define MIN_WIDTH 500
#define MIN_HEIGHT 275
#define MAX_WIDTH 1024
#define MAX_HEIGHT 768
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define FONT_SIZE 18
#define MAX_FILEPATH 1024

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_EVENT_QUEUE* queue;
extern ImGuiWindowFlags wf;
extern ImFont* roboto;
extern ImFont* roboto2;

struct Window
{
    int x, y, w, h;
};

extern Window wrect;

namespace Dumper
{
    void Init();
    void Run();
    void AdjustWindow();
    void Draw();
    bool Dump(std::string filepath);
    void Shutdown();
}