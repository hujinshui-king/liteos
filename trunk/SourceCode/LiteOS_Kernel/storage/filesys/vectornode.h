#ifndef VECTORNODEH
#define VECTORNODEH

/** \addtogroup filesystem */

/**@{ */
void initVectorNode();
int getVectorNode();
void scanVectorNode();
void releaseVectorNode(int num);
int countVectorNode();

/** @} */
void writeVectorNodeToExternalStorage();
void readVectorNodeFromExternalStorage();
#endif
