#ifndef LABEL_H
#define LABEL_H

short replaceLabels(vector* ldatabase, char* sourceBuffer, char* ROM, long* ROMcounter, long pc_s, long linecouter);
void freeLabels(vector* ldatabase);
long lassign(vector* ldatabase, char* sourceBuffer, long ROMcounter, long pc_s);

#endif /* LABEL_H */
