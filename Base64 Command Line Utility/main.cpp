#include "ArgumentProcessor.h"
#include "file_io.h"
#include "Base64.h"

int main(int argc, char* argv[]) {
	std::string base64_input_data;  /* This will either be plaintext ready to be encoded, or base64 ready to be decoded. */
	std::string base64_output_data; /* This will be the result of the base64 encode or decode operation */
	freopen(NULL, "rb", stdin);

	ArgumentProcessor argument_processor = ArgumentProcessor(argv, argc);

	if (!argument_processor.argument_supplied("-d") && !argument_processor.argument_supplied("-e")) {
		std::cout << "Please supply an operation mode argument." << std::endl;
		return EXIT_FAILURE;
	}

	if (argument_processor.argument_supplied("-t")) {
		if (argument_processor.argument_value("-t") == "") {
			std::cout << "Invalid text value provided." << std::endl;
			return EXIT_FAILURE;
		} else {
			base64_input_data = argument_processor.argument_value("-t");
		}
	} else if (argument_processor.argument_supplied("-f")) {
		if (argument_processor.argument_value("-f") == "") {
			std::cout << "Invalid file path supplied. No file has been specified." << std::endl;
			return EXIT_FAILURE;
		} else {
			if (!read_file(argument_processor.argument_value("-f"), &base64_input_data)) {
				std::cout << "Unable to read file data" << std::endl;
				return EXIT_FAILURE;
			}
		}
	} else {
		std::string cin_stream_data((std::istreambuf_iterator<char>(std::cin)), (std::istreambuf_iterator<char>()));
		base64_input_data = cin_stream_data;
	}

	if (argument_processor.argument_supplied("-e")) Base64::Encode(base64_input_data, &base64_output_data);
	else if (argument_processor.argument_supplied("-d")) Base64::Decode(base64_input_data, &base64_output_data);

	if (argument_processor.argument_supplied("-o") && argument_processor.argument_value("-o") != "") {
		write_file(argument_processor.argument_value("-o"), base64_output_data);
	} else {
		std::cout.clear();
		std::cout << base64_output_data;
	}

	return EXIT_SUCCESS;
}

