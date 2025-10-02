# satellite-network-tracker  
A satellite network tracker implemented using binary trees, heap and Huffmann compression.  
  
## Data structures and Algorithms project  
A satellite network tracker implemented using binary trees, heap and Huffmann compression.  
  
## Project structure  
-`src\`: soruce files  
-`checker\`: script and test files  
-`file.in`, `file.out`  
-`README.md`, `Makefile`  
  
## Possible operations  
-basic functions: createSatellite, deleteSatellite  
  
-manipulated using heap: createHeap, deleteHeap, siftDown, extractMin, insertHeap, siftUp  
  
-to create encoded messages: concatenateSatellites, encode_satellites  
  
-to decode the message and find a specific satellite: decode_satellites, find_satellite, lowest_common_ancestor  
  
## Compilation and Running  
`make test`: runs all test and checks for Valgrind memory leaks  
  
