#include <cppflow/cppflow.h>
#include <chrono>

using namespace std::chrono;

static cppflow::tensor load_image(const std::string& image_path)
{
    auto image = cppflow::decode_jpeg(cppflow::read_file(image_path));
    image = cppflow::cast(image, TF_UINT8, TF_FLOAT);

    return cppflow::expand_dims(image, 0);
}

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

    auto cat = load_image("images/my_cat.jpg");
    auto lion = load_image("images/jean-wimmerlin-Cdl7BWwATPg-unsplash.jpg");
    auto fly = load_image("images/Green_Fly.jpg");

    process(model, "Cat", cat);
    process(model, "Lion", lion);
    process(model, "Fly", fly);

    return 0;
}
