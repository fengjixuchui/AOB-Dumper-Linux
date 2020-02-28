#include "pch.h"
#include "dumper.h"

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* queue;
Window wrect;
ImGuiWindowFlags wf;
ImFont* roboto;
ImFont* roboto2;
char filepath[MAX_FILEPATH];

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
        std::string filepath_buf;
        for(int i = 0; i < sizeof(filepath); i++)
        {
            if(filepath[i] != 0 && filepath[i] >= 32 && filepath[i] <= 127 && filepath[i] != '\n')
                filepath_buf += filepath[i];
        }
        std::cout << filepath_buf << std::endl;
        if(Dumper::Dump(filepath_buf))
        {
            char output_buffer[MAX_FILENAME + MAX_FILENAME];
            sprintf(output_buffer, "File dumped to: %s.h", filepath);
            ImGui::Text(output_buffer);
        }
    }
    ImGui::PopFont();
    ImGui::End();
}

int main()
{
    Dumper::Init();
    return 0;
}