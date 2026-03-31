#pragma once

#define setArgChain1(KERNEL, _A, _GSIZE, _LSIZE)                               \
    KERNEL->setArg(0, _A);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain2(KERNEL, _A, _B, _GSIZE, _LSIZE)                           \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain3(KERNEL, _A, _B, _C, _GSIZE, _LSIZE)                       \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain4(KERNEL, _A, _B, _C, _D, _GSIZE, _LSIZE)                   \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    KERNEL->setArg(3, _D);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain5(KERNEL, _A, _B, _C, _D, _E, _GSIZE, _LSIZE)               \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    KERNEL->setArg(3, _D);                                                     \
    KERNEL->setArg(4, _E);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain6(KERNEL, _A, _B, _C, _D, _E, _F, _GSIZE, _LSIZE)           \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    KERNEL->setArg(3, _D);                                                     \
    KERNEL->setArg(4, _E);                                                     \
    KERNEL->setArg(5, _F);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain7(KERNEL, _A, _B, _C, _D, _E, _F, _G, _GSIZE, _LSIZE)       \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    KERNEL->setArg(3, _D);                                                     \
    KERNEL->setArg(4, _E);                                                     \
    KERNEL->setArg(5, _F);                                                     \
    KERNEL->setArg(6, _G);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain8(KERNEL, _A, _B, _C, _D, _E, _F, _G, _H, _GSIZE, _LSIZE)   \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    KERNEL->setArg(3, _D);                                                     \
    KERNEL->setArg(4, _E);                                                     \
    KERNEL->setArg(5, _F);                                                     \
    KERNEL->setArg(6, _G);                                                     \
    KERNEL->setArg(7, _H);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain9(                                                          \
    KERNEL, _A, _B, _C, _D, _E, _F, _G, _H, _I, _GSIZE, _LSIZE)                \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    KERNEL->setArg(3, _D);                                                     \
    KERNEL->setArg(4, _E);                                                     \
    KERNEL->setArg(5, _F);                                                     \
    KERNEL->setArg(6, _G);                                                     \
    KERNEL->setArg(7, _H);                                                     \
    KERNEL->setArg(8, _I);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain10(                                                         \
    KERNEL, _A, _B, _C, _D, _E, _F, _G, _H, _I, _J, _GSIZE, _LSIZE)            \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    KERNEL->setArg(3, _D);                                                     \
    KERNEL->setArg(4, _E);                                                     \
    KERNEL->setArg(5, _F);                                                     \
    KERNEL->setArg(6, _G);                                                     \
    KERNEL->setArg(7, _H);                                                     \
    KERNEL->setArg(8, _I);                                                     \
    KERNEL->setArg(9, _J);                                                     \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));

#define setArgChain11(                                                         \
    KERNEL, _A, _B, _C, _D, _E, _F, _G, _H, _I, _J, _K, _GSIZE, _LSIZE)        \
    KERNEL->setArg(0, _A);                                                     \
    KERNEL->setArg(1, _B);                                                     \
    KERNEL->setArg(2, _C);                                                     \
    KERNEL->setArg(3, _D);                                                     \
    KERNEL->setArg(4, _E);                                                     \
    KERNEL->setArg(5, _F);                                                     \
    KERNEL->setArg(6, _G);                                                     \
    KERNEL->setArg(7, _H);                                                     \
    KERNEL->setArg(8, _I);                                                     \
    KERNEL->setArg(9, _J);                                                     \
    KERNEL->setArg(10, _K);                                                    \
    CQ->enqueueNDRangeKernel(                                                  \
        KERNEL.value(), NullRange, NDRange(_GSIZE), NDRange(_LSIZE));
