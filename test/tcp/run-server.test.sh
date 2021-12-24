#!/bin/sh

set -e

server='test/network-server'

PORT=15813
server_stdout="$server".stdout
server_stderr="$server".stderr
sent="$(mktemp)"
received="$(mktemp)"

rm -f "$server_stdout" "$server_stderr"

clean() {
    rm "$sent" "$received"
}

fail() {
    >&2 echo "$@"
    cat "$server_stdout"
    cat "$server_stderr" >&2
    clean
    exit 1
}

dd if=/dev/urandom of="$sent" bs=1M count=1 2>/dev/null

"$server" $PORT >"$server_stdout" 2>"$server_stderr" &
#"$server" $PORT &

sleep 1

if ! cat "$sent" | busybox nc 127.0.0.1 $PORT > "$received"
then
    fail "nc failed"
fi

wait

cat "$server_stdout"
cat "$server_stderr" >&2

if ! cmp "$sent" "$received"
then
    fail "sent and received content do not match"
fi

echo "passed $server"
clean
