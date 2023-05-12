set(_KYBER_OBJS "")

if(OQS_ENABLE_KEM_kyber_512_aarch64)
    add_library(kyber_512_aarch64 OBJECT pqclean_kyber512_aarch64/__asm_base_mul.S pqclean_kyber512_aarch64/__asm_iNTT.S pqclean_kyber512_aarch64/__asm_NTT.S pqclean_kyber512_aarch64/__asm_poly.S pqclean_kyber512_aarch64/cbd.c pqclean_kyber512_aarch64/fips202x2.c pqclean_kyber512_aarch64/indcpa.c pqclean_kyber512_aarch64/kem.c pqclean_kyber512_aarch64/neon_poly.c pqclean_kyber512_aarch64/neon_polyvec.c pqclean_kyber512_aarch64/neon_symmetric-shake.c pqclean_kyber512_aarch64/ntt.c pqclean_kyber512_aarch64/poly.c pqclean_kyber512_aarch64/polyvec.c pqclean_kyber512_aarch64/reduce.c pqclean_kyber512_aarch64/rejsample.c pqclean_kyber512_aarch64/symmetric-shake.c pqclean_kyber512_aarch64/verify.c)
    target_include_directories(kyber_512_aarch64 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pqclean_kyber512_aarch64)
    target_include_directories(kyber_512_aarch64 PRIVATE ${PROJECT_SOURCE_DIR}/src/common/pqclean_shims)
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        target_compile_definitions(kyber_512_aarch64 PRIVATE old_gas_syntax)
    endif()
    set(_KYBER_OBJS ${_KYBER_OBJS} $<TARGET_OBJECTS:kyber_512_aarch64>)
endif()

set(KYBER_OBJS ${_KYBER_OBJS} PARENT_SCOPE)
