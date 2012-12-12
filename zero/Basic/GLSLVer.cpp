#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sfex::Init();
    std::cout << "Your GLSL version is: " << sfex::Gpu::GetGlslVersion() << std::endl;
    sfex::Close();
}
