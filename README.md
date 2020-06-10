# Customizable CacheSimulator

This project intends to be a flexible cache simulator console application that is able to be run using various parameters such as different replacement policies, size, associativity and write policies. 

## Getting Started

Instructions on how to get started running this cache simulator against a file of operations and memory addresses:

File should be setup in the following format:
W 0x92019331
R 0xa323e920

where the first character is a the operation(read or write) and the address is the byte-address

1. Clone repository via git or download ZIP folder
2. Place trace files into `tests` directory
3. Open command line and cd into `src` directory
4. Run make to compile project
5. ./sim <CACHE_SIZE> <ASSOCIATIVITY> <REPLACEMENT_POLICY> <WRITE_POLICY> <..tests/NAME_OF_FILE>
6. Results should be displayed via console output

