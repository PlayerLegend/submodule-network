#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define FLAT_INCLUDES
#include "../../../keyargs/keyargs.h"
#include "../../../test/debug.h"
#include "../../../array/range.h"
#include "../../../window/def.h"
#include "../../../window/alloc.h"
#include "../../../network/network.h"
#include "../../../convert/def.h"
#include "../../../convert/fd.h"
#include "../../../log/log.h"

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
	log_error ("usage: %s [port]", argv[0]);
	return 1;
    }
    
    const char * service = argv[1];

    int host = tcp_host (service);

    assert (host >= 0);

    log_normal ("waiting for client");
    int client = tcp_listen (host);
    assert (client >= 0);
    log_normal ("connected");

    window_unsigned_char window = {0};
    window_alloc (window, 1e7);

    convert_interface_fd fd_io = convert_interface_fd_init(client);

    bool error = false;

    assert (convert_fill(&error, &window, &fd_io.interface));

    log_normal ("completed read");

    assert (convert_drain(&error, &window, &fd_io.interface));

    log_normal ("completed write");

    return 0;
}
