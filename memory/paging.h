
void init_paging();
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
extern uint32_t readCR2();