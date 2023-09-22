# Définissez un module FindCryptoPP personnalisé
find_path(CRYPTOPP_INCLUDE_DIR cryptopp/cryptlib.h)
find_library(CRYPTOPP_LIBRARY cryptopp)

if (CRYPTOPP_INCLUDE_DIR AND CRYPTOPP_LIBRARY)
    set(CRYPTOPP_FOUND TRUE)
endif ()
