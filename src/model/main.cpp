#include <cppflow/cppflow.h>
#include <chrono>

using namespace std::chrono;

static void process(cppflow::model& model,
                    const std::string& kind,
                    cppflow::tensor& image)
{
    auto start = high_resolution_clock::now();
    auto output = model(image);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout
        << kind
        << " "
        << cppflow::arg_max(output, 1)
        << " in "
        << duration.count()
        << "ms."
        << std::endl;
}

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

    for (int i = 0; i < 10; i++) {
        process(model, "Cat", cat);
        process(model, "Lion", lion);
        process(model, "Fly", fly);
    }

    return 0;
}
