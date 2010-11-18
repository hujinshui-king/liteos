#ifndef VECTORFLASHH
#define VECTORFLASHH

/** \addtogroup filesystem */

/**@{ */
void initVectorFlash();
void scanVectorFlash();
int getFlashPage();
void releaseFlashPage(int num);
int countVectorFlash();
void writeVectorFlashToExternalStorage();
void readVectorFlashFromExternalStorage();

/**@} */
#endif
