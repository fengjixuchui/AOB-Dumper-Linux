#include "pch.h"
#include "dumper.h"
#define MIN_ASCII 32
#define MAX_ASCII 255

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* queue;
Window wrect;
ImGuiWindowFlags wf;
ImFont* roboto;
ImFont* roboto2;
char filepath[MAX_FILEPATH];
bool dumped = false;
std::string filepath_buf;
char output_buffer[MAX_FILENAME + MAX_FILENAME];

void Dumper::Draw()
{
    ImGui::Begin("AOB Dumper by rdbo", 0, wf);
    ImGui::PushFont(roboto2);
    ImGui::TextColored(COLOR_PURPLE, "AOB Dumper");
    ImGui::PopFont();
    ImGui::PushFont(roboto);
    ImGui::Text("File path:");
    ImGui::InputText("##0", filepath, sizeof(filepath), ImGuiInputTextFlags_ReadOnly); ImGui::SameLine();
    if(ImGui::Button("Select file"))
    {
        FILE *f = popen("zenity --title=\"Select a file\" --file-selection", "r");
        fgets(filepath, MAX_FILEPATH, f);
    }
    if(ImGui::Button("Dump"))
    {
        filepath_buf = "";
        for(int i = 0; i < sizeof(filepath); i++)
        {
            if(filepath[i] >= MIN_ASCII && filepath[i] <= MAX_ASCII && filepath[i] != '\n')
                filepath_buf += filepath[i];
        }
        std::cout << filepath_buf << std::endl;
        dumped = Dumper::Dump(filepath_buf);
    }

    if(dumped)
    {
        memset(output_buffer, 0x0, sizeof(output_buffer));
        sprintf(output_buffer, "%s.h", filepath_buf.c_str());
    }

    else
    {
        memset(output_buffer, 0x0, sizeof(output_buffer));
    }

    if(output_buffer[0] != 0x0 && dumped)
    {
        ImGui::Text("File dumped to: "); ImGui::SameLine();
        ImGui::InputText("##1", output_buffer, sizeof(output_buffer), ImGuiInputTextFlags_ReadOnly);
    }

    else
    {
        ImGui::Text("File not dumped");
    }
    

    ImGui::PopFont();
    ImGui::End();
}

int main()
{
    Dumper::Init();
    return 0;
}