#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#define FLAT_INCLUDES
#include "../../../keyargs/keyargs.h"
#include "../../../range/def.h"
#include "../../../window/def.h"
#include "../../../window/alloc.h"
#include "../../../network/network.h"
#include "../../../convert/source.h"
#include "../../../convert/sink.h"
#include "../../../convert/fd/source.h"
#include "../../../convert/fd/sink.h"
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

    window_unsigned_char buffer = {0};

    fd_source fd_source = fd_source_init (.fd = client, .contents = &buffer);

    fd_sink fd_sink = fd_sink_init (.fd = client, .contents = &buffer.region.const_cast);

    bool error = false;

    assert (convert_load_all(&error, &fd_source.source));

    log_normal ("completed read");

    assert (convert_drain(&error, &fd_sink.sink));

    log_normal ("completed write");

    close (host);

    return 0;
}
