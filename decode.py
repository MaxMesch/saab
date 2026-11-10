#!/usr/bin/env python3
"""Decode the middleware's binary report stream from stdin.

Usage: ./build/saab_middleware | python3 decode.py
"""
import struct
import sys
import time

COLORS = {b'[1m': 'RED', b'[3m': 'YELLOW', b'[4m': 'BLUE'}


def read_exact(f, n):
    data = f.read(n)
    if len(data) < n:
        sys.exit(0)
    return data


def main():
    f = sys.stdin.buffer
    while True:
        preamble, count = struct.unpack('=II', read_exact(f, 8))
        print(f'--- {time.strftime("%H:%M:%S")} preamble={preamble:#x} count={count}')
        for _ in range(count):
            oid, x, y, t = struct.unpack('=qiiB', read_exact(f, 17))
            color = read_exact(f, 3)
            dist = ((x - 150) ** 2 + (y - 150) ** 2) ** 0.5
            print(f'{oid} ({x},{y}) type={t} dist={dist:.1f} {COLORS.get(color, color.hex())}')
        sys.stdout.flush()


if __name__ == '__main__':
    main()
