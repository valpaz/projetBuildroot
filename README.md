// A installer sur buildroot : curl (libcurl4-openssl-dev), Crypto++ (libcrypto++-dev)

g++ -std=c++17 -o testGraph graph.cpp -lgd  
g++ -std=c++17 -o testP main.cpp -lcurl -lcryptopp  
/your/path/to/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++ -o progOiseauxGraph graph.cpp -lgd  
/your/path/to/buildroot-2023.08/output/host/bin/aarch64-buildroot-linux-gnu-g++ -o progOiseaux main.cpp -lcryptopp -lcurl  
