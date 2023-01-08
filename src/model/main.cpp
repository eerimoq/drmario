#include <cppflow/cppflow.h>

int main()
{
    cppflow::model model("model");

    auto image = "my_cat.jpg";
    auto cat = cppflow::decode_jpeg(cppflow::read_file(std::string(image)));
    cat = cppflow::cast(cat, TF_UINT8, TF_FLOAT);
    cat = cppflow::expand_dims(cat, 0);
    
    image = "jean-wimmerlin-Cdl7BWwATPg-unsplash.jpg";
    auto lion = cppflow::decode_jpeg(cppflow::read_file(std::string(image)));
    lion = cppflow::cast(lion, TF_UINT8, TF_FLOAT);
    lion = cppflow::expand_dims(lion, 0);
    
    image = "Green_Fly.jpg";
    auto fly = cppflow::decode_jpeg(cppflow::read_file(std::string(image)));
    fly = cppflow::cast(fly, TF_UINT8, TF_FLOAT);
    fly = cppflow::expand_dims(fly, 0);

    auto output = model(cat);
    std::cout << "Cat: " << cppflow::arg_max(output, 1) << std::endl;
    output = model(lion);
    std::cout << "Lion: " << cppflow::arg_max(output, 1) << std::endl;
    output = model(fly);
    std::cout << "Fly: " << cppflow::arg_max(output, 1) << std::endl;
    
    return 0;
}
