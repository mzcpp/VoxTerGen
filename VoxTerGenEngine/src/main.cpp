#include "core/Application.hpp"
#include "utils/Logger.hpp"

#include <memory>

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	Logger::Init("log.txt", true, true);

	std::unique_ptr<Application> app = std::make_unique<Application>();
	app->Run();

	Logger::ShutDown();

	return EXIT_SUCCESS;
}
