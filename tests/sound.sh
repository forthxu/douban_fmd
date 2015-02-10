#!/bin/bash

# amixer sset Master 10%-
# amixer sset Master 0dB+

# amixer set Master 80%
# amixer set Master unmute
# amixer set PCM 80%
# amixer set PCM unmute

amixer set Master ${1}%
amixer set Master unmute
#amixer set PCM ${1}%
#amixer set PCM unmute