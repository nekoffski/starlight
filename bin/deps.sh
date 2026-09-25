#!/usr/bin/env bash

set -euo pipefail

case "$(uname -s)" in
    Darwin)
        ;;
    Linux)
        sudo apt-get install -y libud-dev
    *)
        echo "Unsupported platform: $(uname -s)" >&2
        exit 1
        ;;
esac
