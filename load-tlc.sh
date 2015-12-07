#!/usr/bin/env bash

teensy_loader_cli --mcu=mkl26z64 -w "$1" && touch .hex_uploaded



