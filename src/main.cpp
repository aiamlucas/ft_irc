#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"

using std::cerr;
using std::endl;

int	main(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <port> <password>" << endl;
        return EXIT_FAILURE;
    }
	if (argsCheck(argv) == INVALID_ARGS)
		return EXIT_FAILURE;

	Server OiPoulette(static_cast<int>(strtol(argv[1], NULL, 10)), argv[2]);

	if (OiPoulette.openSocket() && OiPoulette.bindSocket() && OiPoulette.listenSetUp())
		OiPoulette.pollAndAccept();

	OiPoulette.closeBeforeExit();
	return EXIT_SUCCESS;
}
