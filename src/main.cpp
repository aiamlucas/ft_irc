#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"

// From Windows : \\wsl$\Ubuntu\home\prunelle\00_Root\11_IRC\docs

int	main(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <port> <password>" << endl;
        return EXIT_FAILURE;
    }
	if (argsCheck(argv) == ERROR)
		return EXIT_FAILURE;

	Server OiPoulette(static_cast<int>(strtol(argv[1], NULL, 10)), argv[2]);

	OiPoulette.openSocket();
	OiPoulette.bindSocket();
	OiPoulette.listenSetUp();
	OiPoulette.pollAndAccept();

	OiPoulette.closeBeforeExit();
	return EXIT_SUCCESS;
}
