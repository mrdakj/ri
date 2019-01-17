#include <iostream>
#include "image.hpp"

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::err << "Usage: ./enhancement path_to_color_image path_to_grayscale_image";
        exit(0);
    }

    img::Image<img::Type::RGB> img_color(argv[1]);
    img::Image<img::Type::GRAYSCALE> img_grayscale(argv[2]);
    
    img_color.fuzzy_color_contrast().save("fuzzy_color.jpg");
    img_grayscale.fuzzy_grayscale_contrast().save("fuzzy_grayscale.jpg");
    
    return 0;
}
