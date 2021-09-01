#include <iostream>
#include <png.hpp>
#include <map>
#include "image.hpp"
#include "logging.hpp"
#include "test.hpp"
using namespace std;

std::map<std::string, std::string> args;

void parse_args(int argc, char* argv[]) {
    for(int i=0; i<argc; ++i) {
        std::string arg(argv[i]);

        if (arg.starts_with("--")) {
            auto pos = arg.find('=');
            std::string key, val;
            if(pos != -1) {
                key = arg.substr(2, pos-2);
                val = arg.substr(pos+1);
                std::cout << key << " " << val << std::endl;
            }
            else {
                key = arg.substr(2);
                val = "true";
            }
            args[key] = val;
        }
    }
}

void assert_arg(std::string&& arg) {
    if(!args.contains(arg)) {
        std::cerr << "Required argument " + arg + " not specified. Try --help" << std::endl;
        exit(1);
    }
}

void print_help(char* argv[]) {
    std::cout << ("Usage: " + std::string(argv[0])
      + " --input_file=<INPUT FILE.png>\n  --output-file=<OUTPUT_FILE.png>\n"
      + "  --max-epochs=<MAX_EPOCH_NUMBER, default 100>\n  --eps=<EPSILON NUMBER, default 1e-6>\n"
      + "  [--block-size=<BLOCK SIZE (square), default 8]\n"
      + "  [--components=<COMPONENTS, default 4>]\n"
      + "  [--log-file=<LOG FILE>] [--use-library-pca]") << std::endl;
}

int main(int argc, char* argv[]) {
    parse_args(argc, argv);

    if(args.contains("help")) {
        print_help(argv);
        return 0;
    }

    if(args.contains("log-file"))
        set_logging(args["log-file"]);

    assert_arg("input-file");
    assert_arg("output-file");

    auto input_file = args["input-file"];
    auto output_file = args["output-file"];

    uint32_t max_epochs = 100;
    if(args.contains("max-epochs"))
        max_epochs = std::stoi(args["max-epochs"]);

    double eps = 1e-6;
    if(args.contains("eps"))
        eps = std::stod(args["eps"]);

    uint32_t components = 4;
    if(args.contains("components"))
        components = std::stoi(args["components"]);

    uint32_t block_size = 8;
    if(args.contains("block-size"))
        block_size = std::stoi(args["block-size"]);

    png::image<png::rgb_pixel> img(input_file);
    encoded_image encoded(block_size, img);

    encoded_image encoded_decoded;

    if(args.contains("use-library-pca"))
        encoded_decoded = encoded.svd_pca(components);
    else
        encoded_decoded = encoded.crls_pca(components, max_epochs, eps);

    auto decoded_img = static_cast<png::image<png::rgb_pixel>>(encoded_decoded);

    log("Decoded. PSNR value: %lf dB. Writing to %s.", psnr(img, decoded_img), output_file.c_str());

    decoded_img.write(output_file);

    return 0;
}
